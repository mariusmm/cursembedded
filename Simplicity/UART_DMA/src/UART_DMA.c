/**
 ******************************************************************************
 * @file    UART_DMA.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    11-April-2018
 * @brief   UART + DMA example
 * @license This project is released under the GNU Public License.
 *
 * Using USART1 #1 on PD0 (TX) & PD1 (RX) on Expansion header pins 4 & 6.
 * Serial port parameters are: 115200 bps, 8 databits, no parity, 1 stop bit.
 *
 * This example receives a character and outputs the same character
 * plus a string.
 * The TX function is using DMA transfer to send the TX buffer to the USART
 * The RX functionality is the same as in UART_2 example
 * (https://github.com/mariusmm/cursembedded/tree/master/Simplicity/UART_2)
 *
 * ******************************************************************************
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "em_dma.h"
#include "dmactrl.h"

#include "CircularBuffer.h"

#define DMA_USART_TX_CHANNEL 0

USART_InitAsync_TypeDef usart_cfg = USART_INITASYNC_DEFAULT;

static CircularBuffer_t RX_SerialBuffer;

void USART1_RX_IRQHandler(void) {
	char data;

	if (USART1->IF & LEUART_IF_RXDATAV) {
		data = USART_Rx(USART1);
		PushData(data, &RX_SerialBuffer);
		USART_IntClear( USART1, USART_IEN_RXDATAV);
	}
}

void USART_Init(USART_TypeDef *usart) {

	USART_InitAsync_TypeDef init = { usartEnable, /* Enable RX/TX when init completed. */
	0, /* Use current configured reference clock for configuring baudrate. */
	115200, /* 115200 bits/s. */
	usartOVS16, /* 16x oversampling. */
	usartDatabits8, /* 8 databits. */
	usartNoParity, /* No parity. */
	usartStopbits1, /* 1 stopbit. */
	false, /* Do not disable majority vote. */
	false, /* Not USART PRS input mode. */
	usartPrsRxCh0 /* PRS channel 0. */
	};

	/* Define output (TX) */
	GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 1);

	/* Define input, no filtering */
	GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 0);

	/* Configure USART for basic async operation */
	USART_InitAsync(usart, &init);

	/* Clear & activate RX interrupt */
	USART_IntClear(USART1, USART_IEN_RXDATAV);
	USART_IntEnable(USART1, USART_IEN_RXDATAV);


	NVIC_ClearPendingIRQ(USART1_RX_IRQn);
	NVIC_ClearPendingIRQ(USART1_TX_IRQn);

	NVIC_EnableIRQ(USART1_RX_IRQn);

	/* Set route #1 for USART */
	usart->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
			| USART_ROUTE_LOCATION_LOC1;

	/* Finally enable it */
	USART_Enable(usart, usartEnable);
}

void my_DMA_Init(void) {
	DMA_Init_TypeDef dma_init;
	DMA_CfgDescr_TypeDef dma_descr;
	DMA_CfgChannel_TypeDef dma_chn;
	DMA_CB_TypeDef dma_cb;

	/* Init DMA */
	dma_init.hprot = 0;
	dma_init.controlBlock = dmaControlBlock;
	DMA_Init(&dma_init);

	dma_cb.cbFunc = NULL;
	dma_cb.userPtr = NULL;

	dma_chn.enableInt = false;
	dma_chn.highPri = false;
	dma_chn.select = DMAREQ_USART1_TXBL; /* configure to transfer data to USART1 TX register */
	dma_chn.cb = &dma_cb;
	DMA_CfgChannel(DMA_USART_TX_CHANNEL, &dma_chn);

	dma_descr.arbRate = dmaArbitrate1;
	dma_descr.dstInc = dmaDataIncNone;
	dma_descr.hprot = 0;
	dma_descr.size = dmaDataSize1;
	dma_descr.srcInc = dmaDataInc1;
	DMA_CfgDescr(DMA_USART_TX_CHANNEL, true, &dma_descr);
}

void sendUARTbyDMA(void *buffer, int size) {

	/* wait for other DMA transfer to complete */
	while (DMA_ChannelEnabled(DMA_USART_TX_CHANNEL));

	/* Activate DMA */
	DMA_ActivateBasic(DMA_USART_TX_CHANNEL, true, false,
			(void *) &(USART1->TXDATA), buffer, size - 1);
}

int main(void) {
	/* Chip errata */
	CHIP_Init();
	int character;
	char TXbuffer[256];

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_USART1, true);

	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPullDrive, 0); /* LED */

	USART_Init(USART1);

	my_DMA_Init();

	/* Send 'A' character */
	USART_Tx(USART1, 'A');

	/* Infinite loop */
	while (1) {
		if (AvailableData(&RX_SerialBuffer) != 0) {
			character = PopData(&RX_SerialBuffer);
			sprintf(TXbuffer, "Send by DMA (%c)\r\n", character);
			sendUARTbyDMA(TXbuffer, strlen(TXbuffer));
		}
	}
}

