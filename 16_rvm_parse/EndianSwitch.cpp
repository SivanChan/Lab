
#include "stdafx.h"
#include "EndianSwitch.h"

template <>
void EndianSwitch<2>(void* p)
{
	std::uint8_t* bytes = static_cast<std::uint8_t*>(p);
	std::swap(bytes[0], bytes[1]);
}

template <>
void EndianSwitch<4>(void* p)
{
	std::uint8_t* bytes = static_cast<std::uint8_t*>(p);
	std::swap(bytes[0], bytes[3]);
	std::swap(bytes[1], bytes[2]);
}

template <>
void EndianSwitch<8>(void* p)
{
	std::uint8_t* bytes = static_cast<std::uint8_t*>(p);
	std::swap(bytes[0], bytes[7]);
	std::swap(bytes[1], bytes[6]);
	std::swap(bytes[2], bytes[5]);
	std::swap(bytes[3], bytes[4]);
}