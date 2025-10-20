/*
 * ADC.c
 *
 *  Created on: Oct 12, 2025
 *      Author: eslam
 */


#include "stm32f4xx_hal.h"
#include "ADC.h"



//void ADC_init(void)
//{
//
//	//enable RCC for ADC
//	RCC->AHB1ENR=(1U<<0); //Enable clock for GPIO
//
//	RCC->APB2ENR=(1U<<8);//Enable clock for ADC
//
//	GPIOA->MODER=0xC; // 1100 set bit number 2 and 3 by 1 to config  PA1 as analog
//
//
//	ADC1->CR2=0;    //SW Trigger
//	ADC1->SQR3=1;	//Conversion Sequence Start at channel 1
//	ADC1->SQR1=0;	// Conversion Sequence Length 1
//	ADC1->CR2 |=1;  // Enable ADC1
//
//
//
//}



void ADC_init(void)
{
    RCC->AHB1ENR = (1U<<0);
    RCC->APB2ENR = (1U<<8);

    GPIOA->MODER = 0xC;

    ADC1->CR2 = 0;
    ADC1->SQR3 = 1;
    ADC1->SQR1 = 0;
    ADC1->CR2 |= 1;
}


uint32_t read_analog_sensor(void)
{

	ADC1->CR2 |=(1<<30); //start ADC conversion

	while(!(ADC1->SR & 2)){} //wait for conversion to complet

	return ADC1->DR; //return result
}

