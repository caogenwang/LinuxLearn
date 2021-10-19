## 前言

在 golang 中有一个池，它特别神奇，你只要和它有个约定，你要什么它就给什么，你用完了还可以还回去，但是下次拿的时候呢，确不一定是你上次存的那个，这个池就是 sync.Pool

说实话第一次看到这个东西的时候，真的想不到这个东西有啥用啊，为什么要有这个东西呢？等我看完之后，嗯，还有有点用的；等到有一次优化经历的时候，嗯，这个有点意思了。今天我们就来看看这个神奇的 sync.Pool

## 简单案例

首先我们来看看这个 sync.Pool 是如何使用的，其实非常的简单。
 它一共只有三个方法我们需要知道的：New、Put、Get

```go
package main

import (
    "fmt"
    "sync"
)

var strPool = sync.Pool{
    New: func() interface{} {
        return "test str"
    },
}

func main() {
    str := strPool.Get()
    fmt.Println(str)
    strPool.Put(str)
}
```

通过`New`去定义你这个池子里面放的究竟是什么东西，在这个池子里面你只能放一种类型的东西。比如在上面的例子中我就在池子里面放了字符串。

我们随时可以通过`Get`方法从池子里面获取我们之前在New里面定义类型的数据。

当我们用完了之后可以通过`Put`方法放回去，或者放别的同类型的数据进去。

## 目的

那么这个池子的目的是什么呢？其实一句话就可以说明白，就是为了复用已经使用过的对象，来达到优化内存使用和回收的目的。说白了，一开始这个池子会初始化一些对象供你使用，如果不够了呢，自己会通过new产生一些，当你放回去了之后这些对象会被别人进行复用，当对象特别大并且使用非常频繁的时候可以大大的减少对象的创建和回收的时间。

- 临时对象

- 自动移除

- 当这个对象的引用只有sync.Pool持有时，这个对象内存会被释放

- 多线程安全

- 目的就是缓存并重用对象，减少GC的压力

- 自动扩容、缩容

