/******************************************************************************
*                               Include files
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <openssl/sha.h>

#include "onvif_capabilities.h"
#include "onvif_encode.h"
#include "onvif_tools.h"
#include "onvif_curl.h"

/*******************************************************************************
 *                   Preprocessor symbols/macros ("# define")
 ******************************************************************************/

#define DEVICE_ADDR             "http://172.18.0.10/onvif/device_service"
#define GETCAPABILITIES_ACTION  "http://www.onvif.org/ver10/device/wsdl/GetCapabilities"
#define GETPROFILES_ACTION      "http://www.onvif.org/ver10/media/wsdl/GetProfiles"
#define GETSNAPSHOTURI_ACTION   "http://www.onvif.org/ver10/media/wsdl/GetSnapshotUri"

#define USERNAME_SIZE            (30)
#define NONCE_SIZE               (20)
#define NONCE_BASE64_SIZE        (30)
#define CREATED_SIZE             (40)
#define PASSWD_SECURITY_SIZE     (128)
#define POST_DATA_SIZE           (3000)
#define RESPONSE_SIZE            (20000)
#define PROFILES_SIZE            (128)
#define MEDIA_ADDRESS_SIZE       (128)
/*******************************************************************************
 *                         Type definition ('typedef')
 ******************************************************************************/

struct username_token_info {
    char user_name[USERNAME_SIZE];
    char nonce_base64[NONCE_BASE64_SIZE];
    char created[CREATED_SIZE];
    char passwd_security[PASSWD_SECURITY_SIZE];
};


/*******************************************************************************
 *   Global variable definition (declared with 'extern' in the header file)
 ******************************************************************************/
/*******************************************************************************
 *                    Local variable Definition ("staitc")
 ******************************************************************************/
/*******************************************************************************
 *                    Local Function Definition ("staitc")
 ******************************************************************************/

static void get_created(struct username_token_info *username_token);
static void make_WS_Security_passwd(struct username_token_info *username_token);
static void username_token_init(struct username_token_info *username_token);
static void make_get_apabilities_post(char *post_data, int post_len, struct username_token_info *username_token);
static void make_get_profiles_post(char *post_data, int post_len, struct username_token_info *username_token);
static void make_get_snapshot_url_post(char *post_data, int post_len, struct username_token_info *username_token, char *profiles);
/*******************************************************************************
 *       Function Implementation - Global ("extern") and Local ("static")
 ******************************************************************************/

/**
  * @brief  username_token结构体初始化
  * @param  [in] username_token       	@ref struct username_token_info*         需要初始化的结构体
  * @param  None
  * @retval None
*/
static void username_token_init(struct username_token_info *username_token)
{
    memset(username_token, 0, sizeof(struct username_token_info));
    strncpy(username_token->user_name, USERNAME, USERNAME_SIZE);
    get_created(username_token);
    make_WS_Security_passwd(username_token);
}

