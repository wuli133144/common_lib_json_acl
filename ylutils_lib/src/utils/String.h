/*
 * =====================================================================================
 * 
 *        Filename:  String.h
 * 
 *     Description:  ��std::string��һЩ����.
 * 
 *         Version:  1.0
 *         Created:  2006��07��07�� 16ʱ14��05�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
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
 * @brief    ��str���˵�spaceȥ��.
 */
string &Trim(string &str);

string Trim(const char *str);

string str2lower(const char *str);

string str2lower(const string &str);

bool isallspace(const string &str);
bool isallspace(const char *str);

bool isall(const char *str, int(*fuc)(int));

#endif //STRING_H_ZMH_2006_07_07

