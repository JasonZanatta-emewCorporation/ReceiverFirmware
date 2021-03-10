/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BT_Flash_Commands_H
#define BT_Flash_Commands_H

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Functions prototypes ---------------------------------------------*/
void BT_Reset(void);
void BT_EnterTestMode(void);
void BT_EnterAppMode(void);
void BT_OpenFlashMemory(void);
void BT_CloseFlashMemory(void);
void BT_EraseFlashMemory(uint16_t address);
void BT_Read128Bytes(uint16_t address);
void BT_Write128Bytes(uint16_t address, uint8_t *InputDataArray);
void BT_ConfigRun(void);

#endif /* BT_Flash_Commands_H */

