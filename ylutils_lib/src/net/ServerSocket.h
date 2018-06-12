/*
 * =====================================================================================
 * 
 *        Filename:  ServerSocket.h
 * 
 *     Description:  TCP服务器.
 * 
 *         Version:  1.0
 *         Created:  2006年05月29日 15时49分41秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @defgroup net
 *  网络操作
 *  @{
 */


#ifndef SERVERSOCKET_H_ZMH_2006_05_29
#define SERVERSOCKET_H_ZMH_2006_05_29

#include <iostream>
#include <sstream>
#include <string>

#include "SocketHeader.h"
#include "Socket.h"
#include "SocketException.h"
#include <sys/errno.h>

using namespace std;
/*!
 *\brief   TCP服务器.
*! \class ServerSocket ServerSocket.h net/ServerSocket.h   
\code

#include "AcceptSocket4Object.h"
#include "TObject.h"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "usage : " << argv[0] << " ip port" << endl;
		return 1;
	}


	try {
		ServerSocket ss;

		ss.setReuseAddress(true);
		ss.setNoDelay();

		ss.bind(argv[1], (atoi(argv[2])), 5);

		cout << "ss.isClosed() : " << ss.isClosed() << endl;
		cout << "ss.getReuseAddress() : " << ss.getReuseAddress() << endl;
		cout << "ss.isBound() : " << ss.isBound() << endl;
		cout << "ss.getSocket() : " << ss.getSocket() << endl;
		cout << "ss.toString() : " << ss.toString() << endl;

		int socket = ss.accept();

		AcceptSocket a(socket);
		cerr << "accept a connect" << endl;
		sleep(20);

		char *buf = new char[1024*1024];
		a.safeRead(buf, 1024*60);
		a.safeRead(buf, 1024*60);
		cerr << "first ok" << endl;
	} catch (SocketException &E) {
		cout << E.what() << endl;
	}

	return 0;
}
\endcode
 */

class ServerSocket
{
public :
	//! Creates an unbound server socket.
	ServerSocket();

	/**
	 * @brief    Creates a server socket and binds it to the specified 
	 *           local port number, with the specified backlog.
	 *\exception    SocketException   socket() || setsockopt() || bind() 
	 *                || listen() error.
	 * @param    port    the specified port, or 0 to use any free port.
	 * @param    backlog the maximum length of the queue.
	 * 
	 */
	ServerSocket(int port, int backlog = 20) throw(SocketException);

	/**
	 * @brief    Creates a server socket, bound to the specified address.
	 *\exception    SocketException   socket() || setsockopt() || bind() 
	 *                || listen() error.
	 * @param    hostname    ip or domain.
	 * @param    port        the specified port, or 0 to use any free port.
	 * @param    backlog     the maximum length of the queue.
	 * 
	 */
	ServerSocket(const char *hostname, int port, int backlog = 20) throw(SocketException);

	//! Destructor.
	virtual ~ServerSocket();

	/**
	 * @brief    Binds the ServerSocket to a specific port.
	 *\exception    SocketException   socket() || setsockopt() || bind() 
	 * @param    port    the specified port, or 0 to use any free port.
	 * @param    backlog the maximum length of the queue.
	 * 
	 *                || listen() error.
	 */
	void bind(int port, int backlog = 20) throw(SocketException);

	/**
	 * @brief    Binds the ServerSocket to a specific address. (hostname & hostport).
	 *\exception    SocketException   socket() || setsockopt() || bind() 
	 * @param    hostname    ip or domain.
	 * @param    port        the specified port, or 0 to use any free port.
	 * @param    backlog     the maximum length of the queue.
	 * 
	 *                || listen() error.
	 */
	void bind(const char *hostname, int port, int backlog = 20) throw(SocketException);
	/**
	 * @brief    Binds the ServerSocket to a specific address. (hostname & hostport).
	 *\exception    SocketException   socket() || setsockopt() || bind() 
	 * @param    hostname    ip or domain.
	 * @param    port        the specified port, or 0 to use any free port.
	 * @param    backlog     the maximum length of the queue.
	 * 
	 *                || listen() error.
	 */	
	void bind(const string &hostname, int port, int backlog = 20) throw(SocketException);

	//! Close the socket.
	void close() throw(SocketException);

	//! Receive a client.
	int accept(Socket &socket) throw(SocketException);

	//! Receive a client.
	int accept(struct sockaddr_in &addr, socklen_t &addrlen) const throw(SocketException);

	//! sometime only the fd is useful.
	int accept() const throw(SocketException);

	//! Get the listen descriptor.
	int getSocket();

	//! Returns the port on which this socket is listening.
	int getLocalPort();

	//! Tests if SO_REUSEADDR is enabled.
	bool getReuseAddress() throw(SocketException);

	/**
	 * @brief     Enable/disable the SO_REUSEADDR socket option.
	 *            you should code :
	 *
	 *            ServerSocket ss;
	 *            ss.setReuseAddress();
	 *
	 *            try {
	 *            		ss.bind("127.0.0.1", 80);
	 *            	
	 *            		while (1) {
	 *            			int client_fd = ss.accept();
	 *
	 *	            		dosomething(client_fd);
	 *      	      	}
	 *            	} catch (SocketException &E) {
	 *            		cout << E.what() << endl;
	 *            	}
	 *            	............;
	 *            	TCP的REUSEADDR没有太大的用处,他不允许相同的IP地址
	 *            	绑定到同一端口上.
	 */
	void setReuseAddress(bool on = true);

	/**
	 * @brief    设置非延迟.
	 */
	void setNoDelay(bool on = true);

	//! Returns the binding state of the ServerSocket.
	bool isBound();

	//! Returns the closed state of the ServerSocket.
	bool isClosed();

	//! toString.
	string toString();
protected :

private :
	int			mSocket;		//!< socket descriptor.
	bool			mReuseAddress;		//!< SO_REUSEADDR is set?
	bool			mNoDelay;		//!< TCP_NODELAY is set?
	bool			mBound;			//!< is bound?
	bool			mClosed;		//!< is closed?
	sockaddr_in		mSockAddr;		//!< socket adress for IPV4.
};
#endif //SERVERSOCKET_H_ZMH_2006_05_29
/** @} */ // end of 
