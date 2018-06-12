#include "MysqlDB.h"

MysqlDB::MysqlDB()
	:
	mMysql(NULL),
	mMysqlResult(NULL),
	mHost(""),
	mPort(0),
	mUser(""),
	mPasswd(""),
	mDb(""),
	mUnixSocket(""),
	mClientFlag(0)
{
	mMysql = mysql_init(NULL);
}

MysqlDB::~MysqlDB()
{
	if (mMysqlResult != NULL)
		mysql_free_result(mMysqlResult);

	if (mMysql != NULL)
		mysql_close(mMysql);
}

void MysqlDB::setOptions(enum mysql_option option, const char *arg) throw(MysqlDBException)
{
	if (mysql_options(mMysql, option, arg) != 0)
	{
		throw MysqlDBException(string("MysqlDB::setOptions() : you used an unknown option"));
	}

	return;
}

void MysqlDB::connect(const string &conf) throw(MysqlDBException)
{
	Config __config;
	try {
		__config.load(conf);
	} catch (IOException &E) {
		ostringstream oss;
		oss << "MysqlDB::connect(" << conf << ") :" << E.what();
		throw MysqlDBException(oss.str());
	}

	const char *pvalue = __config.getValue("my_host");
	mHost = (pvalue==NULL)?"localhost":pvalue;

	mUser = ((pvalue=__config.getValue("my_user"))==NULL)?"":pvalue;
	mPasswd = ((pvalue=__config.getValue("my_passwd"))==NULL)?"":pvalue;
	mDb = ((pvalue=__config.getValue("my_db"))==NULL)?"":pvalue;
	mPort = ((pvalue=__config.getValue("my_port"))==NULL)?0:atoi(pvalue);
	mUnixSocket = ((pvalue=__config.getValue("my_unix_socket"))==NULL)?"":pvalue;
	mClientFlag = ((pvalue=__config.getValue("my_client_flag"))==NULL)?0:atoi(pvalue);
	mMyCnf = ((pvalue=__config.getValue("my_cnf"))==NULL)?"":pvalue;

	if (!mMyCnf.empty())
		setOptions(MYSQL_READ_DEFAULT_FILE, mMyCnf.c_str());

	//! 连接服务器.
	if (mysql_real_connect(mMysql, mHost.c_str(), mUser.c_str(), mPasswd.c_str(), mDb.c_str(),
				mPort, mUnixSocket.empty()?NULL:mUnixSocket.c_str(), mClientFlag)
			== NULL)
	{
		ostringstream oss;
		oss << "MysqlDB::connect() : mysql_real_connect() : " << mysql_error(mMysql);
		throw MysqlDBException(oss.str());
	}

	//! OK.
	return;
}

void MysqlDB::connect(const char* db_ip, int db_port, const char* db_user, const char* db_passwd, const char* db_name) throw(MysqlDBException)
{
	mHost = db_ip;
	mPort = db_port;
	mUser = db_user;
	mPasswd = db_passwd;
	mDb = db_name;

	//! 连接服务器.
	char value = 1; 
	unsigned int  readTimeOut=1;
	unsigned int  writeTimeOut=1;	
       mysql_options(mMysql, MYSQL_OPT_RECONNECT, (char*)&value); 
       mysql_options(mMysql, MYSQL_OPT_READ_TIMEOUT, (char*)&readTimeOut); 
       mysql_options(mMysql, MYSQL_OPT_WRITE_TIMEOUT, (char*)&writeTimeOut);
	   
	if (mysql_real_connect(mMysql, mHost.c_str(), mUser.c_str(), mPasswd.c_str(), mDb.c_str(),
				mPort, mUnixSocket.empty()?NULL:mUnixSocket.c_str(), mClientFlag)
			== NULL)
	{
		ostringstream oss;
		oss << "MysqlDB::connect() : mysql_real_connect() : " << mysql_error(mMysql);
		throw MysqlDBException(oss.str());
	}
	
}

string MysqlDB::getCharSet()
{
	return mysql_character_set_name(mMysql);
}

void MysqlDB::setCharSet(string charsetname)
{
	mysql_set_character_set(mMysql, charsetname.c_str());
}


