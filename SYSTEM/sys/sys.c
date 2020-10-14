#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 

//********************************************************************************  
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

void IO_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,GPIOMode_TypeDef GPIO_Mode_x,GPIOSpeed_TypeDef GPIO_Speed_x ,u8 sta)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 //PE1	
 if (GPIOx == GPIOA)
 {
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //??????
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;				 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x; 		 //????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_x;		 //IO????50MHz
     GPIO_Init(GPIOA, &GPIO_InitStructure);					 //?????????
	   if(sta) GPIO_SetBits(GPIOA,GPIO_Pin_x);						 // ???
		 else    GPIO_ResetBits(GPIOA,GPIO_Pin_x);						 // ???    
 }
 else if (GPIOx == GPIOB)
 {
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //??????
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;				 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x; 		 //????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_x;		 //IO????50MHz
     GPIO_Init(GPIOB, &GPIO_InitStructure);					 //?????????
	   if(sta) GPIO_SetBits(GPIOB,GPIO_Pin_x);						 // ???
		 else    GPIO_ResetBits(GPIOB,GPIO_Pin_x);						 // ???    
 }
 else if (GPIOx == GPIOC)
 {
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //??????
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;				 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x; 		 //????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_x;		 //IO????50MHz
     GPIO_Init(GPIOC, &GPIO_InitStructure);					 //?????????
	   if(sta) GPIO_SetBits(GPIOC,GPIO_Pin_x);						 // ???
		 else    GPIO_ResetBits(GPIOC,GPIO_Pin_x);						 // ???    
 }
 else if (GPIOx == GPIOD)
 {
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //??????
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;				 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x; 		 //????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_x;		 //IO????50MHz
     GPIO_Init(GPIOD, &GPIO_InitStructure);					 //?????????
	   if(sta) GPIO_SetBits(GPIOD,GPIO_Pin_x);						 // ???
		 else    GPIO_ResetBits(GPIOD,GPIO_Pin_x);						 // ???    
 }
  else if (GPIOx == GPIOE)
 {
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //??????
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;				 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x; 		 //????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_x;		 //IO????50MHz
     GPIO_Init(GPIOE, &GPIO_InitStructure);					 //?????????
	   if(sta) GPIO_SetBits(GPIOE,GPIO_Pin_x);						 // ???
		 else    GPIO_ResetBits(GPIOE,GPIO_Pin_x);						 // ???    
 }
 else if (GPIOx == GPIOF)
 {
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //??????
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;				 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x; 		 //????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_x;		 //IO????50MHz
     GPIO_Init(GPIOF, &GPIO_InitStructure);					 //?????????
	   if(sta) GPIO_SetBits(GPIOF,GPIO_Pin_x);						 // ???
		 else    GPIO_ResetBits(GPIOF,GPIO_Pin_x);						 // ???    
 }
 else if (GPIOx == GPIOG)
 {
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //??????
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;				 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x; 		 //????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_x;		 //IO????50MHz
     GPIO_Init(GPIOG, &GPIO_InitStructure);					 //?????????
	   if(sta) GPIO_SetBits(GPIOG,GPIO_Pin_x);						 // ???
		 else    GPIO_ResetBits(GPIOG,GPIO_Pin_x);						 // ???    
 }
}
