/*
 * =====================================================================================
 * 
 *        Filename:  Socket.h
 * 
 *     Description:  TCP客户端.
 * 
 *         Version:  1.0
 *         Created:  2006年05月29日 21时46分39秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
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
 *\brief   TCP客户端.
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
	//! 构造子. 将会申请一个socket描述符. 但不会有任何连接建立.
	Socket() throw(SocketException);

	/**
	 * @brief    建立本地机器和服务器之间的连接.
	 *\exception    SocketException   建立连接失败抛出异常.
	 * @param    remotehostip      远程服务器的IP.
	 * @param    port              远程服务器端口.
	 * @param    localhostip       本地主机的IP.
	 * @param    localport         本地服务器端口.如果设置了改值,则将会绑定到该端口上.
	 * @param    retry      retry times
	 * @param    conntimeo   connect timeout
	 */
	Socket(const char *remotehostip, int port, int conntimeo = 0, int retry = 3, const char *localhostip = "127.0.0.1", int localport = 0) throw(SocketException);

	/**
	 * @brief    建立本地机器和服务器之间的连接.
	 *\exception    SocketException   建立连接失败抛出异常.
	 * @param    remotehost        远程服务器.
	 * @param    port              远程服务器端口.
	 * @param    localhost         本地主机的.
	 * @param    localport         本地服务器端口.如果设置了改值,则将会绑定到该端口上.
	 * @param    conntimeo   connect timeout
	 * 
	 */
	Socket(const Host &remotehost, int port, int conntimeo = 0, int retry = 3, const Host &localhost = "localhost", int localport = 0) throw(SocketException);

	//! 析构子.
	virtual ~Socket();

	/**
	 * @brief    绑定到本地端口.
	 * \exception    SocketException   绑定失败抛出异常.
	 *
	 * @param    port              本地端口. 
	 * @param    localhostip     本地主机(客户机)IP.
	 */
	void bind(int port, const char *localhostip = "127.0.0.1") throw(SocketException);

	/**
	 * @brief    建立<local-socket, remote-socket>之间的连接.
	 * \exception     SocketException   建立连接失败抛出异常.
	 * @param    remotehostip      远程服务器的IP.
	 * @param    port              远程服务器端口.
	 * @param    retry             连接失败时重试的次数.
	 * @param    localhostip       本地主机IP.
	 * @param    localport         本地服务器端口.如果设置了改值,则将会绑定到该端口上.
	 *
	 */
	void connect(const char *remotehostip, int port, int conntimeo = 15, int retry = 3, const char *localhostip = "127.0.0.1", int localport = 0) throw(SocketException);

	/**
	 * @brief    建立<local-socket, remote-socket>之间的连接.
	 *\exception    SocketException   建立连接失败抛出异常.
	 * @param    remotehost        远程服务器.
	 * @param    port              远程服务器端口.
	 * @param    retry             连接失败时重试的次数.
	 * @param    localhost         本地主机.
	 * @param    localport         本地服务器端口.如果设置了改值,则将会绑定到该端口上.
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
	//! 超时时间均已秒为单位.
	u_int		mConnectTimeout;	//!< 连接超时时间.
	u_int		mRetry;			//!< 连接失败时重连次数.
	string		mLocalHost;		//!< 本地主机的IP.
	int		mLocalPort;		//!< 本地主机端口.
	string		mRemoteHost;		//!< 远程服务器IP.
	int		mRemotePort;		//!< 远程服务器端口.
};

ostream &operator<<(ostream &os, const Socket &socket);
#endif //SOCKET_H_ZMH_2006_05_29
/** @} */ // end of 
