#pragma once
#include <sstream>
#include <memory>

namespace nblog {
	enum Verbosity
	{
		trace = -1,
		debug = 0,
		info = 1,
		warning = 2,
		error = 3,
		fatal = 4,
	};

	class Sink;
	class LoggerBase
	{
	public:
		friend class ObjWrapper;
		static LoggerBase &get();

		virtual void init() = 0;
		virtual void addSink(std::shared_ptr<Sink> const& _pSink) = 0;
		virtual LoggerBase &operator()(Verbosity _v, std::string const& _strFunction, int _line) = 0;
		virtual void setMinVerbosity(Verbosity _v) = 0;

	protected:
	private:
		virtual void flush() = 0;
		virtual std::ostringstream &getStream() = 0;
	};

	class ObjWrapper
	{
	public:
		ObjWrapper(LoggerBase &_logger)
			:m_logger(_logger) {}
		~ObjWrapper()
		{
			m_logger.flush();
		}
		std::ostringstream &getLogger() { return m_logger.getStream(); }
	protected:
	private:
		LoggerBase &m_logger;
	};

#define NBLOG(velocity) \
	ObjWrapper(LoggerBase::get()(velocity, __FUNCTION__, __LINE__)).getLogger()
}
