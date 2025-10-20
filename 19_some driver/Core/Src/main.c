
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "uart.h"
#include "ADC.h"
#include "EXTI.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);


uint8_t btn_state;
uint8_t   sensor_value;


int main(void)
{
	HAL_Init();


	SystemClock_Config();
	MX_GPIO_Init();

	GPIO_init();
	ADC_init();

	printf("System Initialization");



	//xTaskCreate(vMonitorTask, "MonitorTask",256,NULL,1,NULL);

	//vTaskStartScheduler();

	while (1)
	{

		btn_state=Read_Digital_Sensor();

		sensor_value=read_analog_sensor();
	}

}









//
//// This is a new task you would add to your project
//void vMonitorTask(void *pvParameters)
//{
//	UBaseType_t uxHighWaterMark_Red, uxHighWaterMark_Yellow, uxHighWaterMark_Blue;
//
//	for(;;)
//	{
//		// Get the high water mark for each task.
//		// This is the minimum free stack space the task has ever had.
//		uxHighWaterMark_Red = uxTaskGetStackHighWaterMark(RED);
//		uxHighWaterMark_Yellow = uxTaskGetStackHighWaterMark(Yellow);
//		uxHighWaterMark_Blue = uxTaskGetStackHighWaterMark(Blue);
//
//		printf("==================================\r\n");
//		printf("Stack High Water Marks (Words):\r\n");
//		printf("RED: %lu\r\n", uxHighWaterMark_Red);
//		printf("Yellow: %lu\r\n", uxHighWaterMark_Yellow);
//		printf("Blue: %lu\r\n", uxHighWaterMark_Blue);
//		printf("==================================\r\n");
//
//		// Delay for a few seconds before checking again
//		vTaskDelay(pdMS_TO_TICKS(5000));
//	}
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
