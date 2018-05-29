#pragma once

#include <algorithm>
#include <list>
#include <vector>
#include <memory>
#include <map>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")


//#ifndef  DUI_FUNC_CXX_EXPORT
//#define  DUI_FUNC_C_EXPORT  extern "C" __declspec(dllimport)
//#define  DUI_FUNC_CXX_EXPORT  __declspec(dllimport)
//#define  DUI_CLASS_EXPORT __declspec(dllimport)
//#endif

#define  DUI_FUNC_C_EXPORT  
#define  DUI_FUNC_CXX_EXPORT 
#define  DUI_CLASS_EXPORT 


namespace DUI_API {
#define	SAFE_FREE(x) {if(x){ free(x);x = NULL;}}
#define	SAFE_DELETE(x) {if(x){ delete x;x = NULL;}}
#define	SAFE_DELETE_ARRAY(x) {if(x != NULL){ delete[] x;x = NULL;}}
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))



	namespace REG {
		DUI_FUNC_C_EXPORT HKEY OpenKey(HKEY hRootKey, LPCTSTR strKey);

		DUI_FUNC_C_EXPORT  void CloseKey(HKEY& hKey);

		DUI_FUNC_C_EXPORT  void SetVal(HKEY hKey, LPCTSTR lpValue, DWORD data);

		DUI_FUNC_C_EXPORT  DWORD GetVal(HKEY hKey, LPCTSTR lpValue);

		DUI_FUNC_C_EXPORT  void SetString(HKEY hKey, LPCTSTR lpValue, TCHAR* strBuff, int bufSize);

		DUI_FUNC_C_EXPORT  DWORD GetString(HKEY hKey, LPCTSTR lpValue, TCHAR* strBuff, int bufSize);
	}



