#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "uart.h"
#include "adc.h"
#include "exti.h"
UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void start_rx_interrupt(void);
volatile int rxInProgress;


const int STACK_SIZE=128;
static QueueHandle_t uart2_BytesReceived=NULL;

void polledUARTReceiver(void *pvParameters);

void HandlerTask(void *pvParameters);

int main(void)
{

  HAL_Init();


  SystemClock_Config();
  MX_GPIO_Init();

  //xTaskCreate(UARTReceiver, "pollling UART", STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL);
  xTaskCreate(HandlerTask, "UART print task", STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL);
  uart2_BytesReceived=xQueueCreate(10,sizeof(char));
  vTaskStartScheduler();
  while (1)
  {

  }

}




void UARTReceiver(void *pvParameters)
	{
	uint8_t nexByte;
	USART2_UART_RX_Init();
	 start_rx_interrupt();
	while(1)
		{
		xQueueSend(uart2_BytesReceived,&nexByte,0);
		}

	}



char rcByte;

void HandlerTask(void *pvParameters)
	{

	USART2_UART_RX_Init();
	 start_rx_interrupt();

	while(1)
		{
		xQueueReceive(uart2_BytesReceived, &rcByte, portMAX_DELAY);

		}


	}





void start_rx_interrupt(void)
	{
	rxInProgress=1;
	USART2->CR1|=USART_CR1_RXNEIE;
	NVIC_SetPriority(USART2_IRQn,6);
	NVIC_EnableIRQ(USART2_IRQn);

	}




void USART2_IRQHandler(void)
	{
	portBASE_TYPE xHigherPriorityTaskWoken=pdFALSE;
	if(USART2->SR & USART_SR_RXNE)
		{
		uint8_t tempVal=(uint8_t)USART2->DR;
		if(rxInProgress)
			{
			xQueueSendFromISR(uart2_BytesReceived,&tempVal,& xHigherPriorityTaskWoken);
			}
		}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}



void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM11 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM11) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

