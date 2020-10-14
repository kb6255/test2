#ifndef __XIEYI_H
#define __XIEYI_H	 
#include "sys.h"
#include "usart.h"
//2020年9月13日 12:29:24 改写和优化了指令解释，去除了返回参数，使指令增加了一位有效参数位。

//#define tab 7  //不定义会函数表溢出
#define DEBUG 1//调试信息输出

#define CMD_long 11 //指令总长度（更改长度后要增删EXE函数的函数形式）
#define CMD_data CMD_long-3//参数个数 去除0xaa cmd 0xbb



void init_xieyi(u8*p);

//函数表
typedef struct 
{
	void* pf;			//函数指针
	u8 cmd[CMD_long]; 
}NAME_TAB;

extern NAME_TAB name_tab[];


//函数数据表
typedef struct 
{
	 NAME_TAB *pfune;
	 u8(*exe)(void);
	 u8 ID;//函数表中的位置
	 u8 CMD;//指令
	 u8 GS;//参数个数
	 u8 pra[CMD_data];//函数参数分布
	 u32 cs[CMD_data];//函数实际参数
}NAME_par;

extern NAME_par M_name_par;




void xieyi_scan(void);
extern u8 exe(void);//外部不调用，只提供给xieyi_config.c使用

#endif

