#define WM_SVC_NOTIFY	(WM_USER + 22)
	DUI_FUNC_C_EXPORT  HANDLE		DUI_SvcRegister(LPCTSTR szSvcName, HWND hWnd);
	DUI_FUNC_C_EXPORT  void			DUI_SvcDeRegister(HANDLE& hHandle);
	DUI_FUNC_C_EXPORT  BOOL			DUI_SvcExsit(LPCTSTR svcName);
	DUI_FUNC_C_EXPORT  BOOL			DUI_SvcInstall(LPCTSTR szPath,
		LPCTSTR svcName,
		LPCTSTR svcDisplayName,
		LPCTSTR szDescription,
		BOOL  bAutoStart,
		BOOL isDriver = FALSE);
	DUI_FUNC_C_EXPORT  BOOL		DUI_SvcUnInstall(LPCTSTR svcName);
	DUI_FUNC_C_EXPORT  BOOL		DUI_SvcQueryConfig(LPCTSTR svcName, LPTSTR pathName);
	DUI_FUNC_C_EXPORT  BOOL		DUI_SvcStart(LPCTSTR svcName);
	DUI_FUNC_C_EXPORT  BOOL		DUI_SvcStop(LPCTSTR svcName);

	namespace DUI_ENCODE {
		///MD5的结果数据长度
#define ZEN_MD4_HASH_SIZE   16
#define ZEN_MD5_HASH_SIZE   16
		///SHA1的结果数据长度
#define ZEN_SHA1_HASH_SIZE  20



		DUI_FUNC_C_EXPORT	unsigned char *md4_(const unsigned char *buf,
			size_t size,
			unsigned char result[ZEN_MD4_HASH_SIZE]);

		/*!
		@brief      求某个内存块的MD5，
		@return     unsigned char* 返回的的结果，
		@param[in]  buf    求MD5的内存BUFFER指针
		@param[in]  size   BUFFER长度
		@param[out] result 结果
		*/
		DUI_FUNC_C_EXPORT unsigned char *md5_(const unsigned char *buf,
			size_t size,
			unsigned char result[ZEN_MD5_HASH_SIZE]);


		/*!
		@brief      求内存块BUFFER的SHA1值
		@return     unsigned char* 返回的的结果
		@param[in]  buf    求SHA1的内存BUFFER指针
		@param[in]  size   BUFFER长度
		@param[out] result 结果
		*/
		DUI_FUNC_C_EXPORT unsigned char *sha1_(const unsigned char *buf,
			size_t size,
			unsigned char result[ZEN_SHA1_HASH_SIZE]);



		DUI_FUNC_C_EXPORT DWORD des_getsize(DWORD dataSize);
		DUI_FUNC_C_EXPORT DWORD des_encrypt(BYTE* buffer, DWORD dataLength, DWORD bufferLength, TCHAR key[8]);
		DUI_FUNC_C_EXPORT DWORD des_decrypt(BYTE* buffer, DWORD bufferLength, TCHAR key[8]);
	}


	template<typename T>
	T* __strcpy(T*_Dst, const T*_Src);

	template<typename T>
	T* __strncpy(T*_Dst, const T*_Src, int Count);

	template<typename T>
	T* __strcpy_s(T*_Dst, int cchMax, const T*_Src);

	template<typename T>
	int __strlen(const T*_Src);

	template<typename T>
	T* __strcat(T*_Dst, const T*_Src);

	template<typename T>
	bool __strequal(const T*_Dst, const T*_Src);

	template<typename T>
	void __strset(T*_Src, T &value);

	template<typename T>
	int __strpos(const T*_Src, const T*_Sub);

	template<typename T>
	int __strrpos(const T*_Src, const T*_Sub);

	template<typename T>
	int __stripos(const T* __Src, const T* __Sub);

	template<typename T>
	int __strripos(const T* __Src, const T* __Sub);

	template<typename T>
	int __strupper(T* _Src);

	template<typename T>
	int __strlower(T* _Src);

	template<typename T>
	T* __strltrim(T* _Src);

	template<typename T>
	T* __strrtrim(T* _Src);

	template<typename T>
	T* __strtrim(T* _Src);

	//template<typename T>
	//std::vector<int> __strfindpos(T* _Src, const T* _Sub);

	template<typename T>
	bool __strreplace(T* _Src, int len, const T* _Sub, int s_len, const T* _To, int t_len);

	template<typename T>
	T * __strrev(T* _Src, int len = -1);

	template<typename T>
	int __strpos(const T* __Src, const T& x);

	template<typename T>
	int __strrpos(const T* __Src, const T& x);

	template<typename T>
	T* __strchr(const T* src, const T x);

	template<typename T>
	T* __strrchr(const T* src, const T x);



	template<class _TT, int _INIT_ALLOC_LENGTH_ = 256>
	class DUI_CLASS_EXPORT CMyBuffer {
	public:
		CMyBuffer();
		CMyBuffer(int _init_size);
		CMyBuffer(const CMyBuffer<_TT>& myBuffer);
		CMyBuffer(_TT* szBuf, int _iLength);
		virtual~CMyBuffer();
	public:
		CMyBuffer<_TT>& operator=(CMyBuffer<_TT>& myBuffer);
		CMyBuffer<_TT>& operator+=(CMyBuffer<_TT>& myBuffer);
	public:
		bool Assign(const _TT* szBuf, int _iLength);
		bool Append(const _TT* szBuf, int _iLength);
		bool valid();
		bool empty();
		int  length();
		int  GetAllocLength();
		bool SetLength(int iLength);
		_TT* buf();
		void free();
	protected:
		bool xmalloc(int length);
		void xfree();
	private:
		_TT * m_lpBuf;
		int m_length;//存储长度 不是字节
		int m_allocLength;//存储长度 不是字节
	};

	typedef	CMyBuffer<unsigned char, 256>	CMyByteBuffer;

	class CMyStringW;
	class CMyStringA;


	template<class _TypeName>
	class DUI_CLASS_EXPORT CMyStringT {
#define __INIT_ALLOC_LENGTH__	64
		friend class CMyStringW;
		friend class CMyStringA;
	public:
		CMyStringT();
		CMyStringT(int length);
		CMyStringT(const _TypeName* cStr, int length = -1);
		CMyStringT(const CMyStringT<_TypeName>& myString);
		virtual ~CMyStringT();
	public:
		_TypeName * c_str() const;
		const _TypeName* GetString();
		_TypeName* GetBuffer();
		CMyStringT<_TypeName>& operator=(const _TypeName* cStr);
		CMyStringT<_TypeName>& operator=(CMyStringT<_TypeName>& myString);
		CMyStringT<_TypeName>& operator+=(const _TypeName* cStr);
		CMyStringT<_TypeName>& operator+=(CMyStringT<_TypeName>& myString);
		_TypeName& operator[](int i);
	public:
		int ReleaseBuffer(int minBufferLength = -1);
		void SetBufferLength(int iLength);
		CMyStringT<_TypeName> Left(int iLength);
		CMyStringT<_TypeName> Right(int iLength);
		CMyStringT<_TypeName> Mid(int pos, int iLength = -1);
		int Find(_TypeName c);
		int ReverseFind(_TypeName c);
	public:
		int Compare(const _TypeName* str) const;
		bool operator == (const _TypeName* str) const { return (Compare(str) == 0); };
		bool operator != (const _TypeName* str) const { return (Compare(str) != 0); };
		bool operator <= (const _TypeName* str) const { return (Compare(str) <= 0); };
		bool operator <  (const _TypeName* str) const { return (Compare(str) <  0); };
		bool operator >= (const _TypeName* str) const { return (Compare(str) >= 0); };
		bool operator >  (const _TypeName* str) const { return (Compare(str) >  0); };
	public:
		void clear();
		int  GetLength();
		int  length();
		int  GetAllocLength();
		bool empty();
		bool equal(CMyStringT<_TypeName>& _Right);
		bool equal(const _TypeName * pStr);
		bool Assign(const _TypeName* cStr, int length);
		bool Append(const _TypeName* cStr, int length);
	protected:
		bool xmalloc(int length);
		void xfree();
	protected:
		_TypeName * m_pcStr;
		_TypeName  m_buffer[__INIT_ALLOC_LENGTH__];
		int m_allocLength;
	};



	class DUI_CLASS_EXPORT CMyStringA : public CMyStringT<char> {
	public:
		CMyStringA();
		CMyStringA(int length);
		CMyStringA(const char* cStr, int length = -1);
		CMyStringA(const CMyStringA& myString);
		CMyStringA(const CMyStringT<char>& myString);
	public:
		CMyStringA & operator=(const char* cStr);
		CMyStringA& operator=(CMyStringT<char>myString);
		CMyStringA& operator=(CMyStringA& myString);

		CMyStringA& operator+=(const char* cStr);
		CMyStringA& operator+=(CMyStringT<char>& myString);
		CMyStringA& operator+=(CMyStringA& myString);
		CMyStringA operator+(const CMyStringA& src) const;
		CMyStringA operator+(LPCSTR lpStr) const;


		operator LPCSTR()  const;
		operator LPSTR()  const;

		void MakeUpper();
		void MakeLower();
	public:
		CMyStringW toWideString();
		int __cdecl FormatV(const char* pstrFormat, va_list valist);
		int __cdecl Format(const char* pstrFormat, ...);
		int __cdecl AppendFormat(const char* pstrFormat, ...);
	};

	class DUI_CLASS_EXPORT CMyStringW : public CMyStringT<wchar_t> {
	public:
		CMyStringW();
		CMyStringW(int length);
		CMyStringW(const wchar_t* cStr, int length = -1);
		CMyStringW(const CMyStringW& myString);
		CMyStringW(const CMyStringT<wchar_t>& myString);
	public:
		CMyStringW & operator=(const wchar_t* cStr);
		CMyStringW& operator=(CMyStringT<wchar_t>& myString);
		CMyStringW& operator=(CMyStringW& myString);

		CMyStringW& operator+=(const wchar_t* cStr);
		CMyStringW& operator+=(CMyStringT<wchar_t>& myString);
		CMyStringW& operator+=(CMyStringW& myString);

		CMyStringW operator+(const CMyStringW& src) const;
		CMyStringW operator+(LPCWSTR lpStr) const;

		operator LPCWSTR()  const;
		operator LPWSTR()  const;

		void MakeUpper();
		void MakeLower();
	public:
		CMyStringA toMutiString();
		int __cdecl FormatV(const wchar_t* pstrFormat, va_list valist);
		int __cdecl Format(const wchar_t* pstrFormat, ...);
		int __cdecl AppendFormat(const wchar_t* pstrFormat, ...);
	};

