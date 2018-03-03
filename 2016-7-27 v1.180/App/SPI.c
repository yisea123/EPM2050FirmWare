

#include "SPI.H"

void SPIInit()
{
//	unsigned char tmpchar[2] = {0, 0};
    PINSEL_CFG_Type PinCfg;
    __IO FlagStatus exitflag;
//	/*
//	 * Initialize SPI pin connect
//	 * P0.8 - WP;
//	 * P0.9 - SO - used as GPIO
//	 * P0.16 - SCK
//	 * P0.15 - SI
//	 * P2.8 - EC CE
//	 * P2.9 - HOLD
//	 */



//	PinCfg.Funcnum = 0;
//	PinCfg.OpenDrain = 0;
//	PinCfg.Pinmode = 0;
//	PinCfg.Portnum = 0;
//	PinCfg.Pinnum = 15;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 9;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 8;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 16;
//	PinCfg.Funcnum = 0;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 8;
//	PinCfg.Portnum = 2;
//	PINSEL_ConfigPin(&PinCfg);
//	PinCfg.Pinnum = 9;
//	PinCfg.Portnum = 2;
//	PINSEL_ConfigPin(&PinCfg);




    PinCfg.Funcnum = 0;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 4;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 8;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 9;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 10;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 14;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 15;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(1, SCK|SI|WP|CE|HOLD, 1);
    GPIO_SetDir(1, SO, 0);
    SET_CE(1);
    SET_HOLD(1);

    SET_WP(1);
    SET_SCK(0);

}
volatile void DelayAD(int32_t ulTime)
{
    while (ulTime--);
}
void Send_Byte(uint8 dat)
{
    int32 i;
    for(i=0; i<8; i++)
    {
        if(dat&0x80)
        {
            SET_SI(1);
        }
        else
        {
            SET_SI(0);
        }

//		DelayAD(50);
        SET_SCK(1);
//		DelayAD(50);

        dat <<=1  ;
        SET_SCK(0);

    }
}


uint8 Get_Byte(void)
{
//,temp = 0
    uint8 i=0,in=0;
    for(i=0; i<8; i++)
    {
        in = in<<1;
        if(GPIO_ReadValue(1)&SO)
        {
            in |= 0x01;
        }
//		DelayAD(50);
        SET_SCK(1);
//		DelayAD(50);
        SET_SCK(0);
    }
    return in;

}
uint8 SSTF016B_RD(uint32 Dst, uint32 NByte,uint8* RcvBufPt)
{
    uint32 i = 0;
    if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	return (0);	 //	�����ڲ���

    SET_CE(0);
    Send_Byte(0x0B); 						// ���Ͷ�����
    Send_Byte(((Dst & 0xFFFFFF) >> 16));	// ���͵�ַ��Ϣ:�õ�ַ��3���ֽ����
    Send_Byte(((Dst & 0xFFFF) >> 8));
    Send_Byte(Dst & 0xFF);
    Send_Byte(0xFF);						// ����һ�����ֽ��Զ�ȡ����
    for (i = 0; i < NByte; i++)
    {
        RcvBufPt[i] = Get_Byte();
    }
    SET_CE(1);
    return (1);
}


