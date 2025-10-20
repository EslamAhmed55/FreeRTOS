
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);


void prvOneShotTimerCallBack(TimerHandle_t xTimer);
void prvAutoReloadTimerCallBack(TimerHandle_t xTimer);




#define mainONE_SHOT_TIMER_PERIOD  (pdMS_TO_TICKS(4000UL))
#define mainAUTO_RELOAD_TIMER_PERIOD  (pdMS_TO_TICKS(500UL))

TimerHandle_t xAutoReloadTimer , xOneShotTimer;


BaseType_t xTimerStarted , xTimer2Started;

int msg[100];


int main(void)
{
	HAL_Init();


	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	/*
	 * printf won't work sometimes if you didn't enable newlib for RTOS
	 * */

	snprintf(msg, sizeof(msg), "System is initilized \r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);



	xAutoReloadTimer= xTimerCreate("AutoReload",
			mainAUTO_RELOAD_TIMER_PERIOD,
			pdTRUE,//Because use Auto Reload Timer
			0,
			prvAutoReloadTimerCallBack);





	xOneShotTimer= xTimerCreate("OneShotTimer",
			mainONE_SHOT_TIMER_PERIOD,
			pdFALSE,//Because use one shot Timer
			0,
			prvOneShotTimerCallBack);


	xTimerStarted=xTimerStart(xAutoReloadTimer,0);
	xTimerStarted=xTimerStart(xOneShotTimer,0);


	vTaskStartScheduler();

	while (1)
	{

	}

}


void prvOneShotTimerCallBack(TimerHandle_t xTimer)
{


	static TickType_t xTimeNow;

	//obtain the current tick count

	xTimeNow=xTaskGetTickCount();


	snprintf(msg, sizeof(msg),"One Shot Timer Callback executing :%d  \r\n",(int)xTimeNow);
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);




	/*
	 *
	 * We we must make sure we don't put a while(1)	 loop here..
`
	This is a callback function is supposed to run to completion his thoughts and then it completes.
	 */

}

uint32_t time_out_count=0;
const uint32_t STOP_MARK=10;

void prvAutoReloadTimerCallBack(TimerHandle_t xTimer)
{



	static TickType_t xTimeNow;

	//obtain the current tick count

	xTimeNow=xTaskGetTickCount();


	snprintf(msg, sizeof(msg),"Auto-Reload Timer Callback executing :%d  \r\n",(int)xTimeNow);
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	/*
	 *
	 * We we must make sure we don't put a while(1)	 loop here..
`
	This is a callback function is supposed to run to completion his thoughts and then it completes.
	 */


	time_out_count++;
	if (time_out_count==STOP_MARK)
	{
		snprintf(msg, sizeof(msg),"Stop Mark Reached  :%d  \r\n",(int)xTimeNow);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

		xTimerStop(xAutoReloadTimer,0);
	}

}









//
//
//// This is a new task you would add to your project
//void vMonitorTask(void *pvParameters)
//{
//    UBaseType_t uxHighWaterMark_Sender1, uxHighWaterMark_Sender2, uxHighWaterMark_Receiver;
//
//    for(;;)
//    {
//        // Get the high water mark for each task.
//        // This is the minimum free stack space the task has ever had.
//        uxHighWaterMark_Sender1 = uxTaskGetStackHighWaterMark(sender1);
//        uxHighWaterMark_Sender2 = uxTaskGetStackHighWaterMark(sender2);
//        uxHighWaterMark_Receiver = uxTaskGetStackHighWaterMark(reciever);
//
//        printf("==================================\r\n");
//        printf("Stack High Water Marks (Words):\r\n");
//        printf("Sender 1: %lu\r\n", uxHighWaterMark_Sender1);
//        printf("Sender 2: %lu\r\n", uxHighWaterMark_Sender2);
//        printf("Receiver: %lu\r\n", uxHighWaterMark_Receiver);
//        printf("==================================\r\n");
//
//        // Delay for a few seconds before checking again
//        vTaskDelay(pdMS_TO_TICKS(5000));
//    }
//}
//


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

