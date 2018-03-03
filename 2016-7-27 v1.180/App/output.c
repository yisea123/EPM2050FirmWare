//����ӿڰ�Mdbus ������
#include 		"Output.h"
OUTPUTSTRUCT 	outputstruct1;
SysRecord 		goSysRecor;
uint8 			SysrecordReadFlg;
uint8 			OutPutFlg = 0;
uint16          inputDat[16];//��ȡ�۳��ǻ���
uint16 			outputDat[8];

//����Ĵ���

//����Ĵ���
// �������ѹУ׼ʹ��
// void  I_Vcal(uint16 dat)
// {

// //1 У׼���� 2 У׼��� ��Ϊ0����ֵ��
// 	OSWriteSingleReg(0x01,400000,dat );

// }


//�ӿڰ���Դ�����ӻ���
void testboard(uint8 action)
{
    (action)?(OSWriteSingleReg(1,10,0x7fff)):(OSWriteSingleReg(1,10,0x7000));
}

//д���ּĴ���������� ���� 1����ǰ�׶� 2λ
//����Ϊ���
void DigitalOutput(void)
{
    uint16 dat;
    if((MeasureStepFlg<5)&&(MeasureStepFlg>0))
    {
        if(EEORStuation1)
        {
            dat = ((MeasureStepFlg-1)<<1)|0x01;
        }
        else
        {
            dat = ((MeasureStepFlg-1)<<1)|0x00;
        }
        OSWriteSingleReg(1,10,dat);
    }
}


//�������
void OutputCONC(OUTPUTSTRUCT dat)
{
    uint16 *outputdat;
    outputdat = outputDat;

    if(dat.temperature1<0)
    {
        dat.temperature1 = 0;
    }
    *outputdat = (uint32)(160.0*dat.temperature1)+4000	;//�¶�1���ɵ���ֵ
    if(*outputdat>20000)
    {
        *outputdat=20000;
    }

    if(dat.temperature2<0)
    {
        dat.temperature2 = 0;
    }
    outputdat++;
    *outputdat = (uint32)(160.0*dat.temperature2)+4000	;//�¶�2���ɵ���ֵ
    if(*outputdat>20000)
    {
        *outputdat=20000;
    }
    if(dat.temperature3<0)
    {
        dat.temperature3 = 0;
    }
    outputdat++;
    *outputdat = (uint32)(160.0*dat.temperature3)+4000	;//�¶�3���ɵ���ֵ
    if(*outputdat>20000)
    {
        *outputdat=20000;
    }

    if(dat.RH<0)
    {
        dat.RH = 0;
    }
    outputdat++;
    *outputdat = (uint32)(160.0*dat.RH)+4000	;//���Բ�ʪ��RH1���ɵ���ֵ
    if(*outputdat>20000)
    {
        *outputdat=20000;
    }

    if(dat.flow1<0)
    {
        dat.flow1= 0;
    }
    outputdat++;
    *outputdat = (uint32)(800.0*dat.flow1)+4000	;//����1���ɵ���ֵ
    if(*outputdat>20000)
    {
        *outputdat=20000;
    }
    if(dat.flow2<0)
    {
        dat.flow2= 0;
    }
    outputdat++;
    *outputdat = (uint32)(800.0*dat.flow2)+4000	;//����2���ɵ���ֵ
    if(*outputdat>20000)
    {
        *outputdat=20000;
    }

    if(dat.CONC<0)
    {
        dat.CONC = 0;
    }
    outputdat++;
    *outputdat = (uint32)(32.0*dat.CONC)+4000	;//Ũ�Ȼ��ɵ���ֵ
    if(*outputdat>20000)
    {
        *outputdat=20000;
    }

    if(dat.paper<0)
    {
        dat.paper = 0;
    }
    outputdat++;
    *outputdat = (uint32)(160.0*dat.paper)+4000	;//ʣ��ֽ�����ɵ���ֵ
    if(*outputdat>20000)
    {
        *outputdat=20000;
    }

    OSWriteMultipleReg(1,12,8,outputDat);
}



