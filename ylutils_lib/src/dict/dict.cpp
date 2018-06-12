/*
 * =====================================================================================
 * 
 *        Filename:  dict.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年05月25日 21时21分36秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "dict.h"
#include "../sign/ul_sign.h"


Dict::Dict() : mCurrMaxID(-1), mDictMap(1500000)
{
}

Dict::~Dict()
{
}

void Dict::load(const string &dictf) throw(DictException)
{
	/* 词典文件结构.
	--------------------------------------------------------------------------------------------
	|  max_term_id   |   total_term_num  |   <sign,terminfo>  | ......... 
	--------------------------------------------------------------------------------------------
	|<----4bytes---->|<------4bytes----->|<-------total_term_num*(sizeof(sign)+sizeof(terminfo))
	*/
	string msg = string("Dict::load(") + dictf + ") : exception : ";

	ifstream fdict(dictf.c_str(), ios_base::binary);
	if (!fdict)
	{
		throw DictException(msg+strerror(errno));
	}

	u_int term_num;
	if (!fdict.read((char *)&mCurrMaxID, sizeof(mCurrMaxID))
			|| !fdict.read((char *)&term_num, sizeof(term_num)))
	{
		fdict.close();
		throw DictException(msg + "读取最大termID时出错,文件格式有错,词典文件至少要有8个字节的数据");
	}

	if ((u_int)(mCurrMaxID+1) < term_num)
	{
		fdict.close();
		ostringstream omsg;
		omsg << msg << "词典文件中信息出现逻辑错误, mCurrMaxID=" << mCurrMaxID << "term_num=" << term_num;
		throw DictException(omsg.str());
	}

	//! 用vector来申请内存来避开内存申请和释放.
	vector<T_TermPair> allTermPair(term_num);

	if (!fdict.read((char *)&(allTermPair[0]), term_num*sizeof(T_TermPair)))
	{
		fdict.close();
		throw DictException(msg + "词典格式不对,应该是文件长度与词典信息长度不对.");
	}

	fdict.close();

	mDictMap.clear();
	//! 读取完毕,逐条插入词典.
	for (u_int i = 0; i < term_num; i++)
	{
		mDictMap.insert(make_pair(allTermPair[i].sign, allTermPair[i].termInfo));
	}

	//! OK.

	return;
}

void Dict::save(const string &dictf) throw(DictException)
{
	string msg = string("Dict::save(") + dictf + ") error : ";

	ofstream  fdict(dictf.c_str(), ios_base::binary);
	if (!fdict)
	{
		throw DictException(msg+strerror(errno));
	}

	//! 将目前最大词ID存入词典文件.
	fdict.write((char *)&mCurrMaxID, sizeof(int));

	//! 将目前词典中的词数存入词典文件.
	int term_num = mDictMap.size();
	fdict.write((char *)&term_num, sizeof(int));

	//! 将词典中的词条信息全部存入词典文件.
	for (T_dhm_it it = mDictMap.begin(); it != mDictMap.end(); it++)
	{
		fdict.write((char *)&(*it), sizeof(T_TermPair));
	}

	fdict.close();

	//! OK.
	
	return;
}

int Dict::getTermID(const char *term)
{
	//! 做签名坐标.
	T_Sign sign;
	//u_int sign1, sign2;
	//getSign(sign1, sign2, term);
	YeeSign::getSign(sign, term);
	//creat_sign_f64((char *)term, strlen(term), &sign.sign1, &sign.sign2);

	//! 查找.
	T_dhm_it it = mDictMap.find(sign);

	return (it==mDictMap.end())?-1:it->second.termID;
	//return 1;
}

int Dict::getTermID(const string &term)
{
	return this->getTermID(term.c_str());
}

const T_TermInfo *Dict::getTermInfo(const char *term)
{
	//! 做签名坐标.
	T_Sign sign;
	YeeSign::getSign(sign, term);

	//! 查找.
	T_dhm_it it = mDictMap.find(sign);

	return (it==mDictMap.end())?NULL:&(it->second);
}

const T_TermInfo *Dict::getTermInfo(const string &term)
{
	return this->getTermInfo(term.c_str());
}

int Dict::insertTerm(const char *term)
{
	//! 做签名坐标.
	T_Sign sign;
	YeeSign::getSign(sign, term);

	return insertSign(sign);
}

int Dict::insertTerm(const string &term)
{
	return this->insertTerm(term.c_str());
}

int Dict::insertSign(const T_Sign &sign)
{
	int retID = -1;
	//! 查找.
	T_dhm_it it = mDictMap.find(sign);

	//! 找到便修改,找不到就插入.
	if (it != mDictMap.end())
	{
		it->second.termFreq++;
		retID = it->second.termID;
	}
	else
	{
		T_TermInfo termInfo = {++mCurrMaxID, 1, 0, 0};
		mDictMap.insert(make_pair(sign, termInfo));
		retID = mCurrMaxID;
	}

	return retID;
}

int Dict::getSignID(const T_Sign &sign)
{
	//! 查找.
	T_dhm_it it = mDictMap.find(sign);

	return (it==mDictMap.end())?-1:it->second.termID;
}

const T_TermInfo *Dict::getTermInfo(const T_Sign &sign)
{
	//! 查找.
	T_dhm_it it = mDictMap.find(sign);

	return (it==mDictMap.end())?NULL:&(it->second);
}

void Dict::rewind()
{
	mDictMapIt = mDictMap.begin();
	return;
}

const T_TermInfo *Dict::next()
{
	const T_TermInfo *pTermInfo = (mDictMapIt == mDictMap.end())?NULL:&((mDictMapIt++)->second);

	return pTermInfo;
}

const T_TermPair *Dict::nextitem()
{
	if (mDictMapIt == mDictMap.end()) return NULL;
	mIteratorPair.sign = mDictMapIt->first;
	mIteratorPair.termInfo = mDictMapIt->second;
	mDictMapIt ++;
	return &mIteratorPair;
}


#ifdef _TEST

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
#endif //_TEST
