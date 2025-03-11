#ifndef SRC_ONVIF_TOOLS_H
#define SRC_ONVIF_TOOLS_H
/******************************************************************************
*                               Include files
******************************************************************************/
/*******************************************************************************
 *                         Type definition ('typedef')
 ******************************************************************************/
/*******************************************************************************
 *                         symbols/macros ("# define")
 ******************************************************************************/
/*******************************************************************************
 *                      Global variable definition ('extern')
 ******************************************************************************/
/*******************************************************************************
 *                                   api
 ******************************************************************************/

extern void create_random_num(int N, char *random_str);
extern void get_string_para(char *byte_stream, char *para, char *dest, int dest_len);
extern void get_central_string(char *byte_stream, char *left_string, char *right_string, char *dest, int dest_len);
extern void yt_hex_printf(char *name, unsigned int target, char *pBuf, int bufLen);
extern void yt_debug_printf(char *name, char *pBuf);
extern void yt_printf(char *name, char *pBuf);
extern char *mystrcat_s(char *dest, const char *src);
#endif//SRC_ONVIF_TOOLS_H
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
