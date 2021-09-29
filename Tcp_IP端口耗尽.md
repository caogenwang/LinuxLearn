# Tcp/IP 端口耗尽

本文内容：1.查看端口使用状况的方法；2、端口耗尽所带来的问题；3、解决办法

当客户端启动到服务器的 TCP/IP 套接字连接时，客户端通常连接到服务器上的特定端口，并请求服务器通过临时（或暂时）TCP 或 UDP 端口进行响应。在 Windows Server 2003 和 Windows XP 中，客户端应用程序所使用的临时端口的默认范围为 1025 到 5000。在某些情况下，有可能耗尽默认范围的可用端口。

TCP/IP 端口耗尽的症状随客户端应用程序的不同而不同，但通常都表现为显示网络连接失败错误。若要确定网络连接失败是否是由 TCP/IP 端口耗尽造成的，请在客户端计算机上执行以下步骤：

1. 在运行 Windows XP 或 Windows Server 2003 的计算机上单击“开始”，再单击“运行”，然后键入 **cmd**，单击“确定”打开命令提示。

2. 执行以下操作之一：

   - 在 Windows XP 或 Windows Server 2003 计算机上的命令提示中输入以下命令，显示此计算机上 TCP/IP 协议所使用的活动连接：

   netstat -n
   这将列出绑定到客户端计算机的 TCP/IP 地址以及 TCP/IP 地址与远程服务器通信所使用的端口。如果列出的端口使用了所有可用的端口，则出现了 TCP/IP 端口耗尽现象。

   

   - 在基于 Windows Server 2003 的客户端计算机的命令提示中输入以下命令，以显示 TCP/IP 协议所使用的活动连接：

     ```c
     netstat -b
     ```

```
与 TCP/IP 端口耗尽有关的问题
```

当客户端应用程序尝试使用 TCP/IP 套接字连接到 Server，可能会出现类似于下面的情况：

System.Net.WebException: The underlying connection was closed: An unexpected error occurred on a send.

\- 或者 -

Unable to connect to the remote server
System.Net.Sockets.SocketException: Only one usage of each socket address (protocol/network address/port) is normally permitted.

**原因**

如果客户端计算机中存在数目超常的 TCIP/IP 套接字连接，则客户端计算机上可能出现 TCP/IP 端口耗尽的情况。如果多个客户端应用程序都在建立连接，则可能出现这种情况。

如果所有可用的临时端口都分配给了客户端应用程序，则客户端将出现 TCP/IP 端口耗尽的情况。当 TCP/IP 端口耗尽时，将无法预留客户端端口，并且尝试通过 TCP/IP 套接字连接到服务器的客户端应用程序也将出错。

在高负载情况下，比处于正常负载时更容易出现 TCP/IP 端口耗尽的情况。