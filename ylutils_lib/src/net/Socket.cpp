/*
 * =====================================================================================
 * 
 *        Filename:  Socket.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年05月29日 21时47分58秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "Socket.h"


Socket::Socket() throw(SocketException)
	: _Std_Socket()
{
	mLocalHost = "127.0.0.1";
	mLocalPort = 0;

	initSocket();
}

Socket::Socket(const char *remotehostip, int port, int conntimeo, int retry, const char *localhostip, int localport) throw(SocketException) : _Std_Socket()
{
	initSocket();

	this->connect(remotehostip, 				//!< 远程IP.
			port, 					//!< 远程端口.
			conntimeo, 				//!< 连接超时时间.
			retry, 					//!< 连接重试次数.
			localhostip, 				//!< 本地IP.
			localport);				//!< 本地端口.
}

Socket::Socket(const Host &remotehost, int port, int conntimeo, int retry, const Host &localhost, int localport) throw(SocketException)
{
	initSocket();

	this->connect(remotehost,
			port,
			conntimeo,
			retry,
			localhost,
			localport);
}

Socket::~Socket()
{
}

void Socket::initSocket() throw(SocketException)
{
	this->close();

	mBeInitialized = false;
	mBeBound = false;
	mBeConnected = false;
	mBeClosed = true;


	//! socket.
	if ((mSocket=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		throw SocketException(string("Socket::initSocket() : socket(AF_INET, SOCK_STREAM, 0)) : )")
				+ strerror(errno));
	}

	mBeInitialized = true;

	mBeClosed = false;
	
	return;
}

void Socket::bind(int port, const char *localhostip) throw(SocketException)
{
	//! 如果端口小于0,则认为是错误的调用.
	if (port < 0)
	{
		ostringstream omsg;
		omsg << "Socket::bind(" << port << ", " << localhostip << ") : port<0!!!"; 
		throw SocketException(omsg.str());
	}
		
	//! TCP-Client 会自动选择一个端口,所以不用再进行bind操作.
	if (port == 0)
		return;

	if (mBeBound)
	{
		ostringstream omsg;
		omsg << "Socket::bind(" << port << ", " << localhostip << ") :  Socket::mBeBound == true, you have bound.";
		throw SocketException(omsg.str());
	}

	//! init local socket address.
	mLocalSockAddr.sin_family = AF_INET;
	mLocalSockAddr.sin_port = htons(port);
	if (inet_aton(localhostip, &(mLocalSockAddr.sin_addr)) == 0)
	{
		ostringstream omsg;
		omsg << "Socket::bind(" << ((localhostip==NULL)?"null":localhostip) << ","
			<< port << ") : inet_aton() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}


	//! bind local-port.
	if (::bind(mSocket, (sockaddr *)&mLocalSockAddr, sizeof(mLocalSockAddr)) == -1)
	{
		ostringstream omsg;
		omsg << "Socket::bind(" << ((localhostip==NULL)?"null":localhostip) << ","
			<< port << ") : bind() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}
	mLocalHost = localhostip;
	mLocalPort = port;
	mBeBound = true;
	return;
}

void Socket::connect(const Host &remotehost, int port, int conntimeo, int retry, const Host &localhost, int localport) throw(SocketException)
{
	this->connect(remotehost.getRandIP().c_str(), 		//!< 远程IP.
			port, 					//!< 远程端口.
			conntimeo, 				//!< 连接超时时间.
			retry, 					//!< 连接重试次数.
			localhost.getRandIP().c_str(), 		//!< 本地IP.
			localport);				//!< 本地端口.
}

void Socket::connect(const char *remotehostip, int port, int conntimeo, int retry, const char *localhostip, int localport) throw(SocketException)
{
	mConnectTimeout = conntimeo;
	mRetry = retry;
	mRemoteHost = remotehostip;
	mRemotePort = port;

	this->bind(localport, localhostip);

	//! init remote sockaddr_in.
	mRemoteSockAddr.sin_family = AF_INET;
	mRemoteSockAddr.sin_port = htons(port);
	if (inet_aton(remotehostip, &(mRemoteSockAddr.sin_addr)) == 0)
	{
		ostringstream omsg;
		omsg << "Socket::connect(" << remotehostip << ", " << port << ", " << conntimeo << ", "
			<< retry << ", " << localhostip << ", " 
			<< localport << ") : inet_aton() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	//! 连接retry次.
	while (retry--)
	{
		if (_connect(mSocket, mRemoteSockAddr, port, conntimeo) == 0)
			break;

		if (errno == ETIMEDOUT)
			continue;

		this->close();
		//! 连接失败,非超时失败.
		ostringstream omsg;
		omsg << "Socket::connect(" << remotehostip << ", " << port << ", " << conntimeo << ", "
			<< retry << ", " << localhostip << ", " 
			<< localport << ") : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	//! 连接超时.
	if (errno == ETIMEDOUT)
	{
		this->close();
		ostringstream omsg;
		omsg << "Socket::connect(" << remotehostip << ", " << port << ", " << conntimeo << ", "
			<< retry << ", " << localhostip << ", " 
			<< localport << ") : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	mBeConnected = true;

	//! get local socket-address
	socklen_t tLen = sizeof(mLocalSockAddr);
	if (getsockname(mSocket, (struct sockaddr *)&mLocalSockAddr, &tLen) == -1)
	{
		ostringstream omsg;
		omsg << "Socket::connect(" << remotehostip << ", " << port << ", " << retry << ", "
			<< localhostip << ", " << localport << ") : getsockname() error : " << strerror(errno);
		throw SocketException(omsg.str());
	}
	
	return;
}

void Socket::reConnect() throw(SocketException)
{
	this->connect(mRemoteHost.c_str(), mRemotePort, mConnectTimeout, mRetry, mLocalHost.c_str(), mLocalPort);

	return;
}

int Socket::_connect(int sockfd, const sockaddr_in &remotehost, int port, int conntimeo)
{
	/*
	int flags;
	int status;
	fd_set mask;
	struct timeval timeout;

	flags = fcntl(sockfd, F_GETFL, 0);
	if(flags < 0)
	{
		if (errno == EINPROGRESS)
			cerr << "fcntl 1" << endl;
		return -1;
	}


	flags |= O_NONBLOCK;
	if(fcntl(sockfd, F_SETFL, flags) < 0) {
		if (errno == EINPROGRESS)
			cerr << "fcntl 2" << endl;
		return -1;
	}

	if(::connect(sockfd, (const sockaddr *)&remotehost, sizeof(struct sockaddr)) == 0) {
		flags &= ~O_NONBLOCK;
		fcntl(sockfd, F_SETFL, flags);
		return 0;
	}

	FD_ZERO(&mask);
	FD_SET(sockfd,&mask);
	timeout.tv_sec = conntimeo;
	timeout.tv_usec = 0;

	status = select(sockfd+1, NULL, &mask, NULL, &timeout);

	switch(status){
		case -1:
			flags &= ~O_NONBLOCK;
			fcntl(sockfd, F_SETFL, flags);
		if (errno == EINPROGRESS)
			cerr << "fcntl 3" << endl;

			return -1;
		case 0:
			errno = ETIMEDOUT;
			flags &= ~O_NONBLOCK;
			fcntl(sockfd, F_SETFL, flags);

			return -1;

		default:
			FD_CLR(sockfd, &mask);
			flags &= ~O_NONBLOCK;
			fcntl(sockfd, F_SETFL, flags);
			errno = 0;

			return 0;
	}

	*/

	int ret;
	int flags, error;
	socklen_t len;
	struct timeval tval;
	fd_set rset, wset;
	
	if (sockfd == -1)
		goto _err;

	if((flags = fcntl(sockfd, F_GETFL)) == -1) // get flags
		goto _err;
	if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) // set non-block
		goto _err;
	if ( (ret=::connect(sockfd, (struct sockaddr*)&remotehost, sizeof(remotehost))) < 0 )
	{
		if (errno != EINPROGRESS)
		{
			goto _err;
		}
	}
	if (ret == 0) //connect on immediately
		goto _done;
	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	if (conntimeo > 0)
		tval.tv_sec = (long)conntimeo;
	else
		tval.tv_sec = 0;
	tval.tv_usec = 0;
	if((ret = select(sockfd + 1, &rset, &wset, NULL, conntimeo ? &tval : NULL)) == 0) // time out
	{
		errno = ETIMEDOUT;
		goto _err;
	}
	else if(ret == -1)
	{
		goto _err;
	}


	if(FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
		len = sizeof(error);
		if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
		{
		cerr << "fcntl last-1" << endl;
			goto _err;
		}
		if(error)
		{
			errno = error;
			goto _err;
		}
	} else
		goto _err;
