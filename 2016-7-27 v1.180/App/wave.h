/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	wave.h
Author:                   ������ �ĺ麣      Version:       V2.5 Date:      2011-8-3 11:37:43
Description:    	 ���η���������

Others:
Function List:
		extern uint8 StartCV(WAVE_RUN_PARAMETER *runParameter,
					ELECTRODE_CONDITION *electrodeCondition,
					uint8 waveStart);//����CVɨ�����
		extern uint8 SetWaveform (WAVE_RUN_PARAMETER *runParameter,
					WAVE_PARAMETER *parameter,
					uint8 waveType);//�������в��β���
		extern uint8 StartWaveform(WAVE_RUN_PARAMETER *runParameter,uint8 waveStart);
		extern uint16 GetSweepTime(WAVE_PARAMETER *parameter);
		extern uint16 GetXData(MENU_PARAMETER *parameter,int16 *pdat);
		extern int8 GetYDataStart(MENU_PARAMETER *parameter,uint16 *YAddLength);
		extern void WriteRecord(uint16 recordP,MEAS_RECORD *record);//e2rom ����
		extern void ReadRecord(uint16 recordP,MEAS_RECORD *record);

History:
   1. Date:                2011-8-3 12:52:43
       Author:             ��˳��
	   Modification:    �淶��ע��
************************************************************************************************************/
#ifndef __WAVE_H
#define __WAVE_H

/*********************************************************************************************************
**               ͷ�ļ�����
*********************************************************************************************************/
#include "global.h"

/*********************************************************************************************************
**               C++�������
*********************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
    /*  __cplusplus                 */

    typedef struct {
        fp32	startVoltage; 			/*��ʼ��ѹ*/
        fp32	endVoltage;             /*������ѹ*/
        fp32	sweepSpeed;             /*ɨ���ٶ�*/
        fp32	sweepStep;              /*ɨ�貽��*/
        fp32	peakHeight;             /*����߶�*/
        fp32	dutyCycle;              /*����ռ�ձ�*/
        fp32	waveDelay;              /*������ʱms*/
        uint16	waveADCount;
    } WAVE_PARAMETER;

    typedef struct {
        uint32	time1Value; 			/*��ʱ��ֵ1 100us����*/
        uint32	time2Value;             /*��ʱ��ֵ2*/
        uint32	timeCount;              /*�ظ�����*/
        uint8	waveType;
        uint32	waveTimeDelay;			/*���μ�����ʱ*/
    } WAVE_RUN_PARAMETER;

    typedef struct {
        fp32	startVoltage; 			/*��ʼ��ѹ*/
        fp32	endVoltage;             /*������ѹ*/
        fp32	sweepSpeed;             /*ɨ���ٶ�*/
        fp32	sweepStep;              /*ɨ�貽��*/
        uint16	Count;                  /*ɨ��Ȧ��*/
    } ELECTRODE_CONDITION;

    typedef struct {
        fp32	FirstPotential;			/*��һ��ѹ*/
        fp32	SecondPotential;		/*�ڶ���ѹ*/
        fp32	DutyCycle;				/*ռ�ձ�*/
        uint16	CyclePeriod;			/*����ʱ��*/
        uint16	ConditiontionTime;		/*����ʱ��*/
    } ELECTRODE_ACTIVATION;

    typedef struct {
        uint16	filmTime; 				 /*Ĥʱ��*/
        fp32	filmVoltage;             /*Ĥ��ѹ*/
    } FILM_PARAMETER;

    typedef struct {
        fp32	mixVoltage; 			/*��ϵ�ѹ*/
        uint16	mixTime;				/*���ʱ��*/
        fp32	accumulationVoltage;	/*������ѹ*/
        uint16	accumulationTime;       /*����ʱ��*/
        fp32	staticVoltage;			/*���õ�ѹ*/
        uint16	staticTime;       		/*����ʱ��*/
        uint16	MeasureType;            /*��������*/
    } MEASURE_PARAMETER;

    typedef struct {
        fp32	concentration;
        uint16	depositTime;
        uint16	measRange;

    } MENU_RANGE;

    typedef struct {
        uint8				menuName[10];			/*�˵���*/
        uint8				sweepType;				/*ɨ�跽��*/
        MENU_RANGE			range[6];				/*���̲���*/
        fp32				mixPotential; 			/*��ϵ�ѹ*/
        uint16				mixTime;				/*���ʱ��*/
        fp32				depositPotential;		/*������ѹ*/
        fp32				holdPotential;			/*���õ�ѹ*/
        uint16				holdTime;       		/*����ʱ��*/
        fp32				sweepStart; 			/*��ʼ��ѹ*/
        fp32				sweepStop;             	/*������ѹ*/
        fp32				sweepSpeed;             /*ɨ���ٶ�*/
        fp32				sweepStep;              /*ɨ�貽��*/
        fp32				pluseHeight;            /*����߶�*/
        fp32				dutyCycle;              /*����ռ�ձ�*/
        fp32				sampleDelay;            /*������ʱms*/
        uint16				sampleCount; 			/*��������*/
        fp32				measureStart;			/*��������ʼ*/
        fp32				measureStop;			/*����������*/
        fp32				CleanPotential;			/*��ϴ��ѹ*/
        uint16				CleanTime;       		/*��ϴʱ��*/
    } MENU_PARAMETER;

    typedef struct
    {
        uint8 ParamName;
        uint8 font;		//
        uint8 length;
        uint8 decimal;
        uint8 DisSign;
    }
    DISDATSTR;
