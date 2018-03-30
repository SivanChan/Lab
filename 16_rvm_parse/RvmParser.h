
#pragma once

// by [3/9/2018 chenym] rvm解析

#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <inttypes.h>

class RvmParser
{
public:

	enum RvmType
	{
		RT_Unknown,
		RT_TEXT,    
		RT_BINARY
	};

	struct HeadInfo
	{
		std::string banner;
		std::string file_comment;
		std::string data_time;
		std::string author;
		std::string project_name;
		std::string mdb;
	};

// 	struct Prim
// 	{
// 
// 	};
// 	typedef std::shared_ptr<Prim> PrimPtr;

	struct Section;
	typedef std::shared_ptr<Section> SectionPtr;

	struct Section
	{
		std::string name;
		Section*  parent;
		std::vector<SectionPtr> children;

		Section()
			: parent(0)
		{

		}
	};

	RvmParser();
	~RvmParser();

	bool Open(std::string const & file_path);
	void Clear();

	/**
		解析(支持文本格式和二进制格式)
	*/
	void Parse(SectionPtr & root, HeadInfo & head_info);

	/**
		head_str   文件头信息
		desc       文件描述
		return val 下一段地址
	*/
	std::uint32_t ReadHead(HeadInfo & head_info);

	/**
		移动到下一段
	*/
	void NextSection(std::uint32_t next_section_addr);

	/**
		读取大端存储的字符串，每个字符占用4个字节
		num 字符个数
		str 读取到的字符串
	*/
	void ReadHighEndianString(std::uint32_t num, std::string & str);

	/**
		读取下一段地址
	*/
	void ReadSectionAddr(std::uint32_t & addr);

	/**
		读取段标识
	*/
	void ReadSectionFlags(std::uint32_t & section_flag_a, std::uint32_t & section_flag_b);

	/**
		读取字符
	*/
	void ReadString(std::string & desc);

	/**
		读取浮点数
	*/
	void ReadFloat(float & v);

	/**
		读取无符号整形
	*/
	void ReadUint32(std::uint32_t & v);

protected:

	/**
		qualified_name 输入路径
		out_basename   输出名字
		outpath        输出路径
	*/
	void split_file_name(std::string const & qualified_name, std::string & out_basename, std::string & outpath);

	/**
	解析文本
	*/
	void Parse_Text(SectionPtr & root, HeadInfo & head_info);

	/**
	非递归实现，解析二进制
	*/
	void Parse_Binary(SectionPtr & root, HeadInfo & head_info);

protected:
	std::ifstream file_;
	std::string   file_path_;
	RvmType       type_;
	std::uint32_t section_flag_a_;
	std::uint32_t section_flag_b_;
}; 