/*********************************************************************************************************
* Function name:        ReadHistory
* Descriptions:         ����ʷ����
* input parameters:     ��
* output parameters:    ��
* Returned value:       ��
*********************************************************************************************************/
void ReadHistory(void)
{
    MEAS_RECORD rRecord2;
// 	/*��ȡ��Ӧ�����ʷ��¼*/

// 	if(usRegHoldingBuf[201]== 0)
// 	{
    recordOutput = recordPoint;
// 	}
    ReadFlashRecordSimple((recordOutput -1),&rRecord2); //�˴���ע��

// 	ReadRecordSimple((recordPoint - usRegHoldingBuf[201] -1),&rRecord2); //�˴���ע��
// 	/*��ֵ��ʷ����*/
// 	usRegHoldingBuf[200] = 	recordPoint;
    /*������*/
    usRegHoldingBuf[200] = 	 rRecord2.recordTimeStart.year;
    usRegHoldingBuf[201] = 	(rRecord2.recordTimeStart.month) & 0x00ff;
    usRegHoldingBuf[202] = 	(rRecord2.recordTimeStart.date) & 0x00ff;
    /*ʱ��*/
    usRegHoldingBuf[203] = 	(rRecord2.recordTimeStart.hour) & 0x00ff;
    usRegHoldingBuf[204] = 	(rRecord2.recordTimeStart.minute) & 0x00ff;
    /*jiesuʱ��*/
    usRegHoldingBuf[205] = 	(rRecord2.recordTimeEnd.hour) & 0x00ff;
    usRegHoldingBuf[206] = 	(rRecord2.recordTimeEnd.minute) & 0x00ff;

    /*���*/
    if((rRecord2.recordResult<0.0)||(GetNumLen(rRecord2.recordResult)==127)) //������Ч
    {
        usRegHoldingBuf[207] = 	0xffff;
    }
    else
    {
        usRegHoldingBuf[207] = 	(uint16)(rRecord2.recordResult*10);
    }	/*gongkuang���*/
    if((rRecord2.recordResultG<0.0)||(GetNumLen(rRecord2.recordResultG)==127)) //������Ч
    {
        usRegHoldingBuf[208] = 	0xffff;
    }
    else
    {
        usRegHoldingBuf[208] = 	(uint16)(rRecord2.recordResultG*10);
    }
    //�ۼƱ�����
    usRegHoldingBuf[209] = rRecord2.TV;
    //I, I0 RH
    usRegHoldingBuf[210] = rRecord2.I;
    usRegHoldingBuf[211] = rRecord2.I0;
    usRegHoldingBuf[212] = rRecord2.RH*10;

    //�¶Ȳ���ֵ
    usRegHoldingBuf[213] = rRecord2.MAS;

    //�����¶�
    usRegHoldingBuf[214] = rRecord2.T1*10;

    //��������ѹ
    usRegHoldingBuf[215] = rRecord2.P*10;

}
void ReadHistoryM(void)
{
    /*��ȡ��Ӧ�����ʷ��¼*/
    //����ʱ��
    usRegHoldingBuf[220] = TimeDate.month;
    usRegHoldingBuf[221] = TimeDate.date;
    usRegHoldingBuf[222] = TimeDate.hour;
    usRegHoldingBuf[223] = TimeDate.minute;
    //�����¶�
    usRegHoldingBuf[224] = WenDuT1*10;
    //��������ѹ
    usRegHoldingBuf[225] = QiYa*10;

    //��������
    usRegHoldingBuf[226] = LiuLiang_GK*100;
    //���Ũ��
    usRegHoldingBuf[227] = 	measRecord.recordResult*10;
    //����Ũ��
    usRegHoldingBuf[228] = 	Parg_PM25_GK*10;
    //T2
    usRegHoldingBuf[229] = 	WenDuT2*10;
    //T3
    usRegHoldingBuf[230] = 	WenDuT3*10;
    //TV_GK
    usRegHoldingBuf[231] = 	TV_GK/60.0;
    //TV_BK
    usRegHoldingBuf[232] = 	TV_BK/60.0;
    //F2
    usRegHoldingBuf[233] = 	LiuLiang_BK*100;
    //RH
    usRegHoldingBuf[234] = 	HumiditySample*10;
    //״̬��
    usRegHoldingBuf[235] = 	RunStuation;
    //����
    usRegHoldingBuf[236] = 	WarnStuation;
    //����
    usRegHoldingBuf[237] = 	EEORStuation1;
    //ͨ��
    usRegHoldingBuf[238] = 	0;
    //�ÿ����ر�FLG
    usRegHoldingBuf[239] = 	!PumpFlg;
    //DHS���ȿ����ر�FLG
    usRegHoldingBuf[240] = 	!HeatFlg;
    //��ǰ�׶�ʣ��ʱ��
    usRegHoldingBuf[241] = 	100;
    //�������¶�
    usRegHoldingBuf[242] = 	MaxHeatTemp;
    //���õĲ�������
    usRegHoldingBuf[243] = 	FlowOfSamp;
    //���õĲ�������
    usRegHoldingBuf[244] = 	CycleOfSamp;
    //Uֵ
    usRegHoldingBuf[245] = 	Parg_u;
    //T1 jiaoz
    usRegHoldingBuf[246] = 	CaliParT1;
    //t3 jiaoz
    usRegHoldingBuf[247] = 	CaliParT3;
    //p jiaoz
    usRegHoldingBuf[248] = 	CaliParP;
    //f1 jiaoz
    usRegHoldingBuf[249] = 	CaliParF1;
    //rh jiaoz
    usRegHoldingBuf[250] = 	CaliParRH;
    //ϵͳ��־��Ũ�����ݸ��±�־��
    if(SysrecordReadFlg>25) SysrecordReadFlg = 25;

    usRegHoldingBuf[251] = 	(CUpdateFlg<<8)|SysrecordReadFlg;

    usRegHoldingBuf[252] =  PowerV1*10;

    usRegHoldingBuf[253] =  PowerV2*10;

    usRegHoldingBuf[254] =  PowerV3*(-10);

    usRegHoldingBuf[255] =  PowerV4*10;

    usRegHoldingBuf[256] =  TCQWenDuT4*10;  //̽�����¶Ȳ���ֵ

    usRegHoldingBuf[257] =  CYCShiDu*10;    //������ʪ��ֵ

}


