/*
 * =====================================================================================
 * 
 *        Filename:  ChineseSegment.h
 * 
 *     Description:  pku分词库的封装. 分词处理不支持多线程处理, 所以在segment中加上了同步.
 * 
 *         Version:  1.0
 *         Created:  2006年06月22日 16时44分20秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
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
/*!\brief 中文分词
 *@brief  pku分词库的封装. 分词处理不支持多线程处理, 所以在segment中加上了同步.
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
	 * @brief    载入分词词典到内存.
	 *
	 * @param    dictimage    词典文件.
	 * @retval   总是返回true.
	 * @exception    ChineseSegmentException    打开/读文件或者数据格式有错.
	 */
	bool loadDict(const char *dictimage) throw(ChineseSegmentException);
	/**
	 * @brief    添加字典.
	 *
	 * @param    dest_dict   目标字典
	 * @param    dict_dir  字典目录
	 * @return  boolean
	 */
	bool addUserDict(const char *dest_dict, const char *dict_dir) throw(ChineseSegmentException);

	/**
	 * @brief    切分处理.
	 *
	 * @param    out    输出缓冲区. 调用者应当保证out足够大. 2*size(in)足够.
	 *                  如果out==NULL, 结果会输出到标准输出上.
	 * @param    in     输入缓冲区, 被切分的句子.
	 * @param    reserves    保留字符序列, 包含在reserves中的字符将被输出.
	 *                       主要针对一些特殊字符：标点
	 * @return   返回分出的词的个数.
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
