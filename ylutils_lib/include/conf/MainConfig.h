/*
 * =====================================================================================
 * 
 *        Filename:  MainConfig.h
 * 
 *     Description:  单进程往往有一个主配置是各个模块之间共享的, 通过指针进行传递比较复杂.
 * 
 *         Version:  1.0
 *         Created:  2006年08月30日 18时22分13秒 CST
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
 * \file MainConfig.h
 * \brief 单进程往往有一个主配置是各个模块之间共享的, 通过指针进行传递比较复杂.
 *
 * \author  张明辉
 * \date 2006年08月30日 18时22分13秒 CST
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
/*! \brief 配置文件读取单实例类   (name=value)
 *    单进程往往有一个主配置是各个模块之间共享的, 通过指针进行传递比较复杂.
 *! \class MainConfig MainConfig.h conf/MainConfig.h    
 *   \version 1.0    
 */

class MainConfig : public Config
{
public :
	//!< 获取单一实例.
	static MainConfig *instance(const string &conf_f);

	//!< 调用supper->load()方法去做载入操作. 具体的时候请查看父类的方法.

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