#define NUM_PARAM 25
//��������
#define PARAM_NOMAL	   			0
#define PARAM_SHIDUMENFA	   1
#define PARAM_MAXHEAT		   2
#define PARAM_MINHEATPOWER	   3
#define PARAM_LIULIANG		   4
#define PARAM_CYCLEOFSAMP	   5
#define PARAM_HIGHV			   6
#define PARAM_BIAOPIAN_JIAOZHUN	 7
#define PARAM_T1_JIAOZHUN		8
#define PARAM_DUIBIDU			9
#define PARAM_RH_JIAOZHUN		10
#define	PARAM_QIYA_JIAOZHUN		11
#define	PARAM_LIULIANG_JIAOZHUN	 12
#define PARAM_PUMP_JIAOZHUN		 13
#define PARAM_MONITONGDAO		 14
#define PARAM_MONIZHI		 15
#define PARAM_T3_JIAOZHUN		16

    extern void DisValue2(fp32 fValuel,uint16 x,uint8 y, DISDATSTR *DisDat ,uint8 *DanWei);
    extern DISDATSTR  DisDat;
    extern  DISDATSTR const DisDat2[NUM_PARAM];


    typedef struct {
//	uint8		menuName[10];
        TimeDat		recordTimeStart;
        TimeDat		recordTimeEnd;
        fp32		recordResult;
        fp32		I;
        fp32		I0;

    } MEAS_RECORD;

    extern WAVE_PARAMETER waveParameter;

    extern WAVE_RUN_PARAMETER waveRunParameter;
    extern WAVE_PARAMETER waveParameterForTest;

    extern ELECTRODE_CONDITION electrodeCondition;		//CV����

    extern ELECTRODE_ACTIVATION electrodeActivation;

    extern FILM_PARAMETER filmParameter;

    extern MEASURE_PARAMETER measureParameter;

    extern MENU_PARAMETER menuParameter[MENUADDSIZE];

    extern MENU_PARAMETER *pmenuParameter;

    extern MENU_PARAMETER runMenuParameter;

    extern MEAS_RECORD measRecord;

    extern uint16 cleanTime;

    extern uint16 sweepTime;

    extern uint16 currentGain;

