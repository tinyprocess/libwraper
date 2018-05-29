#include "stdafx.h"
#include <regex>
#include "libwraper.h"
#include <iphlpapi.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"iphlpapi.lib")
namespace DUI_API {


	namespace REG {
		DUI_FUNC_C_EXPORT HKEY OpenKey(HKEY hRootKey, LPCTSTR strKey)
		{
			HKEY hKey;
			LONG nError = RegOpenKeyEx(hRootKey, strKey, NULL, KEY_ALL_ACCESS, &hKey);

			if (nError == ERROR_FILE_NOT_FOUND)
			{
				//cout << "Creating registry key: " << strKey << endl;
				nError = RegCreateKeyEx(hRootKey, strKey, NULL, NULL, 
					REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
			}

			//if (nError)
				//cout << "Error: " << nError << " Could not find or create " << strKey << endl;

			return nError == ERROR_SUCCESS ? hKey : NULL;
		}

		DUI_FUNC_C_EXPORT  void CloseKey(HKEY& hKey) {
			if (hKey) {
				RegCloseKey(hKey);
				hKey = NULL;
			}
		}


		DUI_FUNC_C_EXPORT  void SetVal(HKEY hKey, LPCTSTR lpValue, DWORD data)
		{
			LONG nError = RegSetValueEx(hKey, lpValue, NULL, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

			//if (nError)
				//cout << "Error: " << nError << " Could not set registry value: " << (char*)lpValue << endl;
		}

		DUI_FUNC_C_EXPORT  DWORD GetVal(HKEY hKey, LPCTSTR lpValue)
		{
			DWORD data;        
			DWORD size = sizeof(data);    
			DWORD type = REG_DWORD;
			LONG nError = RegQueryValueEx(hKey, lpValue, NULL, &type, (LPBYTE)&data, &size);

			if (nError == ERROR_FILE_NOT_FOUND)
				data = 0; // The value will be created and set to data next time SetVal() is called.
			//else if (nError)
				//cout << "Error: " << nError << " Could not get registry value " << (char*)lpValue << endl;

			return data;
		}

		DUI_FUNC_C_EXPORT  void SetString(HKEY hKey, LPCTSTR lpValue, TCHAR* strBuff, int bufSize)
		{
			LONG nError = RegSetValueEx(hKey, lpValue, NULL, REG_SZ, (LPBYTE)strBuff, bufSize);

			//if (nError)
			//cout << "Error: " << nError << " Could not set registry value: " << (char*)lpValue << endl;
		}

		DUI_FUNC_C_EXPORT  DWORD GetString(HKEY hKey, LPCTSTR lpValue,TCHAR* strBuff,int bufSize)
		{
			strBuff[0] = 0;
			DWORD size = bufSize;
			DWORD type = REG_SZ;
			LONG nError = RegQueryValueEx(hKey, lpValue, NULL, &type, (LPBYTE)strBuff, &size);

			//if (nError == ERROR_FILE_NOT_FOUND) {
				//data = 0; // The value will be created and set to data next time SetVal() is called.
						  //else if (nError)
						  //cout << "Error: " << nError << " Could not get registry value " << (char*)lpValue << endl;
			//}
			return nError;
		}
	}

#ifdef  _USE_MFC
	DUI_FUNC_CXX_EXPORT  CStringW MutiToWideString(LPCSTR data) {
		int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, data, -1, NULL, 0);
		CStringW tmp;
		tmp.GetBufferSetLength(unicodeLen);
		::MultiByteToWideChar(CP_ACP, 0, data, -1, tmp.GetBuffer(), unicodeLen);
		return 	tmp;
	}
	DUI_FUNC_CXX_EXPORT  CStringA WideToMutiString(LPCWSTR data) {
		int mutiLen = ::WideCharToMultiByte(CP_ACP, 0, data, -1, NULL, 0, NULL, NULL);
		CStringA tmp;
		tmp.GetBufferSetLength(mutiLen);
		::WideCharToMultiByte(CP_ACP, 0, data, -1, tmp.GetBuffer(), mutiLen, NULL, NULL);
		return 	tmp;
	}
#else

