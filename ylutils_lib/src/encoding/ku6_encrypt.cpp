#include "ku6_encrypt.h"

string ku6encrypt::Decode( const string &encfid, const char* key)
{
	///   12344111
	/// HdWwcxjouh1DCtXka
	if( encfid.empty() )
		return "";

	char buffstr[100];
	memset(buffstr,0,sizeof(buffstr));
	strncpy(buffstr, encfid.c_str(), 98 );

	char *result=NULL;
	char *pRs = _decode((char*)buffstr,(char**)&result,(unsigned char*)key);
	string out; 
	if ( pRs ) out = pRs;
	if( result ) free(result);

	return out;
}

string ku6encrypt::Encode( const string &encfid, const char* key)
{
	///   12344111
	/// HdWwcxjouh1DCtXka
	if( encfid.empty() )
		return "";

	char buffstr[100];
	memset(buffstr,0,sizeof(buffstr));
	strncpy(buffstr, encfid.c_str(), 98 );

	char *result=NULL;
	char *pRs = _encode((char*)buffstr,(char**)&result,(unsigned char*)key);
	string out; 
	if ( pRs ) out = pRs;
	if( result ) free(result);

	return out;
}

#ifdef _TEST

#include <iostream>

int main(int argc, char* argv[])
{	
	string s = "6589986";

	string ret = ku6encrypt::Encode(s, "U!S@E#R^~ku6&K%E(Ycom)T");
	cout<<ret<<endl;

	cout<<ku6encrypt::Decode(ret, "U!S@E#R^~ku6&K%E(Ycom)T")<<endl;

	cout<<ku6encrypt::Encode(s)<<endl;

	return 0;
}

#endif
