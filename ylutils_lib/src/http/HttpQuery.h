/*
 * =====================================================================================
 * 
 *        Filename:  HttpQuery.h
 * 
 *     Description:  用户传来的原始查询.
 * 
 *         Version:  1.0
 *         Created:  2006年06月29日 15时41分19秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
 /** @defgroup http
 *  便利http操作
 *  @{
 */

#ifndef HTTPQUERY_H_ZMH_2006_06_29
#define HTTPQUERY_H_ZMH_2006_06_29

#include <iostream>
#include <string>
#include <map>

#include "container/SimpleVector.h"

using namespace std;
/*! \brief 获取用户传来的原始查询.
 *! \class HttpQuery HttpQuery.h http/HttpQuery.h    
 *	
 * \version 1.0
 \code
 static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " query_string" << endl;

	cout << "查询串分析器测试 :" << endl << endl;

	cout << "query_string :                       查询串. 格式遵循GET请求的QUERY_STRING" << endl;

	return;
}

int main(int argc, char *argv[])
{

	string line;
	HttpQuery query;

	cout << "input your query string : ";
	while (getline(cin, line))
	{
		query << line;
		cout << query;	
		cout << "input your query string : ";
	}

	cout << endl;

	return 0;
}
 \endcode
 */

class HttpQuery
{
public :
	//! Constrcutor.
	HttpQuery();

	//! Destructor.
	virtual ~HttpQuery();

	//! 接收查询串并分析.
	int recv(const char *query_string);

	/**
	 * @brief    获取某一字段的字符串值.
	 * 
	 * @param    name    服务器端的字段名.
	 * @return   NULL 表示未找到.
	 *
	 */	

	const char *get(const char *name);
	/**
	 * @brief    获取某一字段的字符串值.
	 * 
	 * @param    name    服务器端的字段名.
	 * @return   NULL 表示未找到.
	 *
	 */	
	const char *get(const string &name);
	/**
	 * @brief    流输入操作符
	 * 
	 * @param    os    输入流
	 * @param    query HttpQueryInstance
	 * @return   ostream
	 *
	 */
	string get2(const char* name);
	/**
	 * @brief    流输入操作符
	 * 
	 * @param    os    输入流
	 * @param    query HttpQueryInstance
	 * @return   如果找不到则，结果为空字符串
	 *
	 */
	friend ostream &operator<<(ostream &os, HttpQuery &query);
	/**
	 * @brief    字符串输入操作符
	 * 
	 * @param    squery    服务请求的输入字符串
	 * @param    rquery HttpQueryInstance
	 * @return   HttpQeury rquery
	 *
	 */
	friend HttpQuery &operator<<(HttpQuery &rquery, const string &squery);


public :
	struct strcaselt
	{
		bool
			operator()(const char *left, const char *right)
			{
				return strcasecmp(left, right) < 0;
			}
	};

	typedef map<const char *, char *, strcaselt> T_HttpStrMap;
	typedef T_HttpStrMap::iterator T_HttpStrMapIt;
	typedef T_HttpStrMap::const_iterator T_HttpStrMapConstIt;

	const T_HttpStrMap *getParamMap() const {
		return &mNameValue;
	}

private :
	char *unescape(char *s);
private :
	T_HttpStrMap				mNameValue;
	SimpleVector<char>			mHttpQueryString;
};
ostream &operator<<(ostream &os, HttpQuery &query);
HttpQuery &operator<<(HttpQuery &rquery, const string &squery);
#endif //HTTPQUERY_H_ZMH_2006_06_29
/** @} */ // end of 
