// 
//	log library, by shenjian, last modified at 2010-10-26
//	
//	It is thread-not-safe by default. If you want a thread-safe library, you should define _THREAD_SAFE_LOGLIB_
//

#ifndef _YL_LOG_H_
#define _YL_LOG_H_

/* 
 * start up log functions.
 * [args]
 *  type: 1 - log, 0 - print to console
 *  log_dir: log directory
 *  log_filename: log file name
 *  rotate_count: count of log backup files, if less than 0, using default value
 *  rotate_type: 1 - by day, 0 - by filesize, every 64M
 */
int log_start(int type, const char* log_dir, const char* log_filename, int rotate_count, int rotate_type);

/* 
 * stop log functions.
 */
void log_end();

/* 
 * kinds of log functions.
 */
void log_err(const char* message, ...);
void log_warning(const char* message, ...);
void log_info(const char* message, ...);
void log_debug(const char* message, ...);
void log_perror(const char* message);

#endif
