/*
 * =====================================================================================
 * 
 *        Filename:  SingleLog.h
 * 
 *     Description:  �������̵���־.
 *                   Ϊ��֤���߳������������־�ļ���,ȷ�������������ȴ���־��־ϵͳ
 *                   ����֤<process_id main-thread-id>����ȷӳ���ϵ.
 *                   ������getInstance()�в�û����ͬ������.
 * 
 *         Version:  1.0
 *         Created:  2006��06��10�� 14ʱ56��54�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
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

//!< ��־����.
enum ELogLevel
{
	ELOGNONE,		//!< �����.
	ELOGERROR,		//!< ��������.
	ELOGDEBUG,		//!< ������Ϣ.
	ELOGWARNING,		//!< ������Ϣ.
	ELOGINFO		//!< ������Ϣ.
};

/**
 *  ��־ģʽ. : �̼߳�����߽��̼���.
 */
enum ELogMode
{
	EPROCESSMODE, //!< ���̼��� ���̵߳���־����¼���丸���̵Ľ�����־��.
	ETHREADMODE  //!<  �̼߳��� : �̵߳���־����¼���������߳���־�ļ���.
};

/*!
 *\brief  �������̵���־����.
 *                   Ϊ��֤���߳������������־�ļ���,ȷ�������������ȴ���־��־ϵͳ
 *                   ����֤<process_id main-thread-id>����ȷӳ���ϵ.
 *                   ������getInstance()�в�û����ͬ������.
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

	//! ��ĳһ�������Ϣ��ʽ������Ӧ����־�ļ���.
	void writeLog(ELogLevel level, const char *fmt, ...);

	/**
	 * @brief    �߳���־��ģʽ.
	 *
	 * @param    threadName    �߳�����. ����Ӱ����־�ļ�������, ������ʽ : 
	 *           threadName_threadID.log
	 * @param    mode          ��־ģʽ.
	 * @param   out     ������־�ļ�
	 */
	bool openThreadLog(const char *threadName = NULL, ELogMode mode = EPROCESSMODE, FILE *out = NULL);

	//! �ر��Ѵ򿪵��߳���־. ���߳��˳�ǰӦ�õ���.
	void closeThreadLog();

	/**
	 * @brief    �򿪽�����־.
	 *
	 * @param    proname    ��������.
	 * @param    level      ��־�������.
	 * @return   ����־�ļ��ķ���ֵ.
	 * @param   out     ��־�ļ�ָ��
	 * @retval   true    �򿪳ɹ�.
	 * @retval   false   ��ʧ��.
	 */
	bool openProcessLog(const char *proname, ELogLevel level, FILE *out = NULL);

protected :
	SingleLog();

private :
	typedef struct _log_info
	{
		FILE		*_mFile;	//!< ��־�ļ�������.
		ELogMode	 _mMode;	//!< ��־��¼ģʽ, �̼߳����߽��̼�.
		string		 _mFileName;	//!< ��־�ļ���.
		string		 _mThreadName;	//!< �߳�����.
	} _TLogInfo;

	/**
	 * @brief    ���̺Ż����̺߳Ŷ�Ӧ�ĸ��Ե���־��Ϣ.
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
