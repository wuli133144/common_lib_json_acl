/*
 * =====================================================================================
 * 
 *        Filename:  segment.h
 * 
 *     Description:  ����ԭ�ִʿ��еĺ�������.
 * 
 *         Version:  1.0
 *         Created:  2006��07��01�� 16ʱ22��07�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @defgroup segment
 *  
 *  
 *  @{
 */

#ifndef SEGMENT_H_ZMH_2006_07_01
#define SEGMENT_H_ZMH_2006_07_01

#include <iostream>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

int		InitShareMem(char* fname);
int		MountShareMem(char* fname);
int		UnloadShareMem(char* fname);
int 	SegTagString(const char* buf,char* dest,const char* reservstr);


#ifdef __cplusplus
}
#endif

void    SetDictDirectory(char *DictDirStr);
extern  void    ReadSpecDict(void);



//! for public interface
void 	InitShareData();
//! for public interface

void	SaveShareData(char* fname);

/**
 * @brief    ���ʵ������ڴ�.
 *
 * @param    fname    �ʵ��ļ�.
 * @return   ����״̬.
 * @retval   0   ����ɹ�.
 * 	     -1  ���д�.
 * 	     -2  �ʵ��ļ���ʽ�д�.
 */
int	LoadShareData(char* fname);

//! zmh ���ڵ���LoadShareData�����ڴ�й©,���Խ��ڴ�����Ŵ��������������ͷ�.
bool	ValidDictData(const char *pbuf, int len);
void	LoadShareDataEx(char* pbuf);

//
////for DataFile head
////
struct SShareDataHead{
        char    magic[4];
	int     len;
};




//!  SegTagSM/segtagProcess.h
int mainConvert(char *, char *, int nocate);
//!  SegTagSM/segtagProcess.h
void mainConvertStr(char* buf);
//!  SegTagSM/segtagProcess.h);
int InitConvert(char *curPath);
//!  SegTagSM/segtagProcess.h
int FreeConvert();
#endif //SEGMENT_H_ZMH_2006_07_01
/** @} */ //
