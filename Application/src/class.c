
#include "class.h"


/*================================================*/
//��������
/*================================================*/
/**************************************************************************************************
* ����ԭ�� : void setmem(UCHAR* pmem, UCHAR init_val, UINT length)
* ��    �� : ��ʼ���ڴ��ֵ
* ��ڲ��� : 
* ���ڲ��� : void
* ��    �� : void
* ȫ�ֱ��� : 
* �������� : 2008-01-06
* ��	�� : V0.01.20080106
* ˵    �� : 
**************************************************************************************************/
void setmem(u8* pmem, u8 init_val, u16 length)
{
    u16 i;
    
    for (i = 0; i < length; i++)
    {
       pmem[i] = init_val;
    }
}

/**************************************************************************************************
* ����ԭ�� : void cpymem(UCHAR* pmem, PUCHAR sval, UINT length)
* ��    �� : �����ڴ�
* ��ڲ��� : 
* ���ڲ��� : void
* ��    �� : void
* ȫ�ֱ��� : 
* �������� : 2008-01-06
* ��	�� : V0.01.20080106
* ˵    �� : 
**************************************************************************************************/
void cpymem(u8* pmem, u8* sval, u16 length)
{
    u16 i;
    
    for (i = 0; i < length; i++)
    {
        pmem[i] = sval[i];
    }
}

/**************************************************************************************************
* ����ԭ�� : UINT cpystr(PUCHAR pdmem, PUCHAR psmem)
* ��    �� : �ַ�������
* ��ڲ��� : 
* ���ڲ��� : void
* ��    �� : void
* ȫ�ֱ��� : 
* �������� : 2008-01-06
* ��	�� : V0.01.20080106
* ˵    �� : 
**************************************************************************************************/
u16 cpystr(u8* pdmem, u8* psmem)
{
    u16 i;
    
    for (i = 0; psmem[i] != '\0'; i++)
    {
        pdmem[i] = psmem[i];
    }
    
    return i;
}

/**************************************************************************************************
* ����ԭ�� : UCHAR cmpmem(UCHAR* pmem, PUCHAR sval, UINT length)
* ��    �� : �Ƚ��ڴ�
* ��ڲ��� : 
* ���ڲ��� : void
* ��    �� : 
* ȫ�ֱ��� : 
* �������� : 2008-01-06
* ��	�� : V0.01.20080106
* ˵    �� : 
**************************************************************************************************/
u8 cmpmem(u8* pmem, u8* sval, u16 length)
{
    u16 i;
    
    for (i = 0; i < length; i++)
    {
        if (pmem[i] != sval[i])
        {
            return false;
        }		
    }
    
    return true;
}
/**************************************************************************************************
* ����ԭ�� : UCHAR cmpare_string(const UCHAR * cmpared_string, UCHAR* cmpare_string)
* ��    �� : �Ƚ��ַ���
* ��ڲ��� : 
* ���ڲ��� : 
* ��    �� : 
* ȫ�ֱ��� : 
* �������� : 2008-01-06
* ��	�� : V0.01.20080106
* ˵    �� : 
**************************************************************************************************/
u8 cmpare_string(uc8 * cmpared_string, u8* cmpare_string)
{
    u8 i = 0;
    u8 j = 0;
    
    //�ų����Ƚ��ַ����еĻس���
    do {
          if ((cmpared_string[i] == 0x0d) || (cmpared_string[i] == 0x0a))
          {
              i++;
          }
          else
          {
              break;
          }
    } while(1);
    
    //��ʼ�Ƚ��ַ���
    for (j = 0; cmpare_string[j] != '\0'; j++)
    {
        if (cmpared_string[i++] != cmpare_string[j])
        {
            return false;
        }
    }
    
    return true;
}





