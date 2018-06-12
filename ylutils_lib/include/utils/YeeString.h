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
	 * @brief   格式化字符串
	 * \version 1.0       
	 * @param    str    待格式化的字符串
	 * @param    szFmt   字符串格式化代码
	 * @return   void
	 *
	 */	
	static void Format(string& str, const char* szFmt, ...);
	/**
	 * @brief   去除字符串左侧空白
	 * \version 1.0       
	 * @param    str    待处理的字符串
	 * @param    strTarget  空白字符
	 * @return   void
	 *
	 */	
	static void TrimLeft(string& str, const string &strTarget=" \r\n\t");
	/**
	 * @brief   去除字符串右侧空白
	 * \version 1.0       
	 * @param    str    待处理的字符串
	 * @param    strTarget  空白字符
	 * @return   void
	 *
	 */
	static void TrimRight(string& str, const string &strTarget=" \r\n\t");
	/**
	 * @brief   去除字符串前后空白
	 *  \version 1.0
	 * @param    str    待处理的字符串
	 * @param    strTarget  空白字符
	 * @return   void
	 *
	 */
	static void Trim(string& str, const string &strTarget=" \r\n\t");
	/**
	 * @brief   替换子字符串
	 * \version 1.0       
	 * @param    str    待处理的字符串
	 * @param    strToReplaced  待替换的子字符串
	 * @param    strReplaced    替换字符串
	 * @return   void
	 *
	 */
	static void ReplaceAll(string& str, const string& strToReplaced, const string& strReplaced);
	/**
	 * @brief   字符串分割，连续的多个delims将被作为一个delims进行分割，即两个连续的delims之间不做分割
	 * \version 1.0       
	 * @param    str    待处理的字符串
	 * @param    tokens  结果保存的vector
	 * @param    delims   按delims进行分割
	 * @return   void
	 *
	 */
	static void Tokenize(const string& str, vector<string>& tokens, const string& delims);
	/**
	 * @brief   字符串分割，以delims作为分割符，即两个连续的delims之间也做出分组，为空
	 * \version 1.0       
	 * @param    str    待处理的字符串
	 * @param    tokens  结果保存的vector
	 * @param    delims   按delims进行分割
	 * @return   void
	 *
	 */
	static void SimpleTokenize(const string& str, vector<string>& tokens, const string& delims);
	/**
	 * @brief   字符串结尾判断
	 * \version 1.0       
	 * @param    str    待处理的字符串
	 * @param    endStr  判断用字符串
	 * @return   boolean
	 * @retval   True   以endStr结尾
	 * @retval   False   不以endStr结尾
	 */
	static bool EndsWith(const string& str, const string& endStr);
	/**
	 * @brief   字符串开头判断
	 * \version 1.0       
	 * @param    str    待处理的字符串
	 * @param    startStr  判断用字符串
	 * @return    boolean  
	 * @retval   True   以startStr开始
	 * @retval   False   不以startStr开始
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
	 * @brief   字符串全部小写
	 * \version 1.0       
	 * @param    str    待处理的字符串
	 * @return   void	 
	 */
	static void ToLowerCase(string& str);
	/**
	 * @brief   去除字符串中的HTML Tags
	 * \version 1.0       
	 * @param    strText    待处理的字符串
	 * @return   void
	 */
	static void RemoveHtmlTags(string& strText);
	/**
	 * @brief   计算字符串相似度，非编辑距离
	 * \version 1.0       
	 * @param   s1   待匹配字符串1
	 * @param   s2   待匹配字符串1
	 * @param   n    匹配力度调整参数，通常为2
	 * @return   相似程度
	 *@retval 	1  完全匹配
	 *@retval          0  无相似度
	 *@retval      float  相似度              
	 */               
	static float CalStringSim(const char* s1, const char* s2, int n);

	/**
	 * @brief   当前日期时间.  same with ctime_r()
	 *  \version 1.0       
	 * @return  string 	ex,"Fri Sep 13 00:00:00 1986\n\0"
	 */  
	static string GetCurrTime();
	/**
	 * @brief   计算字符串64位签名
	 * \version 1.0       
	 * @param   str_src  待计算签名的字符串
	 * @param   psign1   结果签名高32位
	 * @param   psign2   结果签名低32位
	 * @return  1
	 */  
	static void create_sign64(const char *str_src, int slen, unsigned int *psign1, unsigned int *psign2);
	static void create_sign64(const char *str_src, unsigned int *psign1, unsigned int *psign2);
	static void create_sign64(const char *str_src, unsigned long *psign);
	/**
	 * @brief   去除子字符串
	 * \version 1.0       
	 * @param   songname  待处理的字符串
	 * @param   sBegin   待去除字符串的开始标志
	 * @param   sEnd   待去除字符串的结束标志，去除的字串将以此结尾
	 * @return  void
	 */  
	static void RemoveSubstr(string& songname, string sBegin, string sEnd);
	/**
	 * @brief   判断字符串是否由空白字符组成
	 * \version 1.0       
	 * @param   str  待处理的字符串
	 * @return  bool
	 */  
	static bool isallspace(const string &str);
	/**
	 * @brief   判断字符串是否由空白字符组成
	 * \version 1.0       
	 * @param   str  待处理的字符串
	 * @return  bool
	 */  
	static bool isallspace(const char *str);
	/**
	 * @brief   判断字符串中的字符都符合 fuc
	 * \version 1.0       
	 * @param   str  待处理的字符串
	 * @param   fuc  判断函数
	 * @return  bool
	 */  
	static bool isall(const char *str, int(*fuc)(int));
	/**
	*@brief 判断由c1,c2组成的宽字符是否是中文
	*
	*/
	static bool isChineseChar(char c1, char c2);
	/**
	 * @brief   提取子字符串
	 * \version 1.0       
	 * @param   src  待处理的字符串
	 * @param   sBegin   待去除字符串的开始标志
	 * @param   sEnd   待去除字符串的结束标志，去除的字串将以此结尾
	 * @return  void
	 */  
	static string ExtractSubstr(string& src, string sBegin, string sEnd);	// return empty string if sEnd not found
	static string ExtractSubstr2(string& src, string sBegin, string sEnd);	// return substr from sBegin if sEnd not found
};

#endif