_done:
	if (fcntl(sockfd, F_SETFL, flags) == -1)  // restore
	{
		cerr << "fcntl last" << endl;
		goto _err;
	}
	        return 0;
_err:
		return -1;
}

string Socket::getPeerPair()
{
	struct sockaddr_in tSockAddr;
	socklen_t tLen = sizeof(tSockAddr);

	if (getpeername(mSocket, (struct sockaddr *)&tSockAddr, &tLen) == -1)
		cerr << "getpeername() : " << strerror(errno) << endl;

	ostringstream ores;
	ores << inet_ntoa(tSockAddr.sin_addr) << ":" << ntohs(tSockAddr.sin_port);

	return ores.str();
}

ostream &operator<<(ostream &os, const Socket &socket)
{
	os << "Socket基础信息 : " << endl;
	os << "  Socket.mConnectTimeout= " << socket.mConnectTimeout << endl;
	os << "  Socket.mRetry         = " << socket.mRetry << endl;
	os << "  Socket.mLocalHost     = " << socket.mLocalHost << endl;
	os << "  Socket.mLocalPort     = " << socket.mLocalPort << endl;
	os << "  Socket.mRemoteHost    = " << socket.mRemoteHost << endl;
	os << "  Socket.mRemotePort    = " << socket.mRemotePort << endl;

	os << "Socket目前状态 :" << endl;
	os << "  Socket.mBeInitialized = " << (socket.mBeInitialized?"true":"false") << endl;
	os << "  Socket.mBeBound       = " << (socket.mBeBound?"true":"false") << endl;
	os << "  Socket.mBeConnected   = " << (socket.mBeConnected?"true":"false") << endl;
	os << "  Socket.mBeClosed      = " << (socket.mBeClosed?"true":"false") << endl;
	os << "  Socket.mSocket        = " << socket.mSocket << endl;

	os << "Socket两连接端 :" << endl;
	os << "  " << (socket.mBeConnected?socket.toString():"NOT_CONNECTED") << endl;
	return os;
}
#ifdef _TEST

#include "Socket4Object.h"
#include "TObject.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "usage : " << argv[0] << " remotehostip port" << endl;
		return 1;
	}	

	try {
		Socket client;
		client.setNoDelay();
		client.connect(argv[1], atoi(argv[2]));
		sleep(2);

		string sline;
		while(1) {
			cin>>sline;
			if(sline == "exit")
				break;
			int len = sline.length();
			client.safeWrite((char*)&len, sizeof(int));
			client.safeWrite((char*)sline.c_str(), len);
		}
	} catch (DNSException &E) {
		cout << E.what() << endl;
	} catch (SocketException &E) {
		cout << E.what() << endl;
	}

	return 0;
}
#endif //_TEST
