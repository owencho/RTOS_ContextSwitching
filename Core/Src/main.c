/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Common.h"
#include "BaseAddress.h"
#include "List.h"
#include "Irq.h"
#include "Tcb.h"
#include "Scb.h"
#include "ThreadContext.h"
#include "TimerEvent.h"
#include "Kernel.h"
#include "Mutex.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
volatile Tcb * tc2;
volatile Tcb * tcMain;
volatile Tcb * tc1;
TimerEvent  evt,evt2,evt3;

volatile ThreadContext * thread1;
volatile ThreadContext * deQueueThread;
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

/* USER CODE BEGIN PV */
int add2Integers(int a,int b);
void switchThreadContext();
void pushIntoTimerQueue();
void blinkSlowLed();
void blinkFastLed();
void blinkUSBLed();
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  disableIRQ();
  enableGpioG();
  gpioSetMode(gpioG, PIN_13, GPIO_OUT);
  gpioSetPinSpeed(gpioG,PIN_13,HIGH_SPEED);
  gpioSetMode(gpioG, PIN_14, GPIO_OUT);
  gpioSetPinSpeed(gpioG,PIN_14,HIGH_SPEED);

  enableGpio(PORT_B);
  gpioSetMode(gpioB, PIN_13, GPIO_OUT);
  gpioSetPinSpeed(gpioG,PIN_13,HIGH_SPEED);

  //switchThreadContext();
  tcMain = tcbCreateMain();
  tc1 = tcbCreate(1024 ,blinkSlowLed ,"tc1");
  tc2 = tcbCreate(1024 ,blinkUSBLed ,"tc2");
  listAddItemToTail((List*)&readyQueue,(ListItem*)tcMain);
  listAddItemToTail((List*)&readyQueue,(ListItem*)tc1);
  listAddItemToTail((List*)&readyQueue,(ListItem*)tc2);
  enableIRQ();
  /*
  //wk2
  volatile int val = add2Integers(45,10);
  volatile int val1 = val;
  val1 += val;
  */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  blinkFastLed();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void blinkFastLed(){
	while(1){
		//green LED
		gpioToggleBit(gpioG, PIN_13 );
		kernelSleep(&evt,300);
		gpioToggleBit(gpioG, PIN_13 );
		kernelSleep(&evt,300);
	}
}
void blinkSlowLed(){
	while(1){
		//red LED
		gpioToggleBit(gpioG, PIN_14 );
		kernelSleep(&evt2,200);
		gpioToggleBit(gpioG, PIN_14 );
		kernelSleep(&evt2,200);
	}
}

void blinkUSBLed(){
	while(1){
		//red LED
		gpioToggleBit(gpioB, PIN_13 );
		kernelSleep(&evt3,200);
		gpioToggleBit(gpioB, PIN_13 );
		kernelSleep(&evt3,200);
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
