/** 
	数据包
*/
#ifndef MessagePack_h__
#define MessagePack_h__

#pragma once

namespace Forge
{
	class MessagePack
	{
	public:
		MessagePack();
		virtual ~MessagePack();

		/** 包大小*/
		virtual uint32_t PackSize();

		/** 编码*/
		virtual bool Encode(char* buffer, uint32_t size);

		/** 解码*/
		virtual bool Decode(char const * buffer, uint32_t size);

	protected:
		bool GetValue(void* data, uint32_t len);
		bool SetValue(void const * data, uint32_t len);

		template <typename T>
		bool SetValue(T const & data);

		template <typename T>
		bool GetValue(T & data);

	protected:
		/** 起始标志*/
		uint8_t begin_flag_;

		/** 结束标志*/
		uint8_t end_flag_;

		/** 缓冲区*/
		char *   buffer_;

		/** 当前位置*/
		uint32_t pos_;

		/** 缓冲区大小*/
		uint32_t buffer_size_;

		/** 内容长度*/
		uint32_t content_size_;  
	};

	class XMLMessagePack : public MessagePack
	{
	public:
		XMLMessagePack();
		~XMLMessagePack();

		void SetXML(std::string const & xml);
		std::string const & GetXML() const;

		/** 编码*/
		virtual bool Encode(char* buffer, uint32_t size);

		/** 解码*/
		virtual bool Decode(char const * buffer, uint32_t size);

	protected:
		std::string xml_;
	};
}

#endif // MessagePack_h__