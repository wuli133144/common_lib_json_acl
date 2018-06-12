/*
 * =====================================================================================
 * 
 *        Filename:  MainConfig.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年08月30日 18时27分46秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "MainConfig.h"

MainConfig *MainConfig::mInstance = NULL;
ThreadCondLock MainConfig::mLock;

MainConfig *MainConfig::instance(const string &conf_f)
{
	if (mInstance == NULL) {
		mLock.lock();
		if (mInstance == NULL) {
			static MainConfig __config(conf_f);
			mInstance = &__config;
		}
		mLock.unlock();
	}

	return mInstance;
}

MainConfig::MainConfig(const string &conf_f)
	:
	Config(conf_f)
{
}
	
MainConfig::~MainConfig()
{
}
