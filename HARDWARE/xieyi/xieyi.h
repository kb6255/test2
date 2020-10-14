#ifndef __XIEYI_H
#define __XIEYI_H	 
#include "sys.h"
#include "usart.h"
//2020��9��13�� 12:29:24 ��д���Ż���ָ����ͣ�ȥ���˷��ز�����ʹָ��������һλ��Ч����λ��

//#define tab 7  //������ắ�������
#define DEBUG 1//������Ϣ���

#define CMD_long 11 //ָ���ܳ��ȣ����ĳ��Ⱥ�Ҫ��ɾEXE�����ĺ�����ʽ��
#define CMD_data CMD_long-3//�������� ȥ��0xaa cmd 0xbb



void init_xieyi(u8*p);

//������
typedef struct 
{
	void* pf;			//����ָ��
	u8 cmd[CMD_long]; 
}NAME_TAB;

extern NAME_TAB name_tab[];


//�������ݱ�
typedef struct 
{
	 NAME_TAB *pfune;
	 u8(*exe)(void);
	 u8 ID;//�������е�λ��
	 u8 CMD;//ָ��
	 u8 GS;//��������
	 u8 pra[CMD_data];//���������ֲ�
	 u32 cs[CMD_data];//����ʵ�ʲ���
}NAME_par;

extern NAME_par M_name_par;




void xieyi_scan(void);
extern u8 exe(void);//�ⲿ�����ã�ֻ�ṩ��xieyi_config.cʹ��

#endif

















