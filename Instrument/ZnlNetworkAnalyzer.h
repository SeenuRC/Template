/*=======================================================================
//  File Name  :  AgilentNetworkAnalyzer.h								   //
//  Author	   :  Velrajan M											   //
//  Created on : 				 									   //
//	Modified by :													   //
//  Comments:								  						   //
//		This file contains macros,class declaration and error codes    //  
//    for Agilent  Network Analyzer wrapper.                            //
//																	   //
//  Copyright (C)2009 Data Patterns (India) Pvt. Ltd.				   //
//  All Rights reserved.											   //
//  No.6, Arya Gowdar Road, West Mambalam, Chennai,India - 600033	   //
//  Email:															   //
//  Phone: 91-44-24837460 / 24848702								   //
//  FAX:   91-44-24848703											   //

  //REVISION LOG ENTRY															
  modification history
  --------------------
  
=========================================================================*/

#ifndef DPAGNA_WRP_H
#define DPAGNA_WRP_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "visa.h"
#include "visatype.h"
#include "../include/dp_types.h"

//#define TESTSEQ_WITH_TESTSUITE_APP
/*******************************DPZnl MACRO**************************/
//#define	AGNETWORKANALYZER_WRP_SIMULATE

#define DPZNL_WRP_SUCCESS				0
#define DPZNL_WRP_ZERO					0
#define DPZNL_WRP_ONE					1
#define DPZNL_WRP_TWO					2
#define DPZNL_WRP_THREE					3
#define DPZNL_WRP_FOUR					4
#define DPZNL_WRP_FIVE					5
#define DPZNL_WRP_TOTAL_MEASURES		11
#define DPZNL_WRP_TOTAL_MEASURES_TWFR	24
#define DPZNL_WRP_TOTAL_MEASURES_FUTN	14

#define DPZNL_WRP_BUFFER_SIZE				256
#define DPZNL_WRP_CHANNEL_NAME_SIZE			100
#define DPZNL_WRP_CHANNEL_MEAS_NAME_SIZE	100
#define DPZNL_WRP_DATA_BUFFER_SIZE			500

#define DPZNL_MARKER_SEARCH_MAX		0
#define DPZNL_MARKER_SEARCH_MIN		1
#define	DPZNL_MARKER_SEARCH_NPEAK	2

#define DP_NA_E5071C			1
#define DP_NA_N5241A			2

#define DP_ZNL_NA_OPEN_FAILURE -1
#define DP_ZNL_NA_OPEN_SUCCESS	0

#define DP_ZNL_NA_CFG_SUCCESS 1

#define DPZNL_HOLD			0
#define DPZNL_CONTINUOUS	1
#define	DPZNL_SINGLE		2

#define RS_SMB200A  2
#define KY_E5080B  3


/*********************DPZNL WRAPPER ERROR CODES**********************/
enum{
    DP_NA_ERR_INVALID_PARAM = -999,
	DPZNL_WRP_ERR_INVALID_SESSION_DEVICE,		
	DPZNL_WRP_ERR_INVALID_INSTANCE,		
	DPZNL_WRP_ERR_INVALID_HANDLE,							
	DPZNL_WRP_ERR_FUNC_NOT_AVAIL,
	DP_ZNL_NA_MOD_CLS_FAILURE,
	DP_ZNL_NA_SET_FREQ_FAILURE,
	DP_ZNL_NA_SET_FREQ_CENT_FAILURE,
	DP_ZNL_NA_SET_FREQ_SPAN_FAILURE,
	DP_ZNL_NA_SET_STRT_FREQ_FAILURE,
	DP_ZNL_NA_SET_STOP_FREQ_FAILURE,
	DP_ZNL_NA_SET_DISP_FAILURE,
	DP_ZNL_NA_RST_FAILURE,
    DP_ZNL_NA_PRESET_FAILURE,
    DP_ZNL_NA_NOF_TRACE_FAILURE,
	DP_ZNL_NA_PWR_LVL_FAILURE,
	DP_ZNL_NA_OPPWR_LVL_FAILURE,
	DP_ZNL_NA_PWROFF_FAILURE,
	DP_ZNL_NA_NEW_TRACE_FAILURE,
	DP_ZNL_NA_DISP_TRACE_FAILURE,
    DP_ZNL_NA_DLT_TRACE_FAILURE,
	DP_ZNL_NA_DLT_ALLTTRACE_FAILURE,
	DP_ZNL_NA_SET_DATAPNT_FAILURE,
	DP_ZNL_NA_SEL_TRACE_FAILURE,
	DP_ZNL_NA_TRACE_MEM_FAILURE,
	DP_ZNL_NA_MATH_TRACE_FAILURE,
	DP_ZNL_NA_MARKER_ON_FAILURE,
	DP_ZNL_NA_MARKER_FREQ_FAILURE,
	DP_ZNL_NA_RD_MARKER_VALUE_FAILURE,
	DP_ZNL_NA_SELF_TST_FAILURE,
	DP_ZNL_NA_PORT_CAL_FAILURE,
	DP_ZNL_NA_OPER_CMPLT_FAILURE,
	DP_ZNL_NA_LOAD_FILE_FAILURE,
	DP_ZNL_NA_SWE_MODE_FAILURE,
	DP_ZNL_NA_MARKER_TYPE_FAILURE,
	DP_ZNL_NA_MARKER_VALUE_FAILURE,
	DP_ZNL_NA_READ_MARKER_FAILURE,
	DP_ZNL_WR_SMO_FAILURE,
	DP_ZNL_NA_APER_FAILURE,
	DP_ZNL_NA_AVG_FAILURE,
	DP_ZNL_NA_AVG_MODE_FAILURE,
	DP_ZNL_NA_AVG_COUNT_FAILURE,
    DP_ZNL_NA_SCALE_FAILURE,
    DP_ZNL_NA_INT_ID_READ_FAILED,
    DP_ZNL_NA_INT_INVALID_MODEL
};

