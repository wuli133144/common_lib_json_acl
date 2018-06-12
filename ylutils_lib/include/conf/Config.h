/*
 * =====================================================================================
 * 
 *        Filename:  Config.h
 * 
 *     Description:  配置文件接口
 * 
 *         Version:  1.0
 *         Created:  2006年04月04日 18时33分19秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
 /*!
 * \file Config.h
 * \brief 配置文件接口.
 *
 * 配置文件接口.
 * \author  张明辉
 * \date 2006年04月04日 18时33分19秒 CST
 */
 
/** @defgroup conf
 *  配置文件读取
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

/*! \brief 配置文件读取类   (name=value)
 *! \class Config Config.h conf/Config.h   
 *  对形如 name=value 的配置文件读取操作
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
	 * @brief    将配置文件载入内存.
	 * \exception IOException
	 * @param    cfgfile    配置文件名字.
	 */
	bool load(const string &cfgfile = "") throw(IOException);
	
	// 将配置中的<name, value>全部转换出来.
	string toString();

	/**
	 * @brief    获取某一属性的值.
	 *
	 * @param    value    属性的值.
	 * @param    name     属性名字.
	 * @return   true     找到该字段.
	 *           false    未找到该字段.
	 */
	bool getValue(string &value, const string &name);

	/*
	 * @brief    获取某一字段的值.
	 *
	 * @param    name    字段名.
	 * @return   NULL 未找到该字段.
	 */
	const char *getValue(const string &name);

	/**
	 * @brief    获取某一属性的long值.
	 *
	 * @param    value    属性的值.
	 * @param    name     属性名字.
	 * @return   true     找到该字段.
	 *           false    未找到该字段.
	 */
	bool getValueOfLong(long &value, const string &name);
/**
	 * @brief    获取某一属性的整型值.
	 *
	 * @param    value    属性的值.
	 * @param    name     属性名字.
	 * @return   true     找到该字段.
	 *           false    未找到该字段.
	 */
	bool getValueOfInt(int &value, const string &name);
	/**
	*@brief   operator<< 
	*/
	friend ostream &operator<<(ostream &os, const Config &config);

protected :

private :
	//! 消去两端的非可视字符.
	string trim(string &str);
private :
	string mConfigFile;
	map<string, string> mNameValue;
};
ostream &operator<<(ostream &os, const Config &config);
#endif //CONFIG_H_ZMH_2006_04_04
/** @} */ // end of conf
