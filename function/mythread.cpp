#include "stdafx.h"
#include "libwraper.h"


namespace DUI_API {
	namespace DUI_THREAD {

		CMyThread::CMyThread() {
			m_hThread = NULL;
			m_threadId = 0;
			m_cb = NULL;
			m_param.pThread = NULL;
			m_param.lpClass = NULL;
			m_param.lpUserData = NULL;
			m_param.bQuit = false;
			m_state = st_stopped;

		}
		CMyThread::~CMyThread() {
			if (m_state != st_stopped) {
				Join(200);
			}
		}
		PMyThreadParam CMyThread::GetThreadParam() {
			return &m_param;
		}
		LPVOID CMyThread::GetUserData() {
			return m_param.lpUserData;
		}
		void CMyThread::SetUserData(LPVOID lpUserData) {
			m_param.lpUserData = lpUserData;
		}
		bool CMyThread::IsWorked() {
			return (m_state != st_stopped);
		}
		/*创建线程*/
		bool CMyThread::Create(PMyThreadCallBack cb, LPVOID lpClass/* = NULL*/, LPVOID lpUserData /*= NULL*/) {
			if (m_state == st_stopped) {
				m_cb = cb;
				m_param.pThread = this;
				m_param.lpClass = lpClass;
				m_param.lpUserData = lpUserData;
				m_param.bQuit = false;

				m_hThread = NULL;
				m_threadId = 0;
				return Create();
			}
			return false;
		}
		bool CMyThread::Create() {
			if (m_state == st_stopped) {
				m_hThread = ::CreateThread(NULL, 0, mainWork, &m_param, 0, &m_threadId);
				if (m_hThread == NULL) {
					return false;
				}
				m_state = st_work;
				return true;
			}
			return false;
		}
		bool CMyThread::SetPriority(DWORD fdwPriority /*= THREAD_PRIORITY_NORMAL*/) {
			bool bRet = false;
			if (m_state == st_work) {
				if (fdwPriority != THREAD_PRIORITY_NORMAL) {
					bRet = ::SetThreadPriority(m_hThread, fdwPriority) == TRUE;
				}
			}
			return bRet;
		}
		/*恢复线程*/
		bool CMyThread::Resume() {
			if (m_state == st_work) {
				m_state = st_work;
				return (ResumeThread(m_hThread) != -1);
			}
			return false;
		}

		/*阻塞线程*/
		bool CMyThread::Suspend() {
			if (m_state == st_pause) {
				m_state = st_pause;
				return (SuspendThread(m_hThread) != -1);
			}
			return false;

		}
		/*自动结束*/
		bool CMyThread::Join(DWORD dwWaitTime/* = INFINITE*/) {//直到线程结束
			bool bRet = false;
			if (m_state == st_work) {
				m_param.bQuit = true;
				WaitForSingleObject(m_hThread, dwWaitTime);
				m_state = st_stopped;
				bRet = true;
			}
			else if (m_state == st_pause) {
				Resume();
				Join();
			}
			m_hThread = NULL;
			m_state = st_stopped;
			m_threadId = 0;
			m_cb = NULL;
			return bRet;
		}
		/*强制结束线程 不建议使用*/
		void CMyThread::TerminalThread() {
			if (m_state != st_stopped) {
				m_param.bQuit = true;
				DWORD dwCode = 0;
				::GetExitCodeThread(m_hThread, &dwCode);
				::TerminateThread(m_hThread, dwCode);
			}
			m_hThread = NULL;
			m_state = st_stopped;
			m_threadId = 0;
			m_cb = NULL;
		}
		void CMyThread::SetExit() {
			m_param.bQuit = true;
		}
		 DWORD WINAPI CMyThread::mainWork(LPVOID lParam) {
			THREAD_LOAD_PARAM;
			pMgr->pThread->m_cb(pMgr);
			/*

			DWORD dwWait = ::WaitForSingleObject(pMgr->pThread->m_hExitEvent, pMgr->pThread->m_waitTime);
			if (dwWait == WAIT_OBJECT_0 || dwWait == WAIT_FAILED) {
			return 0;
			}

			*/
			try {
				pMgr->pThread->m_state = st_stopped;
			}
			catch (...) {
			}
			CloseHandle(pMgr->pThread->m_hThread);
			return 0;
		}

		 /********************************
		 
		 CMyThreadGroup

		 ***************************************/

		 CMyThreadGroup::CMyThreadGroup() {
			 m_thList.clear();
		 }
		 CMyThreadGroup::~CMyThreadGroup() {
			 Join(200);
		 }

		size_t CMyThreadGroup::size() {
			return m_thList.size();
		}
		bool CMyThreadGroup::AddThread(PMyThreadCallBack cb, LPVOID lpClass/* = NULL*/, LPVOID lpUserData /*= NULL*/) {
			std::shared_ptr<CMyThread> thr = std::make_shared<CMyThread>();
			if (thr->Create(cb, lpClass, lpUserData)) {
				m_thList.push_back(thr);
				return true;
			}
			thr = NULL;
			return false;
		}
		void CMyThreadGroup::Join(DWORD dwWaitTime/* = INFINITE*/) {
			for (int i = 0; i < (int)m_thList.size(); i++) {
				m_thList[i]->SetExit();
			}
			iterator it;
			while ((it = m_thList.begin()) != m_thList.end()) {
				(*it)->Join(dwWaitTime);
				(*it) = NULL;
				it = m_thList.erase(it);
			}
		}

		PMyThreadParam CMyThreadGroup::GetThreadParam(int iIndex) {
			return m_thList[iIndex]->GetThreadParam();
		}
		LPVOID CMyThreadGroup::GetUserData(int iIndex) {
			return m_thList[iIndex]->GetUserData();
		}
		CMyThread* CMyThreadGroup::operator [](int iIndex) {
			return m_thList[iIndex].get();
		}
	}
}