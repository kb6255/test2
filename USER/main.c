#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "xieyi.h"
#include "rtc.h" 
#include "spi.h"
#include "flash.h"
#include "i2c.h"
#include "sht20.h"


u8 ramin[256];//�洢��ʽӳ��
u16 temp=0;//�¶�
u16 hum=0;//ʪ��
u16 abc=0;
u8 cmd[6]={0};
u8 usart1_buf[11]={0};
u8 dbug=0;
u8 LYID=0;//��������״̬

//һ������Ϊ8���ֽ�
u8  write_buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};// �¶�*2��ʪ��*2 ʱ��*4
#define SIZE sizeof(write_buf)
u8 Read_Buf[8]={1,2,3,4,5,6,7,8};

void charASCII(u8 temp) 
{ 
		u8 a[3];
		u8 i;
	    a[0]=temp/100+'0';
	    a[1]=temp/10%10+'0';
	    a[2]=temp%10+'0';

			for(i=0;i<3;i++) USART_SendChar(USART2,a[i] );
	
}
void F_jisuan(u16 a,u16 b)//����ȡ��  0x0b
{
		u32 i;
	i=a%b;
	printf("ȡ��=%d\r\n",i);
}
/**************************************************************************
���ܣ�//ramд��256ram
**************************************************************************/
void f_page()
{
  u16 i;
	for(i=0;i<256;i++)
	{
		ramin[i]=i;
	}
}
/***********************************************************************************************
                       
************************************************************************************************/

u8 rec_buf[6][12];
void F_NUM(u16 x)
{
		abc=x;	 
	  if(dbug)printf("д��ҳ���趨Ϊ%dҳ\r\n",abc);
}

void F_dbug(u8 x)
{
		if(x)dbug=1;
	  else dbug=0;
	 
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	 PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	 BKP_WriteBackupRegister(BKP_DR3, dbug);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	 PWR_BackupAccessCmd(DISABLE);
	 printf("dbug=%d\r\n",dbug);
}
void read_Dbug()//��ȡDBUG
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	 PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	 dbug=BKP_ReadBackupRegister(BKP_DR3);
	 PWR_BackupAccessCmd(DISABLE);
	 if(dbug==1)printf("�������:%d��\r\n",dbug);
}


void pin_init()//ָʾ��
{
  IO_Init(GPIOB,GPIO_Pin_2,GPIO_Mode_Out_PP,GPIO_Speed_50MHz ,0);
	#define  LED0 PBout(2)//����ָʾ��
	IO_Init(GPIOB,GPIO_Pin_6,GPIO_Mode_Out_PP,GPIO_Speed_50MHz ,1);
	#define SYS_EN PBout(6)//ϵͳ����ʹ�ܶ�
	IO_Init(GPIOC,GPIO_Pin_13,GPIO_Mode_IPD,GPIO_Speed_50MHz ,1);
	#define PCIN13 PCin(13)//����ʶ���
}




u8 DJJG=20;//�������
void SET_DJJG(u16 a)//�趨�������
{
   DJJG=a;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	 PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	 BKP_WriteBackupRegister(BKP_DR2, DJJG);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	 PWR_BackupAccessCmd(DISABLE);
	if(dbug==1)printf("�������:%d��\r\n",DJJG);
		 
}

void read_DJJG()//��ȡ�������
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	 PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	 DJJG=BKP_ReadBackupRegister(BKP_DR2);
	 PWR_BackupAccessCmd(DISABLE);
	if(dbug==1)printf("�������:%d��\r\n",DJJG);
}














//������W25Q64
/**************************************************************************

**************************************************************************/
void flash_jczx()//�������
{ 	
	  u32 i;
	  i=SPI_Flash_ReadID();
		if(i!=W25Q64)							
		{	
      printf("����δ�ҵ�,ID=%x(%d)/r/n",i,i);			
		}
		else printf("�����ӵ�W25Q64,ID=%x(%d)\r\n",i,i);
}
/**************************************************************************
**************************************************************************/

//Addr��Ҫȫ�������ڱ��֣�����͹ػ�Ҫ���б��� 1048576 ÿ���Ӳɼ�����2��   512��д��Ϊ1������
void flash_write(u8 addr,u16 temp,u16 damp,u16 timer)//д������(�洢��ַ�����ȣ�
{
	  u8 i;
	   write_buf[0]=temp>>8;
	   write_buf[1]=temp;
	   write_buf[2]=damp>>8;
	   write_buf[3]=damp;
	   write_buf[4]=timer>>24;
	   write_buf[5]=timer>>16;
	   write_buf[6]=timer>>8;
	   write_buf[7]=timer;
	    for(i=0;i<8;i++)
	   {
			 printf("%d",write_buf[i]);
	   }
		 printf("\r\n");
		 SPI_Flash_Write(write_buf,addr*8,8);

	   if(dbug==1)printf("addr=%d\r\n",addr);
}

