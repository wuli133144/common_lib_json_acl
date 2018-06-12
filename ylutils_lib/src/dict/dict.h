/*
 * =====================================================================================
 * 
 *        Filename:  dict.h
 * 
 *     Description:  ͨ��hash_mapʵ�ֵĴʵ�.
 * 
 *         Version:  1.0
 *         Created:  2006��05��25�� 17ʱ29��35�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
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
 *  �ֵ���
 *  @{
 */

//! T_Sign�Ĺ�ϣ����.
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

//! T_Sign�ıȽϺ���, Ҳ�Ǹ���hash_map�ڹ�ϣ��ͻʱ�������ж�item�Ƿ����.
struct eqSign
{
	bool operator()(const T_Sign &sign1, const T_Sign &sign2) const
	{
		return (sign1.sign1 == sign2.sign1) && (sign1.sign2 == sign2.sign2);
	}
};

//! ����Ϣ.
typedef struct
{
	int   termID;		//!< �ʵĴ���, ��ͬ��˳�����ӵ����������ɵ�.
	u_int termFreq;		//!< ��Ƶ.
	u_int docFreq;		//!< �ʵ��ĵ�Ƶ��.
	u_int termWeight;	//!< ��Ȩ��.
} T_TermInfo;

//! ����ʵ�ʹ�ʵ䵽���̳����׳�DictException
class DictException : public std::exception
{
public :
	DictException(const string &msg) throw() : mMsg(msg) {}

	virtual ~DictException() throw() {}

	virtual const char *what() const throw() { return mMsg.c_str(); }

private :
	string mMsg;
};

//! �����л�������ʵ䵽�ڴ���ʱ����Է���Ľ��п����.
typedef struct
{
	T_Sign       sign;
	T_TermInfo   termInfo;
} T_TermPair;

/*! \brief ͨ��hash_mapʵ�ֵĴʵ�
 *! \class Dict dict.h dict/dict.h    
 *  
 *   \version 1.0   
 \code
 
#include "../time/Time.h"

static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " -[l|b|t]" << endl;
	cout << "���Դʵ�:" << endl << endl;

	cout << " -l :			����ʵ䵽�ڴ�,���ṩ��ѯ." << endl;
	cout << " -b :			�����зֺ�Ĵʴ����ʵ�." << endl;
	cout << " -t :			���Դʵ�Ĳ�ѯ����." << endl;
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
			cout << end-begin << "����!!" << endl;

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
		{ //! �����ʵ�
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
			cout << end-begin << "����!!" << endl;

			fwords.close();

			cout << "begin save dict into disk ......" << endl;
			begin = Time::getCurrentMillis();
			dict.save(dict_image);
			end = Time::getCurrentMillis();
			cout << end-begin << "����!!" << endl;

			cout << "please input your word : ";

			while (cin >> word)
			{
				cout << word << ":" << dict.getTermID(word) << endl;
				cout << "please input your word : ";
			}
			cout << endl;
		}
		else
		{ //! ���Դʵ��ѯ����.
			cout << "begin load dict into memory ......" << endl;
			begin = Time::getCurrentMillis();
			dict.load(dict_image);
			end = Time::getCurrentMillis();
			cout << end-begin << "����!!" << endl;

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

			cout << "�ʵ�����" << dict.size() << "����" << endl;
			cout << "��������" << i << "����" << endl;

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
			cout << end-begin << "����!!" << endl;
			cout << "(200*" << i << "*1000.0)/" << end-begin << "=?" << endl;
			cout << (int)((200*i*1000.0)/(end-begin)) << "��/��" << endl;
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

	//! ���ʵ��ļ����뵽�ڴ�.
	void load(const string &dictf) throw(DictException);

	//! ���ʵ���Ϣ���л�������.
	void save(const string &dictf) throw(DictException);

	/**
	 * @brief   �ڴʵ��в���ĳ���ʵĴ�ID.
	 *
	 * @param   term    ��.
	 * @return  ���ظôʵ�ID.
	 * @retval  <0   ��ʾ�ʵ��в����ڸô�.
	 */
	int getTermID(const char *term);
	int getTermID(const string &term);

	/**
	 * @brief   �ڴʵ��в���ĳ���ʵĴ���Ϣ.
	 *
	 * @param   term    ��.
	 * @return  ���ظôʵ���Ϣ.
	 * @retval  NULL   ��ʾ�ʵ��в����ڸô�.
	 */
	const T_TermInfo *getTermInfo(const char *term);
	const T_TermInfo *getTermInfo(const string &term);

	/**
	 * @brief    �ṩ�����ӿ�.
	 *
	 *           example : 
	 *
	 * 	     //!< �ҵ�ͷ��.
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
	 * @brief    ���ʲ��뵽�ʵ���.
	 *
	 * @param    term    ��.
	 * @return   �ʵ�Ϊ�ô����ɵ�termID.
	 * @retval   <0    ��ʾ�ʵ��Ѿ��ﵽ�������,�˴β���ʧ��.
	 *           ����  Ϊ�ô����ɵ�ID.    
	 */
	int insertTerm(const char *term);
	int insertTerm(const string &term);


	//! ֱ���ṩSign�Ĳ���Ͳ�ѯ�ӿ�.
	int insertSign(const T_Sign &sign);
	int getSignID(const T_Sign &sign);
	const T_TermInfo *getTermInfo(const T_Sign &sign);	

	//! ���شʵ��еĴ���.
	size_t size() const {
		return mDictMap.size();
	}

	//! ����Ŀǰ�ʵ������Ĵ�ID.
	int maxTermID() const {
		return mCurrMaxID;
	}

	//! ����dict�Ĵʵ�����. ��ʵ�ʲ���Ĵʳ�����ֵʱ,dict���Զ�����hash_bucket.
	void resize(size_t n) {
		mDictMap.resize(n);
	}

protected :

private :
	//! ����hash_map.
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

