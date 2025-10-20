/*
 * EXTI.c
 *
 *  Created on: Oct 12, 2025
 *      Author: eslam
 */

#include "stm32f4xx_hal.h"




void P13_interrupt_init(void)
{
	//Enable GPIOC clock

	RCC->AHB1ENR |=4;

	//Enable SYSconfig clock
	RCC->AHB2ENR |=0x4000;

	//Configure PC13 for Push button interrupt

	GPIOC->MODER &= ~ 0x0C000000;


	//Clear Port Selection for EXTI13

	SYSCFG->EXTICR[3] &= ~ 0x00F0;


	//Select Port C  for EXTI13

	SYSCFG->EXTICR[3] &= ~ 0x0020;

	//un mask EXTI13

	EXTI->IMR |=0x2000; // detect falling edge

	NVIC_SetPriority(EXTI15_10_IRQn,6);

	NVIC_EnableIRQ(EXTI15_10_IRQn);

}


void GPIO_init(void)
{
	RCC->AHB1ENR |=4; //Enable Clock access


}


uint8_t Read_Digital_Sensor(void)
{


	if (GPIOC->IDR & 0x2000)
	{
		return 1;

	}

	else
	{
		return 0;
	}
}
