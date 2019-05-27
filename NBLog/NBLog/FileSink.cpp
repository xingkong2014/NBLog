#include "stdafx.h"
#include "FileSink.h"
#include <boost/filesystem.hpp>
#include <windows.h>
#include <ctime>
#include <thread>
#include <direct.h>
#include <io.h>

namespace nblog {
	FileSink::~FileSink()
	{
		if (m_pLogFile != nullptr)
		{
			fclose(m_pLogFile);
		}
	}

	void FileSink::output(std::list<SinkMsg> const& _msg_list)
	{
		time_t	nowtime = 0;
		char	szTime[128] = { 0 };
		time(&nowtime);
		ctime_s(szTime, sizeof(szTime), &nowtime);
		szTime[strlen(szTime) - 1] = '\0';

		for (auto const& msg : _msg_list)
		{
			if (m_pLogFile == nullptr)
			{
				m_pLogFile = fopen(getLogPath().c_str(), "a");
			}
			if (m_pLogFile != nullptr)
			{
				if (ftell(m_pLogFile) >= m_nFileMaxSize)
				{
					m_pLogFile = freopen(getLogPath().c_str(), "w", m_pLogFile);
				}
				if (m_pLogFile != nullptr)
				{
					fputs(msg.getText().c_str(), m_pLogFile);
					fflush(m_pLogFile);
				}
			}
		}
	}

	std::string FileSink::getLogPath()
	{
		std::string strLogPath(m_LogFilePath);
		if (!strLogPath.empty())
		{
			try
			{
				auto pos = strLogPath.rfind('\\');
				if (pos != std::string::npos)
				{
					std::string strFolder(strLogPath.substr(0, pos));
					createDirectories(strFolder);
				}
			}
			catch (std::exception const&)
			{
				strLogPath = "";
			}
			catch (...)
			{
				strLogPath = "";
			}
		}
		if (strLogPath.empty())
		{
			char szPath[MAX_PATH] = {};
			::GetModuleFileName(nullptr, szPath, MAX_PATH);
			char *pszTmp = strrchr(szPath, '\\');
			*(pszTmp + 1) = 0;
			strLogPath = szPath;
			strLogPath += "out.log";
		}

		return strLogPath;
	}

	void FileSink::createDirectories(std::string &_strDirectoryPath)
	{
		auto pos = _strDirectoryPath.find('\\');
		while (pos != std::string::npos)
		{
			std::string strTopDirectory(_strDirectoryPath.substr(0, pos));
			_mkdir(strTopDirectory.c_str());

			pos = _strDirectoryPath.find('\\', pos + 1);
		}
		_mkdir(_strDirectoryPath.c_str());
	}
}