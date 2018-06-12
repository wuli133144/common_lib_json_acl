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
		if (sign1.sign1 != sign2.sign1)
			return false;
		if (sign1.sign2 != sign2.sign2)
			return false;

		return true;
		//return (sign1.sign1 == sign2.sign1) && (sign1.sign2 == sign2.sign2);
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

class Dict
{
public :
	//! Constructor.
	Dict();

	//! Destructor.
	virtual ~Dict();

	//! ���ʵ��ļ����뵽�ڴ�.
	void load(const string &dictf) throw(DictException);

	//! ���ʵ���Ϣ���л����ڴ�.
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
	 * @brief    ���ʲ��뵽�ʵ���.
	 *
	 * @param    term    ��.
	 * @return   �ʵ�Ϊ�ô����ɵ�termID.
	 * @retval   <0    ��ʾ�ʵ��Ѿ��ﵽ�������,�˴β���ʧ��.
	 *           ����  Ϊ�ô����ɵ�ID.    
	 */
	int insertTerm(const char *term);
	int insertTerm(const string &term);

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
	int             mCurrMaxID;
	T_dict_hash_map mDictMap;
};
#endif //DICT_H_ZMH_2006_05_25

