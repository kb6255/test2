#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
/********************************************************************************************************/
//串口参数结构
typedef struct 
{
	  USART_TypeDef * USARTx;     //USARTx端口  
	  uint8_t    NVIC_IRQChannelPreemptionPriority_K; //中断抢占优先级
	  uint8_t    NVIC_IRQChannelSubPriority_K;        //中断从优先级
	  u8         MODE;             //开启IT_IDLE中断 
}UsartSet;
/*******************************************************************************************************
                     开启串口
*********************************************************************************************************/
#define  OPEN_USART1  1
#define  OPEN_USART2  1
#define  OPEN_USART3  0
#define  OPEN_UART4   0
#define  OPEN_UART5   0
/********************************************************************************************************
                  串口1开启及参数设置
*********************************************************************************************************/
#if OPEN_USART1
  #define USART1_REC_LEN  			200  	//定义最大接收字节数 200
  #define EN_USART1_RX 			     1		//串口中断接收使能（1）/禁止（0）串口1接收  
//USARTx端口  
//中断抢占优先级
//中断从优先级
//开启IT_IDLE中断
  #define NEW_USART1() \
   { \
		USART1,\
    2,\
    1,\
    1 \
   }
  extern UsartSet Usart1_set;//USART1 初始化结构变量
	extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
	extern u16 USART1_RX_STA;         		//接收状态标记	
#endif
	 
/********************************************************************************************************/
	 //USART2设置
#if OPEN_USART2
	#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
  #define EN_USART2_RX 			     1		//串口中断接收使能（1）/禁止（0）串口1接收
  #define NEW_USART2() \
   { \
		USART2,\
    3,\
    1,\
    1 \
   }
  extern UsartSet Usart2_set;//USART2 初始化结构变量
	extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
	extern u16 USART2_RX_STA;         		//接收状态标记	
#endif
/********************************************************************************************************/
	 //USART3设置
#if OPEN_USART3
	#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
  #define EN_USART3_RX 			     1		//串口中断接收使能（1）/禁止（0）串口1接收
  #define NEW_USART3() \
   { \
		USART3,\
    3,\
    2,\
    1 \
   }
  extern UsartSet Usart3_set;//USART2 初始化结构变量
	extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
	extern u16 USART3_RX_STA;         		//接收状态标记	
#endif
/********************************************************************************************************/
	 //USART4设置
#if OPEN_UART4
	#define UART4_REC_LEN  			200  	//定义最大接收字节数 200
  #define EN_UART4_RX 			     1		//串口中断接收使能（1）/禁止（0）串口1接收
  #define NEW_UART4() \
   { \
		UART4,\
    3,\
    2,\
    1 \
   }
  extern UsartSet Uart4_set;//USART4 初始化结构变量
	extern u8  UART4_RX_BUF[UART4_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
	extern u16 UART4_RX_STA;         		//接收状态标记	
#endif
/********************************************************************************************************/
	 //USART5设置
#if OPEN_UART5
	#define UART5_REC_LEN  			200  	//定义最大接收字节数 200
  #define EN_UART5_RX 			     1		//串口中断接收使能（1）/禁止（0）串口1接收
  #define NEW_UART5() \
   { \
		UART5,\
    3,\
    2,\
    1 \
   }
  extern UsartSet Uart5_set;//USART5 初始化结构变量
	extern u8  UART5_RX_BUF[UART5_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
	extern u16 UART5_RX_STA;         		//接收状态标记	
#endif

/***************************************************************************************
                            //通用外部函数
****************************************************************************************/
void USART_SendChar(USART_TypeDef* USARTx, uint16_t Data);
void USART_init(UsartSet* p,u32 bound);
//通用发送DATA
void USART_SendDat(USART_TypeDef* USARTx, uint8_t *phex,u8 gs);
//通用发送STRING
void USART_SendString(USART_TypeDef* USARTx, uint8_t *pString);
#endif


