#include "stdafx.h"
#include <regex>
#include <vector>
#include "libwraper.h"

namespace DUI_API {


	template<typename T>
	T* __strcpy(T*_Dst, const T*_Src) {
		T* pIt = _Dst;
		while (*_Src != NULL) {
			*(_Dst++) = *(_Src++);
		}
		*_Dst = 0;
		return pIt;
	}
	template<typename T>
	T* __strncpy(T*_Dst, const T*_Src, int Count) {
		T* pIt = _Dst;
		int i = 0;
		while (*_Src != NULL) {
			*(_Dst++) = *(_Src++);
			i++;
			if (i == Count) break;
		}
		*_Dst = 0;
		return pIt;
	}
	template<typename T>
	T* __strcpy_s(T*_Dst, int cchMax, const T*_Src) {
		T* pIt = _Dst;
		int i = 0;
		while (*_Src != NULL) {
			*(_Dst++) = *(_Src++);
			i++;
			if (i == cchMax) break;
		}
		*_Dst = 0;
		return pIt;
	}
	template<typename T>
	int __strlen(const T*_Src) {
		int i = 0;
		if (_Src != NULL) {
			while ((*_Src) != NULL) {
				i++;
				_Src++;
			}
		}
		return i;
	}
	template<typename T>
	T* __strcat(T*_Dst, const T*_Src) {
		T* pIt = _Dst;
		while (*_Dst != NULL) { _Dst++; }
		while (*_Src != NULL) {
			*(_Dst++) = *(_Src++);
		}
		*_Dst = 0;
		return pIt;
	}
	template<typename T>
	bool __strequal(const T*_Dst, const T*_Src) {
		if (__strlen(_Src) != __strlen(_Dst)) return false;
		while (*(_Src++) != NULL && *(_Dst++) !NULL) {
			if (*_Src != *_Dst) return false;
		}
		return true;
	}
	template<typename T>
	void __strset(T*_Src, T &value) {
		while (*_Src != NULL) {
			*(_Src)++ = value;
		}
		return true;
	}
	template<typename T>
	int __strpos(const T*_Src, const T*_Sub) {
		int len1 = __strlen(_Src), len2 = __strlen(_Sub);
		int i = 0, j = 0;
		while (i < len1 && j < len2) {
			if (_Src[i] == _Sub[j]) {
				i++;
				j++;
			}
			else {
				i = i - j + 1;
				j = 0;
			}
		}
		if (j == len2 && j != 0)
			return i - len2;
		else
			return -1;
	}
	template<typename T>
	int __strrpos(const T*_Src, const T*_Sub) {
		int len1 = __strlen(_Src), len2 = __strlen(_Sub);
		int i = len1 - len2, j = 0;
		while (i >= 0 && j < len2) {
			if (_Src[i] == _Sub[j]) {
				i++;
				j++;
			}
			else {
				i = i - j - 1;
				j = 0;
			}
		}
		if (j == len2)
			return i - len2;
		else
			return -1;
	}
	template<typename T>
	int __stripos(const T* __Src, const T* __Sub) {
		T* _Src = const_cast<T*>(__Src);
		T* _Sub = const_cast<T*>(__Sub);
		int len1 = __strlen(_Src), len2 = __strlen(_Sub);
		int i = 0, j = 0;
		while (i < len1 && j < len2) {
			if (_Src[i] >= 'a' && _Src[i] <= 'z')
				_Src[i] = _Src[i] - 32;

			if (_Sub[j] >= 'a' && _Sub[j] <= 'z')
				_Sub[j] = _Sub[j] - 32;
			if (_Src[i] == _Sub[j]) {
				i++;
				j++;
			}
			else {
				i = i - j + 1;
				j = 0;
			}
		}
		if (j == len2)
			return i - len2;
		else
			return -1;
	}
	template<typename T>
	int __strripos(const T* __Src, const T* __Sub) {
		T* _Src = const_cast<T*>(__Src);
		T* _Sub = const_cast<T*>(__Sub);
		int len1 = __strlen(_Src), len2 = __strlen(_Sub);
		int i = len1 - len2, j = 0;
		while (i >= 0 && j < len2) {
			if (_Src[i] >= 'a' && _Src[i] <= 'z')
				_Src[i] = _Src[i] - 32;

			if (_Sub[j] >= 'a' && _Sub[j] <= 'z')
				_Sub[j] = _Sub[j] - 32;

			if (_Src[i] == _Sub[j]) {
				i++;
				j++;
			}
			else {
				i = i - j - 1;
				j = 0;
			}
		}
		if (j == len2)
			return i - len2;
		else
			return -1;
	}
	template<typename T>
	int __strupper(T* _Src) {
		T * p = _Src;
		int i = 0;
		while (*p != NULL) {
			if (*p >= 'a' && *p <= 'z') {
				*p = *p - 32;
				i++;
			}
			p++;
		}
		return i;
	}
	template<typename T>
	int __strlower(T* _Src) {
		T * p = _Src;
		int i = 0;
		while (*p != NULL) {
			if (*p >= 'A' && *p <= 'Z') {
				*p = *p + 32;
				i++;
			}
			p++;
		}
		return i;
	}
	template<typename T>
	T* __strltrim(T* _Src) {
		T * p = _Src;
		while (*p != 0) {
			if (*p != ' ') break;
			p++;
		}
		if (p && *p != 0) {
			int len = __strlen(p);
			if (len) {
				memmove(_Src, p, (len + 1) * sizeof(T));
			}
			else {
				_Src[0] = 0;
			}
		}
		return _Src;
	}

