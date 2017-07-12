
#include "stdafx.h"
#include <ServerTree.h>
#include <StringUtil.h>
#include <Util.h>
#include <AppFramework.h>

namespace Forge
{
	ServerTree::ServerTree(CTreeCtrl & tree)
		: tree_(tree), root_client_item_(NULL)
	{
		Init();
	}

	ServerTree::~ServerTree()
	{

	}

	void ServerTree::AddItem(Session* session)
	{
		if (session && root_client_item_)
		{
			auto it = data_.find(session);
			if (it == data_.end())
			{
				std::wstring wstr;
				StringUtil::StringConvert(session->GetIpPortDesc(), wstr);
				HTREEITEM item = tree_.InsertItem( wstr.c_str(), root_client_item_ );
				tree_.SetItemData(item, (DWORD_PTR)session);
				data_.insert( std::make_pair(session,item) );

				tree_.Expand(root_client_item_, TVE_EXPAND);
			}
		}
	}

	void ServerTree::RemoveItem(Session* session)
	{
		if (session && root_client_item_)
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
		root_client_item_ = tree_.InsertItem(L"Client");
		root_camera_item_ = tree_.InsertItem(L"Camera");

		std::wstring wstr;
		std::vector<std::string> const & ips = AppFramework::Instance().GetAppConfig().ips;
		for (auto const & ip : ips)
		{
			StringUtil::StringConvert(ip, wstr);
			HTREEITEM ht = tree_.InsertItem(wstr.c_str(), root_camera_item_);
			if (ht)
			 	tree_.SetItemData(ht, (DWORD_PTR)ip.data());
		}
		tree_.Expand(root_camera_item_, TVE_EXPAND);
	}
}