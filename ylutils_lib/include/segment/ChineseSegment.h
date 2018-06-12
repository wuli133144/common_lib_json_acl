/*
 * =====================================================================================
 * 
 *        Filename:  ChineseSegment.h
 * 
 *     Description:  pku�ִʿ�ķ�װ. �ִʴ���֧�ֶ��̴߳���, ������segment�м�����ͬ��.
 * 
 *         Version:  1.0
 *         Created:  2006��06��22�� 16ʱ44��20�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */

/** @addtogroup segment
 *  
 *  
 *  @{
 */
#ifndef CHINESESEGMENT_H_ZMH_2006_06_22
#define CHINESESEGMENT_H_ZMH_2006_06_22

#include <iostream>
#include <string>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "sync/ThreadCondLock.h"
#include "segment.h"

using namespace std;

class ChineseSegmentException : public std::exception
{
public :
	explicit
	ChineseSegmentException(const string &msg) throw() : mMsg(msg)  {}

	virtual ~ChineseSegmentException() throw() {}

	virtual const char *what() const throw() { return mMsg.c_str(); }

	void appendErrnoMsg() {
		mMsg += strerror(errno);
		return;
	}

private :
	string mMsg;
};
/*!\brief ���ķִ�
 *@brief  pku�ִʿ�ķ�װ. �ִʴ���֧�ֶ��̴߳���, ������segment�м�����ͬ��.
 *! \class ChineseSegment ChineseSegment.h net/ChineseSegment.h   
 \code
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
 \endcode
 */
class ChineseSegment
{
public :
	//! Singleton.
	static ChineseSegment *instance(const char *dictimage) throw(ChineseSegmentException);

	//! Destructor.
	virtual ~ChineseSegment();

	/**
	 * @brief    ����ִʴʵ䵽�ڴ�.
	 *
	 * @param    dictimage    �ʵ��ļ�.
	 * @retval   ���Ƿ���true.
	 * @exception    ChineseSegmentException    ��/���ļ��������ݸ�ʽ�д�.
	 */
	bool loadDict(const char *dictimage) throw(ChineseSegmentException);
	/**
	 * @brief    ����ֵ�.
	 *
	 * @param    dest_dict   Ŀ���ֵ�
	 * @param    dict_dir  �ֵ�Ŀ¼
	 * @return  boolean
	 */
	bool addUserDict(const char *dest_dict, const char *dict_dir) throw(ChineseSegmentException);

	/**
	 * @brief    �зִ���.
	 *
	 * @param    out    ���������. ������Ӧ����֤out�㹻��. 2*size(in)�㹻.
	 *                  ���out==NULL, ������������׼�����.
	 * @param    in     ���뻺����, ���зֵľ���.
	 * @param    reserves    �����ַ�����, ������reserves�е��ַ��������.
	 *                       ��Ҫ���һЩ�����ַ������
	 * @return   ���طֳ��Ĵʵĸ���.
	 */
	int segment(char *out, const char *in, const char *reserves);

protected :
	//! Constructor.
	ChineseSegment();
	//! Constructor.
	ChineseSegment(const char *dictimage) throw(ChineseSegmentException);

private :
	char 			*mDictData;
	static ThreadCondLock	mSegmentLock;
	static ChineseSegment	*mInstance;
};
#endif //CHINESESEGMENT_H_ZMH_2006_06_22
/** @} */ //
