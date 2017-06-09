/********************************************************************
	created:	2014/09/02
	created:	2014/9/2   22:38
	filename: 	Log.h
	author:		Sivan
	purpose:	日志
*********************************************************************/

#ifndef Forge_Log_h__
#define Forge_Log_h__

#pragma once

#include <mutex>
#include <Singleton.h>
#include <array>

namespace Forge
{
	enum LogType
	{
		LT_Info    = 0, // 信息
		LT_Warning = 1, // 警告
		LT_Error   = 2  // 错误
	};

	class  Outputter
	{
	public:
		Outputter()          {}
		virtual ~Outputter() {}
		virtual std::string const & GetName() const = 0;
		virtual bool Initialise() = 0;
		virtual void ShutDown() = 0;
		virtual void OutputMessage(std::string const & msg) = 0;
	};
	typedef std::shared_ptr<Outputter> OutputterPtr;

	class  DebugOutputter : public Outputter
	{
	public:
		std::string const & GetName() const;
		bool Initialise();
		void ShutDown();
		void OutputMessage(std::string const & msg);
	};

	class  FileOutputter : public Outputter
	{
	public:
		FileOutputter(std::string const & file_path);
		~FileOutputter();

		std::string const & GetName() const;
		bool Initialise();
		void ShutDown();
		void OutputMessage(std::string const & msg);

	protected:
		std::ofstream log_file_;
		std::string   path_;
	};

	class  Log : public Singleton<Log>
	{
		typedef std::vector<OutputterPtr> OutputterList;

	public:
		static Log & Instance();

		Log();
		void Add(OutputterPtr const & outputter);
		void Remove(OutputterPtr const & outputter);
		void RemoveAll();
		uint32_t OutputterNum();
		void LogMessage(std::string const & msg, LogType type = LT_Info);

	private:
		std::array<std::string,3>       log_types_;
		OutputterList                   outputters_;
		mutable std::recursive_mutex    mutex_;
	};
	typedef std::shared_ptr<Log> LogPtr;
}

#endif // Forge_Log_h__