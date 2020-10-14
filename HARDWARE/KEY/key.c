
#include "sys.h"
#include "delay.h"
#include "key.h"
u8 INT_MARK;//�жϱ�־λ




void KEY_INT_INIT (void){	 //�����жϳ�ʼ��
	NVIC_InitTypeDef  NVIC_InitStruct;	//����ṹ�����
	EXTI_InitTypeDef  EXTI_InitStruct;
   //1.�����˿�ʱ��ʹ��
	IO_Init(GPIOB,GPIO_Pin_4|GPIO_Pin_5,GPIO_Mode_IPU,GPIO_Speed_50MHz ,1);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);       
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //ѡ��˿ںţ�0~15��all��                        
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ѡ��IO�ӿڹ�����ʽ //��������       
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
//	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	
   //2.�������ù���ʱ�ӣ���������ж����ñ���ġ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);//���ö˿��ж���Ҫ���ø���ʱ��
  //3.����IO���ж��ߵ�ӳ���ϵ
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);  //���� GPIO  �ж�
	//4.�ж��߳�ʼ������
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;  //�����ж���
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //�ж�ʹ��
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //�ж�ģʽΪ �ж�
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //�½��ش���
	EXTI_Init(& EXTI_InitStruct);
	//5.�����жϷ���
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;   //�ж���    
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);

}
//6.д��ǰ�жϳ���
void  EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){//�ж�ĳ�����ϵ��ж��Ƿ��� 
		INT_MARK=1;//��־λ��1����ʾ�а����ж�
		EXTI_ClearITPendingBit(EXTI_Line4);   //��� LINE �ϵ��жϱ�־λ
	}     
}
