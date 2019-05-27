// NLog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "log.h"
#include "FileSink.h"
#include "ConsoleSink.h"

using namespace nblog;
int main()
{
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

	system("pause");

    return 0;
}

