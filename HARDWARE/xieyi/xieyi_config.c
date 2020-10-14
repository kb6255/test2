 #include "xieyi.h"
 //1.引用外部函数 
 void F_dbug(u8 x);
 void flash_jczx(void);
 void flash_write(u8 addr,u16 temp,u16 damp,u16 timer);
 void flash_read(u8 addr);
 void Erase_Sector(u16 Dst_Addr);//擦除扇区 Dst_Addr=0~16*128
 void Erase_Chip(void);
 void write_Chip(void);
 void Read_Chip(u16 x); //整片连续读出
 void F_NUM(u16 x);//s设置数据
 void F_jisuan(u16 a,u16 b);//计算取余
 void F_SPI_FLASH_BufferWrite(void);//连续写入（自动擦除）
 //void F_SPI_Flash_Write(u8 addr,u8 DR);//SPI写入一扇区
// void F_SPI_Flash_Write_Page(void);//W25Q64按页写入，在新扇区时擦除,到达第32768时循环存储;
 void F_SPI_Flash_PowerDown(void);//W25Q64休眠
 void F_SPI_Flash_WAKEUP(void);//w25q64唤醒
 
 void SET_clock(void);//设定时钟
 void RTC_xwsj(void);//询问时间
 void sht20_read(void);//sht20读取温湿度
 
 void SET_DJJG(u16 a);//设定待机间隔
 void IN_STANDBY(void);//进入待机
 void read_DJJG(void);//读取待机间隔
 void W25Q16_write(void);//写数据到存储器
 void W25Q64_SET_addr(u16 x);//设定addr地址
//2.填写指令参数格式
  NAME_TAB name_tab[]=
 {
	  (void*)F_dbug,                         {0xaa,0xdb, 1, 0, 0, 0, 0, 0,0,0, 0xbb},
	  (void*)flash_jczx,                     {0xaa,0x01, 0, 0, 0, 0, 0, 0,0,0, 0xbb},
    (void*)flash_write,                    {0xaa,0x02, 1, 2, 2, 3, 3, 4,4,0, 0xbb},
	  (void*)flash_read,                     {0xaa,0x03, 1, 0, 0, 0, 0, 0,0,0, 0xbb},
	  (void*)Erase_Sector,                   {0xaa,0x04, 1, 1, 0, 0, 0, 0,0,0, 0xbb},
	  (void*)Erase_Chip,                     {0xaa,0x05, 0, 0, 0, 0, 0, 0,0,0, 0xbb},
	  (void*)write_Chip,                     {0xaa,0x06, 0, 0, 0, 0, 0, 0,0,0, 0xbb},
	  (void*)Read_Chip,                      {0xaa,0x07, 1, 1, 0, 0, 0, 0,0,0, 0xbb},//8
		(void*)F_NUM,                          {0xaa,0x0a, 1, 1, 0, 0, 0, 0,0,0, 0xbb},
		(void*)F_jisuan,                       {0xaa,0x0b, 1, 1, 2, 2, 0, 0,0,0, 0xbb},
		(void*)F_SPI_FLASH_BufferWrite,        {0xaa,0x0c, 0, 0, 0, 0, 0, 0,0,0, 0xbb},
		//(void*)F_SPI_Flash_Write,              {0xaa,0x0d, 1, 2, 0, 0, 0, 0,0,0, 0xbb},
		//(void*)F_SPI_Flash_Write_Page,         {0xaa,0x0e, 0, 0, 0, 0, 0, 0,0,0, 0xbb},
		(void*)F_SPI_Flash_PowerDown,          {0xaa,0x0F, 0, 0, 0, 0, 0, 0,0,0, 0xbb},
		(void*)F_SPI_Flash_WAKEUP,             {0xaa,0x10, 0, 0, 0, 0, 0, 0,0,0, 0xbb}, 
		//待机
		
	  (void*)SET_clock,                      {0xaa,0x12, 1, 1, 2, 3, 4, 5,6,0, 0xbb},
		(void*)RTC_xwsj,                       {0xaa,0x13, 1, 1, 2, 3, 4, 5,6,0, 0xbb},
		(void*)sht20_read,                     {0xaa,0x14, 0, 0, 0, 0, 0, 0,0,0, 0xbb},
		
		
		(void*)SET_DJJG,                       {0xaa,0x20, 1, 1, 0, 0, 0, 0,0,0, 0xbb},//设定待机间隔
		(void*)IN_STANDBY,                     {0xaa,0x21, 0, 0, 0, 0, 0, 0,0,0, 0xbb},//进入待机
		(void*)read_DJJG,                      {0xaa,0x22, 0, 0, 0, 0, 0, 0,0,0, 0xbb},//读取待机间隔
		(void*)W25Q16_write,                   {0xaa,0x23, 0, 0, 0, 0, 0, 0,0,0, 0xbb},//写数据到存储器
		(void*)W25Q64_SET_addr,                {0xaa,0x24, 1, 1, 0, 0, 0, 0,0,0, 0xbb},//写数据到存储器
 };
//3.根据函数表修改xieyi.h的tab
//4.在主程序中调用xieyi_scan();


 void init_xieyi(u8*p)
 {
	 *p=sizeof(name_tab)/sizeof(name_tab[0]);
 } 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
NAME_par  M_name_par =
{
	 name_tab,
	 exe,
	 0,//ID
	 0,//CMD
	 0,// u8 GS;
	 0,//u8 pra1;
	 0,//u8 cs2;
};


