# DEMO说明
本DEMO用于演示纯传输层SDK的使用，DEMO接收远端发送的H264裸码流并写文件。<br>

可通过DEMO配置文件设置本地IP（可指定为空，交由操作系统选择默认IP；多网卡时也可以指定IP避免错误）、本地端口、初始JitterBuff延时、是否保存接收码流。


```js
[Config]
;可指定为空，交由操作系统选择默认IP；多网卡时也可以指定IP避免错误
LocalIp=

;本地端口
LocalPort=4000

;ACK模式或NACK模式，收发两端需配置一致。建议默认使用NACK模式
FecEnableAck=0

;初始JitterBuff延时
FecJitterBuffMs=150

;是否保存接收码流
SaveRecvData=1
```
<br>

SDK API的调用集中在SDClient.cpp中
<br>


测试工程使用VS2010或更高版本编译



---

# 相关资源
跟多文档、代码资源见：https://mediapro.apifox.cn

SDK 商用及定制化、技术支持服务可联系：[http://www.mediapro.cc/](http://www.mediapro.cc/)

