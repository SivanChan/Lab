
#include "stdafx.h"
#include <MessagePack.h>

namespace Forge
{
	MessagePack::MessagePack()
		: begin_flag_(0x01),
		end_flag_(0x00),
		buffer_(NULL),
		pos_(0),
		content_size_(0),
		buffer_size_(0)
	{

	}

	MessagePack::~MessagePack()
	{

	}

	uint32_t MessagePack::PackSize()
	{
		return sizeof(begin_flag_) + sizeof(content_size_) + content_size_ + sizeof(end_flag_);
	}

	bool MessagePack::Encode(char* buffer, uint32_t size)
	{
		buffer_ = buffer;
		pos_    = 0;
		buffer_size_ = size;

		return true;
	}

	bool MessagePack::Decode(char const * buffer, uint32_t size)
	{
		buffer_ = (char*)buffer;
		pos_ = 0;
		buffer_size_ = size;

		return true;
	}

	template <typename T>
	bool MessagePack::SetValue(T const & data)
	{
		if (buffer_ == NULL || buffer_size_ < pos_ + sizeof(data))
			return false;
		*((T*)(buffer_ + pos_)) = data;
		pos_ += sizeof(data);
		return true;
	}

	template <typename T>
	bool MessagePack::GetValue(T & data)
	{
		if (buffer_ == NULL || buffer_size_ < pos_ + sizeof(data))
			return false;
		data = *((T*)(buffer_ + pos_));
		pos_ += sizeof(data);
		return true;
	}

	bool MessagePack::GetValue(void* data, uint32_t len)
	{
		if (buffer_ == NULL || buffer_size_ < pos_ + sizeof(data))
			return false;
		memmove_s(data, len, buffer_+pos_, len);
		pos_ += len;
		return true;
	}

	bool MessagePack::SetValue(void const * data, uint32_t len)
	{
		if (buffer_ == NULL || buffer_size_ < pos_ + sizeof(data))
			return false;
		memmove_s(buffer_ + pos_, len, data, len);
		pos_ += len;
		return true;
	}

	XMLMessagePack::XMLMessagePack()
	{

	}

	XMLMessagePack::~XMLMessagePack()
	{

	}

	void XMLMessagePack::SetXML(std::string const & xml)
	{
		xml_ = xml;
		content_size_ = xml_.length();
	}

	bool XMLMessagePack::Encode(char* buffer, uint32_t size)
	{
		MessagePack::Encode(buffer, size);

		if (!SetValue(begin_flag_)) return false;
		if (!SetValue(content_size_)) return false;
		if (!SetValue((void const *)xml_.c_str(), content_size_)) return false;
		if (!SetValue(end_flag_)) return false;

		return true;
	}

	bool XMLMessagePack::Decode(char const * buffer, uint32_t size)
	{
		MessagePack::Decode(buffer, size);

		if (!GetValue(begin_flag_)) return false;
		if (!GetValue(content_size_)) return false;
		xml_.resize(content_size_);
		if (!GetValue((void *)xml_.c_str(), content_size_)) return false;
		if (!GetValue(end_flag_)) return false;

		return true;
	}
}