void  ReadSysRecord(SysRecord *record ,uint32 recordP)
{
    uint8 *dat,err;
    dat = &DatForRecord[0];
    recordP = (recordP*10)%0x3000;//
    recordP = recordP+RECORDSYSLOG;
    OSMutexPend(OSFlashUsedMutex,0,&err);
    SSTF016B_RD( recordP,10,dat);
    OSMutexPost(OSFlashUsedMutex);


    record->recordTime.year=(dat[0])|(dat[1]<<8);
    record->recordTime.month=dat[2];
    record->recordTime.date=dat[3];
    record->recordTime.hour=dat[4];
    record->recordTime.minute=dat[5];
    record->recordTime.second=dat[6];

    record->Event=dat[7];
    record->ContentH=dat[8];
    record->ContentL=dat[9];

}

void ReadHistorySys(void)
{
    SysRecord loSysRecor;
    int i,TempNum;
    TempNum = usRegHoldingBuf[3]-1;
    usRegHoldingBuf[259] = SysrecordPoint;
    for(i = 0; i<20; i++)
    {
        //��
        ReadSysRecord(&loSysRecor,(SysrecordPoint-1-i-20*TempNum));
        //��ֵ
        usRegHoldingBuf[260+i*6] = (uint16)(int16)(loSysRecor.recordTime.year);
        usRegHoldingBuf[261+i*6] = (uint16)(loSysRecor.recordTime.month<<8)|(loSysRecor.recordTime.date);
        usRegHoldingBuf[262+i*6] = (uint16)(loSysRecor.recordTime.hour<<8)|(loSysRecor.recordTime.minute);
        usRegHoldingBuf[263+i*6] = (uint16)loSysRecor.recordTime.second;

        usRegHoldingBuf[264+i*6] = (uint16)loSysRecor.Event;
        usRegHoldingBuf[265+i*6] = (uint16)(loSysRecor.ContentH<<8)|(loSysRecor.ContentL);
    }

}
void ReadHistorySysMin(void)
{
    SysRecord loSysRecor;
    int i;

    if(SysrecordReadFlg>20)
    {
        SysrecordReadFlg = 20;
    }
// 	usRegHoldingBuf[259] = SysrecordPoint;
    for(i = 0; i<SysrecordReadFlg; i++)
    {
        //��
        ReadSysRecord(&loSysRecor,(SysrecordPoint-1-i));
        //��ֵ
        usRegHoldingBuf[260+i*6] = (uint16)(int16)(loSysRecor.recordTime.year);
        usRegHoldingBuf[261+i*6] = (uint16)(loSysRecor.recordTime.month<<8)|(loSysRecor.recordTime.date);
        usRegHoldingBuf[262+i*6] = (uint16)(loSysRecor.recordTime.hour<<8)|(loSysRecor.recordTime.minute);
        usRegHoldingBuf[263+i*6] = (uint16)loSysRecor.recordTime.second;
        usRegHoldingBuf[264+i*6] = (uint16)loSysRecor.Event;
        usRegHoldingBuf[265+i*6] = (uint16)(loSysRecor.ContentH<<8)|(loSysRecor.ContentL);
    }
}