#ifdef _UNICODE
#define	CMyString		CMyStringW
#else
#define	CMyString		CMyStringA
#endif // _UNICODE



	DUI_FUNC_C_EXPORT bool GetAllIpAddress(std::vector<CMyStringA>&ipList);
	DUI_FUNC_C_EXPORT bool GetNetInterface(int IfIndex, char *strIp, u_char* mac = NULL);
	DUI_FUNC_CXX_EXPORT CMyString MyGetFlowSize(__int64 size);

	DUI_FUNC_CXX_EXPORT std::wstring MutiToWideString(LPCSTR data);
	DUI_FUNC_CXX_EXPORT std::string  WideToMutiString(LPCWSTR data);
	DUI_FUNC_CXX_EXPORT std::string  MutiToUtf8String(LPCSTR data);
	DUI_FUNC_CXX_EXPORT std::string  WideToUtf8String(LPCWSTR data);
	DUI_FUNC_CXX_EXPORT std::string Utf8ToMutiString(LPCSTR data);
	DUI_FUNC_CXX_EXPORT std::wstring Utf8ToWideString(LPCSTR data);

	DUI_FUNC_C_EXPORT int gbk_strlen(const char* str);
	DUI_FUNC_C_EXPORT int gbk_getstr(char* str, int _limit);
	

	DUI_FUNC_CXX_EXPORT void GetFileString(unsigned char*buf, int iSize, CMyStringA& strText);


	DUI_FUNC_C_EXPORT BOOL CreateNewProcess(LPCTSTR pszExeName, LPTSTR iCmd = NULL, bool isShow = true);
	DUI_FUNC_C_EXPORT INT StartProcess(TCHAR *Exe, TCHAR *lParams, DWORD timeOut = INFINITE, bool show = true);

	DUI_FUNC_C_EXPORT unsigned char char2byte(char c);
	DUI_FUNC_C_EXPORT int Str2Bytes(char *src, int len, unsigned char *dst);

	DUI_FUNC_C_EXPORT  size_t StringToInts(const char* _str, const char* _strSplict, std::vector<int>* _vecInt);
	DUI_FUNC_C_EXPORT  size_t StringToLongs(const char* _str, const char* _strSplict, std::vector<long>* _vecInt);
	DUI_FUNC_C_EXPORT  size_t StringToLongLongs(const char* _str, const char* _strSplict, std::vector<__int64>* _vecInt);

	DUI_FUNC_CXX_EXPORT  CMyString  GetErrorString(DWORD dwError = ::GetLastError());
	DUI_FUNC_CXX_EXPORT  CMyString  StringFormat(LPCTSTR pstrFormat, ...);
	DUI_FUNC_CXX_EXPORT  CMyStringA  StringFormatA(LPCSTR pstrFormat, ...);
	DUI_FUNC_CXX_EXPORT  CMyStringW  StringFormatW(LPCWSTR pstrFormat, ...);


	class DUI_CLASS_EXPORT CMyCriSec {
	private:
		CRITICAL_SECTION m_cs;
	public:
		CMyCriSec();
		virtual ~CMyCriSec();
		void EnterCriSec();
		void LeaveCriSec();
	};

	class DUI_CLASS_EXPORT CMyLockObj {
	public:
		CMyLockObj();
		virtual~CMyLockObj();
		virtual void Close();
		void SetHandle(HANDLE hHandle);
		HANDLE & GetHanlde();
		bool Valid();
		virtual DWORD Lock(DWORD dwWait = INFINITE) = 0;
		virtual void UnLock() = 0;
	protected:
		HANDLE m_handle;
	};
	class DUI_CLASS_EXPORT CMyMutex {
	private:
		HANDLE m_lpMutex;
	public:
		CMyMutex() :m_lpMutex(NULL) {}
		~CMyMutex() {
			Close();
		}
	public:
		bool Create() {
			Close();
			m_lpMutex = CreateMutex(NULL, FALSE, NULL);
			return m_lpMutex != NULL;
		}
		void Close() {
			if (m_lpMutex != NULL) {
				CloseHandle(m_lpMutex);
				m_lpMutex = NULL;
			}
		}
		virtual DWORD Lock(DWORD dwWait = INFINITE) {
			DWORD dw = ::WaitForSingleObject(m_lpMutex, dwWait);
			return dw;
		}
		void UnLock() {
			ReleaseMutex(m_lpMutex);
		}
	};

	class DUI_CLASS_EXPORT CMyEvent : public CMyLockObj {
	public:
		CMyEvent() :CMyLockObj() {
		}
		virtual~CMyEvent() {
			Close();
		}
		virtual bool Create(TCHAR* szName = NULL, BOOL bManualReset = FALSE, BOOL bInitialState = TRUE) {
			Close();
			m_handle = ::CreateEvent(NULL, bManualReset, bInitialState, szName);
			return m_handle != NULL;
		}
		virtual DWORD Lock(DWORD dwWait = INFINITE) {
			DWORD dw = ::WaitForSingleObject(m_handle, dwWait);
			return dw;
		}
		virtual void UnLock() {
			::SetEvent(m_handle);
		}
	};


	class DUI_CLASS_EXPORT CMyFileMgr {
	public:
		CMyFileMgr();
		virtual ~CMyFileMgr();
	public:
		void Close();
		bool Open(LPCTSTR szFileName, DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE);
		DWORD Write(PBYTE bytes, DWORD dwSize);
		DWORD Read(PBYTE bytes, DWORD dwSize);
		HANDLE GetHandle();
		DWORD  GetFileLength();
	private:
		HANDLE m_hFile;
	};


	template<class _Ty,
		class _Alloc = std::allocator<_Ty>>
		class  CMyList_s : public std::list<_Ty, _Alloc>, public CMyCriSec {
		public:
			typedef typename std::list<_Ty, _Alloc>::iterator iterator;
			CMyList_s() : std::list<_Ty, _Alloc>(), CMyCriSec() {

			}
			~CMyList_s() {
				clear();
			}
		public:
			void push_back(_Ty& pData) {
				EnterCriSec();
				__super::push_back(pData);
				LeaveCriSec();
			}


			void push_front(_Ty& pData) {
				EnterCriSec();
				__super::push_front(pData);
				LeaveCriSec();
			}


			void pop_front() {
				EnterCriSec();
				__super::pop_front();
				LeaveCriSec();
			}


			void pop_back() {
				EnterCriSec();
				__super::pop_back();
				LeaveCriSec();
			}


			/*仅仅 出链表 不删除内容*/

			bool pop_front(_Ty &p) {
				EnterCriSec();
				if (!__super::empty()) {
					p = (*__super::begin());
					__super::pop_front();
					LeaveCriSec();
					return true;
				}
				LeaveCriSec();
				return false;
			}


			/*仅仅 出链表 不删除内容*/

			bool pop_back(_Ty& p) {
				EnterCriSec();
				if (!__super::empty()) {
					p = *(--__super::end());
					__super::pop_back();
					LeaveCriSec();
					return true;
				}
				LeaveCriSec();
				return false;
			}



			void clear() {
				EnterCriSec();
				__super::clear();
				LeaveCriSec();
			}

			//头删法 清空链表 并删除内存

			void clearAndDelete() {
				iterator it;
				EnterCriSec();
				while ((it = __super::begin()) != __super::end())
				{
					SAFE_DELETE((*it));
					__super::pop_front();
				}
				LeaveCriSec();
			}




			//头删法 清空链表 并删除内存

			void clearAndFree() {
				iterator it;
				EnterCriSec();
				while ((it = __super::begin()) != __super::end())
				{
					SAFE_FREE((*it));
					__super::pop_front();
				}
				LeaveCriSec();
			}



			size_t size() {
				size_t size = 0;
				EnterCriSec();
				size = __super::size();
				LeaveCriSec();
				return size;
			}



			bool Find(const _Ty& _val, iterator &it) {
				bool found = false;
				EnterCriSec();
				it = std::find(__super::begin(), __super::end(), _val);
				if (it != __super::end()) {
					found = true;
				}
				LeaveCriSec();
				return found;
			}



			void Remove(iterator& it) {
				EnterCriSec();
				__super::erase(it);
				LeaveCriSec();
			}


			void RemoveAndFree(iterator& it) {
				EnterCriSec();
				if ((*it) != NULL) {
					(*it) = NULL;
					free(*it);
				}
				__super::erase(it);
				LeaveCriSec();
			}


			void RemoveAndDelete(iterator& it) {
				EnterCriSec();
				if ((*it) != NULL) {
					(*it) = NULL;
					delete(*it);
				}
				__super::erase(it);
				LeaveCriSec();
			}


			bool Remove(const _Ty& val) {
				iterator it;
				bool bFound = false;
				if (bFound = Find(val, it)) {
					EnterCriSec();
					__super::erase(it);
					LeaveCriSec();
				}
				return bFound;
			}



			void RemoveAndFree(const _Ty& val) {
				iterator it;
				bool bFound = false;
				if (bFound = Find(val, it)) {
					EnterCriSec();
					if ((*it) != NULL) {
						(*it) = NULL;
						free(*it);
					}
					__super::erase(it);
					LeaveCriSec();
				}
				return bFound;
			}



			void RemoveAndDelete(const _Ty& val) {
				iterator it;
				bool bFound = false;
				if (bFound = Find(val, it)) {
					EnterCriSec();
					if ((*it) != NULL) {
						(*it) = NULL;
						delete(*it);
					}
					__super::erase(it);
					LeaveCriSec();
				}
				return bFound;
			}



			/*判断是否链表为空*/

			bool empty() {
				bool bRet = false;
				EnterCriSec();
				bRet = __super::empty();
				LeaveCriSec();
				return bRet;
			}



			void reverse() {
				EnterCriSec();
				__super::reverse();
				LeaveCriSec();
			}


	};


	template<class _Ty,
		class _Alloc = std::allocator<_Ty>>
		class CMyVector_s :
		public std::vector<_Ty, _Alloc>, public CMyCriSec {
		public:
			typedef typename std::vector<_Ty, _Alloc>::iterator iterator;

			CMyVector_s() :std::vector<_Ty, _Alloc>(), CMyCriSec() { }


			virtual~CMyVector_s() {
				clear();
			}
		public:

			_Ty & operator[](int iIndex) {
				_Ty* t;
				EnterCriSec();
				t = &__super::at(iIndex);
				LeaveCriSec();
				return *t;
			}

			void push_back(_Ty& pData) {
				EnterCriSec();
				__super::push_back(pData);
				LeaveCriSec();
			}


			void push_front(_Ty& pData) {
				EnterCriSec();
				__super::push_front(pData);
				LeaveCriSec();
			}



			void pop_front() {
				EnterCriSec();
				__super::pop_front();
				LeaveCriSec();
			}



			void pop_back() {
				EnterCriSec();
				__super::pop_back();
				LeaveCriSec();
			}


			/*仅仅 出链表 不删除内容*/

			bool pop_front(_Ty &p) {
				EnterCriSec();
				if (!__super::empty()) {
					p = (*__super::begin());
					__super::erase(__super::begin());
					LeaveCriSec();
					return true;
				}
				LeaveCriSec();
				return false;
			}



			/*仅仅 出链表 不删除内容*/

			bool pop_back(_Ty& p) {
				EnterCriSec();
				if (!__super::empty()) {
					p = *(--__super::end());
					__super::pop_back();
					LeaveCriSec();
					return true;
				}
				LeaveCriSec();
				return false;
			}



			void clear() {
				EnterCriSec();
				__super::clear();
				LeaveCriSec();
			}



			//头删法 清空链表 并删除内存

			void clearAndDelete() {
				iterator it;
				EnterCriSec();
				while ((it = __super::begin()) != __super::end())
				{
					SAFE_DELETE((*it));
					__super::pop_front();
				}
				LeaveCriSec();
			}



			//头删法 清空链表 并删除内存

			void clearAndFree() {
				iterator it;
				EnterCriSec();
				while ((it = __super::begin()) != __super::end())
				{
					SAFE_FREE((*it));
					__super::pop_front();
				}
				LeaveCriSec();
			}




			size_t size() {
				size_t size = 0;
				EnterCriSec();
				size = __super::size();
				LeaveCriSec();
				return size;
			}




			bool Find(const _Ty& _val, iterator &it) {
				bool found = false;
				EnterCriSec();
				it = std::find(__super::begin(), __super::end(), _val);
				if (it != __super::end()) {
					found = true;
				}
				LeaveCriSec();
				return found;
			}




			void Remove(iterator& it) {
				EnterCriSec();
				__super::erase(it);
				LeaveCriSec();
			}




			void RemoveAndFree(iterator& it) {
				EnterCriSec();
				if ((*it) != NULL) {
					(*it) = NULL;
					free(*it);
				}
				__super::erase(it);
				LeaveCriSec();
			}




			void RemoveAndDelete(iterator& it) {
				EnterCriSec();
				if ((*it) != NULL) {
					(*it) = NULL;
					delete(*it);
				}
				__super::erase(it);
				LeaveCriSec();
			}




			void Remove(const _Ty& val) {
				iterator it;
				if (Find(val, it)) {
					EnterCriSec();
					__super::erase(it);
					LeaveCriSec();
				}
			}





			void RemoveAndFree(const _Ty& val) {
				iterator it;
				if (Find(val, it)) {
					EnterCriSec();
					if ((*it) != NULL) {
						(*it) = NULL;
						free(*it);
					}
					__super::erase(it);
					LeaveCriSec();
				}
			}




			void RemoveAndDelete(const _Ty& val) {
				iterator it;
				if (Find(val, it)) {
					EnterCriSec();
					if ((*it) != NULL) {
						(*it) = NULL;
						delete(*it);
					}
					__super::erase(it);
					LeaveCriSec();
				}
			}



			/*判断是否链表为空*/

			bool empty() {
				bool bRet = false;
				EnterCriSec();
				bRet = __super::empty();
				LeaveCriSec();
				return bRet;
			}



			void reverse() {
				EnterCriSec();
				__super::reverse();
				LeaveCriSec();
			}
	};




	/**map**/

	template<class _Kty,
		class _Ty,
		class _Pr = std::less<_Kty>,
		class _Alloc = std::allocator<std::pair<const _Kty, _Ty> > >
		class CMyMap_s : public std::map <_Kty, _Ty, _Pr, _Alloc>, public CMyCriSec
	{
	public:
		using iterator = typename std::map <_Kty, _Ty, _Pr, _Alloc>::iterator;
		using key_type = _Kty;
		using const_iterator = typename std::map <_Kty, _Ty, _Pr, _Alloc>::const_iterator;
		using _Pairib = typename std::map <_Kty, _Ty, _Pr, _Alloc>::_Pairib;
		template<class... _Mappedty>
		iterator  end()
		{
			EnterCriSec();
			iterator itor = __super::end();
			LeaveCriSec();
			return itor;
		}

		iterator find(const key_type& _Keyval)
		{
			EnterCriSec();
			iterator itor = __super::find(_Keyval);
			LeaveCriSec();
			return itor;
		}

		iterator erase(const_iterator _Where)
		{
			EnterCriSec();
			iterator itor = __super::erase(_Where);
			LeaveCriSec();
			return itor;
		}

		template<class _Valty>
		_Pairib insert(_Valty&& _Val)
		{
			EnterCriSec();
			_Pairib prb = __super::insert(_Val);
			LeaveCriSec();
			return prb;
		}
	};


	class DUI_CLASS_EXPORT CMyTcpClient {
	public:
		typedef enum _CS_socket_state {
			cs_state_stoped = 0,
			cs_state_inited,
			cs_state_connected
		}CS_socket_state;
	public:
		explicit CMyTcpClient(char* strIp = NULL, int port = 0);
		~CMyTcpClient();
	public:
		void SetServer(const char* strIp, int port);
		bool IsConnected();
		int  Send(char*cBuffer, int length);
		int  Recv(char*cBuffer, int length);
		bool SetSockOpt(__in int level, __in int optname, __in_bcount_opt(optlen) const char FAR * optval, __in int optlen);
		bool Init();
		void UnInit();
		bool Connect();
	public:
		char m_svcIp[16];
		int m_port;
	private:
		SOCKET m_socket;
		CS_socket_state m_state;
	};


	namespace DUI_THREAD {

		class DUI_CLASS_EXPORT CMyThread;
		typedef struct  MyThreadParam {
			CMyThread* pThread;
			LPVOID	lpClass;
			LPVOID	lpUserData;
			bool    bQuit;
		}*PMyThreadParam;

		typedef DWORD(*PMyThreadCallBack)(PMyThreadParam pMgr);


		class DUI_CLASS_EXPORT CMyThread {
		public:
			typedef enum _thread_state {
				st_stopped = 0,
				st_work,
				st_pause,
			}thread_state;

#define THREAD_LOAD_PARAM PMyThreadParam pMgr = (PMyThreadParam)lParam;
#define THREAD_LOAD_RUN {DWORD dwWait = ::WaitForSingleObject(pMgr->pThread->m_hExitEvent, pMgr->pThread->m_waitTime); \
		if (dwWait == WAIT_OBJECT_0 || dwWait == WAIT_FAILED) {\
			return 0;\
		}}

		public:
			HANDLE					m_hThread;
			MyThreadParam			m_param;
			DWORD					m_threadId;
			thread_state			m_state;
			PMyThreadCallBack		m_cb;
		public:
			CMyThread();
			virtual ~CMyThread();
		public:
			PMyThreadParam GetThreadParam();
			LPVOID GetUserData();
			void SetUserData(LPVOID lpUserData);
			bool IsWorked();
			/*创建线程*/
			bool Create(PMyThreadCallBack cb, LPVOID lpClass = NULL, LPVOID lpUserData = NULL);
		protected:
			bool Create();
		public:
			bool SetPriority(DWORD fdwPriority = THREAD_PRIORITY_NORMAL);
			/*恢复线程*/
			bool Resume();

			/*阻塞线程*/
			bool Suspend();
			/*自动结束*/
			bool Join(DWORD dwWaitTime = INFINITE);
			/*强制结束线程 不建议使用*/
			void TerminalThread();
			void SetExit();
		protected:
			static DWORD WINAPI mainWork(LPVOID lParam);
		};


		class DUI_CLASS_EXPORT CMyThreadGroup
		{
		public:
			typedef std::vector<std::shared_ptr<CMyThread>>::iterator iterator;
		private:
			std::vector<std::shared_ptr<CMyThread>>m_thList;
		public:
			CMyThreadGroup();
			virtual~CMyThreadGroup();
		public:
			size_t size();
			bool AddThread(PMyThreadCallBack cb, LPVOID lpClass = NULL, LPVOID lpUserData = NULL);
			void Join(DWORD dwWaitTime = INFINITE);
			PMyThreadParam GetThreadParam(int iIndex);
			LPVOID GetUserData(int iIndex);
			CMyThread* operator [](int iIndex);
		};

	}

	class DUI_CLASS_EXPORT CMyLogger {
	public:
		typedef void (WINAPI *PMyLoggerCallback)(CMyString *msg);
		enum LOGGER_LEVEL {
			LOGGER_DEBUG = 1,
			LOGGER_TRACE,
			LOGGER_INFO,
			LOGGER_WARN,
			LOGGER_ERROR,
			LOGGER_FATAL,
			LOGGER_FILE,
		};
		typedef struct DUI_CLASS_EXPORT  __logdata {
			CMyString strText;
			DWORD thrId;
			SYSTEMTIME sysTime;
			LOGGER_LEVEL level;
		}_logdata;
	public:
		explicit CMyLogger();
		virtual ~CMyLogger();
	public:
		bool Init(const TCHAR* szAppFileName,
			PMyLoggerCallback cb = NULL,
			bool logConsole = false,
			bool logFile = true);
		void UnInit();
	public:
		TCHAR * GetLogLevelDesc(LOGGER_LEVEL ll);
	protected:
		void Log_0(LOGGER_LEVEL& level, std::shared_ptr<_logdata>& _logdata);
		void LogV(LOGGER_LEVEL level, LPCTSTR format, va_list arg_ptr);
	public:
		LOGGER_LEVEL GetLogLevel();
		void SetLogLevel(LOGGER_LEVEL level);
		void SetUseThread(bool bUseThread);
	public:
		void Log(LOGGER_LEVEL level, LPCTSTR pstrFormat, ...);
		void Debug(LPCTSTR pstrFormat, ...);
		void Trace(LPCTSTR pstrFormat, ...);
		void Info(LPCTSTR pstrFormat, ...);
		void Warn(LPCTSTR pstrFormat, ...);
		void Error(LPCTSTR pstrFormat, ...);
		void Fatal(LPCTSTR pstrFormat, ...);
		void File(LPCTSTR pstrFormat, ...);
	public:
		void TryLog(LOGGER_LEVEL ll, LPCTSTR format, ...);
		void TryDebug(LPCTSTR format, ...);
		void TryTrace(LPCTSTR format, ...);
		void TryInfo(LPCTSTR format, ...);
		void TryWarn(LPCTSTR format, ...);
		void TryError(LPCTSTR format, ...);
		void TryFatal(LPCTSTR format, ...);
	protected:
		static DWORD ThreadWrite(DUI_THREAD::PMyThreadParam pMgr);
	protected:
		LPCTSTR GetTimeStr(const SYSTEMTIME& st);
		CMyString* MakeLogMsg(std::shared_ptr<_logdata>&logdata);
		void LogConsoleMsg(CMyString* msg);
	protected:
		HANDLE m_hFile;
		CMyString m_strFile;
		CMyVector_s<std::shared_ptr<_logdata>>m_dataList;
		DUI_THREAD::CMyThread m_thrMgr;
		HANDLE		m_exitEvent;
		DWORD		m_thrId;
		bool		m_logConsole;
		bool		m_logFile;
		LOGGER_LEVEL m_level;
		bool m_bInited;
		bool m_bUseThr;
		PMyLoggerCallback m_callback;
	};


