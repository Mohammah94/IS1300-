
/**
* @file           	: test.c
* @brief          	: test program body
* @describtion     	: Test functionality of the program
* @Author			: Mohamad Abou Helal
**/

#include "test.h"
#include <stdio.h>
#include "main.h"
#include "screen.h"

/*
 * @brief Test_program, test functions .
 * @param has all of the test functions to make it eaiser to test function. .
 * @return void
 */

void test_program(void)
{


	displayInit();
	screen_text_test();
	setTime();
	while(1)
	{

		writeTime();
		set_dimmer();

	}
}





/*
 * @brief test screen
 * @param has all of the test functions to make it eaiser to test function. .
 * @return void
 */

void screen_text_test()
{
	uint8_t* text = "hello";
	screen_string(text,1);
	HAL_Delay(1000);

	screen_send_instruction(0x01);

}





