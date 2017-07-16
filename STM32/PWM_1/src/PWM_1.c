/**
 ******************************************************************************
 * @file    PWM_1.c
 * @author  Màrius Montón
 * @version V1.0
 * @date    15-July-2017
 * @brief   Using PWM example
 * @license This project is released under the GNU Public License.
 ******************************************************************************
 */
#include <stdbool.h>

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/* Timer calculations for PWM : 16_000_000 / 1_600 / 1_000 = 10 Hz */

#define PWM_PRESCALER 1600
#define PWM_PERIOD 1000

volatile int32_t duty_cycle = PWM_PERIOD / 2;
bool increasing = true;

GPIO_InitTypeDef GPIO_InitStructure;
TIM_HandleTypeDef htim;
TIM_OC_InitTypeDef configOC;
TIM_MasterConfigTypeDef sMasterConfig;

void EXTI0_IRQHandler() {
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1) {
		/* We can be increasing or decreasing duty cycle */
		if (increasing == true) {
			duty_cycle += PWM_PERIOD / 4;
			if (duty_cycle >= PWM_PERIOD) {
				increasing = false;
				duty_cycle = PWM_PERIOD;
			}
		} else {
			duty_cycle -= PWM_PERIOD / 4;
			if (duty_cycle <= 0) {
				increasing = true;
				duty_cycle = 0;
			}
		}
	}

	/* update timer duty cycle */
	configOC.Pulse = duty_cycle;
	HAL_TIM_PWM_ConfigChannel(&htim, &configOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);
}

int main(void) {

	HAL_Init();

	SystemCoreClockUpdate();

	/* Config for Green LED on PD12 */
	GPIO_InitStructure.Pin = GPIO_PIN_12;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM4;
	__GPIOD_CLK_ENABLE();

	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Config for Blue button on PA0 */
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	__GPIOA_CLK_ENABLE();


	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable interrupt for Button */
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	/* We use TIM4 (connected to APB1 (16MHz bus) ) */
	htim.Instance = TIM4;
	htim.Init.Prescaler = PWM_PRESCALER;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = PWM_PERIOD;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	__HAL_RCC_TIM4_CLK_ENABLE();
	HAL_TIM_PWM_Init(&htim);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

	configOC.OCMode = TIM_OCMODE_PWM1;
	configOC.Pulse = duty_cycle;
	configOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	configOC.OCFastMode = TIM_OCFAST_DISABLE;
	configOC.OCIdleState = TIM_OCIDLESTATE_SET;

	HAL_TIM_PWM_ConfigChannel(&htim, &configOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);

	for (;;) {
	}
}
