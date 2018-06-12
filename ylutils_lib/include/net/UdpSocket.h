/*
 * =====================================================================================
 * 
 *        Filename:  UdpSocket.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2007年03月30日 19时01分40秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef UDPSOCKET_H_ZMH_2007_03_30
#define UDPSOCKET_H_ZMH_2007_03_30

#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

using namespace std;

class UdpException : public std::exception
{
public :
	UdpException(const string &msg) : mMsg(msg) {}
	virtual ~UdpException() throw() {}

	const char *what() const throw() { return mMsg.c_str(); }

private :
	string mMsg;
};
/*! \brief UdpSocket
*
*
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
\code
\endcode 
*/
class UdpSocket
{
public :
	UdpSocket() throw(UdpException);

	virtual ~UdpSocket();

	void bind(const char *ip, int port) throw(UdpException);

	int recvfrom(void *buf, int len, struct sockaddr *from) throw(UdpException);
	int recvfrom(void *buf, int len, const char *fromip, int fromport) throw(UdpException);

	int sendto(const void *buf, int len, struct sockaddr *to) throw(UdpException);
	int sendto(const void *buf, int len, const char *toip, int toport) throw(UdpException);

	void setSockOpt(int level, int optname, const void *optval, socklen_t optlen) throw(UdpException);
	void getSockOpt(int level, int optname, void *optval, socklen_t *optlen) throw(UdpException);
	void setReuseAddress(int on = 1) throw (UdpException) {
		setSockOpt(SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	}

	int getReceiveBufferSize() throw(UdpException)
	{
		int val;
		socklen_t len = sizeof(val);

		getSockOpt(SOL_SOCKET, SO_RCVBUF, (void *)&val, &len);

		return val;
	}

	void setReceiveBufferSize(int size) throw(UdpException)
	{
		setSockOpt(SOL_SOCKET, SO_RCVBUF, (void *)&size, sizeof(size));

		return;
	}



	void close();
	void convert(struct sockaddr_in &netaddr, const char *ip, int port) throw(UdpException);

	int getSocketFd() {
		return mSocket;
	}
protected :

private :
	int                     mSocket;                //!< socket descriptor.
	sockaddr_in             mSockAddr;              //!< socket adress for IPV4.
};
#endif //UDPSOCKET_H_ZMH_2007_03_30

