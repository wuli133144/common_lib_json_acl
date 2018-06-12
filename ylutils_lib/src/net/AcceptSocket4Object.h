/*
 * =====================================================================================
 * 
 *        Filename:  AcceptSocket4Object.h
 * 
 *     Description:  �ڿͻ��˺ͷ�����֮�䴫�ݶ���.�����˶������ϸ��.
 *
 *                   ����ΪObject����ʵ��toString()/c_str() �� size()����.
 * 
 *         Version:  1.0
 *         Created:  2006��06��05�� 16ʱ16��35�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
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
 *\brief   �ڿͻ��˺ͷ�����֮�䴫�ݶ���.�����˶������ϸ��.
 *
 *                   ����ΪObject����ʵ��toString()/c_str() �� size()����.
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
	 * @brief    ����Object
	 *
	 * @param    object  ����ʵ��
	 * @return  �ɹ����͵�size
	 */
	ssize_t sendObject(const Object &object) throw(SocketException);
	
	 	/**
	 * @brief    ����Object
	 *
	 * @param    object  ����洢�ڴ˶���ʵ��
	 * @return  �ɹ����յ�size
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
