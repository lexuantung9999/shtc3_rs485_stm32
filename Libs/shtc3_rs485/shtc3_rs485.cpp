/*

shtc3_rs485.cpp

Created on: Jan 11, 2023

Author: tung19999

*/

#include "shtc3_rs485.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"

UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_uart4_rx;
uint8_t u4_rx_buffer[SHTC3_BUFF_SIZE];

SHTC3::SHTC3(uint8_t baud, uint8_t addr)
{
_baud = baud;
_addr = addr;
}
/* ----------------------------- Initialization ---------------------------- */
void SHTC3::SHTC3_DMA_Init(void){
	__HAL_RCC_DMA1_CLK_ENABLE();

	  /* DMA interrupt init */
	  /* DMA1_Stream2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
}

void SHTC3::SHTC3_UART4_Init(void){
	  huart4.Instance = UART4;
	  huart4.Init.BaudRate = baudrate;
	  huart4.Init.WordLength = UART_WORDLENGTH_8B;
	  huart4.Init.StopBits = UART_STOPBITS_1;
	  huart4.Init.Parity = UART_PARITY_NONE;
	  huart4.Init.Mode = UART_MODE_TX_RX;
	  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart4) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
void SHTC3::Init(void)
{
	SHTC3_DMA_Init();
	SHTC3_UART4_Init();
	HAL_UARTEx_ReceiveToIdle_DMA(&huart4, u4_rx_buffer, SHTC3_BUFF_SIZE);
	__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
}

void SHTC3::setTimeout(uint16_t timeOut)
{
	if (timeOut < MIN_SHTC3_TIMEOUT)
	{
	_timeOut = MIN_SHTC3_TIMEOUT;
	}
	else if (timeOut > MAX_SHTC3_TIMEOUT)
	{
	_timeOut = MAX_SHTC3_TIMEOUT;
	}
	else
	_timeOut = timeOut;
}

void SHTC3::sendReadAllData()
{
	for(int i=0;i<8;i++){
		HAL_UART_Transmit(&huart4, &send_get_all_value[i], 1, MIN_SHTC3_TIMEOUT);
	}

}
/* --------------------------------- Struct -------------------------------- */
dataSHTC3 value;
void SHTC3::getData()
{
	/* Remove all previous junk data (if have) */
	this -> sendReadAllData();
	/* Number of bytes to respond
	Addr : 1 Byte
	Cmd : 1 Byte (0x03) Resp : 1 Byte (0x04)
	Temp : 2 Byte [H_Temp] [L_Temp] Humi : 2 Byte [H_Humi] [L_Humi]
	** Crc : 2 Byte [L_CRC] [H_CRC] */
	value.temperatureC = (u4_rx_buffer[5] << 8 | u4_rx_buffer[6]);
	if(value.temperatureC >= 32768.0) // Negative value
	{
	value.temperatureC = value.temperatureC - 65536.0;
	}
	value.temperatureC = value.temperatureC / 10.0;
	value.humidity = (u4_rx_buffer[3] << 8 | u4_rx_buffer[4]) / 10.0;
}

/* ------------------------------- Read Data ------------------------------- */

float SHTC3::readTemperature()
{
	return value.temperatureC;
}

float SHTC3::readHumidity()
{
	return value.humidity;
}

/* --------------------------- Read Configuration -------------------------- */

uint16_t SHTC3::readBaudrate()
{
	return 0;
}

uint8_t SHTC3::readAddress()
{
	return 0;
}

/* ------------------------- Configuration Settings ------------------------ */

bool SHTC3::setBaudrate(uint8_t baud)
{
	return 0;
}


