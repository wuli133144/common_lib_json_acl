/*
 * =====================================================================================
 * 
 *        Filename:  TObject.h
 * 
 *     Description:  测试Socket4Object.
 * 
 *         Version:  1.0
 *         Created:  2006年06月05日 18时34分33秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
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
 *\brief   测试Socket4Object.
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
