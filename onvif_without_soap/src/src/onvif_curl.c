
/******************************************************************************
*                               Include files
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>

#include "onvif_curl.h"
#include "onvif_encode.h"
#include "onvif_tools.h"
#include "onvif_capabilities.h"

/*******************************************************************************
 *                         Type definition ('typedef')
 ******************************************************************************/
struct authorization_info {
    char username[32];
    char passwd[32];
    char url_no_auth[64];
    char realm[32];
    char nonce[64];
    char uri[128];
    char cnonce[64];
    char nc[16];
    char qop[16];
    char respense[64];
};

/*******************************************************************************
 *                   Preprocessor symbols/macros ("# define")
 ******************************************************************************/
#define  FALSE 1
#define  TRUE  0
#define  UNAUTHORIZED_401      (401)        //HTTP/1.1 401 Unauthorized
#define  CURL_NC               ("00000001")
#define  CURL_METHOD           ("GET")
#define  CNONCE_LEN            (44)


/*******************************************************************************
 *   Global variable definition (declared with 'extern' in the header file)
 ******************************************************************************/

/*******************************************************************************
 *                    Local Function Definition ("staitc")
 ******************************************************************************/

static void get_auth_info(char *url, struct authorization_info *authorization);
static int httpauth_get_response(struct authorization_info *login, char *cmd);
static void make_req_authorization(char *res_buf, struct authorization_info authorization, char *req_authorization, size_t req_len);
static size_t file_write_func_callback(void *ptr, size_t size, size_t nmemb, void *stream);
static size_t strcat_func_callback(char *b, size_t size, size_t nitems, void *userdata);
/*******************************************************************************
 *                  Local variable definition (“static”)
 ******************************************************************************/
/*******************************************************************************
 *       Function Implementation - Global ("extern") and Local ("static")
 ******************************************************************************/


/**
  * @brief  获取鉴权信息、uri、不带鉴权信息的url
  * @param  [in] *url                @ref char                      带鉴权的url
  * @param  [out] *authorization      @ref struct authorization_info 鉴权信息结构体
  * @retval None
  */
static void get_auth_info(char *url, struct authorization_info *authorization)
{
    if (NULL == url || NULL == authorization) {
        perror("get_auth_info error");
        return;
    }

    strncpy(authorization->uri, strstr(url, "/onvif-http/"), sizeof(authorization->uri));
    strncpy(authorization->passwd,   PASSWD,  sizeof(authorization->passwd));
    strncpy(authorization->username, USERNAME, sizeof(authorization->username));
}

/**
  * @brief  获取鉴权信息中的respense
  * @param  [in] *authorization      @ref struct authorization_info 鉴权信息结构体
  * @param  [in] *cmd                @ref char 获取资源的方式
  * @retval None
  */
static int httpauth_get_response(struct authorization_info *login, char *cmd)
{
    if (NULL == login || NULL == cmd) {
        perror("httpauth error");
        return FALSE;
    }

    char ha1[64] = {0};
    char ha2[64] = {0};
    char tmpStr[512] = {0};
    sprintf(tmpStr, "%s:%s:%s", login->username, login->realm, login->passwd);
    muotu_md5_str(tmpStr, strlen(tmpStr), ha1);
    memset(tmpStr, 0, sizeof(tmpStr));
    sprintf(tmpStr, "%s:%s", cmd, login->uri);
    muotu_md5_str(tmpStr, strlen(tmpStr), ha2);
    memset(tmpStr, 0, sizeof(tmpStr));
    sprintf(tmpStr, "%s:%s:%s:%s:%s:%s", ha1, login->nonce, CURL_NC, login->cnonce, login->qop, ha2);
    muotu_md5_str(tmpStr, strlen(tmpStr), login->respense);
    return TRUE;
}



/**
  * @brief  拼接鉴权信息
  * @param  [in] *res_buf                   @ref char                        整个鉴权信息字符串
  * @param  [in] authorization              @ref struct authorization_info   鉴权信息结构体
  * @retval None
  */
static void make_req_authorization(char *res_buf, struct authorization_info authorization, char *req_authorization, size_t req_len)
{
    if (NULL == res_buf) {
        return;
    }

    /*从收到的http消息中解析下面三个参数*/
    get_string_para(res_buf, "nonce=\"", authorization.nonce, sizeof(authorization.nonce));
    get_string_para(res_buf, "qop=\"",   authorization.qop,   sizeof(authorization.qop));
    get_string_para(res_buf, "realm=\"", authorization.realm, sizeof(authorization.realm));

    /*解析成功则进行消息拼接*/
    if (strlen(authorization.nonce) > 0 && strlen(authorization.qop) > 0 && strlen(authorization.realm) > 0) {
        strncpy(authorization.nonce,    strtok(authorization.nonce, "\""), sizeof(authorization.nonce));
        strncpy(authorization.qop,      strtok(authorization.qop,   "\""), sizeof(authorization.qop));
        strncpy(authorization.realm,    strtok(authorization.realm, "\""), sizeof(authorization.realm));
        create_random_num(CNONCE_LEN, authorization.cnonce);
        httpauth_get_response(&authorization, CURL_METHOD);
        snprintf(req_authorization, req_len,
                 "Authorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", cnonce=\"%s\",nc=%s, qop=%s,  response=\"%s\"",
                 authorization.username, authorization.realm, authorization.nonce, authorization.uri, authorization.cnonce, CURL_NC,
                 authorization.qop, authorization.respense);
    }

}


