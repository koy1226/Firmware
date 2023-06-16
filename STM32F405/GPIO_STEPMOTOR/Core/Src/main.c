/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STEPS_PER_ROTATION 48 // 한 바퀴에 48 스텝(모터 사양에 따라 달라짐)
#define STEPS_PER_90_DEGREE (STEPS_PER_ROTATION / 4) // 12 스텝(90도 회전)
#define STEPS_PER_180_DEGREE (STEPS_PER_ROTATION / 2) // 24 스텝(180도 회전)
#define STEPS_PER_360_DEGREE STEPS_PER_ROTATION // 48 스텝(360도 회전)
#define DELAY_BETWEEN_STEPS_MS 10 // 각 스텝 사이 딜레이 시간 10ms
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t *current_pattern = 0;
int current_pattern_length = 0;
int rotation_steps = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void set_motor_pins(uint8_t pattern);
void motor_wave_drive();
void motor_full_drive();
void motor_half_drive();
void motor_half_drive_ccw();
void rotate_steps(int steps);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (rotation_steps > 0) {
		  rotate_steps(rotation_steps); // 특정 스텝만큼 회전시키는 함수 호출
		  rotation_steps = 0; // 추가 회전 없도록 스텝 다시 0으로 설정.
	  }
    /* USER CODE END WHILE */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// 4개 8비트 이진 정수 저장하는 1차원 배열 선언.

// 1상 : 한 번에 하나의 코일만 활성화. 순차적 코일 활성화. 시계 방향 회전.
uint8_t WAVE_DRIVE_PATTERN[] =
{
		0b1000, 0b0100, 0b0010, 0b0001
};

// 2상 : 한 번에 두 개의 코일 동시 활성화. 순차적 코일 조합 활성화. 시계방향 회전.
uint8_t FULL_DRIVE_PATTERN[] =
{
		0b1100, 0b0110, 0b0011, 0b1001
};

// 1-2상 : 1상, 2상 여자의 조합. 더 세밀한 스텝 수행 가능. 시계 방향 회전
uint8_t HALF_DRIVE_PATTERN[] =
{
		0b1001, 0b1000, 0b1100, 0b0100, 0b0110, 0b0010, 0b0011, 0b0001
};

// 1-2상 : 반시계 방향 회전
uint8_t HALF_DRIVE_PATTERN_CCW[] =
{
		0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001
};


/*----------------------------------------------------------------------------------------------*/

/*
 주어진 패턴에 따라 특정 GPIO(General Purpose Input/Output) 핀 상태를 설정.
 8비트 정수 'pattern' 매개변수로 받아, 각 비트를 GPIO 핀의 상태로 매핑.
 각 비트는 특정 핀에 연결, 비트 값 1이면 해당 핀을 세트 상태(GPIO_PIN_SET), 0이면 리셋 상태(GPIO_PIN_RESET) 설정.
*/
void set_motor_pins(uint8_t pattern) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, (pattern & 0x01) ? GPIO_PIN_SET: GPIO_PIN_RESET);
	//	'pattern' LSB(Least Significant Bit, 가장 낮은 비트) 1인지 0인지에 따라 핀 상태를 세트 또는 리셋.

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, (pattern & 0x02) ? GPIO_PIN_SET: GPIO_PIN_RESET);
	//	'pattern' 두 번째 LSB 1인지 0인지에 따라 핀 상태를 세트 또는 리셋.

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, (pattern & 0x04) ? GPIO_PIN_SET: GPIO_PIN_RESET);
	//	'pattern' 세 번째 LSB 1인지 0인지에 따라 핀 상태를 세트 또는 리셋.

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, (pattern & 0x08) ? GPIO_PIN_SET: GPIO_PIN_RESET);
	//	'pattern' 네 번째 LSB 1인지 0인지에 따라 핀 상태를 세트 또는 리셋.
}

/*
pattern 각 비트 다른 핀에 연결, pattern값에 따라 4개 핀의 상태를 동시에 제어할 수 있음.
모터 드라이브 제어와 같은 애플리케이션에서 유용.
*/

/*----------------------------------------------------------------------------------------------

스텝 모터 구동 방식
1상(wave drive): 한 번에 하나의 코일만 활성화. 간단한 방식. 전력소비, 토크 낮음.
2상(full drive): 한 번에 두개의 코일 동시 활성화. 1상보다 전력소비, 토크 높음.
1-2상(half drive): 모든 코일 50% 전력으로 운영. 1상 - 2상 - 1상 - 2상 번갈아 가며 사용 -> 더 부드럽고, 정밀 제어 가능. but, 전력 소비(약 1.4배) 많음.
모터 해상도 2배(단일상 200스텝 1바퀴, 1-2상 400스텝 1바퀴)

----------------------------------------------------------------------------------------------*/

