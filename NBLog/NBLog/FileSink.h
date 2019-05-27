#pragma once
#include "Sink.h"

namespace nblog {
	class FileSink : public Sink
	{
	public:
		FileSink(std::string const& _logFile)
			:m_LogFilePath(_logFile)
		{}
		~FileSink();

		virtual void output(std::list<SinkMsg> const& _msg_list) override;
	protected:
	private:
		std::string getLogPath();
		void createDirectories(std::string &_strDirectoryPath);

		std::string m_LogFilePath;
		FILE *m_pLogFile = nullptr;
		size_t m_nFileMaxSize{ 500 * 1024 * 1024 };
	};
}
