/*
 * uart.c
 *
 *  Created on: Oct 12, 2025
 *      Author: eslam
 */

#include "stm32f4xx_hal.h"
#include "uart.h"

UART_HandleTypeDef huart2;

 int __io_putchar(int ch);


 void USART2_UART_Tx_Init(void)
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
	huart2.Init.Mode = UART_MODE_TX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{

	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}


 void USART2_UART_Rx_Init(void)
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
 	huart2.Init.Mode = UART_MODE_RX;
 	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
 	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
 	if (HAL_UART_Init(&huart2) != HAL_OK)
 	{

 	}
 	/* USER CODE BEGIN USART2_Init 2 */

 	/* USER CODE END USART2_Init 2 */

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

