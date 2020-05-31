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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GUI.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "MPU6050.h"
#include "math.h"
#include "ESP8266.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define K1        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)  //K1按键PA4
#define K2        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)  //K2按键PA5
#define K3        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)  //K3按键PA8
#define K4        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)  //K2按键PA15
#define D1        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
#define D2        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)
#define D3        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define D4        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define K1_PRES 1
#define K2_PRES 2
#define K3_PRES 0x04
#define K4_PRES 0x08

#define WS_MAIN 0
#define WS_TEST 1
#define WS_MPU 2
#define WS_PWM 3
#define WS_WIFI 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

osThreadId defaultTaskHandle;
osThreadId myTaskGUIHandle;
osThreadId myTaskKeyScanHandle;
osThreadId myTaskMpuHandle;
osThreadId myTaskWifiHandle;
/* USER CODE BEGIN PV */
extern GUI_FLASH const GUI_FONT GUI_FontHZ_SimSun_16;
extern GUI_CONST_STORAGE GUI_BITMAP bmyc;
extern GUI_FLASH const GUI_FONT GUI_FontHZ_SimSun_8;
uint8_t g_ws =WS_MAIN;
uint8_t g_ws_next=WS_TEST;
uint16_t val[20];
uint16_t adval=0;
uint16_t freq=500;
unsigned char key_up=1;     //按键松开标志
// uint8_t wifi_flag=1;
uint8_t g_key_cur=0;
uint8_t ledgo=1;
  int led_type=0;
 int mpuok=0;
 uint8_t num=0;
  uint8_t num1=5;
   uint8_t num2=0;
 uint8_t pwm_flag=0;
 uint8_t show_mpu=0;
uint8_t test_stat=0;
uint8_t rx1_buff[MAX_RECV_LEN] = {0};
uint8_t rx_line[MAX_RECV_LEN]={0};
uint8_t * pBuf = rx1_buff; // 当前接收字节存放位置指针

uint8_t line_flag = 0; // 一行数据接收标志

uint8_t rx2_buff[MAX_RECV_LEN] = {0};
uint8_t rx2_line[MAX_RECV_LEN]={0};


uint8_t line2_flag = 0; // 一行数据接收标志
 uint8_t maxlight = 100;    // 最大亮度占空比
  uint8_t minlight = 1;     // 最低亮度占空比
  char User_ESP8266_TcpServer_IP[4] = {192, 168,4,2}; //要连接的服务器的 IP
