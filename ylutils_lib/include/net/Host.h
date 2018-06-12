/*
 * =====================================================================================
 * 
 *        Filename:  Host.h
 * 
 *     Description:  �����������DNS������
 * 
 *         Version:  1.0
 *         Created:  2005��10��20�� 15ʱ34��34�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
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
 *\brief   ��ӡ��������
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
 *\brief   �����������DNS������
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
	//! ���캯��. name������������Ҳ������ip.
	Host(const string &name) throw(DNSException);

	//! ���캯��. name������������Ҳ������ip.
	Host(const char *name) throw(DNSException);

	//! ������.
	virtual ~Host();

	//! ���ظ�����������Ӧ��ʵ��ip������.
	int size() const;

	//! ���ص�pos�������ַ.
	string &at(int pos);
	//! ����ķ���һ�������ַ.
	string getRandIP() const;

	//! IPV4||IPV6  AF_NET or AF_NET6
	int getAddrType() const;
	

	//InetAddress inetAddr() const;
	u_int32_t inetAddr() const;

	//! ����һ��������������أ�����������.
	friend ostream &operator <<(ostream &os, const Host &host);	

protected :

private :
	//! ����nameָ����������������������������Ӧ�ĳ�Ա.
	void getHostByName(const char *name) throw(DNSException);

protected :
	string	       mName;		//!< �����ԭʼ����.
	string 	       mOName;		//!< ����������ʽ����.
	int	       mAddrType;	//!< ������ַ���ͣ�һ����ָIPV4��IPV6.
	vector<string> mAliases;	//!< �����������б���.
	vector<string> mAddrs;		//!< ��������Ӧ�����������ַ.
	
};

ostream &operator <<(ostream &os, const Host &host);	
#endif //HOST_H_ZMH_2005_10_20
/** @} */ // end of 