	DUI_FUNC_CXX_EXPORT CMyString MyGetFlowSize(long long size) {
		static CMyString strRet = _T("");
		float g = 0, m = 0, k = 0;
		g = (float)(size / 1024.0 / 1024.0 / 1024.0);
		m = (float)((size - g * 1024 * 1024 * 1024) / 1024.0 / 1024.0);
		k = (float)((size - g * 1024 * 1024 * 1024 - m * 1024 * 1024) * size / 1024.0);
		if (g) {
			strRet.Format(_T("%0.2f G"), g);
		}
		else if (m) {
			strRet.Format(_T("%0.2f M"), m);
		}
		else if (k) {
			strRet.Format(_T("%0.2f K"), k);
		}
		else {
			strRet.Format(_T("%d B"), size);
		}
		return strRet;
	}


	bool GetNetInterface(int IfIndex, char *strIp,u_char* mac) {
		bool bSucess = false;
		//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
		PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
		//得到结构体大小,用于GetAdaptersInfo参数
		unsigned long stSize = sizeof(IP_ADAPTER_INFO);
		//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
		int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
		if (ERROR_BUFFER_OVERFLOW == nRel)
		{
			//如果函数返回的是ERROR_BUFFER_OVERFLOW
			//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
			//这也是说明为什么stSize既是一个输入量也是一个输出量
			//释放原来的内存空间
			delete pIpAdapterInfo;
			//重新申请内存空间用来存储所有网卡信息
			pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
			//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
			nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
		}
		if (ERROR_SUCCESS == nRel) {
			//输出网卡信息
			PIP_ADAPTER_INFO pIpAdapter = pIpAdapterInfo;
			while (pIpAdapter) {
				if (pIpAdapter->Index == IfIndex) {
					memcpy(strIp, pIpAdapter->IpAddressList.IpAddress.String, 16);
					if (mac) {
						memcpy(mac, pIpAdapter->Address,6);
					}
					bSucess = true;
					break;
				}
				pIpAdapter = pIpAdapter->Next;
			}
		}
		//释放内存空间
		if (pIpAdapterInfo)
		{
			delete pIpAdapterInfo;
		}
		return bSucess;
	}

	DUI_FUNC_C_EXPORT  bool GetAllIpAddress(std::vector<CMyStringA>&ipList) {
		ipList.clear();
		ipList.resize(0);
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		CMyStringA strIp;
		char szHost[256] = { 0 };
		hostent *lpHost = gethostbyname(szHost);
		memset(szHost, 0, sizeof(szHost));
		// 取得本地主机名称  
		::gethostname(szHost, 256);
		// 打印出所有IP地址  
		in_addr addr;
		for (int i = 0; ; i++)
		{
			char *p = lpHost->h_addr_list[i];
			if (p == NULL)
				break;
			memcpy(&addr.S_un.S_addr, p, lpHost->h_length);
			char *szIp = ::inet_ntoa(addr);//将32位的二进制数转化为字符串;  
			strIp = szIp;
			ipList.push_back(strIp);
		}
		::WSACleanup();
		return ipList.size() > 0;
	}


