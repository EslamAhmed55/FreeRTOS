
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

//Declare a Two Queues
static QueueHandle_t xQueue1=NULL,xQueue2=NULL;

//Declare a single  Queue set
static QueueSetHandle_t xQueueSet=NULL;


void vSenderTask1(void *pvParameters);
void vSenderTask2(void *pvParameters);
void vReceiverTask(void *pvParameters);
void vMonitorTask(void *pvParameters);

TaskHandle_t sender1 ,sender2,reciever;

int main(void)
{
  HAL_Init();


  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /*
   * printf won't work sometimes if you didn't enable newlib for RTOS
   * */

  printf("System is initialized  \r\n");


  //create two queues, each sends a character pointer
  xQueue1=xQueueCreate(1,sizeof(char *));
  xQueue2=xQueueCreate(1,sizeof(int));

  //create a queue set to hold 2 queues
  //Each queue hold 1 element
  xQueueSet=xQueueCreateSet(2);
  //Add two queues to queue set
  xQueueAddToSet (xQueue1,xQueueSet);
  xQueueAddToSet (xQueue2,xQueueSet);
  //Create two sender tasks with the same priority of 1
  xTaskCreate(vSenderTask1, "sender1",100,NULL,1,&sender1);
  xTaskCreate(vSenderTask2, "sender2",100,NULL,1,&sender2);
  ///Create the receiver task , with a higher priority of 2
  xTaskCreate(vReceiverTask, "Receiver",256,NULL,2,&reciever);



  xTaskCreate(vMonitorTask, "MonitorTask",256,NULL,1,NULL);

  vTaskStartScheduler();

  while (1)
  {

  }

}






int uart2_write(int ch)
	{
	/*Make sure the transmit data register is empty*/
	while(!(USART2->SR & USART_SR_TXE)){}

	 /*Write to transmit data register*/
	USART2->DR	=  (ch & 0xFF);
	return ch;
	}

int __io_putchar(int ch)
	{
	uart2_write(ch);
	return ch;
	}





void vSenderTask1(void *pvParameters)
{
	const TickType_t xBlcokTime = pdMS_TO_TICKS(100);
	const char *const msg ="Message from vSenderTask1\r\n";
	while(1)
	{
		///Block for 100ms
		vTaskDelay(xBlcokTime);
		//send the steering "msg" to xQueue1
		xQueueSend(xQueue1,&msg,0);
	}

}
void vSenderTask2(void *pvParameters)
{
	const TickType_t xBlcokTime = pdMS_TO_TICKS(300);
	//const char *const msg ="Message from vSenderTask2\r\n";
	int value=1000;
	while(1)
	{
		 ///Block for 300ms
		vTaskDelay(xBlcokTime);
		//send the steering "msg" to xQueue2
		xQueueSend(xQueue2,&value,0);

	}

}

void vReceiverTask(void *pvParameters)
{
    QueueHandle_t xActiveQueue;
    char *pcMsg;
    int iValue;

    for(;;)
    {
        xActiveQueue = (QueueHandle_t)xQueueSelectFromSet(xQueueSet, portMAX_DELAY);

        if (xActiveQueue == xQueue1)
        {
            xQueueReceive(xQueue1, &pcMsg, 0);
            printf("Got string: %s\r\n", pcMsg);
        }
        else if (xActiveQueue == xQueue2)
        {
            xQueueReceive(xQueue2, &iValue, 0);
            printf("Got number: %d\r\n", iValue);
        }
    }
}




// This is a new task you would add to your project
void vMonitorTask(void *pvParameters)
{
    UBaseType_t uxHighWaterMark_Sender1, uxHighWaterMark_Sender2, uxHighWaterMark_Receiver;

    for(;;)
    {
        // Get the high water mark for each task.
        // This is the minimum free stack space the task has ever had.
        uxHighWaterMark_Sender1 = uxTaskGetStackHighWaterMark(sender1);
        uxHighWaterMark_Sender2 = uxTaskGetStackHighWaterMark(sender2);
        uxHighWaterMark_Receiver = uxTaskGetStackHighWaterMark(reciever);

        printf("==================================\r\n");
        printf("Stack High Water Marks (Words):\r\n");
        printf("Sender 1: %lu\r\n", uxHighWaterMark_Sender1);
        printf("Sender 2: %lu\r\n", uxHighWaterMark_Sender2);
        printf("Receiver: %lu\r\n", uxHighWaterMark_Receiver);
        printf("==================================\r\n");

        // Delay for a few seconds before checking again
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}



// Add this function anywhere in your main.c file

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    // This function is called when a stack overflow is detected.

    // The pcTaskName parameter holds the name of the task that overflowed.
    // The xTask parameter is the handle of the task that overflowed.

    // You can use these to log the error.
    // WARNING: printf itself uses stack and may not be safe to call here.
    printf("!!! STACK OVERFLOW in task: %s !!!\r\n", pcTaskName);

    // Halt the system. Blinking an LED is a common way to indicate a fault.
    for(;;)
    {
        // Your code to toggle an LED, for example.
    }
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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

