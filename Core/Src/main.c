/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BoardManagerWrapper.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ring_buffer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "trcRecorder.h"
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
  extern ring_buffer rb;
  extern uint8_t buf[200];

  /* Exercise 1: watch these in the debugger */
  static TaskHandle_t exWaiter;
  static volatile uint32_t exWakeCount;      /* times ulTaskNotifyTake() returned */
  static volatile uint32_t exLastValue;      /* value it returned last time */
  static volatile eTaskState exStateBefore;  /* waiter state before vTaskResume() */
  static volatile eTaskState exStateAfter;   /* waiter state after vTaskResume() */

  /* Exercise 1 user events (Tracealyzer channel "ex1").
   * eTaskState values: 0 Running, 1 Ready, 2 Blocked, 3 Suspended, 4 Deleted */
  static TraceStringHandle_t exChannel;
  static TraceStringHandle_t exFmtHandle, exFmtWake, exFmtBefore, exFmtAfter, exFmtDone;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * Exercise 1: the waiter blocks forever on its task notification. The
 * controller calls vTaskResume() on it, then later sends a real notification.
 *
 * Expected: the waiter wakes up exactly once, when the real notification
 * arrives, with value 1. Compare that with what you observe.
 */
static void exWaiterTask(void *arg)
{
  (void)arg;
  for (;;)
  {
    exLastValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    exWakeCount++;
    xTracePrintF2(exChannel, exFmtWake, exWakeCount, exLastValue);
    printf("waiter: wake #%lu value=%lu\r\n",
           (unsigned long)exWakeCount, (unsigned long)exLastValue);
  }
}

static void exControllerTask(void *arg)
{
  (void)arg;

  xTracePrintF1(exChannel, exFmtHandle, (TraceUnsignedBaseType_t)exWaiter);

  /* give the waiter time to block */
  vTaskDelay(pdMS_TO_TICKS(500));
  exStateBefore = eTaskGetState(exWaiter);
  xTracePrintF1(exChannel, exFmtBefore, exStateBefore);
  printf("ctrl: vTaskResume\r\n");
  vTaskResume(exWaiter);

  vTaskDelay(pdMS_TO_TICKS(500));
  exStateAfter = eTaskGetState(exWaiter);
  xTracePrintF1(exChannel, exFmtAfter, exStateAfter);
  printf("ctrl: xTaskNotifyGive\r\n");
  xTaskNotifyGive(exWaiter);

  vTaskDelay(pdMS_TO_TICKS(500));
  xTracePrintF2(exChannel, exFmtDone, exWakeCount, exLastValue);
  printf("ctrl: done, wakes=%lu (expected 1)\r\n", (unsigned long)exWakeCount);
  vTaskSuspend(NULL);
}


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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  xTraceEnable(TRC_START);

  rb_create(&rb, buf, 200);

  /* enable the DWT cycle counter (16 cyc = 1 us at 16 MHz) */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk;

  xTraceStringRegister("ex1", &exChannel);
  xTraceStringRegister("waiter handle=0x%X", &exFmtHandle);
  xTraceStringRegister("waiter wake: count=%u value=%u", &exFmtWake);
  xTraceStringRegister("state before vTaskResume=%u", &exFmtBefore);
  xTraceStringRegister("state after vTaskResume=%u", &exFmtAfter);
  xTraceStringRegister("done: count=%u value=%u", &exFmtDone);

  /* waiter has the higher priority so it runs, and blocks, first */
  xTaskCreate(exWaiterTask,     "exWaiter", 256, NULL, 3, &exWaiter);
  xTaskCreate(exControllerTask, "exCtrl",   256, NULL, 2, NULL);
  vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  }




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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

/* USER CODE BEGIN 4 */




/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
