#include "xieyi.h"
u8 CMD_NUM;
u8 exe(void)
{
    u8 res;
    switch(M_name_par.GS)
    {
    case 0://�޲���(void����)
        res=(*(u32(*)())M_name_par.pfune->pf)();
        break;
    case 1://��1������
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0]);
        break;
    case 2://��2������
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1]);
        break;
    case 3://��3������
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2]);
        break;
    case 4://��4������
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3]);
        break;
    case 5://��5������
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3],M_name_par.cs[4]);
        break;
    case 6://��6������
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3],M_name_par.cs[4],M_name_par.cs[5]);
        break;
    case 7://��7������
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3],M_name_par.cs[4],M_name_par.cs[5],M_name_par.cs[6]);
        break;
    case 8://��8������
        res=(*(u32(*)())M_name_par.pfune->pf)(M_name_par.cs[0],M_name_par.cs[1],M_name_par.cs[2],M_name_par.cs[3],M_name_par.cs[4],M_name_par.cs[5],M_name_par.cs[6],M_name_par.cs[7]);
        break;
    }
    return res;
}

//��ȡͨѶָ���ڸ�ʽ�е�ID
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
//���ϲ�����ʽ:����������
void get_fun_par_gslx()
{
    u8 i,j;
    u8 buf[CMD_data];
    u8 par[CMD_data]; //״̬�ݴ�
    for(i=0; i<(CMD_data); i++) par[i]=0;
    j=2;//ָ���ֽ�����λ��
    for(i=0; i<(CMD_data); i++) //����������������
    {
        buf[i]=name_tab[M_name_par.ID].cmd[j];
        j++;
    }
#if 0
    printf("*****************************************\r\n");
    for(i=0; i<(CMD_data); i++) //��ʾ���ڽ���ֵ
    {
        printf("buf[%d]=%d\r\n",i,buf[i]);
    }
#endif
    for(i=0; i<(CMD_data); i++) //���ϲ��� pra
    {
        if(buf[i]!=0)  par[buf[i]-1]++;
    }
    j=0;
    for(i=0; i<(CMD_data); i++) //���ϲ��� GS
    {
        if(par[i]!=0) j++;
    }
    M_name_par.GS=j;
    for(i=0; i<(CMD_data); i++) M_name_par.pra[i]=par[i];
#if 0
    printf("*****************************************\r\n");
    for(i=0; i<(CMD_data); i++) //��ʾ���ڽ���ֵ
    {
        printf("par[%d]=%d\r\n",i,M_name_par.pra[i]);
    }
    printf("GS=%d\r\n",M_name_par.GS);
#endif
}

//��ȡ��ʽ ָ�� ������������������ ��������
//������ܻ�����CMD
void get_fun_par(u8 id)
{
    M_name_par.pfune =&name_tab[id];
    M_name_par.ID=id;
    M_name_par.CMD=name_tab[id].cmd[1];//ָ������
    //���ϲ�����ʽ:����������
    get_fun_par_gslx();
}


//ȡu8����
u32 yiwei1(u8*buf)
{
    u32 i;
    u8 j;
    i=buf[0];//��u8���ݴ��

    for(j=0; j<(CMD_data-1); j++)//����һ��u8�����Ƴ���
    {
        buf[j]=buf[j+1];
    }
    return i;
}
//ȡU16����
u32 yiwei2(u8*buf)
{
    u32 i;
    u8 j;
    u8 k=2;
    i=((u16)buf[0]<<8)+buf[1];


    for(j=0; j<(CMD_data-2); j++)
    {
        buf[j]=buf[k];//�ڶ���װ��BUF,
        k++;
    }
    return i;

}
//ȡU32����
u32 yiwei4(u8*buf)
{
    u32 i;
    u8 j;
    u8 k=4;
    i=((u32)buf[0]<<24)+((u32)buf[1]<<16)+((u32)buf[2]<<8)+buf[3];

    for(j=0; j<(CMD_data-4); j++)
    {
        buf[j]=buf[k];;//�ڶ���װ��BUF,
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
    for(i=0; i<(CMD_data); i++) //��ʾ���ڽ���ֵ
    {
        printf("cbuf[%d]=%d\r\n",i,cbuf[i]);
    }
#endif
    //�����������Ƶ�
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
    for(i=0; i<(CMD_data); i++) //��ʾ���ڽ���ֵ
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
            res=get_fun_ID(USART1_RX_BUF[1],&id);//�����ڵ����ݽ����ɺ������е��ĸ�����ID
            if(!res)
            {
                get_fun_par(id);////��ȡ�������еĸ�ʽ ָ�� ������������������ ��������
                get_uart_par(&USART1_RX_BUF[2]);
                exe();
            }
#if DEBUG
            else printf("�޴�ָ�����\r\n");
#endif
#if 0
            printf("*****************************************\r\n");
						if(!res)//��ʾָ��״̬
                printf("��ȡָ��ɹ���,��ȡ��ָ��=%d,����ָ���=%d,״̬��OK!!\r\n",id,USART1_RX_BUF[1]);
            else printf("��ȡָ��ʧ�ܣ�,��ȡ��ָ��=%d,cmd=%d,err!!\r\n",id,USART1_RX_BUF[1]);
            for(id=0; id<CMD_long; id++) //��ʾ���ڽ���ֵ
            {
                printf("buf[%d]=%d\r\n",id,USART1_RX_BUF[id]);
            }

#endif
            USART1_RX_BUF[0]=0;
             USART1_RX_BUF[CMD_long-1]=0;
        }
#if DEBUG
        else printf("��ʽ����\r\n");
#endif
        USART1_RX_STA=0;
    }
}

























































