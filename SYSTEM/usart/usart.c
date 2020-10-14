#include "sys.h"
#include "usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

#if 1
#pragma import(__use_no_semihosting)//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout; //定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


#if OPEN_USART1
	UsartSet Usart1_set= NEW_USART1();
#endif
#if OPEN_USART2
  UsartSet Usart2_set= NEW_USART2();
#endif
#if OPEN_USART3
  UsartSet Usart3_set= NEW_USART3();
#endif
#if OPEN_UART4
  UsartSet Uart4_set= NEW_UART4();
#endif
#if OPEN_UART5
  UsartSet Uart5_set= NEW_UART5();
#endif






void USART_init(UsartSet* p,u32 bound){
  
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	//使能USARTX时钟，通道中断
	     if (p->USARTx == USART1)
			 {
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
				  USART_DeInit(USART1);
				  IO_Init(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP,GPIO_Speed_50MHz,1);
				  IO_Init(GPIOA,GPIO_Pin_10,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz,1);
				  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  
			 }
	else if (p->USARTx == USART2)
			 {
				  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
				  USART_DeInit(USART2);
				  IO_Init(GPIOA,GPIO_Pin_2,GPIO_Mode_AF_PP,GPIO_Speed_50MHz,1);
				  IO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz,1);
				  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			 }			 
  else if (p->USARTx == USART3)
			 {
				  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
				  USART_DeInit(USART3);
				  IO_Init(GPIOB,GPIO_Pin_10,GPIO_Mode_AF_PP,GPIO_Speed_50MHz,1);
				  IO_Init(GPIOB,GPIO_Pin_11,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz,1);
				  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
			 }
//  else if (p->USARTx == UART4)
//			 {
//				  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
//				  USART_DeInit(UART4);
//				  IO_Init(GPIOC,GPIO_Pin_10,GPIO_Mode_AF_PP,GPIO_Speed_50MHz,1);
//				  IO_Init(GPIOC,GPIO_Pin_11,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz,1);
//				  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//				  
//			 }
//	else if (p->USARTx == UART5)
//			 {
//				  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
//				  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
//				  USART_DeInit(UART5);
//				  IO_Init(GPIOC,GPIO_Pin_12,GPIO_Mode_AF_PP,GPIO_Speed_50MHz,1);
//				  IO_Init(GPIOD,GPIO_Pin_2,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz,1);
//				  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;  
//			 }   

  //Usartx NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=p->NVIC_IRQChannelPreemptionPriority_K ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = p->NVIC_IRQChannelSubPriority_K;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(p->USARTx, &USART_InitStructure); //初始化串口1
    
			 if(p->MODE==0) USART_ITConfig(p->USARTx,USART_IT_IDLE,DISABLE);
	else USART_ITConfig(p->USARTx,USART_IT_IDLE,ENABLE);//开启IT_IDLE中断
	USART_ITConfig(p->USARTx, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(p->USARTx, ENABLE);                    //使能串口1 
}

 


#if EN_USART1_RX   //如果U1使能了接收
/***********************串口1中断服务程序******************************/	
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.接收状态  bit15接收完成标志  bit14接收到0x0d  bit13~0，	接收到的有效字节数目0x3fff
u16 USART1_RX_STA=0;       //接收状态标记	

		void USART1_IRQHandler(void)
		{
			 u8 Res=0;
			 u8 clear=clear;
			 if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
			 {
					USART_ClearITPendingBit(USART1,USART_IT_RXNE);
					Res=USART_ReceiveData(USART1);	
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res;		 
					USART1_RX_STA++;
			 }
			 else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
			 {
					clear=USART1->SR;
					clear=USART1->DR;
					USART1_RX_STA|=0x8000;  			 
			 }
		 } 
#endif
 


#if EN_USART2_RX   //如果U2使能了接收
/***********************串口2中断服务程序******************************/	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.接收状态  bit15接收完成标志  bit14接收到0x0d  bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	

void USART2_IRQHandler(void)
{
	 u8 Res=0;
	 u8 clear=clear;
	 if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	 {
	    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		  Res=USART_ReceiveData(USART2);	
      USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res;		 
			USART2_RX_STA++;
	 }
	 else if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	 {
		  clear=USART2->SR;
		  clear=USART2->DR;
      USART2_RX_STA|=0x8000;  		 
	 }
 } 
#endif
#if EN_USART3_RX   //如果U2使能了接收
/***********************串口2中断服务程序******************************/	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.接收状态  bit15接收完成标志  bit14接收到0x0d  bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	

void USART3_IRQHandler(void)
{
	 u8 Res=0;
	 u8 clear=clear;
	 if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	 {
	    USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		  Res=USART_ReceiveData(USART3);	
      USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res;		 
			USART3_RX_STA++;
	 }
	 else if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	 {
		  clear=USART3->SR;
		  clear=USART3->DR;
      USART3_RX_STA|=0x8000;  			 
	 }
 } 
#endif
#if EN_UART4_RX   //如果U1使能了接收
/***********************串口1中断服务程序******************************/	
u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.接收状态  bit15接收完成标志  bit14接收到0x0d  bit13~0，	接收到的有效字节数目
u16 UART4_RX_STA=0;       //接收状态标记	

void UART4_IRQHandler(void)
{
	 u8 Res=0;
	 u8 clear=clear;
	 if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	 {
	    USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		  Res=USART_ReceiveData(UART4);	
      UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res;		 
			UART4_RX_STA++;
	 }
	 else if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	 {
		  clear=UART4->SR;
		  clear=UART4->DR;
      UART4_RX_STA|=0x8000;  			 
	 }
 } 
#endif

#if EN_UART5_RX   //如果U1使能了接收
/***********************串口1中断服务程序******************************/	
u8 UART5_RX_BUF[UART5_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.接收状态  bit15接收完成标志  bit14接收到0x0d  bit13~0，	接收到的有效字节数目
u16 UART5_RX_STA=0;       //接收状态标记	

void UART5_IRQHandler(void)
{
	 u8 Res=0;
	 u8 clear=clear;
	 if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	 {
	    USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		  Res=USART_ReceiveData(UART5);	
      UART5_RX_BUF[UART5_RX_STA&0X3FFF]=Res;		 
			UART5_RX_STA++;
	 }
	 else if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	 {
		  clear=UART5->SR;
		  clear=UART5->DR;
      UART5_RX_STA|=0x8000;  			 
	 }
 } 
#endif 
 
 //通用发送一字节
void USART_SendChar(USART_TypeDef* USARTx, uint16_t Data)
{ 
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data));
 
 USARTx->DR = (Data & (uint16_t)0x01FF);
 while(!(USARTx->SR & USART_SR_TC));

}

//通用发送DATA
void USART_SendDat(USART_TypeDef* USARTx, uint8_t *phex,u8 gs)
{
 while(gs>0)
 {
  USART_SendChar(USARTx, *phex++);
	 gs--;
 }
}

//通用发送STRING
void USART_SendString(USART_TypeDef* USARTx, uint8_t *pString)
{
 while(*pString)
 {
  USART_SendChar(USARTx, *pString++);
 }
}




