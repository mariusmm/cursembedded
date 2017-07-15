/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void EXTI0_IRQHandler() {

	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	}
}

int main(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	HAL_Init();

	/* Config for Green LED on PD12 */
	GPIO_InitStructure.Pin = GPIO_PIN_12;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	__GPIOD_CLK_ENABLE();

	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Config for Blue button on PA0 , trigger interrupt */
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	__GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable interrupt for Button */
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	for (;;) {

	}
}
