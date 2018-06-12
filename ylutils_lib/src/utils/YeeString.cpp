#include "YeeString.h"
#include <stdarg.h>
#include <string.h>

YeeString::YeeString()
{
}

YeeString::~YeeString()
{
}

void YeeString::Format(string& str, const char* szFmt, ...)
{
	va_list argList;
	int len;
	char* buf;

	va_start( argList, szFmt );
//	len = vprintf(szFmt, argList) + 1;
	len = 1024;

	buf = new char[len];
	bzero(buf, len);

	vsprintf(buf, szFmt, argList);
	str = string(buf, len - 1);

    delete[] buf;

	va_end( argList );
}

void YeeString::TrimLeft(string& str, const string &strTarget/* =" \r\n\t" */)
{
	string::size_type pos = str.find_first_not_of(strTarget);
	if(pos != string::npos){
		str.assign(str.substr(pos));
	}else{
		str.clear();
	}
}

void YeeString::TrimRight(string& str, const string &strTarget/* =" \r\n\t" */)
{
	string::size_type pos = str.find_last_not_of(strTarget);	
	if(pos != string::npos){
		str.assign(str.substr(0,pos+1));
	}else{
		str.clear();
	}
}

void YeeString::Trim(string& str, const string &strTarget/* =" \r\n\t" */)
{
	TrimLeft(str, strTarget);
	TrimRight(str, strTarget);
}

/**
 * Tokenize the given string
 */ 
void YeeString::Tokenize(const string& str, vector<string>& tokens, const string& delims){
	//ensure the tokens vector is empty.
	tokens.clear();
	string::size_type last_pos,pos;	
	pos = last_pos = 0;	

	do{
		last_pos = pos;
		last_pos = str.find_first_not_of(delims,last_pos);
		if(last_pos == string::npos){
			break;
		}
		pos = str.find_first_of(delims, last_pos);
		string token = str.substr(last_pos, pos-last_pos);
		tokens.push_back(token);
	}while(pos != string::npos);

}

bool YeeString::EndsWith(const string& str, const string& endStr)
{
	string::size_type pos = str.find(endStr,str.length() - endStr.length());
	return pos != string::npos;
}

bool YeeString::StartsWith(const string& str, const string& startStr)
{
	string::size_type pos = str.find(startStr);
	return pos == 0;
}

string YeeString::ToString(int a)
{
	char buf[64];
//	itoa(a, buf, 10);
	sprintf(buf, "%d", a);
	string strRet = buf;
	return strRet;
}

void YeeString::ReplaceAll(string& str, const string& strToReplaced, const string& strReplaced)
{
	string strTemp;
	string::size_type pos, tpos = 0;
	bool bHasReplaced = false;

	pos = str.find(strToReplaced);
	while(pos != string::npos) {
		strTemp += str.substr(tpos, pos - tpos) + strReplaced;		
		tpos = pos + strToReplaced.length();
		pos = str.find(strToReplaced, tpos);
		bHasReplaced = true;
	}

	if(bHasReplaced) {
		strTemp += str.substr(tpos);
		str = strTemp;
	}
}

void YeeString::ToLowerCase(string& str)
{
	for(size_t i=0; i<str.length(); ++i) {
		char ch = str.at(i);
		if(ch >= 'A' && ch <= 'Z')
			str.at(i) = ch + 'a' - 'A';	
	}
}

void YeeString::RemoveHtmlTags(string& strText)
{
	string::size_type pos = 0, end = 0;

	pos = strText.find("<", pos);
	while(pos != string::npos) {
		end = strText.find(">", pos);
		if(end == string::npos)
			break;

		if(end-pos+1 < 1024) {
			strText.replace(pos, end-pos+1, "");
			pos = strText.find("<", pos);
		}else {
			pos = strText.find("<", end+1);
		}
	}
}

float YeeString::CalStringSim(const char* s1, const char* s2, int n)
{
	if(!s1 || *s1==0 || !s2 || *s2==0)//NULL or empty string
		return 0;

	int len1=strlen(s1);
	int len2=strlen(s2);
	int common=0;
	if(len1<n && len1<=len2)
	{
		//common=(count_ngram(s2, s1, len1)>0?1:0);
		common=((strstr(s2, s1)==NULL)?0:1);
		return (float(common))/(len2-len1+1);
	}
	if(len2<n && len2<=len1)
	{
		common=((strstr(s1, s2)==NULL)?0:1);
		return (float(common))/(len1-len2+1);
	}       
	typedef map<string, int> ngram_map;
	ngram_map imap; 
	ngram_map::iterator itr;
	for(int i=0; i<len2-n+1; i++)
		imap[string(&s2[i], n)]++;
	for(int i=0; i<len1-n+1; i++)
	{
		itr=imap.find(string(&s1[i], n));
		if(itr!=imap.end())
		{
			common++;
			itr->second--;
			if(itr->second==0)
				imap.erase(itr);
		}               
	}                       
	return (float(2*common))/(len1+len2-2*n+2);
}

