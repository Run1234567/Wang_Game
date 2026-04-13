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
#include "adc.h"
#include "crc.h"
#include "gpdma.h"
#include "i2c.h"
#include "icache.h"
#include "octospi.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "app_touchgfx.h"
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_app.h"		//用户应用程序

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define	ADC_CONVERTED_DATA_BUFFER_SIZE	6	
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t gChatCount = 10;  //全局按键按压计数
extern gTask_MarkEN gTaskEnMark;  //系统任务使能标识
static uint8_t gTaskIndex = 0x00;  //系统任务索引变量
extern volatile uint8_t TcpClosedFlag;	
ADC_ValTypeDef gStruADC={0,0,0,0,0,0}; //A/D通道实时采集的数据
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
/* USER CODE BEGIN PFP */
extern void touchgfx_signalVSynTimer(void);
extern gTask_BitDef gTaskStateBit;  //任务执行过程中使用到的标志位
volatile uint8_t gRX3_BufF[1];	//串口3-wifi模组接收到的数据
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**************************关闭标准库下的半主机模式****************************/
__ASM (".global __use_no_semihosting");	//AC6编译器
//标准库需要的支持函数
struct FILE 
{
  int handle; 
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式  
void _sys_exit(int x) 
{ 
  x = x; 
}
void _ttywrch(int ch)
{
  ch = ch;
}
//printf实现重定向
int fputc(int ch, FILE *f)	
{
	uint8_t temp[1] = {ch};
	HAL_UART_Transmit(&huart1, temp, 1, 2);
	return ch;
}
/******************************************************************************/

/*******************************************************************************
*	函 数 名: OSPI_W25Qxx_mmap
*	入口参数: 无
*	返 回 值: 无
*	函数功能: 设置为内存映射模式
*	说    明: 无	
*******************************************************************************/
void OSPI_W25Qxx_mmap(void)		//Flash读写测试
{
	int32_t OSPI_Status ; 		 //检测标志位
	//
	OSPI_Status = OSPI_W25Qxx_MemoryMappedMode(); //配置OSPI为内存映射模式
	if( OSPI_Status == OSPI_W25Qxx_OK )
	{
		printf ("\r\n内存映射模式设置成功>>>>\r\n");		
	}
	else
	{
		printf ("\r\n内存映射模式设置失败>>>>\r\n");
		Error_Handler();
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

  /* Configure the System Power */
  SystemPower_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_ICACHE_Init();
  MX_OCTOSPI1_Init();
  MX_SPI1_Init();
  MX_CRC_Init();
  MX_I2C1_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_UART5_Init();
  MX_SPI2_Init();
  MX_TouchGFX_Init();
  /* USER CODE BEGIN 2 */
	//ESP8266初始化，HAL库使用USART3
	ESP8266_Init(&huart5,(uint8_t *)gRX3_BufF,115200);
	
	ap3216c_init();	//环境光传感器初始化
  ILI9341_Init();	//显示屏初始化
  FT6336_init();	//触摸屏初始化
	mpu_init_dmp();	//mpu6050 dmp初始化
	System_Time_init();	
  //NOR Flash初始化
  OSPI_W25Qxx_Init();	//初始化W25Q128
  OSPI_W25Qxx_mmap();	//设置为内存映射模式
	HAL_PWREx_EnableVddA();
  HAL_PWREx_EnableVddIO2();
	
	
	
	//读取ADC值
	if (HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&gStruADC,ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)	{Error_Handler();}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Start_IT(&htim16);//开启定时器16开启,系统任务调度开始
  while (1)
  {
    /* USER CODE END WHILE */

  MX_TouchGFX_Process();
    /* USER CODE BEGIN 3 */
		//执行任务列表中的的任务
		
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_0;
  RCC_OscInitStruct.LSIDiv = RCC_LSI_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV4;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 1;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  HAL_PWREx_DisableUCPDDeadBattery();
/* USER CODE BEGIN PWR */
/* USER CODE END PWR */
}

/* USER CODE BEGIN 4 */
//定时器16/17的任务分配
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t p_Time16Cnt = 0,p_Time17Cnt = 0;
	/***************************************************************************************/
	//定时器16进行5ms任务中断
	if (htim->Instance == htim16.Instance) 
	{
		p_Time16Cnt++;
		//
		if(!(p_Time16Cnt % 4))  //20ms(50Hz)进行触发刷新
		{
			touchgfx_signalVSynTimer();  //touchgfx用户接口
		}
	}
}
/**
  * @brief  EXTI line rising detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
	UNUSED(GPIO_Pin);
	//触摸按下事件
	if((!HAL_GPIO_ReadPin(TP_INT_GPIO_Port,TP_INT_Pin)) && (GPIO_Pin == TP_INT_Pin))
	{
		gTaskStateBit.TouchPress = 1;
	}
	if(!HAL_GPIO_ReadPin(USER_KEY_GPIO_Port,USER_KEY_Pin))
	{
		gChatCount = gChatCount + 10;
		if(gChatCount>=100) gChatCount=10;
	}
}
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
	UNUSED(GPIO_Pin);
	//触摸释放事件
	if(HAL_GPIO_ReadPin(TP_INT_GPIO_Port,TP_INT_Pin) && gTaskStateBit.TouchPress)
	{
		FT6336_irq_fuc();	//触摸中断产生
		gTaskStateBit.TouchPress = 0;	//清除触摸标志
	}
	//五向按键按下
	if(HAL_GPIO_ReadPin(EXT_FIVEKEY_GPIO_Port,EXT_FIVEKEY_Pin) && (GPIO_Pin == EXT_FIVEKEY_Pin) && (gTaskStateBit.TouchPress == 0))
	{
		gTaskStateBit.FiveKeyPress = 1; 
	}
}
/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  hadc: ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  //更新DMA传输状态标志
  gTaskStateBit.ADCC = 1;  
}
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//USART3接收数据
	if (huart->Instance == UART5) 
	{
		if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 	//接收到的数据存储至buffer
		{
			//留最后一位做结束位
			ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = gRX3_BufF[0];  
			//UART3开启下一次接收
			HAL_UART_Receive_IT(&huart5,(uint8_t *)&gRX3_BufF, 1);//接收一个字节
		}  
	}

}
/**
  * @brief  UART Abort Receive Complete callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
	//USART3帧传输完成，产生空闲
	if (huart->Instance == UART5)
	{
		ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;	//帧传输完成标志
		TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;	//判断连接
		//UART3开启下一次接收
		HAL_UART_Receive_IT(&huart5,(uint8_t *)&gRX3_BufF, 1);//接收一个字节
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
