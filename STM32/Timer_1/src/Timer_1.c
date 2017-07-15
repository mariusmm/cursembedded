/**
 ******************************************************************************
 * @file    Timer_1.c
 * @author  Màrius Montón
 * @version V1.0
 * @date    15-July-2017
 * @brief
 ******************************************************************************
 */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#define TOP_VALUE 1000

int main(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_HandleTypeDef htim;

	HAL_Init();

	SystemCoreClockUpdate();

	/* Config for Green LED on PD12 */
	GPIO_InitStructure.Pin = GPIO_PIN_12;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	__GPIOD_CLK_ENABLE();
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Config for Blue button on PA0 */
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	__GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Timer config we use TIM2 (connected to APB1 (16MHz bus) ) */
	htim.Instance = TIM2;
	htim.Init.Prescaler = 16000;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 65535;
	htim.Init.RepetitionCounter = 0x0;
	__HAL_RCC_TIM2_CLK_ENABLE();
	HAL_TIM_Base_Init(&htim);

	for (;;) {
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1) {
			 HAL_TIM_Base_Start(&htim);
		}

		if  (__HAL_TIM_GET_COUNTER(&htim) >= TOP_VALUE)  {
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			HAL_TIM_Base_Stop(&htim);
			__HAL_TIM_SET_COUNTER(&htim, 0);
		}
	}
}
