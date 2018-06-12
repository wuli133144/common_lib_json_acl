/*
 * =====================================================================================
 * 
 *        Filename:  String.h
 * 
 *     Description:  对std::string的一些操作.
 * 
 *         Version:  1.0
 *         Created:  2006年07月07日 16时14分05秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef STRING_H_ZMH_2006_07_07
#define STRING_H_ZMH_2006_07_07

#include <iostream>
#include <string>

using namespace std;

bool isChineseChar(char c1, char c2);

/**
 * @brief    将str两端的space去掉.
 */
string &Trim(string &str);

string Trim(const char *str);

string str2lower(const char *str);

string str2lower(const string &str);

bool isallspace(const string &str);
bool isallspace(const char *str);

bool isall(const char *str, int(*fuc)(int));

#endif //STRING_H_ZMH_2006_07_07