/**
  * @brief  接收收到的内容
  * @param  [in] *ptr                    @ref void     接收到的消息
  * @param  [in] size                    @ref size_t   消息大小
  * @param  [in] nmemb                   @ref size_t   消息个数
  * @param  [out] *stream                 @ref void     文件指针或其他容器
  * @retval 接收到的内容总大小       @ref size_t
  */
static size_t file_write_func_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

/**
  * @brief  接收收到的http头内容
  * @param  [in] *b                      @ref void     接收到的消息
  * @param  [in] size                    @ref size_t   消息大小
  * @param  [in] nitems                  @ref size_t   消息个数
  * @param  [out] *userdata               @ref void     文件指针或其他容器
  * @retval 接收到的内容总大小       @ref size_t
  */
static size_t strcat_func_callback(char *b, size_t size, size_t nitems, void *userdata)
{
    size_t numbytes = size * nitems;
    char *tmp = userdata;
    mystrcat_s(tmp, b);                 //拼接http消息
    return numbytes;
}


/**
  * @brief  获取文件
  * @param  [in] *filename                   @ref char 文件名
  * @param  [in] *onvif_url                  @ref char   onvif地址
  * @retval None
  */
void curl_get(char *filename, char *onvif_url)
{
    struct authorization_info authorization;
    memset(&authorization, 0, sizeof(authorization));
    long response_code = 0;
    char curl_buf[1024];
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    FILE *fp;
    if ((fp = fopen(filename, "wb")) == NULL) { // 返回结果用文件存储
        return;
    }

    get_auth_info(onvif_url, &authorization);

    curl = curl_easy_init();                            // curl初始化

    if (curl) {
        //curl_easy_setopt(curl, CURLOPT_VERBOSE,     1U);      //打印调试信息
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH,    CURLAUTH_DIGEST);   //设置鉴权方式
        curl_easy_setopt(curl, CURLOPT_HTTPGET,     1U);   //设置GET方法
        curl_easy_setopt(curl, CURLOPT_URL,         onvif_url);   //设置链接url
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,   fp);         //将接收的内容输出到fp指向的文件
        curl_easy_setopt(curl, CURLOPT_HEADERDATA,  &curl_buf);  //将接收到的http内容输出到curl_buf中
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  file_write_func_callback);                    //设置回调函数
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, strcat_func_callback);

        res = curl_easy_perform(curl);   // 执行

        if (res != CURLE_OK) {
            perror("curl_res_error!");
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        /*如果需要鉴权则再次发送鉴权信息*/
        if (UNAUTHORIZED_401 == response_code) {
            char req_authorization[512];        //鉴权信息
            make_req_authorization(curl_buf, authorization, req_authorization, sizeof(req_authorization));   //获取鉴权信息
            headers = curl_slist_append(headers, req_authorization);                                        //在消息头中增加鉴权信息
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            memset(curl_buf, 0, sizeof(curl_buf));
            curl_easy_perform(curl);
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        fclose(fp);
    }
}

/**
  * @brief  发送onvif的http数据
  * @param  [in] url            @ref char*          发送地址
  * @param  [in] content_len    @ref size_t         post数据长度
  * @param  [in] action       	@ref char*          进行的行动
  * @param  [in] post_data      @ref char*          post数据
  * @param  [out] response      @ref char*          回应的数据
  * @retval None
*/
void onvif_send_http(char *url, size_t content_len, char *action, char *post_data, char *response)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    char header_content_type[256];
    memset(header_content_type, 0, sizeof(header_content_type));
    char header_soapaction[128];
    memset(header_soapaction, 0, sizeof(header_soapaction));
    char header_content_len[32];
    memset(header_content_len, 0, sizeof(header_content_len));

    snprintf(header_content_type, sizeof(header_content_type), "Content-Type: application/soap+xml; charset=utf-8; action=\"%s\"",
             action);
    snprintf(header_soapaction, sizeof(header_soapaction), "SOAPAction: \"%s\"", action);
    snprintf(header_content_len, sizeof(header_content_len), "Content-Length: %zu", content_len);

    curl = curl_easy_init();                            // curl初始化

    if (curl) {
        //curl_easy_setopt(curl, CURLOPT_VERBOSE,     1U);                                         //打印调试信息
        curl_easy_setopt(curl, CURLOPT_POST,        1U);                                        //设置GET方法
        curl_easy_setopt(curl, CURLOPT_URL,         url);                               //设置链接url
        headers = curl_slist_append(headers,
                                    "User-Agent: gSOAP/2.8");                                        //在消息头中增加鉴权信息
        headers = curl_slist_append(headers, header_content_type);
        headers = curl_slist_append(headers, header_content_len);
        headers = curl_slist_append(headers, "Connection: close");
        headers = curl_slist_append(headers, "Accept-Encoding: deflate");
        headers = curl_slist_append(headers, header_soapaction);
        headers = curl_slist_append(headers, "Expect:"); /* initalize custom header list */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,  post_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                         response);                                        //将接收的内容输出到fp指向的文件
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, strcat_func_callback);
        res = curl_easy_perform(curl);   // 执行

        if (res != CURLE_OK) {
            perror("curl_res_error!");
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}


/*******************************************************************************
 *                                  END
 ******************************************************************************/