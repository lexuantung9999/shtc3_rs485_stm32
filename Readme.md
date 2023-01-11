# Setup shtc3 rs485 to your own project:
Step1: Copy shtc3 library in your source, `shtc3_rs485.cpp`, `shtc3_rs485.h`

Step 2: Modify function in `stm32f4xx_hal_msp.c`, delete the configuration of UART4 generated from cubeMX, this config has been created in SHTC3 class (see `SHTC3::Init` function for more information)

```
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
```

Step 3: Example in main.cpp:

``` 
SHTC3 SHTC_sensor;
float temp, humid;

int main(){
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    SHTC_sensor.Init();
    while (1)
  {
	  SHTC_sensor.getData();
	  temp = SHTC_sensor.readTemperature();
	  humid = SHTC_sensor.readHumidity();
	  HAL_Delay(1000);
  }
}
```