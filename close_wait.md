![img](/Users/yangsen/work/LinuxLearn/tcp.jpg)

Close wait 不会自动消失，而last_ack会超时自动消失，时间很短，即使在其存续期间，fd其实也是关闭状态。实际上我这个简单程序，测试的时候不会捕捉搭配last_ack,

出现大量close_wait的现象，主要原因是某种情况下对方关闭了socket链接，但是我方忙与读或者写，没有关闭连接。代码需要判断socket，一旦读到0，断开连接，read返回负，检查一下errno，如果不是AGAIN，就断开连接

![img](/Users/yangsen/work/LinuxLearn/tcp_state.png)