//extern uint16 ADsampleResult[2000];
    /*********************************************************************************************************
    * Function name:        StartCV
    * Descriptions:         ����CVɨ�����
    * input parameters:     waveType:	��������
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern uint8 StartCV(WAVE_RUN_PARAMETER *runParameter,ELECTRODE_CONDITION *electrodeCondition,uint8 waveStart);

    /*********************************************************************************************************
    * Function name:        SetWaveform
    * Descriptions:         �������в��β���
    * input parameters:     waveType:	��������
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern uint8 SetWaveform (WAVE_RUN_PARAMETER *runParameter,WAVE_PARAMETER *parameter,uint8 waveType);

    /*********************************************************************************************************
    * Function name:        StartWaveform
    * Descriptions:         �������β�������
    * input parameters:     waveStart:	��������
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern uint8 StartWaveform(WAVE_RUN_PARAMETER *runParameter,uint8 waveStart);

    /*********************************************************************************************************
    * Function name:        GetSweepTime
    * Descriptions:         ������ɨʱ��
    * input parameters:     *parameter:	�������ò���
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern uint16 GetSweepTime(WAVE_PARAMETER *parameter);
    /*********************************************************************************************************
    * Function name:  		GetXData
    * Descriptions:   		��xdata ��ֵ
    * input parameters:   	*parameter ����������Ϣ
    * output parameters:  	*padt  xdata ����ֵ
    * Returned value:     	icount  ��������
    *********************************************************************************************************/
    extern uint16 GetXData(MENU_PARAMETER *parameter,int16 *pdat);

    /*********************************************************************************************************
    * Function name:   		GetYDataStart
    * Descriptions:      	ydata �Ĵ�С
    * input parameters:   	*parameter  	��������
    * output parameters:  	*YAddLength   ydata ��С
    * Returned value:      	0
    *********************************************************************************************************/
    extern int8 GetYDataStart(MENU_PARAMETER *parameter,uint16 *YAddLength);
    /*********************************************************************************************************
    * Function name:   		GetYDataStop
    * Descriptions:      	ydata �Ĵ�С
    * input parameters:   	*parameter  	��������
    * output parameters:  	*YPluseLength   ydata ��С
    * Returned value:      	0
    *********************************************************************************************************/

    extern int8 GetYDataStop(MENU_PARAMETER *parameter,uint16 *YPluseLength);

    /*********************************************************************************************************
    * Function name: 		WriteRecord
    * Descriptions:  		e2rom ���� ��  ����      ʱ��     ���
    * input parameters:   	recordP  �������λ��    *record 	����Ľṹ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void WriteRecord(uint16 recordP,MEAS_RECORD *record);

    /*********************************************************************************************************
    * Function name:       ReadRecord
    * Descriptions:        �Գ�����λ������ݣ� ���� ʱ�� ���
    * input parameters:    uint16 recordP  �������λ��,
    * output parameters:   MEAS_RECORD *record �����Ľṹ��
    * Returned value:      ��
    *********************************************************************************************************/
    extern void ReadRecord(uint16 recordP,MEAS_RECORD *record);











    /*********************************************************************************************************
    * Function name:        defilm
    * Descriptions:         ȥĤ
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void defilm(void);

    /*********************************************************************************************************
    * Function name:        CoatingFilm
    * Descriptions:         ��Ĥ
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void CoatingFilm(void);

    /*********************************************************************************************************
    * Function name:        clean2
    * Descriptions:         ���ܳ���λ����ϴ
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void Clean2(void);

    /*********************************************************************************************************
    * Function name:        AnodicStrippingVoltammetry
    * Descriptions:         �����ܳ���
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void AnodicStrippingVoltammetry(void);

    /*********************************************************************************************************
    * Function name:        Regulate
    * Descriptions:         ����
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void Regulate(void);

    /*********************************************************************************************************
    * Function name:        Clean
    * Descriptions:         ������ϴ��������ѹ
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void Clean(void);

    /*********************************************************************************************************
    * Function name:        ReadHistory
    * Descriptions:         ����ʷ����
    * input parameters:     ��
    * output parameters:    ��
    * Returned value:       ��
    *********************************************************************************************************/
    extern void ReadHistory(void);
    extern int32 DarwGammaLine(MENU_PARAMETER *parameter,DATA_TYPE *y_data,char flag);

    extern int32 DarwGammaLine2(MENU_PARAMETER *parameter,DATA_TYPE *y_data,char flag);


    void Regulate(void);
#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/