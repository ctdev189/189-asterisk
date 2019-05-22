媒体转发

# 使用
在dailplan中使用应用。

在"/etc/asterisk/extension_custom.conf"中添加定义
```
[dev189relay]
exten => _X.,1,NoOp(${EXTEN})
same => n,Dev189Relay(${STRFTIME(${EPOCH},,%Y%m%d%H%M%S)}-${CALLERID(num)}-${EXTEN}.wav,NULL,NULL,192.168.xxx.xxx:xxxx)
same => n,Dial(PJSIP/${EXTEN},20)
```

# MixMonitor
MixMonitor实现了媒体的本地录制，参考MixMonitor的实现方式，将收到的媒体帧（frame）转发到Asterisk外部的媒体服务器。

# audiohook
MixMonitor通过设置audiohook获得媒体数据。每个指定了MixMonitor应用的channel都会启动一个mixmonitor的实例。

在mixmonitor上初始化一个audiohook的实例
```
ast_audiohook_init(&mixmonitor->audiohook, AST_AUDIOHOOK_TYPE_SPY, mixmonitor_spy_type, 0)
```

autochan有什么用？

启动一个线程进行监听

读取接收到的帧（frame）
```
if (!(fr = ast_audiohook_read_frame_all(&mixmonitor->audiohook, SAMPLES_PER_FRAME, format_slin, &fr_read, &fr_write)))
{
    ast_audiohook_trigger_wait(&mixmonitor->audiohook);

    if (mixmonitor->audiohook.status != AST_AUDIOHOOK_STATUS_RUNNING)
    {
        break;
    }
    continue;
}
```

# Asterisk RTP
asterisk自带了处理RTP的代码，可以直接使用。

rtp_engine.c
res_rtp_asterisk.c

创建RTP实例
```
/**
 * rtp_engine.c
 *
 * Create a new RTP instance
 * engine_name Name of the engine to use for the RTP instance
 * sched Scheduler context that the RTP engine may want to use
 * sa Address we want to bind to
 * data Unique data for the engine
 */
struct ast_rtp_instance *ast_rtp_instance_new(const char *engine_name,
		struct ast_sched_context *sched, const struct ast_sockaddr *sa,
		void *data)
```

设置远端地址
```
int ast_rtp_instance_set_incoming_source_address(struct ast_rtp_instance *instance,
	const struct ast_sockaddr *address)
```

发送RTP包
```
/**
 * rtp_engine.c
 */
int ast_rtp_instance_write(struct ast_rtp_instance *instance, struct ast_frame *frame)
{
	int res;

	ao2_lock(instance);
	res = instance->engine->write(instance, frame);
	ao2_unlock(instance);
	return res;
}
```

# 遗留问题
去掉和录音相关的代码

主被叫都录了吗？

录视频

释放资源