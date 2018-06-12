/*
 * =====================================================================================
 * 
 *        Filename:  dict.h
 * 
 *     Description:  通过hash_map实现的词典.
 * 
 *         Version:  1.0
 *         Created:  2006年05月25日 17时29分35秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef DICT_H_ZMH_2006_05_25
#define DICT_H_ZMH_2006_05_25

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <ext/hash_map>
#include <errno.h>

//! sign fun.
#include "../sign/sign.h"

using namespace std;
using namespace __gnu_cxx;
/** @defgroup dict
 *  字典类
 *  @{
 */

//! T_Sign的哈希函数.
namespace __gnu_cxx
{
	template<> struct hash<T_Sign>
	{
		size_t operator()(const T_Sign& sign) const
		{
			return sign.sign1 + sign.sign2;
			//return sign.sign1;
		}
	};
}

//! T_Sign的比较函数, 也是告诉hash_map在哈希冲突时如何如何判断item是否相等.
struct eqSign
{
	bool operator()(const T_Sign &sign1, const T_Sign &sign2) const
	{
		return (sign1.sign1 == sign2.sign1) && (sign1.sign2 == sign2.sign2);
	}
};

//! 词信息.
typedef struct
{
	int   termID;		//!< 词的代码, 是同过顺序增加的整型来生成的.
	u_int termFreq;		//!< 词频.
	u_int docFreq;		//!< 词的文档频率.
	u_int termWeight;	//!< 词权重.
} T_TermInfo;

//! 载入词典和存词典到磁盘出错抛出DictException
class DictException : public std::exception
{
public :
	DictException(const string &msg) throw() : mMsg(msg) {}

	virtual ~DictException() throw() {}

	virtual const char *what() const throw() { return mMsg.c_str(); }

private :
	string mMsg;
};

//! 在序列化和载入词典到内存中时候可以方便的进行块操作.
typedef struct
{
	T_Sign       sign;
	T_TermInfo   termInfo;
} T_TermPair;

/*! \brief 通过hash_map实现的词典
 *! \class Dict dict.h dict/dict.h    
 *  
 *   \version 1.0   
 \code
 
#include "../time/Time.h"

static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " -[l|b|t]" << endl;
	cout << "测试词典:" << endl << endl;

	cout << " -l :			载入词典到内存,并提供查询." << endl;
	cout << " -b :			根据切分后的词创建词典." << endl;
	cout << " -t :			测试词典的查询性能." << endl;
	return;
}

int donothing(const char *a)
{
	return 0;
}

const string dict_image = "dict.img";
const string word_src = "webpage.dict.uniq";
//const string word_src = "termfile.txt";

int main(int argc, char *argv[])
{

	if ((argc != 2) || ((strcmp(argv[1], "-l") != 0) && (strcmp(argv[1], "-b") != 0) && (strcmp(argv[1], "-t") != 0)))
	{
		dohelp(argc, argv);
		return 1;
	}

	long begin, end;

	Dict dict;

	try {
		if (strcmp(argv[1], "-l") == 0)
		{
			cout << "begin load dict into memory ......" << endl;
			begin = Time::getCurrentMillis();

			dict.load(dict_image);
			
			const T_TermInfo *termi = NULL;
			dict.rewind();
			while ((termi = dict.next()) != NULL)
			{
				cout << termi->termID << endl;
			}

			end = Time::getCurrentMillis();
			cout << end-begin << "毫秒!!" << endl;

			string word;
			cout << "please input your word : ";

			while (cin >> word)
			{
				cout << word << ":" << dict.getTermID(word) << endl;
				cout << "please input your word : ";
			}
			cout << endl;
		}
		else if (strcmp(argv[1], "-b") == 0)
		{ //! 建立词典
			ifstream fwords(word_src.c_str());
			if (!fwords)
			{
				cout << "open words file " << word_src << " error : " << strerror(errno) << endl;
				return 1;
			}

			string word;

			cout << "begin build dict in memory ......" << endl;
			begin = Time::getCurrentMillis();
			while (fwords >> word)
				dict.insertTerm(word);
			end = Time::getCurrentMillis();
			cout << end-begin << "毫秒!!" << endl;

			fwords.close();

			cout << "begin save dict into disk ......" << endl;
			begin = Time::getCurrentMillis();
			dict.save(dict_image);
			end = Time::getCurrentMillis();
			cout << end-begin << "毫秒!!" << endl;

			cout << "please input your word : ";

			while (cin >> word)
			{
				cout << word << ":" << dict.getTermID(word) << endl;
				cout << "please input your word : ";
			}
			cout << endl;
		}
		else
		{ //! 测试词典查询性能.
			cout << "begin load dict into memory ......" << endl;
			begin = Time::getCurrentMillis();
			dict.load(dict_image);
			end = Time::getCurrentMillis();
			cout << end-begin << "毫秒!!" << endl;

			ifstream fwords(word_src.c_str());
			if (!fwords)
			{
				cout << "open words file " << word_src << " error : " << strerror(errno) << endl;
				return 1;
			}

			vector<string> v(600000);

			
			int i = 0;
			while (fwords >> v[i++]);
			fwords.close();

			cout << "词典中有" << dict.size() << "个词" << endl;
			cout << "测试用例" << i << "个词" << endl;

			cout << "begin test ......" << endl;
			begin = Time::getCurrentMillis();

			for (int k = 0; k< 200; k++)
			{
				for (int j = 0; j < i; j++)
				{
					
					//int a = 0;
					//a++;
					//donothing(v[j].c_str());
					dict.getTermID(v[j].c_str());
				}
			}
			end = Time::getCurrentMillis();
			cout << end-begin << "毫秒!!" << endl;
			cout << "(200*" << i << "*1000.0)/" << end-begin << "=?" << endl;
			cout << (int)((200*i*1000.0)/(end-begin)) << "个/秒" << endl;
		}
	} catch (DictException &E) {
		cout << E.what() << endl;
	}

	return 0;
}
 \endcode 
 */