char User_ESP8266_TcpServer_Port[10] = "4321";      //要连接的服务器的端口
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void const * argument);
void StartTaskGUI(void const * argument);
void StartTaskKeyScan(void const * argument);
void StartTaskMpu(void const * argument);
void StartTaskWifi(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int check_5m(void)
{
  if(rx_line[4]>='a'&&rx_line[4]<='z'&&rx_line[5]>='a'&&rx_line[5]<='z')
  {
        if('5'+'M'+rx_line[2]+rx_line[3]==(16*(rx_line[4] - 'a' + 10)+rx_line[5] -'a'+10))
      return 1;
      return 0;
  }
   if(rx_line[4]>='a'&&rx_line[4]<='z'&&rx_line[5]>='0'&&rx_line[5]<='9')
   {
     if('5'+'M'+rx_line[2]+rx_line[3]==(16*(rx_line[4] - 'a' + 10)+rx_line[5] -'0'))
      return 1;
      return 0;
   }
  if(rx_line[4]>='A'&&rx_line[4]<='Z'&&rx_line[5]>='A'&&rx_line[5]<='Z')
  {
        if('5'+'M'+rx_line[2]+rx_line[3]==(16*(rx_line[4] - 'A' + 10)+rx_line[5] -'A'+10))
      return 1;
      return 0;
  }
  return 0;
  
}

int check_pwm(void)
{
  if(rx_line[7]>='0' &&rx_line[7]<='9'){
      if('5'+'M'+rx_line[2]+rx_line[3]+rx_line[4]==(16*16*(rx_line[5]-'0')+16*(rx_line[6]-'0')+rx_line[7]-'0'))
        return 1;
        return 0;
  }
   else{
      if('5'+'M'+rx_line[2]+rx_line[3]+rx_line[4]==(16*16*(rx_line[5]-'0')+16*(rx_line[6]-'0')+rx_line[7]-'A'+10))
        return 1;
        return 0;
  }

}



int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff); //
  return 0;
}
void setLED(unsigned char val)
{
	HAL_GPIO_WritePin(D1_GPIO_Port,D1_Pin,(val&0x01)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(D2_GPIO_Port,D2_Pin,(val&0x02)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(D3_GPIO_Port,D3_Pin,(val&0x04)?GPIO_PIN_RESET:GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port,D4_Pin,(val&0x08)?GPIO_PIN_RESET:GPIO_PIN_SET);
}
unsigned char KEY_Scan(unsigned char mode)
{
    
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(K1==0||K2==0||K3==0||K4==0))
    {
        HAL_Delay(100);
        key_up=0;
        if(K1==0)       return K1_PRES;
        else if(K2==0)  return K2_PRES;
        else if(K3==0) return K3_PRES;
				else if(K4==0) return K4_PRES;			
    }else if(K1==1&&K2==1&&K3==1&&K4==1)key_up=1;
    return 0;   //无按键按下
}
void ShowMainGUI(void)
{
  GUI_Clear();
	GUI_DispStringHCenterAt("主菜单",64,0);
	if(g_ws_next>=WS_PWM)
	{
		if((HAL_GetTick()%1000)>500)
		{
			if(WS_MPU==g_ws_next)
        GUI_DispStringAt("*",0,16);
      if(g_ws_next==WS_PWM)
        GUI_DispStringAt("*",0,32);
      if(g_ws_next==WS_WIFI)
        GUI_DispStringAt("*",0,48);
		}
		
		GUI_DispStringAt("姿态解算",16,16);
		GUI_DispStringAt("呼吸灯",16,32);
		GUI_DispStringAt("WIFI通信",16,48);
	}
	else {
    	if((HAL_GetTick()%1000)>500)
		{
			if(WS_TEST==g_ws_next)
        GUI_DispStringAt("*",0,16);
      if(g_ws_next==WS_MPU)
        GUI_DispStringAt("*",0,32);
      if(g_ws_next==WS_PWM)
        GUI_DispStringAt("*",0,48);
		}
		GUI_DispStringAt("系统测试",16,16);
		GUI_DispStringAt("姿态解算",16,32);
		GUI_DispStringAt("呼吸灯",16,48);
	}
  GUI_Update();
}	
void ShowTestGUI(void)
{
  GUI_Clear();
  GUI_DispStringHCenterAt("系统测试",64,0);
   GUI_SetFont(&GUI_Font8_ASCII);
  if(test_stat==0)
  {
     
       
      char buf[30];
      sprintf(buf,"AD:%d",adval);
      GUI_DispStringAt(buf,10,16);

      GUI_FillRect(50,16,50+(adval%1000)*70/1000,17);
      if(adval>=4000)
      GUI_FillRect(50,18,120,19);
      if(adval>=3000)
      GUI_FillRect(50,20,120,21);
      if(adval>=2000)
      GUI_FillRect(50,22,120,23);
      if(adval>=1000)
      GUI_FillRect(50,24,120,25);
      GUI_DispStringAt("press k3 to i/s",10,55);
      GUI_SetFont(&GUI_FontHZ_SimSun_8);
      if(K1==0)
      {
        GUI_DispStringAt("▁",10,30);
        // GUI_SetColor(GUI_COLOR_BLACK);
        // GUI_DispStringAt("K1",10,30);
        // GUI_SetColor(GUI_COLOR_WHITE);
      }
      else {
        GUI_DispStringAt("█",10,30);
      }

      if(K2==0)
      {
        GUI_DispStringAt("▁",30,30);
        // GUI_SetColor(GUI_COLOR_BLACK);
        // GUI_DispStringAt("K2",30,30);
        // GUI_SetColor(GUI_COLOR_WHITE);
      }
      else {
        GUI_DispStringAt("█",30,30);
      }
      if(K3==0)
      {
        GUI_DispStringAt("▁",50,30);
        // GUI_SetColor(GUI_COLOR_BLACK);
        // GUI_DispStringAt("K3",50,30);
        // GUI_SetColor(GUI_COLOR_WHITE);
                  
      }
      else {
        GUI_DispStringAt("█",50,30);
      }
      if(K4==0)
      {
        GUI_DispStringAt("▁",70,30);
        // GUI_SetColor(GUI_COLOR_BLACK);
        // GUI_DispStringAt("K4",70,30);
        // GUI_SetColor(GUI_COLOR_WHITE);
      }
      else {
        GUI_DispStringAt("█",70,30);
      }
      if(D1==0)
      {
        GUI_DispStringAt("●",10,40);
        // GUI_SetColor(GUI_COLOR_BLACK);
        // GUI_DispStringAt("D1",10,40);
        // GUI_SetColor(GUI_COLOR_WHITE);
      }
      else {
        GUI_DispStringAt("○",10,40);
      }
      if(D2==0)
      {
        GUI_DispStringAt("●",30,40);
        // GUI_SetColor(GUI_COLOR_BLACK);
        // GUI_DispStringAt("D2",30,40);
        // GUI_SetColor(GUI_COLOR_WHITE);
      }
      else {
        GUI_DispStringAt("○",30,40);
      }
      if(D3==0)
      {
        GUI_DispStringAt("●",50,40);
        // GUI_SetColor(GUI_COLOR_BLACK);
        // GUI_DispStringAt("D3",50,40);
        // GUI_SetColor(GUI_COLOR_WHITE);
      }
      else {
        GUI_DispStringAt("○",50,40);
      }
      if(D4==0)
      {
        GUI_DispStringAt("●",70,40);
        // GUI_SetColor(GUI_COLOR_BLACK);
        // GUI_DispStringAt("D4",70,40);
        // GUI_SetColor(GUI_COLOR_WHITE);
      }
      else {
        GUI_DispStringAt("○",70,40);
      }
      GUI_SetFont(&GUI_Font8_ASCII);
  }
  else 
  {
    char buf[30];
     
      sprintf(buf,"ax:%6d   gx:%6d",ax,gx);
    GUI_DispStringAt(buf,4,20);

    if(ax>0)
    GUI_FillRect(32,28,32+ax*32/20000,36);
    else
    GUI_FillRect(32+ax*32/20000,28,32,36);

    if(gx>0)
    GUI_FillRect(96,28,96+gx*32/2000,36);
    else
    {
     GUI_FillRect(96+gx*32/2000,28,96,36);
    }
    
    sprintf(buf,"ay:%6d   gy:%6d",ay,gy);
    GUI_DispStringAt(buf,4,36);
    if(ay>0)
    GUI_FillRect(32,44,32+ay*32/20000,50);
    else
    GUI_FillRect(32+ay*32/20000,44,32,50);
    if(gy>0)
    GUI_FillRect(96,44,96+gy*32/2000,50);
    else
    {
     GUI_FillRect(96+gy*32/2000,44,96,50);
    }
    sprintf(buf,"az:%6d   gz:%6d",az,gz);
    GUI_DispStringAt(buf,4,50);
    if(az>0)
    GUI_FillRect(32,58,32+az*32/20000,64);
    else
    GUI_FillRect(32+az*32/20000,58,32,64);
   if(gz>0)
    GUI_FillRect(96,58,96+gz*32/2000,64);
    else
    {
     GUI_FillRect(96+gz*32/2000,58,96,64);
    }
  }
   GUI_Update();
      GUI_SetFont(&GUI_FontHZ_SimSun_16);
}


void ShowMPUGUI(void)
{
  GUI_Clear();
  GUI_DispStringHCenterAt("姿态解算",64,0);
  if(mpuok)
  {
    ;
    char buf[30];
    switch(show_mpu)
    {
      case 0:
      GUI_SetFont(&GUI_Font8_ASCII);
      sprintf(buf,"ax:%6d   gx:%6d",ax,gx);
    GUI_DispStringAt(buf,4,20);
    sprintf(buf,"ay:%6d   gy:%6d",ay,gy);
    GUI_DispStringAt(buf,4,36);
    sprintf(buf,"az:%6d   gz:%6d",az,gz);
    GUI_DispStringAt(buf,4,50);
    GUI_SetFont(&GUI_FontHZ_SimSun_16);
      break;
      case 1:
      
     sprintf(buf,"导航角：%.1f",fAX);
    GUI_DispStringAt(buf,4,16);
     sprintf(buf,"俯仰角：%.1f",fAY);
    GUI_DispStringAt(buf,4,32);
     sprintf(buf,"横滚角：%.1f",fAZ);
    GUI_DispStringAt(buf,4,48);
        
      break;
      case 2:
      {
        // int x1,y1,x2,y2;
        // int ox=64,oy=32;
        // y1=(fAY>0?fAY:-fAY);
        // xl=ox-(16*sin(fAY*6.28/360));
        // yl=oy-(16*sin(fAZ*6.28/360));
        // xr=ox+(16*sin(fAY*6.28/360));
        // yr=oy+(16*sin(fAZ*6.28/360));
        // if(xl>xr)
        // {
        //   int t=xr;
        //   xr=xl;
        //   xl=t;
        // }
        // if(yl>yr)
        // {
        //   int t=yr;
        //   yr=yl;
        //   yl=t;
        // }
        
        // GUI_DrawVLine(xl,yl,yr);
        // GUI_DrawVLine(xr,yl,yr);

        // GUI_DrawHLine(yl,xl,xr);
        // GUI_DrawHLine(yr,xl,xr);
        // printf("xl:%d yl:%d xr:%d yr:%d\r\n ",xl,yl,xr,yr);
        // GUI_FillRect(32,16,96,24+y1*24/180);
       int xl,yl,xr,yr;
        int ox=64,oy=32;
        xl=ox-(16*cos(fAY*6.28/360));
        yl=oy-(16*cos(fAZ*6.28/360));
        xr=ox+(16*cos(fAY*6.28/360));
        yr=oy+(16*cos(fAZ*6.28/360));
        if(xl>xr)
        {
          int t=xr;
          xr=xl;
          xl=t;
        }
        if(yl>yr)
        {
          int t=yr;
          yr=yl;
          yl=t;
        }
        
        GUI_DrawVLine(xl,yl,yr);
        GUI_DrawVLine(xr,yl,yr);

        GUI_DrawHLine(yl,xl,xr);
        GUI_DrawHLine(yr,xl,xr);
      }
      
      break;
    }
    
     
  }
  else
  {
    GUI_DispStringHCenterAt("MPU6050 ERR!",64,20);
  }
  
  GUI_Update();
}
void ShowPWMGUI(void)
{
  GUI_Clear();
   GUI_DispStringHCenterAt("呼吸灯",64,0);
 	GUI_DispStringAt("K1:调节灯数",16,16);
		GUI_DispStringAt("K2:调节频率",16,32);
		GUI_DispStringAt("K3:调节亮度",16,48);
  GUI_Update();
}
void ShowWiFiGUI(void)
{
  char buf[30];
  GUI_Clear();
  GUI_DispStringHCenterAt("WIFI通信",64,0);
  
  GUI_SetFont(&GUI_Font8_ASCII);
GUI_DispStringAt("SSID: ESP8266_bd6a",0,16);
sprintf(buf,"ip:%d.%d.%d.%d",User_ESP8266_TcpServer_IP[0],User_ESP8266_TcpServer_IP[1],User_ESP8266_TcpServer_IP[2],User_ESP8266_TcpServer_IP[3]);
    GUI_DispStringAt(buf,0,24);
    sprintf(buf,"port:%d%d%d%d",User_ESP8266_TcpServer_Port[0]-'0',User_ESP8266_TcpServer_Port[1]-'0',User_ESP8266_TcpServer_Port[2]-'0',User_ESP8266_TcpServer_Port[3]-'0');
    GUI_DispStringAt(buf,0,32);
 	GUI_DispStringAt("K1  :CONNECT TCP SERVER",0,40);
		GUI_DispStringAt("K2  :SEND ARRAY",0,48);
		GUI_DispStringAt("K3:  Unvarnishsend",0,56);
    	
     GUI_SetFont(&GUI_FontHZ_SimSun_16);
  GUI_Update();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) 
{ 
  int i; 
  uint16_t sum = 0; // 20 次采集数据累加和 
  for (i = 0; i < 20; ++i) 
  sum += val[i]; 
  adval = sum / 20;// 均值滤波 
  }

	
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
  ++ pBuf; // 已接收一个字节数据，当前存储位置指针后移
  if(pBuf == rx1_buff + MAX_RECV_LEN) // 如果指针已移出数组边界
    {
      pBuf = rx1_buff; // 重新指向数组开头
    }
  else if(*(pBuf - 1) == '\n') // 如果之前接收到‘\n’换行符，则表示接收完成
  {
    
    *pBuf='\0';
    memcpy(rx_line,rx1_buff,pBuf-rx1_buff+1);
    pBuf=rx1_buff;
    line_flag = 1;
    
  }
      
  __HAL_UNLOCK(UartHandle);
  HAL_UART_Receive_IT(UartHandle, pBuf, 1);
}

