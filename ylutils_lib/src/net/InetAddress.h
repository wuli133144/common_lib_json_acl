/*
 * =====================================================================================
 * 
 *        Filename:  InetAddress.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��05��31�� 14ʱ00��06�� CST
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

#ifndef INETADDRESS_H_ZMH_2006_05_31
#define INETADDRESS_H_ZMH_2006_05_31

#include <iostream>
#include <string>

using namespace std;
/*!
 *\brief   InetAddress
*! \class InetAddress InetAddress.h net/InetAddress.h   
 */
class InetAddress
{
public :
	//! constructor
	InetAddress();
	//! constructor
	InetAddress(const sockaddr_in &address);

	

protected :

private :

	
};
#endif //INETADDRESS_H_ZMH_2006_05_31

/** @} */ // end of 
