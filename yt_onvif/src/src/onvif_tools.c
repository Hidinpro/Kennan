/******************************************************************************
*                               Include files
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "onvif_tools.h"
/*******************************************************************************
 *                         Type definition ('typedef')
 ******************************************************************************/

/*******************************************************************************
 *                   Preprocessor symbols/macros ("# define")
 ******************************************************************************/

/*******************************************************************************
 *   Global variable definition (declared with 'extern' in the header file)
 ******************************************************************************/
/*******************************************************************************
 *                  Local variable definition (“static”)
 ******************************************************************************/
/*******************************************************************************
 *                    Local Function Definition ("staitc")
 ******************************************************************************/

/**
  * @brief  生成随机字符串
  * @param [in]  N               @ref int 随机字符串的长度
  * @param [out] random_str      @ref char* 存储随机字符串的容器
  * @retval None
  */
void create_random_num(int N, char *random_str)
{
    if (NULL == random_str) return;

    int k = 0, j = 0;
    random_str[0] = '\0';

    //1970到现在的时间sec作为种子
    unsigned int seed = (unsigned)time(NULL);
    srand(seed);

    for (j = 0; j < N; j++) {
        short flag;
        unsigned int random_num = rand();
        flag = random_num % 3;

        if (flag == 0) {
            random_str[k++] = '0' + random_num % 10;
        } else if (flag == 1) {
            random_str[k++] = 'a' + random_num % 26;
        } else if (flag == 2) {
            random_str[k++] = 'A' + random_num % 26;
        }

        srand(random_num);
    }

    random_str[k] = '\0';
}

/**
  * @brief  去除信息中的关键字的前面无关字符
  * @param  [in] byte_stream               @ref char* 整个信息字符串
  * @param  [in] para                      @ref char* 关键字
  * @param  [out] dest                     @ref char* 存储字符串的容器
  * @param  [in] dest_len                   @ref int  容器大小
  * @retval None
  */
void get_string_para(char *byte_stream, char *para, char *dest, int dest_len)
{
    if (NULL == byte_stream || NULL == para || NULL == dest) {
        perror("get_string_para error");
        return;
    }

    if (NULL == strstr(byte_stream, para)) {
        perror("get_string_para error NULL");
        return;
    }

    strncpy(dest, strstr(byte_stream, para) + strlen(para), dest_len);
}


/**
  * @brief  从字符串中提取目标字符串
  * @param  [in] byte_stream         @ref char* 整个信息字符串
  * @param  [in] left_string         @ref char* 关键字左边的字符串（匹配整个字符串）
  * @param  [in] right_string        @ref char* 关键字右边的字符串（匹配字符串中的任一一个即停止）
  * @param  [out] dest               @ref char*  结果
  * @param  [out] dest_len           @ref int  结果大小
  * @retval None
  */
void get_central_string(char *byte_stream, char *left_string, char *right_string, char *dest, int dest_len)
{
    if (NULL == byte_stream || NULL == left_string || NULL == right_string || NULL == dest || dest_len <= 0) {
        perror("Get Central String Error");
        return;
    }

    get_string_para(byte_stream, left_string, dest, dest_len);

    if (strlen(dest)) {
        strncpy(dest, strtok(dest, "<"), dest_len);
    }
}

/**
  * @brief  将字符串打印为十六进制
  * @param  [in] name           @ref char* 打印的16进制字符串的名称
  * @param  [in] target         @ref unsigned int 打印的字符串次序
  * @param  [in] pBuf           @ref char* 要打印的字符串
  * @param  [in] bufLen         @ref int  要打印的字符串长度
  * @retval None
  */
void yt_hex_printf(char *name, unsigned int target, char *pBuf, int bufLen)
{
    int i;

    if (pBuf == NULL) return;

    printf("%s[%u]:[", name, target);

    for (i = 0; i < bufLen; i++) {
        printf("%02X ", pBuf[i] & 0xFF);
    }

    printf("]\r\n");
}

void yt_debug_printf(char *name, char *pBuf)
{

#ifdef DEBUG

    printf("\r\n>> %s : %s\r\n", name, pBuf);

#endif

}

void yt_printf(char *name, char *pBuf)
{
    printf("\r\n>> %s : %s\r\n", name, pBuf);
}


/**
  * @brief  拼接字符串
  * @param  [in] dest        @ref char* 目标字符串
  * @param  [in] src         @ref const char* 用于拼接的字符串
  * @retval None
  */
char *mystrcat_s(char *dest, const char *src)
{
    assert(dest != NULL && src != NULL);
    char *temp = dest;

    while (*temp != '\0') temp++;

    while ((*temp++ = *src++) != '\0');

    return dest;
}

/*******************************************************************************
 *                                  END
 ******************************************************************************/
