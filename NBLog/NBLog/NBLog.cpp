#include "stdafx.h"
#include "NBLog.h"

namespace nblog {
	NBLog g_logger;
	LoggerBase &LoggerBase::get()
	{
		return g_logger;
	}

	NBLog::~NBLog()
	{
		m_bExit = true;
		m_condition.notify_one();
		if (m_outputThread.joinable())
		{
			m_outputThread.join();
		}
	}

	void NBLog::init()
	{
		if (m_outputThread.joinable())
		{
			return;
		}

		m_outputThread = std::thread([&] {
			std::list<SinkMsg> outBuffer;
			while (!m_bExit)
			{
				{
					std::unique_lock<std::mutex> lock(m_cache_mutex);
					m_condition.wait(lock, [&]
					{
						if (m_bExit || m_bufList.size() > 0)
						{
							return true;
						}
						return false;
					});

					outBuffer.swap(m_bufList);
					m_bufList.clear();
				}
				if (!outBuffer.empty())
				{
					if (!outBuffer.empty())
					{
						std::lock_guard<std::mutex> lock(m_sink_mutex);
						for (auto const& pSink : m_sinkArray)
						{
							try
							{
								pSink->output(outBuffer);
							}
							catch (...)
							{
							}
						}
					}
					outBuffer.clear();
				}
			}
		});
	}

	void NBLog::addSink(std::shared_ptr<Sink> const& _pSink)
	{
		std::lock_guard<std::mutex> lock(m_sink_mutex);
		m_sinkArray.push_back(_pSink);
	}

	LoggerBase &NBLog::operator()(Verbosity _v, std::string const& _strFunction, int _line)
	{
		std::lock_guard<std::mutex> lock(m_cache_mutex);
		auto id = std::this_thread::get_id();
		m_cache[id].m_v = _v;
		m_cache[id].m_strFunction = _strFunction;
		m_cache[id].m_line = _line;
		return *this;
	}

	void NBLog::flush()
	{
		std::lock_guard<std::mutex> lock(m_cache_mutex);
		auto id = std::this_thread::get_id();
		if (m_cache[id].m_v < m_minVerbosity)
		{
			m_cache[id].m_oss.str("");
			return;
		}

		if (!m_cache[id].m_oss.str().empty())
		{
			m_cache[id].m_oss << std::endl;

			if (m_bufList.size() >= 300)
			{
				// remove half items
				for (size_t i = 0; i < m_bufList.size() / 2; i++)
				{
					m_bufList.pop_front();
				}
			}
			std::string strLine("[");
			strLine += m_cache[id].m_strFunction + "] ";
			strLine += std::string("[") + std::to_string(m_cache[id].m_line) + "] ";

			std::string strTime = getTime();
			strLine += "[";
			strLine += strTime;
			strLine += "]";
			strLine += m_cache[id].m_oss.str();
			m_bufList.emplace_back(strLine);
			m_cache[id].m_oss.str("");
		}
		m_condition.notify_one();
	}

	std::ostringstream &NBLog::getStream()
	{
		std::lock_guard<std::mutex> lock(m_cache_mutex);
		auto id = std::this_thread::get_id();
		return m_cache[id].m_oss;
	}

	std::string NBLog::getTime()
	{
		static char buffer[16] = {};
		memset(buffer, 0, sizeof(buffer));
		time_t nowtime = time(nullptr);
		struct tm *info;
		info = localtime(&nowtime);
		strftime(buffer, sizeof(buffer), "%H:%M:%S", info);
		return buffer;
	}

}