#define DPZNL_READ_COUNT			5 * 1024
#define DPZNL_1PORT_CAL				1
#define DPZNL_2PORT_CAL				2

typedef struct _NAInstCfg
{
    char m_szManufacturer[64];
    char m_szModel[32];
    unsigned short m_usInstruementID;
}NAInstCfg;

typedef struct _SNAInstHandle
{
    ViSession m_viNASession;
    unsigned short m_usInstID;
}SNAInstHandle;

typedef struct 
{	
	ViSession viInstrumentHandle;   //To hold the Board Handle	
	
}SDPAGNA_WRP_INST_HANDLER;

typedef struct
{
    double m_dFreq;
    double m_dMag;
    double m_dPhase;
}SDPAGNA_WRP_TRACE_DATA, *PSDPAGNA_WRP_TRACE_DATA;

typedef struct
{
    double m_dFreq;
    double m_dPhase;
}SDPAGNA_WRP_PHASETRACE_DATA, *PSDPAGNA_WRP_PHASETRACE_DATA;

typedef struct
{
    double m_dFreq;
    double m_dMag;
}SDPAGNA_WRP_ATTNTRACE_DATA, *PSDPAGNA_WRP_ATTNTRACE_DATA;

/************************NETWORK ANALYZER CLASS DECLARATION*********************/
class CZnlNetworkAnalyzer
{
private :
	
    //CArray<SDPAGNA_WRP_INST_HANDLER,SDPAGNA_WRP_INST_HANDLER> m_carrInsHandler;

	ViStatus status;
	ViSession m_viZnlSession[2];
	ViSession m_viZnlHandle[2];

	char m_szCmdString[50];

    char* m_strOpenStatus;

	char arrcErrStr[256];
	bool m_bOpenStatus;

	//Definition of Base class Virtual functions
	//Open the advantest
public:

    NAInstCfg m_InstCfg[2];

    char* GetModuleName();
    char* GetModuleDescription();
	
    short Open(unsigned char in_ucNAType, char in_strConStr[]);
    void GetOpenStatus(char* csStatus, bool &bstatus);

	//Get error message
    void GetErrorMessage(unsigned long in_ulVi, short in_sErrorCode, char* out_csErrMsg);

	//Close all advantest
	short CloseAllZnl();

	//Constructor
    CZnlNetworkAnalyzer();

	//Destructor
    ~CZnlNetworkAnalyzer();

    short GetInstID(char *in_pcInstDetails, unsigned short  *Out_usModel);

		//Validate handle and instance Id
	short ValidateHandle(unsigned short in_usInstanceId);

	/* functions to set the start stop frequency of the network analyzer */
    short DP_NA_Preset(unsigned short in_usInstanceId);

    short DP_NA_SetStartFrequency(unsigned short in_usInstanceId, double in_dStartFrequency, unsigned short in_usChannelNo);
	
    short DP_NA_SetStopFrequency(unsigned short in_usInstanceId, double in_dStopFrequency, unsigned short in_usChannelNo);
	
    short DP_NA_SetCenterFreqAndSpan(unsigned short in_usInstanceId, unsigned short in_usChannelNo, double in_dCenterFrequency, double in_dSpan );

	/* functions to set the output power level of the network analyzer */
    short DP_NA_SetPowerLevel(unsigned short in_usInstanceId, double in_dInputPower, unsigned short in_usChannelNo);

    short DP_NA_SetFrequency(unsigned short in_usInstanceId, double in_dFrequency, unsigned short in_usChannelNo);

    short DP_NA_CreateNewTrace(unsigned short in_usInstanceId, unsigned short in_usPortNo, unsigned short in_usChannelNo);

    short DP_NA_DisplayTrace(unsigned short in_usInstanceId, unsigned short in_usTraceNo, unsigned short in_usPortNo, unsigned short in_usChannelNo);

