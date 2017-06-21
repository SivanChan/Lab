
#include "stdafx.h"
#include <ListboxOutputter.h>
#include <StringUtil.h>

namespace Forge
{

	ListboxOutputter::ListboxOutputter(CListBox & list_box)
		: list_box_(list_box)
	{

	}

	ListboxOutputter::~ListboxOutputter()
	{

	}

	std::string const & ListboxOutputter::GetName() const
	{
		static std::string name("ListboxOutputter");
		return name;
	}

	bool ListboxOutputter::Initialise()
	{
		return (list_box_.GetSafeHwnd() != NULL);
	}

	void ListboxOutputter::ShutDown()
	{

	}

	void ListboxOutputter::OutputMessage(std::string const & msg)
	{
		StringUtil::StringConvert(msg, buffer_);
		list_box_.AddString(buffer_.c_str());
	}
}