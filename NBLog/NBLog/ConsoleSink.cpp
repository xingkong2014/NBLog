#include "stdafx.h"
#include "ConsoleSink.h"
#include <iostream>

namespace nblog {
	void ConsoleSink::output(std::list<SinkMsg> const&_msgList)
	{
		for (auto const &msg : _msgList)
		{
			std::cout << msg.getText();
		}
	}
}