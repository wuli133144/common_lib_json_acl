/*
 * =====================================================================================
 * 
 *        Filename:  LocalLog.h
 * 
 *     Description:  ������־�����¼�û��Ĳ�ѯ��ʵ��ʷ, �͵�����־ϵͳ��ͬ, ������־
 *                   ��������ĵ���������еĸ�������.
 *                   ������־ϵͳ�ļ��ض��̹߳���һ���ļ�, д��־����Ҫ����.
 *                   ������־ϵͳ�ᰴ������Ϊ��λ��ʵ��¼�û���ÿһ������.
 * 
 *         Version:  1.0
 *         Created:  2006��07��17�� 20ʱ54��04�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */

 /** @defgroup log
 *  ��־���
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
/*! \brief ������־�����¼�û��Ĳ�ѯ��ʵ��ʷ, �͵�����־ϵͳ��ͬ, ������־
 *                   ��������ĵ���������еĸ�������.
 *                   ������־ϵͳ�ļ��ض��̹߳���һ���ļ�, д��־����Ҫ����.
 *                   ������־ϵͳ�ᰴ������Ϊ��λ��ʵ��¼�û���ÿһ������..
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

	//! �����е���־��.
	bool openAllStream() throw(IOException);

	//! ������־·��.
	void setLogDir(const string &logpath);

	//! ׷�������Ĳ�ѯ��־�������ļ���.
	void appQueryLog(const string &logmsg);

	//! ׷�ӷ������Ĵ�����Ϣ��֮Ϊ������Ϣ�������ļ���.
	void appDebugLog(const string &logmsg);

	//! ׷��ͳ����Ϣ�������ļ���.
	void appStatLog(const string &logmsg);

	//! ׷�������Ĵ�����Ϣ�������ļ����Ա�����������������м���Ѱ.
	void appErrorLog(const string &logmsg);

protected :
private :
	string getTodayLogFileName(const string &fntail);

	void reOpenAllLog();

	int getToday();
private :
	string		mLogDir;		//!< ��־�ļ�Ŀ¼.
	int		mToday;			//!< ��January 1, 1970������.
	ofstream	mDebugFile;		//!< debug�ļ�ָ��.
	ofstream	mQueryFile;		//!< query�ļ�ָ��.
	ofstream	mErrorFile;		//!< error�ļ�ָ��.

	FILE		*mDebugStream;
	FILE		*mQueryStream;
	FILE		*mErrorStream;
	FILE		*mStatStream;


	pthread_mutex_t			mMutex;
};
#endif //LOCALLOG_H_ZMH_2006_07_17

/** @} */ // end of 
