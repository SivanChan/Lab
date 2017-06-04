
#include "stdafx.h"
#include <iostream>
#include <modbus.h>

int main()
{
	std::cout << "������ѡ��: 1 ��RTU��ʽ����Slave��2 ��TCP��ʽ����Slave" << std::endl;

	int input_value = 0;
	std::cin >> input_value;

	if (input_value == 1) // RTU
	{

	}
	else if (input_value == 2) // TCP��ʽ
	{
		modbus_t *mb;
		uint16_t tab_reg[32] = { 0 };

		mb = modbus_new_tcp("127.0.0.1", 502);
		modbus_set_slave(mb, 1);
		int i = modbus_connect(mb);

		struct timeval t;
		t.tv_sec = 0;
		t.tv_usec = 1000000;

		modbus_set_response_timeout(mb, &t);
		
		// ��ȡ�Ĵ�������
		int regs = modbus_read_registers(mb, 0, 10, tab_reg);
		std::cout << "count:" << regs << std::endl;

		for (int i=0; i<regs; ++i)
		{
			std::cout << tab_reg[i] << std::endl;
		}

		for (int i = 0; i < regs; ++i)
		{
			tab_reg[i] = 10 - i;
		}

		// д�Ĵ�������
		modbus_write_registers(mb, 0, 10, tab_reg);


		modbus_close(mb);
		modbus_free(mb);	
	}

	return 1;
}