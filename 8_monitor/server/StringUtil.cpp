
#include "stdafx.h"
#include "StringUtil.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdarg.h>

//std::stringstream StringUtil::ss;

namespace Forge
{


void StringUtil::trim( std::string & str, std::string const & delims /*= " \t\r"*/, bool trim_left /*= true*/, bool trim_right /*= true*/ )
{
	if ( trim_right )
		str.erase(str.find_last_not_of(delims)+1);
	if ( trim_left )
		str.erase(0, str.find_first_not_of(delims));
}

void StringUtil::trim_left( std::string & str, std::string const & delims /*= " \t\r"*/ )
{
	StringUtil::trim(str, delims, true, false);
}

void StringUtil::trim_right( std::string & str, std::string const & delims /*= " \t\r"*/ )
{
	StringUtil::trim(str, delims, false, true);
}

void StringUtil::make_lower( std::string & str )
{
	std::transform( str.begin(), str.end(), str.begin(), tolower );
}

void StringUtil::make_upper( std::string & str )
{
	std::transform( str.begin(), str.end(), str.begin(), toupper );
}

std::vector<std::string> StringUtil::split( std::string const & str, std::string const & delims /*= "\t\n "*/ )
{
	std::vector<std::string> out;
	size_t start = 0, pos;

	do 
	{
		pos = str.find_first_of( delims, start );
		if (pos == start)
		{
			start = pos + 1;
		}
		else if (pos == std::string::npos )
		{
			out.push_back( str.substr(start) );
			break;
		}
		else
		{
			out.push_back( str.substr(start, pos - start) );
			start = pos + 1;
		}
		start = str.find_first_not_of(delims, start);

	} while (pos != std::string::npos);

	return out;
}

std::string StringUtil::replace( std::string const & source_str, std::string const & what, std::string const & replaced_what )
{
	std::string output_str     = source_str;
	std::string::size_type pos = 0;
	while(true)
	{
		pos = output_str.find(what, pos);
		if ( pos == std::string::npos )
			break;
		output_str.replace( pos, what.size(), replaced_what );
		pos += replaced_what.size();
	}
	return output_str;
}

std::string StringUtil::format( char const * fmt, ... )
{
	char buffer[4096];
	va_list arglist;
	va_start(arglist, fmt);
	_vsnprintf_s(buffer, 4096, _TRUNCATE, fmt, arglist);
	va_end(arglist);

	std::string output(buffer);
	return output;
}

void StringUtil::format( std::string & str, char const * fmt, ... )
{
	char buffer[4096];
	va_list arglist;
	va_start(arglist, fmt);
	_vsnprintf_s(buffer, 4096, _TRUNCATE, fmt, arglist);
	va_end(arglist);

	str = buffer;
}

void StringUtil::reverse( std::string & str )
{
	std::reverse( str.begin(), str.end() );
}

int StringUtil::collate( std::string const & lhs, std::string const & rhs )
{
	return lhs.compare(rhs);
}

int StringUtil::collate_no_case( std::string const & lhs, std::string const & rhs )
{
	std::string lhs_copy = lhs;
	std::string rhs_copy = rhs;

	StringUtil::make_lower(lhs_copy);
	StringUtil::make_lower(rhs_copy);
	return lhs_copy.compare(rhs_copy);
}

std::string StringUtil::gbk_to_utf8(const char* gbk)
{
	if (gbk == NULL)
		return "";

	wchar_t* widechar = NULL;
	char* multibyte = NULL;
	int length = 0;
	length = ::MultiByteToWideChar(CP_ACP, 0, gbk, -1, NULL, 0);
	if (length <= 0) return "";
	widechar = new wchar_t[length];
	::MultiByteToWideChar(CP_ACP, 0, gbk, -1, widechar, length);
	length = ::WideCharToMultiByte(CP_UTF8, 0, widechar, -1, NULL, 0, NULL, NULL);
	if (length <= 0) { delete[] widechar; return ""; }
	multibyte = new char[length];
	::WideCharToMultiByte(CP_UTF8, 0, widechar, -1, multibyte, length, NULL, NULL);
	delete[] widechar;
	std::string utf8(multibyte);
	delete[] multibyte;
	return utf8;
}

std::string StringUtil::utf8_to_gbk(const char* utf8)
{
	if (!utf8) return "";
	wchar_t* widechar = NULL;
	char* multibyte = NULL;
	int length = 0;
	length = ::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	if (length <= 0) return "";
	widechar = new wchar_t[length];
	::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, widechar, length);
	length = ::WideCharToMultiByte(CP_ACP, 0, widechar, -1, NULL, 0, NULL, NULL);
	if (length <= 0) { delete[] widechar; return ""; }
	multibyte = new char[length];
	::WideCharToMultiByte(CP_ACP, 0, widechar, -1, multibyte, length, NULL, NULL);
	delete[] widechar;
	std::string gbk(multibyte);
	delete[] multibyte;
	return gbk;
}

void StringUtil::replace2(std::string & source_str, std::string const & what, std::string const & replaced_what)
{
	std::string::size_type pos = 0;
	while(true)
	{
		pos = source_str.find(what, pos);
		if ( pos == std::string::npos )
			break;
		source_str.replace( pos, what.size(), replaced_what );
		pos += replaced_what.size();
	}
}


std::string & StringUtil::StringConvert(std::wstring const & src, std::string & dest)
{
	int const mbs_len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), NULL, 0, NULL, NULL);
	std::vector<char> tmp(mbs_len + 1);
	WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), &tmp[0], mbs_len, NULL, NULL);
	dest.assign(tmp.begin(), tmp.end() - 1);
	return dest;
}

std::wstring & StringUtil::StringConvert(std::string const & src, std::wstring & dest)
{
	int const wcs_len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), NULL, 0);
	std::vector<wchar_t> tmp(wcs_len + 1);
	MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), &tmp[0], wcs_len);
	dest.assign(tmp.begin(), tmp.end() - 1);
	return dest;
}

}