uint8 SSTF016B_WR(uint32 Dst,uint8  SndbufPt[], uint32 NByte)
{
    uint16 temp = 0,i = 0,StatRgVal = 0;
    if (( (Dst+NByte-1 > MAX_ADDR)||(NByte == 0) ))
    {
        return (ERROR);	 //	�����ڲ���
    }
    SET_CE(0);
    Send_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
    temp = Get_Byte();							// ������õ�״̬�Ĵ���ֵ
    SET_CE(1);

    SET_CE(0);
    Send_Byte(0x50);							// ʹ״̬�Ĵ�����д
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
    Send_Byte(0);								// ��0BPxλ��ʹFlashоƬȫ����д
    SET_CE(1);



    for(i = 0; i < NByte; i++)
    {
        SET_CE(0);
        Send_Byte(0x06);						// ����дʹ������
        SET_CE(1);


        SET_CE(0);
        Send_Byte(0x02); 						// �����ֽ�������д����

        Send_Byte((((Dst+i) & 0xFFFFFF) >> 16));// ����3���ֽڵĵ�ַ��Ϣ
        Send_Byte((((Dst+i) & 0xFFFF) >> 8));
        Send_Byte((Dst+i) & 0xFF);

        Send_Byte(SndbufPt[i]);					// ���ͱ���д������
        SET_CE(1);
        do
        {
            SET_CE(0);
            Send_Byte(0x05);					// ���Ͷ�״̬�Ĵ�������
            StatRgVal = Get_Byte();				// ������õ�״̬�Ĵ���ֵ
            SET_CE(1);
        }
        while ((StatRgVal&0X03) == 0x03);				// һֱ�ȴ���ֱ��оƬ����

    }


    SET_CE(0);
    Send_Byte(0x06);							// ����дʹ������
    SET_CE(1);

    SET_CE(0);
    Send_Byte(0x50);							// ʹ״̬�Ĵ�����д
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
    Send_Byte(temp);							// �ָ�״̬�Ĵ���������Ϣ
    SET_CE(1);

    return (1);
}
char flash_chip_erase(void)
{
    uint8  StatRgVal = 0;
//    uint32 SecnHdAddr = 0;
//	uint32 no_SecsToEr = 0;				   			// Ҫ������������Ŀ
//	uint32 CurSecToEr = 0;temp2 = 0,temp1 = 0,	  						// ��ǰҪ������������


    SET_CE(0);
    Send_Byte(0x05);								// ���Ͷ�״̬�Ĵ�������
//	temp1 = Get_Byte();								// ������õ�״̬�Ĵ���ֵ
    SET_CE(1);

    SET_CE(0);
    Send_Byte(0x50);								// ʹ״̬�Ĵ�����д
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x01);								// ����д״̬�Ĵ���ָ��
    Send_Byte(0);									// ��0BPxλ��ʹFlashоƬȫ����д
    SET_CE(1);

    SET_CE(0);
    Send_Byte(0x06);								// ����дʹ������
    SET_CE(1);


    SET_CE(0);
    Send_Byte(0x60);							// ����оƬ����ָ��(60h or C7h)
    SET_CE(1);
    do
    {
        SET_CE(0);
        Send_Byte(0x05);						// ���Ͷ�״̬�Ĵ�������
        StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
        SET_CE(1);
    }
    while ((StatRgVal&0X03) == 0x03);				// һֱ�ȴ���ֱ��оƬ����
    return (1);
}

uint8 MX25L1602_RdID(uint8 IDType, uint32* RcvbufPt)
{
    uint32 temp = 0;
    if (IDType == 1)
    {
        SET_CE(0);
        Send_Byte(0x9F);		 		// ���Ͷ�JEDEC ID����(9Fh)
        temp = (temp | Get_Byte()) << 8;// ��������
        temp = (temp | Get_Byte()) << 8;
        temp = (temp | Get_Byte()); 	// �ڱ�����,temp��ֵӦΪ0xBF2541
        SET_CE(1);
        *RcvbufPt = temp;
        return (1);
    }

    if ((IDType == 0) || (IDType == 2) )
    {
        SET_CE(0);
        Send_Byte(0x90);				// ���Ͷ�ID���� (90h or ABh)
        Send_Byte(0x00);				// ���͵�ַ
        Send_Byte(0x00);				// ���͵�ַ
        Send_Byte(IDType);				// ���͵�ַ - ����00H����01H
        temp = Get_Byte();				// ���ջ�ȡ�������ֽ�
        SET_CE(1);
        *RcvbufPt = temp;
        return (1);
    }
    else
    {
        return (0);
    }
}

uint8 SSTF016B_Erase_1sec(uint32 sec1)
{
    uint8  StatRgVal = 0;
    uint32 SecnHdAddr = 0;


    do
    {
        SET_CE(0);
        Send_Byte(0x05);						// ??????????
        StatRgVal = Get_Byte();					// ???????????
        SET_CE(1);
    }
    while ((StatRgVal&0X03) == 0x03);				// һֱ�ȴ���ֱ��оƬ����

    SET_CE(0);
    Send_Byte(0x50);							// ʹ״̬�Ĵ�����д
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
    Send_Byte(0);								// ��0BPxλ��ʹFlashоƬȫ����д
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x06);								// ����дʹ������
    SET_CE(1);

    SecnHdAddr = SEC_SIZE * sec1;				// ?????????
    SET_CE(0);
    Send_Byte(0x20);							// ????????
    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
    Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
    Send_Byte(SecnHdAddr & 0xFF);
    SET_CE(1);

    return 0;
}

