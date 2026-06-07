/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BoardManagerWrapper.h"
#include "adc.h"
#include "tim.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile uint32_t isr_cycle = 0;
volatile uint32_t task_cycle = 0;
volatile uint32_t latency_cycles = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BUF_SIZE			3u

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern void* bm;
extern volatile uint16_t adc_buf[3];

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osMutexId uartHandle;
osSemaphoreId adc_readyHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint32_t buf[BUF_SIZE]={0};
uint8_t index_ptr=0;
uint8_t num=0;

/* USER CODE END FunctionPrototypes */

void low_priority(void const * argument);
void adc_task(void const * argument);
void slow(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
	volatile xTaskHandle bad_task_handle = xTask;
	volatile char* bad_task = pcTaskName;
	while (1){
		__NOP();
	}

   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
	while (1){__NOP();}
}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of uart */
  osMutexDef(uart);
  uartHandle = osMutexCreate(osMutex(uart));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of adc_ready */
  osSemaphoreDef(adc_ready);
  adc_readyHandle = osSemaphoreCreate(osSemaphore(adc_ready), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  osSemaphoreWait(adc_readyHandle, 0);
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, low_priority, osPriorityNormal, 0, 512);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, adc_task, osPriorityHigh, 0, 1024);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, slow, osPriorityAboveNormal, 0, 512);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_low_priority */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_low_priority */
void low_priority(void const * argument)
{
  /* USER CODE BEGIN low_priority */
	//default task
  /* Infinite loop */
  for(;;)
  {
  osDelay(1);

  osMutexWait(uartHandle, osWaitForever);
  printf("low takes mutex at %ld\r\n", HAL_GetTick());
  osDelay(100);
  low_priority_task();
  printf("low returns mutex at %ld\r\n", HAL_GetTick());
  osMutexRelease(uartHandle);
  printf("stack size: %ld\r\n", (unsigned long) uxTaskGetStackHighWaterMark(NULL)*4);

  
  }
  /* USER CODE END low_priority */
}

/* USER CODE BEGIN Header_adc_task */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_adc_task */
void adc_task(void const * argument)
{
  /* USER CODE BEGIN adc_task */
	//mytask02
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buf, 3);
  HAL_TIM_Base_Start(&htim2);

  /* Infinite loop */
  for(;;)
  {

  osSemaphoreWait(adc_readyHandle, osWaitForever);
  task_cycle = DWT->CYCCNT;
  latency_cycles = task_cycle - isr_cycle;
  buf[index_ptr] = latency_cycles;
  index_ptr = (index_ptr+1) % BUF_SIZE;

  uint64_t sum=0;
  for (uint8_t i = 0; i < BUF_SIZE; i++){
	  sum = sum + buf[i];
  }

  

  if (num < BUF_SIZE){
	  num++;
  }

  uint32_t avg = sum / num;

  printf("\r\n\r\ncpu cycles: %lu\r\n\r\n", avg);
  osDelay(500);
  osMutexWait(uartHandle, osWaitForever);

  printf("high takes at %ld\r\n", HAL_GetTick());
  bm_adc(bm);
  osDelay(50);
  printf("high returns at %ld\r\n", HAL_GetTick());
  osMutexRelease(uartHandle);
  printf("stack size: %ld bytes\r\n", (unsigned long) uxTaskGetStackHighWaterMark(NULL)*4);
  //HAL_Delay(2000);
  }
  /* USER CODE END adc_task */
}

/* USER CODE BEGIN Header_slow */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_slow */
void slow(void const * argument)
{
  /* USER CODE BEGIN slow */
	//mytask03
  /* Infinite loop */
  for(;;)
  {
  osDelay(15);
  printf("reach slow at %ld\r\n", HAL_GetTick());
  HAL_Delay(300);
  printf("stack size: %ld\r\n", (unsigned long) uxTaskGetStackHighWaterMark(NULL)*4);
  
  }
  /* USER CODE END slow */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){

	if (hadc->Instance == ADC1){
		//adc_ready=1;
		isr_cycle = DWT->CYCCNT;
		osSemaphoreRelease(adc_readyHandle);

	}
}



/* USER CODE END Application */

