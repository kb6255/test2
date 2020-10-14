#include "xieyi.h"
u8 CMD_NUM;
u8 exe(void)
{
    u8 res;
    switch(M_name_par.GS)
    {
    case 0://无参数(void类型)
        res=(*(u32(*)())M_name_par.pfune->pf)();
        break;
    case 1://有1个参数
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0]);
        break;
    case 2://有2个参数
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1]);
        break;
    case 3://有3个参数
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2]);
        break;
    case 4://有4个参数
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3]);
        break;
    case 5://有5个参数
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3],M_name_par.cs[4]);
        break;
    case 6://有6个参数
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3],M_name_par.cs[4],M_name_par.cs[5]);
        break;
    case 7://有7个参数
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3],M_name_par.cs[4],M_name_par.cs[5],M_name_par.cs[6]);
        break;
    case 8://有8个参数
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3],M_name_par.cs[4],M_name_par.cs[5],M_name_par.cs[6],M_name_par.cs[7]);
        break;
    }
    return res;
}

//获取通讯指令在格式中的ID
u8 get_fun_ID(u8 cmd,u8 *p)
{
    u8 i=0;
    for(i=0; i<=CMD_NUM; i++)
    {
        if(cmd==name_tab[i].cmd[1])
        {
            *p=i;
            return 0;
        }
    }
    return 1;
}
//辨认参数格式:个数和类型
void get_fun_par_gslx()
{
    u8 i,j;
    u8 buf[CMD_data];
    u8 par[CMD_data]; //状态暂存
    for(i=0; i<(CMD_data); i++) par[i]=0;
    j=2;//指令字节所在位置
    for(i=0; i<(CMD_data); i++) //将纯参数放入数组
    {
        buf[i]=name_tab[M_name_par.ID].cmd[j];
        j++;
    }
#if 0
    printf("*****************************************\r\n");
    for(i=0; i<(CMD_data); i++) //显示串口接收值
    {
        printf("buf[%d]=%d\r\n",i,buf[i]);
    }
#endif
    for(i=0; i<(CMD_data); i++) //辨认参数 pra
    {
        if(buf[i]!=0)  par[buf[i]-1]++;
    }
    j=0;
    for(i=0; i<(CMD_data); i++) //辨认参数 GS
    {
        if(par[i]!=0) j++;
    }
    M_name_par.GS=j;
    for(i=0; i<(CMD_data); i++) M_name_par.pra[i]=par[i];
#if 0
    printf("*****************************************\r\n");
    for(i=0; i<(CMD_data); i++) //显示串口接收值
    {
        printf("par[%d]=%d\r\n",i,M_name_par.pra[i]);
    }
    printf("GS=%d\r\n",M_name_par.GS);
#endif
}

//获取格式 指令 参数个数，参数类型 返回类型
//输入接受缓冲区CMD
void get_fun_par(u8 id)
{
    M_name_par.pfune =&name_tab[id];
    M_name_par.ID=id;
    M_name_par.CMD=name_tab[id].cmd[1];//指令名字
    //辨认参数格式:个数和类型
    get_fun_par_gslx();
}


//取u8数据
u32 yiwei1(u8*buf)
{
    u32 i;
    u8 j;
    i=buf[0];//将u8数据存放

    for(j=0; j<(CMD_data-1); j++)//将第一个u8数据移除掉
    {
        buf[j]=buf[j+1];
    }
    return i;
}
//取U16数据
u32 yiwei2(u8*buf)
{
    u32 i;
    u8 j;
    u8 k=2;
    i=((u16)buf[0]<<8)+buf[1];


    for(j=0; j<(CMD_data-2); j++)
    {
        buf[j]=buf[k];//第二次装载BUF,
        k++;
    }
    return i;

}
//取U32数据
u32 yiwei4(u8*buf)
{
    u32 i;
    u8 j;
    u8 k=4;
    i=((u32)buf[0]<<24)+((u32)buf[1]<<16)+((u32)buf[2]<<8)+buf[3];

    for(j=0; j<(CMD_data-4); j++)
    {
        buf[j]=buf[k];;//第二次装载BUF,
        k++;
    }

    return i;
}



void get_uart_par(u8 *buf)
{
    u8 i;
    u8 cbuf[CMD_data];

    for(i=0; i<(CMD_data); i++)
    {
        cbuf[i]=buf[i];
    }
#if 0
    printf("*****************************************\r\n");
    for(i=0; i<(CMD_data); i++) //显示串口接收值
    {
        printf("cbuf[%d]=%d\r\n",i,cbuf[i]);
    }
#endif
    //参数是有限制的
    for(i=0; i<CMD_data; i++)
    {
        if(M_name_par.pra[i]!=0)
        {
            switch(M_name_par.pra[i])
            {
            case 1://u8
                M_name_par.cs[i]= yiwei1(cbuf);
                break;
            case 2://u16
                M_name_par.cs[i]= yiwei2(cbuf);
                break;
            case 4://u32
                M_name_par.cs[i]=yiwei4(cbuf);
                break;
            }
        }
    }

#if 0
    printf("*****************************************\r\n");
    for(i=0; i<(CMD_data); i++) //显示串口接收值
    {
        printf("cs[%d]=%d\r\n",i,M_name_par.cs[i]);
    }
#endif
}


//aa 00 a0 01 01 01 01 01 01 01 bb
void xieyi_scan(void)
{
    u8 id;
    u8 res;
	  static u8 i=0;
	  if(i==0)
	 {
			i=1;
		  init_xieyi(&CMD_NUM);
		  #if 0
		  printf("NUM=%d\r\n",CMD_NUM);
		  #endif
	 }
    if(USART1_RX_STA&0x8000)
    {
        if((USART1_RX_BUF[0]==0xaa)&&(USART1_RX_BUF[CMD_long-1]==0xbb))
        {
            res=get_fun_ID(USART1_RX_BUF[1],&id);//将串口的数据解析成函数表中的哪个函数ID
            if(!res)
            {
                get_fun_par(id);////获取函数表中的格式 指令 参数个数，参数类型 返回类型
                get_uart_par(&USART1_RX_BUF[2]);
                exe();
            }
#if DEBUG
            else printf("无此指令！！！\r\n");
#endif
#if 0
            printf("*****************************************\r\n");
						if(!res)//显示指令状态
                printf("读取指令成功！,读取的指令=%d,串口指令号=%d,状态：OK!!\r\n",id,USART1_RX_BUF[1]);
            else printf("读取指令失败！,读取的指令=%d,cmd=%d,err!!\r\n",id,USART1_RX_BUF[1]);
            for(id=0; id<CMD_long; id++) //显示串口接收值
            {
                printf("buf[%d]=%d\r\n",id,USART1_RX_BUF[id]);
            }

#endif
            USART1_RX_BUF[0]=0;
             USART1_RX_BUF[CMD_long-1]=0;
        }
#if DEBUG
        else printf("格式错误！\r\n");
#endif
        USART1_RX_STA=0;
    }
}

























