	template<typename T>
	T* __strrtrim(T* _Src) {
		int len = __strlen(_Src);
		while (len--) {
			if (_Src[len] != ' ') break;
			_Src[len] = 0;
		}
		return _Src;
	}
	template<typename T>
	T* __strtrim(T* _Src) {
		__strrtrim(_Src);
		__strrtrim(_Src);
		return _Src;
	}
	template<typename T>
	std::vector<int> __strfindpos(T* _Src, const T* _Sub) {
		int pos = -1, count = 0, basepos = 0;
		int SubLen = __strlen(_Sub), element_size = sizeof(T);

		std::vector<int>vec;
		T*p = _Src;
		pos = __strpos(p, _Sub);
		while (pos != -1) {
			basepos += pos;
			vec.push_back(basepos);
			basepos += SubLen;
			p = p + pos + SubLen;
			pos = __strpos(p, _Sub);
		}
		return vec;
	}
	template<typename T>
	bool __strreplace(T* _Src, int len, const T* _Sub, int s_len, const T* _To, int t_len) {
		int pos = __strpos(_Sub);
		if (pos == -1) return false;
		if (len < 0) len = __strlen(_Src);
		if (s_len < 0) s_len = __strlen(_Sub);
		if (t_len < 0) t_len = __strlen(_To);
		int move = t_len - s_len;
		if (move)
			memmove(_Src + pos + s_len + move, _Src + pos + s_len, (len - abs(move)) * m_elementsize);
		memcpy(m_pStr + pos, pDst, t_len *m_elementsize);
		return true;
	}

	template<typename T>
	T * __strrev(T* _Src, int len/* = -1*/)
	{
		if (len < 0) len = __strlen(_Src);
		T    *start = _Src;
		T    *end = _Src + len - 1;
		T    ch;
		if (len)
		{
			while (start < end)
			{
				ch = *start;
				*start++ = *end;
				*end-- = ch;
			}
		}
		return _Src;
	}



	template<typename T>
	int __strpos(const T* __Src, const T& x) {
		int pos = -1;
		while (*__Src != 0) {
			pos++;
			if (*__Src == x) return pos;
			__Src++;
		}
		return -1;
	}

