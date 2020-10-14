#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
/********************************************************************************************************/
//���ڲ����ṹ
typedef struct 
{
	  USART_TypeDef * USARTx;     //USARTx�˿�  
	  uint8_t    NVIC_IRQChannelPreemptionPriority_K; //�ж���ռ���ȼ�
	  uint8_t    NVIC_IRQChannelSubPriority_K;        //�жϴ����ȼ�
	  u8         MODE;             //����IT_IDLE�ж� 
}UsartSet;
/*******************************************************************************************************
                     ��������
*********************************************************************************************************/
#define  OPEN_USART1  1
#define  OPEN_USART2  1
#define  OPEN_USART3  0
#define  OPEN_UART4   0
#define  OPEN_UART5   0
/********************************************************************************************************
                  ����1��������������
*********************************************************************************************************/
#if OPEN_USART1
  #define USART1_REC_LEN  			200  	//�����������ֽ��� 200
  #define EN_USART1_RX 			     1		//�����жϽ���ʹ�ܣ�1��/��ֹ��0������1����  
//USARTx�˿�  
//�ж���ռ���ȼ�
//�жϴ����ȼ�
//����IT_IDLE�ж�
  #define NEW_USART1() \
   { \
		USART1,\
    2,\
    1,\
    1 \
   }
  extern UsartSet Usart1_set;//USART1 ��ʼ���ṹ����
	extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	extern u16 USART1_RX_STA;         		//����״̬���	
#endif
	 
/********************************************************************************************************/
	 //USART2����
#if OPEN_USART2
	#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
  #define EN_USART2_RX 			     1		//�����жϽ���ʹ�ܣ�1��/��ֹ��0������1����
  #define NEW_USART2() \
   { \
		USART2,\
    3,\
    1,\
    1 \
   }
  extern UsartSet Usart2_set;//USART2 ��ʼ���ṹ����
	extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	extern u16 USART2_RX_STA;         		//����״̬���	
#endif
/********************************************************************************************************/
	 //USART3����
#if OPEN_USART3
	#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
  #define EN_USART3_RX 			     1		//�����жϽ���ʹ�ܣ�1��/��ֹ��0������1����
  #define NEW_USART3() \
   { \
		USART3,\
    3,\
    2,\
    1 \
   }
  extern UsartSet Usart3_set;//USART2 ��ʼ���ṹ����
	extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	extern u16 USART3_RX_STA;         		//����״̬���	
#endif
/********************************************************************************************************/
	 //USART4����
#if OPEN_UART4
	#define UART4_REC_LEN  			200  	//�����������ֽ��� 200
  #define EN_UART4_RX 			     1		//�����жϽ���ʹ�ܣ�1��/��ֹ��0������1����
  #define NEW_UART4() \
   { \
		UART4,\
    3,\
    2,\
    1 \
   }
  extern UsartSet Uart4_set;//USART4 ��ʼ���ṹ����
	extern u8  UART4_RX_BUF[UART4_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	extern u16 UART4_RX_STA;         		//����״̬���	
#endif
/********************************************************************************************************/
	 //USART5����
#if OPEN_UART5
	#define UART5_REC_LEN  			200  	//�����������ֽ��� 200
  #define EN_UART5_RX 			     1		//�����жϽ���ʹ�ܣ�1��/��ֹ��0������1����
  #define NEW_UART5() \
   { \
		UART5,\
    3,\
    2,\
    1 \
   }
  extern UsartSet Uart5_set;//USART5 ��ʼ���ṹ����
	extern u8  UART5_RX_BUF[UART5_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	extern u16 UART5_RX_STA;         		//����״̬���	
#endif

/***************************************************************************************
                            //ͨ���ⲿ����
****************************************************************************************/
void USART_SendChar(USART_TypeDef* USARTx, uint16_t Data);
void USART_init(UsartSet* p,u32 bound);
//ͨ�÷���DATA
void USART_SendDat(USART_TypeDef* USARTx, uint8_t *phex,u8 gs);
//ͨ�÷���STRING
void USART_SendString(USART_TypeDef* USARTx, uint8_t *pString);
#endif


