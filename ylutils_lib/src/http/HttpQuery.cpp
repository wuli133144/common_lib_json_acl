/*
 * =====================================================================================
 * 
 *        Filename:  HttpQuery.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月29日 17时00分33秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "HttpQuery.h"

#define XDIGIT_TO_NUM(h) ((h) < 'A' ? (h) - '0' : toupper (h) - 'A' + 10)
#define X2DIGITS_TO_NUM(h1, h2) ((XDIGIT_TO_NUM (h1) << 4) + XDIGIT_TO_NUM (h2))

HttpQuery::HttpQuery()
{
}

HttpQuery::~HttpQuery()
{
}

int HttpQuery::recv(const char *query_string)
{
	mNameValue.clear();
	//strncpy(mRawMsg, query_string, MAXMSGLEN-1);
	mHttpQueryString.resize(strlen(query_string) + 1);
	sprintf((char *)&mHttpQueryString[0], "%s", query_string);

	char *pmsg = mHttpQueryString.begin();
	char *pnamebegin = pmsg;
	char *pvaluebegin = pmsg;
	char *p = pmsg;

	while (*p != '\0')
	{
		if (*p == '&')
		{
			if (*pnamebegin != '=' && *pnamebegin != '&' && 
					*pvaluebegin == '=' && pnamebegin < pvaluebegin)
			{
				*pvaluebegin = '\0';
				*p = '\0';
				char *_p = unescape(pvaluebegin + 1);
				//mNameValue.insert(make_pair(pnamebegin, unescape(pvaluebegin+1)));
				mNameValue.insert(make_pair(pnamebegin, _p));
			}	
			pnamebegin = pvaluebegin = p+1;
		}	
		else if (*p == '=' && *pvaluebegin != '=')
		{
			pvaluebegin = p;
		}

		p++;
	}

	if (*pnamebegin != '=' && *pnamebegin != '&' && 
			*pvaluebegin == '=' && pnamebegin < pvaluebegin)
	{
		*pvaluebegin = '\0';
		mNameValue.insert(make_pair(pnamebegin, unescape(pvaluebegin+1)));
	}

	return 0;
}

char *HttpQuery::unescape (char *s)
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


const char *HttpQuery::get(const char *name)
{
	map<const char *, char *, strcaselt>::const_iterator it = mNameValue.find(name);
	return (it==mNameValue.end())?NULL:it->second;
}

const char *HttpQuery::get(const string &name)
{
	return get(name.c_str());
}

string HttpQuery::get2(const char* name)
{
	const char* s = get(name);
	if(s)
		return string(s);
	else
		return "";
}

HttpQuery &operator<<(HttpQuery &rquery, const string &squery)
{
	rquery.recv(squery.c_str());
	return rquery;
}

ostream &operator<<(ostream &os, HttpQuery &query)
{
	for (map<const char *, char *, HttpQuery::strcaselt>::const_iterator it = query.mNameValue.begin();
			it != query.mNameValue.end();
			it++)
	{
		//os << "[" << it->first << ",\t" << it->second << "]"<< endl;
		os << "[" << it->first << ",\t" << query.get(it->first) << "]" << endl;
	}

	return os;
}

#ifdef _TEST

static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " query_string" << endl;

	cout << "查询串分析器测试 :" << endl << endl;

	cout << "query_string :                       查询串. 格式遵循GET请求的QUERY_STRING" << endl;

	return;
}

int main(int argc, char *argv[])
{

	string line;
	HttpQuery query;

	cout << "input your query string : ";
	while (getline(cin, line))
	{
		query << line;
		cout << query;	
		cout << "input your query string : ";
	}

	cout << endl;

	return 0;
}
#endif //_TEST
