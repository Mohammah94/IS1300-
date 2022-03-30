/*
 * screen.c
 *
 *  Created on: Jan 5, 2022
 *      Author: Mohamad Abou Helal
 */

#include "adc.h"
#include "stm32l4xx_hal.h"
#include "rtc.h"
#include "screen.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "spi.h"


static RTC_TimeTypeDef sTime;
static RTC_DateTypeDef sDate;
static uint32_t hour = 0;
static uint32_t minutes = 0;
static uint32_t seconds = 0;
static uint32_t prevsecond = 0;


/*
 * @brief initializing for screen
 * @param no parameters
 *
 */

void displayInit(void)
{
	SET_CS;
	HAL_Delay(10);
	RESET_CS;

	screen_reset();
	HAL_Delay(10);

	screen_send_instruction(0x80);
	screen_send_instruction(0x3A);
	screen_send_instruction(0x09);
	screen_send_instruction(0x06);
	screen_send_instruction(0x1E);
	screen_send_instruction(0x39);
	screen_send_instruction(0x1B);
    screen_send_instruction(0x6E);
    screen_send_instruction(0x56);
    screen_send_instruction(0x7A);
    screen_send_instruction(0x38);
	screen_send_instruction(0x0F);
	screen_send_instruction(0x01);

	SET_CS;
}
/*
 * @brief sends instruction to screen
 * @param uint8_t instruction
 *
 */

void screen_send_instruction(uint8_t instruction)
{
	SET_CS;
	HAL_Delay(10);
	RESET_CS;

	uint8_t buff[3];
	buff[0] = 0x1F;
	buff[1] = instruction & 0x0F;
	buff[2] = (instruction >> 4);

	HAL_SPI_Transmit(&hspi2, buff, 3, 10);
}

/*
 * @brief Ask question for time to the user.
 * @more Works through terminal with screen function on IOS and linux.
 * @more Works with Putty on windows, dowload putty if you have windows.
 *
 */

void setTime(void){

	uint8_t *buff = "Put hours\n\r";
	HAL_UART_Transmit(&huart5, (uint8_t *)buff, 11, 5000);
	while(HAL_UART_Receive(&huart5, (uint8_t*)&hour, 2, 1000) != HAL_OK);

	buff = "Put minutes\n\r";
    HAL_UART_Transmit(&huart5, (uint8_t *)buff, 12, 5000);
    while(HAL_UART_Receive(&huart5, (uint8_t*)&minutes, 2, 1000) != HAL_OK);

    buff = "Put seconds\n\r";
    HAL_UART_Transmit(&huart5, (uint8_t *)buff, 13, 5000);
    while(HAL_UART_Receive(&huart5, (uint8_t*)&seconds, 2, 1000) != HAL_OK);

    buff = "done!\n\r";
    HAL_UART_Transmit(&huart5, (uint8_t *)buff, 6, 5000);


    sscanf((uint8_t *)&hour, "%hhu", &sTime.Hours);
    sscanf((uint8_t *)&minutes, "%hhu", &sTime.Minutes);
    sscanf((uint8_t *)&seconds, "%hhu", &sTime.Seconds);

    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

}

/*
 * @brief writes time to the screen
 *
 */
void writeTime(void)
{
	char timearray[8];
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	sprintf(timearray, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	if(prevsecond!= sTime.Seconds){
		screen_string(timearray, 1);
		prevsecond = sTime.Seconds;
	}


}

/*
 * @brief sends string to be displayed on screen
 *
 */
void screen_string(uint8_t* string, uint8_t row)
{
	if (row == 1)
	{
		screen_send_instruction(0x80);
		do
		{
			screen_char(*string++);
		}
		while (*string);

	}
}


/*
 * @brief displays one char at a time on the screen
 *
 */
void screen_char(uint8_t chars)
{
	SET_CS;
	HAL_Delay(10);
	RESET_CS;

	uint8_t transfer[3];
	transfer[0] = 0x5f;
	transfer[1] = chars & 0x0f;
	transfer[2] = chars >> 4;
	HAL_SPI_Transmit(&hspi2, transfer, 3, 10);

}

/*
 * @brief sets dimmer from adc value
 *
 */
void set_dimmer()
{
	uint32_t adc = get_adc();
	htim3.Instance -> CCR2 = adc;
}

/*
 * @brief gets adc value to set the dimmer
 *
 */

uint32_t get_adc()
{
	uint32_t adc;
	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
	{
		adc = HAL_ADC_GetValue(&hadc1);
	}
	return adc;
}

/*
 * @brief reset dimmer
 *
 */
void screen_reset()
{
	HAL_GPIO_WritePin(Display_reset_GPIO_Port, Display_reset_Pin, GPIO_PIN_SET);
}
