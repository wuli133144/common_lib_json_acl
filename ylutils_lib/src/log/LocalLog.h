/*
 * =====================================================================================
 * 
 *        Filename:  LocalLog.h
 * 
 *     Description:  本地日志负责记录用户的查询真实历史, 和调试日志系统不同, 调试日志
 *                   用来方便的调试软件运行的各个步骤.
 *                   本地日志系统的记载多线程共享一套文件, 写日志不需要加锁.
 *                   本地日志系统会按照以天为单位真实记录用户的每一次请求.
 * 
 *         Version:  1.0
 *         Created:  2006年07月17日 20时54分04秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */

 /** @defgroup log
 *  日志相关
 *  @{
 */
 
#ifndef LOCALLOG_H_ZMH_2006_07_17
#define LOCALLOG_H_ZMH_2006_07_17

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "exception/IOException.h"

using namespace std;

const string QUERYFILETAIL = ".query.log";
const string DEBUGFILETAIL = ".debug.log";
const string ERRORFILETAIL = ".error.log";
const string STATFILETAIL = ".stat.log";
/*! \brief 本地日志负责记录用户的查询真实历史, 和调试日志系统不同, 调试日志
 *                   用来方便的调试软件运行的各个步骤.
 *                   本地日志系统的记载多线程共享一套文件, 写日志不需要加锁.
 *                   本地日志系统会按照以天为单位真实记录用户的每一次请求..
 *! \class LocalLog LocalLog.h log/LocalLog.h    
 *\version 1.0	
 * 
 */

class LocalLog
{
public :
	//! Constructor.
	LocalLog(const string &logpath = "./");

	//! Destrcutor.
	virtual ~LocalLog();

	//! 打开所有的日志流.
	bool openAllStream() throw(IOException);

	//! 设置日志路径.
	void setLogDir(const string &logpath);

	//! 追加正常的查询日志到本地文件中.
	void appQueryLog(const string &logmsg);

	//! 追加非致命的错误信息称之为调试信息到本地文件中.
	void appDebugLog(const string &logmsg);

	//! 追加统计信息到本地文件中.
	void appStatLog(const string &logmsg);

	//! 追加致命的错误信息到本地文件中以便程序正常的死掉后有迹可寻.
	void appErrorLog(const string &logmsg);

protected :
private :
	string getTodayLogFileName(const string &fntail);

	void reOpenAllLog();

	int getToday();
private :
	string		mLogDir;		//!< 日志文件目录.
	int		mToday;			//!< 自January 1, 1970的天数.
	ofstream	mDebugFile;		//!< debug文件指针.
	ofstream	mQueryFile;		//!< query文件指针.
	ofstream	mErrorFile;		//!< error文件指针.

	FILE		*mDebugStream;
	FILE		*mQueryStream;
	FILE		*mErrorStream;
	FILE		*mStatStream;


	pthread_mutex_t			mMutex;
};
#endif //LOCALLOG_H_ZMH_2006_07_17

/** @} */ // end of 
