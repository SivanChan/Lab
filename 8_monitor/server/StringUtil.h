#ifndef StringUtil_h__
#define StringUtil_h__

#pragma once

#include <string>
#include <vector>

namespace Forge
{


/**
	标准库字符串 工具
	对std::string使用标准库算法，实现类似于MFC字符串CStringT中的一些方法
*/

class StringUtil
{
public:
	/**
		去掉字符中的空格、回车、换行
	*/
	static void trim(std::string & str, std::string const & delims = " \t\r", bool trim_left = true, bool trim_right = true);
	static void trim_left(std::string & str, std::string const & delims = " \t\r");
	static void trim_right(std::string & str, std::string const & delims = " \t\r");

	/**
		将字符串转为小写
	*/
	static void make_lower(std::string & str);

	/**
		将字符串转为大写
	*/
	static void make_upper(std::string & str);

	/**
		分割字符串
		@remarks      可代替原有 CCOM_StringParser::ActParamParser
		@param str    原始输入字符串
		@param delims 分隔符
	*/
	static std::vector<std::string> split(std::string const & str, std::string const & delims = "\t\n ");

	/**
		替换
		@param source_str    源字符串
		@param what          要被替换的部分
		@param replaced_what 替换为新的字符串
	*/
	static std::string replace(std::string const & source_str, std::string const & what, std::string const & replaced_what);
	static void replace2(std::string & source_str, std::string const & what, std::string const & replaced_what);
	/**
		类似于CString的Format
		@remark    长度限制，小于4096
		@param str 源字符串
	*/
	static std::string format(char const * fmt, ...);
	static void        format(std::string & str, char const * fmt, ...);

	/**
		字符串逆序
	*/
	static void reverse(std::string & str);

	/**
		比较两个字符串
		@param lhs 源字符串
		@param rhs 被比较字符串
		@return
			lhs == rhs return 0
			lhs <  rhs return <0
			lhs >  rhs return >0
	*/
	static int collate(std::string const & lhs, std::string const & rhs);
	static int collate_no_case(std::string const & lhs, std::string const & rhs);


	static std::string & StringUtil::StringConvert(std::wstring const & src, std::string & dest);
	static std::wstring & StringUtil::StringConvert(std::string const & src, std::wstring & dest);

	/**
		gbk2utf8
	*/
	static std::string gbk_to_utf8(const char* gbk);
};
}
#endif // StringUtil_h__