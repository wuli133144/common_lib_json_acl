/*
 * =====================================================================================
 * 
 *        Filename:  std_socket4object.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��06��05�� 17ʱ14��28�� CST
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

#ifndef STD_SOCKET4OBJECT_H_ZMH_2006_06_05
#define STD_SOCKET4OBJECT_H_ZMH_2006_06_05

#include <iostream>
#include <string>

#include "std_socket.h"

using namespace std;
/*!
 *\brief   std_socket4object.
*! \class _Std_Socket4Object Std_Socket4Object.h net/Std_Socket4Object.h   
 */
template <class Object>
class _Std_Socket4Object : public _Std_Socket
{
public :
	
	//! constructor
	_Std_Socket4Object();
	//! destructor
	virtual ~_Std_Socket4Object();
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
_Std_Socket4Object<Object>::_Std_Socket4Object()
	: _Std_Socket()
{
}

template <class Object>
_Std_Socket4Object<Object>::~_Std_Socket4Object()
{
}

template <class Object>
ssize_t _Std_Socket4Object<Object>::sendObject(const Object &object) throw(SocketException)
{
	//! send body's len.
	int size = object.size();
	safeWrite((char *)&size, sizeof(int));

	//! send body's bytes.
	safeWrite((char *)object.c_str(), object.size());

	return object.size() + sizeof(int);
}

template <class Object>
ssize_t _Std_Socket4Object<Object>::recvObject(Object &object) throw(SocketException)
{
	int tLen = 0;
	safeRead((char *)&tLen, sizeof(int));

	safeRead(object.address(), tLen);

	return tLen + sizeof(tLen);
}
#endif //STD_SOCKET4OBJECT_H_ZMH_2006_06_05
/** @} */ // end of 
