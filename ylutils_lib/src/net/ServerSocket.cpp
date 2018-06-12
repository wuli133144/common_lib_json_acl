/*
 * =====================================================================================
 * 
 *        Filename:  ServerSocket.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年05月29日 17时54分54秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "ServerSocket.h"

#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netinet/tcp.h>


ServerSocket::ServerSocket() : mSocket(-1), mReuseAddress(false), mBound(false), mClosed(true)
{
	bzero(&mSockAddr, sizeof(sockaddr_in));
}

ServerSocket::ServerSocket(int port, int backlog) throw(SocketException)
	: mSocket(-1), mReuseAddress(false), mBound(false), mClosed(true)
{
	bzero(&mSockAddr, sizeof(sockaddr_in));

	bind(port, backlog);
}

ServerSocket::ServerSocket(const char *hostname, int port, int backlog) throw(SocketException)
	: mSocket(-1), mReuseAddress(false), mBound(false), mClosed(true)
{
	bzero(&mSockAddr, sizeof(sockaddr_in));

	bind(hostname, port, backlog);
}

ServerSocket::~ServerSocket()
{
	close();
}

void ServerSocket::close() throw(SocketException)
{
	if (mSocket != -1)
	{
		int ret = ::close(mSocket);

		if (ret == -1)
		{
			string msg = string("ServerSocket::close() : error : ") + strerror(errno);
			throw SocketException(msg);
		}

		mSocket = -1;
		mClosed = true;
		mBound = false;
	}

	return;
}

void ServerSocket::bind(int port, int backlog) throw(SocketException)
{
	//! bind localhost.
	bind("127.0.0.1", port, backlog);

	return;
}

void ServerSocket::bind(const char *hostname, int port, int backlog) throw(SocketException)
{

	//! get a socket fd.
	if ((mSocket=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		ostringstream omsg;
		omsg << "ServerSocket::bind(" << ((hostname==NULL)?"null":hostname) << ","
			<< port << ") : socket() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	int on = 1;
	if (mNoDelay
			&& setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(int)) == -1)
	{
		close();
		ostringstream omsg;
		omsg << "ServerSocket::bind(" << ((hostname==NULL)?"null":hostname) << ","
			<< port << ") : setsockopt(" << mSocket << ") error : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	//! init socket address.
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_port = htons(port);
	if (inet_aton(hostname, &(mSockAddr.sin_addr)) == 0)
	{
		ostringstream omsg;
		omsg << "ServerSocket::bind(" << ((hostname==NULL)?"null":hostname) << ","
			<< port << ") : inet_aton() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	mClosed = false;

	//! set the SO_REUSEADDR. linux's default is NO, so do it when mReuseAddress is set true.
	if (mReuseAddress
			&& setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1)
	{
		close();
		ostringstream omsg;
		omsg << "ServerSocket::bind(" << ((hostname==NULL)?"null":hostname) << ","
			<< port << ") : setsockopt(" << mSocket << ") error : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	//! bind it.
	if (::bind(mSocket, (sockaddr *)&mSockAddr, sizeof(mSockAddr)) == -1)
	{
		close();
		ostringstream omsg;
		omsg << "ServerSocket::bind(" << ((hostname==NULL)?"null":hostname) << ","
			<< port << ") : bind() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	//! if port is 0, I'll get the port through calling getsockname myself.
	socklen_t tLen = sizeof(sockaddr_in);
	if ((port == 0)
			&& getsockname(mSocket, (sockaddr *)&mSockAddr, &tLen) == -1)
	{
		close();
		ostringstream omsg;
		omsg << "ServerSocket::bind(" << ((hostname==NULL)?"null":hostname) << ","
			<< port << ") : getsockname() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	//! listen ... ...
	if (listen(mSocket, backlog) == -1)
	{
		close();
		ostringstream omsg; 
		omsg << "ServerSocket::bind(" << ((hostname==NULL)?"null":hostname) << ","
			<< port << ") : listen() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	mBound = true;

	return;
}

int ServerSocket::accept(Socket &socket) throw(SocketException)
{
	return 0;
}

int ServerSocket::accept() const throw(SocketException)
{
	sockaddr_in tSockAddrClient;
	bzero(&tSockAddrClient, sizeof(sockaddr_in));

	socklen_t tLen = sizeof(sockaddr_in);

	return accept(tSockAddrClient, tLen);
}

int ServerSocket::accept(struct sockaddr_in &addr, socklen_t &addrlen) const throw(SocketException)
{
	int ret;
	do
		ret = ::accept(mSocket, (struct sockaddr *)&addr, &addrlen);
	while ((ret==-1) && (errno==EINTR));
	//while ((ret==-1) && (errno==EINTR || errno==EMFILE));

	if (ret == -1)
	{
		string msg = string("ServerSocket::accept() error : ") + strerror(errno);
		throw SocketException(msg);
	}

	return ret;
}

int ServerSocket::getSocket()
{
	return mSocket;
}

int ServerSocket::getLocalPort()
{
	return ntohs(mSockAddr.sin_port);
}

bool ServerSocket::getReuseAddress() throw(SocketException)
{
	//! getsockopt()....
	//return mReuseAddress;

	int open, len = sizeof(int);
	if (getsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (void *)&open, (socklen_t *)&len) == -1)
	{
		string msg = string("ServerSocket::getReuseAddress() : getsockopt() error : ") + strerror(errno);
		throw SocketException(msg);
	}

	cout << "open : " << open << endl;

	return open;
}

void ServerSocket::setReuseAddress(bool on)
{
	mReuseAddress = on;
	return;
}

void ServerSocket::setNoDelay(bool on)
{
	mNoDelay = on;
	return;
}

bool ServerSocket::isBound()
{
	return mBound;
}

bool ServerSocket::isClosed()
{
	return mClosed;
}

string ServerSocket::toString()
{
	ostringstream oss;
	u_int32_t ip = mSockAddr.sin_addr.s_addr;
	oss << (u_int)(ip&0xff) << '.' << (u_int)((ip>>8)&0xff) << '.' << (u_int)((ip>>16)&0xff) << '.' << (u_int)((ip>>24)&0xff) << ':' << getLocalPort();
	return oss.str();
}


#ifdef _TEST

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
#endif //_TEST
