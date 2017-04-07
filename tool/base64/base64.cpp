#include "base64.h"

CBase64::CBase64()
{
}

int CBase64::EncodeBase64(const uint8_t *pSrc, uint8_t *pDst, uint32_t nSrcLen)
{
    if (0 == pSrc || 0 == nSrcLen)
    {
            return -1;
    }

    uint8_t c1, c2, c3;
    uint32_t nDstLen = 0;
    uint32_t nDiv = nSrcLen / 3;
    uint32_t nMod = nSrcLen % 3;

    for (uint32_t i = 0; i < nDiv; i++)
    {
        c1 = *pSrc++;
        c2 = *pSrc++;
        c3 = *pSrc++;

        *pDst++ = myBase64Table[c1 >> 2];
        *pDst++ = myBase64Table[((c1 << 4) | c2 >> 4) & 0x3f];
        *pDst++ = myBase64Table[((c2 << 2) | c3 >> 6) & 0x3f];
        *pDst++ = myBase64Table[c3 & 0x3f];
        nDstLen += 4;
    }

    if (1 == nMod)
    {
        *pDst++ = myBase64Table[(*pSrc) >> 2];
        *pDst++ = myBase64Table[((*pSrc) << 4) & 0x3f];
        *pDst++ = '=';
        *pDst++ = '=';
        nDstLen += 4;
    }
    else if (2 == nMod)
    {
        c1 = *pSrc++;
        c2 = *pSrc++;

        *pDst++ = myBase64Table[c1 >> 2];
        *pDst++ = myBase64Table[((c1 << 4) | c2 >> 4) & 0x3f];
        *pDst++ = myBase64Table[c2 >> 2];
        *pDst++ = '=';
        nDstLen += 4;
    }

    *pDst = '\0';
    return nDstLen;
}

int CBase64::DecodeBase64(const uint8_t *pSrc, uint8_t *pDst, uint32_t nSrcLen)
{
    if (0 == pSrc || 0 == nSrcLen)
    {
        return -1;
    }

    uint32_t nDstLen = 0;
    uint8_t pc; // now char ptr
    uint32_t ncode = 0;
    uint32_t dc = 0; //'=' count
    uint32_t nc = 0; // get char count
    for (uint32_t i = 0; i < nSrcLen; i++)
    {
        pc = myBase64Map[*pSrc++];

        if (pc == 0xff)continue;
        if (pc == 0xfe){ pc = 0; dc++; }

        ncode = (ncode << 6) | pc;

        if (++nc == 4)
        {
            *pDst++ = (uint8_t)(ncode >> 16 & 0xff);
            nDstLen++;
            if (dc < 2)
            {
                *pDst++ = (uint8_t)((ncode >> 8) & 0xff);
                nDstLen++;
            }
            if (dc < 1)
            {
                *pDst++ = (uint8_t)(ncode & 0xff);
                nDstLen++;
            }
            nc = 0;
            ncode = 0;
        }
    }
    *pDst = '\0';
    return nDstLen;
}
