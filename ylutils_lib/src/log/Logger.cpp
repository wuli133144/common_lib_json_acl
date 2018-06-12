// 
//	log library, by shenjian, last modified at 2010-12-02
//

#include "Logger.h"
#include "../time/Time.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <syslog.h>

#define LOG_FORMAT_SIZE 1024

#define DEFAULT_ROTATE_CNT 6
#define DEFAULT_ROTATE_FSIZE 64000000		// about 64MB
#define DEFAULT_ROTATE_INTERVAL 86400		// 1 day

Logger::Logger():
	logfp(NULL),
	log_type(0),
	log_rotate_type(0),
	log_rotate_count(DEFAULT_ROTATE_CNT),
	log_rotate_fsize(DEFAULT_ROTATE_FSIZE),
	log_rotate_interval(DEFAULT_ROTATE_INTERVAL)
{
#ifdef _THREAD_SAFE_LOGLIB_
        pthread_mutex_init(&log_write_lock, NULL);
#endif
	today = get_today_begining_time();
}

Logger::~Logger()
{
	if(log_type && logfp)
		fclose(logfp);
#ifdef _THREAD_SAFE_LOGLIB_
        pthread_mutex_destroy(&log_write_lock);
#endif
}

time_t Logger::get_today_begining_time()
{
        struct tm ta;
        time_t a = time(0);
        localtime_r(&a, &ta);
	ta.tm_hour = 0;
	ta.tm_min = 0;
	ta.tm_sec = 0;
	return mktime(&ta);
}

void Logger::rotate_log()
{
	int need_rotate = 0;
	if(!log_rotate_type) {
		// by filesize
		if(ftell(logfp) > log_rotate_fsize)
			need_rotate = 1;
	}else {
		// by day
		time_t now = time(0);
		if(now - today >= log_rotate_interval) {
			need_rotate = 1;
			today = get_today_begining_time();
		}
	}

	if(need_rotate) {
		fclose(logfp);
		char from[LOG_PATH_LEN];
		char to[LOG_PATH_LEN];

		// remove oldest
		snprintf(from, LOG_PATH_LEN, "%s/%s.%d", log_dir, log_filename, log_rotate_count-1);
		if(access(from, F_OK) == 0)
			remove(from);

		// move median
		for(int i=log_rotate_count-2; i>=0; i--) {
			if(i>0) 
				snprintf(from, LOG_PATH_LEN, "%s/%s.%d", log_dir, log_filename, i);
			else
				snprintf(from, LOG_PATH_LEN, "%s/%s", log_dir, log_filename);

			snprintf(to, LOG_PATH_LEN, "%s/%s.%d", log_dir, log_filename, i+1);
			if(access(from, F_OK) == 0)
				rename(from, to);
		}

		// open newest
		snprintf(to, LOG_PATH_LEN, "%s/%s", log_dir, log_filename);
		
		logfp = fopen(to, "a");
		if(logfp == NULL) {
			perror(to);
			return;
		}
	}
}

void  Logger::set_rotate_count(int count)
{
	log_rotate_count = count;
}

void  Logger::set_rotate_filesize(int filesize)
{
	log_rotate_fsize = filesize;
}

bool Logger::init(int type, const char* dir, const char* filename, int rotate_type)
{
	log_type = type;

	if(log_type) {
		char fpath[LOG_PATH_LEN];
		snprintf(fpath, LOG_PATH_LEN, "%s/%s", dir, filename);
		logfp = fopen(fpath, "a");
		if(logfp == NULL) {
			perror("log_start");
			return false;
		}

		snprintf(log_dir, LOG_PATH_LEN, "%s", dir);
		snprintf(log_filename, LOG_PATH_LEN, "%s", filename); 
		log_rotate_type = rotate_type;
	}

	return true;
}

void Logger::log_err(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	char time[32];
	Time::getCurrTimeStr(time);

	char buf[LOG_FORMAT_SIZE];
	snprintf(buf, LOG_FORMAT_SIZE, "%s [ERROR]: %s\n", time, message); 

#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_lock(&log_write_lock);
#endif

	if(log_type) {
		rotate_log();
		vfprintf(logfp, buf, ap);
		fflush(logfp);
	}else
		vfprintf(stderr, buf, ap);
#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_unlock(&log_write_lock);
#endif

	va_end(ap);	
}

void Logger::log_warning(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	char time[32];
	Time::getCurrTimeStr(time);

	char buf[LOG_FORMAT_SIZE];
	snprintf(buf, LOG_FORMAT_SIZE, "%s [WARNING]: %s\n", time, message); 

#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_lock(&log_write_lock);
#endif
	if(log_type) {
		rotate_log();
		vfprintf(logfp, buf, ap);
		fflush(logfp);
	}else
		vfprintf(stderr, buf, ap);
#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_unlock(&log_write_lock);
#endif

	va_end(ap);	
}

void Logger::log_info(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	char time[32];
	Time::getCurrTimeStr(time);

	char buf[LOG_FORMAT_SIZE];
	snprintf(buf, LOG_FORMAT_SIZE, "%s [INFO]: %s\n", time, message); 

#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_lock(&log_write_lock);
#endif
	if(log_type) {
		rotate_log();
		vfprintf(logfp, buf, ap);
		fflush(logfp);
	}else
		vfprintf(stdout, buf, ap);
#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_unlock(&log_write_lock);
#endif

	va_end(ap);	
}

void Logger::log_debug(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	char time[32];
	Time::getCurrTimeStr(time);

	char buf[LOG_FORMAT_SIZE];
	snprintf(buf, LOG_FORMAT_SIZE, "%s [DEBUG]: %s\n", time, message); 

#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_lock(&log_write_lock);
#endif
	if(log_type) {
		rotate_log();
		vfprintf(logfp, buf, ap);
		fflush(logfp);
	}else
		vfprintf(stdout, buf, ap);
#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_unlock(&log_write_lock);
#endif

	va_end(ap);	
}

void Logger::log_perror(const char* message)
{
	char time[32];
	Time::getCurrTimeStr(time);

#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_lock(&log_write_lock);
#endif
	if(log_type) {
		int errsv = errno;
		rotate_log();
		fprintf(logfp, "%s [ERROR]: %s: error code (%d)\n", time, message, errsv); 
		fflush(logfp);
	}else
		perror(message); 
#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_unlock(&log_write_lock);
#endif
}

#ifdef _TEST

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int log_type = atoi(argv[1]);
	string log_dir = "log";
	string log_filename = "test.log";

	Logger g;
	g.init(log_type, log_dir.c_str(), log_filename.c_str(), 0);

	g.log_info("info %u", time(0));
	g.log_debug("debug %u", time(0));
}

#endif

