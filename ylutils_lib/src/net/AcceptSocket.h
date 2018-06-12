/*
 * =====================================================================================
 * 
 *        Filename:  AcceptSocket.h
 * 
 *     Description:  ServerSocket接收到的socket.
 * 
 *         Version:  1.0
 *         Created:  2006年06月05日 12时26分58秒 CST
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

#ifndef ACCEPTSOCKET_H_ZMH_2006_06_05
#define ACCEPTSOCKET_H_ZMH_2006_06_05

#include <iostream>
#include <string>

#include "std_socket.h"

using namespace std;
/*!
 *\brief   ServerSocket接收到的socket.
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
	 * @brief    根据socket描述符获取本地主机的地址和远程客户机的地址.
	 */
	void assignAddressBySocket() throw(SocketException);
};
#endif //ACCEPTSOCKET_H_ZMH_2006_06_05
/** @} */ // end of 
