//	INCLUDES

	#include "stm32f0xx.h"
	#include <stdint.h>

//	FUNCTION PROTOTYPES

	void init_Timer();
	void init_adc();

/*
Abstract
--------
The code was written for the STM32F0. The function of the code is to produce two PWM signals - with one being inverted.
The PWM signal are produces using timer 2 at PB10 and PB11. The resulting singals have a 3.3V peak to peak voltage at a
frequency of 100kHz.
*/

int main(void)
{
	init_Timer(offset);
	init_adc();

	/* Infinite loop */
	while (1)
	{
    \\ ADC used to change the duty cycle.
		TIM2->CCR3 =  (((ADC1-> DR)*0.1)).80; // PB10
		TIM2->CCR4 =  (((ADC1-> DR)*0.1)).80; // PB11
		}
}

//	FUNCTIONS

	void init_Timer()
	{
		RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;  // enable clock PORT B.
		RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN; // enable clock for TIM2. 
		
		GPIOB -> MODER |= GPIO_MODER_MODER10_1; // set PB10 to AF mode.
		GPIOB -> MODER |= GPIO_MODER_MODER11_1; // set PB11 to AF mode.

		GPIOB -> AFR[1] |= 0x200;   // set PB10 to AF2.
		GPIOB -> AFR[1] |= 0x2000;  // set PB11 to AF2.

		TIM2->ARR = 79; // f = 100kHz.
		
		// specify PWM mode: OCxM bits in CCMRx.
		TIM2->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // select PWM Mode 1 for PB10.
		TIM2->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // select PWM Mode 1 for PB11.

		TIM2->CCER |= TIM_CCER_CC4P;  // set channel 4 as active low

		// set PWM percentages
		TIM2->CCR3 = 0 * .80; // PB10.
		TIM2->CCR4 = 0 * .80; // PB11.

		// enable the OC channels.
		TIM2->CCER |= TIM_CCER_CC3E; // PB10.
		TIM2->CCER |= TIM_CCER_CC4E; // PB11.
        	
		TIM2 -> CR1 |= TIM_CR1_CEN; // enable counter for TIM2.
	}

	void init_adc()
	{
		RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
		ADC1->CFGR1 |= ADC_CFGR1_CONT;
		ADC1->CFGR1 |= ADC_CFGR1_RES_0; 	// 10 BIT
		ADC1->CHSELR |= ADC_CHSELR_CHSEL5;
		ADC1->CR |=ADC_CR_ADEN;
		while ((ADC1->ISR &= 1<<0)==0){}
		ADC1->CR |= ADC_CR_ADSTART;
		while ((ADC1->ISR &=1<<2)==0){}
	}
