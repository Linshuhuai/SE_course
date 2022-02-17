## 2.15

### UDP和TCP的socket接口有什么区别？如何实现？

TCP/IP代表传输控制协议/网际协议，指的是一系列协组。TCP和UDP都是传输层协议，IP是网络层协议。TCP和UDP使用IP协议从一个网络传送数据包到另一个网络。
把IP想像成一种高速公路，它允许其它协议在上面行驶并找到到其它电脑的出口。TCP和UDP是高速公路上的“卡车”，它们携带的货物就是像HTTP，文件传输协议FTP这样的应用层协议等。

TCP和UDP是FTP，HTTP和SMTP之类使用的传输层协议。虽然TCP和UDP都是用来传输其他协议的，它们却有一个显著的不同：TCP提供有保证的数据传输，而UDP不提供。
这意味着TCP有一个特殊的机制来确保数据安全的不出错的从一个端点传到另一个端点，而UDP不提供任何这样的保证。

Socket是应用层与TCP/IP协议族通信的中间软件抽象层，它是一组接口。在设计模式中，Socket其实就是一个门面模式，它把复杂的TCP/IP协议族隐藏在Socket接口后面，
对用户来说，一组简单的接口就是全部，让Socket去组织数据，以符合指定的协议。


![image](https://user-images.githubusercontent.com/68037461/154440129-aa699406-3310-4383-8760-7f30750e9727.png)


TCP编程的客户端一般步骤是：

1、创建一个socket，用函数socket()；

2、设置socket属性，用函数setsockopt();  * 可选

3、绑定IP地址、端口等信息到socket上，用函数bind();  * 可选

4、设置要连接的对方的IP地址和端口等属性；

5、连接服务器，用函数connect()；

6、收发数据，用函数send()和recv()，或者read()和write();

7、关闭网络连接；


与之对应的UDP编程步骤要简单许多，一般步骤如下： 

1、创建一个socket，用函数socket()；

2、设置socket属性，用函数setsockopt();  * 可选

3、绑定IP地址、端口等信息到socket上，用函数bind();

4、循环接收数据，用函数recvfrom();

5、关闭网络连接；


### 图计算算法基础知识

见算法设计 chapter3
