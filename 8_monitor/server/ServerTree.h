
#ifndef Forge_ServerTree_hpp__
#define Forge_ServerTree_hpp__

#pragma once

#include <Session.h>

namespace Forge
{
	class ServerTree
	{
	public:
		ServerTree(CTreeCtrl & tree);
		~ServerTree();

		void AddItem(Session* session);
		void RemoveItem(Session* session);

	protected:
		void Init();

	protected:
		CTreeCtrl & tree_;
		HTREEITEM root_item_;
		std::map<Session*, HTREEITEM> data_;
	};
}

#endif // Forge_ServerTree_hpp__