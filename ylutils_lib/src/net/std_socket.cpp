/*
 * =====================================================================================
 * 
 *        Filename:  std_socket.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年05月30日 20时04分56秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "std_socket.h"


_Std_Socket::_Std_Socket() : mBeInitialized(false), mBeConnected(false), mBeBound(false), mBeClosed(false), mReadTimeout(5),mWriteTimeout(5), mSocket(-1)
{
	bzero(&mLocalSockAddr, sizeof(mLocalSockAddr));
	bzero(&mRemoteSockAddr, sizeof(mRemoteSockAddr));
}

_Std_Socket::~_Std_Socket()
{
	close();
}

void _Std_Socket::close() throw(SocketException)
{
	if (mSocket != -1)
		::close(mSocket);

	mBeInitialized = false;
	mBeBound = false;
	mBeConnected = false;
	mBeClosed = true;

	mSocket = -1;

	return;
}

int _Std_Socket::getLocalPort() const
{
	return mBeBound?ntohs(mLocalSockAddr.sin_port):-1;
}

int _Std_Socket::getRemotePort() const
{
	return mBeConnected?ntohs(mRemoteSockAddr.sin_port):0;
}

const sockaddr_in *_Std_Socket::getLocalSockAddr() const
{
	return &mLocalSockAddr;
}

const sockaddr_in *_Std_Socket::getRemoteSockAddr() const
{
	return &mRemoteSockAddr;
}

void _Std_Socket::getSockOpt(int level, int optname, void *optval, socklen_t *optlen) throw(SocketException)
{
	if (!mBeInitialized)
	{
		throw SocketException("_Std_Socket::getSockOpt() : the socket should be initialized first.");
	}

	if (getsockopt(mSocket, level, optname, optval, optlen) == -1)
	{
		ostringstream omsg;
		omsg << "_Std_Socket::getSockOpt() :getsockopt(" << mSocket << ", " << level << ", "
			<< optname << ", " << *(int *)optval << ", " << *optlen << ") : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	return;
}

void _Std_Socket::setSockOpt(int level, int optname, const void *optval, socklen_t optlen) throw(SocketException)
{
	if (!mBeInitialized)
	{
		throw SocketException("_Std_Socket::setSockOpt() : the socket should be initialized first.");
	}

	if (setsockopt(mSocket, level, optname, optval, optlen) == -1)
	{
		ostringstream omsg;
		omsg << "_Std_Socket::setSockOpt() :setsockopt(" << mSocket << ", " << level << ", "
			<< optname << ", " << *(int *)optval << ", " << optlen << ") : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	return;
}

bool _Std_Socket::getNoDelay() throw(SocketException)
{
	int val;
	socklen_t len = sizeof(val);
	getSockOpt(IPPROTO_TCP, TCP_NODELAY, (void *)&val, &len);

	return val;
}

void _Std_Socket::setNoDelay(bool on) throw(SocketException)
{
	int val = on?1:0;
	setSockOpt(IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(val));

	return;
}

bool _Std_Socket::getKeepAlive() throw(SocketException)
{
	int val;
	socklen_t len = sizeof(val);

	getSockOpt(SOL_SOCKET, SO_KEEPALIVE, (void *)&val, &len);

	return val;
}

void _Std_Socket::setKeepAlive(bool on) throw(SocketException)
{
	int val = on?1:0;

	setSockOpt(SOL_SOCKET, SO_KEEPALIVE, (const void *)&val, sizeof(val));

	return;
}

bool _Std_Socket::getReuseAddress() throw(SocketException)
{
	int val;
	socklen_t len = sizeof(val);

	getSockOpt(SOL_SOCKET, SO_REUSEADDR, (void *)&val, &len);

	return val;
}

void _Std_Socket::setReuseAddress(bool on) throw(SocketException)
{
	int val = on?1:0;

	setSockOpt(SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(val));

	return;
}

int _Std_Socket::getReceiveBufferSize() throw(SocketException)
{
	int val;
	socklen_t len = sizeof(val);

	getSockOpt(SOL_SOCKET, SO_RCVBUF, (void *)&val, &len);

	return val;
}

void _Std_Socket::setReceiveBufferSize(int size) throw(SocketException)
{
	setSockOpt(SOL_SOCKET, SO_RCVBUF, (void *)&size, sizeof(size));

	return;
}

int _Std_Socket::getSendBufferSize() throw(SocketException)
{
	int val;
	socklen_t len = sizeof(val);

	getSockOpt(SOL_SOCKET, SO_SNDBUF, (void *)&val, &len);

	return val;
}

void _Std_Socket::setSendBufferSize(int size) throw(SocketException)
{
	setSockOpt(SOL_SOCKET, SO_SNDBUF, (void *)&size, sizeof(size));

	return;
}

//! get the SO_SNDTIMEO
int _Std_Socket::getSoTimeout() throw(SocketException)
{
	timeval val;
	socklen_t len = sizeof(val);

	getSockOpt(SOL_SOCKET, SO_SNDTIMEO, (void *)&val, &len);

	return val.tv_sec;
}

void _Std_Socket::setSoTimeout(int timeout) throw(SocketException)
{
	return;
}

string _Std_Socket::toString() const 
{
	ostringstream oss;

	//! local ip:port
	u_int32_t ip = mLocalSockAddr.sin_addr.s_addr;
	oss << (u_int)(ip&0xff) << '.' << (u_int)((ip>>8)&0xff) << '.' << (u_int)((ip>>16)&0xff) << '.' << (u_int)((ip>>24)&0xff) << ':' << ntohs(mLocalSockAddr.sin_port);
	oss << "==>";

	//! remote ip:port
	ip = mRemoteSockAddr.sin_addr.s_addr;
	oss << (u_int)(ip&0xff) << '.' << (u_int)((ip>>8)&0xff) << '.' << (u_int)((ip>>16)&0xff) << '.' << (u_int)((ip>>24)&0xff) << ':' << ntohs(mRemoteSockAddr.sin_port);

	return oss.str();
}

string _Std_Socket::getRemoteIP()
{
	ostringstream oss;

	//! remote ip:port
	u_int32_t ip = mRemoteSockAddr.sin_addr.s_addr;
	oss << (u_int)(ip&0xff) << '.' << (u_int)((ip>>8)&0xff) << '.' << (u_int)((ip>>16)&0xff) << '.' << (u_int)((ip>>24)&0xff);

	return oss.str();
}

int _Std_Socket::_select_fd(int sockfd, double timeout, bool writep)
{
	if (sockfd == -1)
	{
		errno = EBADF;
		return -1;
	}
	fd_set fds;
	fd_set *rd = NULL, *wrt = NULL;
	struct timeval tmout;
	int result;

	FD_ZERO (&fds);
	FD_SET (sockfd, &fds);
	*(writep ? &wrt : &rd) = &fds;

	tmout.tv_sec = (long)timeout;
	tmout.tv_usec = (long)(1000000L * (timeout - (long)timeout));

	do
		result = select (sockfd + 1, rd, wrt, NULL, (timeout==0)?NULL:&tmout);
	while (result < 0 && errno == EINTR);

	if (result == 0)
		errno = ETIMEDOUT;

	return result;
}

int _Std_Socket::_poll_fd(int sockfd, double timeout, bool writep)
{
	if (sockfd == -1)
	{
		errno = EBADF;
		return -1;
	}

	int result;
	int waittime = (long)timeout*1000;
	struct pollfd  client;
	client.fd = sockfd;
	if (writep)
		client.events = POLLOUT;
	else
		client.events = POLLIN;	
	do
		result=poll(&client, 1, (timeout==0)?-1:waittime);
	while (result < 0 && errno == EINTR);
	
	if (result == 0)
		errno = ETIMEDOUT;

	return result;
}


ssize_t _Std_Socket::read(char *buf, ssize_t count)
{
	ssize_t res = 0;
	do
		res = ::read(mSocket, buf, count);
	while (res == -1 && errno == EINTR);

	return res;
}

ssize_t _Std_Socket::read(char *buf, ssize_t count, int to) throw(SocketException)
{
	if (_select_fd(mSocket, to, false) <= 0)
	{
		string msg = "_Std_Socket::safeRead() _select_fd error :";
		msg += (errno==ETIMEDOUT)?"Read Timeout":strerror(errno);
		throw SocketException(msg);
	}
	ssize_t res = 0;
	do
		res = ::read(mSocket, buf, count);
	while (res == -1 && errno == EINTR);

	if (res == -1)
	{
		ostringstream omsg;
	      	omsg << "_Std_Socket::read() mSocket : " << mSocket << " : " << strerror(errno);
		throw SocketException(omsg.str());
	}

	return res;
}

ssize_t _Std_Socket::safeReadHttpRequest(char *buf,ssize_t count,bool isPoll) throw(SocketException)
{
	int res = 0;
	int total = 0;
	char* begin=buf;

	while (1)
	{
		int result;
		if (!isPoll)
			result=_select_fd(mSocket, mReadTimeout, false);	
		else
			result=_poll_fd(mSocket, mReadTimeout, false);

		if (result<= 0)
		{
			string msg = "_Std_Socket::safeReadHttpRequest() _select_fd error :";
				msg += (errno==ETIMEDOUT)?"Read Timeout":strerror(errno);
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);
		}		

		do
			res = ::read(mSocket, buf, count);
		while (res==-1 && errno==EINTR);
		if (res == 0)
		{
			string msg = string("_Std_Socket::safeReadHttpRequest() : read() error : peer shut down");
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);
		} else if (res < 0)
		{
			string msg = string("_Std_Socket::safeReadHttpRequest() : read() error ") + strerror(errno);
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);
		}

		buf += res;
		count-=res;
 		*buf='\0';
		char* pos;
		pos=strstr(begin,"\r\n\r\n");
		if (pos){
		      pos=strchr(begin,'?');
                    if (pos){	
                    		pos++;	
				char* argEnd=strchr(pos,' ');	
				if (argEnd){
					total=pos-begin;
					*argEnd='\0';
					break;
				}				
			}
			string msg = string("_Std_Socket::safeReadHttpRequest() : http arg format error");
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);
		}
	}
	return total;
}


ssize_t _Std_Socket::safeRead(char *buf, ssize_t count,bool isPoll) throw(SocketException)
{
	int res = 0;
	int total = 0;

	while (count > 0)
	{
		int result;
		if (!isPoll)
			result=_select_fd(mSocket, mReadTimeout, false);	
		else
			result=_poll_fd(mSocket, mReadTimeout, false);

		if (result<= 0)
		{
			string msg = "_Std_Socket::safeRead() _select_fd error :";
				msg += (errno==ETIMEDOUT)?"Read Timeout":strerror(errno);
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);
		}		

		do
			res = ::read(mSocket, buf, count);
		while (res==-1 && errno==EINTR);
		if (res == 0)
		{
			string msg = string("_Std_Socket::safeRead() : read() error : peer shut down");
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);
		} else if (res < 0)
		{
			string msg = string("_Std_Socket::safeRead() : read() error ") + strerror(errno);
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);
		}

		buf += res;
		count -= res;
		total += res;
	}

	return total;
}


ssize_t _Std_Socket::write(char *buf, ssize_t count)
{
	return 0;
}



ssize_t _Std_Socket::safeWrite(char *buf, ssize_t count,bool isPoll) throw(SocketException)
{
	int res = 0;

	while (count > 0)
	{
		int result;
		if (!isPoll)
			result=_select_fd(mSocket, mWriteTimeout, true);	
		else
			result=_poll_fd(mSocket, mWriteTimeout, true);

		if (result<= 0)
		{
			string msg = string("_Std_Socket::safeWrite() : _select_fd() error ") + strerror(errno);
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);

		}		
		
		do
			res = ::write(mSocket, buf, count);
		while (res == -1 && errno == EINTR);
		if (res <= 0)
		{
			string msg = string("_Std_Socket::safeWrite() : write() error ") + strerror(errno);
			ostringstream ofd; 
			ofd << " fd : " << mSocket;
			msg += ofd.str();
			throw SocketException(msg);
		}
		buf += res;
		count -= res;
	}

	return res;
}


bool _Std_Socket::isConnect() const
{
	return mBeConnected;
}

bool _Std_Socket::good() const
{
	if (mSocket == -1)
		return false;

	fd_set check_set;
	struct timeval to;

	/* Check if we still have a valid (non-EOF) connection.  From Andrew
	 *          *           Maholski's code in the Unix Socket FAQ.  */

	FD_ZERO (&check_set);
	FD_SET (mSocket, &check_set);

	/* Wait one microsecond */
	to.tv_sec = 0;
	to.tv_usec = 1;

	/* If we get a timeout, then that means still connected */
	if (select (mSocket + 1, &check_set, NULL, NULL, &to) == 0)
	{
		/* Connection is valid (not EOF), so continue */
		return true;
	}
	else
		return false;
}

bool _Std_Socket::tcp_establish() const
{
        struct tcp_info info; 
        int len=sizeof(info); 
        getsockopt(mSocket, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len); 
        if  (info.tcpi_state==TCP_ESTABLISHED)
            return true;
        else
            return false;
}  