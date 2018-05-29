#include "stdafx.h"
#include "libwraper.h"
#pragma comment(lib,"Advapi32.lib")

namespace DUI_API {


	class CMySvcMgr {
	public:
		CMySvcMgr() {
			m_schSCManager = NULL;
			m_schService = NULL;
			m_bInited = false;
			memset(&m_serviceNotify, 0, sizeof(m_serviceNotify));
		}
		virtual~CMySvcMgr() {
			UnInit();
		}
	public:
		BOOL RegisterService(const TCHAR* strSvcName, HWND hWnd);
		void UnInit() {
			if (m_bInited) {
				m_bInited = false;
				if (m_hExit != NULL) {
					SetEvent(m_hExit);
				}
				m_thr.Join();
				CloseHandle(m_hExit);
				m_hExit = NULL;
				if (m_schService != NULL) {
					CloseServiceHandle(m_schService);
					m_schService = NULL;
				}
				if (m_schSCManager != NULL) {
					CloseServiceHandle(m_schSCManager);
					m_schSCManager = NULL;
				}
			}

		}
		static VOID CALLBACK ServiceNotifyCallback(PVOID pParameter);
	protected:
		static DWORD thrFuc(DUI_THREAD::PMyThreadParam pMgr);
	private:
		SC_HANDLE m_schSCManager, m_schService;
		SERVICE_NOTIFY m_serviceNotify;
		DUI_THREAD::CMyThread m_thr;
		HANDLE		m_hExit;
		bool m_bInited;
		HWND m_hWnd;
	};


