
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);


void redLedControllerTask(void *pvParameter);
void yellowLedControllerTask(void *pvParameter);
void blueLedControllerTask(void *pvParameter);

void vMonitorTask(void *pvParameters);


SemaphoreHandle_t xBinarySemaphore;

TaskHandle_t RED=NULL , Yellow=NULL , Blue=NULL;

typedef int TaskProfiler;

TaskProfiler RedLEDProfiler;
TaskProfiler YellowLEDProfiler;
TaskProfiler BlueLEDProfiler;




int main(void)
{
	HAL_Init();


	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART2_UART_Init();

	printf("System Initialization");



	xBinarySemaphore=xSemaphoreCreateBinary();

	xSemaphoreGive(xBinarySemaphore);


	xTaskCreate(redLedControllerTask, "Red LED Task",256,NULL,1,&RED);
	xTaskCreate(yellowLedControllerTask, "Yellow LED Task",256,NULL,1,&Yellow);
	xTaskCreate(blueLedControllerTask, "Blue LED Task",256,NULL,1,&Blue);


	//xTaskCreate(vMonitorTask, "MonitorTask",256,NULL,1,NULL);

	vTaskStartScheduler();

	while (1)
	{

	}

}



void redLedControllerTask(void *pvParameter)
{



	while(1)
	{
		//printf("Red Task: Trying to take semaphore...\r\n");

		xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);

		//printf("Red Task: >> Got semaphore! Incrementing.\r\n"); // This is the important one

		RedLEDProfiler++;

		printf("RedLEDProfiler= %d \r\n",RedLEDProfiler); // This is the important one

		xSemaphoreGive(xBinarySemaphore);

		vTaskDelay(1);
		/*
		 * Excellent! Adding vTaskDelay(1) is the classic solution to this problem.
		 *  Let's break down exactly what was happening and why your solution worked.

			The Problem: CPU Starvation
T			he issue you discovered is a classic RTOS problem called CPU Starvation or Task Starvation.

			Even though your tasks were all the same priority, one task (redLedControllerTask) was running in such a tight,
		    fast loop that it never gave the other tasks a chance to run. It would Give the semaphore and immediately loop back to Take it again before the FreeRTOS scheduler had an opportunity to switch to the yellow or blue tasks.

			Think of it like a person at a buffet who takes a cookie, and instead of stepping aside,
			 immediately grabs the next one, and the next, never letting anyone else in line get a turn.

			The vTaskDelay(1) call fixes this by forcing the task to "step aside.
			" It tells the scheduler: "Put me in the Blocked state for one tick." When a task enters the Blocked state,
			the scheduler is forced to look for the next highest priority task that is Ready to run, which gave your other tasks their turn.
		 */



		/* explain the best method to avoid it in the future and without waste the time in blocking state*/

		/*
		 * Excellent! Adding vTaskDelay(1) is the classic solution to this problem. Let's break down exactly what was happening and why your solution worked.

			The Problem: CPU Starvation
			The issue you discovered is a classic RTOS problem called CPU Starvation or Task Starvation.

			Even though your tasks were all the same priority, one task (redLedControllerTask) was running in such a tight,
			 fast loop that it never gave the other tasks a chance to run. It would Give the semaphore and immediately loop back to Take it again before the FreeRTOS scheduler had an opportunity to switch to the yellow or blue tasks.

			Think of it like a person at a buffet who takes a cookie, and instead of stepping aside,
			immediately grabs the next one, and the next, never letting anyone else in line get a turn.

			The vTaskDelay(1) call fixes this by forcing the task to "step aside." It tells the scheduler: "Put me in the Blocked state for one tick."
			 When a task enters the Blocked state, the scheduler is forced to look for the next highest priority task that is Ready to run, which gave your other tasks their turn.

			The Best Method: An Event-Driven Approach
			This is an excellent question, and it shows you're thinking about efficiency.
			You are correct that calling vTaskDelay() forces the task to "waste time" in a blocking state. While effective, it's not the most efficient or elegant solution for sequencing.

			A more advanced and efficient method is to design your system to be event-driven, where tasks spend almost all their time blocked, waiting for a specific signal to do their work.

			Instead of having all tasks compete for one semaphore, you could implement a Gatekeeper Task:

			Create a "Gatekeeper" Task: This task runs at a higher priority (e.g., priority 2) and its only job is to manage the sequence.

			Give Each Worker Its Own Semaphore: Create three separate binary semaphores: xRedSemaphore, xYellowSemaphore, and xBlueSemaphore.

			Centralize the Logic: The Gatekeeper's loop would look like this:
		 */

	}
}



void blueLedControllerTask(void *pvParameter)
{


	while(1)
	{

		xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);

		BlueLEDProfiler++;
		printf("BlueLEDProfiler= %d \r\n",BlueLEDProfiler); // This is the important one

		xSemaphoreGive(xBinarySemaphore);
		vTaskDelay(1);

	}
}



void yellowLedControllerTask(void *pvParameter)
{


	while(1)
	{

		xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);

		YellowLEDProfiler++;
		printf("YellowLEDProfiler= %d \r\n",YellowLEDProfiler); // This is the important one

		xSemaphoreGive(xBinarySemaphore);
		vTaskDelay(1);
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







// This is a new task you would add to your project
void vMonitorTask(void *pvParameters)
{
	UBaseType_t uxHighWaterMark_Red, uxHighWaterMark_Yellow, uxHighWaterMark_Blue;

	for(;;)
	{
		// Get the high water mark for each task.
		// This is the minimum free stack space the task has ever had.
		uxHighWaterMark_Red = uxTaskGetStackHighWaterMark(RED);
		uxHighWaterMark_Yellow = uxTaskGetStackHighWaterMark(Yellow);
		uxHighWaterMark_Blue = uxTaskGetStackHighWaterMark(Blue);

		printf("==================================\r\n");
		printf("Stack High Water Marks (Words):\r\n");
		printf("RED: %lu\r\n", uxHighWaterMark_Red);
		printf("Yellow: %lu\r\n", uxHighWaterMark_Yellow);
		printf("Blue: %lu\r\n", uxHighWaterMark_Blue);
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