/**************************************************************************************************
* ����ԭ�� : void run_led(void)
* ��    �� : ���е���˸����
* ��ڲ��� : UCHAR f_tim
* ���ڲ��� : void
* ��    �� : void
* ȫ�ֱ��� : UINT g_led_cnt; //���е���˸ʱ����Ƽ�����
* �������� : 2007-10-16
* ��	�� : V0.01.20071016
* ˵    �� : ��˸Ƶ��Ϊ1Hz
**************************************************************************************************/
/*void run_led(void)
{
			//ι��
	//���е�1S��ת1��
	if (DCD2_PIN)
	{
		if (g_led_cnt >= 500)
		{
			//g_led_cnt = 0xF0;	 //CPU������12ʱ��	
			g_led_cnt = 0x00;
			
			RUN_PIN ^= 1; //���еƷ�ת
		}
	}
	else
	{
		if (RUN_PIN)
		{
			if (g_led_cnt >= 950)
			{
				//g_led_cnt = 0xF0;	 //CPU������12ʱ��	
				g_led_cnt = 0x00;
				
				RUN_PIN ^= 1; //���еƷ�ת
			}
		}
		else
		{
			if (g_led_cnt >= 50)
			{
				//g_led_cnt = 0xF0;	 //CPU������12ʱ��	
				g_led_cnt = 0x00;
				
				RUN_PIN ^= 1; //���еƷ�ת
			}
		}		
	}
	
	RTS2_PIN = 0;        //RTS�õͣ�������������
}
*/
/**************************************************************************************************
* ����ԭ�� : void hextostr(UCHAR f_hex, PUCHAR f_str)
* ��    �� : һ��HEX����ת�����ַ�������
* ��ڲ��� : UCHAR f_hex ��ת����HEX����
* ���ڲ��� : PUCHAR f_str ת���ɵ��ַ������ݵ�ַ
* ��    �� : ��
* ȫ�ֱ��� : ��
* �������� : 2008-04-14
* ��	�� : V0.01.20080414
* ˵    �� : 
**************************************************************************************************/
void hex2str(u8 f_hex, u8* f_str)
{
    f_str[0] = f_hex%10 + 0x30;
    f_str[1] = (f_hex%100)/10 + 0x30;
    f_str[2] = f_hex/100 + 0x30;
    f_str[3] = '\0';
    mem_exchange(f_str, 3);
    inden_str(f_str);
}

/**************************************************************************************************
* ����ԭ�� : void shorttostr(UINT f_short, PUCHAR f_str)
* ��    �� : һ������������ת�����ַ���
* ��ڲ��� : UINT f_short ��ת������������
* ���ڲ��� : PUCHAR f_str ת�����ַ����ĵ�ַ
* ��    �� : ��
* ȫ�ֱ��� : ��
* �������� : 2008-04-14
* ��	�� : V0.01.20080414
* ˵    �� : 
**************************************************************************************************/
void short2str(u16 f_short, u8* f_str)
{	
    f_str[0] = f_short%10 + 0x30;
    f_str[1] = (f_short%100)/10 + 0x30;
    f_str[2] = (f_short%1000)/100 + 0x30;
    f_str[3] = (f_short%10000)/1000 + 0x30;
    f_str[4] = f_short/100000 + 0x30;
    f_str[5] = '\0';
    mem_exchange(f_str, 5);
    inden_str(f_str);
}
/**************************************************************************************************
* ����ԭ�� : void mem_exchange(PUCHAR f_pmem, UCHAR f_num)
* ��    �� : ʹһ�������ڴ��е�����ǰ�����
* ��ڲ��� : UCHAR f_num Ҫ�����ڴ���ֽ���
* ���ڲ��� : PUCHAR f_pmem ���������ڴ��ַ
* ��    �� : ��
* ȫ�ֱ��� : ��
* �������� : 2008-04-14
* ��	�� : V0.01.20080414
* ˵    �� : 
**************************************************************************************************/
void mem_exchange(u8* f_pmem, u8 f_num)
{
    u8 i,j, l_tmp;
    
    j = f_num -1;
    f_num /= 2;
    for (i=0;i<f_num;i++)
    {
        l_tmp = f_pmem[i];
        f_pmem[i] = f_pmem[j];
        f_pmem[j--] = l_tmp;
    }
}

/**************************************************************************************************
* ����ԭ�� : void byte_exchange(PUCHAR f_pmem, UCHAR f_num)
* ��    �� : ��������ǰ�����
* ��ڲ��� : UCHAR f_num Ҫ�����ڴ���ֽ���
* ���ڲ��� : PUCHAR f_pmem ���������ڴ��ַ 
* ��    �� : ��
* ȫ�ֱ��� : ��
* �������� : 2008-04-26
* ��	�� : V0.01.20080426
* ˵    �� : 
**************************************************************************************************/
void parity_exchange(u8* f_pmem, u8 f_num)
{
	u8 i, l_tmp;

	for (i=0;i<f_num;i++)
	{
            l_tmp = f_pmem[i];
            f_pmem[i] = f_pmem[i+1];
            f_pmem[++i] = l_tmp;
	}
}

