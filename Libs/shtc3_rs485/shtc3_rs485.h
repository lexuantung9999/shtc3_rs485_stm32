
#ifndef SHTC3RS485_SHTC3_RS485_H
#define SHTC3RS485_SHTC3_RS485_H

#include "stdio.h"

#define BAUD_2400 0x00
#define BAUD_4800 0x01
#define BAUD_9600 0x02
#define INVALID_BAUD 0xFF
#define MAX_SHTC3_TIMEOUT (20000) // Unit (ms)
#define MIN_SHTC3_TIMEOUT (100) // Unit (ms)
#define SHTC3_BUFF_SIZE 8

struct dataSHTC3
{
	float temperatureC;
	float humidity;
};

class SHTC3
{
private:
	/* Command frame format structure: 8 bytes
	Address : 1 Byte
	Command : 1 Byte Register : 2 Byte
	Data : 2 Byte L_CRC : 1 Byte
	H_CRC : 1 Byte */
	uint8_t send_get_all_value[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B}; // 0x xx 03 00 00 00 02 xx xx
	uint8_t send_get_temp[8] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA};
	uint8_t send_get_humid[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
	uint8_t setBaud[8] = {0x01, 0x06, 0x07, 0xD1, 0x00, 0x00, 0x00, 0x00}; // 0x xx 06 07 D1 xx xx xx xx
	uint8_t getBaud[8] = {0x01, 0x03, 0x07, 0xD1, 0x00, 0x01, 0x00, 0x00}; // 0x xx 03 07 D1 00 01 xx xx
	uint8_t setAddr[8] = {0x01, 0x06, 0x07, 0xD0, 0x00, 0x00, 0x00, 0x00}; // 0x xx 06 07 D0 xx xx xx xx
	uint8_t getAddr[8] = {0xFF, 0x03, 0x07, 0xD0, 0x00, 0x01, 0x91, 0x59}; // 0x FF 03 07 D0 00 01 91 59

	uint16_t baudrate = 4800;
	uint8_t _baud;
	uint8_t _addr;
	uint16_t _timeOut = MIN_SHTC3_TIMEOUT;
	void SHTC3_DMA_Init(void);
	void SHTC3_UART4_Init(void);

public:
	SHTC3(uint8_t baud = BAUD_4800, uint8_t addr = 0x01); // HardwareSerial UART4 (RX=C11) (TX=C10)

	/* Initialization */
	void Init();
	void setTimeout(uint16_t timeOut);
	void sendReadAllData();
	/* Struct */
	void getData();

	/* Read Data */
	float readTemperature();
	float readHumidity();

	/* Read Configuration */
	uint16_t readBaudrate();
	uint8_t readAddress();

	/* Configuration Settings (return "TRUE" when success) */
	bool setBaudrate(uint8_t baud);
	bool setAddress(uint8_t addr);
};

#endif /* SHTC3RS485_SHTC3_RS485_H */