	template<typename T>
	int __strrpos(const T* __Src, const T& x) {
		int len = __strlen(__Src);
		for (int i = len - 1; i >= 0; i--) {
			if (__Src[i] == x)
				return i;
		}
		return -1;
	}


	template<typename T>
	T* __strchr(const T* src, const T x) {
		int pos = __strpos(src, x);
		if (pos >= 0)
			return (T*)(src + pos);
		return NULL;
	}
	template<typename T>
	T* __strrchr(const T* src, const T x) {
		int pos = __strrpos(src, x);
		if (pos >= 0)
			return (T*)(src + pos);
		return NULL;
	}



	/**************************************************

	CMyBuffer

	**************************************************/
	template class
		CMyBuffer<char>;
	template class
		CMyBuffer<unsigned char>;


	template<class _TT, int _INIT_ALLOC_LENGTH_>
	CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::CMyBuffer() :m_lpBuf(NULL), m_allocLength(0) {
		xmalloc(_INIT_ALLOC_LENGTH_);
	}
	template<class _TT, int _INIT_ALLOC_LENGTH_>
	CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::CMyBuffer(int _init_size) : m_lpBuf(NULL), m_allocLength(0) {
		xmalloc(_init_size);
	}
	template<class _TT, int _INIT_ALLOC_LENGTH_>
	CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::CMyBuffer(const CMyBuffer<_TT>& myBuffer) : m_lpBuf(NULL), m_allocLength(0) {
		CMyBuffer<_TT>*p = (CMyBuffer<_TT>*)&myBuffer;
		Assign(p->buf(), p->length());
	}
	template<class _TT, int _INIT_ALLOC_LENGTH_>
	CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::CMyBuffer(_TT* szBuf, int _iLength) : m_lpBuf(NULL), m_allocLength(0) {
		Assign(szBuf, _iLength);
	}
	template<class _TT, int _INIT_ALLOC_LENGTH_>
	CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::~CMyBuffer() {
		xfree();
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	CMyBuffer<_TT>& CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::operator=(CMyBuffer<_TT>& myBuffer) {
		Assign(myBuffer.buf(), myBuffer.length());
		return *this;
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	CMyBuffer<_TT>& CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::operator+=(CMyBuffer<_TT>& myBuffer) {
		Append(myBuffer.buf(), myBuffer.length());
		return *this;
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	bool CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::Assign(const _TT* szBuf, int _iLength) {
		if (szBuf == NULL) return false;
		if (!xmalloc(_iLength)) return false;
		m_length = _iLength;
		memcpy(m_lpBuf, szBuf, _iLength * sizeof(_TT));
		return true;
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	bool CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::Append(const _TT* szBuf, int _iLength) {
		if (szBuf == NULL) return false;
		if (!xmalloc(m_length + _iLength)) return false;
		memcpy(m_lpBuf + m_length, szBuf, _iLength * sizeof(_TT));
		m_length += _iLength;
		return true;
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	bool CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::empty() {
		return (m_lpBuf == NULL);
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	bool CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::valid() {
		return (m_lpBuf != NULL);
	}


	template<class _TT, int _INIT_ALLOC_LENGTH_>
	int CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::length() {
		return m_length;
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	int CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::GetAllocLength() {
		return m_allocLength;
	}


	template<class _TT, int _INIT_ALLOC_LENGTH_>
	bool CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::SetLength(int iLength){
		if (iLength <= m_allocLength) {
			m_length = iLength;
			return true;
		}
		return false;
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	_TT* CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::buf() {
		return m_lpBuf;
	}


	template<class _TT, int _INIT_ALLOC_LENGTH_>
	void CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::free() {
		xfree();
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	bool CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::xmalloc(int length) {
		if (length <= m_allocLength) {
			return true;
		}
		_TT* p = NULL;
		try {
			if (m_lpBuf == NULL) {
				p = (_TT*)malloc(length * sizeof(_TT));
			}
			else {
				p = (_TT*)realloc(m_lpBuf, length * sizeof(_TT));
			}
		}
		catch (...) {
			return false;
		}
		if (p == NULL) {
			return false;
		}
		m_allocLength = length;
		m_lpBuf = p;
		return true;
	}

	template<class _TT, int _INIT_ALLOC_LENGTH_>
	void CMyBuffer<_TT, _INIT_ALLOC_LENGTH_>::xfree() {
		if (m_lpBuf != NULL) {
			::free(m_lpBuf);
			m_lpBuf = NULL;
		}
		m_allocLength = m_length = 0;
	}


	/************************************************************

	CMyStringT

	***********************************************************/


	template class CMyStringT<char>;
	template class CMyStringT<wchar_t>;

	template<class _TypeName>
	CMyStringT<_TypeName>::CMyStringT() :m_pcStr(m_buffer), m_allocLength(__INIT_ALLOC_LENGTH__) {
		m_pcStr[0] = 0;
	}

	template<class _TypeName>
	CMyStringT<_TypeName>::CMyStringT(int length) : m_pcStr(m_buffer), m_allocLength(__INIT_ALLOC_LENGTH__) {
		m_pcStr[0] = 0;
		xmalloc(length);
	}

	template<class _TypeName>
	CMyStringT<_TypeName>::CMyStringT(const _TypeName* cStr, int length /* = -1 */) :m_pcStr(m_buffer), m_allocLength(__INIT_ALLOC_LENGTH__) {
		m_pcStr[0] = 0;
		Assign(cStr, length);
	}
	template<class _TypeName>
	CMyStringT<_TypeName>::CMyStringT(const CMyStringT<_TypeName>& myString) : m_pcStr(m_buffer), m_allocLength(__INIT_ALLOC_LENGTH__) {
		m_pcStr[0] = 0;
		const _TypeName*p = myString.c_str();
		this->Assign(p, -1);
	}
	template<class _TypeName>
	CMyStringT<_TypeName>::~CMyStringT() {
		xfree();
	}


	template<class _TypeName>
	_TypeName * CMyStringT<_TypeName>::c_str() const {
		return m_pcStr;
	}


	template<class _TypeName>
	const _TypeName* CMyStringT<_TypeName>::GetString() {
		return m_pcStr;
	}


	template<class _TypeName>
	_TypeName* CMyStringT<_TypeName>::GetBuffer() {
		return m_pcStr;
	}

	template<class _TypeName>
	CMyStringT<_TypeName>& CMyStringT<_TypeName>::operator=(const _TypeName* cStr) {
		Assign(cStr, -1);
		return *this;
	}

	template<class _TypeName>
	CMyStringT<_TypeName>& CMyStringT<_TypeName>::operator=(CMyStringT<_TypeName>& myString) {
		Assign(myString.c_str(), -1);
		return *this;
	}


	template<class _TypeName>
	CMyStringT<_TypeName>& CMyStringT<_TypeName>::operator+=(const _TypeName* cStr) {
		Append(cStr, -1);
		return *this;
	}



	template<class _TypeName>
	CMyStringT<_TypeName>& CMyStringT<_TypeName>::operator+=(CMyStringT<_TypeName>& myString) {
		Append(myString.c_str(), -1);
		return *this;
	}

	template<class _TypeName>
	_TypeName& CMyStringT<_TypeName>::operator[](int i) {
		return *(m_pcStr + i);
	}


	template<class _TypeName>
	int CMyStringT<_TypeName>::ReleaseBuffer(int minBufferLength /* = -1 */) {
		if (minBufferLength <= 0) minBufferLength = GetLength();
		CMyStringT<_TypeName>tmp(m_pcStr);
		clear();
		Assign(tmp.c_str(), minBufferLength);
		return GetAllocLength();
	}

	template<class _TypeName>
	void CMyStringT<_TypeName>::SetBufferLength(int iLength) {
		xmalloc(iLength);
	}



	template<class _TypeName>
	CMyStringT<_TypeName> CMyStringT<_TypeName>::Left(int iLength) {
		int len = min(GetLength(), iLength);
		CMyStringT<_TypeName>strRet(c_str(), len);
		return strRet;

		return CMyStringT<_TypeName>();
	}


	template<class _TypeName>
	CMyStringT<_TypeName> CMyStringT<_TypeName>::Right(int iLength) {
		int pos = 0;
		if (iLength > 0) {
			pos = GetLength() - iLength;
			if (pos < 0) {
				pos = 0;
				iLength = -1;
			}
		}
		CMyStringT<_TypeName>strRet(c_str() + pos, iLength);
		return strRet;
	}


	template<class _TypeName>
	CMyStringT<_TypeName> CMyStringT<_TypeName>::Mid(int pos, int iLength/* = -1*/) {
		int iStrLen = GetLength();
		CMyStringT<_TypeName>strRet;
		if (pos < 0) pos = 0;
		if (pos <= iStrLen) {
			strRet.Assign(c_str() + pos, iLength);
		}
		return strRet;
	}

	template<class _TypeName>
	int CMyStringT<_TypeName>::Find(_TypeName c) {
		return __strpos(c_str(), c);
	}

	template<class _TypeName>
	int CMyStringT<_TypeName>::ReverseFind(_TypeName c) {
		return __strrpos(c_str(), c);
	}
	template<class _TypeName>
	int CMyStringT<_TypeName>::Compare(const _TypeName* str) const {
		int len1 = __strlen(str);
		int len2 = __strlen(str);
		if (len1 != len2) return -1;
		return memcmp(m_pcStr, str, min(len1,len2)); 
	}


	template<class _TypeName>
	void CMyStringT<_TypeName>::clear() {
		xfree();
	}


	template<class _TypeName>
	int CMyStringT<_TypeName>::GetLength() {
		return __strlen(m_pcStr);
	}


	template<class _TypeName>
	int CMyStringT<_TypeName>::length() {
		return __strlen(m_pcStr);
	}

	template<class _TypeName>
	int CMyStringT<_TypeName>::GetAllocLength() {
		return m_allocLength;
	}


	template<class _TypeName>
	bool CMyStringT<_TypeName>::empty() {
		return (m_pcStr == NULL || __strlen(m_pcStr) == 0);
	}

	template<class _TypeName>
	bool CMyStringT<_TypeName>::equal(CMyStringT<_TypeName>& _Right) {
		int len1 = _Right.GetLength();
		int len2 = GetLength();
		if (len1 != len2) return false;
		return memcmp(GetBuffer(), _Right.GetBuffer(), min(len1, len2)) == 0;
	}



	template<class _TypeName>
	bool CMyStringT<_TypeName>::equal(const _TypeName * pStr) {
		int len1 = __strlen(pStr);
		int len2 = GetLength();
		if (len1 != len2) return false;
		return memcmp(GetBuffer(), pStr, min(len1, len2)) == 0;
	}

	template<class _TypeName>
	bool CMyStringT<_TypeName>::Assign(const _TypeName* cStr, int length) {
		int strLen = __strlen(cStr);
		if (length < 0) {
			length = strLen;
		}
		if (!xmalloc(length)) return false;
		if (strLen < length) length = strLen;
		if (length > 0) {
			memcpy(m_pcStr, cStr, length * sizeof(_TypeName));
		}
		m_pcStr[length] = 0;
		return true;
	}



	template<class _TypeName>
	bool CMyStringT<_TypeName>::Append(const _TypeName* cStr, int length) {
		int strLen = __strlen(cStr);
		if (length < 0) length = strLen;
		int oldLen = __strlen(m_pcStr);
		if (!xmalloc(oldLen + length)) return false;
		if (length > 0) {
			memcpy(m_pcStr + oldLen, cStr, length * sizeof(_TypeName));
		}
		m_pcStr[oldLen + length] = 0;
		return true;
	}


	template<class _TypeName>
	bool CMyStringT<_TypeName>::xmalloc(int length) {
		if (length < m_allocLength) return true;
		__try {
			_TypeName*p = NULL;
			int tmp_length = 0;
			if (m_pcStr == m_buffer) {
				tmp_length = length + 1;
				p = (_TypeName*)malloc(tmp_length * sizeof(_TypeName));
				if (p == NULL) {
					return false;
				}
				memcpy(p, m_buffer, (__strlen(m_buffer) + 1) * sizeof(_TypeName));
			}
			else {
				if (m_pcStr == NULL) {
					p = (_TypeName*)malloc(tmp_length * sizeof(_TypeName));
				}
				else {
					p = (_TypeName*)realloc(m_pcStr, (length + 1) * sizeof(_TypeName));
				}
				if (p == NULL) {
					return false;
				}
				tmp_length = length + 1;
			}
			m_pcStr = p;
			m_allocLength = tmp_length;
			return true;
		}
		__except (1) {
			m_pcStr = m_buffer;
			m_pcStr[0] = 0;
			m_allocLength = __INIT_ALLOC_LENGTH__;
		}
		return false;
	}

	template<class _TypeName>
	void CMyStringT<_TypeName>::xfree() {
		__try {
			if (m_pcStr != m_buffer) {
				free(m_pcStr);
				m_pcStr = m_buffer;
				m_pcStr[0] = 0;
				m_allocLength = __INIT_ALLOC_LENGTH__;
			}

		}
		__except (1) {
			m_pcStr = m_buffer;
			m_pcStr[0] = 0;
			m_allocLength = 0;
		}

	}


	/*******************************************************************************

				CMyStringW

	********************************************************************************/


	CMyStringA::CMyStringA() :CMyStringT<char>() {
	}
	CMyStringA::CMyStringA(int length) : CMyStringT<char>(length) {
	}
	CMyStringA::CMyStringA(const char* cStr, int length/* = -1*/) : CMyStringT<char>(cStr, length) {
	}
	CMyStringA::CMyStringA(const CMyStringA& myString) : CMyStringT<char>(myString) {
	}
	CMyStringA::CMyStringA(const CMyStringT<char>& myString) : CMyStringT<char>(myString) {
	}



	CMyStringA & CMyStringA::operator=(const char* cStr) {
		Assign(cStr, -1);
		return *this;
	}
	CMyStringA& CMyStringA::operator=(CMyStringT<char>myString) {
		Assign(myString.c_str(), -1);
		return *this;
	}
	CMyStringA& CMyStringA::operator=(CMyStringA& myString) {
		Assign(myString.c_str(), -1);
		return *this;
	}

	CMyStringA& CMyStringA::operator+=(const char* cStr) {
		Append(cStr, -1);
		return *this;
	}
	CMyStringA& CMyStringA::operator+=(CMyStringT<char>& myString) {
		Append(myString.c_str(), -1);
		return *this;
	}
	CMyStringA& CMyStringA::operator+=(CMyStringA& myString) {
		Append(myString.c_str(), -1);
		return *this;
	}

	CMyStringA CMyStringA::operator+(const CMyStringA& src) const
	{
		CMyStringA sTemp = *this;
		sTemp.Append(src,-1);
		return sTemp;
	}

	CMyStringA CMyStringA::operator+(LPCSTR lpStr) const
	{
		if (lpStr)
		{
			CMyStringA sTemp = *this;
			sTemp.Append(lpStr,-1);
			return sTemp;
		}

		return *this;
	}

	CMyStringA::operator LPCSTR()  const {
		return (LPCSTR)m_pcStr;
	}
	CMyStringA::operator LPSTR()  const {
		return (LPSTR)m_pcStr;
	}

	void CMyStringA::MakeUpper()
	{
		strupr(m_pcStr);
	}

	void CMyStringA::MakeLower()
	{
		strlwr(m_pcStr);
	}

	CMyStringW CMyStringA::toWideString()
	{
		CMyStringW tmp;
		try {
			int unicodeLen = MultiByteToWideChar(CP_ACP, 0, m_pcStr, -1, NULL, 0);
			tmp.SetBufferLength(unicodeLen + 1);
			MultiByteToWideChar(CP_ACP, 0, m_pcStr, -1, (wchar_t*)tmp.c_str(), unicodeLen);
		}
		catch (...) {
			tmp[0] = 0;
		}
		return  tmp;
	}
	int  __cdecl CMyStringA::FormatV(const char* pstrFormat, va_list valist) {
		int iRet = 0;
		do {
			iRet = _vsnprintf(c_str(), GetAllocLength(), pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 0;
			for (int i = 0; i < 8; i++) {
				_bufLen = GetAllocLength() * 2 * (i + 1);
				SetBufferLength(_bufLen);
				m_pcStr[0] = 0;
				iRet = _vsnprintf(c_str(), GetAllocLength(), pstrFormat, valist);
				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		return iRet;
	}
	int __cdecl CMyStringA::Format(const char* pstrFormat, ...) {
		va_list valist;                   /* 定义保存函数参数的结构 */
		va_start(valist, pstrFormat);
		int iRet = 0;
		do {
			iRet = _vsnprintf(c_str(), GetAllocLength(), pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 0;
			for (int i = 0; i < 8; i++) {
				_bufLen = GetAllocLength() * 2 * (i + 1);
				SetBufferLength(_bufLen);
				m_pcStr[0] = 0;
				iRet = _vsnprintf(c_str(), GetAllocLength(), pstrFormat, valist);
				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		va_end(valist);
		return iRet;
	}

	int __cdecl CMyStringA::AppendFormat(const char* pstrFormat, ...) {
		CMyStringA tmp;
		va_list valist;                   /* 定义保存函数参数的结构 */
		va_start(valist, pstrFormat);
		int iRet = 0;
		do {
			iRet = _vsnprintf(tmp.c_str(), tmp.GetAllocLength(), pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 0;
			for (int i = 0; i < 8; i++) {
				_bufLen = tmp.GetAllocLength() * 2 * (i + 1);
				tmp.SetBufferLength(_bufLen);
				tmp[0] = 0;
				iRet = _vsnprintf(tmp.c_str(), tmp.GetAllocLength(), pstrFormat, valist);
				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		va_end(valist);
		Append(tmp.c_str(), -1);
		return iRet;
	}




	/*******************************************************************************

				CMyStringW

	********************************************************************************/



	CMyStringW::CMyStringW() :CMyStringT<wchar_t>() {
	}
	CMyStringW::CMyStringW(int length) : CMyStringT<wchar_t>(length) {
	}
	CMyStringW::CMyStringW(const wchar_t* cStr, int length/* = -1*/) : CMyStringT<wchar_t>(cStr, length) {
	}
	CMyStringW::CMyStringW(const CMyStringW& myString) : CMyStringT<wchar_t>(myString) {
	}
	CMyStringW::CMyStringW(const CMyStringT<wchar_t>& myString) : CMyStringT<wchar_t>(myString) {
	}


	CMyStringW & CMyStringW::operator=(const wchar_t* cStr) {
		Assign(cStr, -1);
		return *this;
	}
	CMyStringW& CMyStringW::operator=(CMyStringT<wchar_t>& myString) {
		Assign(myString.c_str(), -1);
		return *this;
	}
	CMyStringW& CMyStringW::operator=(CMyStringW& myString) {
		Assign(myString.c_str(), -1);
		return *this;
	}

	CMyStringW& CMyStringW::operator+=(const wchar_t* cStr) {
		Append(cStr, -1);
		return *this;
	}
	CMyStringW& CMyStringW::operator+=(CMyStringT<wchar_t>& myString) {
		Append(myString.c_str(), -1);
		return *this;
	}
	CMyStringW& CMyStringW::operator+=(CMyStringW& myString) {
		Append(myString.c_str(), -1);
		return *this;
	}
	CMyStringW CMyStringW::operator+(const CMyStringW& src) const
	{
		CMyStringW sTemp = *this;
		sTemp.Append(src, -1);
		return sTemp;
	}

	CMyStringW CMyStringW::operator+(LPCWSTR lpStr) const
	{
		if (lpStr)
		{
			CMyStringW sTemp = *this;
			sTemp.Append(lpStr, -1);
			return sTemp;
		}

		return *this;
	}
	CMyStringW::operator LPCWSTR()  const {
		return (LPCWSTR)m_pcStr;
	}
	CMyStringW::operator LPWSTR()  const {
		return (LPWSTR)m_pcStr;
	}

	void CMyStringW::MakeUpper()
	{
		wcsupr(m_pcStr);
	}

	void CMyStringW::MakeLower()
	{
		wcslwr(m_pcStr);
	}


	CMyStringA CMyStringW::toMutiString() {
		CMyStringA tmp;
		try {
			int mutiLen = WideCharToMultiByte(CP_ACP, 0, m_pcStr, -1, NULL, 0, NULL, NULL);
			tmp.SetBufferLength(mutiLen + 1);
			WideCharToMultiByte(CP_ACP, 0, m_pcStr, -1, (char*)tmp.c_str(), mutiLen, NULL, NULL);
			return 	tmp;
		}
		catch (std::exception & e) {
			UNREFERENCED_PARAMETER(e);
			tmp[0] = 0;
		}
		return tmp;
	}

	int __cdecl CMyStringW::FormatV(const wchar_t* pstrFormat, va_list valist) {
		int iRet = 0;
		do {
			iRet = _vsnwprintf(c_str(), GetAllocLength(), pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 0;
			for (int i = 0; i < 8; i++) {
				_bufLen = GetAllocLength() * 2 * (i + 1);
				SetBufferLength(_bufLen);
				m_pcStr[0] = 0;
				iRet = _vsnwprintf(c_str(), GetAllocLength(), pstrFormat, valist);
				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		return iRet;
	}
	int __cdecl CMyStringW::Format(const wchar_t* pstrFormat, ...) {
		va_list valist;                   /* 定义保存函数参数的结构 */
		va_start(valist, pstrFormat);
		int iRet = 0;
		do {
			iRet = _vsnwprintf(c_str(), GetAllocLength(), pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 0;
			for (int i = 0; i < 8; i++) {
				_bufLen = GetAllocLength() * 2 * (i + 1);
				SetBufferLength(_bufLen);
				m_pcStr[0] = 0;
				iRet = _vsnwprintf(c_str(), GetAllocLength(), pstrFormat, valist);
				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		va_end(valist);
		return iRet;
	}

	int __cdecl CMyStringW::AppendFormat(const wchar_t* pstrFormat, ...) {
		CMyStringW tmp;
		va_list valist;                   /* 定义保存函数参数的结构 */
		va_start(valist, pstrFormat);
		int iRet = 0;
		do {
			iRet = _vsnwprintf(tmp.c_str(), tmp.GetAllocLength(), pstrFormat, valist);
			if (iRet != -1) {
				break;
			}
			int _bufLen = 0;
			for (int i = 0; i < 8; i++) {
				_bufLen = tmp.GetAllocLength() * 2 * (i + 1);
				tmp.SetBufferLength(_bufLen);
				tmp[0] = 0;
				iRet = _vsnwprintf(tmp.c_str(), tmp.GetAllocLength(), pstrFormat, valist);
				if (iRet != -1) {
					break;
				}
			}
		} while (0);
		va_end(valist);
		Append(tmp.c_str(), -1);
		return iRet;
	}

}