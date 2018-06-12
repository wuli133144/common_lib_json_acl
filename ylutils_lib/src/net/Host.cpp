/*
 * =====================================================================================
 * 
 *        Filename:  Host.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2005年10月26日 11时31分26秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), zmh@netera.cn
 * 
 * =====================================================================================
 */


#include <iterator>
#include <netdb.h>
#include "Host.h"


extern int h_errno;

Host::Host(const string &name) throw(DNSException): mName(name)
{
	this->getHostByName(mName.c_str());
}

Host::Host(const char *name) throw(DNSException): mName(name)
{
	this->getHostByName(mName.c_str());
}

Host::~Host()
{
}

void Host::getHostByName(const char *name) throw(DNSException)
{
	struct hostent *phost = gethostbyname(name);

	if (phost != NULL)
	{
		mOName=phost->h_name;
		for (char** pptr=phost->h_aliases; *pptr!=0; pptr++)
			mAliases.push_back(*pptr);
		for (char**pptr=phost->h_addr_list; *pptr!=0; pptr++)
			mAddrs.push_back(string(*pptr, phost->h_length));
		mAddrType = phost->h_addrtype;
	}
	else 
	{
		string msg = string("Host::getHostByName(") + name + ") : " + strerror(h_errno); 
		throw DNSException(msg);
	}

	return;
}



int Host::size() const
{
	return mAddrs.size();
}

string &Host::at(int pos)
{
	return mAddrs.at(pos);
}

string Host::getRandIP() const
{
	ostringstream oss;
	u_int32_t ip = *(u_int32_t *)mAddrs.at(0).c_str();
	oss << (u_int)(ip&0xff) << '.' << (u_int)((ip>>8)&0xff) << '.' << (u_int)((ip>>16)&0xff) << '.' << (u_int)((ip>>24)&0xff);
	return oss.str();
}

u_int32_t Host::inetAddr() const
{
	return *(u_int32_t *)mAddrs.at(0).c_str();
}

ostream &operator <<(ostream &os, const Host &host)
{
	os << "your input : " << host.mName << endl;
	os << "the official name : " << host.mOName << endl;
	os << "the host address type : " << ((host.mAddrType==AF_INET)?"AF_INET":"AF_INET6") << endl;
	os << "the alias list : " << endl;
	copy(host.mAliases.begin(), host.mAliases.end(), ostream_iterator<string>(os, "\n"));
	os << "the following " << host.size() << " ip : " << endl;
	for_each(host.mAddrs.begin(), host.mAddrs.end(), ConvertAndPrint(os, host.mAddrType));

	return os;
}


#ifdef _TEST

int main()
{
	string domain;

	cout << "please input your domain : ";

	while (cin >> domain)
	{
		//try 
		//{
			cout << Host(domain) << endl;	
		//}
		//catch (DNSException &E)
		//{
		//	cout << E.what() << endl;
		//}
		cout << "please input your domain : ";
	}

	cout << "Bye !!!" << endl;

	return 0;
}
#endif //_TEST