#ifdef _USE_SERVICE

	/////////////////////////////
	class CServerManger {
	public:
		CServerManger() {
			m_schSCManager = NULL;
		}
		~CServerManger() {
			Close();
		}
	public:
		static CServerManger* GetInstance() {
			static CServerManger instance;
			return &instance;
		}
		SC_HANDLE Open() {
			m_schSCManager = OpenSCManager(
				NULL,                    // local computer
				NULL,                    // ServicesActive database 
				SC_MANAGER_ALL_ACCESS);  // full access rights 

			if (NULL == m_schSCManager)
			{
				wprintf(_T("OpenSCManager failed %s"), GetErrorString().c_str());
				return NULL;
			}
			return m_schSCManager;
		}
		void Close() {
			if (m_schSCManager) {
				CloseServiceHandle(m_schSCManager);
				m_schSCManager = NULL;
			}
		}
		SC_HANDLE GetSCManager() {
			if (m_schSCManager == NULL) {
				if (!Open()) return NULL;
			}
			return m_schSCManager;

		}
	private:
		SC_HANDLE m_schSCManager;
	};
#define		GetSCManager		CServerManger::GetInstance()->GetSCManager



	class CMyServiceBase {
	public:
		CMyServiceBase() {
		}
		virtual~CMyServiceBase() {
		}
	public:
		BOOL SvcInstall(LPCTSTR szPath,
			LPCTSTR svcName,
			LPCTSTR svcDisplayName,
			LPCTSTR szDescription,
			BOOL  bAutoStart) {
			return DUI_SvcInstall(szPath, svcName, svcDisplayName, szDescription, bAutoStart);
		}
		BOOL SvcUnInstall(LPCTSTR svcName) {
			return DUI_SvcUnInstall(svcName);
		}
		BOOL Svc_Start(LPCTSTR svcName) {
			DUI_SvcStart(svcName);
		}
		BOOL Svc_Stop(LPCTSTR svcName) {
			DUI_SvcStart(svcName);
		}
	protected:
		virtual bool Run() {
			return true;
		}
	};

	template<class T>
	class CMySerice :public CMyServiceBase {
	private:
		SERVICE_STATUS          m_SvcStatus;
		SERVICE_STATUS_HANDLE   m_SvcStatusHandle;
		HANDLE                  m_hSvcStopEvent;
		static CMySerice<T>*	ms_inst;
		std::wstring			m_serviceName;
		bool					m_IsConsoleMode;
	public:
		CMySerice(std::wstring strService) {
			ms_inst = this;
			m_serviceName = strService;
			m_SvcStatusHandle = NULL;
			m_hSvcStopEvent = NULL;
			m_IsConsoleMode = true;
		}
		virtual~CMySerice() {
			Close();
		}
	public:
		void SetServiceMode(bool isService) {
			if (isService) {
				if (m_serviceName.length() == 0) {
					m_IsConsoleMode = true;
				}
				else {
					m_IsConsoleMode = false;
				}
			}
			else {
				m_IsConsoleMode = true;
			}
		}
		bool IsServiceMode() {
			return !m_IsConsoleMode;
		}
		bool WinMain() {
			bool bRet = false;
			if (m_hSvcStopEvent != NULL) {
				CloseHandle(m_hSvcStopEvent);
				m_hSvcStopEvent = NULL;
			}
			m_hSvcStopEvent = CreateEvent(NULL, TRUE, FALSE, _T(""));
			if (!m_IsConsoleMode && m_serviceName.length() != 0) {
				bRet = _WinMain_s();
			}
			else {
				bRet = _WinMain();
			}
			return bRet;
		}
	protected:
		void SetQuit() {
			if (m_hSvcStopEvent) {
				SetEvent(m_hSvcStopEvent);
			}
		}
		void Close() {
			if (m_hSvcStopEvent != NULL) {
				CloseHandle(m_hSvcStopEvent);
				m_hSvcStopEvent = NULL;
			}
		}
		void MessageLoop() {
			WaitForSingleObject(m_hSvcStopEvent, INFINITE);
		}
		bool _WinMain() {
			SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
			T* pT = static_cast<T*>(this);
			return pT->Run();
		}
		bool _WinMain_s() {
			SERVICE_TABLE_ENTRY DispatchTable[] =
			{
				{ (LPWSTR)ms_inst->m_serviceName.c_str(), (LPSERVICE_MAIN_FUNCTION)SvcMain },
				{ NULL, NULL }
			};
			if (!StartServiceCtrlDispatcher(DispatchTable)) {
				wprintf(_T("StartServiceCtrlDispatcher failed"));
				return false;
			}
			return true;
		}
	protected:
		static VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv)
		{
			ms_inst->m_SvcStatusHandle = RegisterServiceCtrlHandler(
				ms_inst->m_serviceName.c_str(), SvcCtrlHandler);
			if (!ms_inst->m_SvcStatusHandle) {
				ms_inst->SvcReportEvent(_T("RegisterServiceCtrlHandler"));
				return;
			}
			ms_inst->m_SvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
			ms_inst->m_SvcStatus.dwServiceSpecificExitCode = 0;
			ms_inst->ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

			T* pT = static_cast<T*>(ms_inst);
			pT->Run();
		}

		static VOID WINAPI SvcCtrlHandler(DWORD dwCtrl)
		{
			// Handle the requested control code. 
			switch (dwCtrl)
			{
			case SERVICE_CONTROL_STOP: {
				ms_inst->ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
				SetEvent(ms_inst->m_hSvcStopEvent);
				ms_inst->ReportSvcStatus(ms_inst->m_SvcStatus.dwCurrentState, NO_ERROR, 0);
				break;
			}
			case SERVICE_CONTROL_INTERROGATE: {
				break;
			}
			default:
				break;
			}
		}

		static BOOL __stdcall CtrlHandler(DWORD fdwCtrlType)
		{
			switch (fdwCtrlType)
			{
				// Handle the CTRL-C signal. 
			case CTRL_C_EVENT: {
				//printf("Ctrl-C event\n");
				SetEvent(ms_inst->m_hSvcStopEvent);
				return(TRUE);
			}
			case CTRL_CLOSE_EVENT: {
				//printf("Ctrl-Close event\n");
				SetEvent(ms_inst->m_hSvcStopEvent);
				return(TRUE);
			}
								   // Pass other signals to the next handler. 
			case CTRL_BREAK_EVENT: {
				//printf("Ctrl-Break event\n");
				SetEvent(ms_inst->m_hSvcStopEvent);
				return(TRUE);
			}
			case CTRL_LOGOFF_EVENT: {
				//printf("Ctrl-Logoff event\n\n");
				SetEvent(ms_inst->m_hSvcStopEvent);
				return(TRUE);
			}
			case CTRL_SHUTDOWN_EVENT: {
				//printf("Ctrl-Shutdown event\n\n");
				SetEvent(ms_inst->m_hSvcStopEvent);
				return(TRUE);
			}
			default:
				return FALSE;
			}
		}


		VOID ReportSvcStatus(DWORD dwCurrentState,
			DWORD dwWin32ExitCode,
			DWORD dwWaitHint)
		{
			static DWORD dwCheckPoint = 1;
			m_SvcStatus.dwCurrentState = dwCurrentState;
			m_SvcStatus.dwWin32ExitCode = dwWin32ExitCode;
			m_SvcStatus.dwWaitHint = dwWaitHint;

			if (dwCurrentState == SERVICE_START_PENDING)
				m_SvcStatus.dwControlsAccepted = 0;
			else m_SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

			if ((dwCurrentState == SERVICE_RUNNING) ||
				(dwCurrentState == SERVICE_STOPPED)) {
				m_SvcStatus.dwCheckPoint = 0;
			}
			else {
				m_SvcStatus.dwCheckPoint = dwCheckPoint++;
			}
			// Report the status of the service to the SCM.
			SetServiceStatus(m_SvcStatusHandle, &m_SvcStatus);
		}

		VOID SvcReportEvent(LPTSTR szFunction) {
			wprintf(_T("%s %d\n"), szFunction, ::GetLastError());
		}
	};

	template<class T>
	CMySerice<T>* CMySerice<T>::ms_inst = NULL;

	/*class CTest :public CMySerice<CTest> {
	public:
		CTest(std::wstring strServiceName) :CMySerice<CTest>(strServiceName) {
		}
	public:
		virtual bool Run() {
			if (IsConsoleMode()) {
				printf("控制台模式\n");
			}
			else {
				printf("服务模式\n");
			}
			return true;
		}
	};*/
#endif // _USE_SERVICE
}