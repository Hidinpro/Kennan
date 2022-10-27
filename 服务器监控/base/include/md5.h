#ifndef MD5_H
#define MD5_H


#include <stdint.h>

// 根据文件路径获取文件的MD5值
uint8_t *getmd5_path(const char *path, uint8_t *encrypt);

// 根据文件描述符获取文件的MD5值
uint8_t *getmd5_fd(int fd, uint8_t *encrypt);

// 根据文件指针获取文件的MD5值
uint8_t *getmd5_file(FILE *frp, uint8_t *encrypt);

// 根据字符串获取MD5值
uint8_t *getmd5_str(char *str, uint8_t *encrypt);

// 比较两个MD5值
int md5cmp(const uint8_t *enc1, const uint8_t *enc2);

// 显示MD5值
void show_md5(const uint8_t *encrypt);

typedef struct md5
{
	uint32_t count[2];
	uint32_t state[4];
	uint8_t buffer[64];
} MD5;

void md5_init(MD5 *context);
void md5_update(MD5 *context, uint8_t *input, uint32_t inputlen);
void md5_final(MD5 *context, uint8_t digest[16]);

#endif

