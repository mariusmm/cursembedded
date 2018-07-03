/**
 ******************************************************************************
 * @file    ErrorHandling.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    05-July-2017
 * @brief   Error Handling example
 * @license This project is released under the GNU Public License
 * ******************************************************************************
 */

#include<stdio.h>
#include "em_device.h"
#include "em_chip.h"

#include "BSP.h"

#define DEBUG_BREAK __ASM volatile("BKPT #01")

void WrongfunctionDiv0() {
	int a = 10;
	int b = 0;
	volatile int c;
	c = a / b;
}

void WrongfunctionAlign() {
	volatile unsigned int* p;
	volatile unsigned int n;

	p = (unsigned int*) 0x00000001;
	n = *p;
}

void WrongfunctionWrongMemory() {
	volatile unsigned int* p;
	volatile unsigned int n;

	p = (unsigned int*) 0xCCCCCCCC;
	n = *p;
}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	setupSWOForPrint();

	BSP_Init();

	/* Function that points to 0 */
	void (*fp)(void) = (void (*)(void))(0x00000000);


	/* Enable DIV_0 Trap */
	SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;

	/* Enable Unaligned access Trap */
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;

	SCB->CCR |= SCB_CCR_USERSETMPEND_Msk;


	WrongfunctionDiv0();
//	WrongfunctionAlign();
//	WrongfunctionWrongMemory();
//	fp();

	/* Infinite loop */
	while (1) {

	}
}

__attribute__( (naked) )
void HardFault_Handler(void) {
	__asm volatile
	("tst lr, #4                                    \n"
	 "ite eq                                        \n"
	 "mrseq r0, msp                                 \n"
	 "mrsne r0, psp                                 \n"
	 "ldr r1, debugHardfault_address                \n"
	 "bx r1                                         \n"
	 "debugHardfault_address: .word my_HardFault_Handler  \n"
	);
}

void my_HardFault_Handler(uint32_t *stack) {

	printf("Error Handler\r\n");
	printf("SCB->HFSR = 0x%08lx\r\n", (uint32_t) SCB->HFSR);

	if ((SCB->HFSR & (1 << 30)) != 0) {
		printf("Forced Hard Fault\r\n");
		printf("SCB->CFSR = 0x%08lx\r\n", SCB->CFSR);

		if ((SCB->CFSR & 0x02000000) != 0) {
			printf("Divide by zero\r\n");
		}
		if ((SCB->CFSR & 0x01000000) != 0) {
			printf("Unaligned\r\n");
		}
		if ((SCB->CFSR & 0x00010000) != 0) {
			printf("Undefined\r\n");
		}
		if ((SCB->CFSR & 0x00020000) != 0) {
			printf("Invalid State\r\n");
		}
		if ((SCB->CFSR & 0x00040000) != 0) {
			printf("Invalid PC\r\n");
		}
		if ((SCB->CFSR & 0x00080000) != 0) {
			printf("No Coprocessor\r\n");
		}
		if ((SCB->CFSR & 0x00000200) != 0) {
			printf("Imprecise data bus error\r\n");
		}
		if ((SCB->CFSR & 0x00000400) != 0) {
			printf("Precise data bus error\r\n");
		}
	}

	if ((SCB->CFSR & 0x00008000) != 0) {
		printf("SCB->BFAR = 0x%08lx\r\n", SCB->BFAR);
	}

	if ((SCB->HFSR & (1 << 1)) != 0) {
		printf("Vector table read error\r\n");
	}
	printf("sp = 0x%08lX\r\n", (uint32_t) stack);
	printf("r0 = 0x%08lX\r\n", stack[0]);
	printf("r1 = 0x%08lX\r\n", stack[1]);
	printf("r2 = 0x%08lX\r\n", stack[2]);
	printf("r3 = 0x%08lX\r\n", stack[3]);
	printf("r12 = 0x%08lX\r\n", stack[4]);
	printf("lr = 0x%08lX\r\n", stack[5]);
	printf("pc = 0x%08lX\r\n", stack[6]);
	printf("psr = 0x%08lX\r\n", stack[7]);

	DEBUG_BREAK;

	while (1) {
		LedToggle();

	}
}

void UsageFault_Handler(void) {
	__ASM volatile("BKPT #01");
}

void BusFault_Handler(void) {
	__ASM volatile("BKPT #01");

	while (1) {
		LedToggle();

	}
}
