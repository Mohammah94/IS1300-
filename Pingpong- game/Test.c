#include<stdint.h>
#include"stdbool.h"

void Test_program(void)
{
Test_Led();
Test_show_points();
Test_buttons();
}
void Test_Led(void)
{
 int8_t Lednr;
 /* Loop checking that all leds can be turned on*/
 for (Lednr=1; Lednr<= 8; Lednr++) {
 Led_on(Lednr);
 HAL_Delay(500);
 }
 Led_on(9); // Turn off led 8
 HAL_Delay(1000); // 1000 ms
 return;
 }

void Test_show_points(void)
{
	int8_t L_points=1;
	int8_t R_points=0;
	int8_t j=0;
	for(int i =0 ; i<8; i++)
	{
		show_points(R_points, L_points);
		HAL_Delay (500);
		if (i%2 ==0)
			R_points++;
		else
			L_points++;
		i++;

	}
	return;
}
void Test_buttons(void)
{
 int8_t j;

 /* Checking buttons */

 j=4;
 Led_on(j); // Light on

 while (j<9 && j>0)
 {

 if (L_hit() == true)  // Wait for left button hit
 {
   j++; // next led to the right
   Led_on(j); // Light on
   HAL_Delay(100); // 100 ms
   while ( L_hit() == true ); // Wait for button release
   HAL_Delay(100); // 100 ms

 }


 if ( R_hit() == true ) // Wait for right button hit
 {
   j--; // next led to the left
   Led_on(j); // Light on
   HAL_Delay(100); // 100 ms
   while ( R_hit() == true ); // Wait for button release
   HAL_Delay(100); // 100 ms
   if (j<1) j=0; // Start again from left
 }
 	 }
 return;
}
