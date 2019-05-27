#pragma once

#include <string>
#include <list>
#include <ctime>

namespace nblog {
	class SinkMsg
	{
	public:
		SinkMsg(std::string const &text)
			:m_strText(text) {}
		std::string const& getText() const { return m_strText; }
		time_t getTimestamp() const { return m_timestamp; }
	protected:
	private:
		std::string m_strText;
		time_t m_timestamp = time(nullptr);
	};

	class Sink
	{
	public:
		virtual void output(std::list<SinkMsg> const&) = 0;
	};
}