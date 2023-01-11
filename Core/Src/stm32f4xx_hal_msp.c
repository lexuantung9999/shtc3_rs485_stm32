/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef hdma_uart4_rx;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  if(huart->Instance==UART4)
	  {
	  /* USER CODE BEGIN UART4_MspInit 0 */

	  /* USER CODE END UART4_MspInit 0 */
	    /* Peripheral clock enable */
	    __HAL_RCC_UART4_CLK_ENABLE();

	    __HAL_RCC_GPIOC_CLK_ENABLE();
	    /**UART4 GPIO Configuration
	    PC10     ------> UART4_TX
	    PC11     ------> UART4_RX
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
	    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	    /* UART4 DMA Init */
	    /* UART4_RX Init */
	    hdma_uart4_rx.Instance = DMA1_Stream2;
	    hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
	    hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	    hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	    hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
	    hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	    hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	    hdma_uart4_rx.Init.Mode = DMA_CIRCULAR;
	    hdma_uart4_rx.Init.Priority = DMA_PRIORITY_LOW;
	    hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	    if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
	    {
	      Error_Handler();
	    }

	    __HAL_LINKDMA(huart,hdmarx,hdma_uart4_rx);

	  /* USER CODE BEGIN UART4_MspInit 1 */

	  /* USER CODE END UART4_MspInit 1 */
	    /* UART4 interrupt Init */
	    HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(UART4_IRQn);
	  }
}



/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
