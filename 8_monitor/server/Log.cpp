
#include "stdafx.h"
#include <Log.h>
#include <iomanip>
#include <Windows.h>
#include <sstream>
#include <iostream>

namespace Forge
{
	std::string const & DebugOutputter::GetName() const
	{
		static std::string name("DebugOutputter");
		return name;
	}

	bool DebugOutputter::Initialise()
	{
		return true;
	}

	void DebugOutputter::ShutDown()
	{

	}

	void DebugOutputter::OutputMessage( std::string const & msg )
	{
		OutputDebugStringA(msg.c_str());
	}

	FileOutputter::FileOutputter( std::string const & file_path ) 
		: path_(file_path)
	{
	}

	FileOutputter::~FileOutputter()
	{
		ShutDown();
	}

	std::string const & FileOutputter::GetName() const
	{
		static std::string name("FileOutputter");
		return name;
	}

	bool FileOutputter::Initialise()
	{
		if (!log_file_.is_open())
			log_file_.open(path_, std::ofstream::out);
		return log_file_.is_open();
	}

	void FileOutputter::ShutDown()
	{
		if ( log_file_.is_open() )
			log_file_.close();
	}

	void FileOutputter::OutputMessage( std::string const & msg )
	{
		log_file_ << msg << std::endl;
		log_file_.flush();
	}

	template<> Log* Singleton<Log>::singleton_ = nullptr;

	Log & Log::Instance()
	{
		assert(singleton_);
		return (*singleton_);
	}

	Log::Log()
	{
		log_types_[0] = "[Info]";
		log_types_[1] = "[Warning]";
		log_types_[2] = "[Error]";
	}

	void Log::Add(OutputterPtr const & outputter)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		outputters_.push_back(outputter);
		outputter->Initialise();
	}

	void Log::Remove(OutputterPtr const & outputter)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		auto it = std::find(outputters_.begin(), outputters_.end(), outputter);
		if ( it != outputters_.end() )
		{
			outputter->ShutDown();
			it = outputters_.erase(it);
		}
	}

	void Log::RemoveAll()
	{
		for (auto it = outputters_.begin(); it != outputters_.end(); ++it)
			(*it)->ShutDown();
		outputters_.clear();
	}

	void Log::LogMessage( std::string const & msg, LogType type /*= LT_Info*/ )
	{
		std::lock_guard<std::recursive_mutex> lock(mutex_);
		std::stringstream out_stream;
		struct tm cur_tm;
		time_t cur_time;

		time( &cur_time );
		localtime_s( &cur_tm, &cur_time );
		out_stream.clear();
		out_stream.str() = "";
		out_stream << "[" 
			<< cur_tm.tm_year + 1900 << "/"
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_mon + 1  << "/"
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_mday << " " 
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_hour << ":"
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_min  << ":"
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_sec
			<< "]"
			<< log_types_[type] << " " << msg;

		for (auto it = outputters_.begin(); it != outputters_.end(); ++it)
			(*it)->OutputMessage(out_stream.str());
	}

	uint32_t Log::OutputterNum()
	{
		return static_cast<uint32_t>(outputters_.size());
	}

	std::string const & CoutOutputter::GetName() const
	{
		static std::string name("CoutOutputter");
		return name;
	}

	bool CoutOutputter::Initialise()
	{
		return true;
	}

	void CoutOutputter::ShutDown()
	{

	}

	void CoutOutputter::OutputMessage(std::string const & msg)
	{
		std::cout << msg << std::endl;
	}
}