/*
 * =====================================================================================
 * 
 *        Filename:  Config.h
 * 
 *     Description:  �����ļ��ӿ�
 * 
 *         Version:  1.0
 *         Created:  2006��04��04�� 18ʱ33��19�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
 /*!
 * \file Config.h
 * \brief �����ļ��ӿ�.
 *
 * �����ļ��ӿ�.
 * \author  ������
 * \date 2006��04��04�� 18ʱ33��19�� CST
 */
 
/** @defgroup conf
 *  �����ļ���ȡ
 *  @{
 */


#ifndef CONFIG_H_ZMH_2006_04_04
#define CONFIG_H_ZMH_2006_04_04

#include <iostream>
#include <sstream>
#include <map>
#include <fstream>
#include <string>

#include "exception/IOException.h"

using namespace std;

/*! \brief �����ļ���ȡ��   (name=value)
 *! \class Config Config.h conf/Config.h   
 *  ������ name=value �������ļ���ȡ����
 *   \version 1.0    
 \code
 
static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " configfile" << endl;

	return;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		dohelp(argc, argv);
		return -1;
	}

	Config config(argv[1]);
	config.load();

	cout << config.toString() << endl;
	int value = 0;
	cout << (config.getValueOfInt(value, "int")?value:0) << endl;
	cout << (config.getValueOfInt(value, "long")?value:0) << endl;

	return 0;
}
 \endcode
 */
class Config
{
public :
	//! Constructor.
	Config();

	//! Constructor.
	Config(const string &cfgfile);

	//! Destructor.
	virtual ~Config();

	/**
	 * @brief    �������ļ������ڴ�.
	 * \exception IOException
	 * @param    cfgfile    �����ļ�����.
	 */
	bool load(const string &cfgfile = "") throw(IOException);
	
	// �������е�<name, value>ȫ��ת������.
	string toString();

	/**
	 * @brief    ��ȡĳһ���Ե�ֵ.
	 *
	 * @param    value    ���Ե�ֵ.
	 * @param    name     ��������.
	 * @return   true     �ҵ����ֶ�.
	 *           false    δ�ҵ����ֶ�.
	 */
	bool getValue(string &value, const string &name);

	/*
	 * @brief    ��ȡĳһ�ֶε�ֵ.
	 *
	 * @param    name    �ֶ���.
	 * @return   NULL δ�ҵ����ֶ�.
	 */
	const char *getValue(const string &name);

	/**
	 * @brief    ��ȡĳһ���Ե�longֵ.
	 *
	 * @param    value    ���Ե�ֵ.
	 * @param    name     ��������.
	 * @return   true     �ҵ����ֶ�.
	 *           false    δ�ҵ����ֶ�.
	 */
	bool getValueOfLong(long &value, const string &name);
/**
	 * @brief    ��ȡĳһ���Ե�����ֵ.
	 *
	 * @param    value    ���Ե�ֵ.
	 * @param    name     ��������.
	 * @return   true     �ҵ����ֶ�.
	 *           false    δ�ҵ����ֶ�.
	 */
	bool getValueOfInt(int &value, const string &name);
	/**
	*@brief   operator<< 
	*/
	friend ostream &operator<<(ostream &os, const Config &config);

protected :

private :
	//! ��ȥ���˵ķǿ����ַ�.
	string trim(string &str);
private :
	string mConfigFile;
	map<string, string> mNameValue;
};
ostream &operator<<(ostream &os, const Config &config);
#endif //CONFIG_H_ZMH_2006_04_04
/** @} */ // end of conf
