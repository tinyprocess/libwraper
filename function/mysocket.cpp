#include "stdafx.h"
#include "libwraper.h"

namespace DUI_API {

	CMyTcpClient::CMyTcpClient(char* strIp/* = NULL*/, int port/* = 0*/) {
		m_socket = NULL;
		m_state = cs_state_stoped;
		if (strIp) {
			strcpy_s(m_svcIp, min(strlen(strIp) + 1, 16), strIp);
		}
		else {
			m_svcIp[0] = 0;
		}
		m_port = port;
	}
	CMyTcpClient::~CMyTcpClient() {
		UnInit();
	}

	void CMyTcpClient::SetServer(const char* strIp, int port) {
		if (m_state == cs_state_stoped) {
			strcpy_s(m_svcIp, min(strlen(strIp) + 1, 16), strIp);
			m_port = port;
		}
	}
	bool CMyTcpClient::IsConnected() {
		return (m_state == cs_state_connected);
	}
	int CMyTcpClient::Send(char*cBuffer, int length) {
		int len = -1;
		if (m_state == cs_state_connected) {
			len = send(m_socket, cBuffer, length, 0);
		}
		return len;
	}


	int CMyTcpClient::Recv(char*cBuffer, int length) {
		int recvLength = -1;
		if (m_state == cs_state_connected) {
			recvLength = recv(m_socket, cBuffer, length, 0);
		}
		return recvLength;
	}
	bool CMyTcpClient::SetSockOpt(__in int level,
		__in int optname,
		__in_bcount_opt(optlen) const char FAR * optval,
		__in int optlen) {
		bool bRet = false;
		if (m_state != cs_state_stoped) {
			bRet = (setsockopt(m_socket, level, optname, optval, optlen) != SOCKET_ERROR);
		}
		return bRet;
	}
	bool CMyTcpClient::Init() {//接收超时 
		if (m_state == cs_state_stoped) {
			m_socket = socket(AF_INET, SOCK_STREAM, 0);
			if (m_socket == INVALID_SOCKET || m_socket == NULL) {
				return false;
			}
			//int nNetTimeout = 2000
			//SetSockOpt(SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
			m_state = cs_state_inited;
			return true;
		}
		return true;
	}
	void CMyTcpClient::UnInit() {
		if (m_state != cs_state_stoped) {
			try {
				closesocket(m_socket);
			}
			catch (...) {
			}
			m_socket = NULL;
		}
		m_state = cs_state_stoped;
	}
	bool CMyTcpClient::Connect() {
		bool bRet = false;
		if (m_state == cs_state_connected) {
			bRet = true;
		}
		else if (m_state == cs_state_stoped) {
			Init();
		}
		else if (m_state == cs_state_inited) {
			//填充服务器套接字地址  
			sockaddr_in saServer = { 0 };
			saServer.sin_family = AF_INET;
			saServer.sin_addr.s_addr = ::inet_addr(m_svcIp);
			saServer.sin_port = htons(m_port);
			if (connect(m_socket, (sockaddr*)&saServer, sizeof(sockaddr_in)) == SOCKET_ERROR) {
				return false;
			}
			m_state = cs_state_connected;
			bRet = true;
		}
		return bRet;
	}














	
/*
typedef struct _SockTimerCb {
		virtual void SockTmConnected(LPVOID lParam) {};
		virtual void SockTmDisConnect(LPVOID lParam) {};
		virtual void SockTmConnectFailed(LPVOID lParam) {};
		virtual void SockTmThreadEnd(LPVOID lParam) {};
	}SockTimerCb, *PSockTimerCb;

	class CDetectSock{
	public:
		CDetectSock() {
			m_bQuit = false;
			m_bInited = false;
		}
		virtual~CDetectSock() {
			UnInit();
		}
	public:
		bool Init(const char* strIp, int port, PSockTimerCb notice) {
			bool bRet = false;
			if (!m_bInited) {
				if (notice == NULL) return false;
				m_tcpSock.SetServer(strIp, port);
				m_notice = notice;
				bRet = m_thrSock.Create(Thr_socketMain, this, NULL, 30);
				m_bInited = bRet;
			}
			return bRet;
		}
		void UnInit() {
			m_bQuit = true;
			if (m_bInited) {
				m_thrSock.SetExitSignal();
				m_tcpSock.Close();
				m_thrSock.Join();
				m_bInited = false;
			}
		}
		bool IsInited() {
			return m_bInited;
		}
	protected:
		static DWORD	Thr_socketMain(DUI_THREAD::PMyThreadParam pMgr) {//定时检测相机是否离线
			CDetectSock* pThis = (CDetectSock*)pMgr->lpClass;
			HANDLE &hExitEvent = pMgr->hExit;
			DWORD dwWait = 0;
			char cBuffer[128] = { 0 };
			int recvLength = 0;
			DWORD iError = 0;
			while (1) {
				dwWait = ::WaitForSingleObject(hExitEvent, pMgr->waitTime);
				if (dwWait == WAIT_OBJECT_0 || dwWait == WAIT_FAILED) {
					break;
				}
				if (!pThis->m_tcpSock.InitSocket()) {
					continue;
				}
				if (!pThis->m_tcpSock.Connect()) {//连接失败
					pThis->m_notice->SockTmConnectFailed(pThis);
					pMgr->waitTime = 30 * 1000;
					continue;
				}
				pThis->m_notice->SockTmConnected(pThis);//连接已经成功
				pMgr->waitTime = 30;
				while (1) {
					recvLength = pThis->m_tcpSock.Recv(cBuffer, 128);
					if (recvLength < 0) {
						iError = ::GetLastError();
						//EAGAIN
						if (iError == 10060 ) {//接收超时 则继续 || iError == EWOULDBLOCK
							continue;
						}
						//其他错误则 重置 socket
						pThis->m_tcpSock.Close();
						pThis->m_notice->SockTmDisConnect(pThis);//客户端主动断开
						break;
					}
				}
			}
			pThis->m_notice->SockTmThreadEnd(pThis);
			return 0;
		}
	private:
		CMyTcpClient m_tcpSock;
		DUI_THREAD::CMyThread m_thrSock;// 启动 socket线程
		bool m_bQuit;
		bool m_bInited;
		PSockTimerCb m_notice;
	};

*/
}