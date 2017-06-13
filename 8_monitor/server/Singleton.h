/********************************************************************
	created:	2014/10/19
	created:	2014/10/19   11:48
	filename: 	Singleton.h
	author:		Sivan
	purpose:	����ģʽ�ӿڣ������������������ڹ���
*********************************************************************/

#ifndef Forge_Singleton_h__
#define Forge_Singleton_h__

#pragma once

namespace Forge
{
	template<typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			assert(!singleton_);
			singleton_ = static_cast<T*>(this);
		}

		~Singleton()
		{
			assert(singleton_);
			singleton_ = 0;
		}

		static T & Instance()
		{
			assert(singleton_);
			return (*singleton_);
		}

	protected:
		static T * singleton_;

	private:
		Singleton(Singleton<T> const &);
		Singleton & operator = (Singleton<T> const &);
	};
}

#endif // Forge_Singleton_h__