#ifndef _MYSQLDB_H_
#define _MYSQLDB_H_

#include <string>
#include <vector>

#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include "conf/Config.h"

using namespace std;

//!< mysql异常类MysqlDBException.
class MysqlDBException : public std::exception
{
public :
	MysqlDBException(const string &msg) throw(): mMsg(msg) {}

	virtual ~MysqlDBException() throw() {}

	virtual const char *what() const throw() { return mMsg.c_str(); }

private :
	string mMsg;
};

class MysqlDB
{
public :
	//! Constructor.
	MysqlDB();

	//! Destructor.
	virtual ~MysqlDB();

	/**
	 * init之后connect之前设置的连接参数, 具体参数值参照mysql C API : mysql_options中的说明.
	 */
	void setOptions(enum mysql_option option, const char *arg) throw(MysqlDBException);

	//! Connect mysqld.
	void connect(const string &conf) throw(MysqlDBException);
	void connect(const char* db_ip, int db_port, const char* db_user, const char* db_passwd, const char* db_name) throw(MysqlDBException);

	string getCharSet();
	void   setCharSet(string charsetname);
		
	//! normal?
	bool good();

	//! last errno.
	u_int lastErrNo();

	//! connected?
	bool lostConnection();

	//! close the connection.
	void close();

	//! mysql server has gone away?
	bool hasGoneAway();

	//! reconnect.
	void reConnect() throw(MysqlDBException);

	/**
	 * @brief    执行sql语句. 
	 *
	 * @param    sql    结构化查询句子.
	 * @param    len    查询句长度.
	 */
	void execute(const char *sql, size_t len) throw(MysqlDBException);
	
	void execute(const string &sql) throw(MysqlDBException);

	/**
	 * @brief    从服务器端把查询结果取回.
	 *
	 * @return   出错抛出异常.
	 */
	void getResult() throw(MysqlDBException);

	/**
	 * @brief    返回上次UPDATE更改的行数，上次DELETE删除的行数，或上次INSERT语句插入的行数。
	 * 对于UPDATE、DELETE或INSERT语句，可在mysql_query()后立刻调用。对于SELECT语句，
	 * mysql_affected_rows()的工作方式与mysql_num_rows()类似。
	 *
	 * @return    大于0的整数表明受影响或检索的行数。“0”表示UPDATE语句未更新记录，
	 * 在查询中没有与WHERE匹配的行，或未执行查询。“-1”表示查询返回错误，或者，
	 * 对于SELECT查询，在调用mysql_store_result()之前调用了mysql_affected_rows()。
	 * 由于mysql_affected_rows()返回无符号值，通过比较返回值和“(my_ulonglong)-1”
	 * 或等效的“(my_ulonglong)~0”，检查是否为“-1”。
	 */
	long getAffectedRows();

	/**
	 * @brief    获取结果的行数.
	 */
	my_ulonglong getRowsNum();

	/**
	 * @brief    getResult之后可以调用该函数遍历结果集.
	 *
	 * @return   NULL表示到行尾.
	 */
	MYSQL_ROW nextRow();

	//! 获取最近一次查询的字段数.
	u_int getFieldsNum();

	//! 获取最近一次查询第pos字段的名字.
	const char *getFieldNameAt(int pos);
	//! 获取最近一次查询第pos字段的类型.
	enum enum_field_types getFieldTypeAt(int pos);

	//! 获取当前结果行的第pos字段的值.
	const char *getFieldValueAt(int pos);
	//! 获取当前结果行的fname域的值.
	const char *getFieldValueByName(const char *fname);

	//! 获取当前结果第pos字段值的长度.
	u_long getFieldValueLenAt(int pos);
	//! 获取当前结果fname域值的长度.
	u_long getFieldValueLenByName(const char *fname);


	//!< 获取当前数据库的所有的表名.
	void getTableNames(vector<string> &tables, const char *wild = NULL) throw(MysqlDBException);
	//!< 获取当前数据库的所有的表名.
	void getTableFields(vector<string> &fields, const char *table, const char *wild) throw(MysqlDBException);

public :
	//! 加转义字符
	static string escape(const string &field_value);
	//! 加转义字符
	static string escape(const char *field_value);
	string real_escape(const char *sql, int len);

protected :

private :
	struct strcaselt
	{
		bool
		operator()(const char *left, const char *right)
		{
			return strcasecmp(left, right) < 0;
		}
	};


private :
	MYSQL		*mMysql;		//!< mysql connecting.
	MYSQL_RES	*mMysqlResult;		//!< sql's result.
	MYSQL_ROW	mMysqlCursorRow;	//!< 当前行.
	MYSQL_FIELD	*mMysqlField;		//!< sql's field.
	u_long		*mCursorFieldValueLen;	//!< 游标行字段值的长度.

	string		mHost;
	u_int		mPort;
	string		mUser;
	string		mPasswd;
	string		mDb;
	string		mUnixSocket;
	u_long		mClientFlag;
	string		mMyCnf;

	map<const char *, int, strcaselt>	mFieldNamePos;
};
#endif 
