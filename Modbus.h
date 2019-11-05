/*
 * Copyright © 2018-2019 wenion <wenion@qq.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PERIPHERAL_MODBUS_H_
#define PERIPHERAL_MODBUS_H_

#include <modbus/modbus.h>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus-version.h>

#include <memory>
#include <string>

namespace modbus{

class Modbus {
public:
	Modbus(std::string device,
		int baud,
		char parity,
		int data_bit,
		int stop_bit);

	Modbus(std::string ip,
		int port);

	bool open();

	inline int set_slave(int slave);
	inline int set_debug(bool flag);

	void close();
	/*void free();*/

	inline int modbus_read_bits(int addr, int nb, uint8_t* dest);
	inline int modbus_read_input_bits(int addr, int nb, uint8_t* dest);

	inline int modbus_read_registers(int addr, int nb, uint16_t* dest);
	inline int modbus_read_input_registers(int addr, int nb, uint16_t* dest);

	inline int modbus_write_bit(int coil_addr, int status);
	inline int modbus_write_bits(int addr, int nb, const uint8_t* data);

	inline int modbus_write_register(int reg_addr, int value);
	inline int modbus_write_registers(int addr, int nb, const uint16_t* data);

	bool connected();
	
private:
	std::shared_ptr<modbus_t> node;
	bool isConnected;
};

inline int Modbus::modbus_read_bits(int addr, int nb, uint8_t* dest)
{
	return ::modbus_read_bits(node.get(), addr, nb, dest);
}


inline int Modbus::modbus_read_input_bits(int addr, int nb, uint8_t* dest)
{
	return ::modbus_read_input_bits(node.get(), addr, nb, dest);
}


inline int Modbus::modbus_read_registers(int addr, int nb, uint16_t* dest)
{
	return ::modbus_read_registers(node.get(), addr, nb, dest);
}

inline int Modbus::modbus_read_input_registers(int addr, int nb, uint16_t* dest)
{
	return ::modbus_read_input_registers(node.get(), addr, nb, dest);
}


inline int Modbus::modbus_write_bit(int coil_addr, int status)
{
	return ::modbus_write_bit(node.get(), coil_addr, status);
}


inline int Modbus::modbus_write_bits(int addr, int nb, const uint8_t* data)
{
	return ::modbus_write_bits(node.get(), addr, nb, data);
}


inline int Modbus::modbus_write_register(int reg_addr, int value)
{
	return ::modbus_write_register(node.get(), reg_addr, value);
}


inline int Modbus::modbus_write_registers(int addr, int nb, const uint16_t* data)
{
	return ::modbus_write_registers(node.get(), addr, nb, data);
}

void deleter(modbus_t* ctx)
{
	modbus_close(ctx);
	modbus_free(ctx);
}

Modbus::Modbus(std::string device,
	int baud,
	char parity,
	int data_bit,
	int stop_bit):
	node(modbus_new_rtu(device.c_str(), baud, parity, data_bit, stop_bit), deleter)
{
}


Modbus::Modbus(std::string ip, int port):node(modbus_new_tcp(ip.c_str(), port), deleter)
{
}


bool Modbus::open()
{
	if (modbus_connect(node.get()) != -1) {
		isConnected = true;
	}
	else {
		isConnected = false;
	}
	return isConnected;
}

inline int Modbus::set_slave(int slave)
{
	return modbus_set_slave(node.get(), slave);
}

inline int Modbus::set_debug(bool flag)
{
	return modbus_set_debug(node.get(), flag);
}

void Modbus::close()
{
	if (!isConnected) {
		modbus_close(node.get());
		isConnected = false;
	}
}

bool Modbus::connected()
{
	return isConnected;
}

}

#endif /* PERIPHERAL_MODBUS_H_ */