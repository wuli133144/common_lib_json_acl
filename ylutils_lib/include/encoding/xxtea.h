/*********************************************************************************
 *	Author: Dare Fish
 *  Version: 1.0
 *  Filename: xxtea.h
 *
 *
 *  Copyright (c) Wednesday, February 13, 2008 Dare Fish.
 *
 *
 ********************************************************************************/

#include <stddef.h>
#include <stdint.h>

#define XXTEA_MX (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z)
#define XXTEA_DELTA 0x9e3779b9
typedef uint32_t xxtea_long;

extern unsigned char *xxtea_encrypt(unsigned char *data, unsigned char *key, int *len);
extern unsigned char *xxtea_decrypt(unsigned char *data, unsigned char *key, int len);

