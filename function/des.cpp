#include "stdafx.h"
#include "libwraper.h"
#include <wincrypt.h>
#include <atlstr.h>
namespace DUI_API
{

	namespace DUI_ENCODE {


		typedef struct
		{
			BLOBHEADER header;
			DWORD cbKeySize;
			BYTE rgbKeyData[8];
		}KeyBlob;

		const BYTE IV[] = { 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF };

		DUI_FUNC_CXX_EXPORT DWORD des_getsize(DWORD dataSize) {
			return  dataSize + 8 - (dataSize % 8);
		}
		/* 明文密文问缓冲区*/
		DUI_FUNC_CXX_EXPORT DWORD des_encrypt(BYTE* buffer, DWORD dataLength, DWORD bufferLength, TCHAR key[8])
		{
			CT2CA passwd(key, CP_UTF8);
			if (buffer == NULL || bufferLength < dataLength + 8 - (dataLength % 8)) return 0;
			HCRYPTPROV hProv = NULL;
			HCRYPTKEY hSessionKey = NULL;
			BOOL bResult = TRUE;
			KeyBlob blob;
			memset(&blob, 0, sizeof(blob));
			blob.header.bType = PLAINTEXTKEYBLOB;
			blob.header.bVersion = CUR_BLOB_VERSION;
			blob.header.reserved = 0;
			blob.header.aiKeyAlg = CALG_DES;
			blob.cbKeySize = 8;
			memcpy(blob.rgbKeyData, passwd, min(8, _tcslen(key)));
			bResult &= CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 0);
			bResult &= CryptImportKey(hProv, (BYTE*)&blob, sizeof(blob), 0, 0, &hSessionKey);
			bResult &= CryptSetKeyParam(hSessionKey, KP_IV, (BYTE*)IV, 0);
			bResult &= CryptEncrypt(hSessionKey, NULL, TRUE, 0, (BYTE*)buffer, &dataLength, bufferLength);
			bResult &= CryptDestroyKey(hSessionKey);
			bResult &= CryptReleaseContext(hProv, 0);
			return bResult ? dataLength : 0;
		}
		DUI_FUNC_CXX_EXPORT DWORD des_decrypt(BYTE* buffer, DWORD bufferLength, TCHAR key[8])
		{
			CT2CA passwd(key, CP_UTF8);
			DWORD dataLength = bufferLength;
			if (buffer == NULL) return 0;
			HCRYPTPROV hProv = NULL;
			HCRYPTKEY hSessionKey = NULL;
			BOOL bResult = TRUE;
			KeyBlob blob;
			memset(&blob, 0, sizeof(blob));
			blob.header.bType = PLAINTEXTKEYBLOB;
			blob.header.bVersion = CUR_BLOB_VERSION;
			blob.header.reserved = 0;
			blob.header.aiKeyAlg = CALG_DES;
			blob.cbKeySize = 8;
			memcpy(blob.rgbKeyData, passwd, min(8, _tcslen(key)));
			bResult &= CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 0);
			bResult &= CryptImportKey(hProv, (BYTE*)&blob, sizeof(blob), 0, 0, &hSessionKey);
			bResult &= CryptSetKeyParam(hSessionKey, KP_IV, (BYTE*)IV, 0);
			bResult &= CryptDecrypt(hSessionKey, NULL, TRUE, 0, buffer, &dataLength);
			bResult &= CryptDestroyKey(hSessionKey);
			bResult &= CryptReleaseContext(hProv, 0);
			return bResult ? dataLength : 0;
		}

	}
}