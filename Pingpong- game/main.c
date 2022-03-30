#include "main.h"
#undef RUN_TEST_PROGRAM

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();

#ifdef RUN_TEST_PROGRAM
 Test_program();
#else
 Pingpong();
#endif

  while (1)
  {
  }
}

