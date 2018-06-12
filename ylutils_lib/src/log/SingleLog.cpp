/*
 * =====================================================================================
 * 
 *        Filename:  SingleLog.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��06��12�� 15ʱ02��57�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ (), minghui.zhang@yeelion.com
 *         Company:  �����Ƽ�
 * 
 * =====================================================================================
 */

#include "SingleLog.h"
#include <set>

SingleLog *SingleLog::mInstance = NULL;

SingleLog::SingleLog()
	:
	mLogDir("./"),
	mLogLevel(ELOGNONE),
	mProcessName("")
{
	pthread_mutex_init(&mLogMutex, NULL);
}

SingleLog::~SingleLog()
{
	pthread_mutex_lock(&mLogMutex);
	set<FILE *> __deleted_f;
	for (_TLogMapIterator it = mLogMap.begin(); it != mLogMap.end(); it++)
	{
		if (it->second._mFile == stdout || it->second._mFile == stderr)
			continue;

		if (__deleted_f.find(it->second._mFile) == __deleted_f.end())
		{
			__deleted_f.insert(it->second._mFile);
			fclose(it->second._mFile);
		}
	}
	pthread_mutex_unlock(&mLogMutex);
	pthread_mutex_destroy(&mLogMutex);
}

SingleLog *SingleLog::getInstance()
{
	if (mInstance == NULL)
	{
		static SingleLog tInstance;
		mInstance = &tInstance;
	}

	return mInstance;
}

void SingleLog::setLogDir(const string &logDir)
{
	mLogDir = logDir;

	return;
}

void SingleLog::setLogLevel(ELogLevel level)
{
	mLogLevel = level;

	return;
}

bool SingleLog::openProcessLog(const char *proname, ELogLevel level, FILE *out)
{
	//! ����Ѿ����򿪹��Ͳ��ٴ�.
	if (!mProcessName.empty())
	{
		return true;
	}

	mProcessName = (proname==NULL)?"null":proname;
	mLogLevel = level;

	//! ���������־�ļ�������.
	ostringstream oProLogName;
	oProLogName << mLogDir << "/P_" << mProcessName << "_" << getpid() << ".log";

	FILE *pFile = (out!=NULL)?out:fopen(oProLogName.str().c_str(), "a");
	if (pFile == NULL)
	{
		cerr << "open file " << oProLogName.str() << " : " << strerror(errno) << endl;
		return false;
	}

	//! ��������־��Ϣ���뵽ȫ�ֵ���־map��.
	_TLogInfo tLogInfo = {pFile, EPROCESSMODE, oProLogName.str(), mProcessName};

	pthread_mutex_lock(&mLogMutex);
	mLogMap.insert(make_pair(pthread_self(), tLogInfo));
	mLogMap.insert(make_pair(getpid(), tLogInfo));
	pthread_mutex_unlock(&mLogMutex);

	return true;
}

bool SingleLog::openThreadLog(const char *threadName, ELogMode mode, FILE *out)
{
	//! �Ƿ���߳��Ѿ��򿪹���־�ļ�.
	pthread_mutex_lock(&mLogMutex);
	if (mLogMap.find(pthread_self()) != mLogMap.end())
	{
		pthread_mutex_unlock(&mLogMutex);
		return true;
	}

	_TLogInfo tLogInfo;

	//! ����ǽ���ģʽ��ʹ�ý��̵��ļ�ָ��.
	if (mode == EPROCESSMODE)
	{
		_TLogMapIterator it = mLogMap.find(getpid());
		if (it == mLogMap.end())
		{
			pthread_mutex_unlock(&mLogMutex);
			return false;
		}
		tLogInfo = it->second;
		tLogInfo._mThreadName = (threadName==NULL)?"null":threadName;
	}
	else if (mode == ETHREADMODE)
	{
		//! ������־�ļ�������.
		ostringstream oThreadLogName;
		oThreadLogName << mLogDir << "/T_" << ((threadName==NULL)?"null":threadName)
			<< "_" << pthread_self() << ".log";

		FILE *pFile = (out!=NULL)?out:fopen(oThreadLogName.str().c_str(), "a");
		if (pFile == NULL)
		{
			pthread_mutex_unlock(&mLogMutex);
			return false;
		}

		tLogInfo._mFile = pFile;
		tLogInfo._mMode = mode;
		tLogInfo._mFileName = oThreadLogName.str();
		tLogInfo._mThreadName = (threadName==NULL)?"null":threadName;
	}
	else
	{
		pthread_mutex_unlock(&mLogMutex);
		return false;
	}

	//! ��ӳ���ϵ���뵽map��.
	mLogMap.insert(make_pair(pthread_self(), tLogInfo));

	pthread_mutex_unlock(&mLogMutex);
	
	return true;
}

void SingleLog::writeLog(ELogLevel level, const char *fmt, ...)
{
	if (mLogLevel < level)
		return;

	//! ��ȡ��־�ļ�ָ��. ���������hash_mapʹ���ڲ�ѯʱ���ü���.
	_TLogMapIterator it = mLogMap.find(pthread_self());
	if (it == mLogMap.end())
		return;

	FILE *pFile = it->second._mFile;

	ostringstream ofmt;
	switch (level)
	{
	case ELOGNONE : 
		return;
	case ELOGWARNING :
		ofmt << "WARNING: ";
		break;
	case ELOGINFO :
		ofmt << "INFO:";
		break;
	case ELOGDEBUG :
		ofmt << "DEBUG: ";
		break;
	case ELOGERROR :
		ofmt << "ERROR: ";
		break;
	default :
		return;
	}

	ofmt << it->second._mThreadName << ": " << pthread_self() << ":";

	//! д����־�ļ�.
	va_list args;
	va_start(args, fmt);
	ofmt << fmt << '\n';
	vfprintf(pFile, ofmt.str().c_str(), args);
	va_end(args);
	fflush(pFile);
	return;
}

void SingleLog::closeThreadLog()
{
	pthread_mutex_lock(&mLogMutex);

	_TLogMapIterator it = mLogMap.find(pthread_self());
	if (it != mLogMap.end()) {
		if (it->second._mMode == ETHREADMODE && it->second._mFile != stdout
				&& it->second._mFile != stderr) {
			fclose(it->second._mFile);
		}
		mLogMap.erase(it);
	}

	pthread_mutex_unlock(&mLogMutex);

	return;
}



#ifdef _TEST

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
#endif //_TEST
