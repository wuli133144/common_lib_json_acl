/*
 * =====================================================================================
 * 
 *        Filename:  Config.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年04月04日 18时41分41秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "Config.h"
#include <errno.h>


Config::Config()
{
}

Config::Config(const string &cfgfile) : mConfigFile(cfgfile)
{
}

Config::~Config()
{
}

bool Config::load(const string &cfgfile) throw(IOException)
{
	if (!cfgfile.empty())
		mConfigFile = cfgfile;

	ifstream ifConfig(mConfigFile.c_str());
	if (!ifConfig)
	{
		ostringstream oexcp;
		oexcp << "Config::load(" << cfgfile << ") : " << strerror(errno);
		throw IOException(oexcp.str());
	}

	mNameValue.clear();

	string line;
	while (getline(ifConfig, line))
	{
		line = line.substr(0, line.find('#'));
		trim(line);
		if (line.empty())
			continue;
		size_t pos;
		if ((pos=line.find('=')) == string::npos)
		{
			continue;
		}
		
		string name, value;
		name = line.substr(0, pos);
		value = line.substr(pos+1);
		trim(name);
		trim(value);

		if (name.empty())
			continue;

		mNameValue.insert(make_pair(name, value));
	}
	ifConfig.close();

	return true;
}

bool Config::getValue(string &value, const string &name)
{
	map<string, string>::iterator it = mNameValue.find(name);
	if (it == mNameValue.end())
		return false;

	value = it->second;

	return true;
}

const char *Config::getValue(const string &name)
{
	map<string, string>::iterator it = mNameValue.find(name);
	if (it == mNameValue.end())
		return NULL;

	return it->second.c_str();
}
bool Config::getValueOfLong(long &value, const string &name)
{
	string sValue;
	if (!getValue(sValue, name))
		return false;

	if (sValue[0] != '0' || sValue.length() <= 1)
	{
		sscanf(sValue.c_str(), "%ld", &value);
		
	} else if (sValue[1] != 'X' && sValue[1] != 'x')
	{
		sscanf(sValue.c_str(), "%o", &value);
	} else
	{
		sscanf(sValue.c_str(), "%x", &value);
	}

	return true;
}

bool Config::getValueOfInt(int &value, const string &name)
{
	string sValue;
	if (!getValue(sValue, name))
		return false;

	if (sValue[0] != '0' || sValue.length() <= 1)
	{
		sscanf(sValue.c_str(), "%d", &value);
	} else if (sValue[1] != 'X' && sValue[1] != 'x')
	{
		sscanf(sValue.c_str(), "%o", &value);
	} else
	{
		sscanf(sValue.c_str(), "%x", &value);
	}

	return true;
}

string Config::toString()
{
	ostringstream oss;

	map<string, string>::iterator it = mNameValue.begin();
	for (; it != mNameValue.end(); it++)
	{
		oss << '[' << it->first << ',' << it->second << ']' << endl;
	}

	return oss.str();
}

ostream &operator<<(ostream &os, const Config &config)
{
	map<string, string>::const_iterator it = config.mNameValue.begin();
	for (; it != config.mNameValue.end(); it++)
	{
		os << '[' << it->first << ',' << it->second << ']' << endl;
	}

	return os;
}

string Config::trim(string &src)
{
	if (src.empty())
		return "";

	size_t begin = 0;

	while ((begin < src.length()) and ((src[begin]=='\n') or (src[begin]==' ') or (src[begin]=='\t')
				))
		begin++;

	size_t end = src.length()-1;

	while ((end > begin) and ((src[end]=='\n') or (src[end]==' ') or (src[end]=='\t')))
		end--;

	src = src.substr(begin, end-begin+1);

	return src;
}

#ifdef _TEST

static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " configfile" << endl;

	return;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		dohelp(argc, argv);
		return -1;
	}

	Config config(argv[1]);
	config.load();

	cout << config.toString() << endl;
	int value = 0;
	cout << (config.getValueOfInt(value, "int")?value:0) << endl;
	cout << (config.getValueOfInt(value, "long")?value:0) << endl;
	long lvalue=0;
	cout << (config.getValueOfLong(lvalue, "long")?lvalue:0) << endl;

	return 0;
}
#endif //_TEST
