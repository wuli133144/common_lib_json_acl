/*
 * =====================================================================================
 * 
 *        Filename:  Exception.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2007��04��12�� 14ʱ39��53�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */



 /** @defgroup exception
 *  �쳣
 *  @{
 */

#ifndef EXCEPTION_H_ZMH_2007_04_12
#define EXCEPTION_H_ZMH_2007_04_12

#include <iostream>
#include <string>

using namespace std;
/*! \brief Exception �̳��� std::exception
 *! \class Exception Exception.h exception/Exception.h    
 *  
 *   \version 1.0    
 */
class Exception : public std::exception
{
public :
	//!< Constructor.
	Exception(const string &msg) throw() : mMsg(msg) {}
	//!< Constructor 
	//!param errn errno
	Exception(int errn) throw() : mMsg(strerror(errn)) {}
	//!< Destructor.
	virtual ~Exception() throw() {}
	//! return error msg
	virtual const char *what() const throw() {return mMsg.c_str();}



protected :

private :
	string mMsg;
};
#endif //EXCEPTION_H_ZMH_2007_04_12
/** @} */ // end of 
