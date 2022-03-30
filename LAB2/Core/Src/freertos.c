/********************************************************************************/
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * Name : Mohamad Abou Helal        : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "semphr.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t varBlink1 = 0;
uint8_t varBlink2 = 0;
uint8_t varBlink3 = 0;
SemaphoreHandle_t xSemaphore;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Blink1Task */
osThreadId_t Blink1TaskHandle;
const osThreadAttr_t Blink1Task_attributes = {
  .name = "Blink1Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TriggTask */
osThreadId_t TriggTaskHandle;
const osThreadAttr_t TriggTask_attributes = {
  .name = "TriggTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for UserbuttonTask */
osThreadId_t UserbuttonTaskHandle;
const osThreadAttr_t UserbuttonTask_attributes = {
  .name = "UserbuttonTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void wait_cycles( uint32_t n );
void ToggleLed(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Blink1(void *argument);
void Trigg(void *argument);
void Userbutton(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	xSemaphore = xSemaphoreCreateBinary();
	/* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Blink1Task */
  Blink1TaskHandle = osThreadNew(Blink1, NULL, &Blink1Task_attributes);

  /* creation of TriggTask */
  TriggTaskHandle = osThreadNew(Trigg, NULL, &TriggTask_attributes);

  /* creation of UserbuttonTask */
  UserbuttonTaskHandle = osThreadNew(Userbutton, NULL, &UserbuttonTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Blink1 */
/**
* @brief Function implementing the Blink1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Blink1 */
void Blink1(void *argument)
{
  /* USER CODE BEGIN Blink1 */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100) ; // ms to ticks
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	/* Infinite loop */
	for(;;)
	{
		if(xSemaphoreTake (xSemaphore, 0)  == pdPASS){
			ToggleLed();
			xSemaphoreGive(xSemaphore);
		}
		vTaskDelayUntil( &xLastWakeTime, xPeriod );
		//		vTaskDelay(10);
	}
  /* USER CODE END Blink1 */
}

/* USER CODE BEGIN Header_Trigg */
/**
* @brief Function implementing the TriggTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Trigg */
void Trigg(void *argument)
{
  /* USER CODE BEGIN Trigg */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(200);	//ms to ticks
	//Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil( &xLastWakeTime, xPeriod);
    wait_cycles(10);
  }
  /* USER CODE END Trigg */
}

/* USER CODE BEGIN Header_Userbutton */
/**
* @brief Function implementing the UserbuttonTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Userbutton */
void Userbutton(void *argument)
{
  /* USER CODE BEGIN Userbutton */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(20);
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	xSemaphoreGive(xSemaphore);

	/* Infinite loop */
	for(;;)
	{
		if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)==GPIO_PIN_RESET)
		{
			xSemaphoreTake(xSemaphore, 10);
			varBlink1 = 1;
		}
		else{
			xSemaphoreGive(xSemaphore);
			varBlink1 = 0;
		}
		vTaskDelayUntil( &xLastWakeTime, xPeriod );
	}
  /* USER CODE END Userbutton */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void wait_cycles( uint32_t n) {
	uint32_t l = n/3;
	asm volatile( "0:" "SUBS %[count], 1;" "BNE 0b;" :[count]"+r"(l));
}

void ToggleLed(void){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

/* USER CODE END Application */

/*
 Includes ------------------------------------------------------------------
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include"semphr.h"

 Private includes ----------------------------------------------------------
 USER CODE BEGIN Includes

 USER CODE END Includes

 Private typedef -----------------------------------------------------------
 USER CODE BEGIN PTD

 USER CODE END PTD

 Private define ------------------------------------------------------------
 USER CODE BEGIN PD

 USER CODE END PD

 Private macro -------------------------------------------------------------
 USER CODE BEGIN PM

 USER CODE END PM

 Private variables ---------------------------------------------------------
 USER CODE BEGIN Variables
uint8_t varBlink1 = 0;
uint8_t varBlink2 = 0;
uint8_t varBlink3 = 0;
SemaphoreHandle_t xSemaphore;
 USER CODE END Variables
 Definitions for defaultTask
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
 Definitions for Blink1Task
osThreadId_t Blink1TaskHandle;
const osThreadAttr_t Blink1Task_attributes = {
  .name = "Blink1Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,

};
 Definitions for TriggTask
osThreadId_t TriggTaskHandle;
const osThreadAttr_t TriggTask_attributes = {
  .name = "TriggTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
 Definitions for UserbuttonTask
osThreadId_t UserbuttonTaskHandle;
const osThreadAttr_t UserbuttonTask_attributes = {
  .name = "UserbuttonTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
 Definitions for myMutex01
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
  .name = "myMutex01"
};


/* Private function prototypes -----------------------------------------------
 USER CODE BEGIN FunctionPrototypes
void wait_cycles( uint32_t n);
void ToggleLed(void);
 USER CODE END FunctionPrototypes

void StartDefaultTask(void *argument);
void Blink1(void *argument);
//void Blink2(void *argument);
void Trigg(void *argument);
void Userbutton(void *argument);

void MX_FREERTOS_Init(void);  (MISRA C 2004 rule 8.1)

*
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None

void MX_FREERTOS_Init(void) {
   USER CODE BEGIN Init

   USER CODE END Init

   USER CODE BEGIN RTOS_MUTEX
   add mutexes, ...
   USER CODE END RTOS_MUTEX

   USER CODE BEGIN RTOS_SEMAPHORES
   add semaphores, ...
   USER CODE END RTOS_SEMAPHORES

   USER CODE BEGIN RTOS_TIMERS
   start timers, add new ones, ...
   USER CODE END RTOS_TIMERS

   USER CODE BEGIN RTOS_QUEUES
   add queues, ...
   USER CODE END RTOS_QUEUES

   Create the thread(s)
   creation of defaultTask
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

   creation of Blink1Task
  Blink1TaskHandle = osThreadNew(Blink1, NULL, &Blink1Task_attributes);

   creation of TriggTask
  TriggTaskHandle = osThreadNew(Trigg, NULL, &TriggTask_attributes);

   creation of UserbuttonTask
  UserbuttonTaskHandle = osThreadNew(Userbutton, NULL, &UserbuttonTask_attributes);

   USER CODE BEGIN RTOS_THREADS
   add threads, ...
   USER CODE END RTOS_THREADS

   USER CODE BEGIN RTOS_EVENTS
   add events, ...
   USER CODE END RTOS_EVENTS

}

 USER CODE BEGIN Header_StartDefaultTask
*
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None

 USER CODE END Header_StartDefaultTask
void StartDefaultTask(void *argument)
{
   USER CODE BEGIN StartDefaultTask
   Infinite loop
  for(;;)
  {
    osDelay(1);
  }
   USER CODE END StartDefaultTask
}

 USER CODE BEGIN Header_Blink1
*
* @brief Function implementing the Blink1Task thread.
* @param argument: Not used
* @retval None

 USER CODE END Header_Blink1
void Blink1(void *argument)
{
   USER CODE BEGIN Blink1
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100) ; // ms to ticks
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	//myMutex01Handle= xSemaphoreCreateMutex();
   Infinite loop
  for(;;)
  {
	  if(xSemaphoreTake (xSemaphore, 0)  == pdPASS){
	  			ToggleLed();
	  			xSemaphoreGive(xSemaphore);
	  		}
	  		vTaskDelayUntil( &xLastWakeTime, xPeriod );
	  		//		vTaskDelay(10);

  }
	  //if(osMutexGetOwner(myMutex01Handle) == NULL)
	  		 // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);	// get Toggle PC10
	  osMutexAcquire(myMutex01Handle, 0U);
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	 		  osMutexRelease(myMutex01Handle);


	  vTaskDelayUntil( &xLastWakeTime, xPeriod );

  }
  /* USER CODE END Blink1
}

 USER CODE BEGIN Header_Blink2
*
* @brief Function implementing the Blink2Task thread.
* @param argument: Not used
* @retval None

 USER CODE END Header_Blink2
void Blink2(void *argument)
{
   USER CODE BEGIN Blink2
	TickType_t xLastWakeTime;
		const TickType_t xPeriod = pdMS_TO_TICKS(20) ; // ms to ticks
		// Initialise the xLastWakeTime variable with the current time.
		xLastWakeTime = xTaskGetTickCount();

   Infinite loop
  for(;;)
  {
	  varBlink2 = 1;
	  wait_cycles(400000);
	  varBlink2 = 0;
	  vTaskDelay(20);
	  //vTaskDelayUntil( &xLastWakeTime, xPeriod );
  }
   USER CODE END Blink2
//}

 USER CODE BEGIN Header_Trigg
*
* @brief Function implementing the TriggTask thread.
* @param argument: Not used
* @retval None

 USER CODE END Header_Trigg
void Trigg(void *argument)
{
   USER CODE BEGIN Trigg
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(200) ; // ms to ticks
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
   Infinite loop
  for(;;)
  {
	  vTaskDelayUntil( &xLastWakeTime, xPeriod);
	      wait_cycles(10);
	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);	//Toggle PD2
	  	  vTaskDelayUntil(&xLastWakeTime, xPeriod);
	      //osDelay(1);
	    }
	     USER CODE END Trigg
	  }


 USER CODE BEGIN Header_Userbutton
*
* @brief Function implementing the UserbuttonTask thread.
* @param argument: Not used
* @retval None

 USER CODE END Header_Userbutton
void Userbutton(void *argument)
{
   USER CODE BEGIN Userbutton
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(20);	// ms to ticks
	xLastWakeTime = xTaskGetTickCount();// Initialize xLastWakeTime with the current time
	xSemaphoreGive(xSemaphore);
   Infinite loop
  for(;;)
  {
	  if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)==GPIO_PIN_RESET)
	  		{
	  			xSemaphoreTake(xSemaphore, 10);
	  			varBlink1 = 1;
	  		}
	  		else{
	  			xSemaphoreGive(xSemaphore);
	  			varBlink1 = 0;
	  		}
	  		vTaskDelayUntil( &xLastWakeTime, xPeriod );


	  if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)	//PUSH BUTTON B1; connected to I/O PC13
		  osMutexAcquire(myMutex01Handle, 0U);
	  else
		  osMutexRelease(myMutex01Handle);

	  vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }

   USER CODE END Userbutton
}

 Private application code --------------------------------------------------
 USER CODE BEGIN Application
void wait_cycles ( uint32_t n){
	uint32_t l = n/3; //cycles per loop is 3
	asm volatile( "0:" "SUBS %[count], 1;" "BNE 0b;" :[count]"+r"(l) );
}
void ToggleLed(void){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

 USER CODE END Application

*********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE***
