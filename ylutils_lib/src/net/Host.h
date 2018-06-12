/*
 * =====================================================================================
 * 
 *        Filename:  Host.h
 * 
 *     Description:  这个类用来做DNS解析，
 * 
 *         Version:  1.0
 *         Created:  2005年10月20日 15时34分34秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 * 
 * =====================================================================================
 */
/** @addtogroup net
 *  
 *  
 *  @{
 */

#ifndef HOST_H_ZMH_2005_10_20
#define HOST_H_ZMH_2005_10_20

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;
/*!
 *\brief   DNSException
*! \class DNSException Host.h net/Host.h   
 */
class DNSException : public std::exception
{
public :
	//! constructor
	DNSException(const string &msg) throw() : mMsg(msg) {}
	//! constructor
	DNSException(int err) : mMsg(strerror(err)) {} 
	//! destructor
	virtual ~DNSException() throw()	{}
	//! destructor
	virtual const char *what() const throw() {return mMsg.c_str();}

private :
	string mMsg;
};
/*!
 *\brief   打印操作帮助
*! \class ConvertAndPrint Host.h net/Host.h   
 */
struct ConvertAndPrint : public unary_function<string, void>
{
public :
	ConvertAndPrint(ostream &os, int addrType) : _os(os), _addrType(addrType) {}
	
	void
	operator ()(const string &element) const
	{
		char _ipBuffer[256];
		bzero(_ipBuffer, sizeof(_ipBuffer));
		
		_os << inet_ntop(_addrType, element.c_str(), _ipBuffer, sizeof(_ipBuffer)) << endl;

		return;
	}
	
private :
	ostream &_os;
	int      _addrType;
};
/*!
 *\brief   这个类用来做DNS解析，
*! \class Host Host.h net/Host.h   
\code
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
\endcode 
 */
class Host
{
public :
	//! 构造函数. name可以是域名，也可以是ip.
	Host(const string &name) throw(DNSException);

	//! 构造函数. name可以是域名，也可以是ip.
	Host(const char *name) throw(DNSException);

	//! 析构子.
	virtual ~Host();

	//! 返回该域名主机对应的实际ip的数量.
	int size() const;

	//! 返回第pos个网络地址.
	string &at(int pos);
	//! 随机的返回一个网络地址.
	string getRandIP() const;

	//! IPV4||IPV6  AF_NET or AF_NET6
	int getAddrType() const;
	

	//InetAddress inetAddr() const;
	u_int32_t inetAddr() const;

	//! 这是一个输出操作的重载，用来做测试.
	friend ostream &operator <<(ostream &os, const Host &host);	

protected :

private :
	//! 分析name指向的主机域名，将分析结果填入相应的成员.
	void getHostByName(const char *name) throw(DNSException);

protected :
	string	       mName;		//!< 传入的原始名字.
	string 	       mOName;		//!< 该域名的正式名字.
	int	       mAddrType;	//!< 主机地址类型，一般是指IPV4或IPV6.
	vector<string> mAliases;	//!< 该域名的所有别名.
	vector<string> mAddrs;		//!< 该域名对应的所有网络地址.
	
};

ostream &operator <<(ostream &os, const Host &host);	
#endif //HOST_H_ZMH_2005_10_20
/** @} */ // end of 
