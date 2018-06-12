/*
 * =====================================================================================
 * 
 *        Filename:  IOException.h
 * 
 *     Description:  IO异常类.
 * 
 *         Version:  1.0
 *         Created:  2006年08月24日 18时36分27秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup exception
 *  
 *  
 *  @{
 */

#ifndef IOEXCEPTION_H_ZMH_2006_08_24
#define IOEXCEPTION_H_ZMH_2006_08_24

#include <iostream>
#include <string>

using namespace std;
/*! \brief IOException
 *  
 *! \class IOException IOException.h exception/IOException.h    
 *   \version 1.0    
 */
class IOException : public std::exception
{
public :
	//!< Constructor.
	IOException(const string &msg) throw() : mMsg(msg) {}
	//!< Constructor 
	//@param errn errno
	IOException(int errn) throw() : mMsg(strerror(errn)) {}
	//!< Destructor.
	virtual ~IOException() throw() {}
	//! return error msg
	virtual const char *what() const throw() {return mMsg.c_str();}



protected :

private :
	string mMsg;
};
#endif //IOEXCEPTION_H_ZMH_2006_08_24
/** @} */ // end of Utils
