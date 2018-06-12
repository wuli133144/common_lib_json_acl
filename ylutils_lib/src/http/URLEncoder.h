/*
 * =====================================================================================
 * 
 *        Filename:  URLEncoder.h
 * 
 *     Description:  ΪURL����RFC�涨��ʽ�ı���.
 * 
 *         Version:  1.0
 *         Created:  2005��10��08�� 16ʱ58��59�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 * 
 * =====================================================================================
 */
 /** @addtogroup http
 *  ����http����
 *  @{
 */

#ifndef URLENCODER_H_ZMH_2005_10_08
#define URLENCODER_H_ZMH_2005_10_08

#include <iostream>
#include <string>

using namespace std;
/*! \brief ΪURL����RFC�涨��ʽ�ı���.
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
	 * @brief    ����RFC2396��url�ַ������б���.
	 *
	 * @param    url   string to be translated.
	 * return    the translated string.
	 */
	static string encode(const string &url);

	static string encode(const char* url, int len);

	/**
	 * @brief    ͬ��.
	 *
	 * @param    to    �����Ľ��buffer.
	 * @param    from  Դurl.
	 * @return   ���ر������ַ���.
	 */
	static char *encode(char *tourl, const char *fromurl);

	/**
	 * @brief    ��query_string�е�value���б���.
	 *
	 * @param    query    Դqueryֵ.
	 * @return   ���ر����Ľ��.
	 */
	static string encodeQuery(const string &query);

protected :

private :
};
#endif //URLENCODER_H_ZMH_2005_10_08
/** @} */ // end of 
