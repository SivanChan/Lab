
#ifndef Forge_ListboxOutputter_hpp__
#define Forge_ListboxOutputter_hpp__

#pragma once

#include <Log.h>
#include <Windows.h>
#include <afxwin.h>

namespace Forge
{
	class ListboxOutputter : public Outputter
	{
	public:
		ListboxOutputter(CListBox & list_box);
		~ListboxOutputter();

		std::string const & GetName() const;
		bool Initialise();
		void ShutDown();
		void OutputMessage(std::string const & msg);

	protected:
		CListBox & list_box_;
		std::wstring buffer_;
	};
}

#endif // Forge_ListboxOutputter_hpp__