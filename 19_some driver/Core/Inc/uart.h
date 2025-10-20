/*
 * uart.h
 *
 *  Created on: Oct 12, 2025
 *      Author: eslam
 */

#ifndef INC_UART_H_
#define INC_UART_H_









void USART2_UART_Tx_Init(void);
void USART2_UART_Rx_Init(void);
int __io_putchar(int ch);
int uart2_write(int ch);


#endif /* INC_UART_H_ */