string YeeString::GetCurrTime()
{
	time_t a = time(0);
	char buf[32];

	if(ctime_r(&a, buf) == NULL)
		return "unknown time";

	string t = string(buf);
	return t.substr(0, t.length()-1);
}

void YeeString::SimpleTokenize(const string& str, vector<string>& tokens, const string& delims)
{
        tokens.clear();
        string::size_type p0 = 0, p1;

        while(true) {
                p1 = str.find(delims, p0);
                if(p1 == string::npos) {
                        tokens.push_back(str.substr(p0));
                        break;
                }else {
                        tokens.push_back(str.substr(p0, p1-p0));
                        p0 = p1 + delims.length();
                }
        }
}

void YeeString::create_sign64(const char *str_src,int slen,unsigned int *psign1,unsigned int *psign2)
{
	int times, i;
	char  tstr[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned int sign1, sign2, tint;

	/* if string length <=8, do not change it */
	if (slen <= 8){
		strcpy(tstr, str_src);
		*psign1 = *((unsigned int *)tstr);
		*psign2 = *((unsigned int *)(tstr+4));
		return;
	}

	/* get times */
	times = slen/4;

	/* get the sign */
	sign1=sign2=0;
	for (i=0; i<times; i++){
		if (i & 1)
			sign1 += *( (unsigned int *) (str_src + i*4) );
		else
			sign2 += *( (unsigned int *) (str_src + i*4) );
	}

	times%=32;
	sign1 = (sign1<<times)|(sign1>>(32-times));
	sign2 = (sign2<<times)|(sign2>>(32-times));

	/* the tail of the string */
	tint=0;
	memcpy((char*)(&tint),str_src+i*4,slen-i*4);
//	strcpy( (char*)(&tint), str_src+i*4);
	sign1 += tint;
	sign2 += tint;

	/* return */
	*psign1 = sign1;
	*psign2 = sign2;
}

void YeeString::create_sign64(const char *str_src, unsigned int *psign1, unsigned int *psign2)
{
	int slen = (int)strlen(str_src);
	create_sign64(str_src, slen, psign1, psign2);
}

void YeeString::create_sign64(const char *str_src, unsigned long *psign)
{
	unsigned int sig1, sig2;
	create_sign64(str_src, &sig1, &sig2);
	*psign = sig1;
	*psign = (*psign)<<32 + sig2;
}

void YeeString::RemoveSubstr(string& songname, string sBegin, string sEnd)
{
        string::size_type p0, p1;

        p0 = songname.find(sBegin);
        if(p0 == string::npos)
                return;

//      p1 = songname.find(sEnd, p0 + sBegin.length());
        p1 = songname.rfind(sEnd);
        if(p1 == string::npos)
                return;
	p1 += sEnd.length();

        string s = songname.substr(p0, p1-p0);
        YeeString::ReplaceAll(songname, s, "");
}

bool YeeString::isChineseChar(char c1, char c2)
{
	return (unsigned char)c1>=0x80&&(unsigned char)c1<=0xFE&&(unsigned char)c2>=0x40&&(unsigned char)c2<=0xFE;
}

bool YeeString::isallspace(const string &str)
{
	return isallspace(str.c_str());
}

bool YeeString::isallspace(const char *str)
{
	while (*str != '\0')
	{
		if (!isspace(*str++))
			return false;
	}

	return true;
}

bool YeeString::isall(const char *str, int(*func)(int))
{
	while (*str != '\0')
	{
		if (!func(*str++))
			return false;
	}
	return true;
}

string YeeString::ExtractSubstr(string& src, string sBegin, string sEnd)
{
	string::size_type p = src.find(sBegin);
	if(p == string::npos)
		return "";
	p += sBegin.length();
	string::size_type q = src.find(sEnd, p);
	if(q == string::npos)
		return "";
	return src.substr(p, q-p); 
}

string YeeString::ExtractSubstr2(string& src, string sBegin, string sEnd)
{
	string::size_type p = src.find(sBegin);
	if(p == string::npos)
		return "";
	p += sBegin.length();
	string::size_type q = src.find(sEnd, p);
	if(q == string::npos)
		return src.substr(p);
	return src.substr(p, q-p); 
}

