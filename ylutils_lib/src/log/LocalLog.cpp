/*
 * =====================================================================================
 * 
 *        Filename:  LocalLog.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年07月17日 21时31分20秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "LocalLog.h"
#include <errno.h>


LocalLog::LocalLog(const string &logpath)
	:
	mLogDir(logpath)
{
	//mToday = (time(NULL)/(60*60*24*1L));
	mToday = getToday();
	pthread_mutex_init(&mMutex, NULL);

	mDebugStream = mQueryStream = mErrorStream = NULL;
}

LocalLog::~LocalLog()
{
	/*
	if (mDebugFile.is_open())
		mDebugFile.close();

	if (mQueryFile.is_open())
		mQueryFile.close();

	if (mErrorFile.is_open())
		mErrorFile.close();
		*/
}

/*
bool LocalLog::openAllStream() throw(IOException)
{
	ostringstream oexcp;
	oexcp << "LoaclLog::openAllStream() ";

	if (mDebugFile.is_open())
		mDebugFile.close();
	mDebugFile.open(getTodayLogFileName(DEBUGFILETAIL).c_str(), ios_base::app);
	if (!mDebugFile) {
		oexcp << "open(" << getTodayLogFileName(DEBUGFILETAIL).c_str() << ") : " << strerror(errno);
		throw IOException(oexcp.str());
	}

	if (mQueryFile.is_open())
		mQueryFile.close();
	mQueryFile.open(getTodayLogFileName(QUERYFILETAIL).c_str(), ios_base::app);
	if (!mQueryFile) {
		oexcp << "open(" << getTodayLogFileName(QUERYFILETAIL).c_str() << ") : " << strerror(errno);
		throw IOException(oexcp.str());
	}

	if (mErrorFile.is_open())
		mErrorFile.close();
	mErrorFile.open(getTodayLogFileName(ERRORFILETAIL).c_str(), ios_base::app);
	if (!mErrorFile) {
		oexcp << "open(" << getTodayLogFileName(ERRORFILETAIL).c_str() << ") : " << strerror(errno);
		throw IOException(oexcp.str());
	}

	return true;
}
*/
bool LocalLog::openAllStream() throw(IOException)
{
	ostringstream oexcp;
	oexcp << "LoaclLog::openAllStream() ";
	string filename = getTodayLogFileName(DEBUGFILETAIL);
	mDebugStream = fopen(filename.c_str(), "a");
	if (mDebugStream == NULL) {
		oexcp << "open(" << getTodayLogFileName(DEBUGFILETAIL).c_str() << ") : " << strerror(errno);
		throw IOException(oexcp.str());
	}

	filename = getTodayLogFileName(QUERYFILETAIL);
	mQueryStream = fopen(filename.c_str(), "a");
	if (mQueryStream == NULL) {
		oexcp << "open(" << getTodayLogFileName(QUERYFILETAIL).c_str() << ") : " << strerror(errno);
		throw IOException(oexcp.str());
	}

	filename = getTodayLogFileName(ERRORFILETAIL);
	mErrorStream = fopen(filename.c_str(), "a");
	if (mErrorStream == NULL) {
		oexcp << "open(" << getTodayLogFileName(ERRORFILETAIL).c_str() << ") : " << strerror(errno);
		throw IOException(oexcp.str());
	}

	filename = getTodayLogFileName(STATFILETAIL);
	mStatStream = fopen(filename.c_str(), "a");
	if (mStatStream == NULL) {
		oexcp << "open(" << getTodayLogFileName(STATFILETAIL).c_str() << ") : " << strerror(errno);
		throw IOException(oexcp.str());
	}
	return true;
}

void LocalLog::setLogDir(const string &logpath)
{
	mLogDir = logpath;
	return;
}

void LocalLog::appQueryLog(const string &logmsg)
{
	//long __today = time(NULL)/(60*60*24*1L);
	int __today = getToday();
	if (__today != mToday)	//!< 新的一天.
	{
		//mToday = __today;
		reOpenAllLog();
	}

	//mQueryFile << logmsg << endl;
	int ret = fprintf(mQueryStream, "%s\n", logmsg.c_str());
	if (ret < 0)
	{
		ofstream flog((mLogDir+"/LAST.log").c_str(), ios_base::app);
		flog << mQueryStream->_fileno << "  " << strerror(ferror(mQueryStream)) << endl;
		flog.close();
	}
	fflush(mQueryStream);
	/*
	if (!mQueryFile || !mQueryFile.good())
	{
		reOpenAllLog();
		mQueryFile << logmsg << endl;
	}
	*/

	return;
}

