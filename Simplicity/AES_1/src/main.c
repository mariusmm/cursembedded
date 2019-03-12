/**
 ******************************************************************************
 * @file    main.c
 * @author  Màrius Montón <marius.monton@gmail.com>
 * @version V1.0
 * @date    10-March-2019
 * @brief   Use of AES module using ECB mode
 * @license This project is released under the GNU Public License.s
 * ******************************************************************************
 */

#include <stdio.h>
#include <string.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_aes.h"

#include "BSP.h"

uint8_t myKey[16] = {0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
						0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32};

char my_message[] = "This is a plain message to be encrypted with the AES module.";
uint8_t my_buffer[64];

uint8_t decryptionKey[16];
char my_buffer_decrypt[64];

int my_message_len;

int main(void)
{
  int i;

  /* Chip errata */
  CHIP_Init();

  setupSWOForPrint();

  my_message_len = strlen(my_message);

  /* must be multiple of 16, so we take the next multiple */
  my_message_len = ((59/16) + 1) * 16;

  CMU_ClockEnable(cmuClock_AES, true);

  printf("Original message: %s\r\n", my_message);
  printf("Message length: %d\r\n", my_message_len);

  printf("key: ");
  for(i = 0; i < 16; i++) {
	  printf("%02X", myKey[i]);
  }
  printf("\r\n");

  /* Encrypt message */
  AES_ECB128(my_buffer, (uint8_t*) my_message, my_message_len, myKey, true);

  printf("Encrypted message: ");
  for(i = 0; i < my_message_len; i++) {
	printf("%02X", my_buffer[i]);
  }
  printf("\r\n");


  /* Generate decrypt key from original key */
  AES_DecryptKey128(decryptionKey, myKey);

  /* Decrypt message */
  AES_ECB128((uint8_t*) my_buffer_decrypt, my_buffer, my_message_len, decryptionKey, false);

  printf("Decrypted message: %s\r\n", my_buffer_decrypt);

  /* Infinite loop */
  while (1) {
	  ;
  }
}