/**************************************************************************
**************************************************************************/

void flash_read(u8 addr)//�������ݣ�Ŀ�����飬��ȡ��ַ�����ȣ�
{
	 u8 i;
   SPI_Flash_Read((u8*)Read_Buf,addr*8,8);
	 for(i=0;i<8;i++)
	{
		  USART_SendChar(USART1,Read_Buf[i] );
	}
	 if(dbug==1)printf("\r\n");
	 if(dbug==1)printf("addr=%d\r\n",addr);
}



/**************************************************************************
**************************************************************************/

void Erase_Chip(void) //��Ƭ����
{
	SPI_Flash_Erase_Chip(); 
	printf("��Ƭ������ɣ�\r\n");
}

/**************************************************************************
(����)ȫƬ����д��
**************************************************************************/

void write_Chip(void) //��Ƭ����д��
{
	u8 num2=0;
	u32 addr=0;
	u32 j,k,i;
           for(i=0;i<abc;i++)//д����	16
						{
						    for(k=0;k<32;k++)//д��ҳ
						    {
										for(j=0;j<8;j++)
										{ 
											 write_buf[j]=num2;
											 printf("num=%d,",num2);
											 num2++;
										}
									
										SPI_Flash_Write(write_buf,addr*8,8);
										printf("addr=%d\r\n",addr);
										addr++;
								
					
						    }
			          printf("д��%dҳ\r\n",k);
						}
					  printf("д��%d����r\n",i);
		

}
/****************************************************************************/
void Erase_Sector(u16 Dst_Addr)//�������� Dst_Addr=0~16*128     0x04
{
	SPI_Flash_Erase_Sector(Dst_Addr);
	if(dbug==1)printf("����������ɣ�\r\n");
}
void F_SPI_Flash_PowerDown(void)//W25Q64���� 0X0F
{
		SPI_Flash_PowerDown();
}

void F_SPI_Flash_WAKEUP(void)//w25q64���� 0X10
{
	 SPI_Flash_WAKEUP();
}

/**************************************************************************
���ܣ���ȡ����
**************************************************************************/
void Read_Chip(u16 x) //��Ƭ�������� 0x07
{
	u32 add=0;
	u32 i=0,j,k;
	// SPI_Flash_WAKEUP();//����
	delay_ms(2);
	for(i=0;i<x;i++)//д����	16
	{
			for(k=0;k<32;k++)//д��ҳ
			{
				      SPI_Flash_Read(Read_Buf,add*8,8);   
							  for(j=0;j<8;j++)
								{ 
									 printf("num=%x,",Read_Buf[j]);
								}
								printf("addr=%d\r\n",add);
								add+=1;
			

			}
			printf("����%dҳ\r\n",i+1);
	}	
	   // printf("����%d������\r\n\r\n",i+1);
	   // SPI_Flash_PowerDown();

}







void F_SPI_FLASH_BufferWrite(void)//����д�루�Զ�������0x0c
{
	u8 Addr=0,num=0;
	u16 i,j,k;
	  
	  for(k=0;k<abc;k++)//�ȱ��
	  {
		  for(j=0;j<32;j++)//ҳ���
			 {
					for(i=0;i<8;i++)//8�ֽ�һ��
					{
						 write_buf[i]=num;
						 printf("num=%d,",num);
						 num++;
					}	
					SPI_FLASH_BufferWrite(write_buf, Addr*8, 8);//����д�루�Զ�������
					Addr++;
					printf("Addr=%d\r\n",Addr);
					//printf("д��8�ֽ�\r\n");
			 }
	     printf("ҳ���\r\n");
		}
		printf("�ȱ��\r\n");		
}	
	


















void D_write_buf_w(u16 wendu, u16 sidu,u32 x_timer)
{
   static u32 addr=0;
	 static u16 sanqu=0;
	 if(addr>=4069)
	 {
	    SPI_Flash_Erase_Sector(sanqu);
		  SPI_Flash_Write(write_buf,sanqu*4096,4096);
		  sanqu++;
		  addr=0;
	 }
	 if(sanqu>2048)	printf("�Ѿ�д��\r\n");
	 write_buf[addr]=(u8)wendu>>8;    addr++;
	 write_buf[addr]=(u8)wendu;       addr++;
	 write_buf[addr]=(u8)sidu>>8;     addr++;
	 write_buf[addr]=(u8)sidu>>8;     addr++;
	 write_buf[addr]=(u8)x_timer>>24; addr++;
	 write_buf[addr]=(u8)x_timer>>16; addr++;
	 write_buf[addr]=(u8)x_timer>>8;  addr++;
	 write_buf[addr]=(u8)x_timer;     addr++;
	 
}









