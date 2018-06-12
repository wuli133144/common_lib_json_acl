#include <string>
#include "edcode.h"

using namespace std;

#define XXTEA_KEY "!@#^~ku6&%(com)T"

class ku6encrypt
{
	public:
		static string Decode( const string &encfid,const char*key=XXTEA_KEY);
		static string Encode( const string &encfid,const char*key=XXTEA_KEY);

};