- 不要去拷贝pool，也就是说最好单例

  ## 源码分析

  下面我们从源码层面来看看这个 sync.Pool；可能需要你有GPM模型和GC的相关知识。
  使用golang版本： go version go1.13

  ```go
  type Pool struct {
      noCopy noCopy
  
      local     unsafe.Pointer // local fixed-size per-P pool, actual type is [P]poolLocal
      localSize uintptr        // size of the local array
  
      victim     unsafe.Pointer // local from previous cycle
      victimSize uintptr        // size of victims array
  
      // New optionally specifies a function to generate
      // a value when Get would otherwise return nil.
      // It may not be changed concurrently with calls to Get.
      New func() interface{}
  }
  
  // Local per-P Pool appendix.
  type poolLocalInternal struct {
      private interface{} // Can be used only by the respective P.
      shared  poolChain   // Local P can pushHead/popHead; any P can popTail.
  }
  
  type poolLocal struct {
      poolLocalInternal
  
      // Prevents false sharing on widespread platforms with
      // 128 mod (cache line size) = 0 .
      pad [128 - unsafe.Sizeof(poolLocalInternal{})%128]byte
  }
  ```

  我们可以看到其实结构并不复杂，但是如果自己看的话有点懵。注意几个细节就ok。

  - local这里面真正的是[P]poolLocal其中P就是GPM模型中的P，有多少个P数组就有多大，也就是每个P维护了一个本地的poolLocal。
  - poolLocal里面维护了一个private一个shared，看名字其实就很明显了，private是给自己用的，而shared的是一个队列，可以给别人用的。注释写的也很清楚，自己可以从队列的头部存然后从头部取，而别的P可以从尾部取。
  - victim这个从字面上面也可以知道，幸存者嘛，当进行gc的stw时候，会将local中的对象移到victim中去，也就是说幸存了一次gc，

  ### Get

  ```go
  func (p *Pool) Get() interface{} {
      ......
      l, pid := p.pin()
      x := l.private
      l.private = nil
      if x == nil {
          // Try to pop the head of the local shard. We prefer
          // the head over the tail for temporal locality of
          // reuse.
          x, _ = l.shared.popHead()
          if x == nil {
              x = p.getSlow(pid)
          }
      }
      runtime_procUnpin()
      ......
      if x == nil && p.New != nil {
          x = p.New()
      }
      return x
  }
  
  func (p *Pool) getSlow(pid int) interface{} {
      // See the comment in pin regarding ordering of the loads.
      size := atomic.LoadUintptr(&p.localSize) // load-acquire
      locals := p.local                        // load-consume
      // Try to steal one element from other procs.
      for i := 0; i < int(size); i++ {
          l := indexLocal(locals, (pid+i+1)%int(size))
          if x, _ := l.shared.popTail(); x != nil {
              return x
          }
      }
  
      // Try the victim cache. We do this after attempting to steal
      // from all primary caches because we want objects in the
      // victim cache to age out if at all possible.
      size = atomic.LoadUintptr(&p.victimSize)
      if uintptr(pid) >= size {
          return nil
      }
      locals = p.victim
      l := indexLocal(locals, pid)
      if x := l.private; x != nil {
          l.private = nil
          return x
      }
      for i := 0; i < int(size); i++ {
          l := indexLocal(locals, (pid+i)%int(size))
          if x, _ := l.shared.popTail(); x != nil {
              return x
          }
      }
  
      // Mark the victim cache as empty for future gets don't bother
      // with it.
      atomic.StoreUintptr(&p.victimSize, 0)
  
      return nil
  }
  ```

  我去掉了其中一些竞态分析的代码，Get的逻辑其实非常清晰。

  - 如果 private 不是空的，那就直接拿来用
  - 如果 private 是空的，那就先去本地的shared队列里面从头 pop 一个
  - 如果本地的 shared 也没有了，那 getSlow 去拿，其实就是去别的P的 shared 里面偷，偷不到回去 victim 幸存者里面找
  - 如果最后都没有，那就只能调用 New 方法创建一个了

  我随手画了一下，可能不是特别准确，意思到位了

  ### Put

  ```go
  // Put adds x to the pool.
  func (p *Pool) Put(x interface{}) {
      if x == nil {
          return
      }
      ......
      l, _ := p.pin()
      if l.private == nil {
          l.private = x
          x = nil
      }
      if x != nil {
          l.shared.pushHead(x)
      }
      runtime_procUnpin()
      ......
  }
  ```

  看完Get其实Put就很简单了

  - 如果 private 没有，就放在 private
  - 如果 private 有了，那么就放到 shared 队列的头部

  ## 实际测试

  让我们实际写个测试的案例来测测具体使用时会有什么样的变化

  ### Put之后马上Get

  ```go
  var pool = sync.Pool{
      New: func() interface{} {
          return "123"
      },
  }
  
  func main() {
      t := pool.Get().(string)
      fmt.Println(t)
  
      pool.Put("321")
      pool.Put("321")
      pool.Put("321")
      pool.Put("321")
  
      runtime.GC()
      time.Sleep(1 * time.Second)
  
      t2 := pool.Get().(string)
      fmt.Println(t2)
  
      runtime.GC()
      time.Sleep(1 * time.Second)
  
      t2 = pool.Get().(string)
      fmt.Println(t2)
  }
  ```

输出：
123
321
123

你知道为什么吗？

## 总结

这次总结来点不一样的，提几个问题吧。

1. 什么情况下适合使用sync.Pool呢？
2. sync.Pool的对象什么时候会被回收呢？
3. sync.Pool是如何实现线程安全的？
    如果你能回答上面的问题，证明你对它已经足够了解了，那么就可以尝试在具体的情况下使用它来玩玩了。试试吧~

