/*
 * =====================================================================================
 * 
 *        Filename:  AcceptSocket.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��06��05�� 12ʱ52��59�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ (), minghui.zhang@yeelion.com
 *         Company:  �����Ƽ�
 * 
 * =====================================================================================
 */

#include "AcceptSocket.h"


AcceptSocket::AcceptSocket(int socket)throw(SocketException)
	: _Std_Socket()
{
	mSocket = socket;
	mBeInitialized = true;

	this->assignAddressBySocket();
}

AcceptSocket::~AcceptSocket()
{
}

void AcceptSocket::assignAddressBySocket() throw(SocketException)
{
	socklen_t tLen = sizeof(struct sockaddr_in);
	if (getsockname(mSocket, (sockaddr *)&mLocalSockAddr, &tLen) == -1)
	{
		close();
		ostringstream omsg;
		omsg << "AcceptSocket::assignAddressBySocket() getsockname(" << mSocket << ", &sockaddr, &len) "
			<< strerror(errno);
		throw SocketException(omsg.str());
	}

	if (getpeername(mSocket, (sockaddr *)&mRemoteSockAddr, &tLen) == -1)
	{
		close();
		ostringstream omsg;
		omsg << "AcceptSocket::assignAddressBySocket() getpeername(" << mSocket << ", &sockaddr, &len) "
			<< strerror(errno);
		throw SocketException(omsg.str());
	}
	return;
}
