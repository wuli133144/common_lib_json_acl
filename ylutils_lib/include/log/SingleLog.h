/*
 * =====================================================================================
 * 
 *        Filename:  SingleLog.h
 * 
 *     Description:  整个进程的日志.
 *                   为保证主线程输出到进程日志文件中,确保由主进程首先打开日志日志系统
 *                   来保证<process_id main-thread-id>的正确映射关系.
 *                   所以在getInstance()中并没有做同步处理.
 * 
 *         Version:  1.0
 *         Created:  2006年06月10日 14时56分54秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup log
 *  
 *  
 *  @{
 */

#ifndef SINGLELOG_H_ZMH_2006_06_10
#define SINGLELOG_H_ZMH_2006_06_10

#include <iostream>
#include <sstream>
#include <string>
#include <ext/hash_map>
#include <errno.h>

#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
using namespace __gnu_cxx;

//!< 日志级别.
enum ELogLevel
{
	ELOGNONE,		//!< 不输出.
	ELOGERROR,		//!< 致命错误.
	ELOGDEBUG,		//!< 调试信息.
	ELOGWARNING,		//!< 警告信息.
	ELOGINFO		//!< 所有信息.
};

/**
 *  日志模式. : 线程级别或者进程级别.
 */
enum ELogMode
{
	EPROCESSMODE, //!< 进程级别 ：线程的日志将记录到其父进程的进程日志中.
	ETHREADMODE  //!<  线程级别 : 线程的日志将记录到单独的线程日志文件中.
};

/*!
 *\brief  整个进程的日志单例.
 *                   为保证主线程输出到进程日志文件中,确保由主进程首先打开日志日志系统
 *                   来保证<process_id main-thread-id>的正确映射关系.
 *                   所以在getInstance()中并没有做同步处理.
 *! \class SingleLog SingleLog.h log/SingleLog.h   
 \code
 
int call()
{
	SingleLog::getInstance()->writeLog(ELOGWARNING, "hello, I send log message");
	return 0;
}

void *crawler(void *arg)
{
	SingleLog *plog = SingleLog::getInstance();
	//plog->openThreadLog("crawler", ETHREADMODE);
	plog->openThreadLog("crawler", EPROCESSMODE);

	call();

	plog->closeThreadLog();

	cout << "can output?" << endl;

	return NULL;
}

int main()
{
	SingleLog *plog = SingleLog::getInstance();
	plog->setLogDir("./log/");
	//plog->openProcessLog("main_crawler", ELOGINFO, stderr);
	plog->openProcessLog("main_crawler", ELOGINFO);

	plog->writeLog(ELOGWARNING, "open process log");
	
	pthread_t pid1, pid2;
	pthread_create(&pid1, NULL, crawler, NULL);
	pthread_create(&pid2, NULL, crawler, NULL);

	plog->writeLog(ELOGWARNING, "waiting for my children...");
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	plog->writeLog(ELOGWARNING, "two children over...");

	return 0;
}
 \endcode
 */
class SingleLog
{
public :
	//! Destrutor.
	virtual ~SingleLog();

	//! get an instance.
	static SingleLog *getInstance();

	//! set the log level.
	void setLogLevel(ELogLevel level);
	//! set the log dir
	void setLogDir(const string &logDir);

	//! 将某一级别的信息格式化到相应的日志文件中.
	void writeLog(ELogLevel level, const char *fmt, ...);

	/**
	 * @brief    线程日志打开模式.
	 *
	 * @param    threadName    线程名字. 他将影响日志文件的名字, 命名方式 : 
	 *           threadName_threadID.log
	 * @param    mode          日志模式.
	 * @param   out     输入日志文件
	 */
	bool openThreadLog(const char *threadName = NULL, ELogMode mode = EPROCESSMODE, FILE *out = NULL);

	//! 关闭已打开的线程日志. 在线程退出前应该调用.
	void closeThreadLog();

	/**
	 * @brief    打开进程日志.
	 *
	 * @param    proname    进程名字.
	 * @param    level      日志输出级别.
	 * @return   打开日志文件的返回值.
	 * @param   out     日志文件指针
	 * @retval   true    打开成功.
	 * @retval   false   打开失败.
	 */
	bool openProcessLog(const char *proname, ELogLevel level, FILE *out = NULL);

protected :
	SingleLog();

private :
	typedef struct _log_info
	{
		FILE		*_mFile;	//!< 日志文件描述符.
		ELogMode	 _mMode;	//!< 日志记录模式, 线程级或者进程级.
		string		 _mFileName;	//!< 日志文件名.
		string		 _mThreadName;	//!< 线程名字.
	} _TLogInfo;

	/**
	 * @brief    进程号或者线程号对应的各自的日志信息.
	 */
	typedef hash_map<unsigned long int, _TLogInfo> _TLogMap;
	typedef _TLogMap::iterator _TLogMapIterator;
private :
	static SingleLog	*mInstance;
	string			 mLogDir;
	ELogLevel		 mLogLevel;
	string			 mProcessName;
	pthread_mutex_t		 mLogMutex;
	_TLogMap		 mLogMap;
};
#endif //SINGLELOG_H_ZMH_2006_06_10
/** @} */ // end of 
