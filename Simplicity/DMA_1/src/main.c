/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    10-April-2018
 * @brief   DMA example, just a transfer between two memory buffers
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include <stdio.h>
#include <string.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_dma.h"
#include "dmactrl.h"
#include "BSP.h"

#define BUFFER_SIZE 256

uint32_t dst_buffer[BUFFER_SIZE];
uint32_t src_buffer[BUFFER_SIZE];

volatile bool dma_end;

void DmaComplete(unsigned int channel, bool primary, void *user) {
	dma_end = true;
}


bool check_buffers_copy() {
	int i;

	for (i=0;i<BUFFER_SIZE;i++) {
		if (dst_buffer[i] != src_buffer[i]) {
			return false;
		}
	}

	return true;
}

int main(void) {
	int i;

	DMA_Init_TypeDef dma_init;
	DMA_CfgDescr_TypeDef dma_descr;
	DMA_CfgChannel_TypeDef dma_chn;
	DMA_CB_TypeDef dma_cb;

	/* Chip errata */
	CHIP_Init();

	setupSWOForPrint();

	dma_end = false;

	/* Init DMA */
	dma_init.hprot = 0;
	dma_init.controlBlock = dmaControlBlock;
	DMA_Init(&dma_init);

	dma_cb.cbFunc = DmaComplete;
	dma_cb.userPtr = NULL;

	dma_chn.enableInt = true;
	dma_chn.highPri = false;
	dma_chn.select = 0; /* set to 0 because is a memory-to-memory transfer */
	dma_chn.cb = &dma_cb;
	DMA_CfgChannel(0, &dma_chn);

	dma_descr.arbRate = dmaArbitrate1;
	dma_descr.dstInc = dmaDataInc4;
	dma_descr.hprot = 0;
	dma_descr.size = dmaDataSize4;
	dma_descr.srcInc = dmaDataInc4;
	DMA_CfgDescr(0, true, &dma_descr);

	/* prepare buffers */
	for (i = 0; i < BUFFER_SIZE; i++) {
		dst_buffer[i] = 0;
	}

	for (i = 0; i < BUFFER_SIZE; i++) {
		src_buffer[i] = i*i;
	}

	DMA_ActivateAuto(0, true, dst_buffer, src_buffer, BUFFER_SIZE - 1);

	printf("hello world\n");

	/* Infinite loop */
	while (1) {
		if (dma_end == true) {

			dma_end = false;

			printf("DMA transfer complete\n");
			printf("src buffer: %lu, %lu, %lu, %lu, %lu\n",
					src_buffer[0], src_buffer[1], src_buffer[2], src_buffer[3],
					src_buffer[4]);
			printf("dst buffer: %lu, %lu, %lu, %lu, %lu\n",
					dst_buffer[0], dst_buffer[1], dst_buffer[2], dst_buffer[3],
					dst_buffer[4]);

			printf("src buffer: %lu, %lu, %lu, %lu, %lu\n",
					src_buffer[BUFFER_SIZE-5], src_buffer[BUFFER_SIZE-4], src_buffer[BUFFER_SIZE-3], src_buffer[BUFFER_SIZE-2],
					src_buffer[BUFFER_SIZE-1]);
			printf("dst buffer: %lu, %lu, %lu, %lu, %lu\n",
					dst_buffer[BUFFER_SIZE-5], dst_buffer[BUFFER_SIZE-4], dst_buffer[BUFFER_SIZE-3], dst_buffer[BUFFER_SIZE-2],
					dst_buffer[BUFFER_SIZE-1]);

			if (check_buffers_copy() == true) {
				printf("Buffers are identical\n");
			} else {
				printf("Buffers are different!\n");
			}
		}
	}
}
