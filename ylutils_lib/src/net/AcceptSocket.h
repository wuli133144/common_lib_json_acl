/*
 * =====================================================================================
 * 
 *        Filename:  AcceptSocket.h
 * 
 *     Description:  ServerSocket���յ���socket.
 * 
 *         Version:  1.0
 *         Created:  2006��06��05�� 12ʱ26��58�� CST
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

#ifndef ACCEPTSOCKET_H_ZMH_2006_06_05
#define ACCEPTSOCKET_H_ZMH_2006_06_05

#include <iostream>
#include <string>

#include "std_socket.h"

using namespace std;
/*!
 *\brief   ServerSocket���յ���socket.
*! \class AcceptSocket AcceptSocket.h net/AcceptSocket.h   
 */
class AcceptSocket : public _Std_Socket
{
public :
	//! costructor
	//! @param socket socket_descrpitor
	AcceptSocket(int socket) throw(SocketException);
	//! destructor
	virtual ~AcceptSocket();

protected :

private :
	/**
	 * @brief    ����socket��������ȡ���������ĵ�ַ��Զ�̿ͻ����ĵ�ַ.
	 */
	void assignAddressBySocket() throw(SocketException);
};
#endif //ACCEPTSOCKET_H_ZMH_2006_06_05
/** @} */ // end of 
