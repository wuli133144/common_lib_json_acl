// 
//	log library, by shenjian, last modified at 2010-10-26
//

#include "yl_log.h"
#include "../time/Time.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <syslog.h>

#ifdef _THREAD_SAFE_LOGLIB_
#include <pthread.h>
#endif

#define LOG_PATH_LEN 128
#define LOG_FORMAT_SIZE 1024

#define ROTATE_CNT 6
#define ROTATE_FSIZE 64000000		// about 64MB
#define ROTATE_INTERVAL 86400		// 1 day

static FILE* logfp = NULL;
static char log_dir[LOG_PATH_LEN];
static char log_filename[LOG_PATH_LEN];
static int log_type = 0;
static int log_rotate_count = 0;
static int log_rotate_type = 0;
static time_t today;

#ifdef _THREAD_SAFE_LOGLIB_
static pthread_mutex_t log_write_lock;
#endif

static time_t get_today_begining_time()
{
        struct tm ta;
        time_t a = time(0);
        localtime_r(&a, &ta);
	ta.tm_hour = 0;
	ta.tm_min = 0;
	ta.tm_sec = 0;
	return mktime(&ta);
}

static void rotate_log()
{
	int need_rotate = 0;
	if(!log_rotate_type) {
		// by filesize
		if(ftell(logfp) > ROTATE_FSIZE)
			need_rotate = 1;
	}else {
		// by day
		time_t now = time(0);
		if(now - today >= ROTATE_INTERVAL) {
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

int log_start(int type, const char* dir, const char* filename, int rotate_count, int rotate_type)
{
	log_type = type;

	if(log_type) {
		char fpath[LOG_PATH_LEN];
		snprintf(fpath, LOG_PATH_LEN, "%s/%s", dir, filename);
		logfp = fopen(fpath, "a");
		if(logfp == NULL) {
			perror("log_start");
			return -1;
		}

		snprintf(log_dir, LOG_PATH_LEN, "%s", dir);
		snprintf(log_filename, LOG_PATH_LEN, "%s", filename); 
		log_rotate_count = rotate_count;
		log_rotate_type = rotate_type;
		today = get_today_begining_time();

		if(log_rotate_count <= 0)
			log_rotate_count = ROTATE_CNT;
	}

#ifdef _THREAD_SAFE_LOGLIB_
        if(pthread_mutex_init(&log_write_lock, NULL) != 0) {
		perror("pthread_mutex_init in log_start");
                return -1;
        }
#endif

	return 0;
}

void log_end()
{
	if(log_type)
		fclose(logfp);
}

void log_err(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	char time[32];
	Time::getCurrTimeStr(time);

	char buf[LOG_FORMAT_SIZE];
	snprintf(buf, LOG_FORMAT_SIZE, "%s [ERROR][%lu]: %s\n", time, pthread_self(), message); 

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

void log_warning(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	char time[32];
	Time::getCurrTimeStr(time);

	char buf[LOG_FORMAT_SIZE];
	snprintf(buf, LOG_FORMAT_SIZE, "%s [WARN][%lu]: %s\n", time, pthread_self(), message); 

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

void log_info(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	char time[32];
	Time::getCurrTimeStr(time);

	char buf[LOG_FORMAT_SIZE];
	snprintf(buf, LOG_FORMAT_SIZE, "%s [INFO][%lu]: %s\n", time, pthread_self(), message); 

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

void log_debug(const char* message, ...)
{
	va_list ap;
	va_start(ap, message);

	char time[32];
	Time::getCurrTimeStr(time);

	char buf[LOG_FORMAT_SIZE];
	snprintf(buf, LOG_FORMAT_SIZE, "%s [DEBUG][%lu]: %s\n", time, pthread_self(), message); 

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

void log_perror(const char* message)
{
	char time[32];
	Time::getCurrTimeStr(time);

#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_lock(&log_write_lock);
#endif
	if(log_type) {
		int errsv = errno;
		rotate_log();
		fprintf(logfp, "%s [ERROR][%lu]: %s: error code (%d)\n", time, pthread_self(), message, errsv); 
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
	if(log_type)
                log_start(1, log_dir.c_str(), log_filename.c_str(), -1, 0);
        else   
                log_start(0, "", "", 0, 0);

	log_info("info %u", time(0));
	log_debug("debug %u", time(0));

	log_end();	
}

#endif

