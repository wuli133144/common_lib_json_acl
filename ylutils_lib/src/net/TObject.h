/*
 * =====================================================================================
 * 
 *        Filename:  TObject.h
 * 
 *     Description:  ����Socket4Object.
 * 
 *         Version:  1.0
 *         Created:  2006��06��05�� 18ʱ34��33�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup net
 *  
 *  
 *  @{
 */

#ifndef TOBJECT_H_ZMH_2006_06_05
#define TOBJECT_H_ZMH_2006_06_05

#include <iostream>
#include <string>

using namespace std;
/*!
 *\brief   ����Socket4Object.
*! \class TObject TObject.h net/TObject.h   
 */
class TObject
{
public :
	typedef struct
	{
		char mName[32];
		char a[64];
	} T_Data;
	T_Data mData;

	const char *c_str() const
	{
		return (const char *)&mData;
	}

	int size() const
	{
		return sizeof(mData);
	}

	char *address()
	{
		return (char *)&mData;
	}

	void setName(const char *name)
	{
		memcpy(mData.mName, name, sizeof(mData.mName));

		return;
	}

	friend ostream &operator<<(ostream &os, const TObject &object);
};

ostream &operator<<(ostream &os, const TObject &object)
{
	os << "mName : " << object.mData.mName;

	return os;
}
#endif //TOBJECT_H_ZMH_2006_06_05
/** @} */ // end of 
