/* 包含头文件 ----------------------------------------------------------------*/
#include "ESP8266.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "main.h"
#include "GUI.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
//UART_HandleTypeDef husartx_esp8266;
extern UART_HandleTypeDef huart2;
#define husartx_esp8266 huart2

STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
uint8_t esp8266_rxdata;

char Esp8266IPAddress[30] = "192.168.4.1";  // 模块IP地址
char Esp8266SSID[30] = "ESP8266_yyc";      // 模块SSID
char Esp8266PWD[30] = "12345678";           //模块热点的密钥

char User_ESP8266_ApSsid[30] = "111";      //要连接的热点的名称
char User_ESP8266_ApPwd[30] = "dalao666";   //要连接的热点的密钥
extern char User_ESP8266_TcpServer_IP[4] ; //要连接的服务器的 IP
extern char User_ESP8266_TcpServer_Port[10];      //要连接的服务器的端口


/**
  * 函数功能: 重启ESP8266模块
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：被ESP8266_AT_Test调用
  */
void ESP8266_Rst ( void )
{
#if 1
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
#else
	 ESP8266_RST_LOW();
	 HAL_Delay( 500 ); 
	 ESP8266_RST_HIGH(); 
#endif
}

/**
  * 函数功能: 对ESP8266模块发送AT指令
  * 输入参数: cmd，待发送的指令
  *           reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
  *           waittime，等待响应的时间
  * 返 回 值: 1，指令发送成功
  *           0，指令发送失败
  * 说    明：无
  */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, uint32_t waittime )
{
	strEsp8266_Fram_Record .FramLength = 0;               //从新开始接收新的数据包
	char str[80];
	sprintf ( str,"%s\r\n", cmd );
	HAL_UART_Transmit_DMA(&huart2,(uint8_t *)str,strlen(str));
	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	HAL_Delay( waittime );                 //延时
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .FramLength ]  = '\0';

//	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
//  printf("%s->%s\n",cmd,strEsp8266_Fram_Record .Data_RX_BUF);
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

/**
  * 函数功能: 对ESP8266模块进行AT测试启动
  * 输入参数: 无
  * 返 回 值: 1，选择成功
  *           0，选择失败
  * 说    明：无
  */
bool ESP8266_AT_Test ( void )
{
	char count=0;
	
	
//	HAL_Delay(1000 );
	while(count<2)
	{
		if(ESP8266_Cmd("AT","OK",NULL,500)) return 1;
		ESP8266_Rst();
    HAL_Delay(2000);
		++count;
	}
	return 0;
}

/**
  * 函数功能: 选择ESP8266模块的工作模式
  * 输入参数: enumMode，工作模式
  * 返 回 值: 1，选择成功
  *           0，选择失败
  * 说    明：无
  */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	bool result=0;
	char count=0;
	while(count<3)
	{
		switch ( enumMode )
		{
			case STA:
				result=ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 500 ); 
			break;
			case AP:
				result=ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 500 ); 
			break;
			case STA_AP:
				result=ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 500 ); 
			break;
			default:
				result=false;
			break;
		}
		if(result) return result;
		++count;
	}
	return 0;
}

/**
  * 函数功能: ESP8266模块连接外部WiFi
  * 输入参数: pSSID，WiFi名称字符串
  *           pPassWord，WiFi密码字符串
  * 返 回 值: 1，连接成功
  *           0，连接失败
  * 说    明：无
  */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];
	char count=0;
	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
//	while(count<10)
	{
		if(ESP8266_Cmd(cCmd,"OK",NULL,6000))return 1;
		++count;
	}
	return 0;	
}

/**
  * 函数功能: ESP8266模块创建WiFi热点
  * 输入参数: pSSID，WiFi名称字符串
  *           pPassWord，WiFi密码字符串
  *           enunPsdMode，WiFi加密方式代号字符串
  * 返 回 值: 1，创建成功
  *           0，创建失败
  * 说    明：无
  */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];
	char count=0;
	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	while(count<3)
	{
		if(ESP8266_Cmd(cCmd,"OK",0,500))return 1;
		++count;
	}
	return 0;	
}

/**
  * 函数功能: ESP8266模块启动多连接
  * 输入参数: enumEnUnvarnishTx，配置是否多连接
  * 返 回 值: 1，配置成功
  *           0，配置失败
  * 说    明：无
  */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	char count=0;
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	while(count<3)
	{
		if(ESP8266_Cmd(cStr,"OK",0,500))return 1;
		++count;
	}
	return 0;		
}

