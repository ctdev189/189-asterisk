自定义的扩展应用。

# 媒体实时转发（app_dev189_relay）

实现将媒体信息实时转发给媒体服务器。

参考Asterisk自带的MixMonitor应用。

在"/etc/asterisk/extension_custom.conf"中添加定义
```
[dev189relay]
exten => _X.,1,NoOp(${EXTEN})
same => n,Dev189Relay(${STRFTIME(${EPOCH},,%Y%m%d%H%M%S)}-${CALLERID(num)}-${EXTEN}.wav)
same => n,Dial(PJSIP/${EXTEN},20)
```

目前实现了对channel的录音，录音文件存储在“/var/spool/asterisk/monitor”。目标是实现直接转发RTP包。