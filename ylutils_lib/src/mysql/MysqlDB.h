#ifndef _MYSQLDB_H_
#define _MYSQLDB_H_

#include <string>
#include <vector>

#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include "conf/Config.h"

using namespace std;

//!< mysql�쳣��MysqlDBException.
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
	 * init֮��connect֮ǰ���õ����Ӳ���, �������ֵ����mysql C API : mysql_options�е�˵��.
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
	 * @brief    ִ��sql���. 
	 *
	 * @param    sql    �ṹ����ѯ����.
	 * @param    len    ��ѯ�䳤��.
	 */
	void execute(const char *sql, size_t len) throw(MysqlDBException);
	
	void execute(const string &sql) throw(MysqlDBException);

	/**
	 * @brief    �ӷ������˰Ѳ�ѯ���ȡ��.
	 *
	 * @return   �����׳��쳣.
	 */
	void getResult() throw(MysqlDBException);

	/**
	 * @brief    �����ϴ�UPDATE���ĵ��������ϴ�DELETEɾ�������������ϴ�INSERT�������������
	 * ����UPDATE��DELETE��INSERT��䣬����mysql_query()�����̵��á�����SELECT��䣬
	 * mysql_affected_rows()�Ĺ�����ʽ��mysql_num_rows()���ơ�
	 *
	 * @return    ����0������������Ӱ����������������0����ʾUPDATE���δ���¼�¼��
	 * �ڲ�ѯ��û����WHEREƥ����У���δִ�в�ѯ����-1����ʾ��ѯ���ش��󣬻��ߣ�
	 * ����SELECT��ѯ���ڵ���mysql_store_result()֮ǰ������mysql_affected_rows()��
	 * ����mysql_affected_rows()�����޷���ֵ��ͨ���ȽϷ���ֵ�͡�(my_ulonglong)-1��
	 * ���Ч�ġ�(my_ulonglong)~0��������Ƿ�Ϊ��-1����
	 */
	long getAffectedRows();

	/**
	 * @brief    ��ȡ���������.
	 */
	my_ulonglong getRowsNum();

	/**
	 * @brief    getResult֮����Ե��øú������������.
	 *
	 * @return   NULL��ʾ����β.
	 */
	MYSQL_ROW nextRow();

	//! ��ȡ���һ�β�ѯ���ֶ���.
	u_int getFieldsNum();

	//! ��ȡ���һ�β�ѯ��pos�ֶε�����.
	const char *getFieldNameAt(int pos);
	//! ��ȡ���һ�β�ѯ��pos�ֶε�����.
	enum enum_field_types getFieldTypeAt(int pos);

	//! ��ȡ��ǰ����еĵ�pos�ֶε�ֵ.
	const char *getFieldValueAt(int pos);
	//! ��ȡ��ǰ����е�fname���ֵ.
	const char *getFieldValueByName(const char *fname);

	//! ��ȡ��ǰ�����pos�ֶ�ֵ�ĳ���.
	u_long getFieldValueLenAt(int pos);
	//! ��ȡ��ǰ���fname��ֵ�ĳ���.
	u_long getFieldValueLenByName(const char *fname);


	//!< ��ȡ��ǰ���ݿ�����еı���.
	void getTableNames(vector<string> &tables, const char *wild = NULL) throw(MysqlDBException);
	//!< ��ȡ��ǰ���ݿ�����еı���.
	void getTableFields(vector<string> &fields, const char *table, const char *wild) throw(MysqlDBException);

public :
	//! ��ת���ַ�
	static string escape(const string &field_value);
	//! ��ת���ַ�
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
	MYSQL_ROW	mMysqlCursorRow;	//!< ��ǰ��.
	MYSQL_FIELD	*mMysqlField;		//!< sql's field.
	u_long		*mCursorFieldValueLen;	//!< �α����ֶ�ֵ�ĳ���.

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
