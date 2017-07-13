// 12_pattern.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "decorator.hpp"

int main()
{
	using namespace Forge;

	// 1.decorator
	std::shared_ptr<Decorator> d = std::make_shared<ConcreteDecorator>(std::make_shared<ConcreateComponent>());
	d->Operation();


    return 0;
}