/**
  * 函数功能: ESP8266模块连接外部服务器
  * 输入参数: enumE，网络协议
  *           ip，服务器IP字符串
  *           ComNum，服务器端口字符串
  *           id，模块连接服务器的ID
  * 返 回 值: 1，连接成功
  *           0，连接失败
  * 说    明：无
  */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%d.%d.%d.%d\",%s", "TCP", ip[0], ip[1], ip[2], ip[3], ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%d.%d.%d.%d\",%s", "UDP", ip[0], ip[1], ip[2], ip[3], ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREADY CONNECTED", 2000 );
	
}

/**
  * 函数功能: ESP8266模块开启或关闭服务器模式
  * 输入参数: enumMode，开启/关闭
  *           pPortNum，服务器端口号字符串
  *           pTimeOver，服务器超时时间字符串，单位：秒
  * 返 回 值: 1，操作成功
  *           0，操作失败
  * 说    明：无
  */
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
}

/**
  * 函数功能: 获取ESP8266 的连接状态，较适合单端口时使用
  * 输入参数: 无
  * 返 回 值: 2，获得ip
  *           3，建立连接
  *           4，失去连接
  *           0，获取状态失败
  * 说    明：无
  */
uint8_t ESP8266_Get_LinkStatus ( void )
{
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		
	}
	return 0;
}

/**
  * 函数功能: 获取ESP8266 的端口（Id）连接状态，较适合多端口时使用
  * 输入参数: 无
  * 返 回 值: 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，某位若置1表该Id建立了连接，若被清0表该Id未建立连接
  * 说    明：无
  */
uint8_t ESP8266_Get_IdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	
	}
	return ucIdLinkStatus;
}

/**
  * 函数功能: 获取ESP8266 的 AP IP
  * 输入参数: pApIp，存放 AP IP 的数组的首地址
  *           ucArrayLength，存放 AP IP 的数组的长度
  * 返 回 值: 1，获取成功
  *           0，获取失败
  * 说    明：无
  */
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	
	char * pCh;
	
	
  ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 500 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"" );
	
	if ( pCh )
		pCh += 6;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = * ( pCh + uc);
		
		if ( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
	}
	return 1;
}

/**
  * 函数功能: 获取ESP8266 的 STA IP
  * 输入参数: pStaIp，存放 STA IP 的数组的首地址
  *           ucArrayLength，存放 STA IP 的数组的长度
  * 返 回 值: 1，获取成功
  *           0，获取失败
  * 说    明：无
  */
uint8_t ESP8266_Inquire_StaIp ( char * pStaIp, uint8_t ucArrayLength )
{
	char uc;
	char * pCh;
  ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 500 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STAIP,\"" );
	
	if ( pCh )
		pCh += 7;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pStaIp [ uc ] = * ( pCh + uc);
		
		if ( pStaIp [ uc ] == '\"' )
		{
			pStaIp [ uc ] = '\0';
			break;
		}
	}
	return 1;
}

/**
  * 函数功能: 获取ESP8266 的 AP MAC
  * 输入参数: pApMac，存放 AP MAC 的数组的首地址
  *           ucArrayLength，存放 AP MAC 的数组的长度
  * 返 回 值: 1，获取成功
  *           0，获取失败
  * 说    明：无
  */
uint8_t ESP8266_Inquire_ApMac( char * pApMac, uint8_t ucArrayLength )
{
	char uc, tc;
	char * pCh;
  ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 1500 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APMAC,\"" );
	
	if ( pCh )
		pCh += 7;
	else
		return 0;
	
	for ( uc = 0, tc = 0; uc < ucArrayLength; uc ++, tc++ )
	{
    if (*(pCh + uc) == ':')
    {
      --tc;
      continue;
    }
		pApMac [ tc ] = * ( pCh + uc);
		
		if ( pApMac [ tc ] == '\"' )
		{
			pApMac [ tc ] = '\0';
			break;
		}
	}
	return 1;
}

/**
  * 函数功能: 配置ESP8266模块进入透传发送
  * 输入参数: 无
  * 返 回 值: 1，配置成功
  *           0，配置失败
  * 说    明：无
  */
bool ESP8266_UnvarnishSend ( void )
{
	
	if ( ! ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 1000 ) )
		return false;
	return  ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 1000 );
	
}

/**
  * 函数功能: 配置ESP8266模块退出透传模式
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void ESP8266_ExitUnvarnishSend ( void )
{
	HAL_Delay(1000);	
	HAL_UART_Transmit_DMA(&huart2,"+++",3);	
	HAL_Delay(500);
}

/**
  * 函数功能: ESP8266模块发送字符串
  * 输入参数: enumEnUnvarnishTx，声明是否已使能了透传模式
  *           pStr，要发送的字符串
  *           ucId，哪个ID发送的字符串
  *           ulStrLength，要发送的字符串的字节数
  * 返 回 值: 1，发送成功
  *           0，发送失败
  * 说    明：无
  */
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
	
		
	if ( enumEnUnvarnishTx )
	{
		HAL_UART_Transmit_DMA ( &huart2, (uint8_t *)pStr ,ulStrLength);
		
		bRet = true;
		
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Cmd ( cStr, "> ", 0, 1000 );

		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;

}

