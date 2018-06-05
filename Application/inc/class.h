/**************************************************************************/
/*  Copyright (C) 2007 by ShuGW(Mr)                                       */
/*  All Rights Reserved                                                   */
/*  WRITER:     ShuGW                                                     */
/*  Purpose:    ͨ�ú���ͷ�ļ�                                            */
/*  FILE NAME:	gnr_fnc.h                                                 */
/*  Ver:	    Ver0.01.20071020                                          */
/*	MCU:	    STC89C58                                                  */
/*  Crystal:    11.0592MHz  											  */ 
/*  RUN BASED on:   WQGB_V0.05.20071006.PCB  Hardware                     */
/*  project:  WQPD-LAMP                                                   */
/**************************************************************************/

#ifndef  _CLASS_H
#define  _CLASS_H
/*======================================================*/
//����ͷ�ļ�
/*======================================================*/


#include "main.h"

/*================================================*/
//�궨��
/*================================================*/
#define LEFT_MOVE	0
#define RIGHT_MOVE	1

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
void setmem(u8* pmem, u8 init_val, u16 length);

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
void cpymem(u8* pmem, u8* sval, u16 length);

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
u8 cmpmem(u8* pmem, u8* sval, u16 length);

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
u16 cpystr(u8* pdmem, u8* psmem);

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
u8 cmpare_string(uc8 * cmpared_string, u8* cmpare_string);


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
void hex2str(u8 f_hex, u8* f_str);


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
void short2str(u16 f_short, u8* f_str);

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
void mem_exchange(u8* f_pmem, u8 f_num);

/**************************************************************************************************
* ����ԭ�� : void byte_exchange(PUCHAR f_pmem, UCHAR f_num)
* ��    �� : �ֽڵĸ߰��ֽ���Ͱ��ֽڵ���
* ��ڲ��� : UCHAR f_num Ҫ�����ڴ���ֽ���
* ���ڲ��� : PUCHAR f_pmem ���������ڴ��ַ 
* ��    �� : ��
* ȫ�ֱ��� : ��
* �������� : 2008-04-26
* ��	�� : V0.01.20080426
* ˵    �� : 
**************************************************************************************************/
void byte_exchange(u8* f_pmem, u8 f_num);

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
void parity_exchange(u8* f_pmem, u8 f_num);

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
void inden_str(u8* f_str);

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
void move_str(u8* f_str,u8 f_mve_dir, u8 f_mve_num);

/**************************************************************************************************
* ����ԭ�� : UCHAR sbuftobcd(const PUCHAR f_sbuf, UINT* f_bcd)
* ��    �� : �ѻ�����������������ĸת����BCD��
* ��ڲ��� : PUCHAR f_sbuf	�����ַ
* ���ڲ��� : UINT* f_bcd	ת���ɵ�BCD���ַ
* ��    �� : ������ת����������ĸ����
* ȫ�ֱ��� : ��
* �������� : 2008-04-22
* ��	�� : V0.01.20080422
* ˵    �� : 
**************************************************************************************************/
u8 sbuf2bcd(u8*  f_sbuf, u16* f_bcd);


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
u8 bcd2hex(u16 f_bcd);

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
u8 char2hex(u8* f_pchar);



#endif
