/*
 * =====================================================================================
 * 
 *        Filename:  ChineseSegment.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月22日 17时49分29秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "ChineseSegment.h"

ChineseSegment *ChineseSegment::mInstance = NULL;
ThreadCondLock  ChineseSegment::mSegmentLock;

ChineseSegment *ChineseSegment::instance(const char *dictimage) throw(ChineseSegmentException)
{
	if (mInstance == NULL) {	//!< 1 point 
		mSegmentLock.lock();
		//! 防止第二个线程进入1后, 第一个进入1的线程已经创建了instace对象.
		if (mInstance == NULL) {
			try {
				static ChineseSegment tInstance(dictimage);
				mInstance = &tInstance;
			} catch (ChineseSegmentException &E) {
				mSegmentLock.unlock();
				throw E;
			}
		}
		mSegmentLock.unlock();
	}

	return mInstance;
}

ChineseSegment::ChineseSegment(const char *dictimage) throw(ChineseSegmentException)
	:
	mDictData(NULL)
{
	loadDict(dictimage);
}


ChineseSegment::~ChineseSegment()
{
	if (mDictData != NULL)
	{
		delete []mDictData;
	}
}

//! 按照常规不应该在此调用
bool ChineseSegment::loadDict(const char *dictimage) throw(ChineseSegmentException)
{
	int fdict = open(dictimage, O_RDONLY);
	if (fdict == -1)
	{
		string msg = string("ChineseSegment::loadDict(") + ((dictimage==NULL)?"null":dictimage) + ") open error : " + strerror(errno);
		throw ChineseSegmentException(msg);
	}

	struct stat tStat;
	if (fstat(fdict, &tStat) == -1)
	{
		close(fdict);
		ostringstream omsg;
		omsg << "ChineseSegment::loadDict(" << ((dictimage==NULL)?"null":dictimage) << ") fstat(" 
		       << fdict <<  ") error : " <<  strerror(errno);
		throw ChineseSegmentException(omsg.str());
	}

	int fsize = tStat.st_size;
	mDictData = new char[fsize];

	read(fdict, mDictData, fsize);

	if (!ValidDictData(mDictData, fsize))
	{
		delete []mDictData;
		close(fdict);
		ostringstream omsg;
		omsg << "ChineseSegment::loadDict(" << ((dictimage==NULL)?"null":dictimage) << ") the dict data's format error";
		throw ChineseSegmentException(omsg.str());
	}

	close(fdict);

	LoadShareDataEx(mDictData+sizeof(SShareDataHead));

	return true;
}

bool ChineseSegment::addUserDict(const char *dest_dict, const char *dict_dir) throw(ChineseSegmentException)
{
	SetDictDirectory((char *)dict_dir);
	ReadSpecDict();

	SaveShareData((char *)dest_dict);

	return true;
}

int ChineseSegment::segment(char *out, const char *in, const char *reserves)
{
	mSegmentLock.lock();
	int tnum = SegTagString(in, out, reserves);
	mSegmentLock.unlock();

	return tnum;
}


#ifdef _TEST

static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " dictimage" << endl;
	return;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		dohelp(argc, argv);
		return -1;
	}

	ChineseSegment *segment = NULL;
	string buffer;
	char buffer_o[2048];

	try {
		segment = ChineseSegment::instance(argv[1]);
		cout << "please input your sentence : ";
		while (getline(cin, buffer))
		{
			bzero(buffer_o, sizeof(buffer_o));
			segment->segment(buffer_o, buffer.c_str(), " ,.'\"&^%$#");
			cout << "segment : " << buffer_o << endl;
			cout << "\nplease input your sentence : ";
		}

		cout << "add user dict. y/n?";
		if (getline(cin, buffer))
		{
			if (strcmp(buffer.c_str(), "y") == 0)
			{
				buffer = argv[1];
				buffer += ".new";
				segment->addUserDict(buffer.c_str(), "./");
				cout << "OK, see " << buffer << endl;
			}
		}
	} catch (ChineseSegmentException &E) {
		cout << E.what() << endl;
	}
		
	cout << endl;

	return 0;
}
#endif //_TEST
