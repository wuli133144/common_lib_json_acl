/*
 * =====================================================================================
 * 
 *        Filename:  AcceptSocket4Object.h
 * 
 *     Description:  在客户端和服务器之间传递对象.隐藏了对象构造的细节.
 *
 *                   必须为Object方法实现toString()/c_str() 和 size()方法.
 * 
 *         Version:  1.0
 *         Created:  2006年06月05日 16时16分35秒 CST
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

#ifndef ACCEPTSOCKET4OBJECT_H_ZMH_2006_06_05
#define ACCEPTSOCKET4OBJECT_H_ZMH_2006_06_05

#include <iostream>
#include <string>

#include "AcceptSocket.h"

using namespace std;
/*!
 *\brief   在客户端和服务器之间传递对象.隐藏了对象构造的细节.
 *
 *                   必须为Object方法实现toString()/c_str() 和 size()方法.
 *! \class AcceptSocket4Object AcceptSocket4Object.h net/AcceptSocket4Object.h   
 */
template <class Object>
class AcceptSocket4Object : public AcceptSocket
{
public :
	//! constructor
	AcceptSocket4Object(int socket) throw(SocketException);
	//! deconstrutor
	virtual ~AcceptSocket4Object();
/**
	 * @brief    发送Object
	 *
	 * @param    object  对象实例
	 * @return  成功发送的size
	 */
	ssize_t sendObject(const Object &object) throw(SocketException);
	
	 	/**
	 * @brief    接受Object
	 *
	 * @param    object  结果存储于此对象实例
	 * @return  成功接收的size
	 */
	ssize_t recvObject(Object &object) throw(SocketException);

protected :

private :

	
};

template <class Object>
AcceptSocket4Object<Object>::AcceptSocket4Object(int socket) throw(SocketException)
	: AcceptSocket(socket) 
{
}

template <class Object>
AcceptSocket4Object<Object>::~AcceptSocket4Object()
{
}

template <class Object>
ssize_t AcceptSocket4Object<Object>::sendObject(const Object &object) throw(SocketException)
{
	//! send body's len.
	int size = object.size();
	safeWrite((char *)&size, sizeof(int));

	//! send body's bytes.
	safeWrite((char *)object.c_str(), object.size());

	return object.size() + sizeof(int);
}

template <class Object>
ssize_t AcceptSocket4Object<Object>::recvObject(Object &object) throw(SocketException)
{
	int tLen = 0;
	safeRead((char *)&tLen, sizeof(int));

	safeRead(object.address(), tLen);

	return tLen + sizeof(tLen);
}
#endif //ACCEPTSOCKET4OBJECT_H_ZMH_2006_06_05
/** @} */ // end of 
