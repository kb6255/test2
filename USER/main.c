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


u8 ramin[256];//存储格式映像
u16 temp=0;//温度
u16 hum=0;//湿度
u16 abc=0;
u8 cmd[6]={0};
u8 usart1_buf[11]={0};
u8 dbug=0;
u8 LYID=0;//蓝牙连接状态

//一组数据为8个字节
u8  write_buf[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};// 温度*2，湿度*2 时钟*4
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
void F_jisuan(u16 a,u16 b)//计算取余  0x0b
{
		u32 i;
	i=a%b;
	printf("取余=%d\r\n",i);
}
/**************************************************************************
功能：//ram写入256ram
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
	  if(dbug)printf("写入页数设定为%d页\r\n",abc);
}

void F_dbug(u8 x)
{
		if(x)dbug=1;
	  else dbug=0;
	 
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	 PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	 BKP_WriteBackupRegister(BKP_DR3, dbug);	//向指定的后备寄存器中写入用户程序数据
	 PWR_BackupAccessCmd(DISABLE);
	 printf("dbug=%d\r\n",dbug);
}
void read_Dbug()//读取DBUG
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	 PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	 dbug=BKP_ReadBackupRegister(BKP_DR3);
	 PWR_BackupAccessCmd(DISABLE);
	 if(dbug==1)printf("待机间隔:%d秒\r\n",dbug);
}


void pin_init()//指示灯
{
  IO_Init(GPIOB,GPIO_Pin_2,GPIO_Mode_Out_PP,GPIO_Speed_50MHz ,0);
	#define  LED0 PBout(2)//报警指示灯
	IO_Init(GPIOB,GPIO_Pin_6,GPIO_Mode_Out_PP,GPIO_Speed_50MHz ,1);
	#define SYS_EN PBout(6)//系统供电使能端
	IO_Init(GPIOC,GPIO_Pin_13,GPIO_Mode_IPD,GPIO_Speed_50MHz ,1);
	#define PCIN13 PCin(13)//蓝牙识别端
}




u8 DJJG=20;//待机间隔
void SET_DJJG(u16 a)//设定待机间隔
{
   DJJG=a;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	 PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	 BKP_WriteBackupRegister(BKP_DR2, DJJG);	//向指定的后备寄存器中写入用户程序数据
	 PWR_BackupAccessCmd(DISABLE);
	if(dbug==1)printf("待机间隔:%d秒\r\n",DJJG);
		 
}

void read_DJJG()//读取待机间隔
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	 PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	 DJJG=BKP_ReadBackupRegister(BKP_DR2);
	 PWR_BackupAccessCmd(DISABLE);
	if(dbug==1)printf("待机间隔:%d秒\r\n",DJJG);
}














//以下是W25Q64
/**************************************************************************

**************************************************************************/
void flash_jczx()//检测在线
{ 	
	  u32 i;
	  i=SPI_Flash_ReadID();
		if(i!=W25Q64)							
		{	
      printf("器件未找到,ID=%x(%d)/r/n",i,i);			
		}
		else printf("已连接到W25Q64,ID=%x(%d)\r\n",i,i);
}
/**************************************************************************
**************************************************************************/

//Addr需要全生命周期保持，掉电和关机要进行保存 1048576 每分钟采集可用2年   512次写入为1个扇区
void flash_write(u8 addr,u16 temp,u16 damp,u16 timer)//写入数据(存储地址，长度）
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

void flash_read(u8 addr)//读出数据（目标数组，读取地址，长度）
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

void Erase_Chip(void) //整片擦除
{
	SPI_Flash_Erase_Chip(); 
	printf("整片擦除完成！\r\n");
}

/**************************************************************************
(扇区)全片连续写入
**************************************************************************/

void write_Chip(void) //整片连续写入
{
	u8 num2=0;
	u32 addr=0;
	u32 j,k,i;
           for(i=0;i<abc;i++)//写扇区	16
						{
						    for(k=0;k<32;k++)//写满页
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
			          printf("写入%d页\r\n",k);
						}
					  printf("写入%d扇区r\n",i);
		

}
/****************************************************************************/
void Erase_Sector(u16 Dst_Addr)//擦除扇区 Dst_Addr=0~16*128     0x04
{
	SPI_Flash_Erase_Sector(Dst_Addr);
	if(dbug==1)printf("扇区擦除完成！\r\n");
}
void F_SPI_Flash_PowerDown(void)//W25Q64休眠 0X0F
{
		SPI_Flash_PowerDown();
}

