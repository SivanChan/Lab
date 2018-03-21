
#pragma once

// by [3/9/2018 chenym] rvm����

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
		RT_TEXT,    // �ݲ�֧��
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
		����(֧���ı���ʽ�Ͷ����Ƹ�ʽ)
	*/
	void Parse(SectionPtr & root, HeadInfo & head_info);

	/**
		head_str   �ļ�ͷ��Ϣ
		desc       �ļ�����
		return val ��һ�ε�ַ
	*/
	std::uint32_t ReadHead(HeadInfo & head_info);

	/**
		�ƶ�����һ��
	*/
	void NextSection(std::uint32_t next_section_addr);

	/**
		��ȡ��˴洢���ַ�����ÿ���ַ�ռ��4���ֽ�
		num �ַ�����
		str ��ȡ�����ַ���
	*/
	void ReadHighEndianString(std::uint32_t num, std::string & str);

	/**
		��ȡ��һ�ε�ַ
	*/
	void ReadSectionAddr(std::uint32_t & addr);

	/**
		��ȡ�α�ʶ
	*/
	void ReadSectionFlags(std::uint32_t & section_flag_a, std::uint32_t & section_flag_b);

	/**
		��ȡ�ַ�
	*/
	void ReadString(std::string & desc);

	/**
		��ȡ������
	*/
	void ReadFloat(float & v);

	/**
		��ȡ�޷�������
	*/
	void ReadUint32(std::uint32_t & v);

protected:

	/**
		qualified_name ����·��
		out_basename   �������
		outpath        ���·��
	*/
	void split_file_name(std::string const & qualified_name, std::string & out_basename, std::string & outpath);

	/**
	�����ı�
	*/
	void Parse_Text(SectionPtr & root, HeadInfo & head_info);

	/**
	�ǵݹ�ʵ�֣�����������
	*/
	void Parse_Binary(SectionPtr & root, HeadInfo & head_info);

protected:
	std::ifstream file_;
	std::string   file_path_;
	RvmType       type_;
	std::uint32_t section_flag_a_;
	std::uint32_t section_flag_b_;
}; 

