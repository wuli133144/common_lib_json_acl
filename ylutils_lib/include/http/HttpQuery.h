/*
 * =====================================================================================
 * 
 *        Filename:  HttpQuery.h
 * 
 *     Description:  �û�������ԭʼ��ѯ.
 * 
 *         Version:  1.0
 *         Created:  2006��06��29�� 15ʱ41��19�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
 /** @defgroup http
 *  ����http����
 *  @{
 */

#ifndef HTTPQUERY_H_ZMH_2006_06_29
#define HTTPQUERY_H_ZMH_2006_06_29

#include <iostream>
#include <string>
#include <map>

#include "container/SimpleVector.h"

using namespace std;
/*! \brief ��ȡ�û�������ԭʼ��ѯ.
 *! \class HttpQuery HttpQuery.h http/HttpQuery.h    
 *	
 * \version 1.0
 \code
 static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " query_string" << endl;

	cout << "��ѯ������������ :" << endl << endl;

	cout << "query_string :                       ��ѯ��. ��ʽ��ѭGET�����QUERY_STRING" << endl;

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

	//! ���ղ�ѯ��������.
	int recv(const char *query_string);

	/**
	 * @brief    ��ȡĳһ�ֶε��ַ���ֵ.
	 * 
	 * @param    name    �������˵��ֶ���.
	 * @return   NULL ��ʾδ�ҵ�.
	 *
	 */	

	const char *get(const char *name);
	/**
	 * @brief    ��ȡĳһ�ֶε��ַ���ֵ.
	 * 
	 * @param    name    �������˵��ֶ���.
	 * @return   NULL ��ʾδ�ҵ�.
	 *
	 */	
	const char *get(const string &name);
	/**
	 * @brief    �����������
	 * 
	 * @param    os    ������
	 * @param    query HttpQueryInstance
	 * @return   ostream
	 *
	 */
	string get2(const char* name);
	/**
	 * @brief    �����������
	 * 
	 * @param    os    ������
	 * @param    query HttpQueryInstance
	 * @return   ����Ҳ����򣬽��Ϊ���ַ���
	 *
	 */
	friend ostream &operator<<(ostream &os, HttpQuery &query);
	/**
	 * @brief    �ַ������������
	 * 
	 * @param    squery    ��������������ַ���
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