class Dict
{
public :
	//! Constructor.
	Dict();

	//! Destructor.
	virtual ~Dict();

	//! 将词典文件载入到内存.
	void load(const string &dictf) throw(DictException);

	//! 将词典信息序列化到磁盘.
	void save(const string &dictf) throw(DictException);

	/**
	 * @brief   在词典中查找某个词的词ID.
	 *
	 * @param   term    词.
	 * @return  返回该词的ID.
	 * @retval  <0   表示词典中不存在该词.
	 */
	int getTermID(const char *term);
	int getTermID(const string &term);

	/**
	 * @brief   在词典中查找某个词的词信息.
	 *
	 * @param   term    词.
	 * @return  返回该词的信息.
	 * @retval  NULL   表示词典中不存在该词.
	 */
	const T_TermInfo *getTermInfo(const char *term);
	const T_TermInfo *getTermInfo(const string &term);

	/**
	 * @brief    提供遍历接口.
	 *
	 *           example : 
	 *
	 * 	     //!< 找到头儿.
	 *           dict.rewind();
	 *
	 *           const T_TermInfo *termi = NULL;
	 *
	 *           while ((termi == dict.next()) != NULL)
	 *           {
	 *           	// do something.
	 *           }
	 */
	void rewind();
	const T_TermInfo *next();
	const T_TermPair *nextitem();

	/**
	 * @brief    将词插入到词典中.
	 *
	 * @param    term    词.
	 * @return   词典为该词生成的termID.
	 * @retval   <0    表示词典已经达到最大容量,此次插入失败.
	 *           其他  为该词生成的ID.    
	 */
	int insertTerm(const char *term);
	int insertTerm(const string &term);


	//! 直接提供Sign的插入和查询接口.
	int insertSign(const T_Sign &sign);
	int getSignID(const T_Sign &sign);
	const T_TermInfo *getTermInfo(const T_Sign &sign);	

	//! 返回词典中的词数.
	size_t size() const {
		return mDictMap.size();
	}

	//! 返回目前词典中最大的词ID.
	int maxTermID() const {
		return mCurrMaxID;
	}

	//! 设置dict的词典容量. 当实际插入的词超过该值时,dict会自动调整hash_bucket.
	void resize(size_t n) {
		mDictMap.resize(n);
	}

protected :

private :
	//! 定义hash_map.
	typedef hash_map<T_Sign, T_TermInfo, hash<T_Sign>, eqSign> T_dict_hash_map;
	typedef T_dict_hash_map::iterator T_dhm_it;
private :
	int             		mCurrMaxID;
	T_dict_hash_map			mDictMap;
	T_dict_hash_map::const_iterator mDictMapIt;

	T_TermPair			mIteratorPair;
};
#endif //DICT_H_ZMH_2006_05_25
/** @} */ // end of Utils