void spring(void){
    led_type=0;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
         /*Configure GPIO pins : D1_Pin D2_Pin */
          GPIO_InitStruct.Pin = D1_Pin|D2_Pin;
          GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
          GPIO_InitStruct.Pull = GPIO_NOPULL;
          GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
          HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

          /*Configure GPIO pins : D3_Pin D4_Pin */
          GPIO_InitStruct.Pin = D3_Pin|D4_Pin;
          GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
          GPIO_InitStruct.Pull = GPIO_NOPULL;
          GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
          HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
      printf("hello\n"); // 向串口 1 发送 hello 字符串
      HAL_UART_Receive_IT(&huart1, pBuf, 1); // 开启第一次中断，每次接收 1 字节
      __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
      HAL_UART_Receive_DMA(&huart2,rx2_buff,MAX_RECV_LEN);
	 /*Configure GPIO pins : D1_Pin D2_Pin */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = D1_Pin|D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D3_Pin D4_Pin */
  GPIO_InitStruct.Pin = D3_Pin|D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	setLED(0);
	HAL_Delay(250);
	setLED(0x0f);
	HAL_Delay(250);

	setLED(0x00);
	HAL_Delay(250);
	setLED(0x0f);
	HAL_Delay(250);
	setLED(0x00);

	GUI_Init();
	GUI_SetFont(&GUI_FontHZ_SimSun_16);
	GUI_DispStringAt("专业设计实践II",0,0);
	GUI_DispStringAt("杨宇晨17041129",0,20);
	GUI_Update();
	GUI_Delay(1000);
	GUI_Clear();
	GUI_DrawBitmap(&bmyc,(128-bmyc.XSize)/2,(64-bmyc.YSize)/2);
	GUI_Update();
	
	
	while(K1==1&&K2==1&&K3==1&&K4==1);
  //  MX_TIM3_Init();
	HAL_Delay(250);
	GUI_Clear();
	GUI_Update();
  HAL_ADCEx_Calibration_Start(&hadc1); 
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)val, 20); // 启动 DMA 方式连续采集，每采 20 个数据触发中断
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTaskGUI */
  osThreadDef(myTaskGUI, StartTaskGUI, osPriorityIdle, 0, 256);
  myTaskGUIHandle = osThreadCreate(osThread(myTaskGUI), NULL);

  /* definition and creation of myTaskKeyScan */
  osThreadDef(myTaskKeyScan, StartTaskKeyScan, osPriorityIdle, 0, 256);
  myTaskKeyScanHandle = osThreadCreate(osThread(myTaskKeyScan), NULL);

  /* definition and creation of myTaskMpu */
  osThreadDef(myTaskMpu, StartTaskMpu, osPriorityIdle, 0, 128);
  myTaskMpuHandle = osThreadCreate(osThread(myTaskMpu), NULL);

  /* definition and creation of myTaskWifi */
  osThreadDef(myTaskWifi, StartTaskWifi, osPriorityIdle, 0, 128);
  myTaskWifiHandle = osThreadCreate(osThread(myTaskWifi), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 719;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100;
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
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OLED_SCL_Pin|OLED_SDA_Pin|MPU_SCL_Pin|MPU_SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : K1_Pin K2_Pin K3_Pin K4_Pin */
  GPIO_InitStruct.Pin = K1_Pin|K2_Pin|K3_Pin|K4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_SCL_Pin OLED_SDA_Pin MPU_SCL_Pin MPU_SDA_Pin */
  GPIO_InitStruct.Pin = OLED_SCL_Pin|OLED_SDA_Pin|MPU_SCL_Pin|MPU_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
    
    
    
    
    
    

  /* USER CODE BEGIN 5 */
  uint8_t led_sta=0x01;
  uint32_t led_tick=HAL_GetTick();
 
  uint8_t light_3 = (maxlight - minlight) / 3; // 临时变量

  uint8_t cnt[4] = {minlight, minlight + light_3, minlight + light_3 * 2, maxlight}; 
  uint8_t bdir[4] = {0};	// 亮度变化趋势
  htim3.Instance->CCR1=cnt[0];
  htim3.Instance->CCR2=cnt[1];
  htim3.Instance->CCR3=cnt[2];
  htim3.Instance->CCR4=cnt[3];
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 开启 TIM3 定时器的通道 1PWM 输出 
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
 
  /* Infinite loop */
  for(;;)
  {
     if(line_flag)
    {
      line_flag=0;
      printf("%s",rx_line);
      if(strstr((char *)rx_line,"5M")==(char * )rx_line)
      {
        if(check_pwm()&&rx_line[3]=='M')
        {
   
          num2=rx_line[4]-'0';
          printf("%d\r\n",num2);
        }
         if(check_pwm()&&rx_line[3]=='S')
        {
   
          num1=rx_line[4]-'0';
          printf("%d\r\n",num1);
        }
      }
      if(strstr((char *)rx_line,"AT")==(char * )rx_line)
      HAL_UART_Transmit_DMA(&huart2,rx_line,strlen((char *)rx_line));
      
    }
		if(led_type)
    {
        
        freq=1000-num1*100;
        if(num1==0)
        freq=10000;
        maxlight=(100-num2*10);
        // printf("%d\n\r",maxlight);
              osDelay(freq / (maxlight - minlight));	// 设计 LED 亮度从最低变化到最高需要 500 毫秒
          for (uint8_t i = 0; i < 4; ++i)
          {	// 四个 LED 灯亮度分别变化
                if (0 == bdir[i]) 
                {  // 渐亮变化

              if (++cnt[i] >= maxlight)  
              bdir[i] = 1;

              }
              else 
                {	// 渐暗变化

                  if (--cnt[i] == minlight) 
                  bdir[i] = 0;

                }
          } 
          switch(num)
          {
            case 0:
                htim3.Instance->CCR1=cnt[0];
                htim3.Instance->CCR2=cnt[1];
                htim3.Instance->CCR3=cnt[2];
                htim3.Instance->CCR4=cnt[3];
            break;
            case 1:
                 htim3.Instance->CCR1=0;
                htim3.Instance->CCR2=cnt[1];
                htim3.Instance->CCR3=cnt[2];
                htim3.Instance->CCR4=cnt[3];
            break;
 
            case 2:
                htim3.Instance->CCR1=0;
                htim3.Instance->CCR2=0;
                htim3.Instance->CCR3=cnt[2];
                htim3.Instance->CCR4=cnt[3];
           break;

            case 3:
               htim3.Instance->CCR1=0;
                htim3.Instance->CCR2=0;
                htim3.Instance->CCR3=0;
                htim3.Instance->CCR4=cnt[3];
          break;
          }

        
      
 
    }
 
       setLED(led_sta);
          if(HAL_GetTick()>=led_tick+300&&ledgo==1)
          {
            led_tick=HAL_GetTick();
            led_sta<<=1;
            if(led_sta>0x08)
            led_sta=0x01;
          }
    
    osDelay(1);

  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTaskGUI */
/**
* @brief Function implementing the myTaskGUI thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskGUI */
void StartTaskGUI(void const * argument)
{
  /* USER CODE BEGIN StartTaskGUI */
  /* Infinite loop */
  for(;;)
  {
    switch(g_ws)
		{
			default:
			case WS_MAIN:
      ShowMainGUI();
      spring();
       pwm_flag=0;
				break;
			case WS_TEST:
        ShowTestGUI();
				break;
			case WS_MPU:
        ShowMPUGUI();
				break;
			case WS_PWM:
        ShowPWMGUI();
        pwm_flag++;
        if(pwm_flag==1)
        {
                led_type=1;
                MX_TIM3_Init();
                HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1); 
                HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
                HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_3);
                HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
                maxlight=80;
                HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 开启 TIM3 定时器的通道 1PWM 输出 
                HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
                HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
                HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
                
        }
        
				break;
			case WS_WIFI:
      
      ShowWiFiGUI();
      
      
				break;
		}
    osDelay(1);
  }
  /* USER CODE END StartTaskGUI */
}