/**************************************************************************************************
* ����ԭ�� : void inden_str(PUCHAR f_str)
* ��    �� : �������ַ�'0'��ͷ���ַ���
* ��ڲ��� : ��
* ���ڲ��� : PUCHAR f_str ���������ַ���
* ��    �� : ��
* ȫ�ֱ��� : ��
* �������� : 2008-04-14
* ��	�� : V0.01.20080414
* ˵    �� : ����"0123"������"123"
**************************************************************************************************/
void inden_str( u8* f_str)
{
    s8 i,l_tmp;
    
    l_tmp = strlen((const char *)f_str);
    
    for (i=0;i<l_tmp;i++)
    {
        if (f_str[0] == '0')
        {
            move_str(f_str,LEFT_MOVE,1);
        }
    }
}

/**************************************************************************************************
* ����ԭ�� : void move_str(PUCHAR f_str,UCHAR f_mve_dir, UCHAR f_mve_num)
* ��    �� : ����������ƶ��ַ���
* ��ڲ��� : UCHAR f_mve_dir �ƶ�����(0 ����, 1 ����)
			 UCHAR f_mve_num �ƶ�λ��
* ���ڲ��� : PUCHAR f_str ���ƶ����ַ���
* ��    �� : ��
* ȫ�ֱ��� : ��
* �������� : 2008-04-14
* ��	�� : V0.01.20080414
* ˵    �� : 
**************************************************************************************************/
void move_str(u8* f_str,u8 f_mve_dir, u8 f_mve_num)
{
    s8 i,l_tmp;
    
    l_tmp = strlen((const char *)f_str) - f_mve_num;
    
    if (f_mve_dir) //����
    {
        f_str[l_tmp] = '0';
    }
    else		   //����
    {
        for (i=0;i<l_tmp;i++)
        {
            f_str[i] = f_str[i+f_mve_num];
        }
        f_str[i] = '\0';
    }
}


/**************************************************************************************************
* ����ԭ�� : UCHAR sbuftobcd(PUCHAR  f_sbuf, UINT* f_bcd)
* ��    �� : �ѻ�����������������ĸת����BCD��
* ��ڲ��� : PUCHAR f_sbuf	�����ַ
* ���ڲ��� : UINT* f_bcd	ת���ɵ�BCD���ַ
* ��    �� : ������ת����������ĸ����
* ȫ�ֱ��� : ��
* �������� : 2008-04-22
* ��	�� : V0.01.20080422
* ˵    �� : 
**************************************************************************************************/
u8 sbuf2bcd(u8*  f_sbuf, u16* f_bcd)
{
    u8 i;
    
    *f_bcd = 0;
    for (i=0;i<4;i++)
    {
        if ((f_sbuf[i] >= '0')&&(f_sbuf[i] <= '9'))
        {
            *f_bcd <<= 4;
            *f_bcd += f_sbuf[i] - '0';
        }
        else
        {
            return i;
        }
    }
    return i;
}

/**************************************************************************************************
* ����ԭ�� : UCHAR bcdtohex(UINT f_bcd)
* ��    �� : ��(255-0)��Χ�ڵ�BCDת����HEX��
* ��ڲ��� : UINT f_bcd	��ת����BCD��
* ���ڲ��� : ��
* ��    �� : ת���ɵ�HEX��
* ȫ�ֱ��� : ��
* �������� : 2008-05-07
* ��	�� : V0.01.20080507
* ˵    �� : 
**************************************************************************************************/
u8 bcd2hex(u16 f_bcd)
{
    u8 l_tmp;
    l_tmp = ((f_bcd >> 8) % 3) * 100;
    l_tmp += ((f_bcd & 0xF0) >> 4) * 10;
    l_tmp += (f_bcd & 0x0F);
    return l_tmp;
}

/**************************************************************************************************
* ����ԭ�� : UCHAR chartohex(PUCHAR f_pchar)
* ��    �� : ����λ�ַ�CHARת����HEX��
* ��ڲ��� : PUCHAR f_pchar	��ת�����ַ�CHAR��ָ��
* ���ڲ��� : ��
* ��    �� : ת���ɵ�HEX��
* ȫ�ֱ��� : ��
* �������� : 2008-05-09
* ��	�� : V0.01.20080509
* ˵    �� : 
**************************************************************************************************/
u8 char2hex(u8* f_pchar)
{
    u8 l_tmp;
    
    l_tmp = (f_pchar[0] > '9' ? f_pchar[0] - '7' : f_pchar[0] - '0')  << 4;
    l_tmp |= (f_pchar[1] > '9' ? f_pchar[1] - '7' : f_pchar[1] - '0') & 0x0F;
    return l_tmp;
}