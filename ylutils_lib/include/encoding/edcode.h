/*********************************************************************************
 *	Author: Dare Fish
 *  Version: 1.0
 *  Filename: edcode.h
 *
 *
 *  Copyright (c) Thursday, February 14, 2008 Dare Fish.
 *
 *
 ********************************************************************************/

#include "xxtea.h"
#include "base64_x.h"
#include <stdlib.h>
#include <string.h>

extern char *_encode(char *src, char **dst, unsigned char *key);
extern char *_decode(char *src, char **dst, unsigned char *key);