/* USER CODE BEGIN Header_StartTaskKeyScan */
/**
* @brief Function implementing the myTaskKeyScan thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskKeyScan */
void StartTaskKeyScan(void const * argument)
{
  /* USER CODE BEGIN StartTaskKeyScan */
  const char TAB[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  /* Infinite loop */
  for(;;)
  {
     g_key_cur=KEY_Scan(0);            //按键扫描
		if(g_key_cur==K4_PRES)
		{
			g_ws=WS_MAIN;
      pwm_flag=0;
     spring();
      
		}
    	switch(g_ws)
		{
			default:
			case WS_MAIN:

					switch(g_key_cur)
						{				 
						case K1_PRES:
							if(g_ws_next>WS_TEST)
								--g_ws_next;
							break;
						case K2_PRES:
							if(g_ws_next<WS_WIFI)
								++g_ws_next; 
							break;
						case K3_PRES:				
							g_ws=g_ws_next;	
             	
							break;
					;
						default:
							HAL_Delay(10);
								break;
						}
						
				break;
			case WS_TEST:
     
        switch(g_key_cur)
        {
          case K3_PRES:
          ledgo=!ledgo;
          break;
          case K1_PRES:
          test_stat=1;
          break;
          case K2_PRES:
          test_stat=0;
        }
				break;
			case WS_MPU:
        switch(g_key_cur)
        {
          case K1_PRES:
          if(show_mpu<1)
        {
          show_mpu++;
        }
        else {
          show_mpu=0;
        }
            break;
          case K2_PRES:
           if(show_mpu>0)
        {
          show_mpu--;
        }
        else {
          show_mpu=1;
        }
          break;
          case K3_PRES:
          show_mpu=2;
          break;
          
        }
        
				break;
			case WS_PWM:
        if(g_key_cur==K4_PRES||K4==0)
        {
          spring();
          
        }
        if(g_key_cur==K1_PRES)
        {
          num++;
          if(num==4)num=0;
        }
        if(g_key_cur==K2_PRES)
        {
          num1++;
          if(num1==10)num1=0;
        }
       if(g_key_cur==K3_PRES)
        {
          num2++;
          if(num2==10)num2=0;
        }
 		  break;
			case WS_WIFI:
        if(g_key_cur==K1_PRES)
        {
          InitESP8266();
        }
       
        if(g_key_cur==K2_PRES&&K1==1&&K3==1&&K4==1)
        {
          char str[80];
          sprintf(str,"5MYD%4d%6d%6d%6d%6d%6d%6d",adval,ax,ay,az,gx,gy,gz);
          uint8_t sum=0;
          uint8_t i,len;
          len=strlen(str);
          for(i=0;i<len;++i)
          {
              sum+=str[i];
          }
          str[len]=TAB[sum>>4];
          str[len+1]='\n';
          str[len+2]='\0';
          HAL_UART_Transmit_DMA(&huart2,(uint8_t*)str,strlen(str));
          printf("%s\n",str);
        }
        if(g_key_cur==K3_PRES&&K1==1&&K2==1&&K4==1)
        {
          ESP8266_ExitUnvarnishSend();
        }
        
        
				break;
		}
    osDelay(1);
  }
  /* USER CODE END StartTaskKeyScan */
}

/* USER CODE BEGIN Header_StartTaskMpu */
/**
* @brief Function implementing the myTaskMpu thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskMpu */
void StartTaskMpu(void const * argument)
{
  /* USER CODE BEGIN StartTaskMpu */
  mpuok=MPU_init();
  
  float temp=0;
  if(!mpuok)
  {
    printf("Error init MPU\n");
  }
  /* Infinite loop */
  for(;;)
  {
		//  if(line_flag)
    // {
    //   line_flag=0;
    //   printf("%s",rx_line);
    //   if(strstr((char *)rx_line,"5M")==(char * )rx_line)
    //   {
        
    //     switch(rx_line[2])
    //     {
    //       case '+':
    //       if(rx_line[3]=='1'&&rx_line[4]=='D'&&rx_line[5]=='E')
    //       break;
    //     }
    //   }
    //   if(strstr((char *)rx_line,"AT")==(char * )rx_line)
    //   HAL_UART_Transmit_DMA(&huart2,rx_line,strlen((char *)rx_line));
      
    // }
    if(mpuok)
    {
      MPU_getdata();
      MPU6050_ReturnTemp(&temp);
      
    }
    osDelay(1);
  }
  /* USER CODE END StartTaskMpu */
}

/* USER CODE BEGIN Header_StartTaskWifi */
/**
* @brief Function implementing the myTaskWifi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskWifi */
void StartTaskWifi(void const * argument)
{
  /* USER CODE BEGIN StartTaskWifi */
  /* Infinite loop */
  for(;;)
  {
    if(line2_flag)
    {
      line2_flag=0;
      printf("<--%s\n",rx2_line);
      memcpy(strEsp8266_Fram_Record .Data_RX_BUF,rx2_line,strlen((char *)rx2_line));
      strEsp8266_Fram_Record.FramLength=strlen((char *)rx2_line);
    }
    osDelay(1);
  }
  /* USER CODE END StartTaskWifi */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
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
