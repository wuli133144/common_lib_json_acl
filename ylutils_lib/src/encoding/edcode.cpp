/*********************************************************************************
 *	Author: Dare Fish
 *  Version: 1.0
 *  Filename: edcode.c
 *
 *
 *  Copyright (c) Thursday, February 14, 2008 Dare Fish.
 *
 *
 ********************************************************************************/

#include "edcode.h"

char *_encode(char *src, char **dst, unsigned char *key)
{
	unsigned char *xxtea, *result;
	int i, xxtea_length, base64_length;

	if((xxtea = xxtea_encrypt((unsigned char *) src, key, &xxtea_length)) == NULL)
		return NULL;
	if((result = base64_encode(xxtea, xxtea_length, &base64_length)) == NULL) {
		free(xxtea);
		return NULL;
	}

	for(i = 0; i < base64_length; i++) {
		switch(result[i]) {
			case 43:
				//replace '+' to '-'
				result[i] = 45;
				break;
			case 47:
				//replace '/' to '_'
				result[i] = 95;
				break;
			case 61:
				//replace '=' to '.'
				result[i] = 46;
				break;
		}
	}

	*dst = strdup((const char*)result);
	free(xxtea);
	free(result);
	return *dst;
}

char *_decode(char *src, char **dst, unsigned char *key)
{
	unsigned char *xxtea, *result;
	int i, xxtea_length, base64_length;

	base64_length = strlen(src);

	for(i = 0; i < base64_length; i++) {
		switch(src[i]) {
			case 45:
				//replace '-' to '+'
				src[i] = 43;
				break;
			case 95:
				//replace '_' to '/'
				src[i] = 47;
				break;
			case 46:
				//replace '.' to '='
				src[i] = 61;
				break;
		}
	}

	if((xxtea = base64_decode((unsigned char*)src, base64_length, &xxtea_length)) == NULL)
		return NULL;
	if((result = xxtea_decrypt(xxtea, key, xxtea_length)) == NULL) {
		free(xxtea);
		return NULL;
	}
	*dst = strdup((const char*)result);
	free(xxtea);
	free(result);
	return *dst;
}

