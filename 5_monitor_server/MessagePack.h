/** 
	���ݰ�
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

		/** ����С*/
		virtual uint32_t PackSize();

		/** ����*/
		virtual bool Encode(char* buffer, uint32_t size);

		/** ����*/
		virtual bool Decode(char const * buffer, uint32_t size);

	protected:
		bool GetValue(void* data, uint32_t len);
		bool SetValue(void const * data, uint32_t len);

		template <typename T>
		bool SetValue(T const & data);

		template <typename T>
		bool GetValue(T & data);

	protected:
		/** ��ʼ��־*/
		uint8_t begin_flag_;

		/** ������־*/
		uint8_t end_flag_;

		/** ������*/
		char *   buffer_;

		/** ��ǰλ��*/
		uint32_t pos_;

		/** ��������С*/
		uint32_t buffer_size_;

		/** ���ݳ���*/
		uint32_t content_size_;  
	};

	class XMLMessagePack : public MessagePack
	{
	public:
		XMLMessagePack();
		~XMLMessagePack();

		void SetXML(std::string const & xml);
		std::string const & GetXML() const;

		/** ����*/
		virtual bool Encode(char* buffer, uint32_t size);

		/** ����*/
		virtual bool Decode(char const * buffer, uint32_t size);

	protected:
		std::string xml_;
	};
}

#endif // MessagePack_h__