void ReadHistoryDat(void)
{
    int i;
    MEAS_RECORD rRecord2;
    /*��ȡ��Ӧ�����ʷ��¼*/
    recordOutput = recordPoint;
    usRegHoldingBuf[9] = 	recordOutput;
    for(i=0; i<7; i++)
    {
        ReadFlashRecordSimple((recordOutput-i-1-(usRegHoldingBuf[4]-1)*7),&rRecord2); //�˴���ע��
        /*������*/
        usRegHoldingBuf[10+16*i] = 	 rRecord2.recordTimeStart.year;
        usRegHoldingBuf[11+16*i] = 	(rRecord2.recordTimeStart.month) & 0x00ff;
        usRegHoldingBuf[12+16*i] = 	(rRecord2.recordTimeStart.date) & 0x00ff;
        /*ʱ��*/
        usRegHoldingBuf[13+16*i] = 	(rRecord2.recordTimeStart.hour) & 0x00ff;
        usRegHoldingBuf[14+16*i] = 	(rRecord2.recordTimeStart.minute) & 0x00ff;
        /*jiesuʱ��*/
        usRegHoldingBuf[15+16*i] = 	(rRecord2.recordTimeEnd.hour) & 0x00ff;
        usRegHoldingBuf[16+16*i] = 	(rRecord2.recordTimeEnd.minute) & 0x00ff;

        /*���*/
        if((rRecord2.recordResult<0.0)||(GetNumLen(rRecord2.recordResult)==127)) //������Ч
        {
            usRegHoldingBuf[17+16*i] = 	0xffff;
        }
        else
        {
            usRegHoldingBuf[17+16*i] = 	(uint16)(rRecord2.recordResult*10);
        }	/*gongkuang���*/
        if((rRecord2.recordResultG<0.0)||(GetNumLen(rRecord2.recordResultG)==127)) //������Ч
        {
            usRegHoldingBuf[18+16*i] = 	0xffff;
        }
        else
        {
            usRegHoldingBuf[18+16*i] = 	(uint16)(rRecord2.recordResultG*10);
        }
        //�ۼƱ�����
        usRegHoldingBuf[19+16*i] = rRecord2.TV;
        //I, I0 RH
        usRegHoldingBuf[20+16*i] = rRecord2.I;
        usRegHoldingBuf[21+16*i] = rRecord2.I0;
        usRegHoldingBuf[22+16*i] = rRecord2.RH*10;


        //�¶Ȳ���ֵ
        usRegHoldingBuf[23+16*i] = rRecord2.MAS;


        //�����¶�
        usRegHoldingBuf[24+16*i] = rRecord2.T1*10;

        //��������ѹ
        usRegHoldingBuf[25+16*i] = rRecord2.P*10;
    }
}




