
#ifndef Forge_decorator_hpp__
#define Forge_decorator_hpp__

#pragma once

#include <iostream>
#include <memory>

namespace Forge
{
	class Component 
	{
	public:
		virtual void Operation() = 0;
	};
	typedef std::shared_ptr<Component> ComponentPtr;

	class ConcreateComponent : public Component 
	{
	public:
		void Operation()
		{
			std::cout << "ConcreateComponent Operation" << std::endl;
		}
	};

	class Decorator : public Component 
	{
	public:
		Decorator(ComponentPtr const & c)
			: component_(c)
		{
		}

	protected:
		ComponentPtr component_;
	};

	class ConcreteDecorator : public Decorator
	{
	public:
		ConcreteDecorator(ComponentPtr const & c)
			: Decorator(c)
		{

		}

		void Operation()
		{
			std::cout << "ConcreteDecorator Operation begin" << std::endl;
			if (component_)
				component_->Operation();
			std::cout << "ConcreteDecorator Operation end" << std::endl;
		}
	};
}

#endif // Forge_decorator_hpp__