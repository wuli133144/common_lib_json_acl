/*
 * =====================================================================================
 * 
 *        Filename:  SocketException.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年05月30日 19时52分24秒 CST
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

#ifndef SOCKETEXCEPTION_H_ZMH_2006_05_30
#define SOCKETEXCEPTION_H_ZMH_2006_05_30

#include <iostream>
#include <string>

using namespace std;
/*!
 *\brief   SocketException.
*! \class SocketException SocketException.h net/SocketException.h   
 */
class SocketException : public std::exception
{
public :
	//! constructor
	SocketException(const string &msg) throw(): mMsg(msg) {}
	//! destructor
	virtual ~SocketException() throw() {}
	//! error msg
	virtual const char *what() const throw() { return mMsg.c_str(); }
	//! append error msg to self.
	void append(const string &msg) { mMsg.append(msg); return; }

private :
	string mMsg;
};

#endif //SOCKETEXCEPTION_H_ZMH_2006_05_30
/** @} */ // end of 
