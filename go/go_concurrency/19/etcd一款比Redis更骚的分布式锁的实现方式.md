# etcd：一款比Redis更骚的分布式锁的实现方式！

## 分布式锁

关于为什么要有**分布式\**\**锁**这个东西，欢迎阅读我的zk分布式锁的实现，介绍了单机高并发、分布式高并发的解决方案：

用ZooKeeper实现分布式锁

这里再切入本例将使用的场景模拟：商品秒杀，或者说高并发下，对于商品库存扣减操作。我用一个SpringBoot小项目模拟一下该操作。

本例用到的技术栈：

- SpringBoot
- Redis
- etcd

在正式肝代码之前，先来对etcd分布式锁实现的机制和原理做一个了解。

## etcd分布式锁实现的基础机制

## Lease机制

租约机制（**TTL**，Time To Live），etcd 可以为存储的 **key-value** 对设置租约，**当租约到期，key-value 将失效删除**；

同时也支持**续约**，通过客户端可以在租约到期之前续约， 以避免 **key-value** 对过期失效。

Lease 机制可以**保证分布式锁的安全性**，为锁对应的 key 配置租约， **即使锁的持有者因故障而不能主动释放锁，锁也会因租约到期而自动释放**。

## Revision机制

每个 key 带有一个 Revision 号，每进行一次事务便+1，它是全局唯一的， 通过 Revision 的大小就可以知道进行写操作的顺序。

在实现分布式锁时，多个客户端同时抢锁， 根据 Revision 号大小依次获得锁，可以避免 “羊群效应” ，实现**公平锁**。

> 羊群效应：羊群是一种很散乱的组织，平时在一起也是盲目地左冲右撞，但一旦有一只头羊动起来，其他的羊也会不假思索地一哄而上，全然不顾旁边可能有的狼和不远处更好的草。
> etcd的Revision机制，可以根据Revision号的大小顺序进行写操作，因而可以避免“羊群效应”。
> 这和zookeeper的临时顺序节点+监听机制可以避免羊群效应的原理是一致的。

## Prefix机制

即前缀机制。

例如，一个名为 /etcd/lock 的锁，两个争抢它的客户端进行写操作， 实际写入的 key 分别为：key1="/etcd/lock/UUID1"，key2="/etcd/lock/UUID2"。

其中，UUID 表示全局唯一的 ID，确保两个 key 的唯一性。

写操作都会成功，但返回的 Revision 不一样， 那么，如何判断谁获得了锁呢？通过前缀 /etcd/lock 查询，返回包含两个 key-value 对的的 KeyValue 列表， 同时也包含它们的 Revision，通过 Revision 大小，客户端可以判断自己是否获得锁。

## Watch机制

即监听机制。

Watch 机制支持 Watch 某个固定的 key，也支持 Watch 一个范围（前缀机制）。

当被 Watch 的 key 或范围发生变化，客户端将收到通知；在实现分布式锁时，如果抢锁失败， 可通过 Prefix 机制返回的 Key-Value 列表获得 Revision 比自己小且相差最小的 key（称为 pre-key）， 对 pre-key 进行监听，因为只有它释放锁，自己才能获得锁，如果 Watch 到 pre-key 的 DELETE 事件， 则说明 pre-key 已经释放，自己将持有锁。

![](/Users/yangsen/work/LinuxLearn/go/go_concurrency/19/etcd.jpg)

## etcd分布式锁的实现流程

1.建立连接

客户端连接 etcd，以 /etcd/lock 为前缀创建全局唯一的 key， 假设第一个客户端对应的 key="/etcd/lock/UUID1"，第二个为 key="/etcd/lock/UUID2"； 客户端分别为自己的 key 创建租约 - Lease，租约的长度根据业务耗时确定；

2.创建定时任务作为租约的“心跳”

当一个客户端持有锁期间，其它客户端只能等待，为了避免等待期间租约失效， 客户端需创建一个定时任务作为“心跳”进行续约。此外，如果持有锁期间客户端崩溃， 心跳停止，key 将因租约到期而被删除，从而锁释放，避免死锁；

3.客户端将自己全局唯一的 key 写入 etcd

执行 put 操作，将步骤 1 中创建的 key 绑定租约写入 Etcd，根据 Etcd 的 Revision 机制， 假设两个客户端 put 操作返回的 Revision 分别为 1、2，客户端需记录 Revision 用以 接下来判断自己是否获得锁；

4.客户端判断是否获得锁

客户端以前缀 /etcd/lock/ 读取 key-Value 列表，判断自己 key 的 Revision 是否为当前列表中 最小的，如果是则认为获得锁；否则监听列表中前一个 Revision 比自己小的 key 的删除事件，一旦监听到删除事件或者因租约失效而删除的事件，则自己获得锁；

