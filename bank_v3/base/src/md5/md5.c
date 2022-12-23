#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "md5.h"

// 根据文件路径获取文件的MD5值
uint8_t *getmd5_path(const char *path, uint8_t *encrypt)
{
	FILE *frp = fopen(path, "rb");
	if (NULL == frp)
	{
		perror("fopen");
		return NULL;
	}

	getmd5_file(frp, encrypt);
	fclose(frp);

	return encrypt;
}

// 根据文件描述符获取文件的MD5值
uint8_t *getmd5_fd(int fd, uint8_t *encrypt)
{
	FILE *frp = fdopen(fd, "rb");
	if (NULL == frp)
	{
		perror("fdopen");
		return NULL;
	}

	getmd5_file(frp, encrypt);
	fclose(frp);

	return encrypt;
}

// 根据文件指针获取文件的MD5值
uint8_t *getmd5_file(FILE *frp, uint8_t *encrypt)
{
	if (NULL == frp || NULL == encrypt)
	{
		printf("frp pointer or encrypt NULL\n");
		return NULL;
	}

	MD5_CTX md5;
	md5_init(&md5);
	int ret = 0;
	uint8_t buf[1024] = {};

	rewind(frp);
	while (0 < (ret = fread(buf, 1, sizeof(buf), frp)))
	{
		md5_update(&md5, buf, ret);
	}

	md5_final(&md5, encrypt);

	return encrypt;
}

// 根据字符串获取MD5值
uint8_t *getmd5_str(char *str, uint8_t *encrypt)
{
	if (NULL == str || NULL == encrypt)
	{
		printf("str pointer or encrypt NULL\n");
		return NULL;
	}

	MD5_CTX md5;
	md5_init(&md5);
	md5_update(&md5, (uint8_t*)str, strlen(str));
	md5_final(&md5, encrypt);

	return encrypt;
}

// 比较两个MD5值
int md5cmp(const uint8_t *enc1, const uint8_t *enc2)
{
	return memcmp(enc1, enc2, 16);
}

// 显示MD5值
void show_md5(const uint8_t *encrypt)
{
	printf("md5:");
	for (int i = 0; i < 16; i++)
	{
		printf("%02x", encrypt[i]);
	}
	printf("\n");
}

int main()
{
	FILE *frp = fopen("md5.h", "rb");
	if (NULL == frp)
	{
		perror("fopen");
		return -1;
	}
	uint8_t md5[16] = {};
	show_md5(getmd5_file(frp, md5));
	show_md5(getmd5_str("zzxx", md5));
}


#define F(x, y, z) ((x & y) | (~x & z))
#define G(x, y, z) ((x & z) | (y & ~z))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))
#define ROTATE_LEFT(x, n) ((x << n) | (x >> (32 - n)))
#define FF(a, b, c, d, x, s, ac)  \
	{                             \
		a += F(b, c, d) + x + ac; \
		a = ROTATE_LEFT(a, s);    \
		a += b;                   \
	}
#define GG(a, b, c, d, x, s, ac)  \
	{                             \
		a += G(b, c, d) + x + ac; \
		a = ROTATE_LEFT(a, s);    \
		a += b;                   \
	}
#define HH(a, b, c, d, x, s, ac)  \
	{                             \
		a += H(b, c, d) + x + ac; \
		a = ROTATE_LEFT(a, s);    \
		a += b;                   \
	}
#define II(a, b, c, d, x, s, ac)  \
	{                             \
		a += I(b, c, d) + x + ac; \
		a = ROTATE_LEFT(a, s);    \
		a += b;                   \
	}

void md5_trans_form(uint32_t state[4], uint8_t block[64]);
void md5_encode(uint8_t *output, uint32_t *input, uint32_t len);
void md5_dencode(uint32_t *output, uint8_t *input, uint32_t len);

uint8_t panding[] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void md5_init(MD5_CTX *context)
{
	context->count[0] = 0;
	context->count[1] = 0;
	context->state[0] = 0x67452301;
	context->state[1] = 0xEFCDAB89;
	context->state[2] = 0x98BADCFE;
	context->state[3] = 0x10325476;
}

void md5_update(MD5_CTX *context, uint8_t *input, uint32_t inputlen)
{
	uint32_t i = 0, index = 0, partlen = 0;
	index = (context->count[0] >> 3) & 0x3F;
	partlen = 64 - index;
	context->count[0] += inputlen << 3;
	if (context->count[0] < (inputlen << 3))
	{
		context->count[1]++;
	}
	context->count[1] += inputlen >> 29;

	if (inputlen >= partlen)
	{
		memcpy(&context->buffer[index], input, partlen);
		md5_trans_form(context->state, context->buffer);
		for (i = partlen; i + 64 <= inputlen; i += 64)
		{
			md5_trans_form(context->state, &input[i]);
		}
		index = 0;
	}
	else
	{
		i = 0;
	}

	memcpy(&context->buffer[index], &input[i], inputlen - i);
}

