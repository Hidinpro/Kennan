#ifndef SRC_ONVIF_CAPABILITIES_H
#define SRC_ONVIF_CAPABILITIES_H
/******************************************************************************
*                               Include files
******************************************************************************/
/*******************************************************************************
 *                         Type definition ('typedef')
 ******************************************************************************/

enum stream_list {
    MAINSTREAM  = 0,
    SUBSTREAM   = 1,
    THIRDSTREAM = 2
};

/*******************************************************************************
 *                         symbols/macros ("# define")
 ******************************************************************************/

#define USERNAME "admin"
#define PASSWD   "Xinzailing_Web"

/*******************************************************************************
 *                      Global variable definition ('extern')
 ******************************************************************************/
/*******************************************************************************
 *                                   api
 ******************************************************************************/

extern int onvif_get_capabilities(char *media_address, size_t in_size);
extern int onvif_get_profiles(char *profiles, size_t in_size, enum stream_list stream);
extern int onvif_get_snapshot_url(char *snapshot_url, size_t in_size, enum stream_list stream);
#endif//SRC_ONVIF_CAPABILITIES_H
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