/**
  * @brief  获取Created
  * @param  [in] username_token       	@ref struct username_token_info*         UsernameToken结构体
  * @param  None
  * @retval None
*/
static void get_created(struct username_token_info *username_token)
{
    time_t currentTime;
    struct tm *localTime;

    // 获取当前时间
    currentTime = time(NULL);
    // 转换为本地时间
    localTime = localtime(&currentTime);

    sprintf(username_token->created, "%04d-%02d-%02dT%02d:%02d:%02dZ", localTime->tm_year + 1900, localTime->tm_mon + 1,
            localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
}

/**
  * @brief  制作WS_Security规则鉴权下的密码
  * @param  [in] username_token       	@ref struct username_token_info*         UsernameToken结构体
  * @param  None
  * @retval None
*/
static void make_WS_Security_passwd(struct username_token_info *username_token)
{
    /*初始化所需数组*/
    char nonce[NONCE_SIZE];
    memset(nonce, 0, sizeof(nonce));
    char sha1[20];
    memset(sha1, 0, sizeof(sha1));
    char temp[128];
    memset(temp, 0, sizeof(temp));

    /*创建created与nonce以及B64ENCODE(nonce)*/
    create_random_num(NONCE_SIZE, nonce);
    base64_encode((unsigned char *)nonce, username_token->nonce_base64, NONCE_SIZE);

    /*计算SHA1( B64DECODE( Nonce ) + Date + Password ) */
    snprintf(temp, sizeof(temp), "%s%s%s", nonce, username_token->created, PASSWD);
    SHA1(temp, strlen(temp), sha1);

    /*计算B64ENCODE( SHA1( B64DECODE( Nonce ) + Date + Password ) )*/
    base64_encode((unsigned char *)sha1, username_token->passwd_security, 20);

}

/**
  * @brief  制作 get_apabilities 行动在http发送时的post数据
  * @param  [in] post_data       	@ref char*              post数据
  * @param  [in] post_len       	@ref int                post数据长度
  * @param  [in] username_token     @ref struct username_token_info*   UsernameToken结构体
  * @retval None
*/
static void make_get_apabilities_post(char *post_data, int post_len, struct username_token_info *username_token)
{
    snprintf(post_data, post_len,
             "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\" xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" xmlns:c14n=\"http://www.w3.org/2001/10/xml-exc-c14n#\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:saml1=\"urn:oasis:names:tc:SAML:1.0:assertion\" xmlns:saml2=\"urn:oasis:names:tc:SAML:2.0:assertion\" xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" xmlns:xenc=\"http://www.w3.org/2001/04/xmlenc#\" xmlns:wsc=\"http://docs.oasis-open.org/ws-sx/ws-secureconversation/200512\" xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:tdn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\"><SOAP-ENV:Header><wsse:Security SOAP-ENV:mustUnderstand=\"true\"><wsse:UsernameToken><wsse:Username>%s</wsse:Username><wsse:Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">%s</wsse:Password><wsse:Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">%s</wsse:Nonce><wsu:Created>%s</wsu:Created></wsse:UsernameToken></wsse:Security></SOAP-ENV:Header><SOAP-ENV:Body><tds:GetCapabilities></tds:GetCapabilities></SOAP-ENV:Body></SOAP-ENV:Envelope> ",
             username_token->user_name, username_token->passwd_security, username_token->nonce_base64, username_token->created);
}

/**
  * @brief  制作 get_profiles 行动在http发送时的post数据
  * @param  [in] post_data       	@ref char*              post数据
  * @param  [in] post_len       	@ref int                post数据长度
  * @param  [in] username_token     @ref struct username_token_info*   UsernameToken结构体
  * @retval None
*/
static void make_get_profiles_post(char *post_data, int post_len, struct username_token_info *username_token)
{
    snprintf(post_data, post_len,
             "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\" xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" xmlns:c14n=\"http://www.w3.org/2001/10/xml-exc-c14n#\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:saml1=\"urn:oasis:names:tc:SAML:1.0:assertion\" xmlns:saml2=\"urn:oasis:names:tc:SAML:2.0:assertion\" xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" xmlns:xenc=\"http://www.w3.org/2001/04/xmlenc#\" xmlns:wsc=\"http://docs.oasis-open.org/ws-sx/ws-secureconversation/200512\" xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:tdn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\"><SOAP-ENV:Header><wsse:Security SOAP-ENV:mustUnderstand=\"true\"><wsse:UsernameToken><wsse:Username>%s</wsse:Username><wsse:Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">%s</wsse:Password><wsse:Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">%s</wsse:Nonce><wsu:Created>%s</wsu:Created></wsse:UsernameToken></wsse:Security></SOAP-ENV:Header><SOAP-ENV:Body><trt:GetProfiles></trt:GetProfiles></SOAP-ENV:Body></SOAP-ENV:Envelope>",
             username_token->user_name, username_token->passwd_security, username_token->nonce_base64, username_token->created);
}

/**
  * @brief  制作 get_snapshot_url 行动在http发送时的post数据
  * @param  [in] post_data       	@ref char*              post数据
  * @param  [in] post_len       	@ref int                post数据长度
  * @param  [in] username_token     @ref struct username_token_info*   UsernameToken结构体
  * @param  [in] profiles       	@ref char*              选择的码流
  * @retval None
*/
static void make_get_snapshot_url_post(char *post_data, int post_len, struct username_token_info *username_token, char *profiles)
{
    snprintf(post_data, post_len,
             "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\" xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" xmlns:c14n=\"http://www.w3.org/2001/10/xml-exc-c14n#\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:saml1=\"urn:oasis:names:tc:SAML:1.0:assertion\" xmlns:saml2=\"urn:oasis:names:tc:SAML:2.0:assertion\" xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" xmlns:xenc=\"http://www.w3.org/2001/04/xmlenc#\" xmlns:wsc=\"http://docs.oasis-open.org/ws-sx/ws-secureconversation/200512\" xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:tdn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\"><SOAP-ENV:Header><wsse:Security SOAP-ENV:mustUnderstand=\"true\"><wsse:UsernameToken><wsse:Username>%s</wsse:Username><wsse:Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">%s</wsse:Password><wsse:Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">%s</wsse:Nonce><wsu:Created>%s</wsu:Created></wsse:UsernameToken></wsse:Security></SOAP-ENV:Header><SOAP-ENV:Body><trt:GetSnapshotUri><trt:ProfileToken>%s</trt:ProfileToken></trt:GetSnapshotUri></SOAP-ENV:Body></SOAP-ENV:Envelope>",
             username_token->user_name, username_token->passwd_security, username_token->nonce_base64, username_token->created, profiles);

}

/**
  * @brief  获取设备能力中的媒体服务地址
  * @param  [in] media_address  @ref char*         媒体服务地址
  * @param  [in] in_size       	@ref size_t        传入的字符串长度
  * @retval 返回函数执行结果 0成功 1失败
*/
int onvif_get_capabilities(char *media_address, size_t in_size)
{
    char post_data[POST_DATA_SIZE];
    memset(post_data, 0, sizeof(post_data));
    char response[RESPONSE_SIZE];
    memset(response, 0, sizeof(response));

    struct username_token_info username_token;
    username_token_init(&username_token);

    make_get_apabilities_post(post_data, sizeof(post_data), &username_token);
    onvif_send_http(DEVICE_ADDR, strlen(post_data), GETCAPABILITIES_ACTION, post_data, response);

    get_central_string(response, "<tt:Media><tt:XAddr>", "<", media_address, in_size);

    if (!strlen(media_address)){
        yt_printf("Get Capabilities Error", "Media Address No Found!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
  * @brief  获取设备视频码流简介
  * @param  [in] profiles       @ref char*         视频码流简介
  * @param  [in] in_size       	@ref size_t        传入的字符串长度
  * @param  [in] stream       	@ref enum stream_list  选择的码流
  * @retval 返回函数执行结果 0成功 1失败
*/
int onvif_get_profiles(char *profiles, size_t in_size, enum stream_list stream)
{
    char post_data[POST_DATA_SIZE];
    memset(post_data, 0, sizeof(post_data));
    char response[RESPONSE_SIZE];
    memset(response, 0, sizeof(response));

    struct username_token_info username_token;
    username_token_init(&username_token);

    make_get_profiles_post(post_data, sizeof(post_data), &username_token);
    onvif_send_http(DEVICE_ADDR, strlen(post_data), GETPROFILES_ACTION, post_data, response);

    get_string_para(response, "<trt:Profiles token=\"", profiles, in_size);

    if (MAINSTREAM != stream) {
        get_string_para(profiles, "<trt:Profiles token=\"", profiles, in_size);
    }

    if (THIRDSTREAM == stream) {
        get_string_para(profiles, "<trt:Profiles token=\"", profiles, in_size);
    }

    if (strlen(profiles)) {
        strncpy(profiles, strtok(profiles, "\""), in_size);
        return EXIT_SUCCESS;
    } else {
        yt_printf("Get Profiles Error", "Profiles Token No Found!");
        return EXIT_FAILURE;
    }

}

/**
  * @brief  获取设备抓图地址
  * @param  [in] snapshot_url   @ref char*         抓图地址
  * @param  [in] in_size       	@ref size_t        传入的字符串长度
  * @param  [in] stream       	@ref enum stream_list  选择的码流
  * @retval 返回函数执行结果 0成功 1失败
*/
int onvif_get_snapshot_url(char *snapshot_url, size_t in_size, enum stream_list stream)
{
    char post_data[POST_DATA_SIZE];
    memset(post_data, 0, sizeof(post_data));
    char response[RESPONSE_SIZE];
    memset(response, 0, sizeof(response));
    char profiles[PROFILES_SIZE];
    memset(profiles, 0, sizeof(profiles));
    char media_address[MEDIA_ADDRESS_SIZE];
    memset(media_address, 0, sizeof(media_address));

    struct username_token_info username_token;
    username_token_init(&username_token);

    if (onvif_get_capabilities(media_address, sizeof(media_address))){
        return EXIT_FAILURE;
    }

    if (onvif_get_profiles(profiles, sizeof(profiles), stream)){
        return EXIT_FAILURE;
    }

    make_get_snapshot_url_post(post_data, sizeof(post_data), &username_token, profiles);
    memset(response, 0, sizeof(response));
    onvif_send_http(media_address, strlen(post_data), GETSNAPSHOTURI_ACTION, post_data, response);

    get_central_string(response, "<trt:MediaUri><tt:Uri>", "<", snapshot_url, in_size);

    if (!strlen(snapshot_url)){
        yt_printf("Get SnapShot Url Error", "SnapShot Url No Found!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
/*******************************************************************************
 *                                  END
 ******************************************************************************/
