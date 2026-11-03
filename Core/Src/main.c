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

  /* Exercise 2: watch these in the debugger */
  static TaskHandle_t exReceiver;
  static QueueHandle_t exQueue;
  static volatile eTaskState exStateBefore;  /* receiver state before the send */
  static volatile eTaskState exStateDuring;  /* receiver state after the send, scheduler still suspended */
  static volatile eTaskState exStateAfter;   /* receiver state after xTaskResumeAll() */
  static volatile BaseType_t exRecvResult;   /* return value of xQueueReceive() */
  static volatile uint32_t exRecvValue;      /* value the receiver got */

  /* Exercise 2 user events (Tracealyzer channel "ex2").
   * eTaskState values: 0 Running, 1 Ready, 2 Blocked, 3 Suspended, 4 Deleted */
  static TraceStringHandle_t exChannel;
  static TraceStringHandle_t exFmtHandle, exFmtBefore, exFmtDuring, exFmtAfter, exFmtRecv;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * Exercise 2: the receiver blocks on a queue with a long timeout. The
 * controller suspends the scheduler, sends one item to the queue, and asks
 * eTaskGetState() about the receiver before resuming the scheduler.
 *
 * Expected (see the eTaskState comments in task.h): once the item has been
 * sent, the receiver is no longer waiting for anything, so it should be
 * reported as Ready (1), even though it can't run until the scheduler resumes.
 * Compare that with exStateDuring.
 */
static void exReceiverTask(void *arg)
{
  (void)arg;
  uint32_t value = 0;

  exRecvResult = xQueueReceive(exQueue, &value, pdMS_TO_TICKS(5000));
  exRecvValue = value;
  xTracePrintF2(exChannel, exFmtRecv, exRecvResult, exRecvValue);
  printf("receiver: result=%ld value=%lu\r\n",
         (long)exRecvResult, (unsigned long)exRecvValue);
  vTaskSuspend(NULL);
}

static void exControllerTask(void *arg)
{
  (void)arg;
  uint32_t item = 42;

  xTracePrintF1(exChannel, exFmtHandle, (TraceUnsignedBaseType_t)exReceiver);

  /* give the receiver time to block on the empty queue */
  vTaskDelay(pdMS_TO_TICKS(500));
  exStateBefore = eTaskGetState(exReceiver);
  xTracePrintF1(exChannel, exFmtBefore, exStateBefore);

  vTaskSuspendAll();
  {
    xQueueSend(exQueue, &item, 0);
    exStateDuring = eTaskGetState(exReceiver);
  }
  xTaskResumeAll();
  xTracePrintF1(exChannel, exFmtDuring, exStateDuring);

  vTaskDelay(pdMS_TO_TICKS(100));
  exStateAfter = eTaskGetState(exReceiver);
  xTracePrintF1(exChannel, exFmtAfter, exStateAfter);

  printf("ctrl: before=%d during=%d (expected 1) after=%d\r\n",
         (int)exStateBefore, (int)exStateDuring, (int)exStateAfter);
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

  xTraceStringRegister("ex2", &exChannel);
  xTraceStringRegister("receiver handle=0x%X", &exFmtHandle);
  xTraceStringRegister("state before send=%u", &exFmtBefore);
  xTraceStringRegister("state after send, scheduler suspended=%u", &exFmtDuring);
  xTraceStringRegister("state after resume=%u", &exFmtAfter);
  xTraceStringRegister("receiver: result=%d value=%u", &exFmtRecv);

  exQueue = xQueueCreate(1, sizeof(uint32_t));

  /* receiver has the higher priority so it runs, and blocks, first */
  xTaskCreate(exReceiverTask,   "exRecv", 256, NULL, 3, &exReceiver);
  xTaskCreate(exControllerTask, "exCtrl", 256, NULL, 2, NULL);
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
