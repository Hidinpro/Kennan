#ifndef SRC_ONVIF_ENCODE_H
#define SRC_ONVIF_ENCODE_H
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

extern void muotu_md5_str(void *data, int length, void *md5_str);
extern char *base64_encode(const unsigned char *s, char *t, int n);
extern const char *base64_decode(const char *s, char *t, size_t l, int *n);
#endif//SRC_ONVIF_ENCODE_H
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
