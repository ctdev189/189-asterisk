本项目包含了Dev189Lab针对Asterisk进行的扩展。

可以通过编写自己的应用（application）扩展Asterisk的功能。

将代码放在/usr/local/src/asterisk/apps目录下（scp app_dev189_relay.c root@192.168.xxx.xxx:/usr/local/src/asterisk/apps），

/usr/local/src/asterisk目录下执行make，执行make install。

asterisk -r
core restart gracefully
core show applications like Dev189Relay

遗留问题

是否应该放在addons中。
