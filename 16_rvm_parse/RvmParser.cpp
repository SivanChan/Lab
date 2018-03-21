#include "stdafx.h"
#include "RvmParser.h"
#include <array>
#include "EndianSwitch.h"

RvmParser::RvmParser() : type_(RT_Unknown),
	section_flag_a_(0),
	section_flag_b_(0)
{

}


RvmParser::~RvmParser()
{
	Clear();
}

bool RvmParser::Open(std::string const & file_path)
{
	if (file_.is_open())
		return false;

	file_.open(file_path, std::fstream::in | std::fstream::binary);
	if (!file_.is_open())
		return false;

	file_path_ = file_path;

	std::string str_head(4, '\0');
	file_.read(&str_head[0], str_head.length());
	type_ = (str_head.compare("HEAD") == 0) ? RT_TEXT : RT_BINARY;

	return true;
}

void RvmParser::Clear()
{
	if (file_.is_open())
		file_.close();
	file_path_.clear();
}

void RvmParser::Parse(SectionPtr & root, HeadInfo & head_info)
{
	if (type_ == RT_TEXT)
		Parse_Text(root, head_info);
	else
		Parse_Binary(root, head_info);
}

void RvmParser::Parse_Text(SectionPtr & root, HeadInfo & head_info)
{
	file_.close();
	file_.open(file_path_, std::fstream::in);
	if (!file_.is_open())
		return;
	std::string buffer;
	std::string file_base_path;
	split_file_name(file_path_, root->name, file_base_path);

	Section* parent = root.get();
	int count = 0;
	while (!file_.eof())
	{
		std::getline(file_, buffer);
		++count;

		switch (count)
		{
			case 1:
			{
				if (buffer.compare("HEAD") != 0)
					return;
			}
			break;
			case 2:
			{
				// flag
			}
			break;
			case 3:
			{
				head_info.banner = buffer;
			}
			break;
			case 4:
			{
				head_info.file_comment = buffer;
			}
			break;
			case 5:
			{
				head_info.data_time = buffer;
			}
			break;
			case 6:
			{
				head_info.author = buffer;
			}
			break;
			case 7:
			{
				// ×Ö·û¼¯
			}
			break;
			default:
			{
				if (buffer.compare("MODL") == 0)
				{
					std::getline(file_, buffer);
					std::getline(file_, head_info.project_name);
					std::getline(file_, head_info.mdb);
				} 
				else if (buffer.compare("CNTB") == 0)
				{
					RvmParser::SectionPtr node = std::make_shared<RvmParser::Section>();

					std::getline(file_, buffer);
					std::getline(file_, node->name);
					std::getline(file_, buffer);
					std::getline(file_, buffer);

					if (parent)
					{
						parent->children.push_back(node);
						node->parent = parent;

						parent = node.get();
					}
				}
				else if (buffer.compare("CNTE") == 0)
				{
					std::getline(file_, buffer);
					if (parent)
					{
						parent = parent->parent;
					}
				}
				else if (buffer.compare("PRIM") == 0)
				{
					std::getline(file_, buffer);
					std::getline(file_, buffer);
					std::getline(file_, buffer);
					std::getline(file_, buffer);
					std::getline(file_, buffer);
					std::getline(file_, buffer);
					std::getline(file_, buffer);
					std::getline(file_, buffer);
				}
			}
			break;
		}
	}
}

void RvmParser::Parse_Binary(SectionPtr & root, HeadInfo & head_info)
{
	std::string file_base_path;
	split_file_name(file_path_, root->name, file_base_path);

	std::string section_type, name;
	std::uint32_t section_flag_a = 0, section_flag_b = 0;
	float fa = 0, fb = 0, fc = 0;
	std::uint32_t iv = 0;
	std::uint32_t next_section_addr = 0;
	next_section_addr = ReadHead(head_info);

	Section* parent = root.get();

	while (next_section_addr)
	{
		section_type.clear();
		name.clear();

		NextSection(next_section_addr);
		ReadHighEndianString(4, section_type);
		ReadSectionAddr(next_section_addr);
		ReadSectionFlags(section_flag_a, section_flag_b);

		if (section_type.compare("MODL") == 0)
		{
			ReadString(head_info.project_name);
			ReadString(head_info.mdb);
		}
		else if (section_type.compare("CNTB") == 0)
		{
			ReadString(name);
			ReadFloat(fa);
			ReadFloat(fb);
			ReadFloat(fc);
			ReadUint32(iv);

			if (parent)
			{
				RvmParser::SectionPtr node = std::make_shared<RvmParser::Section>();
				node->name = name;
				parent->children.push_back(node);
				node->parent = parent;

				parent = node.get();
			}
		}
		else if (section_type.compare("CNTE") == 0)
		{
			if (parent)
			{
				parent = parent->parent;
			}
		}
		else if (section_type.compare("PRIM") == 0)
		{
		}
		else if (section_type.compare("END:") == 0)
		{
			return;
		}
	}
}

