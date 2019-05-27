#pragma once
#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include "log.h"
#include "Sink.h"

namespace nblog {
	class NBLog : public LoggerBase
	{
		class TextLogger
		{
		public:
			friend class NBLog;
		protected:
		private:
			std::ostringstream m_oss;
			Verbosity m_v = info;
			std::string m_strFunction;
			int m_line = 0;
		};
	public:
		virtual ~NBLog();

		virtual void init() override;
		virtual void addSink(std::shared_ptr<Sink> const& _pSink) override;

		virtual void setMinVerbosity(Verbosity _v) override { m_minVerbosity = _v; }
	private:
		virtual LoggerBase &operator()(Verbosity _v, std::string const& _strFunction, int _line) override;
		virtual void flush() override;
		virtual std::ostringstream &getStream() override;
		std::string getTime();

		std::mutex m_sink_mutex;
		std::mutex m_cache_mutex;
		std::condition_variable m_condition;

		bool m_bExit = false;
		std::thread m_outputThread;
		std::vector<std::shared_ptr<Sink>> m_sinkArray;

		Verbosity m_minVerbosity{ info };
		std::list<SinkMsg> m_bufList;
		std::unordered_map<std::thread::id, TextLogger> m_cache;
	};

}