	DUI_FUNC_CXX_EXPORT  std::wstring MutiToWideString(LPCSTR data) {
		try {
			int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, data, -1, NULL, 0);
			std::wstring tmp(unicodeLen, '\0');
			::MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)tmp.c_str(), unicodeLen);
			return 	tmp;
		}
		catch (...) {
		}
		return  std::wstring(L"");
	}
	DUI_FUNC_CXX_EXPORT  std::string WideToMutiString(LPCWSTR data) {
		try {
			int mutiLen = ::WideCharToMultiByte(CP_ACP, 0, data, -1, NULL, 0, NULL, NULL);
			std::string tmp(mutiLen, '\0');
			::WideCharToMultiByte(CP_ACP, 0, data, -1, (char*)tmp.c_str(), mutiLen, NULL, NULL);
			return 	tmp;
		}
		catch (...) {
		}
		return std::string("");
	}

	DUI_FUNC_CXX_EXPORT  std::string MutiToUtf8String(LPCSTR data) {
		try {
			int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, data, -1, NULL, 0);
			std::wstring tmp(unicodeLen, '\0');
			::MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)tmp.c_str(), unicodeLen);


			int utf8Len = ::WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)tmp.c_str(), (int)tmp.length(), NULL, 0, NULL, NULL);
			std::string tmp2(utf8Len, '\0');
			::WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)tmp.c_str(), (int)tmp.length(), (char*)tmp2.c_str(), utf8Len, NULL, NULL);
			return tmp2;
		}
		catch (...) {
		}
		return  std::string("");
	}

	DUI_FUNC_CXX_EXPORT  std::string WideToUtf8String(LPCWSTR data) {
		int iLen = ::WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
		std::string tmp(iLen,'\0');
		::WideCharToMultiByte(CP_UTF8, 0, data, -1, (char*)tmp.c_str(), iLen, NULL, NULL);
		tmp[iLen] = '\0';
		return tmp;
	
	}
	DUI_FUNC_CXX_EXPORT  std::string Utf8ToMutiString(LPCSTR data) {
		int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, data, -1, NULL, 0);
		std::wstring tmp(unicodeLen, '\0');
		::MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)tmp.c_str(), unicodeLen);

		int mLen = WideCharToMultiByte(CP_ACP, 0, tmp.c_str(), -1, NULL, 0, NULL, NULL);
		std::string tmp2(mLen, '\0');
		WideCharToMultiByte(CP_ACP, 0, tmp.c_str(), -1, (char*)tmp2.c_str(), mLen, NULL, NULL);
		return 	tmp2;
	}


	DUI_FUNC_CXX_EXPORT  std::wstring Utf8ToWideString(LPCSTR data) {
		int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, data, -1, NULL, 0);
		std::wstring tmp(unicodeLen, '\0');
		::MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)tmp.c_str(), unicodeLen);


		int mLen = WideCharToMultiByte(CP_ACP,0,tmp.c_str(),-1,NULL,0,NULL,NULL);
		std::string tmp2(mLen, '\0');
		WideCharToMultiByte(CP_ACP, 0, tmp.c_str(), -1, (char*)tmp2.c_str(), mLen, NULL, NULL);

		unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, tmp2.c_str(), -1, NULL, 0);
		tmp.clear();
		tmp.resize(unicodeLen);
		::MultiByteToWideChar(CP_ACP, 0, tmp2.c_str(), -1, (wchar_t*)tmp.c_str(), unicodeLen);
		tmp[unicodeLen - 1] = 0;
		return 	tmp;
	}

