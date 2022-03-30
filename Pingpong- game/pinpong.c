/**
******************************************************************************
@brief Pingpong statemachine for Pingpong-program
@file pingpong.c
@author mohamad Abou Helal
@version 1.0
@date 10-11-2021
@brief Functions and structures for program Pingpong
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

#include "pingpong.h"
#include "stdbool.h"
#include "stdint.h"


/* Define states for state machine*/
typedef enum
{ Start,
MoveRight,
MoveLeft
} states;
static states State, NextState;

void Pingpong(void)
{

	 bool ButtonPressed = false; // To remember that button is pressed
	 uint32_t Varv, Speed; // Ball speed
	 uint8_t Led; // LED nr
	 uint8_t L_player;
	 uint8_t R_player;
	 bool L_serve = true;
	 bool R_serve = true;
	 State= Start; // Initiate State to Start
	 NextState= Start;
	 Speed= 500000; // Number of loops


	 /* Infinite loop */

	 while (1)
	 {
	 State = NextState;
	 switch (State) // State machine
	 {
	 case Start:{
		 if(L_player == 4|| R_player==4 ){
			 L_player = 0;
			 R_player = 0;
			 L_serve = true;
			 R_serve = true;
 		 }
	 Led_on(0); // Turn off all LEDs

	 if ( L_hit() == true && L_serve== true ) // L serve
	 {
		 L_serve = false;
		 R_serve = true;
	 Led = 1;
	 NextState= MoveRight;
	 while ( L_hit() == true ); // wait until button is released
	 }
	 else if ( R_hit() == true && R_serve== true ) // R serve
	 {
		 R_serve = false;
		 L_serve = true;
	 Led = 8;
	 NextState= MoveLeft;
	 while ( R_hit() == true ); // wait until button is released
	 }
	 else
	 NextState = Start; // Stay in Start state
	 }
	 break;
	 case MoveRight:
	 {
	 Led_on(Led);
	 Varv = Speed;
	 while( Varv != 0 )
	 {
	 if ( R_hit() ) ButtonPressed = true; // R hit
	 Varv--;
	 }
	 if ( ButtonPressed ) // R pressed
	 {
	 if ( Led == 8 ) // and LED8 activa
	 {
		Speed -= 45000;
	 NextState = MoveLeft; // return ball
	 Led=7;
	 }
	 else{
		 L_player++;
	 show_points(L_player, R_player);
	 Speed= 500000;
	 NextState = Start; // hit to early
	 }
	 }
	 else
	 {

	 if ( Led == 9 ){
		 Speed= 500000;                // no hit or to late
		 L_player++;
	 show_points(L_player, R_player);
	 NextState = Start;
	 }
	 else
	 NextState = MoveRight; // ball continues to move right
	 }
	 if ( !ButtonPressed ) Led++; // prepare to turn next LED on
	 ButtonPressed = false;
	 }
	 break;
	 case MoveLeft:
	 {
	  Led_on(Led);
	  Varv = Speed;
	  while(Varv != 0)
	  {
	  if ( L_hit() ) ButtonPressed = true; // L hit
	  Varv--;
	  }
	  if ( ButtonPressed ) // L pressed
	  {
	  if ( Led == 1 ) // and LED1 active
	  {
		  Speed -= 45000;
	  NextState = MoveRight; // return ball
	  Led=2;
	  }
	  else{
		  R_player++;
	  show_points(L_player, R_player);
	  Speed= 500000;
	  NextState = Start; // hit to early
	  }
	  }
	  else
	  {
	  if ( Led == 0 ){
		  Speed= 500000;              // no hit or to late
	  	  R_player++;
	  show_points(L_player,R_player);
	  NextState = Start;
	  }
	  else{
	  NextState = MoveLeft; // ball continues to move left
	  }
	  }
	  if ( !ButtonPressed ) Led--; // prepare to turn next LED on
	  ButtonPressed= false;
	  }
	  break;

	  default:
	  break;
	  }
	  }
	 } // End of function Pingpong
