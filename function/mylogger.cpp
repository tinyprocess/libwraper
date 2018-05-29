#include "stdafx.h"
#include "libwraper.h"
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

namespace DUI_API {

	CMyLogger::CMyLogger(){
		m_bInited = false;
		m_hFile = NULL;
		m_exitEvent = NULL;
		m_thrId = 0;
		m_callback = NULL;
	}

	CMyLogger::~CMyLogger() {
		UnInit();
	}
	bool CMyLogger::Init(const TCHAR* szAppFileName, 
		PMyLoggerCallback cb/* = NULL*/,
		bool logConsole/* = false*/,
		bool logFile/* = true*/) {
		if (m_bInited) return true;

		m_logConsole = logConsole;
		m_logFile = logFile;
		m_callback = cb;

		SYSTEMTIME t = { 0 };
		::GetLocalTime(&t);
		CMyString strTmp(MAX_PATH);
		::GetModuleFileName(NULL, strTmp.c_str(), MAX_PATH);
		m_bUseThr = true;
		m_level = LOGGER_DEBUG;
		strTmp = strTmp.Left(strTmp.ReverseFind(L'\\'));
		m_strFile.Format(_T("%s\\logs\\%s-%04d%02d%02d.txt"), strTmp.c_str(), szAppFileName, t.wYear, t.wMonth, t.wDay);


		DWORD dwWriten = 0;
		if (PathFileExists(m_strFile.c_str())) {//打开已存在的文件 
			m_hFile = CreateFile(m_strFile.c_str(), GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_DELETE | FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (m_hFile != INVALID_HANDLE_VALUE) {
				DWORD t = ::SetFilePointer(m_hFile, 0, 0, FILE_END);
				/*if (t != 2) {
					WriteFile(m_hFile, L"\r\n", 4, &dwWriten, NULL);
				}*/
			}
		}
		else {
			m_hFile = CreateFile(m_strFile.c_str(), GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_DELETE | FILE_SHARE_READ,
				NULL,
				CREATE_ALWAYS,        //打开已存在的文件 
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (m_hFile != INVALID_HANDLE_VALUE) {
				BYTE b[2] = { 0xff,0xfe };
				WriteFile(m_hFile, b, 2, &dwWriten, NULL);
			}
		}
		if (m_hFile == INVALID_HANDLE_VALUE) {
			m_hFile = NULL;
			return false;
		}

		if (m_bUseThr) {
			m_exitEvent = ::CreateEvent(NULL, TRUE, FALSE, _T(""));
			if (m_exitEvent == NULL) {
				CloseHandle(m_hFile);
				m_hFile = NULL;
				return false;
			}
			if (!m_thrMgr.Create(ThreadWrite, this, NULL)) {
				CloseHandle(m_hFile);
				m_hFile = NULL;
				return false;
			}
		}
		m_thrId = 0;// ::GetThreadId(m_thrMgr->native_handle());
		m_bInited = true;
		return true;
	}
	void CMyLogger::UnInit() {
		if (m_bInited) {
			m_bInited = false;
			if (m_exitEvent != NULL) {
				::SetEvent(m_exitEvent);
				CloseHandle(m_exitEvent);
				m_exitEvent = NULL;
			}
			m_thrMgr.Join();
			if (m_hFile != NULL) {
				::CloseHandle(m_hFile);
				m_hFile = NULL;
			}
			m_dataList.clear();
		}

	}
	TCHAR* CMyLogger::GetLogLevelDesc(LOGGER_LEVEL level)
	{
		switch (level)
		{
		case LOGGER_DEBUG:
			return _T("Debug");
		case LOGGER_TRACE:
			return _T("Trace");
		case LOGGER_INFO:
			return _T("Info");
		case LOGGER_WARN:
			return _T("Warn");
		case LOGGER_ERROR:
			return _T("Error");
		case LOGGER_FATAL:
			return _T("FATAL");
		case LOGGER_FILE:
			return _T("FILE");
		default:
			return _T("-----");
		}
	}
	void CMyLogger::Log_0(LOGGER_LEVEL& level, std::shared_ptr<_logdata>& _logdata) {
		_logdata->thrId = m_thrId;
		_logdata->level = level;
		::GetLocalTime(&_logdata->sysTime);
		m_dataList.push_back(_logdata);
	}
	void CMyLogger::LogV(LOGGER_LEVEL level, LPCTSTR format, va_list arg_ptr) {
		std::shared_ptr<_logdata>logdata = std::make_shared<_logdata>();
		CMyString* str = &logdata->strText;
		str->FormatV(format, arg_ptr);
		Log_0(level, logdata);
	}
	void CMyLogger::SetLogLevel(LOGGER_LEVEL level) {
		m_level = level;
	}
	CMyLogger::LOGGER_LEVEL CMyLogger::GetLogLevel() {
		return m_level;
	}
	void CMyLogger::SetUseThread(bool bUseThread) {
		if (!m_bInited) {
			m_bUseThr = bUseThread;
		}
	}
	/*
	void CMyLogger::LogHex(LOGGER_LEVEL level, unsigned char*data, int length) {
		CMyString strHex(length * 4 +100);
		TCHAR strBuf[20] = { 0 };
		for (int i = 0; i < length; i++) {
			if ((i + 1) % 16 == 0) {
				swprintf(strBuf,L"\r\n");
			}
			swprintf(strBuf + _tcslen(strBuf), L"02x", data[i]);
			strHex += strBuf;
		}
		wprintf(L"%s\r\n", strHex.c_str());
	}
	void CMyLogger::DebugHex(unsigned char*data, int length) {
		CMyStringA strHex(length * 10 + 1000);
		char* str = strHex.GetBuffer();
		int pos = 0;
		for (int i = 0; i < length; i++) {
			pos += sprintf(str + pos, "%02x ", data[i]);
			if ((i + 1) % 16 == 0) {
				pos += sprintf(str + pos, "\r\n");
			}
		}
		CMyString strFile;
		strFile.Format(_T("c:\\测试\\以图搜图 %ld-%d.txt"), time(NULL), length);
		CMyFileMgr file;
		if (file.Open(strFile)) {
			file.Write((PBYTE)str, pos);
			file.Close();
		}
	}*/

	void CMyLogger::Log(LOGGER_LEVEL level, LPCTSTR pstrFormat, ...)
	{
		if (level < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, pstrFormat);
		LogV(level, pstrFormat, arg_ptr);
		va_end(arg_ptr);
	}
	void CMyLogger::Debug(LPCTSTR pstrFormat, ...) {
		if (LOGGER_DEBUG < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, pstrFormat);
		LogV(LOGGER_DEBUG, pstrFormat, arg_ptr);
		va_end(arg_ptr);
	}
	void CMyLogger::Trace(LPCTSTR pstrFormat, ...) {
		if (LOGGER_TRACE < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, pstrFormat);
		LogV(LOGGER_TRACE, pstrFormat, arg_ptr);
		va_end(arg_ptr);
	}
	void CMyLogger::Info(LPCTSTR pstrFormat, ...) {
		if (LOGGER_INFO < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, pstrFormat);
		LogV(LOGGER_INFO, pstrFormat, arg_ptr);
		va_end(arg_ptr);
	}
	void CMyLogger::Warn(LPCTSTR pstrFormat, ...) {

		if (LOGGER_WARN < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, pstrFormat);
		LogV(LOGGER_WARN, pstrFormat, arg_ptr);
		va_end(arg_ptr);
	}
	void CMyLogger::Error(LPCTSTR pstrFormat, ...) {

		if (LOGGER_ERROR < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, pstrFormat);
		LogV(LOGGER_ERROR, pstrFormat, arg_ptr);
		va_end(arg_ptr);
	}
	void CMyLogger::Fatal(LPCTSTR pstrFormat, ...) {

		if (LOGGER_FATAL < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, pstrFormat);
		LogV(LOGGER_FATAL, pstrFormat, arg_ptr);
		va_end(arg_ptr);
	}
	void CMyLogger::File(LPCTSTR pstrFormat, ...) {
		va_list arg_ptr;
		va_start(arg_ptr, pstrFormat);
		LogV(LOGGER_FILE, pstrFormat, arg_ptr);
		va_end(arg_ptr);
	}

	void CMyLogger::TryLog(LOGGER_LEVEL level, LPCTSTR format, ...)
	{
		if (level < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, format);
		LogV(level, format, arg_ptr);
		va_end(arg_ptr);
	}

	void CMyLogger::TryDebug(LPCTSTR format, ...)
	{
		if (LOGGER_DEBUG < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, format);
		LogV(LOGGER_DEBUG, format, arg_ptr);
		va_end(arg_ptr);
	}

	void CMyLogger::TryTrace(LPCTSTR format, ...)
	{
		if (LOGGER_TRACE < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, format);
		LogV(LOGGER_TRACE, format, arg_ptr);
		va_end(arg_ptr);
	}

	void CMyLogger::TryInfo(LPCTSTR format, ...)
	{
		if (LOGGER_INFO < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, format);
		LogV(LOGGER_INFO, format, arg_ptr);
		va_end(arg_ptr);
	}

	void CMyLogger::TryWarn(LPCTSTR format, ...)
	{
		if (LOGGER_WARN < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, format);
		LogV(LOGGER_WARN, format, arg_ptr);
		va_end(arg_ptr);
	}

	void CMyLogger::TryError(LPCTSTR format, ...)
	{
		if (LOGGER_ERROR < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, format);
		LogV(LOGGER_ERROR, format, arg_ptr);
		va_end(arg_ptr);
	}

	void CMyLogger::TryFatal(LPCTSTR format, ...)
	{
		if (LOGGER_FATAL < GetLogLevel())
			return;

		va_list arg_ptr;
		va_start(arg_ptr, format);
		LogV(LOGGER_FATAL, format, arg_ptr);
		va_end(arg_ptr);
	}

	DWORD CMyLogger::ThreadWrite(DUI_THREAD::PMyThreadParam pMgr) {
		CMyLogger*pThis = (CMyLogger*)pMgr->lpClass;
		pThis->m_thrId = pMgr->pThread->m_threadId;
		DWORD dwWait = 0;


		std::shared_ptr<_logdata> msg = NULL;
		CMyString* strMsg = NULL;
		DWORD writeLen = 0;

		while (!pMgr->bQuit) {
			while (pThis->m_dataList.size() > 0) {
				if (!pThis->m_dataList.pop_front(msg)) {
					continue;
				}
				strMsg = pThis->MakeLogMsg(msg);
				if (pThis->m_logFile) {
					writeLen = 0;
					if (pThis->m_hFile != NULL) {
						::WriteFile(pThis->m_hFile, strMsg->c_str(), (DWORD)(strMsg->GetLength() * 2), &writeLen, NULL);
					}
				}
				try {
					if (msg->level != LOGGER_FILE) {
						if (pThis->m_callback) {
							pThis->m_callback(&msg->strText);
						}
						if (pThis->m_logConsole) {
							pThis->LogConsoleMsg(strMsg);
						}
					}
				}
				catch (...) {
					pThis->m_callback = NULL;
				}
			}
		}
		//保存剩下的数据
		while (pThis->m_dataList.size() > 0) {
			if (!pThis->m_dataList.pop_front(msg)) {
				continue;
			}
			strMsg = pThis->MakeLogMsg(msg);
			if (pThis->m_logFile) {
				writeLen = 0;
				if (pThis->m_hFile != NULL) {
					::WriteFile(pThis->m_hFile, strMsg->c_str(), (DWORD)(strMsg->GetLength() * 2), &writeLen, NULL);
				}
			}
			try {
				if (msg->level != LOGGER_FILE) {
					if (pThis->m_callback) {
						pThis->m_callback(&msg->strText);
					}
					if (pThis->m_logConsole) {
						pThis->LogConsoleMsg(strMsg);
					}
				}
			}
			catch (...) {
				pThis->m_callback = NULL;
			}
		}
		return 0;
	}

	LPCTSTR CMyLogger::GetTimeStr(const SYSTEMTIME& st)
	{
		static TCHAR t[40];
		_stprintf(t,
			_T("%04d-%02d-%02d %02d:%02d:%02d"),
			st.wYear,st.wMonth,st.wDay,
			st.wHour, st.wMinute, st.wSecond);
		return t;
	}

	CMyString* CMyLogger::MakeLogMsg(std::shared_ptr<_logdata>&logdata)
	{
		static CMyString msg(10000);
		msg.Format(_T("%s [%-5s]  %s\r\n"),
			GetTimeStr(logdata->sysTime),
			GetLogLevelDesc(logdata->level), logdata->strText.c_str());
		return &msg;
	}
	void CMyLogger::LogConsoleMsg(CMyString* msg)
	{
		wprintf(_T("%s"), msg->c_str());
	}
}