///**************************************************************************
//���ܣ�//W25Q64��ҳд�룬��������ʱ����,�����32768ʱѭ���洢
//**************************************************************************/
//u16 addr=0;
//void F_SPI_Flash_Write_Page(void)//W25Q64��ҳд�룬��������ʱ����,�����32768ʱѭ���洢  0X0E
//{
//	  if(addr>=32768)addr=0;//ѭ��д��		
//	  if(addr%16==0){//����������ǰ����������
//		  SPI_Flash_Erase_Sector(addr/16);
//		  if(dbug==1)printf("���ڲ���%d����",addr/16);
//		}
//    SPI_Flash_Write_Page(ramin,addr*256, 256);
//	  addr++;     
//	  if(dbug==1)printf("%dд�����\r\n",addr);
//}

void sht20_read(void);//sht20��ȡ��ʪ�� 0x14
void IN_STANDBY(void);//������� 0x11
u32 addr=0;
void W25Q16_write(void)
{
	//  u8 i;
    u32 t;
	 	
	if(addr>32768)addr=0;//ѭ��д��	
	if(addr%512==0)//ʹ��ǰ����
	{
	   	SPI_Flash_Erase_Sector(addr/512);
		  if(dbug==1)printf("���ڲ���%d����",addr/32);
	}
	  sht20_read();//������ʪ��
	  printf("�¶ȣ�%d,ʪ�ȣ�%d\r\n",temp,hum);
	 write_buf[0]=temp>>8;    
	 write_buf[1]=temp; 
  	
	 write_buf[2]=hum>>8;     
	 write_buf[3]=hum;
   t=RTC_GetCounter();//��ȡʱ��ֵ
	  printf("timer=%d,\r\n",t);	
	 write_buf[4]=t>>24;     
	 write_buf[5]=t>>16;      
	 write_buf[6]=t>>8;       
	 write_buf[7]=t;   
   
//	 for(i=0;i<8;i++)
//   {
//			printf("%x\r\n",write_buf[i]);
//	 }	
	 SPI_Flash_Write_Page(write_buf,addr*8, 8);
	 if(dbug==1)printf("%dд�����\r\n",addr);
	 addr++;
   if(dbug)printf("addr=%d\r\n",addr);
}


 void W25Q64_SET_addr(u16 x)//�趨addr��ַ
{
   addr=x;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	 PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	 BKP_WriteBackupRegister(BKP_DR4, addr);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	 PWR_BackupAccessCmd(DISABLE);
	 if(dbug==1)printf("��addr=%d\r\n",addr);
}

void read_BKPaddr()//��ȡ��ADDR
{

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	 PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	 addr=BKP_ReadBackupRegister(BKP_DR4);
	 PWR_BackupAccessCmd(DISABLE);
	 if(dbug==1)printf("��addr=%d\r\n",addr);
}










































//������SHT20����

/**************************************************************************
���ܣ�//sht20��ʼ��
**************************************************************************/
void sht20_Init(void)
{	
	IIC_Init();										//IIC???
	
  if(dbug==1)	printf(" Hardware init OK\r\n");
	
}
/**************************************************************************
���ܣ�//sht20��ȡ��ʪ��
**************************************************************************/
void sht20_read()//sht20��ȡ��ʪ�� 0x14
{
	SHT20_GetValue();
	temp=(u16)(sht20_info.tempreture*100);
	hum=(u16)(sht20_info.humidity*100);
	if(dbug==1)printf("�¶�=%d,ʪ��=%d\r\n",temp,hum);

}





















