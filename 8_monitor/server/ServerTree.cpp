
#include "stdafx.h"
#include <ServerTree.h>
#include <StringUtil.h>

namespace Forge
{
	ServerTree::ServerTree(CTreeCtrl & tree)
		: tree_(tree), root_item_(NULL)
	{
		Init();
	}

	ServerTree::~ServerTree()
	{

	}

	void ServerTree::AddItem(Session* session)
	{
		if (session && root_item_)
		{
			auto it = data_.find(session);
			if (it == data_.end())
			{
				std::wstring wstr;
				StringUtil::StringConvert(session->GetIpPortDesc(), wstr);
				HTREEITEM item = tree_.InsertItem( wstr.c_str(), root_item_ );
				tree_.SetItemData(item, (DWORD_PTR)session);
				data_.insert( std::make_pair(session,item) );

				tree_.Expand(root_item_, TVE_EXPAND);
			}
		}
	}

	void ServerTree::RemoveItem(Session* session)
	{
		if (session && root_item_)
		{
			auto it = data_.find(session);
			if (it != data_.end())
			{
				tree_.DeleteItem(it->second);
				data_.erase(it);
			}
		}
	}

	void ServerTree::Init()
	{
		root_item_ = tree_.InsertItem(L"Camera Server");
	}

}