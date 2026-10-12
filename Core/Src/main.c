/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "model_40hz_1sec.h"
#include "model_40hz_1sec_data.h"
#include "ai_platform.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SAMPLE_INTERVAL_MS 25
#define SAMPLE_COUNT       40
#define INPUT_LIMIT        30
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
float ai_in[SAMPLE_COUNT];
float ai_out[4];


static ai_handle network = AI_HANDLE_NULL;
/* Aktivasyon tamponu, boyutu X-CUBE-AI'nin urettigi makrodan gelir */
static uint8_t activations[AI_MODEL_40HZ_1SEC_DATA_ACTIVATIONS_SIZE] __attribute__((aligned(32)));

/* Morse buffers */
static char token[8];
static uint8_t token_len = 0;

static char message[128];
static uint8_t msg_len = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
volatile uint32_t ms_counter = 0;
volatile uint8_t  sample_idx = 0;
volatile uint8_t  sampling_done = 0;
volatile int last_class = -1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */


static void AI_Init(void);
static int  AI_Predict(const float *in40, float *out4);
static int  argmax4(const float *y);

static void push_char(char c);
static void flush_token_to_message(void);
static char morse_to_char(const char *t);

typedef struct { const char *code; char ch; } MorseMap;
static const MorseMap MORSE[] = {
  {".-", 'A'},   {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'},  {".", 'E'},
  {"..-.", 'F'}, {"--.", 'G'},  {"....", 'H'}, {"..", 'I'},   {".---", 'J'},
  {"-.-", 'K'},  {".-..", 'L'}, {"--", 'M'},   {"-.", 'N'},   {"---", 'O'},
  {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'},  {"...", 'S'},  {"-", 'T'},
  {"..-", 'U'},  {"...-", 'V'}, {".--", 'W'},  {"-..-", 'X'}, {"-.--", 'Y'},
  {"--..", 'Z'},
};
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static char morse_to_char(const char *t)
{
  for (unsigned i = 0; i < sizeof(MORSE)/sizeof(MORSE[0]); i++)
    if (strcmp(MORSE[i].code, t) == 0) return MORSE[i].ch;
  return '?';
}

static void push_char(char c)
{
  if (msg_len < sizeof(message) - 1)
  {
    message[msg_len++] = c;
    message[msg_len] = '\0';
  }
}

static void flush_token_to_message(void)
{
  if (token_len == 0) return;
  token[token_len] = '\0';
  push_char(morse_to_char(token));
  token_len = 0;
}

static int argmax4(const float *y)
{
  int imax = 0;
  float vmax = y[0];
  for (int i = 1; i < 4; i++)
  {
    if (y[i] > vmax) { vmax = y[i]; imax = i; }
  }
  return imax;
}

static void AI_Init(void)
{
  ai_error err;
  const ai_handle acts[] = { activations };

  err = ai_model_40hz_1sec_create_and_init(&network, acts, NULL);
  if (err.type != AI_ERROR_NONE)
  {
    Error_Handler();
  }
}
static int AI_Predict(const float *in40, float *out4)
{
  ai_i32 n_batch;

  ai_buffer *in  = ai_model_40hz_1sec_inputs_get(network, NULL);
  ai_buffer *out = ai_model_40hz_1sec_outputs_get(network, NULL);
  if (!in || !out) return -1;

  in[0].data  = AI_HANDLE_PTR((void*)in40);
  out[0].data = AI_HANDLE_PTR((void*)out4);

  n_batch = ai_model_40hz_1sec_run(network, in, out);
  return (n_batch == 1) ? 0 : -1;
}

/* TIM3 interrupt callback: 1ms tick, sample every 25ms, 40 samples = 1 second */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3)
  {
    ms_counter++;

    if ((ms_counter % SAMPLE_INTERVAL_MS) == 0 && sample_idx < SAMPLE_COUNT)
    {
      /* PC13 active-low button: pressed=1, released=0 */
      uint8_t btn = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) ? 1 : 0;
      ai_in[sample_idx++] = (float)btn;
    }

    if (sample_idx >= SAMPLE_COUNT)
      sampling_done = 1;
  }
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
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  AI_Init();
  HAL_TIM_Base_Start_IT(&htim3);

  memset(message, 0, sizeof(message));
   memset(token, 0, sizeof(token));

   int input_count = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  if (sampling_done)
	      {
	        sampling_done = 0;

	        /* Run AI on last 1-second window (40 samples) */
	        if (AI_Predict(ai_in, ai_out) == 0)
	        {
	          last_class = argmax4(ai_out); /* 0..3 */

	          /* Morse logic:
	             0: dot -> add '.'
	             2: dash -> add '-'
	             3: blank -> letter end
	             1: double click -> word end
	          */
	          if (last_class == 0)
	          {
	            if (token_len < sizeof(token) - 1) token[token_len++] = '.';
	          }
	          else if (last_class == 2)
	          {
	            if (token_len < sizeof(token) - 1) token[token_len++] = '-';
	          }
	          else if (last_class == 3)
	          {
	            flush_token_to_message();
	          }
	          else if (last_class == 1)
	          {
	            flush_token_to_message();
	            push_char(' ');
	          }
	        }

	        /* Prepare next 1-second capture window */
	        ms_counter = 0;
	        sample_idx = 0;

	        input_count++;
	        if (input_count >= INPUT_LIMIT)
	        {
	          /* finish any remaining token */
	          flush_token_to_message();
	          /* Now 'message' is final output (watch in debugger) */
	          while (1) { /* stop here for debugger */ }
	        }
	      }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 48-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
