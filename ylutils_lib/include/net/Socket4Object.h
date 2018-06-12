/*
 * =====================================================================================
 * 
 *        Filename:  Socket4Object.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月05日 16时51分16秒 CST
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

#ifndef SOCKET4OBJECT_H_ZMH_2006_06_05
#define SOCKET4OBJECT_H_ZMH_2006_06_05

#include <iostream>
#include <string>

#include "Socket.h"

using namespace std;
/*!
 *\brief   Socket4Object.
*! \class Socket4Object Socket4Object.h net/Socket4Object.h   
 */

template <class Object>
class Socket4Object : public Socket
{
public :
	Socket4Object() throw(SocketException);

	virtual ~Socket4Object();

	Socket4Object(const char *remotehostip, int port, int conntimeo = 0, int retry = 3, const char *localhostip = "127.0.0.1", int localport = 0) throw(SocketException);

	Socket4Object(const Host &remotehost, int port, int conntimeo = 0, int retry = 3, const Host &localhost = "localhost", int localport = 0) throw(SocketException);

	ssize_t sendObject(const Object &object) throw(SocketException);

	ssize_t recvObject(Object &object) throw(SocketException);

protected :

private :

	
};

template <class Object>
Socket4Object<Object>::Socket4Object() throw(SocketException)
	: Socket()
{
}

template <class Object>
Socket4Object<Object>::~Socket4Object()
{
}

template <class Object>
Socket4Object<Object>::Socket4Object(const char *remotehostip, int port, int conntimeo, int retry, const char *localhostip, int localport) throw(SocketException)
	: Socket(remotehostip, port, conntimeo, retry, localhostip, localport)
{
}

template <class Object>
Socket4Object<Object>::Socket4Object(const Host &remotehost, int port, int conntimeo, int retry, const Host &localhost, int localport) throw(SocketException)
	: Socket(remotehost, port, conntimeo, retry, localhost, localport)
{
}

template <class Object>
ssize_t Socket4Object<Object>::sendObject(const Object &object) throw(SocketException)
{
	//! send body's len.
	int size = object.size();
	safeWrite((char *)&size, sizeof(int));

	//! send body's bytes.
	safeWrite((char *)object.c_str(), object.size());

	return object.size() + sizeof(int);
}

template <class Object>
ssize_t Socket4Object<Object>::recvObject(Object &object) throw(SocketException)
{
	int tLen = 0;
	safeRead((char *)&tLen, sizeof(int));

	safeRead(object.address(), tLen);

	return tLen + sizeof(tLen);
}
#endif //SOCKET4OBJECT_H_ZMH_2006_06_05
/** @} */ // end of 