#endif


	DUI_FUNC_C_EXPORT int gbk_strlen(const char* str)
	{
		const char* p = str;        //p用于后面遍历  

		while (*p)                   //若是结束符0，则结束循环  
		{
			if (*p < 0 && (*(p + 1)<0 || *(p + 1) < 63))            //中文汉字情况  
			{
				str++;              //str移动一位，p移动移动2位，因此长度加1  
				p += 2;
			}
			else
			{
				p++;                //str不动，p移动一位，长度加1  
			}
		}
		return p - str;               //返回地址之差  
	}
	DUI_FUNC_C_EXPORT int gbk_getstr(char* str, int _limit) {
		if (str == NULL) return 0;
		char* p = str;        //p用于后面遍历  
		int len = 0;
		while ((*p) != '\0')                   //若是结束符0，则结束循环  
		{
			if (len >= _limit) {
				(*p) = 0;
				break;
			}
			if (*p < 0 && (*(p + 1)<0 || *(p + 1) < 63))
			{

				p += 2;
			}
			else
			{
				p++;
			}
			len++;
		}
		return len;
	}



	DUI_FUNC_CXX_EXPORT void GetFileString(unsigned char*buf, int iSize, CMyStringA& strText) {
		USHORT file_flag = ntohs(*(USHORT*)buf);
		bool IsAnsi = false;

		if (iSize >=2) {
		
			if (file_flag == 0xFFFE) {//Unicode
				strText = WideToMutiString((TCHAR*)(buf + 2)).c_str();
			}else if (file_flag == 0xFEFF) {//Unicode big endian：
				BYTE t;
				for (int i = 2; i<iSize; i = i + 2)
				{
					t = buf[i];
					buf[i] = buf[i + 1];
					buf[i + 1] = t;
				}
				strText = WideToMutiString((TCHAR*)(buf + 2)).c_str();
			}else if (file_flag == 0xEFBB) {//UTF-8：
				if (iSize >= 3 && buf[2] == 0xBF) {
					strText = WideToMutiString(Utf8ToWideString((LPCSTR)(buf + 3)).c_str()).c_str();
				}
				else {
					IsAnsi = true;
				}
			}
			else {
				IsAnsi = true;
			}
		}
		else {
			IsAnsi = true;
		}
		if (IsAnsi) {
			strText =  (char*)buf;
		}
	}


	DUI_FUNC_C_EXPORT BOOL CreateNewProcess(LPCTSTR pszExeName, LPTSTR iCmd, bool isShow)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		ZeroMemory(&si, sizeof(si));

		si.cb = sizeof(si);
		si.wShowWindow = isShow ? SW_SHOWDEFAULT : SW_HIDE;

		if (CreateProcess(pszExeName, iCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return TRUE;
		}
		return FALSE;
	}

	DUI_FUNC_C_EXPORT INT StartProcess(TCHAR *Exe, TCHAR *lParams, DWORD timeOut, bool show) {
		INT iRet = -1;
		DWORD dwRet = 0, dwCreateFlag = 0;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(STARTUPINFOA);
		if (show) {
			si.wShowWindow = SW_SHOW;
			si.dwFlags = STARTF_USESHOWWINDOW;
			dwCreateFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
		}
		else {
			dwCreateFlag = NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW;
		}
		CMyString cmd(256);
		cmd = Exe;
		if (lParams && lParams[0] != 0) {
			cmd += _T(" ");
			cmd += lParams;
		}
		BOOL working = ::CreateProcess(NULL, cmd.c_str(), NULL, NULL,
			FALSE, dwCreateFlag, NULL, NULL, &si, &pi);  //建立程序
		if (working) { //打开错误
			dwRet = WaitForSingleObject(pi.hProcess, timeOut);  //等待程序结束
			if (dwRet == WAIT_OBJECT_0) {
				DWORD Result = 0;
				if (GetExitCodeProcess(pi.hProcess, &Result)) { //得到程序返回值
					iRet = Result;
				}
			}
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		return iRet;
	}

	DUI_FUNC_C_EXPORT unsigned char char2byte(char c) {
		unsigned char out;
		if (c >= 'a' && c <= 'f') {
			out = c - 'a' + 10;
		}
		else if (c >= 'A' && c <= 'Z') {
			out = c - 'A' + 10;
		}
		else if (c >= '0' && c <= '9') {
			out = c - '0';
		}
		else out = 0;
		return out;
	}
	DUI_FUNC_C_EXPORT int Str2Bytes(char *src, int len, unsigned char *dst)
	{
		int i = 0;
		if (len<0)
			len = (int)strlen(src);
		int t = 0;
		while (i<len) {
			dst[t++] = char2byte(src[i]) * 16 + char2byte(src[i + 1]);
			i += 2;
		}
		return t;
	}


	DUI_FUNC_CXX_EXPORT  CMyString  GetErrorString(DWORD dwError) {
		LPVOID lpMsgBuf;
		int iRet = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);

		CMyString  strRet((TCHAR*)lpMsgBuf, iRet);
		LocalFree(lpMsgBuf);
		return strRet;
	}
	DUI_FUNC_CXX_EXPORT  CMyString  StringFormat(LPCTSTR pstrFormat, ...) {
		va_list valist;                   /* 定义保存函数参数的结构 */
		CMyString  szBuffer;
		va_start(valist, pstrFormat);
		do {
			int iRet = _vsntprintf(szBuffer.c_str(), szBuffer.GetAllocLength() - 1, pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 256;
			for (int i = 0; i < 8; i++) {
				if (i != 0) {
					_bufLen = 1024 * i;
				}
				szBuffer.SetBufferLength(_bufLen);
				szBuffer[0] = 0;
				iRet = _vsntprintf(szBuffer.c_str(), szBuffer.GetAllocLength() - 1, pstrFormat, valist);

				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		va_end(valist);
		return szBuffer;
	}
	DUI_FUNC_CXX_EXPORT  CMyStringA  StringFormatA(LPCSTR pstrFormat, ...) {
		va_list valist;                   /* 定义保存函数参数的结构 */
		CMyStringA  szBuffer;
		va_start(valist, pstrFormat);
		do {
			int iRet = _vsnprintf(szBuffer.c_str(), szBuffer.GetAllocLength() - 1, pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 256;
			for (int i = 0; i < 8; i++) {
				if (i != 0) {
					_bufLen = 1024 * i;
				}
				szBuffer.SetBufferLength(_bufLen);
				szBuffer[0] = 0;
				iRet = _vsnprintf(szBuffer.c_str(), szBuffer.GetAllocLength() - 1, pstrFormat, valist);

				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		va_end(valist);
		return szBuffer;
	}


	DUI_FUNC_CXX_EXPORT  CMyStringW  StringFormatW(LPCWSTR pstrFormat, ...) {
		va_list valist;                   /* 定义保存函数参数的结构 */
		CMyStringW  szBuffer;
		va_start(valist, pstrFormat);
		do {
			int iRet = _vsnwprintf(szBuffer.c_str(), szBuffer.GetAllocLength() - 1, pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 256;
			for (int i = 0; i < 8; i++) {
				if (i != 0) {
					_bufLen = 1024 * i;
				}
				szBuffer.SetBufferLength(_bufLen);
				szBuffer[0] = 0;
				iRet = _vsnwprintf(szBuffer.c_str(), szBuffer.GetAllocLength() - 1, pstrFormat, valist);

				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		va_end(valist);
		return szBuffer;
	}



	DUI_FUNC_C_EXPORT  size_t StringToInts(const char* _str, const char* _strSplict, std::vector<int>* _vecInt) {
		_vecInt->clear();
		char strPattern[255] = { 0 };
		sprintf_s(strPattern, 255, "([0-9]+)%s*", _strSplict);
		if (_str) {
			std::string strString = _str;
			std::regex pattern(strPattern, std::regex::icase);
			std::smatch result;
			std::string::const_iterator itS = strString.begin();
			std::string::const_iterator itE = strString.end();
			while (regex_search(itS, itE, result, pattern))//如果匹配成功
			{
				_vecInt->push_back(atoi(result[1].str().c_str()));
				itS = result[0].second;//新的位置开始匹配
			}
		}
		return _vecInt->size();
	}

	//分隔符 123,222,333
	DUI_FUNC_C_EXPORT  size_t StringToLongs(const char* _str, const char* _strSplict, std::vector<long>* _vecInt)
	{
		_vecInt->clear();
		char strPattern[255] = { 0 };
		sprintf_s(strPattern, 255, "([0-9]+)%s*", _strSplict);
		if (_str) {
			std::string strString = _str;
			std::regex pattern(strPattern, std::regex::icase);
			std::smatch result;
			std::string::const_iterator itS = strString.begin();
			std::string::const_iterator itE = strString.end();
			while (regex_search(itS, itE, result, pattern))//如果匹配成功
			{
				_vecInt->push_back(atol(result[1].str().c_str()));
				itS = result[0].second;//新的位置开始匹配
			}
		}
		return _vecInt->size();
	}
	DUI_FUNC_C_EXPORT  size_t StringToLongLongs(const char* _str, const char* _strSplict, std::vector<long long>* _vecInt) {
		_vecInt->clear();
		char strPattern[255] = { 0 };
		sprintf_s(strPattern, 255, "([0-9]+)%s*", _strSplict);
		if (_str) {
			std::string strString = _str;
			std::regex pattern(strPattern, std::regex::icase);
			std::smatch result;
			std::string::const_iterator itS = strString.begin();
			std::string::const_iterator itE = strString.end();
			while (regex_search(itS, itE, result, pattern))//如果匹配成功
			{

				_vecInt->push_back(_atoi64(result[1].str().c_str()));
				itS = result[0].second;//新的位置开始匹配
			}
		}
		return _vecInt->size();
	}


	/*****************************************

	CMyLockObj

	****************************************/


	CMyLockObj::CMyLockObj() {
		m_handle = NULL;
	}
	CMyLockObj::~CMyLockObj() {
		Close();
	}
	void CMyLockObj::Close() {
		if (m_handle != NULL) {
			CloseHandle(m_handle);
			m_handle = NULL;
		}
	}
	void CMyLockObj::SetHandle(HANDLE hHandle) {
		Close();
		m_handle = hHandle;
	}
	HANDLE& CMyLockObj::GetHanlde() {
		return m_handle;
	}
	bool CMyLockObj::Valid() {
		return (m_handle == NULL);
	}






	/*****************************************
	
	CMyCriSec
	
	****************************************/


	CMyCriSec::CMyCriSec() {
		memset(&m_cs, 0, sizeof(m_cs));
		InitializeCriticalSection(&m_cs);
	}
	CMyCriSec::~CMyCriSec() {
		DeleteCriticalSection(&m_cs);
	}
	void CMyCriSec::EnterCriSec() {
		EnterCriticalSection(&m_cs);
	}
	void CMyCriSec::LeaveCriSec() {
		LeaveCriticalSection(&m_cs);
	}


	/******************************

	CMyFileMgr

	******************************/
	CMyFileMgr::CMyFileMgr() {
		m_hFile = NULL;
	}
	CMyFileMgr::~CMyFileMgr() {
		Close();
	}
	void CMyFileMgr::Close() {
		if (m_hFile != NULL && m_hFile != INVALID_HANDLE_VALUE) {
			CloseHandle(m_hFile);
			m_hFile = NULL;
		}
	}
	bool CMyFileMgr::Open(LPCTSTR szFileName, DWORD dwDesiredAccess/* = GENERIC_READ | GENERIC_WRITE*/) {
		m_hFile = ::CreateFile(szFileName, dwDesiredAccess, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) {
			return false;
		}
		return true;
	}
	DWORD CMyFileMgr::Write(PBYTE bytes, DWORD dwSize) {
		DWORD dwNumberOfBytesWritten = 0;
		::WriteFile(m_hFile, bytes, dwSize, &dwNumberOfBytesWritten, NULL);
		return dwNumberOfBytesWritten;
	}
	DWORD CMyFileMgr::Read(PBYTE bytes, DWORD dwSize) {
		DWORD dwNumberOfBytesRead = 0;
		::ReadFile(m_hFile, bytes, dwSize, &dwNumberOfBytesRead, NULL);
		return dwNumberOfBytesRead;
	}
	HANDLE CMyFileMgr::GetHandle() {
		return m_hFile;
	}
	DWORD  CMyFileMgr::GetFileLength() {
		return GetFileSize(m_hFile, 0);
	}



	/*

	template<class T>
	class CMyList {
	public:
		typedef struct MEM_NODE {
			T  data;
			struct MEM_NODE* next;
			struct MEM_NODE* prev;
		}*PMEM_NODE;
		CMyList() {
			m_pHead = m_pTail = (PMEM_NODE)malloc(sizeof(MEM_NODE));
			m_pHead->next = NULL;
			m_pHead->prev = NULL;
			m_count = 0;
		}
		~CMyList() {
			clear();
			free(m_pHead);
		}
	public:
		bool IsEmpty() {
			return (m_pHead == m_pTail);
		}
		bool InsertAt(const T& X, int iIndex) {
			PMEM_NODE p = m_pHead->next;
			if (iIndex == 0 || m_count == 0) {
				return push_front(X);
			}
			else if (iIndex >= m_count) {
				return push_back(X);
			}
			int i = 0;
			while (i++ < iIndex) {
				p = p->next;
			}
			if (!p) {
				return push_back(X);
			}
			PMEM_NODE node = (PMEM_NODE)malloc(sizeof(MEM_NODE));
			if (node == NULL) return false;
			memcpy(&node->data, &X, sizeof(T));

			p->prev->next = node;
			node->prev = p->prev;

			node->next = p;
			p->prev = node;

			m_count++;
			return true;

		}
		bool push_back(const T& X) {
			PMEM_NODE node = (PMEM_NODE)malloc(sizeof(MEM_NODE));
			memcpy(&node->data, &X, sizeof(T));
			if (IsEmpty()) {
				m_pHead->next = node;
				m_pHead->prev = NULL;
				node->prev = m_pHead;
				node->next = NULL;
				m_pTail = node;
			}
			else {
				m_pTail->next = node;
				node->prev = m_pTail;
				node->next = NULL;

				m_pTail = node;
			}
			m_count++;
			return true;
		}
		bool push_front(const T& X) {
			PMEM_NODE node = (PMEM_NODE)malloc(sizeof(MEM_NODE));
			memcpy(&node->data, &X, sizeof(T));
			if (IsEmpty()) {
				m_pHead->next = node;
				m_pHead->prev = NULL;
				node->prev = m_pHead;
				node->next = NULL;
				m_pTail = node;
			}
			else {
				node->next = m_pHead->next;
				m_pHead->next->prev = node;

				node->prev = m_pHead;
				m_pHead->next = node;
			}
			m_count++;
			return true;
		}
		bool pop_front(T&X) {
			if (IsEmpty()) {
				return false;
			}
			PMEM_NODE node = m_pHead->next;
			memcpy(&X, &node->data, sizeof(T));
			if (m_count == 1) {
				m_pHead->next = NULL;
				m_pHead->prev = NULL;
				m_pTail = m_pHead;
			}
			else {
				node->prev->next = node->next;
				node->next->prev = m_pHead;


			}
			free(node);
			m_count--;
			return true;
		}
		bool pop_back(T&X) {
			if (IsEmpty()) {
				return false;
			}
			PMEM_NODE node = m_pTail;
			memcpy(&X, &node->data, sizeof(T));
			if (m_count == 1) {
				m_pHead->next = NULL;
				m_pHead->prev = NULL;
				m_pTail = m_pHead;
			}
			else {
				m_pTail = node->prev;
				m_pTail->next = NULL;

			}
			free(node);
			m_count--;
			return true;
		}
		PMEM_NODE GetHead() {
			return m_pHead->next;
		}
		PMEM_NODE GetTail() {
			if (IsEmpty()) return NULL;
			return m_pTail;
		}
		void clear() {
			PMEM_NODE p = m_pHead->next, q;
			while (p) {
				q = p;
				p = p->next;
				free(q);
			}
			m_pHead->next = NULL;
			m_pHead->prev = NULL;
			m_pTail = m_pHead;
			m_count = 0;
		}

		void clearAndFree() {
			PMEM_NODE p = m_pHead->next, q;
			while (p) {
				q = p;
				p = p->next;
				SAFE_FREE(q->data);
				free(q);
			}
			m_pHead->next = NULL;
			m_pHead->prev = NULL;
			m_pTail = m_pHead;
			m_count = 0;
		}
		void clearAndDelete() {
			PMEM_NODE p = m_pHead->next, q;
			while (p) {
				q = p;
				p = p->next;
				SAFE_DELETE(q->data);
				free(q);
			}
			m_pHead->next = NULL;
			m_pHead->prev = NULL;
			m_pTail = m_pHead;
			m_count = 0;
		}
		int GetCount() {
			return m_count;
		}
	private:
		PMEM_NODE m_pHead, m_pTail;
		int m_count;
	};
	*/

}