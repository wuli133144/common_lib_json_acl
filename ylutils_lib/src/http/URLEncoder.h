/*
 * =====================================================================================
 * 
 *        Filename:  URLEncoder.h
 * 
 *     Description:  为URL进行RFC规定格式的编码.
 * 
 *         Version:  1.0
 *         Created:  2005年10月08日 16时58分59秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 * 
 * =====================================================================================
 */
 /** @addtogroup http
 *  便利http操作
 *  @{
 */

#ifndef URLENCODER_H_ZMH_2005_10_08
#define URLENCODER_H_ZMH_2005_10_08

#include <iostream>
#include <string>

using namespace std;
/*! \brief 为URL进行RFC规定格式的编码.
 *         
*! \class URLEncoder URLEncoder.h http/URLEncoder.h    
 * \version 1.0
 \code
 int main() {
	string query;

	cout << "please input your query : ";
	while (getline(cin, query)) {
		cout << URLEncoder::encodeQuery(query) << endl;	
		cout << URLEncoder::encode(query) << endl;	
		cout << "please input your query : ";
	}
	cout << endl;

	return 0;
}
 \endcode
 
 */

class URLEncoder
{
public :
	//! Constructor.
	URLEncoder();

	//! Destructor.
	virtual ~URLEncoder();

	/**
	 * @brief    依照RFC2396对url字符串进行编码.
	 *
	 * @param    url   string to be translated.
	 * return    the translated string.
	 */
	static string encode(const string &url);

	static string encode(const char* url, int len);

	/**
	 * @brief    同上.
	 *
	 * @param    to    编码后的结果buffer.
	 * @param    from  源url.
	 * @return   返回编码后的字符串.
	 */
	static char *encode(char *tourl, const char *fromurl);

	/**
	 * @brief    对query_string中的value进行编码.
	 *
	 * @param    query    源query值.
	 * @return   返回编码后的结果.
	 */
	static string encodeQuery(const string &query);

protected :

private :
};
#endif //URLENCODER_H_ZMH_2005_10_08
/** @} */ // end of 