/**
  * 函数功能: ESP8266模块接收字符串
  * 输入参数: enumEnUnvarnishTx，声明是否已使能了透传模式
  * 返 回 值: 接收到的字符串首地址
  * 说    明：无
  */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	
	strEsp8266_Fram_Record .FramLength = 0;

	
	while(strEsp8266_Fram_Record .FramLength==0);
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
	return pRecStr;	
}

uint8_t InitESP8266(void)
{
			uint8_t cnt = 0;
  static int ret = 1;
	HAL_UART_Transmit_DMA(&huart2,"+++",3)	;
	HAL_Delay(500);
  if(ESP8266_AT_Test())
  {
    printf("AT test OK\n");

  }
  else
	{
	
		ret=1;	
		
    return ret;
	}
  if(ret<2)
	{
					 ret = 2;
				printf("<1> Set 8266 net mode...\n");
				if(ESP8266_Net_Mode_Choose(STA_AP))
				{
					printf("ESP8266_Net_Mode_Choose OK\n");
	
				}
				
				if (ESP8266_Inquire_ApMac(Esp8266IPAddress, 30))
				{
					sprintf(Esp8266SSID, "ESP8266_%s", Esp8266IPAddress + 8);
					ESP8266_BuildAP(Esp8266SSID, Esp8266PWD, WPA2_PSK);
				}
				else
					{
						ret=1;

						return ret;
					}
					
	}
	if(ret<3)
	{
			 ret = 3;
		printf("<2> Jion Wifi AP:%s, use password:%s\n", User_ESP8266_ApSsid, User_ESP8266_ApPwd);

		while(!ESP8266_JoinAP(User_ESP8266_ApSsid, User_ESP8266_ApPwd))
			if (++cnt > 1)
			{
				ret=2;
				
				return ret;
			}
	}
	
 if(ret<4)
 {
  ret = 4;
	printf("<3> Set single link\n");
//	 ESP8266_Cmd ( "AT+CIPMODE=0", "OK", 0, 1000 );
  ESP8266_Enable_MultipleId(DISABLE);
//	 ESP8266_StartOrShutServer(ENABLE,"333","50");
  
  if (ESP8266_Inquire_StaIp(Esp8266IPAddress, 30))
    {
		printf("ESP8266 IP Address:%s\n", Esp8266IPAddress);
	}
	else
	{
		ret=3;
	
		return ret;
	}
 }
 
  
 
    if(ret<5)
		{
				ret = 5;
			printf("<4> Link to tcp server %d.%d.%d.%d.:%s\n", 
						User_ESP8266_TcpServer_IP[0], User_ESP8266_TcpServer_IP[1], 
						User_ESP8266_TcpServer_IP[2], User_ESP8266_TcpServer_IP[3], User_ESP8266_TcpServer_Port);
			cnt = 0;
			while(!ESP8266_Link_Server(enumTCP,User_ESP8266_TcpServer_IP,User_ESP8266_TcpServer_Port,Single_ID_0))
				if (++cnt > 1)
				{
					ret=4;
					return ret;
				}
		}
  
  
  ret = 6;
	printf("<5> Begin unvarnishSend...\n");
  cnt = 0;
  while(!ESP8266_UnvarnishSend())
    if (++cnt > 10)
		{
			ret=5;
      return ret;
		}
	printf("配置 ESP8266 完毕\n");
  return 0;
}

int tcp_go(void)
{
	static int ret=0;
	int cnt;
	
if(ret<5)
		{
				ret = 5;
			ESP8266_Cmd ( "AT+CIPSERVER=0", "OK", 0, 500 );
			ESP8266_Cmd ( "AT+CIPMUX=0", "OK", 0, 500 );
			printf("<4> Link to tcp server %d.%d.%d.%d.:%s\n", 
						User_ESP8266_TcpServer_IP[0], User_ESP8266_TcpServer_IP[1], 
						User_ESP8266_TcpServer_IP[2], User_ESP8266_TcpServer_IP[3], User_ESP8266_TcpServer_Port);
			cnt = 0;
			while(!ESP8266_Link_Server(enumTCP,User_ESP8266_TcpServer_IP,User_ESP8266_TcpServer_Port,Single_ID_0))
				if (++cnt > 1)
				{
					ret=4;
					return ret;
				}
		}
  
  
  ret = 6;
	printf("<5> Begin unvarnishSend...\n");
  cnt = 0;
  while(!ESP8266_UnvarnishSend())
    if (++cnt > 10)
		{
			ret=5;
      return ret;
		}
			printf("配置 ESP8266 tcp完毕\n");
  return 0;

}
