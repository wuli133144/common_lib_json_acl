/*
 * =====================================================================================
 * 
 *        Filename:  Socket.h
 * 
 *     Description:  TCP�ͻ���.
 * 
 *         Version:  1.0
 *         Created:  2006��05��29�� 21ʱ46��39�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup net
 *  
 *  
 *  @{
 */

#ifndef SOCKET_H_ZMH_2006_05_29
#define SOCKET_H_ZMH_2006_05_29

#include <iostream>
#include <string>

#include <unistd.h>
#include <fcntl.h>

#include "Host.h"
#include "SocketHeader.h"
#include "std_socket.h"

using namespace std;
/*!
 *\brief   TCP�ͻ���.
*! \class Socket Socket.h net/Socket.h   
\code
#include "Socket4Object.h"
#include "TObject.h"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "usage : " << argv[0] << " remotehostip port" << endl;
		cout << "  connect remote host" << endl << endl;

		cout << "  remotehostip \t\t\tdomain/ip" << endl;
		cout << "  port         \t\t\tport as integer" << endl;
		return 1;
	}	

	char *buf = new char[1024*1024];
	int n = sprintf(buf, "GET /post.yl HTTP/1.0\r\n\r\n");
	try {
		Socket client;
		client.setNoDelay();
		client.connect(argv[1], atoi(argv[2]));
		sleep(2);
		client.safeWrite(buf, n);
	} catch (DNSException &E) {
		cout << E.what() << endl;
	} catch (SocketException &E) {
		cout << E.what() << endl;
	}

	return 0;
}
\endcode
 */
class Socket : public _Std_Socket
{
public :
	//! ������. ��������һ��socket������. ���������κ����ӽ���.
	Socket() throw(SocketException);

	/**
	 * @brief    �������ػ����ͷ�����֮�������.
	 *\exception    SocketException   ��������ʧ���׳��쳣.
	 * @param    remotehostip      Զ�̷�������IP.
	 * @param    port              Զ�̷������˿�.
	 * @param    localhostip       ����������IP.
	 * @param    localport         ���ط������˿�.��������˸�ֵ,�򽫻�󶨵��ö˿���.
	 * @param    retry      retry times
	 * @param    conntimeo   connect timeout
	 */
	Socket(const char *remotehostip, int port, int conntimeo = 0, int retry = 3, const char *localhostip = "127.0.0.1", int localport = 0) throw(SocketException);

	/**
	 * @brief    �������ػ����ͷ�����֮�������.
	 *\exception    SocketException   ��������ʧ���׳��쳣.
	 * @param    remotehost        Զ�̷�����.
	 * @param    port              Զ�̷������˿�.
	 * @param    localhost         ����������.
	 * @param    localport         ���ط������˿�.��������˸�ֵ,�򽫻�󶨵��ö˿���.
	 * @param    conntimeo   connect timeout
	 * 
	 */
	Socket(const Host &remotehost, int port, int conntimeo = 0, int retry = 3, const Host &localhost = "localhost", int localport = 0) throw(SocketException);

	//! ������.
	virtual ~Socket();

	/**
	 * @brief    �󶨵����ض˿�.
	 * \exception    SocketException   ��ʧ���׳��쳣.
	 *
	 * @param    port              ���ض˿�. 
	 * @param    localhostip     ��������(�ͻ���)IP.
	 */
	void bind(int port, const char *localhostip = "127.0.0.1") throw(SocketException);

	/**
	 * @brief    ����<local-socket, remote-socket>֮�������.
	 * \exception     SocketException   ��������ʧ���׳��쳣.
	 * @param    remotehostip      Զ�̷�������IP.
	 * @param    port              Զ�̷������˿�.
	 * @param    retry             ����ʧ��ʱ���ԵĴ���.
	 * @param    localhostip       ��������IP.
	 * @param    localport         ���ط������˿�.��������˸�ֵ,�򽫻�󶨵��ö˿���.
	 *
	 */
	void connect(const char *remotehostip, int port, int conntimeo = 15, int retry = 3, const char *localhostip = "127.0.0.1", int localport = 0) throw(SocketException);

	/**
	 * @brief    ����<local-socket, remote-socket>֮�������.
	 *\exception    SocketException   ��������ʧ���׳��쳣.
	 * @param    remotehost        Զ�̷�����.
	 * @param    port              Զ�̷������˿�.
	 * @param    retry             ����ʧ��ʱ���ԵĴ���.
	 * @param    localhost         ��������.
	 * @param    localport         ���ط������˿�.��������˸�ֵ,�򽫻�󶨵��ö˿���.
	 * 
	 */
	void connect(const Host &remotehost, int port, int conntimeo = 15, int retry = 3, const Host &localhost = "localhost", int localport = 0) throw(SocketException);

	//! init the mLocalSockAddr & the mSocket.
	void initSocket() throw(SocketException);
	//! reconnect
	void reConnect() throw(SocketException);

	//! ip:port
	string getPeerPair();

	friend ostream &operator<<(ostream &os, const Socket &socket);

protected :

private :


	int _connect(int sockfd, const sockaddr_in &remotehost, int port, int conntimeo);
private :
	//! ��ʱʱ�������Ϊ��λ.
	u_int		mConnectTimeout;	//!< ���ӳ�ʱʱ��.
	u_int		mRetry;			//!< ����ʧ��ʱ��������.
	string		mLocalHost;		//!< ����������IP.
	int		mLocalPort;		//!< ���������˿�.
	string		mRemoteHost;		//!< Զ�̷�����IP.
	int		mRemotePort;		//!< Զ�̷������˿�.
};

ostream &operator<<(ostream &os, const Socket &socket);
#endif //SOCKET_H_ZMH_2006_05_29
/** @} */ // end of 
