
#include "stdafx.h"
#include <Util.h>
#include <windows.h>

namespace Forge
{
	std::string GetExeDirectory()
	{
		char buf[MAX_PATH];
		::GetModuleFileNameA(nullptr, buf, sizeof(buf));
		std::string exe_path = buf;
		exe_path = exe_path.substr(0, exe_path.rfind("\\"));
		return exe_path;
	}
}