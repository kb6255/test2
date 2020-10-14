 #include "xieyi.h"
 //1.�����ⲿ���� 
 void F_dbug(u8 x);
 void flash_jczx(void);
 void flash_write(u8 addr,u16 temp,u16 damp,u16 timer);
 void flash_read(u8 addr);
 void Erase_Sector(u16 Dst_Addr);//�������� Dst_Addr=0~16*128
 void Erase_Chip(void);
 void write_Chip(void);
 void Read_Chip(u16 x); //��Ƭ��������
 void F_NUM(u16 x);//s��������
 void F_jisuan(u16 a,u16 b);//����ȡ��
 void F_SPI_FLASH_BufferWrite(void);//����д�루�Զ�������
 //void F_SPI_Flash_Write(u8 addr,u8 DR);//SPIд��һ����
// void F_SPI_Flash_Write_Page(void);//W25Q64��ҳд�룬��������ʱ����,�����32768ʱѭ���洢;
 void F_SPI_Flash_PowerDown(void);//W25Q64����
 void F_SPI_Flash_WAKEUP(void);//w25q64����
 
 void SET_clock(void);//�趨ʱ��
 void RTC_xwsj(void);//ѯ��ʱ��
 void sht20_read(void);//sht20��ȡ��ʪ��
 
 void SET_DJJG(u16 a);//�趨�������
 void IN_STANDBY(void);//�������
 void read_DJJG(void);//��ȡ�������
 void W25Q16_write(void);//д���ݵ��洢��
 void W25Q64_SET_addr(u16 x);//�趨addr��ַ
//2.��дָ�������ʽ
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
		//����
		
	  (void*)SET_clock,                      {0xaa,0x12, 1, 1, 2, 3, 4, 5,6,0, 0xbb},
		(void*)RTC_xwsj,                       {0xaa,0x13, 1, 1, 2, 3, 4, 5,6,0, 0xbb},
		(void*)sht20_read,                     {0xaa,0x14, 0, 0, 0, 0, 0, 0,0,0, 0xbb},
		
		
		(void*)SET_DJJG,                       {0xaa,0x20, 1, 1, 0, 0, 0, 0,0,0, 0xbb},//�趨�������
		(void*)IN_STANDBY,                     {0xaa,0x21, 0, 0, 0, 0, 0, 0,0,0, 0xbb},//�������
		(void*)read_DJJG,                      {0xaa,0x22, 0, 0, 0, 0, 0, 0,0,0, 0xbb},//��ȡ�������
		(void*)W25Q16_write,                   {0xaa,0x23, 0, 0, 0, 0, 0, 0,0,0, 0xbb},//д���ݵ��洢��
		(void*)W25Q64_SET_addr,                {0xaa,0x24, 1, 1, 0, 0, 0, 0,0,0, 0xbb},//д���ݵ��洢��
 };
//3.���ݺ������޸�xieyi.h��tab
//4.���������е���xieyi_scan();


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


