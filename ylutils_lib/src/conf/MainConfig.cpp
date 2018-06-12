/*
 * =====================================================================================
 * 
 *        Filename:  MainConfig.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��08��30�� 18ʱ27��46�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ (), minghui.zhang@yeelion.com
 *         Company:  �����Ƽ�
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