	VOID CALLBACK CMySvcMgr::ServiceNotifyCallback(PVOID pParameter) {

		SERVICE_NOTIFY* sNotify = static_cast<SERVICE_NOTIFY*>(pParameter);
		CMySvcMgr*pThis = (CMySvcMgr*)sNotify->pContext;
		::PostMessage(pThis->m_hWnd, WM_SVC_NOTIFY, sNotify->ServiceStatus.dwProcessId, sNotify->ServiceStatus.dwCurrentState);

		/*CString str;
		switch (sNotify->ServiceStatus.dwCurrentState)
		{
		case SERVICE_STOPPED: {
			str.Format(L"状态已停止");
			break;
		}
		case SERVICE_START_PENDING: {
			str.Format(L"状态启动中...");
			break;
		}
		case SERVICE_STOP_PENDING: {
			str.Format(L"状态停止中...");
			break;
		}
		case SERVICE_RUNNING: {
			str.Format(L"状态已启动");
			break;
		}
		case SERVICE_CONTINUE_PENDING: {
			str.Format(L"状态继续Pending");
			break;
		}
		case SERVICE_PAUSE_PENDING: {
			str.Format(L"状态暂停Pending");
			break;
		}
		case SERVICE_PAUSED: {
			str.Format(L"状态已暂停");
			break;
		}
		default:
			str.Format(L"未知");
			break;
		}

		::AfxMessageBox(str);*/
	}
	DWORD CMySvcMgr::thrFuc(DUI_THREAD::PMyThreadParam pMgr) {
		CMySvcMgr*pThis = (CMySvcMgr*)pMgr->lpClass;
		DWORD dwRet = 0;
		bool bQuit = false;

		DWORD dwNotifyMask = SERVICE_NOTIFY_START_PENDING |
			SERVICE_NOTIFY_RUNNING |
			SERVICE_NOTIFY_STOP_PENDING |
			SERVICE_NOTIFY_STOPPED;
		do {

			dwRet = NotifyServiceStatusChange(pThis->m_schService, dwNotifyMask, &pThis->m_serviceNotify);
			if (dwRet != ERROR_SUCCESS) {
				::PostMessage(pThis->m_hWnd, WM_SVC_NOTIFY, -1, dwRet);
				break;
			}
			while (::SleepEx(20, TRUE) != WAIT_IO_COMPLETION) {
				dwRet = ::WaitForSingleObject(pThis->m_hExit, 20);
				if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_FAILED || dwRet == WAIT_ABANDONED) {
					bQuit = true;
					break;
				}
			}
			if (bQuit) break;
		} while (1);
		return 0;
	}
	/*
	BOOL CMySvcMgr::OpenService(TCHAR* szError) {
		BOOL bSExsit = FALSE, bIsRun = FALSE;
		CMyTStringW    sPath(256);
		GetModuleFileName(NULL, sPath.c_str(), MAX_PATH);
		sPath = sPath.Left(sPath.ReverseFind(L'\\'));
		sPath += L"\\FaceCollector.exe";
		if (MyQueryService(SERVICENAME, &bSExsit, &bIsRun)) {
		if (!bSExsit) {
		BOOL bRet = SvcInstall(sPath.c_str(), SERVICENAME, SERVICENAME, _T("东华软件动态比对服务"), FALSE);
		if (bRet == FALSE) {
		strError = L"安装服务失败";
		return FALSE;
		}
		}
		}
		else {
		strError = L"查询服务失败";
		return FALSE;
		}
		return TRUE;
	}*/
	BOOL CMySvcMgr::RegisterService(const TCHAR* strSvcName, HWND hWnd) {
		//SERVICE_STATUS status;
		if (m_bInited) return TRUE;
		m_hWnd = hWnd;
		m_hExit = ::CreateEvent(NULL, TRUE, FALSE, _T(""));
		if (m_hExit == NULL) {
			return FALSE;
		}
		m_schSCManager = OpenSCManager(
			NULL,                    // local computer
			SERVICES_ACTIVE_DATABASE,                  // ServicesActive database 
			SC_MANAGER_ENUMERATE_SERVICE);  // full access rights 
		if (NULL == m_schSCManager) {
			CloseHandle(m_hExit);
			m_hExit = NULL;
			return FALSE;
		}
		m_schService = OpenService(m_schSCManager, strSvcName, SERVICE_QUERY_STATUS);    // 获得服务句柄
		if (m_schService == NULL) {
			CloseHandle(m_hExit);
			m_hExit = NULL;
			CloseServiceHandle(m_schSCManager);
			return FALSE;
		}
		m_serviceNotify.dwVersion = SERVICE_NOTIFY_STATUS_CHANGE;
		m_serviceNotify.pfnNotifyCallback = ServiceNotifyCallback;
		m_serviceNotify.pContext = this;
		m_serviceNotify.dwNotificationStatus = ERROR_SUCCESS;

		m_thr.Create(thrFuc, this, NULL);
		m_bInited = true;
		return TRUE;
	}





	/*********************************

	服务相关函数


	**********************************/




	DUI_FUNC_C_EXPORT HANDLE DUI_SvcRegister(LPCTSTR szSvcName, HWND hWnd)
	{
		CMySvcMgr*pSvc = new CMySvcMgr();
		if (pSvc == NULL) return NULL;
		if (!pSvc->RegisterService(szSvcName, hWnd)) {
			delete pSvc;
			return NULL;
		}
		return (HANDLE)pSvc;
	}

	DUI_FUNC_C_EXPORT void DUI_SvcDeRegister(HANDLE& hHandle)
	{
		CMySvcMgr*pSvc = (CMySvcMgr*)hHandle;
		if (pSvc != NULL) {
			pSvc->UnInit();
			try
			{
				delete pSvc;
				hHandle = NULL;
			}
			catch (...)
			{
				hHandle = NULL;
			}
		}
	}



	DUI_FUNC_C_EXPORT BOOL DUI_SvcExsit(LPCTSTR svcName) {
		SC_HANDLE schService, schSCManager;
		schSCManager = OpenSCManager(
			NULL,                    // local computer
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 
		if (NULL == schSCManager)
		{
			wprintf(_T("OpenSCManager %s failed %s"), svcName, GetErrorString().c_str());
			return FALSE;
		}
		// Get a handle to the SCM database. 
		schService = OpenService(schSCManager, svcName,
			SERVICE_ALL_ACCESS | SERVICE_START | SERVICE_STOP | DELETE);    // 获得服务句柄
		if (schService == NULL) {
			CloseServiceHandle(schSCManager);
			wprintf(_T("OpenService failed %s"), GetErrorString().c_str());
			return FALSE;
		}
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return TRUE;
	}

	//
	// Purpose: 
	//   Installs a service in the SCM database
	//
	// Parameters:
	//   None
	// 
	// Return value:
	//   None
	//

	DUI_FUNC_C_EXPORT  BOOL DUI_SvcInstall(LPCTSTR szPath,
		LPCTSTR svcName,
		LPCTSTR svcDisplayName,
		LPCTSTR szDescription,
		BOOL  bAutoStart,
		BOOL isDriver /*= FALSE */)
	{
		if (szPath == NULL) return FALSE;
		SC_HANDLE schService, schSCManager;
		schSCManager = OpenSCManager(
			NULL,                    // local computer
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 

		if (NULL == schSCManager)
		{
			wprintf(_T("OpenSCManager %s failed %s"), svcName, GetErrorString().c_str());
			return FALSE;
		}

		DWORD dwServiceType = isDriver ? SERVICE_KERNEL_DRIVER : SERVICE_WIN32_OWN_PROCESS;
		DWORD dwStartType = bAutoStart ? SERVICE_AUTO_START : SERVICE_DEMAND_START;
		// Create the service
		schService = CreateService(
			schSCManager,              // SCM database 
			svcName,                   // name of service 
			svcDisplayName,                   // service name to display 
			SERVICE_ALL_ACCESS,        // desired access 
			dwServiceType, // service type 
			dwStartType,      // start type 
			SERVICE_ERROR_NORMAL,      // error control type 
			szPath,                    // path to service's binary 
			NULL,                      // no load ordering group 
			NULL,                      // no tag identifier 
			NULL,                      // no dependencies 
			NULL,                      // LocalSystem account 
			NULL);                     // no password 
		if (schService == NULL)
		{
			wprintf(_T("CreateService  %s failed %s"), svcName, GetErrorString().c_str());
			CloseServiceHandle(schSCManager);
			return FALSE;
		}
		else {
			if (szDescription) {
				SERVICE_DESCRIPTION desc;
				desc.lpDescription = (LPWSTR)szDescription;
				ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &desc);
			}
			wprintf(_T("Service %s installed successfully"), svcName);
			if (StartService(schService, 0, NULL) == FALSE) {
				wprintf(_T("Service %s Start failed"), svcName);
			}
		}
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return TRUE;
	}


	DUI_FUNC_C_EXPORT  BOOL DUI_SvcStart(LPCTSTR svcName) {
		SC_HANDLE schSCManager, schService;
		SERVICE_STATUS status;
		schSCManager = OpenSCManager(
			NULL,                    // local computer
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 

		if (NULL == schSCManager)
		{
			wprintf(_T("OpenSCManager %s failed %s"), svcName, GetErrorString().c_str());
			return FALSE;
		}
		schService = OpenService(schSCManager, svcName,
			SERVICE_ALL_ACCESS | SERVICE_START | SERVICE_STOP | DELETE);    // 获得服务句柄
		if (schService == NULL) {
			CloseServiceHandle(schSCManager);
			wprintf(_T("OpenService failed (%d)"), GetLastError());
			return FALSE;
		}
		// 获得服务的当前状态
		if (!QueryServiceStatus(schService, &status)) {
			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
			wprintf(_T("QueryServiceStatus failed %s"), GetErrorString().c_str());
			return FALSE;
		}
		BOOL bRet = TRUE;
		if (status.dwCurrentState == SERVICE_STOPPED)   // 如果服务处于停止状态，则将其状态设置为启动
		{
			bRet = FALSE;
			if (StartService(schService, 0, NULL))   //启动服务
			{
				bRet = TRUE;
			}
		}
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		if (bRet) {
			wprintf(_T("%s starts successfully"), svcName);
			return TRUE;
		}
		wprintf(_T("%s starts meet a fail"), svcName);
		return FALSE;
	}
	DUI_FUNC_C_EXPORT  BOOL DUI_SvcStop(LPCTSTR svcName)
	{
		SC_HANDLE schSCManager, schService;
		SERVICE_STATUS status;
		schSCManager = OpenSCManager(
			NULL,                    // local computer
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 

		if (NULL == schSCManager)
		{
			wprintf(_T("OpenSCManager %s failed %s"), svcName, GetErrorString().c_str());
			return FALSE;
		}

		schService = OpenService(schSCManager, svcName,
			SERVICE_ALL_ACCESS | DELETE);    // 获得服务句柄
		if (schService == NULL) {
			CloseServiceHandle(schSCManager);
			wprintf(_T("OpenService failed %s"), GetErrorString().c_str());
			return FALSE;
		}
		// 获得服务的当前状态
		if (!QueryServiceStatus(schService, &status)) {
			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
			wprintf(_T("QueryServiceStatus failed %s"), GetErrorString().c_str());
			return FALSE;
		}
		if (status.dwCurrentState == SERVICE_RUNNING) {
			status.dwCurrentState = SERVICE_STOPPED;
			ControlService(schService, SERVICE_CONTROL_STOP, &status);
		}
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		wprintf(_T("Service stopped successfully"));
		return TRUE;

	}
	DUI_FUNC_C_EXPORT  BOOL DUI_SvcUnInstall(LPCTSTR svcName) {
		SC_HANDLE schSCManager, hSCService;
		SERVICE_STATUS curStatus;
		SERVICE_STATUS ctrlstatus;
		schSCManager = OpenSCManager(
			NULL,                    // local computer
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 

		if (NULL == schSCManager)
		{
			wprintf(_T("OpenSCManager %s failed %s"), svcName, GetErrorString().c_str());
			return FALSE;
		}
		hSCService = OpenService(schSCManager, svcName, SERVICE_ALL_ACCESS);
		if (hSCService == NULL) {
			CloseServiceHandle(schSCManager);
			wprintf(_T("OpenService %s failed %s"), svcName, GetErrorString().c_str());
			return FALSE;
		}
		if (!QueryServiceStatus(hSCService, &curStatus)) {
			CloseServiceHandle(hSCService);
			CloseServiceHandle(schSCManager);
			wprintf(_T("QueryServiceStatus %s failed %s"), svcName, GetErrorString().c_str());
			return FALSE;
		}

		if (curStatus.dwCurrentState != SERVICE_STOPPED) {
			if (!ControlService(hSCService, SERVICE_CONTROL_STOP, &ctrlstatus)) {
				CloseServiceHandle(hSCService);
				CloseServiceHandle(schSCManager);
				wprintf(_T("Stop %s failed: %d"), svcName, GetLastError());
				return FALSE;
			}
		}
		if (DeleteService(hSCService)) {
			wprintf(_T("Uninstall %s successfully"), svcName);
		}
		else {
			wprintf(_T("Uninstall %s failed:%d"), svcName, GetLastError());
		}
		CloseServiceHandle(hSCService);
		CloseServiceHandle(schSCManager);
		return TRUE;
	}

	DUI_FUNC_C_EXPORT  BOOL  DUI_SvcQueryConfig(LPCTSTR svcName, LPTSTR pathName) {
		BOOL bStatus = FALSE;
		SC_HANDLE schSCManager = NULL;
		SC_HANDLE schService = NULL;
		LPQUERY_SERVICE_CONFIG lpsc = NULL;
		DWORD dwBytesNeeded, cbBufSize, dwError;

		// Get a handle to the SCM database. 
		schSCManager = OpenSCManager(
			NULL,                    // local computer
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 

		if (NULL == schSCManager)
		{
			wprintf(L"OpenSCManager failed %s\n", GetErrorString().c_str());
			return FALSE;
		}

		// Get a handle to the service.

		schService = OpenService(
			schSCManager,          // SCM database 
			svcName,             // name of service 
			SERVICE_QUERY_CONFIG); // need query config access 

		if (schService == NULL)
		{
			wprintf(L"OpenService failed %s\n", GetErrorString().c_str());
			CloseServiceHandle(schSCManager);
			return FALSE;
		}

		// Get the configuration information.

		if (!QueryServiceConfig(
			schService,
			NULL,
			0,
			&dwBytesNeeded))
		{
			dwError = GetLastError();
			if (ERROR_INSUFFICIENT_BUFFER == dwError)
			{
				cbBufSize = dwBytesNeeded;
				lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, cbBufSize);
			}
			else
			{
				wprintf(L"QueryServiceConfig failed %s", GetErrorString(dwError).c_str());
				goto cleanup;
			}
		}

		if (!QueryServiceConfig(
			schService,
			lpsc,
			cbBufSize,
			&dwBytesNeeded))
		{
			wprintf(L"QueryServiceConfig failed %s", GetErrorString(dwError).c_str());
			goto cleanup;
		}
		bStatus = TRUE;
		try {
			_tcscpy(pathName, lpsc->lpBinaryPathName);
		}
		catch (...) {
			pathName[0] = 0;
			bStatus = FALSE;
		}
		LocalFree(lpsc);
	cleanup:
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return bStatus;
	}

}