void F_SPI_Flash_WAKEUP(void)//w25q64唤醒 0X10
{
	 SPI_Flash_WAKEUP();
}

/**************************************************************************
功能：读取数据
**************************************************************************/
void Read_Chip(u16 x) //整片连续读出 0x07
{
	u32 add=0;
	u32 i=0,j,k;
	// SPI_Flash_WAKEUP();//唤醒
	delay_ms(2);
	for(i=0;i<x;i++)//写扇区	16
	{
			for(k=0;k<32;k++)//写满页
			{
				      SPI_Flash_Read(Read_Buf,add*8,8);   
							  for(j=0;j<8;j++)
								{ 
									 printf("num=%x,",Read_Buf[j]);
								}
								printf("addr=%d\r\n",add);
								add+=1;
			

			}
			printf("读出%d页\r\n",i+1);
	}	
	   // printf("读出%d扇区！\r\n\r\n",i+1);
	   // SPI_Flash_PowerDown();

}







void F_SPI_FLASH_BufferWrite(void)//连续写入（自动擦除）0x0c
{
	u8 Addr=0,num=0;
	u16 i,j,k;
	  
	  for(k=0;k<abc;k++)//扇编程
	  {
		  for(j=0;j<32;j++)//页编程
			 {
					for(i=0;i<8;i++)//8字节一组
					{
						 write_buf[i]=num;
						 printf("num=%d,",num);
						 num++;
					}	
					SPI_FLASH_BufferWrite(write_buf, Addr*8, 8);//连续写入（自动擦除）
					Addr++;
					printf("Addr=%d\r\n",Addr);
					//printf("写入8字节\r\n");
			 }
	     printf("页编程\r\n");
		}
		printf("扇编程\r\n");		
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
	 if(sanqu>2048)	printf("已经写满\r\n");
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
//功能：//W25Q64按页写入，在新扇区时擦除,到达第32768时循环存储
//**************************************************************************/
//u16 addr=0;
//void F_SPI_Flash_Write_Page(void)//W25Q64按页写入，在新扇区时擦除,到达第32768时循环存储  0X0E
//{
//	  if(addr>=32768)addr=0;//循环写入		
//	  if(addr%16==0){//在用新扇区前做扇区擦除
//		  SPI_Flash_Erase_Sector(addr/16);
//		  if(dbug==1)printf("正在擦除%d扇区",addr/16);
//		}
//    SPI_Flash_Write_Page(ramin,addr*256, 256);
//	  addr++;     
//	  if(dbug==1)printf("%d写入完成\r\n",addr);
//}

void sht20_read(void);//sht20读取温湿度 0x14
void IN_STANDBY(void);//进入待机 0x11
u32 addr=0;
void W25Q16_write(void)
{
	//  u8 i;
    u32 t;
	 	
	if(addr>32768)addr=0;//循环写入	
	if(addr%512==0)//使用前擦除
	{
	   	SPI_Flash_Erase_Sector(addr/512);
		  if(dbug==1)printf("正在擦除%d扇区",addr/32);
	}
	  sht20_read();//更新温湿度
	  printf("温度：%d,湿度：%d\r\n",temp,hum);
	 write_buf[0]=temp>>8;    
	 write_buf[1]=temp; 
  	
	 write_buf[2]=hum>>8;     
	 write_buf[3]=hum;
   t=RTC_GetCounter();//读取时间值
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
	 if(dbug==1)printf("%d写入完成\r\n",addr);
	 addr++;
   if(dbug)printf("addr=%d\r\n",addr);
}


 void W25Q64_SET_addr(u16 x)//设定addr地址
{
   addr=x;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	 PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	 BKP_WriteBackupRegister(BKP_DR4, addr);	//向指定的后备寄存器中写入用户程序数据
	 PWR_BackupAccessCmd(DISABLE);
	 if(dbug==1)printf("后备addr=%d\r\n",addr);
}

void read_BKPaddr()//读取后备ADDR
{

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	 PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	 addr=BKP_ReadBackupRegister(BKP_DR4);
	 PWR_BackupAccessCmd(DISABLE);
	 if(dbug==1)printf("后备addr=%d\r\n",addr);
}










































//以下是SHT20函数

/**************************************************************************
功能：//sht20初始化
**************************************************************************/
void sht20_Init(void)
{	
	IIC_Init();										//IIC???
	
  if(dbug==1)	printf(" Hardware init OK\r\n");
	
}
/**************************************************************************
功能：//sht20读取温湿度
**************************************************************************/
void sht20_read()//sht20读取温湿度 0x14
{
	SHT20_GetValue();
	temp=(u16)(sht20_info.tempreture*100);
	hum=(u16)(sht20_info.humidity*100);
	if(dbug==1)printf("温度=%d,湿度=%d\r\n",temp,hum);

}





















//以下是RTC函数
/**************************************************************************
功能：获得时间秒数,wendu,shidu  baocunzai ram
**************************************************************************/
//void F_RTC_GetCounter()
//{
//	u8 i;
//	static u8 ramcount=0;
//	u32 timecount=0;
//	char humidity;           //定义一个变量，保存湿度值
//  char temperature;        //定义一个变量，保存温度值
//	              
//	if(DHT11_Read_Data(&temperature,&humidity)==0)  		//读取温湿度值
//	{
//		    timecount=RTC_GetCounter();//读取日期时间
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
//				    SPI_Flash_WAKEUP();//唤醒
//					  delay_ms(2);
//					  F_SPI_Flash_Write_Page();	
//					  SPI_Flash_PowerDown();
//				}
//							
//	}
//	
//	  
//}
void RTC_xwsj(void)//询问时间0x13
{
	
   printf("时间：%d:%d:%d\r\n",calendar.hour,calendar.min,calendar.sec);
}


void IN_STANDBY(void)//进入待机 0x11
{
     //保存ADDR
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		 PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
		 BKP_WriteBackupRegister(BKP_DR4, addr);	//向指定的后备寄存器中写入用户程序数据
		 PWR_BackupAccessCmd(DISABLE);
		//向蓝牙发送休眠
	   SYS_EN=0;
		
	  #if DBUG
    printf("进入待机...... \r\n");
	  #endif
		RTC_ClearITPendingBit(RTC_IT_ALR);//清闹钟标志
	  RTC_WaitForLastTask();	
		
	  PWR_BackupAccessCmd(ENABLE);  //
		RTC_EnterConfigMode();	
	  RTC_WaitForLastTask();
		RTC_SetAlarm(DJJG+RTC_GetCounter());	 // 设置闹钟时长
		RTC_WaitForLastTask();
		RTC_ExitConfigMode(); 
    RTC_WaitForLastTask();  
		PWR_BackupAccessCmd(DISABLE);
	  RTC_ITConfig(RTC_IT_ALR, ENABLE);//打开闹钟唤醒中断
		RTC_WaitForLastTask();
	  delay_ms(2);
	  PWR_EnterSTANDBYMode();
}

void SET_clock (int a,u8 b,u8 c,u8 d,u8 e,u8 f)//设置时钟0x12
{
   printf("b\r\n");
}









//以下是蓝牙唤醒处理
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


						 












































/*待机模式RTC唤醒：
1.要开启闹钟中断才能唤醒，秒中断没有唤醒功能。
2.在闹钟中断中要重新设置下一次的闹钟值，然后进入待机模式。

 

*/









int main(void)
{
	u32 i=0;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	pin_init();
	SYS_EN=1;//供电
	USART_init(&Usart1_set,115200);
	USART_init(&Usart2_set,9600);
	delay_init();
	IIC_Init();
	RTC_Init();//初始化待机唤醒
	SPI_Flash_Init();
	sht20_Init();
	read_DJJG();//读取待机间隔设定值
	read_BKPaddr();
	printf("addr=%d\r\n",addr);
	f_page();//写256进ram
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
		   W25Q16_write();//采集温湿度和时间顺序保存到W25Q64
			printf("数据保存完成！！！\r\n");
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




