void md5_final(MD5_CTX *context, uint8_t digest[16])
{
	uint32_t index = 0, padlen = 0;
	uint8_t bits[8];
	index = (context->count[0] >> 3) & 0x3F;
	padlen = (index < 56) ? (56 - index) : (120 - index);
	md5_encode(bits, context->count, 8);
	md5_update(context, panding, padlen);
	md5_update(context, bits, 8);
	md5_encode(digest, context->state, 16);
}
void md5_encode(uint8_t *output, uint32_t *input, uint32_t len)
{
	uint32_t i = 0, j = 0;
	while (j < len)
	{
		output[j++] = (input[i] >> 0) & 0xFF;
		output[j++] = (input[i] >> 8) & 0xFF;
		output[j++] = (input[i] >> 16) & 0xFF;
		output[j++] = (input[i] >> 24) & 0xFF;
		i++;
	}
}
void md5_dencode(uint32_t *output, uint8_t *input, uint32_t len)
{
	uint32_t i = 0, j = 0;
	while (j < len)
	{
		output[i] = (input[j]) | (input[j + 1] << 8) | (input[j + 2] << 16) | (input[j + 3] << 24);
		i++;
		j += 4;
	}
}
void md5_trans_form(uint32_t state[4], uint8_t block[64])
{
	uint32_t a = state[0];
	uint32_t b = state[1];
	uint32_t c = state[2];
	uint32_t d = state[3];
	uint32_t x[64];

	md5_dencode(x, block, 64);

	FF(a, b, c, d, x[0], 7, 0xd76aa478);
	FF(d, a, b, c, x[1], 12, 0xe8c7b756);
	FF(c, d, a, b, x[2], 17, 0x242070db);
	FF(b, c, d, a, x[3], 22, 0xc1bdceee);
	FF(a, b, c, d, x[4], 7, 0xf57c0faf);
	FF(d, a, b, c, x[5], 12, 0x4787c62a);
	FF(c, d, a, b, x[6], 17, 0xa8304613);
	FF(b, c, d, a, x[7], 22, 0xfd469501);
	FF(a, b, c, d, x[8], 7, 0x698098d8);
	FF(d, a, b, c, x[9], 12, 0x8b44f7af);
	FF(c, d, a, b, x[10], 17, 0xffff5bb1);
	FF(b, c, d, a, x[11], 22, 0x895cd7be);
	FF(a, b, c, d, x[12], 7, 0x6b901122);
	FF(d, a, b, c, x[13], 12, 0xfd987193);
	FF(c, d, a, b, x[14], 17, 0xa679438e);
	FF(b, c, d, a, x[15], 22, 0x49b40821);

	GG(a, b, c, d, x[1], 5, 0xf61e2562);
	GG(d, a, b, c, x[6], 9, 0xc040b340);
	GG(c, d, a, b, x[11], 14, 0x265e5a51);
	GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);
	GG(a, b, c, d, x[5], 5, 0xd62f105d);
	GG(d, a, b, c, x[10], 9, 0x2441453);
	GG(c, d, a, b, x[15], 14, 0xd8a1e681);
	GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);
	GG(a, b, c, d, x[9], 5, 0x21e1cde6);
	GG(d, a, b, c, x[14], 9, 0xc33707d6);
	GG(c, d, a, b, x[3], 14, 0xf4d50d87);
	GG(b, c, d, a, x[8], 20, 0x455a14ed);
	GG(a, b, c, d, x[13], 5, 0xa9e3e905);
	GG(d, a, b, c, x[2], 9, 0xfcefa3f8);
	GG(c, d, a, b, x[7], 14, 0x676f02d9);
	GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);

	HH(a, b, c, d, x[5], 4, 0xfffa3942);
	HH(d, a, b, c, x[8], 11, 0x8771f681);
	HH(c, d, a, b, x[11], 16, 0x6d9d6122);
	HH(b, c, d, a, x[14], 23, 0xfde5380c);
	HH(a, b, c, d, x[1], 4, 0xa4beea44);
	HH(d, a, b, c, x[4], 11, 0x4bdecfa9);
	HH(c, d, a, b, x[7], 16, 0xf6bb4b60);
	HH(b, c, d, a, x[10], 23, 0xbebfbc70);
	HH(a, b, c, d, x[13], 4, 0x289b7ec6);
	HH(d, a, b, c, x[0], 11, 0xeaa127fa);
	HH(c, d, a, b, x[3], 16, 0xd4ef3085);
	HH(b, c, d, a, x[6], 23, 0x4881d05);
	HH(a, b, c, d, x[9], 4, 0xd9d4d039);
	HH(d, a, b, c, x[12], 11, 0xe6db99e5);
	HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
	HH(b, c, d, a, x[2], 23, 0xc4ac5665);

	II(a, b, c, d, x[0], 6, 0xf4292244);
	II(d, a, b, c, x[7], 10, 0x432aff97);
	II(c, d, a, b, x[14], 15, 0xab9423a7);
	II(b, c, d, a, x[5], 21, 0xfc93a039);
	II(a, b, c, d, x[12], 6, 0x655b59c3);
	II(d, a, b, c, x[3], 10, 0x8f0ccc92);
	II(c, d, a, b, x[10], 15, 0xffeff47d);
	II(b, c, d, a, x[1], 21, 0x85845dd1);
	II(a, b, c, d, x[8], 6, 0x6fa87e4f);
	II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
	II(c, d, a, b, x[6], 15, 0xa3014314);
	II(b, c, d, a, x[13], 21, 0x4e0811a1);
	II(a, b, c, d, x[4], 6, 0xf7537e82);
	II(d, a, b, c, x[11], 10, 0xbd3af235);
	II(c, d, a, b, x[2], 15, 0x2ad7d2bb);
	II(b, c, d, a, x[9], 21, 0xeb86d391);
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}