// 1상
void motor_wave_drive() {
	current_pattern = WAVE_DRIVE_PATTERN;
// 웨이브 드라이브 실행하는데 필요한 핀(또는 코일) 구성 나타내는 비트 패턴(일반적으로 상수 배열로 정의)
// 각 배열 요소 순차적으로 모터를 웨이브 드라이브 방식으로 돌리는데 사용.

current_pattern_length = sizeof(WAVE_DRIVE_PATTERN) / sizeof(uint8_t);
//	패턴의 길이 계산하여 저장
//	패턴 배열의 총 요소 수 = 패턴 전체 바이트 크기 반환 / 한 요소의 바이트 크기
}

// 2상
void motor_full_drive() {
	current_pattern = FULL_DRIVE_PATTERN;
	current_pattern_length = sizeof(FULL_DRIVE_PATTERN) / sizeof(uint8_t);
}

// 1-2상
void motor_half_drive() {
	current_pattern = HALF_DRIVE_PATTERN;
	current_pattern_length = sizeof(HALF_DRIVE_PATTERN) / sizeof(uint8_t);
}

// 1-2상 반시계
void motor_half_drive_ccw() {
	current_pattern = HALF_DRIVE_PATTERN_CCW;
	current_pattern_length = sizeof(HALF_DRIVE_PATTERN_CCW) / sizeof(uint8_t);
}

// 특정 스텝 수 만큼 스텝 모터 회전시키는 함수. 스텝 수 매개변수로 입력받아, 그 수만큼 모터 회전시킴.
void rotate_steps(int steps) {
	// 매개면수로 받는 값만큼 반복, 원하는 스텝 수 만큼 모터 회전시키기 위함.
	for (int i = 0; i < steps; i++)	{
		set_motor_pins(current_pattern[i % current_pattern_length]);
		//1. 현재 설정된 패턴(current_pattern)의 i번째 핀 설정을 적용
		//2. i % current_pattern_length : 패턴 순환
		//ex) 패턴 길이 4, 스텝 5인 경우 나머지 1이므로 다시 첫번째 상태로 순환
		HAL_Delay(DELAY_BETWEEN_STEPS_MS);
	}
}

// 특정 GPIO 핀에 외부 인터럽트 발생할 때마다 자동 호출
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	// 외부 인터럽트 연결 확인 메시지 출력
	printf("HAL_GPIO_EXTI_Callback triggered!\r\n");
	switch (GPIO_Pin) {
	case GPIO_PIN_4: // 버튼 누르면 1상 시계방향 90도
		printf("Set rotation 90 degrees\r\n");
		motor_wave_drive();
		rotation_steps = STEPS_PER_90_DEGREE;
		break;

	case GPIO_PIN_5: // 버튼 누르면 1상 시계방향 180도
		printf("Set rotation 180 degrees\r\n");
		motor_wave_drive();
		rotation_steps = STEPS_PER_180_DEGREE;
		break;

	case GPIO_PIN_6: // 버튼 누르면 1상 시계방향 360도
		printf("Set rotation 360 degrees\r\n");
		motor_wave_drive();
		rotation_steps = STEPS_PER_360_DEGREE;
		break;

	case GPIO_PIN_7: // 버튼 누르면 1-2상 반시계 360도
		printf("Set rotation CCW Direction\r\n");
		motor_half_drive_ccw();
		rotation_steps = STEPS_PER_360_DEGREE * 2;
		// Since it is 1-2 phase, you need to do 1 or 2 phase step x 2.
		break;

	default:
		break;
	}
}
// UART(Universal Asynchronous Receiver/Transmitter) 데이터 전송 함수
int __io_putchar(int ch) // 단일 문자 ch 전송
{
	//UART_HandleTypeDef huart1;
	HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 0xFFFF);
	// (UART 핸들러, 전송할 데이터의 포인터, 전송할 데이터 크기(1바이트), 전송 시간 제한(최대값))
	return ch;
}
// 문자열 전송(파일 디스크립터, 전송 문자열 시작 포인터, 전송 문자열 길이)
int _write(int file, char *ptr, int len) {
	int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++) {
		__io_putchar(*ptr++); // 문자열의 각 문자 UART 전송
	}
	return len; // 문자열의 모든 문자 전송 후, 전송한 문자 수(len) 반환
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
