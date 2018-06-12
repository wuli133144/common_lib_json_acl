#include "URLDecoder.h"

#define XDIGIT_TO_NUM(h) ((h) < 'A' ? (h) - '0' : toupper (h) - 'A' + 10)
#define X2DIGITS_TO_NUM(h1, h2) ((XDIGIT_TO_NUM (h1) << 4) + XDIGIT_TO_NUM (h2))

char* unescape (char *s)
{
	char *t = s;                  /* t - tortoise */
	char *h = s;                  /* h - hare     */

	for (; *h; h++, t++)
	{
		if (*h != '%')
		{
copychar:
			*t = (*h=='+')?' ':*h;
		}
		else
		{
			char c;
			/* Do nothing if '%' is not followed by two hex digits. */
			if (!h[1] || !h[2] || !(isxdigit (h[1]) && isxdigit (h[2])))
				goto copychar;
			c = X2DIGITS_TO_NUM (h[1], h[2]);
			/* Don't unescape %00 because there is no way to insert it
			 *              into a C string without effectively truncating it. */
			if (c == '\0')
				goto copychar;
			*t = c;
			h += 2;
		}
	}
	*t = '\0';

	return s;
}

URLDecoder::URLDecoder()
{
}

URLDecoder::~URLDecoder()
{
}


string URLDecoder::decode( const string& url )
{
	char* buf = new char[url.size()+1];
	strcpy(buf,url.c_str());
	unescape(buf);
	string ret(buf);
	delete[] buf;
	return ret;
}
