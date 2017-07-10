#ifndef StringUtil_h__
#define StringUtil_h__

#pragma once

#include <string>
#include <vector>

namespace Forge
{


/**
	��׼���ַ��� ����
	��std::stringʹ�ñ�׼���㷨��ʵ��������MFC�ַ���CStringT�е�һЩ����
*/

class StringUtil
{
public:
	/**
		ȥ���ַ��еĿո񡢻س�������
	*/
	static void trim(std::string & str, std::string const & delims = " \t\r", bool trim_left = true, bool trim_right = true);
	static void trim_left(std::string & str, std::string const & delims = " \t\r");
	static void trim_right(std::string & str, std::string const & delims = " \t\r");

	/**
		���ַ���תΪСд
	*/
	static void make_lower(std::string & str);

	/**
		���ַ���תΪ��д
	*/
	static void make_upper(std::string & str);

	/**
		�ָ��ַ���
		@remarks      �ɴ���ԭ�� CCOM_StringParser::ActParamParser
		@param str    ԭʼ�����ַ���
		@param delims �ָ���
	*/
	static std::vector<std::string> split(std::string const & str, std::string const & delims = "\t\n ");

	/**
		�滻
		@param source_str    Դ�ַ���
		@param what          Ҫ���滻�Ĳ���
		@param replaced_what �滻Ϊ�µ��ַ���
	*/
	static std::string replace(std::string const & source_str, std::string const & what, std::string const & replaced_what);
	static void replace2(std::string & source_str, std::string const & what, std::string const & replaced_what);
	/**
		������CString��Format
		@remark    �������ƣ�С��4096
		@param str Դ�ַ���
	*/
	static std::string format(char const * fmt, ...);
	static void        format(std::string & str, char const * fmt, ...);

	/**
		�ַ�������
	*/
	static void reverse(std::string & str);

	/**
		�Ƚ������ַ���
		@param lhs Դ�ַ���
		@param rhs ���Ƚ��ַ���
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