
#include "sys.h"
#include "delay.h"
#include "key.h"
u8 INT_MARK;//中断标志位




void KEY_INT_INIT (void){	 //按键中断初始化
	NVIC_InitTypeDef  NVIC_InitStruct;	//定义结构体变量
	EXTI_InitTypeDef  EXTI_InitStruct;
   //1.按键端口时钟使能
	IO_Init(GPIOB,GPIO_Pin_4|GPIO_Pin_5,GPIO_Mode_IPU,GPIO_Speed_50MHz ,1);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);       
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //选择端口号（0~15或all）                        
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
//	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	
   //2.开启复用功能时钟，这个是线中断设置必须的。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);//配置端口中断需要启用复用时钟
  //3.设置IO与中断线的映射关系
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);  //定义 GPIO  中断
	//4.中断线初始化配置
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;  //定义中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //中断使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //中断模式为 中断
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //下降沿触发
	EXTI_Init(& EXTI_InitStruct);
	//5.配置中断分组
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;   //中断线    
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);

}
//6.写当前中断程序
void  EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){//判断某个线上的中断是否发生 
		INT_MARK=1;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line4);   //清除 LINE 上的中断标志位
	}     
}