5.执行业务

获得锁后，操作共享资源，执行业务代码

6.释放锁

完成业务流程后，删除对应的key释放锁

## 肝代码

有了以上理论做基础，我们开始etcd分布式锁的代码实现。

## jetcd客户端

**jetcd**是etcd的Java客户端，它提供了丰富的接口来操作etcd，使用方便。

![](/Users/yangsen/work/LinuxLearn/go/go_concurrency/19/jetcd.jpg)

## redis数据准备

初始化库存stock=300，再设置一个lucky=0，表示抢到库存的人，实际场景中可以是用户订单信息，每扣减一个库存，lucky便加1。（相关阅读：[阿里架构师分享：redis 短链接生成实战，网友：还能有这操作？](https://link.zhihu.com/?target=https%3A//www.toutiao.com/i6952678830456717832/%3Fgroup_id%3D6952678830456717832)）

![](/Users/yangsen/work/LinuxLearn/go/go_concurrency/19/redis.jpg)

## etcd分布式锁的实现

由于etcd的Lock接口有一套自己的实现，zookeeper的Lock接口也有自己的一套实现，redis...各种分布式锁实现方案都有自己的Lock，因此，我封装了一个模板方法：

```java
/**
 * @program: distributed-lock
 * @description: 各种分布式锁的基类，模板方法
 * @author: 行百里者
 * @create: 2020/10/14 12:29
 **/
public class AbstractLock implements Lock {
    @Override
    public void lock() {
        throw new RuntimeException("请自行实现该方法");
    }

    @Override
    public void lockInterruptibly() throws InterruptedException {
        throw new RuntimeException("请自行实现该方法");
    }

    @Override
    public boolean tryLock() {
        throw new RuntimeException("请自行实现该方法");
    }

    @Override
    public boolean tryLock(long time, TimeUnit unit) throws InterruptedException {
        throw new RuntimeException("请自行实现该方法");
    }

    @Override
    public void unlock() {
        throw new RuntimeException("请自行实现该方法");
    }

    @Override
    public Condition newCondition() {
        throw new RuntimeException("请自行实现该方法");
    }
}
```

有了这个模板方法之后，后续分布式锁的实现均可以继承这个模板方法类。

**etcd分布式锁的实现**

```java
@Data
public class EtcdDistributedLock extends AbstractLock {
    private final static Logger LOGGER = LoggerFactory.getLogger(EtcdDistributedLock.class);

    private Client client;
    private Lock lockClient;
    private Lease leaseClient;
    private String lockKey;
    //锁路径，方便记录日志
    private String lockPath;
    //锁的次数
    private AtomicInteger lockCount;
    //租约有效期。作用 1：客户端崩溃，租约到期后自动释放锁，防止死锁 2：正常执行自动进行续租
    private Long leaseTTL;
    //续约锁租期的定时任务，初次启动延迟，默认为1s，根据实际业务需要设置
    private Long initialDelay = 0L;
    //定时任务线程池
    ScheduledExecutorService scheduledExecutorService;
    //线程与锁对象的映射
    private final ConcurrentMap<Thread, LockData> threadData = Maps.newConcurrentMap();

    public EtcdDistributedLock(Client client, String lockKey, Long leaseTTL, TimeUnit unit) 		{
        this.client = client;
        this.lockClient = client.getLockClient();
        this.leaseClient = client.getLeaseClient();
        this.lockKey = lockKey;
        this.leaseTTL = unit.toNanos(leaseTTL);
        scheduledExecutorService = Executors.newSingleThreadScheduledExecutor();
    }

    @Override
    public void lock() {

    }

    @Override
    public void unlock() {

    }
}
```

其中lock方法的实现：

```java
@Override
public void lock() {
    Thread currentThread = Thread.currentThread();
    LockData existsLockData = threadData.get(currentThread);
    //System.out.println(currentThread.getName() + " 加锁 existsLockData：" + existsLockData);
    //锁重入
    if (existsLockData != null && existsLockData.isLockSuccess()) {
        int lockCount = existsLockData.lockCount.incrementAndGet();
        if (lockCount < 0) {
            throw new Error("超出etcd锁可重入次数限制");
        }
        return;
    }
    //创建租约，记录租约id
    long leaseId;
    try {
        leaseId = leaseClient.grant(TimeUnit.NANOSECONDS.toSeconds(leaseTTL)).get().getID();
        //续租心跳周期
        long period = leaseTTL - leaseTTL / 5;
        //启动定时续约
        scheduledExecutorService.scheduleAtFixedRate(new KeepAliveTask(leaseClient, leaseId),
                initialDelay,
                period,
                TimeUnit.NANOSECONDS);

        //加锁
        LockResponse lockResponse = lockClient.lock(ByteSequence.from(lockKey.getBytes()), leaseId).get();
        if (lockResponse != null) {
            lockPath = lockResponse.getKey().toString(StandardCharsets.UTF_8);
            LOGGER.info("线程：{} 加锁成功，锁路径：{}", currentThread.getName(), lockPath);
        }

        //加锁成功，设置锁对象
        LockData lockData = new LockData(lockKey, currentThread);
        lockData.setLeaseId(leaseId);
        lockData.setService(scheduledExecutorService);
        threadData.put(currentThread, lockData);
        lockData.setLockSuccess(true);
    } catch (InterruptedException | ExecutionException e) {
        e.printStackTrace();
    }
}
```

简而言之，加锁的代码就是按照如下步骤来的：

1. 检查锁重入性
2. 设置租约
3. 开启定时任务心跳检查
4. 阻塞获取锁
5. 加锁成功，设置锁对象

业务处理完成（扣减库存）后，解锁：

```java
@Override
public void unlock() {
    Thread currentThread = Thread.currentThread();
    //System.out.println(currentThread.getName() + " 释放锁..");
    LockData lockData = threadData.get(currentThread);
    //System.out.println(currentThread.getName() + " lockData " + lockData);
    if (lockData == null) {
        throw new IllegalMonitorStateException("线程：" + currentThread.getName() + " 没有获得锁，lockKey：" + lockKey);
    }
    int lockCount = lockData.lockCount.decrementAndGet();
    if (lockCount > 0) {
        return;
    }
    if (lockCount < 0) {
        throw new IllegalMonitorStateException("线程：" + currentThread.getName() + " 锁次数为负数，lockKey：" + lockKey);
    }
    try {
        //正常释放锁
        if (lockPath != null) {
            lockClient.unlock(ByteSequence.from(lockPath.getBytes())).get();
        }
        //关闭续约的定时任务
        lockData.getService().shutdown();
        //删除租约
        if (lockData.getLeaseId() != 0L) {
            leaseClient.revoke(lockData.getLeaseId());
        }
    } catch (InterruptedException | ExecutionException e) {
        //e.printStackTrace();
        LOGGER.error("线程：" + currentThread.getName() + "解锁失败。", e);
    } finally {
        //移除当前线程资源
        threadData.remove(currentThread);
    }
    LOGGER.info("线程：{} 释放锁", currentThread.getName());
}
```

解锁过程：

1. 重入性检查

2. 移除当前锁的节点路径释放锁

3. 清除重入的线程资源

   ## 接口测试

   ```java
   /**
    * @program: distributed-lock
    * @description: etcd分布式锁演示-高并发下库存扣减
    * @author: 行百里者
    * @create: 2020/10/15 13:24
    **/
   @RestController
   public class StockController {
   
       private final StringRedisTemplate redisTemplate;
   
       @Value("${server.port}")
       private String port;
   
       @Value("${etcd.lockPath}")
       private String lockKey;
   
       private final Client etcdClient;
   
       public StockController(StringRedisTemplate redisTemplate, @Value("${etcd.servers}") String servers) {
           //System.out.println("etcd servers:" + servers);
           this.redisTemplate = redisTemplate;
           this.etcdClient = Client.builder().endpoints(servers.split(",")).build();
       }
   
       @RequestMapping("/stock/reduce")
       public String reduceStock() {
           Lock lock = new EtcdDistributedLock(etcdClient, lockKey, 30L, TimeUnit.SECONDS);
           //获得锁
           lock.lock();
           //扣减库存
           int stock = Integer.parseInt(redisTemplate.opsForValue().get("stock"));
           if (stock > 0) {
               int realStock = stock - 1;
               redisTemplate.opsForValue().set("stock", String.valueOf(realStock));
               //同时lucky+1
               redisTemplate.opsForValue().increment("lucky");
           } else {
               System.out.println("库存不足");
           }
           //释放锁
           lock.unlock();
           return port + " reduce stock end!";
       }
   }
   ```

   这个就很简单了，当一个请求打进来，先试图上锁，上锁成功后，执行业务，扣减库存，同时订单信息+1，业务处理完成后，释放锁。

   ## 压力测试

   测试接口已经完成，用**JMeter**模拟高并发场景，在同一时刻同时发送500个请求（库存只有300），观察结果。

   先启动两个服务，一个8080，一个8090：

   

































