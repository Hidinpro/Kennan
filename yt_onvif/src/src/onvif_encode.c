/******************************************************************************
*                               Include files
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#include "onvif_encode.h"


/*******************************************************************************
 *                         Type definition ('typedef')
 ******************************************************************************/

/*******************************************************************************
 *                   Preprocessor symbols/macros ("# define")
 ******************************************************************************/

#define soap_coblank(c)         ((c)+1 > 0 && (c) <= 32)

/*******************************************************************************
 *   Global variable definition (declared with 'extern' in the header file)
 ******************************************************************************/
/*******************************************************************************
 *                  Local variable definition (“static”)
 ******************************************************************************/

static const char base64o[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64i[81] =
    "\76XXX\77\64\65\66\67\70\71\72\73\74\75XXXXXXX\00\01\02\03\04\05\06\07\10\11\12\13\14\15\16\17\20\21\22\23\24\25\26\27\30\31XXXXXX\32\33\34\35\36\37\40\41\42\43\44\45\46\47\50\51\52\53\54\55\56\57\60\61\62\63";

/*******************************************************************************
 *                    Local Function Definition ("staitc")
 ******************************************************************************/
/*******************************************************************************
 *       Function Implementation - Global ("extern") and Local ("static")
 ******************************************************************************/
/**
  * @brief  更改md5格式
  * @param  [in] data       	@ref void         要加密的内容
  * @param  [in] length       	@ref int          要加密的内容的长度
  * @param  [out] *md5_str      @ref void         加密结果
  * @retval None
*/
void muotu_md5_str(void *data, int length, void *md5_str)
{
    unsigned char md5[16] = {0};
    MD5(data, length, md5);
    sprintf(md5_str, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            md5[0] & 0xFF, md5[1] & 0xFF, md5[2] & 0xFF, md5[3] & 0xFF,
            md5[4] & 0xFF, md5[5] & 0xFF, md5[6] & 0xFF, md5[7] & 0xFF,
            md5[8] & 0xFF, md5[9] & 0xFF, md5[10] & 0xFF, md5[11] & 0xFF,
            md5[12] & 0xFF, md5[13] & 0xFF, md5[14], md5[15] & 0xFF);
}


/**
  * @brief  base64编码
  * @param  [in] s       	@ref const unsigned char *      要加密的内容
  * @param  [in] n       	@ref int                        加密结果的长度
  * @param  [out] t       	@ref char*                      加密结果
  * @retval char* 加密结果
*/
char *base64_encode(const unsigned char *s, char *t, int n)
{
    int i;
    unsigned long m;
    char *p;

    if (!t) {
        return NULL;
    }

    p = t;
    t[0] = '\0';

    if (!s)
        return p;

    for (; n > 2; n -= 3, s += 3) {
        m = s[0];
        m = (m << 8) | s[1];
        m = (m << 8) | s[2];

        for (i = 4; i > 0; m >>= 6)
            t[--i] = base64o[m & 0x3F];

        t += 4;
    }

    t[0] = '\0';

    if (n > 0) { /* 0 < n <= 2 implies that t[0..4] is allocated (base64 scaling formula) */
        m = 0;

        for (i = 0; i < n; i++)
            m = (m << 8) | *s++;

        for (; i < 3; i++)
            m <<= 8;

        for (i = 4; i > 0; m >>= 6)
            t[--i] = base64o[m & 0x3F];

        for (i = 3; i > n; i--)
            t[i] = '=';

        t[4] = '\0';
    }

    return p;
}

/**
  * @brief  base64解码
  * @param  [in] s       	@ref const unsigned char *      要解码的内容
  * @param  [in] l       	@ref size_t                     解码内容的长度
  * @param  [out] t       	@ref char*                      解码结果
  * @param  [out] n       	@ref int *                      解码结果的长度
  * @retval const char* 解码结果
*/
const char *base64_decode(const char *s, char *t, size_t l, int *n)
{
    if (n)
        *n = 0;

    if (!s || !*s)
        return NULL;

    if (!t)
        return NULL;


    char c;
    const char *p;

    p = t;

    for (int i = 0; ; i += 3, l -= 3) {
        int j = 0;
        unsigned long m = 0;

        while (j < 4) {
            c = *s++;

            if (c == '=' || !c) {
                if (l >= j - 1) {
                    switch (j) {
                    case 2:
                        *t++ = (char)((m >> 4) & 0xFF);
                        i++;
                        l--;
                        break;

                    case 3:
                        *t++ = (char)((m >> 10) & 0xFF);
                        *t++ = (char)((m >> 2) & 0xFF);
                        i += 2;
                        l -= 2;
                    }
                }

                if (n)
                    *n = (int)i;

                if (l)
                    *t = '\0';

                return p;
            }

            c -= '+';

            if (c >= 0 && c <= 79) {
                int b = base64i[c];

                if (b >= 64) {
                    return NULL;
                }

                m = (m << 6) + b;
                j++;
            } else if (!soap_coblank(c + '+')) {
                return NULL;
            }
        }

        if (l < 3) {
            if (n)
                *n = (int)i;

            if (l)
                *t = '\0';

            return p;
        }

        *t++ = (char)((m >> 16) & 0xFF);
        *t++ = (char)((m >> 8) & 0xFF);
        *t++ = (char)(m & 0xFF);
    }
}

/*******************************************************************************
 *                                  END
 ******************************************************************************/
