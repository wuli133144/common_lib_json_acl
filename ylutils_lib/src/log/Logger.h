// 
//	log library, by shenjian, last modified at 2010-12-02
//	It is thread-not-safe by default. If you want a thread-safe library, you should define _THREAD_SAFE_LOGLIB_
//

#ifndef _LOGGER_H_
#define _LOGGER_H_

#ifdef _THREAD_SAFE_LOGLIB_
#include <pthread.h>
#endif
#include <stdio.h>
#include <time.h>

#define LOG_PATH_LEN 256

class Logger
{
public:
	Logger();
	~Logger();

	// initialize up log functions.
	// [args]
	//  type: 1 - log, 0 - print to console
	//  log_dir: log directory
	//  log_filename: log file name
	//  rotate_count: count of log backup files, if less than 0, using default value
	//  rotate_type: 1 - by day, 0 - by filesize, every 64M	
	bool init(int type, const char* log_dir, const char* log_filename, int rotate_type);
	void set_rotate_count(int count);
	void set_rotate_filesize(int filesize);

	// kinds of log functions.
	void log_err(const char* message, ...);
	void log_warning(const char* message, ...);
	void log_info(const char* message, ...);
	void log_debug(const char* message, ...);
	void log_perror(const char* message);

private:
	char log_dir[LOG_PATH_LEN];
	char log_filename[LOG_PATH_LEN];
	FILE* logfp;
	int log_type;

	int log_rotate_type;
	int log_rotate_count;
	int log_rotate_fsize;
	int log_rotate_interval;
	time_t today;

#ifdef _THREAD_SAFE_LOGLIB_
	pthread_mutex_t log_write_lock;
#endif

	time_t get_today_begining_time();
	void rotate_log();
};

#endif
