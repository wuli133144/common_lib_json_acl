/*
 * =====================================================================================
 * 
 *        Filename:  InetAddress.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年05月31日 14时00分06秒 CST
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
