#include "stdafx.h"
#include "libwraper.h"

namespace DUI_API
{
	namespace DUI_ENCODE {

		void myMD4_SubHASH(DWORD InputData[16], DWORD HashValue[4]) {

#define C2  013240474631     /* round 2 constant = sqrt(2) in octal */
#define C3  015666365641     /* round 3 constant = sqrt(3) in octal */

#define f(X,Y,Z)             ((X&Y) | ((~X)&Z))
#define g(X,Y,Z)             ((X&Y) | (X&Z) | (Y&Z))
#define h(X,Y,Z)             (X^Y^Z)

#define ff(A,B,C,D,i,s)      A = myMD4SRL32((A + f(B,C,D) + InputData[i]),s)
#define gg(A,B,C,D,i,s)      A = myMD4SRL32((A + g(B,C,D) + InputData[i] + C2),s)
#define hh(A,B,C,D,i,s)      A = myMD4SRL32((A + h(B,C,D) + InputData[i] + C3),s)

#define fs1  3               /* round 1 shift amounts */
#define fs2  7
#define fs3 11
#define fs4 19
#define gs1  3               /* round 2 shift amounts */
#define gs2  5
#define gs3  9
#define gs4 13
#define hs1  3               /* round 3 shift amounts */
#define hs2  9
#define hs3 11
#define hs4 15

#define myMD4SRL32(x,n) (((x)<<n)|((x)>>(32-n)))

			DWORD A, B, C, D;

			A = HashValue[0];
			B = HashValue[1];
			C = HashValue[2];
			D = HashValue[3];
			/* Update the message digest buffer */
			ff(A, B, C, D, 0, fs1); /* Round 1 */
			ff(D, A, B, C, 1, fs2);
			ff(C, D, A, B, 2, fs3);
			ff(B, C, D, A, 3, fs4);
			ff(A, B, C, D, 4, fs1);
			ff(D, A, B, C, 5, fs2);
			ff(C, D, A, B, 6, fs3);
			ff(B, C, D, A, 7, fs4);
			ff(A, B, C, D, 8, fs1);
			ff(D, A, B, C, 9, fs2);
			ff(C, D, A, B, 10, fs3);
			ff(B, C, D, A, 11, fs4);
			ff(A, B, C, D, 12, fs1);
			ff(D, A, B, C, 13, fs2);
			ff(C, D, A, B, 14, fs3);
			ff(B, C, D, A, 15, fs4);
			gg(A, B, C, D, 0, gs1); /* Round 2 */
			gg(D, A, B, C, 4, gs2);
			gg(C, D, A, B, 8, gs3);
			gg(B, C, D, A, 12, gs4);
			gg(A, B, C, D, 1, gs1);
			gg(D, A, B, C, 5, gs2);
			gg(C, D, A, B, 9, gs3);
			gg(B, C, D, A, 13, gs4);
			gg(A, B, C, D, 2, gs1);
			gg(D, A, B, C, 6, gs2);
			gg(C, D, A, B, 10, gs3);
			gg(B, C, D, A, 14, gs4);
			gg(A, B, C, D, 3, gs1);
			gg(D, A, B, C, 7, gs2);
			gg(C, D, A, B, 11, gs3);
			gg(B, C, D, A, 15, gs4);
			hh(A, B, C, D, 0, hs1); /* Round 3 */
			hh(D, A, B, C, 8, hs2);
			hh(C, D, A, B, 4, hs3);
			hh(B, C, D, A, 12, hs4);
			hh(A, B, C, D, 2, hs1);
			hh(D, A, B, C, 10, hs2);
			hh(C, D, A, B, 6, hs3);
			hh(B, C, D, A, 14, hs4);
			hh(A, B, C, D, 1, hs1);
			hh(D, A, B, C, 9, hs2);
			hh(C, D, A, B, 5, hs3);
			hh(B, C, D, A, 13, hs4);
			hh(A, B, C, D, 3, hs1);
			hh(D, A, B, C, 11, hs2);
			hh(C, D, A, B, 7, hs3);
			hh(B, C, D, A, 15, hs4);
			HashValue[0] += A;
			HashValue[1] += B;
			HashValue[2] += C;
			HashValue[3] += D;

		}

		void myMD4_Run(const unsigned char *pData, unsigned _int64 LenData, DWORD OutInt[4])
		{
			OutInt[0] = 0x67452301;
			OutInt[1] = 0xefcdab89;
			OutInt[2] = 0x98badcfe;
			OutInt[3] = 0x10325476;

			unsigned _int64	TheLen(LenData), i(0);
			BYTE	Data64[64] = { 0 };

			while (TheLen >= 64)
			{
				memcpy(Data64, pData + LenData - TheLen, 64);
				myMD4_SubHASH((DWORD *)Data64, OutInt);
				TheLen -= 64;
			}

			if (TheLen < 56)
			{
				BYTE	LastData64[64] = { 0 };
				for (i = 0; i < TheLen; i++)
					LastData64[i] = pData[LenData - TheLen + i];
				LastData64[TheLen] = 0x80;
				((unsigned _int64 *)LastData64)[7] = (LenData << 3);
				myMD4_SubHASH((DWORD *)LastData64, OutInt);
			}
			else
			{
				BYTE	LastData128[128] = { 0 };
				for (i = 0; i < TheLen; i++)
					LastData128[i] = pData[LenData - TheLen + i];
				LastData128[TheLen] = 0x80;
				((unsigned _int64 *)LastData128)[15] = (LenData << 3);
				myMD4_SubHASH((DWORD *)LastData128, OutInt);
				myMD4_SubHASH((DWORD *)LastData128 + 16, OutInt);
			}

			return;
		}

		DUI_FUNC_C_EXPORT unsigned char* md4_(const unsigned char *buf, size_t size, unsigned char result[ZEN_MD4_HASH_SIZE])
		{
			DWORD OutInt[4];
			myMD4_Run(buf, size, OutInt);
			memcpy(result, (unsigned char*)&OutInt, 16);
			return result;
		}

	}

}