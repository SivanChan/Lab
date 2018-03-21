// 16_rvm_parse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <stdint.h>
#include <iostream>
#include "EndianSwitch.h"
#include "RvmParser.h"


int main()
{
	//std::string file_path = "D:/Program Files (x86)/Review-12.1.1/models/stab.rvm";
	std::string file_path = "C:/Users/Administrator/Desktop/201803/RvmTranslator6.0/spooler.rvm";
	RvmParser rvm_parser;

	RvmParser::SectionPtr root = std::make_shared<RvmParser::Section>();
	RvmParser::HeadInfo head_info;

	rvm_parser.Open(file_path);
	rvm_parser.Parse(root, head_info);

	return 1;
}