uint8 SSTF016B_Erase_64k(uint32 sec1)
{
    uint8  StatRgVal = 0;
    uint32 SecnHdAddr = 0;


    do
    {
        SET_CE(0);
        Send_Byte(0x05);						// ??????????
        StatRgVal = Get_Byte();					// ???????????
        SET_CE(1);
    }
    while ((StatRgVal&0X03) == 0x03);				// һֱ�ȴ���ֱ��оƬ����

    SET_CE(0);
    Send_Byte(0x50);							// ʹ״̬�Ĵ�����д
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
    Send_Byte(0);								// ��0BPxλ��ʹFlashоƬȫ����д
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x06);								// ����дʹ������
    SET_CE(1);

    SecnHdAddr = SEC_SIZE * sec1;				// ?????????
    SET_CE(0);
    Send_Byte(0xD8);							// ????????
    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
    Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
    Send_Byte(SecnHdAddr & 0xFF);
    SET_CE(1);

    return 0;
}







//ʹ��ʱע��  ��ַ��ΧҪȷ�����й���64k������ʼ��ַҪ��0x10000����������
uint8 SSTF016B_Erase(uint32 sec1, uint32 sec2)
{
    uint8  temp1 = 0,temp2 = 0,StatRgVal = 0;
    uint32 SecnHdAddr = 0;
    uint32 no_SecsToEr = 0;				   			// ????????
    uint32 CurSecToEr = 0;	  						// ?????????

    /*  ?????? */
    if ((sec1 > SEC_MAX)||(sec2 > SEC_MAX))
    {
        return (ERROR);
    }

    SET_CE(0);
    Send_Byte(0x05);								// ??????????
    temp1 = Get_Byte();								// ???????????
    SET_CE(1);

    SET_CE(0);
    Send_Byte(0x50);								// ????????
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x01);								// ??????????
    Send_Byte(0);									// ?0BPx?,?Flash??????
    SET_CE(1);

    SET_CE(0);
    Send_Byte(0x06);								// ???????
    SET_CE(1);

    /* ???????????????????,???????? */
    if (sec1 > sec2)
    {
        temp2 = sec1;
        sec1  = sec2;
        sec2  = temp2;
    }
    /* ??????????????? */
    if (sec1 == sec2)
    {
        SecnHdAddr = SEC_SIZE * sec1;				// ?????????
        SET_CE(0);
        Send_Byte(0x20);							// ????????
        Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
        Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
        Send_Byte(SecnHdAddr & 0xFF);
        SET_CE(1);
        do
        {
            SET_CE(0);
            Send_Byte(0x05);						// ??????????
            StatRgVal = Get_Byte();					// ???????????
            SET_CE(1);
        }
        while (StatRgVal == 0x03);					// ????,??????
        return (OK);
    }

    /* 		??????????????????????? 							*/

    if (sec2 - sec1 == SEC_MAX)
    {
        SET_CE(0);
        Send_Byte(0x60);							// ????????(60h or C7h)
        SET_CE(1);
        do
        {
            SET_CE(0);
            Send_Byte(0x05);						// ??????????
            StatRgVal = Get_Byte();					// ???????????
            SET_CE(1);
        }
        while (StatRgVal == 0x03);					// ????,??????
        return (OK);
    }

    no_SecsToEr = sec2 - sec1 +1;					// ??????????
    CurSecToEr  = sec1;								// ?????????

    /* ????????????,???16?????? */
    while (no_SecsToEr >= 16)
    {
        SecnHdAddr = SEC_SIZE * CurSecToEr;			// ?????????
        SET_CE(0);
        Send_Byte(0xD8);							// ??64KB?????
        Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
        Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
        Send_Byte(SecnHdAddr & 0xFF);
        SET_CE(1);
        do
        {
            SET_CE(0);
            Send_Byte(0x05);						// ??????????
            StatRgVal = Get_Byte();					// ???????????
            SET_CE(1);
        }
        while (StatRgVal == 0x03);					// ????,??????
        CurSecToEr  += 16;							// ?????16????,??????????????
        no_SecsToEr -=  16;							// ?????????????
    }
    /* ????????????,???8?????? */
    while (no_SecsToEr >= 8)
    {
        SecnHdAddr = SEC_SIZE * CurSecToEr;			// ?????????
        SET_CE(0);
        Send_Byte(0x52);							// ??32KB????
        Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
        Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
        Send_Byte(SecnHdAddr & 0xFF);
        SET_CE(1);
        do
        {
            SET_CE(0);
            Send_Byte(0x05);						// ??????????
            StatRgVal = Get_Byte();					// ???????????
            SET_CE(1);
        }
        while (StatRgVal == 0x03);					// ????,??????
        CurSecToEr  += 8;
        no_SecsToEr -=  8;
    }
    /* ??????????????? */
    while (no_SecsToEr >= 1)
    {
        SecnHdAddr = SEC_SIZE * CurSecToEr;			// ?????????
        SET_CE(0);
        Send_Byte(0x20);							// ????????
        Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ??3????????
        Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
        Send_Byte(SecnHdAddr & 0xFF);
        SET_CE(1);
        do
        {
            SET_CE(0);
            Send_Byte(0x05);						// ??????????
            StatRgVal = Get_Byte();					// ???????????
            SET_CE(1);
        }
        while (StatRgVal == 0x03);					// ????,??????
        CurSecToEr  += 1;
        no_SecsToEr -=  1;
    }
    /*				????,?????????							*/
    SET_CE(0);
    Send_Byte(0x06);								// ???????
    SET_CE(1);

    SET_CE(0);
    Send_Byte(0x50);								// ????????
    SET_CE(1);
    SET_CE(0);
    Send_Byte(0x01);								// ??????????
    Send_Byte(temp1);								// ???????????
    SET_CE(1);
    return (OK);
}