bool MysqlDB::good()
{
	return mysql_errno(mMysql)==0;
}

u_int MysqlDB::lastErrNo()
{
	return mysql_errno(mMysql);
}

bool MysqlDB::lostConnection()
{
	return mysql_errno(mMysql)==CR_SERVER_LOST;
}

bool MysqlDB::hasGoneAway()
{
	return mysql_errno(mMysql)==CR_SERVER_GONE_ERROR;
}

void MysqlDB::reConnect() throw(MysqlDBException)
{
	if (this->good())
		return;

	this->close();

	mMysql=mysql_init(NULL);

	if (!mMyCnf.empty())
		setOptions(MYSQL_READ_DEFAULT_FILE, mMyCnf.c_str());
	
	char value = 1; 
	unsigned int  readTimeOut=1;
	unsigned int  writeTimeOut=1;	
       mysql_options(mMysql, MYSQL_OPT_RECONNECT, (char*)&value); 
       mysql_options(mMysql, MYSQL_OPT_READ_TIMEOUT, (char*)&readTimeOut); 
       mysql_options(mMysql, MYSQL_OPT_WRITE_TIMEOUT, (char*)&writeTimeOut); 

	//! 连接服务器.
	if (mysql_real_connect(mMysql, mHost.c_str(), mUser.c_str(), mPasswd.c_str(), mDb.c_str(),
				mPort, mUnixSocket.empty()?NULL:mUnixSocket.c_str(), mClientFlag)
			== NULL)
	{
		ostringstream oss;
		oss << "MysqlDB::reConnect() : mysql_real_connect() : " << mysql_error(mMysql);
		throw MysqlDBException(oss.str());
	}

	return;
}

void MysqlDB::close()
{
	if (mMysql != NULL)
	{
		mysql_close(mMysql);
		mMysql = NULL;
	}

	return;
}

void MysqlDB::execute(const char *sql, size_t len) throw(MysqlDBException)
{
	if (mysql_real_query(mMysql, sql, len) != 0)
	{
		ostringstream oss;
		oss << "MysqlDB::execute(" << sql << ", " << len << ") mysql_real_query(,) : " << mysql_error(mMysql);
		throw MysqlDBException(oss.str());
	}
	return;
}

void MysqlDB::execute(const string &sql) throw(MysqlDBException)
{
	return execute(sql.c_str(), sql.length());
}

void MysqlDB::getResult() throw(MysqlDBException)
{
	//! 释放本地结果集空间.
	if (mMysqlResult != NULL)
	{
		mysql_free_result(mMysqlResult);
		mMysqlResult = NULL;
		mMysqlField = NULL;
	}

	//! 从服务器端取回结果.
	if ((mMysqlResult = mysql_store_result(mMysql)) == NULL)
	{
		ostringstream oss;
		oss << "MysqlDB::getResult() : mysql_store_result(mysql) : " << mysql_error(mMysql);
		throw MysqlDBException(oss.str());
	}

	//! 给字段指针赋值.
	mMysqlField = mMysqlResult->fields;

	//! 对字段map进行赋值.
	mFieldNamePos.clear();
	for (u_int i = 0; i < getFieldsNum(); i++)
	{
		mFieldNamePos.insert(make_pair(getFieldNameAt(i), i));
	}	

	return;
}

long MysqlDB::getAffectedRows()
{	
	return mysql_affected_rows(mMysql);
}

my_ulonglong MysqlDB::getRowsNum()
{
	if (mMysqlResult == NULL)
		return 0;

	return mysql_num_rows(mMysqlResult);
}

MYSQL_ROW MysqlDB::nextRow()
{
	if (mMysqlResult == NULL)
		return NULL;
	//! 取出下一行.
	mMysqlCursorRow = mysql_fetch_row(mMysqlResult);

	//! 获取字段值的长度序列.
	if (mMysqlCursorRow != NULL)
		mCursorFieldValueLen = mysql_fetch_lengths(mMysqlResult);

	return mMysqlCursorRow;
}

u_int MysqlDB::getFieldsNum()
{
	if (mMysqlResult == NULL)
		return 0;
	//mMysqlField = mysql_fetch_fields(mMysqlResult);

	return mysql_num_fields(mMysqlResult);
}

