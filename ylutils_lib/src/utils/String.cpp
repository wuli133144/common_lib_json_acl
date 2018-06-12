/*
 * =====================================================================================
 * 
 *        Filename:  String.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年07月07日 16时16分02秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "String.h"

bool isChineseChar(char c1, char c2)
{
	return (unsigned char)c1>=0x80&&(unsigned char)c1<=0xFE&&(unsigned char)c2>=0x40&&(unsigned char)c2<=0xFE;
}


string &Trim(string &str)
{
	size_t begin = 0;
	for (; begin < str.length(); begin++)
	{
		if (strncmp(str.c_str()+begin, "　", 2)==0)
		{
			begin++;
			continue;
		}
		if (!isspace(str[begin]))
			break;
	}

	if (begin == str.length())
	{
		str.clear();
		return str;
	}

	size_t end = str.length()-1;
	for (; end > begin; end--)
	{
		if (!isspace(str[end]))
			break;
	}

	str = str.substr(begin, end-begin+1);

	return str;
}

string Trim(const char *str)
{
	string ret = str;
	ret = Trim(ret);
	return ret;
}

string str2lower(const char *str)
{
	string _lower;
	while (*str != '\0')
	{
		if (isChineseChar(*str, *(str+1)))
		{
			_lower.append(str, 2);
			str += 2;
		}
		else
		{
			_lower.push_back(tolower(*str));
			str++;
		}
	}

	return _lower;
}

string str2lower(const string &str)
{
	return str2lower(str.c_str());
}

bool isallspace(const string &str)
{
	return isallspace(str.c_str());
}

bool isallspace(const char *str)
{
	while (*str != '\0')
	{
		if (!isspace(*str++))
			return false;
	}

	return true;
}

bool isall(const char *str, int(*func)(int)) {
	while (*str != '\0')
	{
		if (!func(*str++))
			return false;
	}
	return true;
}

#ifdef _TEST

static const char *tests[] = {
	"\t\n\r\n ",
	"          "
};

#define arraysize(array) ((sizeof(array))/sizeof(*array))

int main()
{

	for (size_t i = 0; i < arraysize(tests); i++)
		cout << (isallspace(tests[i])?"is all space":"not all space") << endl;

	return 0;
}
#endif //__TEST