std::uint32_t RvmParser::ReadHead(HeadInfo & head_info)
{
	std::uint32_t next_section_addr = 0;
	std::string head_str;

	file_.seekg(std::ios::beg);
	if (type_ == RT_TEXT)
	{
		head_str.resize(4);
		file_.read(static_cast<char*>((void*)head_str.data()), head_str.length());
	}
	else if (type_ == RT_BINARY)
	{
		ReadHighEndianString(4, head_str);
	}

	if (head_str.compare("HEAD") != 0)
		return 0;

	ReadSectionAddr(next_section_addr);
	ReadSectionFlags(section_flag_a_, section_flag_b_);
	ReadString(head_info.banner);
	ReadString(head_info.file_comment); 
	ReadString(head_info.data_time);
	ReadString(head_info.author); 

	return next_section_addr;
}

void RvmParser::NextSection(std::uint32_t next_section_addr)
{
	file_.seekg(static_cast<std::streampos>(next_section_addr));
}

void RvmParser::ReadHighEndianString(std::uint32_t num, std::string & str)
{
	std::array<char, 4> ch;
	for (std::uint32_t i = 0; i < num; ++i)
	{
		memset(static_cast<char*>(ch.data()), 0, 4);
		file_.read(static_cast<char*>(ch.data()), 4);

		for (auto it = ch.crbegin(); it != ch.crend(); ++it)
		{
			if ((*it) != '\0')
				str.push_back((*it));
		}
	}
}

void RvmParser::ReadSectionAddr(std::uint32_t & addr)
{
	file_.read(static_cast<char*>((void*)&addr), sizeof(addr));
	EndianSwitch<sizeof(std::uint32_t)>(&addr);
}

void RvmParser::ReadSectionFlags(std::uint32_t & section_flag_a, std::uint32_t & section_flag_b)
{
	file_.read(static_cast<char*>((void*)&section_flag_a), sizeof(section_flag_a));
	EndianSwitch<sizeof(std::uint32_t)>(&section_flag_a);
	file_.read(static_cast<char*>((void*)&section_flag_b), sizeof(section_flag_b));
	EndianSwitch<sizeof(std::uint32_t)>(&section_flag_b);
}

void RvmParser::ReadString(std::string & desc)
{
	std::uint32_t desc_length;
	file_.read(static_cast<char*>((void*)&desc_length), sizeof(desc_length));
	EndianSwitch<sizeof(std::uint32_t)>(&desc_length);
	desc_length *= 4;
	if (desc_length)
	{
		desc.resize(desc_length + 1);
		memset(static_cast<char*>((void*)desc.data()), 0, desc.size());
		file_.read(static_cast<char*>((void*)desc.data()), desc_length);
	}
}

void RvmParser::ReadFloat(float & v)
{
	file_.read(static_cast<char*>((void*)&v), sizeof(v));
	EndianSwitch<sizeof(float)>(&v);
}

void RvmParser::ReadUint32(std::uint32_t & v)
{
	file_.read(static_cast<char*>((void*)&v), sizeof(v));
	EndianSwitch<sizeof(float)>(&v);
}

void RvmParser::split_file_name(std::string const & qualified_name, std::string & out_basename, std::string & outpath)
{
	std::string path = qualified_name;
	std::replace(path.begin(), path.end(), '\\', '/');
	size_t i = path.find_last_of('/');

	if (i == std::string::npos)
	{
		outpath.clear();
		out_basename = qualified_name;
	}
	else
	{
		out_basename = path.substr(i + 1, path.size() - i - 1);
		outpath = path.substr(0, i + 1);
	}
}