//������RTC����
/**************************************************************************
���ܣ����ʱ������,wendu,shidu  baocunzai ram
**************************************************************************/
//void F_RTC_GetCounter()
//{
//	u8 i;
//	static u8 ramcount=0;
//	u32 timecount=0;
//	char humidity;           //����һ������������ʪ��ֵ
//  char temperature;        //����һ�������������¶�ֵ
//	              
//	if(DHT11_Read_Data(&temperature,&humidity)==0)  		//��ȡ��ʪ��ֵ
//	{
//		    timecount=RTC_GetCounter();//��ȡ����ʱ��
//		    ramin[ramcount]=0;              ramcount++;
//		    ramin[ramcount]=temperature;    ramcount++;
//		    ramin[ramcount]=0;              ramcount++;
//		    ramin[ramcount]=humidity ;      ramcount++;
//		    ramin[ramcount]=timecount>>24;  ramcount++;
//		    ramin[ramcount]=timecount>>16;  ramcount++;
//		    ramin[ramcount]=timecount>>8;   ramcount++;
//		    ramin[ramcount]=timecount;      ramcount++;
//		    if(dbug==1)
//				{
//				   printf("ramcount=%d\r\n",ramcount);
//		       for(i=8;i>0;i--) printf("ram[%d]=%d,",i,ramin[ramcount-i]);
//		       printf("\r\n");
//				}					
//					
//		    if(ramcount==0)
//				{
//				    SPI_Flash_WAKEUP();//����
//					  delay_ms(2);
//					  F_SPI_Flash_Write_Page();	
//					  SPI_Flash_PowerDown();
//				}
//							
//	}
//	
//	  
//}
void RTC_xwsj(void)//ѯ��ʱ��0x13
{
	
   printf("ʱ�䣺%d:%d:%d\r\n",calendar.hour,calendar.min,calendar.sec);
}


void IN_STANDBY(void)//������� 0x11
{
     //����ADDR
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
		 PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
		 BKP_WriteBackupRegister(BKP_DR4, addr);	//��ָ���ĺ󱸼Ĵ�����д���û���������
		 PWR_BackupAccessCmd(DISABLE);
		//��������������
	   SYS_EN=0;
		
	  #if DBUG
    printf("�������...... \r\n");
	  #endif
		RTC_ClearITPendingBit(RTC_IT_ALR);//�����ӱ�־
	  RTC_WaitForLastTask();	
		
	  PWR_BackupAccessCmd(ENABLE);  //
		RTC_EnterConfigMode();	
	  RTC_WaitForLastTask();
		RTC_SetAlarm(DJJG+RTC_GetCounter());	 // ��������ʱ��
		RTC_WaitForLastTask();
		RTC_ExitConfigMode(); 
    RTC_WaitForLastTask();  
		PWR_BackupAccessCmd(DISABLE);
	  RTC_ITConfig(RTC_IT_ALR, ENABLE);//�����ӻ����ж�
		RTC_WaitForLastTask();
	  delay_ms(2);
	  PWR_EnterSTANDBYMode();
}

void SET_clock (int a,u8 b,u8 c,u8 d,u8 e,u8 f)//����ʱ��0x12
{
   printf("b\r\n");
}









//�������������Ѵ���
void LY_EXE()
{
      LYID=1;
	    USART_init(&Usart2_set,9600);
	    delay_ms(2);
//	    EXTI->EMR|=1<<13;//???line BITx???? (???????,????????,????????????????!)
//     if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx????????
//     if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx?????????
//	    //USART_SendString(USART2, "AT\r\n");
	    
		  
}


						 












































/*����ģʽRTC���ѣ�
1.Ҫ���������жϲ��ܻ��ѣ����ж�û�л��ѹ��ܡ�
2.�������ж���Ҫ����������һ�ε�����ֵ��Ȼ��������ģʽ��

 

*/









int main(void)
{
	u32 i=0;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	pin_init();
	SYS_EN=1;//����
	USART_init(&Usart1_set,115200);
	USART_init(&Usart2_set,9600);
	delay_init();
	IIC_Init();
	RTC_Init();//��ʼ����������
	SPI_Flash_Init();
	sht20_Init();
	read_DJJG();//��ȡ��������趨ֵ
	read_BKPaddr();
	printf("addr=%d\r\n",addr);
	f_page();//д256��ram
	while(1)
	{
		xieyi_scan();  
		delay_ms(1);
		  i++;
		
	    
		  
		if(i%500==0)LED0=!LED0;
		if(PCIN13==1) LYID=1;
		else LYID=0;
		if(i%1000==0)
		{
		   W25Q16_write();//�ɼ���ʪ�Ⱥ�ʱ��˳�򱣴浽W25Q64
			printf("���ݱ�����ɣ�����\r\n");
		}
		  
		
		
		if(LYID==0)
		{
			if(i%1000==0)IN_STANDBY();
		}
		else
		{
		   if(USART2_RX_STA&0x8000) 
			{
				  printf("sjdfl=%d",USART2_RX_BUF[0]);
		      if(USART2_RX_BUF[0]==0x31)
					{
					   printf("lanyaOK1\r\n");
						USART_SendString(USART2, "CMD1\r\n");
					}
					
					else if(USART2_RX_BUF[0]==0x32)
					{
					  printf("lanyacmd2\r\n");
						USART_SendString(USART2, "ok2\r\n");
					}
				  USART2_RX_BUF[0]=0;
				  USART2_RX_STA=0;
			}
		}
		
		
		

		
		
		
	}
}




