    short DP_NA_SetDisplayFormat(unsigned short in_usInstanceId, unsigned short in_usFormat, unsigned short in_usChannelNo);

    short DP_NA_DeleteTrace(unsigned short in_usInstanceId, unsigned short in_usTraceNo, unsigned short in_usPortNo, unsigned short in_usChannelNo);
	
    short DP_NA_DeleteAllTraces(unsigned short in_usInstanceId);

    short DP_NA_SetDataPoints(unsigned short in_usInstanceId, unsigned short in_usDataPoints, unsigned short in_usChannelNo);

    short DP_NA_SelectTrace(unsigned short in_usInstanceId, unsigned short in_usDataPoints,  unsigned short in_usTraceNo, unsigned short in_usPortNo, unsigned short in_usChannelNo);

    short DP_NA_StoreData(unsigned short in_usInstanceId, unsigned short in_usChannelNo);

    short DP_NA_DoMathOperations(unsigned short in_usInstanceId, unsigned short in_usMathOperator, unsigned short in_usChannelNo);

    short DP_NA_SetMarkerPosAndFreq(unsigned short in_usInstanceId, unsigned short in_usMarkerNo, double in_dMarkerFreq, unsigned short in_usChannelNo);

    short DP_NA_ReadMarkerValue(unsigned short in_usInstanceId, unsigned short in_usMarkerNo,unsigned char in_ucMeasType, unsigned short in_usChannelNo, double *Out_dMarkerValue);

    short DP_NA_SetPowerON(unsigned short in_usInstanceId);

    short DP_NA_SetPowerOFF(unsigned short in_usInstanceId);

    short DP_NA_SelfTest(unsigned short in_usInstanceId, short *out_sStatus);

    short DP_NA_UserPreset(unsigned short in_usInstanceId);
	
    short DP_NA_Calibration(unsigned short in_usInstanceId, unsigned char in_ucPortNo, unsigned char in_ucCalTypeSel /*1 - 1 Port Cal 2 - 2 Port Cal*/);
	
    short DP_NA_LoadStateFromFile(unsigned short in_usInstanceId,char* in_csFileName);
	
    short DP_NA_NumberOfTrace(unsigned short in_usInstanceId, unsigned short in_usTraceNo);

    short DP_NA_StoreStateIntoFile(unsigned short in_usInstanceId,char* in_csFileName);

    short DP_NA_Trigger(unsigned short in_usInstanceId,unsigned char in_ucTriggerMode);

    short DP_NA_SetMarkerSearchType(unsigned short in_usInstanceId,unsigned short in_usChannelNo, unsigned short in_usMarkerNo,  unsigned short in_usSearchType,double *Out_dMarkerValue, double *Out_dMarkerFreq);
	
    short DP_NA_SetSmoothing(unsigned short in_usInstanceId, unsigned short in_usEnableDisable, unsigned short in_usPoints, float in_fAperture);

    short DP_NA_SetSweepMode(unsigned short in_usInstanceId, unsigned short in_usChannelNo, unsigned char in_ucMode);

    short DP_NA_SetAveraging(unsigned short in_usInstanceId, unsigned short in_usEnableDisable, unsigned char in_ucAvgMode, unsigned short in_usAvgPoints);

    short DP_NA_SetScale(unsigned short in_usInstanceId, unsigned char in_ucTrace, double in_dScale);
	
    short DP_NA_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize);

    short DP_NA_SetNormalize(unsigned short in_usInstanceId);

    short DP_NA_ReadTraceData(unsigned short in_usInstanceId, double in_dStartFreq, double in_dStopFreq, unsigned int in_uiNoofPoints, PSDPAGNA_WRP_TRACE_DATA out_pSTraceData);

    short DP_NA_ReadPhaseTraceData(unsigned short in_usInstanceId, double in_dStartFreq, double in_dStopFreq, unsigned int in_uiNoofPoints, PSDPAGNA_WRP_PHASETRACE_DATA out_pSPhaseTraceData);

    short DP_NA_ReadAttnTraceData(unsigned short in_usInstanceId, double in_dStartFreq, double in_dStopFreq, unsigned int in_uiNoofPoints, PSDPAGNA_WRP_ATTNTRACE_DATA out_pSAttnTraceData);

    short DP_NA_SetIFBandWidth(unsigned short in_usInstanceId, double in_dBandWidth);

    short DP_NA_ReadSweepTime(unsigned short in_usInstanceId, double *Out_dSweepTime);

    short DP_NA_SetSweepTime(unsigned short in_usInstanceId, double in_dSweepTime);

    ViStatus _VI_FUNC  AgNA_close ( ViSession Vi );

    ViStatus _VI_FUNC  AgNA_error_query ( ViSession Vi, ViInt32* ErrorCode, ViChar ErrorMessage[] );

    ViStatus _VI_FUNC  AgNA_error_message ( ViSession Vi, ViStatus ErrorCode, ViChar ErrorMessage[] );
};

#endif
