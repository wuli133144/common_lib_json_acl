#ifndef _YEESTRING_H_
#define _YEESTRING_H_

#include <string>
#include <vector>
#include <map>

using namespace std;

class YeeString {
public:
	//! Constrcutor.
	YeeString();
	//! Destructor.
	virtual ~YeeString();
	/**
	 * @brief   ��ʽ���ַ���
	 * \version 1.0       
	 * @param    str    ����ʽ�����ַ���
	 * @param    szFmt   �ַ�����ʽ������
	 * @return   void
	 *
	 */	
	static void Format(string& str, const char* szFmt, ...);
	/**
	 * @brief   ȥ���ַ������հ�
	 * \version 1.0       
	 * @param    str    ��������ַ���
	 * @param    strTarget  �հ��ַ�
	 * @return   void
	 *
	 */	
	static void TrimLeft(string& str, const string &strTarget=" \r\n\t");
	/**
	 * @brief   ȥ���ַ����Ҳ�հ�
	 * \version 1.0       
	 * @param    str    ��������ַ���
	 * @param    strTarget  �հ��ַ�
	 * @return   void
	 *
	 */
	static void TrimRight(string& str, const string &strTarget=" \r\n\t");
	/**
	 * @brief   ȥ���ַ���ǰ��հ�
	 *  \version 1.0
	 * @param    str    ��������ַ���
	 * @param    strTarget  �հ��ַ�
	 * @return   void
	 *
	 */
	static void Trim(string& str, const string &strTarget=" \r\n\t");
	/**
	 * @brief   �滻���ַ���
	 * \version 1.0       
	 * @param    str    ��������ַ���
	 * @param    strToReplaced  ���滻�����ַ���
	 * @param    strReplaced    �滻�ַ���
	 * @return   void
	 *
	 */
	static void ReplaceAll(string& str, const string& strToReplaced, const string& strReplaced);
	/**
	 * @brief   �ַ����ָ�����Ķ��delims������Ϊһ��delims���зָ������������delims֮�䲻���ָ�
	 * \version 1.0       
	 * @param    str    ��������ַ���
	 * @param    tokens  ��������vector
	 * @param    delims   ��delims���зָ�
	 * @return   void
	 *
	 */
	static void Tokenize(const string& str, vector<string>& tokens, const string& delims);
	/**
	 * @brief   �ַ����ָ��delims��Ϊ�ָ����������������delims֮��Ҳ�������飬Ϊ��
	 * \version 1.0       
	 * @param    str    ��������ַ���
	 * @param    tokens  ��������vector
	 * @param    delims   ��delims���зָ�
	 * @return   void
	 *
	 */
	static void SimpleTokenize(const string& str, vector<string>& tokens, const string& delims);
	/**
	 * @brief   �ַ�����β�ж�
	 * \version 1.0       
	 * @param    str    ��������ַ���
	 * @param    endStr  �ж����ַ���
	 * @return   boolean
	 * @retval   True   ��endStr��β
	 * @retval   False   ����endStr��β
	 */
	static bool EndsWith(const string& str, const string& endStr);
	/**
	 * @brief   �ַ�����ͷ�ж�
	 * \version 1.0       
	 * @param    str    ��������ַ���
	 * @param    startStr  �ж����ַ���
	 * @return    boolean  
	 * @retval   True   ��startStr��ʼ
	 * @retval   False   ����startStr��ʼ
	 */
	static bool StartsWith(const string& str, const string& startStr);
	/**
	 * @brief   int 2 string
	 * \version 1.0       
	 * @param    a	    int
	 * @return   string   itoa(a)
	 */
	static string ToString(int a);
	/**
	 * @brief   �ַ���ȫ��Сд
	 * \version 1.0       
	 * @param    str    ��������ַ���
	 * @return   void	 
	 */
	static void ToLowerCase(string& str);
	/**
	 * @brief   ȥ���ַ����е�HTML Tags
	 * \version 1.0       
	 * @param    strText    ��������ַ���
	 * @return   void
	 */
	static void RemoveHtmlTags(string& strText);
	/**
	 * @brief   �����ַ������ƶȣ��Ǳ༭����
	 * \version 1.0       
	 * @param   s1   ��ƥ���ַ���1
	 * @param   s2   ��ƥ���ַ���1
	 * @param   n    ƥ�����ȵ���������ͨ��Ϊ2
	 * @return   ���Ƴ̶�
	 *@retval 	1  ��ȫƥ��
	 *@retval          0  �����ƶ�
	 *@retval      float  ���ƶ�              
	 */               
	static float CalStringSim(const char* s1, const char* s2, int n);

	/**
	 * @brief   ��ǰ����ʱ��.  same with ctime_r()
	 *  \version 1.0       
	 * @return  string 	ex,"Fri Sep 13 00:00:00 1986\n\0"
	 */  
	static string GetCurrTime();
	/**
	 * @brief   �����ַ���64λǩ��
	 * \version 1.0       
	 * @param   str_src  ������ǩ�����ַ���
	 * @param   psign1   ���ǩ����32λ
	 * @param   psign2   ���ǩ����32λ
	 * @return  1
	 */  
	static void create_sign64(const char *str_src, int slen, unsigned int *psign1, unsigned int *psign2);
	static void create_sign64(const char *str_src, unsigned int *psign1, unsigned int *psign2);
	static void create_sign64(const char *str_src, unsigned long *psign);
	/**
	 * @brief   ȥ�����ַ���
	 * \version 1.0       
	 * @param   songname  ��������ַ���
	 * @param   sBegin   ��ȥ���ַ����Ŀ�ʼ��־
	 * @param   sEnd   ��ȥ���ַ����Ľ�����־��ȥ�����ִ����Դ˽�β
	 * @return  void
	 */  
	static void RemoveSubstr(string& songname, string sBegin, string sEnd);
	/**
	 * @brief   �ж��ַ����Ƿ��ɿհ��ַ����
	 * \version 1.0       
	 * @param   str  ��������ַ���
	 * @return  bool
	 */  
	static bool isallspace(const string &str);
	/**
	 * @brief   �ж��ַ����Ƿ��ɿհ��ַ����
	 * \version 1.0       
	 * @param   str  ��������ַ���
	 * @return  bool
	 */  
	static bool isallspace(const char *str);
	/**
	 * @brief   �ж��ַ����е��ַ������� fuc
	 * \version 1.0       
	 * @param   str  ��������ַ���
	 * @param   fuc  �жϺ���
	 * @return  bool
	 */  
	static bool isall(const char *str, int(*fuc)(int));
	/**
	*@brief �ж���c1,c2��ɵĿ��ַ��Ƿ�������
	*
	*/
	static bool isChineseChar(char c1, char c2);
	/**
	 * @brief   ��ȡ���ַ���
	 * \version 1.0       
	 * @param   src  ��������ַ���
	 * @param   sBegin   ��ȥ���ַ����Ŀ�ʼ��־
	 * @param   sEnd   ��ȥ���ַ����Ľ�����־��ȥ�����ִ����Դ˽�β
	 * @return  void
	 */  
	static string ExtractSubstr(string& src, string sBegin, string sEnd);	// return empty string if sEnd not found
	static string ExtractSubstr2(string& src, string sBegin, string sEnd);	// return substr from sBegin if sEnd not found
};

#endif