void 	FlashDatChange(uint8 *dat,MEAS_RECORD measRecord)
{
    uint8 *p,a[4];
    p = dat;

    //��ʼʱ��
    *p = measRecord.recordTimeStart.year;
    p++;
    *p = (measRecord.recordTimeStart.year>>8);
    p++;
    *p = measRecord.recordTimeStart.month;
    p++;
    *p = measRecord.recordTimeStart.date;
    p++;
    *p = measRecord.recordTimeStart.hour;
    p++;
    *p = measRecord.recordTimeStart.minute;
    p++;
    *p = measRecord.recordTimeStart.second;
    p++;
    //����ʱ��
    *p = measRecord.recordTimeEnd.year;
    p++;
    *p = (measRecord.recordTimeEnd.year>>8);
    p++;
    *p = measRecord.recordTimeEnd.month;
    p++;
    *p = measRecord.recordTimeEnd.date;
    p++;
    *p = measRecord.recordTimeEnd.hour;
    p++;
    *p = measRecord.recordTimeEnd.minute;
    p++;
    *p = measRecord.recordTimeEnd.second;
    p++;

    //���
    FloatToChArray(measRecord.recordResult,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;
    //����
    FloatToChArray(measRecord.I,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;

    FloatToChArray(measRecord.I0,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;
    FloatToChArray(measRecord.TV,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;

    FloatToChArray(measRecord.RH,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;

    FloatToChArray(measRecord.T1,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;

    FloatToChArray(measRecord.P,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;

    FloatToChArray(measRecord.recordResultG,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;

    FloatToChArray(measRecord.MAS,a);
    *p = a[0];
    p++;
    *p = a[1];
    p++;
    *p = a[2];
    p++;
    *p = a[3];
    p++;

    *p = 0;

}


void 	FlashSysDatChange(uint8 *dat,SysRecord Record)
{
    uint8 *p;
    p = dat;

    //ʱ��
    *p = Record.recordTime.year;
    p++;
    *p = (Record.recordTime.year>>8);
    p++;
    *p = Record.recordTime.month;
    p++;
    *p = Record.recordTime.date;
    p++;
    *p = Record.recordTime.hour;
    p++;
    *p = Record.recordTime.minute;
    p++;
    *p = Record.recordTime.second;
    p++;
    //����ʱ��
    *p = Record.Event;
    p++;
    *p = (Record.ContentH);
    p++;
    *p = Record.ContentL;
    p++;
    *p = 0;

}