const char *MysqlDB::getFieldNameAt(int pos)
{
	return mMysqlField[pos].name;
}

enum enum_field_types MysqlDB::getFieldTypeAt(int pos)
{
	return mMysqlField[pos].type;
}

const char *MysqlDB::getFieldValueAt(int pos)
{
	return mMysqlCursorRow[pos];
}

u_long MysqlDB::getFieldValueLenAt(int pos)
{
	return mCursorFieldValueLen[pos];
}

void MysqlDB::getTableNames(vector<string> &tables, const char *wild) throw(MysqlDBException)
{
	MYSQL_RES *__mysql_res = mysql_list_tables(mMysql, NULL);
	if (__mysql_res == NULL)
	{
		ostringstream omsg;
		omsg << "MysqlDB::getTableNames() : mysql_list_tables(MYSQL, NULL) : " << mysql_error(mMysql);
		throw MysqlDBException(omsg.str());
	}

	MYSQL_ROW __cursor;
	while ((__cursor = mysql_fetch_row(__mysql_res)) != NULL)
	{
		tables.push_back(__cursor[0]);
	}
	mysql_free_result(__mysql_res);

	return;
}

void MysqlDB::getTableFields(vector<string> &fields, const char *table, const char *wild) throw(MysqlDBException)
{
	MYSQL_RES *__mysql_res = mysql_list_fields(mMysql, table, wild);
	if (__mysql_res == NULL)
	{
		ostringstream omsg;
		omsg << "MysqlDB::getTableFields() : mysql_list_fields(MYSQL," << table << "," << (wild==NULL?"NULL":wild) << mysql_error(mMysql);
		throw MysqlDBException(omsg.str());
	}

	MYSQL_ROW __cursor;
	while ((__cursor = mysql_fetch_row(__mysql_res)) != NULL)
	{
		fields.push_back(__cursor[0]);
	}
	mysql_free_result(__mysql_res);

	return;
}

const char *MysqlDB::getFieldValueByName(const char *fname)
{
	map<const char *, int, strcaselt>::iterator it = mFieldNamePos.find(fname);
	if (it == mFieldNamePos.end())
		return NULL;
	return mMysqlCursorRow[it->second];
}

u_long MysqlDB::getFieldValueLenByName(const char *fname)
{
	map<const char *, int, strcaselt>::iterator it = mFieldNamePos.find(fname);
	if (it == mFieldNamePos.end())
		return 0;
	return mCursorFieldValueLen[it->second];
}


string MysqlDB::escape(const string &field_value)
{
	return escape(field_value.c_str());
}

string MysqlDB::escape(const char *field_value)
{
	string ret;
	while (*field_value != '\0')
	{
		if (*field_value == '\'')
			ret += "\\";
		ret += *field_value;
		field_value++;
	}
	return ret;
}

string MysqlDB::real_escape(const char *sql, int len)
{
	string _escape;
	_escape.resize(2*len+1);

	_escape.resize(mysql_real_escape_string(mMysql, (char *)&_escape[0], sql, len));
	return _escape;
}


#ifdef _TEST

#include <unistd.h>
#include <iterator>
#include <iostream>

int main(int argc, char *argv[])
{
	try {
		MysqlDB mysqldb;
		MYSQL_ROW MyRow;

		mysqldb.connect("localhost", 3306, "root", "", "file_dispatch");

		string sql = "select pfid, pfname from pfid_info limit 10";
		mysqldb.execute(sql);
		mysqldb.getResult();
		while((MyRow = mysqldb.nextRow())) {
			int pfid = atoi(MyRow[0]);
			string pfname = MyRow[1];

			cout<<pfid<<"\t"<<pfname<<endl;
		}

		sql = "select id, video_path from file_ownload limit 10";
		mysqldb.execute(sql);
		mysqldb.getResult();
		while((MyRow = mysqldb.nextRow())) {
			int id = atoi(MyRow[0]);
			string path = MyRow[1];

			cout<<id<<"\t"<<path<<endl;
		}

	} catch (exception &E) {
		cout << E.what() << endl;
		return -1;
	}
	return 0;
}
#endif //_TEST
