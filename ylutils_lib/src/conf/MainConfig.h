/*
 * =====================================================================================
 * 
 *        Filename:  MainConfig.h
 * 
 *     Description:  ������������һ���������Ǹ���ģ��֮�乲���, ͨ��ָ����д��ݱȽϸ���.
 * 
 *         Version:  1.0
 *         Created:  2006��08��30�� 18ʱ22��13�� CST
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
 * \file MainConfig.h
 * \brief ������������һ���������Ǹ���ģ��֮�乲���, ͨ��ָ����д��ݱȽϸ���.
 *
 * \author  ������
 * \date 2006��08��30�� 18ʱ22��13�� CST
 */
/** @addtogroup conf
 *  
 *  
 *  @{
 */

#ifndef MAINCONFIG_H_ZMH_2006_08_30
#define MAINCONFIG_H_ZMH_2006_08_30

#include <iostream>
#include <string>
#include "Config.h"
#include "sync/ThreadCondLock.h"

using namespace std;
/*! \brief �����ļ���ȡ��ʵ����   (name=value)
 *    ������������һ���������Ǹ���ģ��֮�乲���, ͨ��ָ����д��ݱȽϸ���.
 *! \class MainConfig MainConfig.h conf/MainConfig.h    
 *   \version 1.0    
 */

class MainConfig : public Config
{
public :
	//!< ��ȡ��һʵ��.
	static MainConfig *instance(const string &conf_f);

	//!< ����supper->load()����ȥ���������. �����ʱ����鿴����ķ���.

	//!< Destructor.
	virtual ~MainConfig();
	
protected :
	//!< Constructor.
	MainConfig(const string &conf_f);

private :
	static MainConfig 	*mInstance;
	static ThreadCondLock	mLock;
};
#endif //MAINCONFIG_H_ZMH_2006_08_30
/** @} */ // end of conf
