/*
 * =====================================================================================
 * 
 *        Filename:  UdpSocket.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2007年03月30日 19时33分30秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "UdpSocket.h"


UdpSocket::UdpSocket() throw(UdpException)
	:
	mSocket(-1)
{
	if ((mSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		ostringstream oexcpt;
		oexcpt << "UdpSocket::UdpSocket() : socket(AF_INET, SOCK_DGRAM, 0) : " << strerror(errno);
		throw UdpException(oexcpt.str());
	}
}

UdpSocket::~UdpSocket()
{
	this->close();
}

void UdpSocket::close()
{
	if (mSocket != -1)
		::close(mSocket);
}

void UdpSocket::getSockOpt(int level, int optname, void *optval, socklen_t *optlen) throw(UdpException)
{
	if (getsockopt(mSocket, level, optname, optval, optlen) == -1)
	{
		ostringstream omsg;
		omsg << "UdpSocket::getSockOpt() :getsockopt(" << mSocket << ", " << level << ", "
			<< optname << ", " << *(int *)optval << ", " << *optlen << ") : " << strerror(errno);
		throw UdpException(omsg.str());
	}

	return;
}

void UdpSocket::setSockOpt(int level, int optname, const void *optval, socklen_t optlen) throw(UdpException)
{
	if (setsockopt(mSocket, level, optname, optval, optlen) == -1)
	{
		ostringstream omsg;
		omsg << "UdpSocket::setSockOpt() :setsockopt(" << mSocket << ", " << level << ", "
			<< optname << ", " << *(int *)optval << ", " << optlen << ") : " << strerror(errno);
		throw UdpException(omsg.str());
	}

	return;
}


void UdpSocket::bind(const char *ip, int port) throw(UdpException)
{
	//! 填充.
	bzero(&mSockAddr, sizeof(mSockAddr));
	mSockAddr.sin_family=AF_INET;
	mSockAddr.sin_port=htons(port);
	if (strcmp(ip, "localhost") == 0)
	{
		mSockAddr.sin_addr.s_addr=htonl(INADDR_ANY) ;
	}
	else if (inet_aton(ip, &(mSockAddr.sin_addr)) == 0)
	{
		ostringstream omsg;
		omsg << "UdpSocket::bind(" << ((ip==NULL)?"null":ip) << ","
			<< port << ") : inet_aton() error : " << strerror(errno);
		throw UdpException(omsg.str());
	}

	
	//! 绑定.
	if (::bind(mSocket, (sockaddr *)&mSockAddr, sizeof(mSockAddr)) == -1)
	{
		this->close();
		ostringstream omsg;
		omsg << "UdpSocket:bind(" << ((ip==NULL)?"null":ip) << ","
			<< port << ") : bind() error : " << strerror(errno);
		throw UdpException(omsg.str());
	}

	return;
}

void UdpSocket::convert(struct sockaddr_in &netaddr, const char *ip, int port) throw(UdpException)
{
	bzero(&netaddr,sizeof(netaddr));
	netaddr.sin_family = AF_INET;
	netaddr.sin_port = htons(port);
	if (inet_aton(ip, &(netaddr.sin_addr)) == 0)
	{
		ostringstream omsg;
		omsg << "UdpSocket::sendto(" << ((ip==NULL)?"null":ip) << ","
			<< port << ") : inet_aton() error : " << strerror(errno);
		throw UdpException(omsg.str());
	}

	return;
}

int UdpSocket::recvfrom(void *buf, int len, const char *fromip, int fromport) throw(UdpException)
{
	struct sockaddr_in addr_to;
	this->convert(addr_to, fromip, fromport);
	return this->recvfrom(buf, len, (sockaddr *)&addr_to);
}

int UdpSocket::recvfrom(void *buf, int len, struct sockaddr *from) throw(UdpException)
{
	socklen_t fromlen = sizeof(sockaddr_in);
	int ret = ::recvfrom(mSocket, buf, len, 0, from, &fromlen);
	if (ret == -1)
	{
		ostringstream oexcpt;
		oexcpt << "UdpSocket::recvfrom(, , , ) : recvfrom() : " << strerror(errno);
		throw UdpException(oexcpt.str());
	}

	return ret;
}

int UdpSocket::sendto(const void *buf, int len, const char *toip, int toport) throw(UdpException)
{
	struct sockaddr_in addr_to;
	this->convert(addr_to, toip, toport);
	return this->sendto(buf, len, (sockaddr *)&addr_to);
}

int UdpSocket::sendto(const void *buf, int len, struct sockaddr *to) throw(UdpException)
{
	socklen_t tolen = sizeof(sockaddr_in);
	int ret = ::sendto(mSocket, buf, len, 0, to, tolen);
	if (ret == -1)
	{
		ostringstream oexcpt;
		oexcpt << "UdpSocket::sendto(" << mSocket << ", buf, " <<  len << ", sockaddr, socklen) : sendto() : " << strerror(errno);
		throw UdpException(oexcpt.str());
	}

	return ret;
}


#ifdef _TEST

#include <fstream>

int main(int argc, char *argv[])
{
	daemon(1, 0);
	ofstream log("udp.log");
	char buffer[1024];
	try {
		UdpSocket s;
		s.bind("localhost", 9000);

		log << "bind(localhost, 9000) succ ! listen ... ..." << endl << flush;

		struct sockaddr_in client_addr;

		while (1) {
			bzero(buffer, sizeof(buffer));
			s.recvfrom(buffer, sizeof(buffer), (sockaddr *)&client_addr);
			log << buffer << "\tfrom : " << inet_ntoa(client_addr.sin_addr) << endl << flush;
		}
	
	} catch (exception &E) {
		log << E.what() << endl << flush;
	}

	return 0;
}
#endif //_TEST