void LocalLog::appDebugLog(const string &logmsg)
{
	//long __today = time(NULL)/(60*60*24*1L);
	int __today = getToday();
	if (__today != mToday)	//!< 新的一天.
	{
		reOpenAllLog();
	}

	//mDebugFile << logmsg << endl;
	fprintf(mDebugStream, "%s\n", logmsg.c_str());
	fflush(mDebugStream);
	/*
	if (!mDebugFile || !mDebugFile.good())
	{
		reOpenAllLog();
		mDebugFile << logmsg << endl;
	}
	*/

	return;
}

void LocalLog::appStatLog(const string &logmsg)
{
	//long __today = time(NULL)/(60*60*24*1L);
	int __today = getToday();
	if (__today != mToday)	//!< 新的一天.
	{
		reOpenAllLog();
	}

	//mErrorFile << logmsg << endl;
	fprintf(mStatStream, "%s\n", logmsg.c_str());
	fflush(mStatStream);
	/*
	if (!mErrorFile || !mErrorFile.good())
	{
		reOpenAllLog();
		mErrorFile << logmsg << endl;
	}
	*/

	return;
}

void LocalLog::appErrorLog(const string &logmsg)
{
	//long __today = time(NULL)/(60*60*24*1L);
	int __today = getToday();
	if (__today != mToday)	//!< 新的一天.
	{
		reOpenAllLog();
	}

	//mErrorFile << logmsg << endl;
	fprintf(mErrorStream, "%s\n", logmsg.c_str());
	fflush(mErrorStream);
	/*
	if (!mErrorFile || !mErrorFile.good())
	{
		reOpenAllLog();
		mErrorFile << logmsg << endl;
	}
	*/

	return;
}

string LocalLog::getTodayLogFileName(const string &fntail)
{
	ostringstream oss;
	oss << mLogDir << "/";

	time_t __timep = time(NULL);
	struct tm __tm;
	localtime_r(&__timep, &__tm);

	oss << 1900+__tm.tm_year << "_" << 1+__tm.tm_mon << "_" << __tm.tm_mday << fntail;

	return oss.str();
}

int LocalLog::getToday()
{
	time_t __timep = time(NULL);
	struct tm __tm;
	localtime_r(&__timep, &__tm);

	return __tm.tm_mday;
}

/*
//! openAllStream已经对目录进行了判断.
void LocalLog::reOpenAllLog()
{
	pthread_mutex_lock(&mMutex);
	if (mToday != getToday())
	{
		//! Error Log.
		string filename = getTodayLogFileName(ERRORFILETAIL);
		//mErrorFile.clear();
		if (mErrorFile.is_open())
			mErrorFile.close();
		mErrorFile.open(filename.c_str(), ios_base::app);

		//! Debug Log.
		filename = getTodayLogFileName(DEBUGFILETAIL);
		if (mDebugFile.is_open())
			mDebugFile.close();
		mDebugFile.open(filename.c_str(), ios_base::app);

		//! Query Log.
		filename = getTodayLogFileName(QUERYFILETAIL);
		if (mQueryFile.is_open())
			mQueryFile.close();
		mQueryFile.open(filename.c_str(), ios_base::app);

		mToday = getToday();
	}
	pthread_mutex_unlock(&mMutex);

	return;
}
*/

//! openAllStream已经对目录进行了判断.
void LocalLog::reOpenAllLog()
{
	pthread_mutex_lock(&mMutex);
	if (mToday != getToday())
	{
		string filename = getTodayLogFileName(DEBUGFILETAIL);
		fclose(mDebugStream);
		mDebugStream = fopen(filename.c_str(), "a");

		filename = getTodayLogFileName(QUERYFILETAIL);
		fclose(mQueryStream);
		mQueryStream = fopen(filename.c_str(), "a");

		filename = getTodayLogFileName(ERRORFILETAIL);
		fclose(mErrorStream);
		mErrorStream = fopen(filename.c_str(), "a");

		filename = getTodayLogFileName(STATFILETAIL);
		fclose(mStatStream);
		mStatStream = fopen(filename.c_str(), "a");

		mToday = getToday();
	}
	pthread_mutex_unlock(&mMutex);

	return;
}
