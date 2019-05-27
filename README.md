# NBLog
一个轻量级异步日志框架

## 使用方法

目前仅支持console和文件

	LoggerBase::get().init();
	LoggerBase::get().addSink(std::make_shared<ConsoleSink>());
	LoggerBase::get().addSink(std::make_shared<FileSink>("f:\\temp\\mysink\\boton\\out.txt"));
	LoggerBase::get().setMinVerbosity(info);

	NBLOG(trace) << "level trace";
	NBLOG(debug) << "level debug";
	NBLOG(info) << "level info";
	NBLOG(warning) << "level warning";
	NBLOG(error) << "level error";
	NBLOG(fatal) << "level fatal";

## 日志级别

	trace < debug < info < warning < error < fatal

## 输出格式

    [main] [19] [14:42:56]level info
    [main] [20] [14:42:56]level warning
    [main] [21] [14:42:56]level error
    [main] [22] [14:42:56]level fatal

分别是[调用函数名] [行号] [时间]日志内容