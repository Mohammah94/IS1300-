#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "stm32l4xx_hal.h"

#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#define RESET_CS HAL_GPIO_WritePin(CS_SPI2_GPIO_Port, CS_SPI2_Pin, 0) // Reset pin
#define SET_CS HAL_GPIO_WritePin(CS_SPI2_GPIO_Port, CS_SPI2_Pin, 1)  // Set pin

void screen_string(uint8_t* string, uint8_t row);
void screen_reset();
void setDimmer(void);
uint32_t get_adc(void);
void displayInit(void);
void screen_send_instruction(uint8_t instruction);
void screen_char(uint8_t chars);
void setTime(void);
void writeTime(void);
void wait_cycles(uint32_t n );

#endif
