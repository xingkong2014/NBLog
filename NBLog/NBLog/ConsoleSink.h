#pragma once

#include "Sink.h"
namespace nblog {
	class ConsoleSink : public Sink
	{
	public:
		virtual void output(std::list<SinkMsg> const&) override;
	};

}
