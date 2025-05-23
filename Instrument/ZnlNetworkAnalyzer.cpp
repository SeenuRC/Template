/*=======================================================================
//  File Name  :  ZNLNetworkAnalyzer.cpp								//
//  Author	   :  -									   	//
//  Created on :  10-Jan-23		 									//
//	Modified by :													   	//
//  Comments:								  						   	//
//		This file contains , Wrapper class definitoin of advantest  	//
//																	   	//
//  Copyright (C)2009 Data Patterns (India) Ltd.				   	//
//  All Rights reserved.											   	//
//  No.6, Arya Gowdar Road, West Mambalam, Chennai,India - 600033	   	//
//  Email:															   	//
//  Phone: 91-44-24837460 / 24848702								   	//
//  FAX:   91-44-24848703											   	//

  //REVISION LOG ENTRY
  modification history
  --------------------

=========================================================================*/

#include "ZnlNetworkAnalyzer.h"
#include <unistd.h>
#include "qthread.h"


ViSession instrsession = 0;
SNAInstHandle m_arrSNAInstHandle[16];
extern unsigned char g_ucNACount;

/**************************************************************************
// Name						:  CZnlNetworkAnalyzer
// Author					:  -
// Global Variables affected:  NIL
// Created Date				:  10-Jan-23	
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Initalize the member variable and register the 
                               wrapper functions.
**************************************************************************/
CZnlNetworkAnalyzer::CZnlNetworkAnalyzer()
{	   

    memset(m_arrSNAInstHandle, 0, sizeof(m_arrSNAInstHandle) );
}

/**************************************************************************
// Name						:  ~CZnlNetworkAnalyzer
// Author					:  -
// Global Variables affected:  NIL
// Created Date				:  22-09-09 	
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Close All advantest
**************************************************************************/
CZnlNetworkAnalyzer::~CZnlNetworkAnalyzer()
{
    //Close all ZNL Network anlayzer
    CloseAllZnl();
}

/**************************************************************************
// Name						: GetErrorMessage
// Author					: -
// Global Variables affected: NIL				 
// Created Date				: 10-Jan-23	   
// Revision Date			: NIL	 
// Reason for Revising		: NIL
// Description				: To return error message corresponding error code.
**************************************************************************/

short CZnlNetworkAnalyzer:: DP_NA_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize)
{
    //Variable Declaration
    char szErrStr[DPZNL_WRP_BUFFER_SIZE] = {0};

    if(out_ps8ErrStringBuf == NULL)
        return DP_NA_ERR_INVALID_PARAM;

    switch(in_s32ErrCode)
    {
    case DP_ZNL_NA_CFG_SUCCESS:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Configuration Success");
        break;

    case DP_ZNL_NA_OPEN_SUCCESS:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Open Success");
        break;

    case DP_ZNL_NA_OPEN_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Open Failure");
        break;

    case DPZNL_WRP_ERR_INVALID_SESSION_DEVICE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Could not open a session");
        break;

    case DPZNL_WRP_ERR_INVALID_HANDLE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid Board Handle");
        break;

    case DPZNL_WRP_ERR_INVALID_INSTANCE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid Instance ID");
        break;

    case DPZNL_WRP_ERR_FUNC_NOT_AVAIL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Function Not Available");
        break;

    case DP_ZNL_NA_MOD_CLS_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "All Modules Close Failed");
        break;

    case DP_ZNL_NA_SET_FREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Frequency to Network Analyzer Failed");
        break;

    case DP_ZNL_NA_SET_FREQ_CENT_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Center Frequency to Network Analyzer Failed");
        break;

    case DP_ZNL_NA_SET_FREQ_SPAN_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Span to Network Analyzer Failed");
        break;

    case DP_ZNL_NA_SET_STRT_FREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Start Frequency to Network Analyzer Failed");
        break;

    case DP_ZNL_NA_SET_STOP_FREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Stop Frequency to Network Analyzer Failed");
        break;

    case DP_ZNL_NA_SET_DISP_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Display Format for the Measurement Failed");
        break;

    case DP_ZNL_NA_RST_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Reset Failed");
        break;

    case DP_ZNL_NA_PRESET_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Preset Failed");
        break;

    case DP_ZNL_NA_PWR_LVL_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Power Level to Network Analyzer Failed");
        break;

    case DP_ZNL_NA_NOF_TRACE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set No of Trace Failed");
        break;

    case DP_ZNL_NA_OPPWR_LVL_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Output Power Level to Network Analyzer Failed");
        break;

    case DP_ZNL_NA_PWROFF_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Output Power Off Failed");
        break;

    case DP_ZNL_NA_NEW_TRACE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Creating New Trace Failed");
        break;

    case DP_ZNL_NA_DISP_TRACE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Displaying Newly Created Trace Failed");
        break;

    case DP_ZNL_NA_DLT_ALLTTRACE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Deleting All Traces Failed");
        break;

    case DP_ZNL_NA_SET_DATAPNT_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Sets the Number of Data Points For the Measurement Failed");
        break;

    case DP_ZNL_NA_SEL_TRACE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Trace Selection Failed");
        break;

    case DP_ZNL_NA_TRACE_MEM_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Measurement Trace Selection puts into Memory Failed");
        break;

    case DP_ZNL_NA_MATH_TRACE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Sets Math operations on the Selected Measurement and to store Trace in Memory Failed");
        break;

    case DP_ZNL_NA_MARKER_ON_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Turns the Specified Marker ON Failed");
        break;

    case DP_ZNL_NA_DLT_TRACE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Deleting Traces Failed");
        break;

    case DP_ZNL_NA_MARKER_FREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Sets Marker Frequency Failed");
        break;

    case DP_ZNL_NA_RD_MARKER_VALUE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Read Specified Marker Value Failed");
        break;

    case DP_ZNL_NA_SELF_TST_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Self Test Failed");
        break;

    case DP_ZNL_NA_PORT_CAL_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Selects a one-port or two-port calibration type Failed");
        break;

    case DP_ZNL_NA_OPER_CMPLT_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Operation Complete Failed");
        break;

    case DP_ZNL_NA_LOAD_FILE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Loads cal kit data from the specified cal kit file Failed");
        break;

    case DP_ZNL_NA_SWE_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set the Mode for trigger Failed");
        break;

    case DP_ZNL_NA_MARKER_TYPE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Marker search Type Failed");
        break;

    case DP_ZNL_NA_MARKER_VALUE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Get Marker Value Failed");
        break;

    case DP_ZNL_NA_READ_MARKER_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Read Specified Marker Frequency Failed");
        break;

    case DP_ZNL_WR_SMO_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Activate Smoothing for default Trace Failure");
        break;

    case DP_ZNL_NA_APER_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Smoothing Aperture Failure");
        break;

    case DP_ZNL_NA_AVG_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Enable or Disable Sweep Average Failure");
        break;

    case DP_ZNL_NA_AVG_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Set Sweep Average Mode Failure");
        break;

    case DP_ZNL_NA_AVG_COUNT_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Number of consecutive sweeps to be combined for the sweep average Failed");
        break;

    case DP_ZNL_NA_SCALE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Changing Scale Failed");
        break;

    case DP_ZNL_NA_INT_ID_READ_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Model Number Read Failure!");
        break;

    case DP_ZNL_NA_INT_INVALID_MODEL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Network Analyzer Invalid Model!");
        break;

    default:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid Error Code !");
    }
}

/********************** Basic functions **********************/
/**************************************************************************
// Name						:  Open
// Author					:  - 
// Global Variables affected:  NIL
// Created Date				:  10-Jan-23	
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Initialize DP2502 module
**************************************************************************/
short CZnlNetworkAnalyzer::Open(unsigned char in_ucNA, char in_strConStr[])
{
    ViSession defaultRM    = 0;

    ViChar value[128]      = {0};
    char ucTemp[20]        = {0};
    char ucLoop            =  0;
    unsigned short usLoop  = 0;
    char *strData	       = NULL;
    char arripadress[100]={0};
    unsigned short usInstID = 0;
    unsigned int uiReadCnt = 0;
    char szInstDetails [256] = {NULL};

    SDPAGNA_WRP_INST_HANDLER InstanceHandle ;
    memset(&InstanceHandle,0,sizeof(InstanceHandle));

#ifndef	ZNLNETWORKANALYZER_WRP_SIMULATE	

    status = viOpenDefaultRM (&defaultRM);

    if (status != VI_SUCCESS)
    {
        return -3;//DP_ZNL_NA_OPEN_FAILURE;
    }
#endif

    for( usLoop = 0; usLoop < 1; usLoop++ )
    {
        m_bOpenStatus = false;

#ifndef	ZNLNETWORKANALYZER_WRP_SIMULATE			
        try
        {
            strcpy(value, in_strConStr);

            for( ucLoop = 8; ucLoop <= 18; ucLoop++)
            {
                ucTemp[ucLoop-8] = value[ucLoop];
            }
            //strcpy(gGlobalStuct.sCOTSInstInfo.m_cNAInfo,ucTemp);
            //sprintf(arripadress, "TCPIP0::%s::inst0::INSTR", in_strConStr);

            status = viOpen (defaultRM,  in_strConStr, VI_NULL, VI_NULL, &instrsession);
            if(status != VI_SUCCESS)
            {
                return -2;//DP_ZNL_NA_OPEN_FAILURE;
            }
            else
            {
                //gGlobalStuct.sHandles.m_viRFNA[0] = instrsession;
            }

            // m_viZnlHandle[usLoop] = instrsession;

            status = viSetAttribute(instrsession, VI_ATTR_SUPPRESS_END_EN, VI_FALSE);
            if(status != VI_SUCCESS)
            {
                return DP_ZNL_NA_OPEN_FAILURE;
            }


            status = viSetAttribute(instrsession, VI_ATTR_TERMCHAR_EN, VI_TRUE);
            if(status != VI_SUCCESS)
            {
                return DP_ZNL_NA_OPEN_FAILURE;
            }

            if(1)
            {
                status = viGetAttribute(instrsession, VI_ATTR_RSRC_NAME, value);
                if(status != VI_SUCCESS)
                {
                    sprintf(strData,"%d, %s",status, value );
                    return DP_ZNL_NA_OPEN_FAILURE;
                }

                unsigned char  ucTemp[20] = {0};
                for(unsigned char  ucLoop = 8; ucLoop <= 18; ucLoop++)
                {
                    ucTemp[ucLoop-8] = value[ucLoop];
                }
            }

            /*Read Instrument Type*/

            status = viPrintf(instrsession, (char*)"*IDN?\n");
            if(status)
            {
                return DP_ZNL_NA_INT_ID_READ_FAILED;
            }

            status = viBufRead(instrsession, (ViPBuf)szInstDetails, 256, (ViPUInt32)&uiReadCnt);
            if(status)
            {
                return DP_ZNL_NA_INT_ID_READ_FAILED;
            }

            /*Read the Model ID*/
            status = GetInstID(szInstDetails, &usInstID);

            if(!status )
            {
                viClose(instrsession);

                return DP_ZNL_NA_INT_INVALID_MODEL;
            }
            else
            {
                m_arrSNAInstHandle[in_ucNA-1].m_viNASession = instrsession;
                m_arrSNAInstHandle[in_ucNA-1].m_usInstID = usInstID;
            }

        }
        catch(...)
        {

        }

#endif	

        m_bOpenStatus = true;

    }
    return DP_ZNL_NA_OPEN_SUCCESS;
}


/**************************************************************************
// Name						: GetOpenStatus
// Author					: -
// Global Variables affected: NIL				 
// Created Date				: 10-Jan-23   
// Revision Date			: NIL	 
// Reason for Revising		: NIL
// Description				: To return error message corresponding error code.
**************************************************************************/
void CZnlNetworkAnalyzer::GetOpenStatus(char *csStatus, bool &bStatus)
{
    csStatus = m_strOpenStatus;
    bStatus = m_bOpenStatus;
}

/**************************************************************************
// Name						:  CloseAllZnl
// Author					:  M.Velrajan
// Global Variables affected:  NIL
// Created Date				:  10-Jan-23 	
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Close all advantest modules
**************************************************************************/
short CZnlNetworkAnalyzer::CloseAllZnl()
{
    unsigned short usLoop = 0;
#ifndef	ZnlNETWORKANALYZER_WRP_SIMULATE
    status = viClose(instrsession);

    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_MOD_CLS_FAILURE;
    }
#endif		
    //gGlobalStuct.sCOTSInstInfo.m_bNA = 0;
    return DP_ZNL_NA_CFG_SUCCESS;
}

/**************************************************************************
// Name						:  GetModuleName
// Author					:  - 
// Global Variables affected:  NIL
// Created Date				:  10-Jan-23	
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Returns module name
**************************************************************************/

char * CZnlNetworkAnalyzer::GetModuleName()
{
    //Return Module Name
    //	return m_strModuleName;
    return "";
}

/*********************************************************************************
// Name						: ValidateHandle  
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To validate the handle.
*********************************************************************************/
short CZnlNetworkAnalyzer::ValidateHandle(unsigned short in_usInstanceId)
{
    // Invalid instance id checking.

    return DP_ZNL_NA_CFG_SUCCESS;
}
/********************** End of Basic functions **********************/

/********************** Network Analyzer functions **********************/

/*********************************************************************************
// Name						: ZNL_SetFrequency  
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the frequency of the network analyzer.
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_SetFrequency(unsigned short in_usInstanceId, double in_dFrequency, unsigned short in_usChannelNo)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "SENS%d:FREQ:FIX  %lf\n", in_usChannelNo, in_dFrequency);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else{
//        sprintf(m_szCmdString, "SENS%d:FREQ:CW  %lf\n", in_usChannelNo, in_dFrequency);
//        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        sprintf(m_szCmdString, "CALC%d:MEAS1:MARK%d ON\n", in_usChannelNo, 1);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_SET_FREQ_FAILURE;
        }

        /** CALC:MEAS:MARK:REF:X 1e9

                    calculate2:measure1:marker:reference:x 1e6 **/

        sprintf(m_szCmdString, "CALC%d:MEAS1:MARK%d:X %lf\n",in_usChannelNo,1,in_dFrequency);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_SET_FREQ_FAILURE;
        }

        QThread::msleep(100);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SET_FREQ_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetCenterFreqAndSpan  
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the center frequency and span to the network analyzer.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetCenterFreqAndSpan(unsigned short in_usInstanceId, unsigned short in_usChannelNo, double in_dCenterFrequency, double in_dSpan )
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "SENS%d:FREQ:CENT %f\n",in_usChannelNo, in_dCenterFrequency);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_SET_FREQ_CENT_FAILURE;
        }

        sprintf(m_szCmdString, "SENS%d:FREQ:SPAN %f\n", in_usChannelNo, in_dSpan);

        /* Setting the center frequency to NA*/
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else{
        sprintf(m_szCmdString, "SENS%d:FREQ:CENT %f\n",in_usChannelNo, in_dCenterFrequency);
        /* Setting the center frequency to NA*/
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_SET_FREQ_CENT_FAILURE;
        }

        sprintf(m_szCmdString, "SENS%d:FREQ:SPAN %f\n", in_usChannelNo, in_dSpan);

        /* Setting the center frequency to NA*/
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SET_FREQ_SPAN_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetStartFrequency
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the start frequency of the network analyzer. 
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetStartFrequency(unsigned short in_usInstanceId, double in_dStartFrequency, unsigned short in_usChannelNo)
{
    U8BIT ucOPCCnt = 0;
    int iOper = 0;

    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "SENS%d:FREQ:STAR %lfHz\n", in_usChannelNo, in_dStartFrequency);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else{
        sprintf(m_szCmdString, "SENS%d:FREQ:STAR %lfHz\n",in_usChannelNo,in_dStartFrequency); //change
        //updated by moorthy
        /* Setting the start frequency */

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SET_STRT_FREQ_FAILURE;
    }

    while(!iOper)
    {
        /*wait for operation to complete*/

        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (char*)"*OPC?\n", "%d", &iOper);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetStopFrequency
// Author					: Sankar M 
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the stop frequency of the network analyzer. 
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_SetStopFrequency(unsigned short in_usInstanceId, double in_dStopFrequency, unsigned short in_usChannelNo)
{

    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, "SENS:FREQ:STOP %lf\n", in_dStopFrequency);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else
    {
        sprintf(m_szCmdString, "SENS%d:FREQ:STOP %lf\n", in_usChannelNo, in_dStopFrequency);

        /* Setting the stop frequency */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SET_STOP_FREQ_FAILURE;
    }

    U8BIT ucOPCCnt = 0;
    int iOper = 0;
    while(!iOper)
    {
        /*wait for operation to complete*/

        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (char*)"*OPC?\n", "%d", &iOper);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetDisplayFormat
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the display format for the measurement. 
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetDisplayFormat(unsigned short in_usInstanceId, unsigned short in_usFormat, unsigned short in_usChannelNo)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        char szMeasureFormat[DPZNL_WRP_TOTAL_MEASURES_FUTN][DPZNL_WRP_FIVE] = {"MLIN", "MLOG", "PHAS", "UPH", "IMAG", "REAL", "POL", "SMIT", "SADM", "SWR", "GDEL"};
        /*MLINear, MLOGarithmic, PHASe,UPHase 'Unwrapped phase,
            IMAGinary,REAL,POLar,SMITh,SADMittance 'Smith Admittance,SWR,GDELay 'Group Delay*/
        sprintf(m_szCmdString, "CALC%d:FORM %s\n", in_usChannelNo,szMeasureFormat[in_usFormat]);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else{
        //sprintf(m_szCmdString, "CALC:PAR:MNUM 1\n");//Not Available command

        /* Setting the stop frequency */
      //  status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        /*MLINear | MLOGarithmic | PHASe | UPHase | POLar | SMITh |ISMith | GDELay | REAL
            | IMAGinary | SWR | COMPlex |MAGNitude | LOGarithmic*/

        memset(m_szCmdString, 0, sizeof(m_szCmdString));
        char szMeasureFormat[DPZNL_WRP_TOTAL_MEASURES_FUTN][DPZNL_WRP_FIVE] = {"MLIN", "MLOG", "PHAS", "UPH", "IMAG", "REAL", "POL", "SMIT", "ISM", "SWR", "GDEL", "COMP", "MAGN", "LOG"};

        //sprintf(m_szCmdString, "CALC%d:FORM %s\n", in_usChannelNo,szMeasureFormat[in_usFormat]);
        sprintf(m_szCmdString, "CALC1:MEAS%d:FORM %s\n", in_usChannelNo,  szMeasureFormat[in_usFormat]);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SET_DISP_FAILURE;
    }

    U8BIT ucOPCCnt = 0;
    int iOper = 0;
    while(!iOper)
    {
        /*wait for operation to complete*/

        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (char*)"*OPC?\n", "%d", &iOper);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }
    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_Preset
// Author					: - 
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the RF power output level. 
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_Preset(unsigned short in_usInstanceId)
{

    sprintf(m_szCmdString, "*RST\n");

#ifndef	ZNLNETWORKANALYZER_WRP_SIMULATE
    /* Setting the input power level */
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_RST_FAILURE;
    }
#endif
    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetPowerLevel
// Author					: - 
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the RF power output level. 
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_SetPowerLevel(unsigned short in_usInstanceId, double in_dInputPower, unsigned short in_usChannelNo)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "SOUR%d:POW %lf\n", in_usChannelNo, in_dInputPower);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else{
        sprintf(m_szCmdString, "SOUR%d:POW %lf\n", in_usChannelNo, in_dInputPower);

        /* Setting the input power level */
        //status = viPrintf(m_viAgNaHandle[in_usInstanceId], m_szCmdString);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_PWR_LVL_FAILURE;
    }

    U8BIT ucOPCCnt = 0;
    int iOper = 0;
    while(!iOper)
    {
        /*wait for operation to complete*/

        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (char*)"*OPC?\n", "%d", &iOper);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetPowerON
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the RF power output level. 
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetPowerON(unsigned short in_usInstanceId)
{	
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, ":OUTP ON\n");
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        sprintf(m_szCmdString, ":OUTP ON\n");
        /* Setting the input power level */
        //status = viPrintf(m_viAgNaHandle[in_usInstanceId], m_szCmdString);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPPWR_LVL_FAILURE;
        }
    }
    else{
        sprintf(m_szCmdString, ":OUTP ON\n");
        /* Setting the input power level */

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_OPPWR_LVL_FAILURE;
    }

    U8BIT ucOPCCnt = 0;
    int iOper = 0;
    while(!iOper)
    {
        /*wait for operation to complete*/

        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (char*)"*OPC?\n", "%d", &iOper);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }
    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetPowerOFF
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the RF power output level. 
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_SetPowerOFF(unsigned short in_usInstanceId)
{    
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "OUTP OFF\n");
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {

        sprintf(m_szCmdString, ":OUTP OFF\n");

        /* Setting the input power level */
        //status = viPrintf(m_viAgNaHandle[in_usInstanceId],m_szCmdString);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_PWROFF_FAILURE;
        }
    }
    else{
        sprintf(m_szCmdString, ":OUTP OFF\n");

        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession,m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_PWROFF_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_CreateNewTrace
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To create the new trace.
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_CreateNewTrace(unsigned short in_usInstanceId, unsigned short in_usChannelNo, unsigned short in_usTraceNo )//Not Available command
{
    unsigned short usTrace[16] = {11,12,13,14,21,22,23,24,31,32,33,34,41,42,43,44};
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "CALC%d:PAR:DEF:EXT ""'CH%d_S%d','S%d'""\n", in_usChannelNo, in_usChannelNo,usTrace[in_usTraceNo], usTrace[in_usTraceNo]);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_NEW_TRACE_FAILURE;
        }
    }
    else
    {
        unsigned short usTrace[4] = {11,12,21,22};
        sprintf(m_szCmdString, "CALC%d:PAR:DEF:EXT  'CH%d_S%d','S%d'\n", in_usChannelNo, in_usChannelNo,usTrace[in_usTraceNo-1], usTrace[in_usTraceNo-1]);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_NEW_TRACE_FAILURE;
        }
    }

    return DP_ZNL_NA_CFG_SUCCESS;
} 

/*********************************************************************************
// Name						: ZNL_DisplayTrace
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To display the newly created trace.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_DisplayTrace(unsigned short in_usInstanceId, unsigned short in_usTraceNo, unsigned short in_usPortNo,unsigned short in_usChannelNo)
{
    /*S11 | S12 | S13 | S14 | S21 | S22 | S23 | S24 | S31 | S32 | S33 |
    S34 | S41 | S42 | S43 | S44 */
    unsigned short usTrace[16] = {11,12,13,14,21,22,23,24,31,32,33,34,41,42,43,44};
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "DISP:WIND:TRAC%d:FEED ""'CH%d_S%d'""\n", in_usChannelNo, in_usChannelNo, usTrace[in_usTraceNo]);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        unsigned short usTrace[4] = {11,12,21,22};

        //sprintf(m_szCmdString, ":DISP:WIND%d:TRAC%d:FEED 'Meas_%d'\n", in_usChannelNo, in_usTraceNo, in_usChannelNo);
        sprintf(m_szCmdString, "DISP:WIND%d:TRAC%d:FEED 'CH%d_S%d'\n", in_usChannelNo,in_usTraceNo, in_usChannelNo, usTrace[in_usTraceNo-1]);

        /* Setting the input power level */
        //status = viPrintf(m_viAgNaHandle[in_usInstanceId], m_szCmdString);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_DISP_TRACE_FAILURE;
        }
    }
    else
    {
        unsigned short usTrace[16] = {11,12,13,14,21,22,23,24,31,32,33,34,41,42,43,44};
        sprintf(m_szCmdString, "DISP:WIND2:STAT ON\n");
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_DISP_TRACE_FAILURE;
        }
        sprintf(m_szCmdString, "DISP:WIND2:TRAC%d:FEED 'Ch%d_Tr%d'\n", in_usTraceNo, in_usChannelNo, in_usTraceNo);

        /* Setting the input power level */
        //status = viPrintf(m_viAgNaHandle[in_usInstanceId], m_szCmdString);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_DISP_TRACE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_DeleteTrace
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To delete the specified trace.
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_DeleteTrace(unsigned short in_usInstanceId, unsigned short in_usTraceNo, unsigned short in_usPortNo, unsigned short in_usChannelNo)
{

    /*S11 | S12 | S13 | S14 | S21 | S22 | S23 | S24 | S31 | S32 | S33 |
    S34 | S41 | S42 | S43 | S44 */
    unsigned short usTrace[16] = {11,12,13,14,21,22,23,24,31,32,33,34,41,42,43,44};
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "CALC%d:PAR:DEL 'CH%d_S%d'\n", in_usChannelNo, in_usChannelNo, usTrace[in_usTraceNo]);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else
    {
        sprintf(m_szCmdString, "CALC%d:PAR:DEL 'Ch%d_Tr%d'\n", in_usChannelNo, in_usChannelNo,in_usTraceNo);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_DLT_TRACE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_DeleteAllTraces
// Author					: - 
// Global Variables affected: NIL
// Created Date				: 10-Jan-23
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To delete all the traces.
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_DeleteAllTraces(unsigned short in_usInstanceId)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "CALC:PAR:DEL:ALL\n");
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else
    {
        sprintf(m_szCmdString, "CALC:PAR:DEL:ALL\n");
        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_DLT_ALLTTRACE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetDataPoints
// Author					: - 
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function sets the number of data points for the measurement.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetDataPoints(unsigned short in_usInstanceId, unsigned short in_usDataPoints, unsigned short in_usChannelNo)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "SENS%d:SWE:POIN %d\n", in_usChannelNo, in_usDataPoints);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else
    {
        sprintf(m_szCmdString, "SENS%d:SWE:POIN %d\n", in_usChannelNo, in_usDataPoints);

        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SET_DATAPNT_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

short CZnlNetworkAnalyzer::DP_NA_SetIFBandWidth(unsigned short in_usInstanceId, double in_dBandWidth)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        /*To be updated fort RS*/
    }
    else
    {
        sprintf(m_szCmdString, "SENS:BWID %f\n", in_dBandWidth);
        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }

    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SET_DATAPNT_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*ADD normalize function header*/

short CZnlNetworkAnalyzer::DP_NA_SetNormalize(unsigned short in_usInstanceId)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "OUTP OFF\n");
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_PWROFF_FAILURE;
        }
    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        sprintf(m_szCmdString, "CALC:MATH:MEM\n");

        /* Copy Trace data to memory */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession,m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_PWROFF_FAILURE;
        }
    }
    else
    {
        sprintf(m_szCmdString, "CALC:MATH:MEM\n");

        /* Copy Trace data to memory */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession,m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_PWROFF_FAILURE;
        }

        sprintf(m_szCmdString, "CALC:MATH:FUNC DIV\n");

        /* Do Math Operation */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession,m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_PWROFF_FAILURE;
        }
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}


/*********************************************************************************
// Name						: ZNL_SelectTrace
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function sets the selected measurement.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SelectTrace(unsigned short in_usInstanceId, unsigned short in_usDataPoints,  unsigned short in_usTraceNo, unsigned short in_usPortNo, unsigned short in_usChannelNo)
{

    unsigned short usTrace[16] = {11,12,13,14,21,22,23,24,31,32,33,34,41,42,43,44};
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, "CALC%d:PAR:SEL CH%d_S%d_%d\n",in_usChannelNo,in_usChannelNo,usTrace[in_usTraceNo],in_usTraceNo);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        unsigned short usTrace[4] = {11,12,21,22};
//        sprintf(m_szCmdString, ":CALC%d:PAR:DEF:EXT 'Meas_%d','S%d'\n",in_usChannelNo, in_usChannelNo, usTrace[in_usTraceNo]);

        //status = viPrintf(m_viAgNaHandle[in_usInstanceId], m_szCmdString);

        sprintf(m_szCmdString, "DISP:WIND:TRAC%d:SEL\n",in_usTraceNo);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_SEL_TRACE_FAILURE;
        }
    }
    else
    {
        sprintf(m_szCmdString, "CALC%d:PAR:SEL 'Trc%d'\n",in_usChannelNo, in_usTraceNo);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_SEL_TRACE_FAILURE;
        }

        sprintf(m_szCmdString, "SENS%d:SWE:POIN %d\n",in_usChannelNo, in_usDataPoints);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SET_DATAPNT_FAILURE;
    }

    U8BIT ucOPCCnt = 0;
    int iOper = 0;
    while(!iOper)
    {
        /*wait for operation to complete*/

        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (char*)"*OPC?\n", "%d", &iOper);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }


    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_StoreData
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function puts the currently selected measurement trace into memory.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_StoreData(unsigned short in_usInstanceId, unsigned short in_usChannelNo)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, "CALC%d:MATH:MEM\n", in_usChannelNo);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else
    {
        sprintf(m_szCmdString, "CALC%d:MATH:MEM\n", in_usChannelNo);

        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_TRACE_MEM_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_DoMathOperations
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function sets math operations on the currently selected measurement 
                              and the trace stored in memory.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_DoMathOperations(unsigned short in_usInstanceId, unsigned short in_usMathOperator, unsigned short in_usChannelNo)
{


    char szMathOperator[DPZNL_WRP_FIVE][DPZNL_WRP_FIVE] = {"NORM", "ADD", "SUBT", "MULT", "DIV"};
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, "CALC%d:MATH:MEM\n", in_usChannelNo);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else
    {
        sprintf(m_szCmdString, "CALC%d:MATH:MEM\n", in_usChannelNo);

        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_MATH_TRACE_FAILURE;
    }
    sleep(1);

    sprintf(m_szCmdString, "CALC%d:MATH:FUNC %s\n", in_usChannelNo, szMathOperator[in_usMathOperator]);

    /* Setting the input power level */
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_MATH_TRACE_FAILURE;
    }

    sprintf(m_szCmdString, "CALC%d:MATH:MEM\n", in_usChannelNo);

    /* Setting the input power level */
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_MATH_TRACE_FAILURE;
    }


    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SetMarkerPosAndFreq
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function will turns the specified marker ON and sets the marker position(X-Axis Frequency).
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_SetMarkerPosAndFreq(unsigned short in_usInstanceId, unsigned short in_usMarkerNo, double in_dMarkerFreq, unsigned short in_usChannelNo)
{		
    /* Setting the specified marker ON */
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A){
        sprintf(m_szCmdString, "CALC%d:MARK%d:STAT ON\n", in_usChannelNo,in_usMarkerNo);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_ON_FAILURE;
        }

        QThread::msleep(100);

        sprintf(m_szCmdString, "CALC%d:MARK%d:X %fMHz\n", in_usChannelNo, in_usMarkerNo, in_dMarkerFreq/1e6);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        /** CALC:MEAS:MARK:REF ON
            calculate2:measure1:marker:reference:state OFF **/
        /* Setting the specified marker ON */
        //sprintf(m_szCmdString, "CALC%d:MEAS1:MARK%d:REF ON\n", in_iChannelNo,in_iMarkerNo);
        sprintf(m_szCmdString, "CALC%d:MEAS1:MARK%d:STAT ON\n", in_usChannelNo,in_usMarkerNo);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_FREQ_FAILURE;
        }

        /** CALC:MEAS:MARK:REF:X 1e9

            calculate2:measure1:marker:reference:x 1e6 **/

        sprintf(m_szCmdString, "CALC1:MEAS%d:MARK%d:X %lf\n", in_usChannelNo, in_usMarkerNo, in_dMarkerFreq);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_FREQ_FAILURE;
        }

        QThread::msleep(100);
    }
    else
    {
        sprintf(m_szCmdString, "CALC%d:MARK%d:STATE ON\n", in_usChannelNo, in_usMarkerNo);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_ON_FAILURE;
        }

        QThread::msleep(100);

        sprintf(m_szCmdString, "CALC%d:MARK%d:X %f\n", in_usChannelNo, in_usMarkerNo, in_dMarkerFreq);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_MARKER_FREQ_FAILURE;
    }

    QThread::msleep(100);

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_ReadMarkerValue
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function will read the specified marker(Y-Axis) value.
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_ReadMarkerValue(unsigned short in_usInstanceId, unsigned short in_usMarkerNo,unsigned char in_ucMeasType, unsigned short in_usChannelNo, double *Out_dMarkerValue)
{
    long lNumBytesRead = 0;
    unsigned long retCnt = 0;
    unsigned char * pucReadData = NULL;
    char* strData = NULL;


    char szMeasureFormat[DPZNL_WRP_TOTAL_MEASURES_TWFR][DPZNL_WRP_FIVE] = {"MLIN", "MLOG", "PHAS", "LOGP", "IMAG", "REAL", "POL", "SWR", "INDX", "IMP",
                                                                           "ADM", "DEF", "COMP", "MDB", "MLPH", "MDPH", "MIMP", "PADM", "PIMP", "MADM", "MPIM", "MPAD"};
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString,"CALC%d:MARK%d:FORM %s\n", in_usChannelNo, in_usMarkerNo, szMeasureFormat[in_ucMeasType] );

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_RD_MARKER_VALUE_FAILURE;
        }

        sprintf(m_szCmdString, "CALC%d:MARK%d:Y?\n", in_usChannelNo, in_usMarkerNo);

        /* Get the marker value */
        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString, "%lf", Out_dMarkerValue);
    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        /** CALC:MEAS:MARK:FORMat MLIN
            calculate2:measure:marker8:format Character **/


        sprintf(m_szCmdString, "CALC%d:MEAS:MARK%d:FORM %s\n", in_usChannelNo, in_usMarkerNo,szMeasureFormat[in_ucMeasType] );

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_RD_MARKER_VALUE_FAILURE;
        }
        /** CALC:MEAS:MARK:Y?
            calculate2:measure1:marker3:y? **/

        sprintf(m_szCmdString, "CALC%d:MEAS:MARK%d:Y?\n", in_usChannelNo,in_usMarkerNo);

        /* Get the marker value */
        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString, "%lf", Out_dMarkerValue);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_RD_MARKER_VALUE_FAILURE;
        }
    }
    else
    {
        sprintf(m_szCmdString,"CALC%d:MARK%d:FORM %s\n", in_usChannelNo, in_usMarkerNo, szMeasureFormat[in_ucMeasType] );

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_RD_MARKER_VALUE_FAILURE;
        }

        sprintf(m_szCmdString, "CALC%d:MARK%d:Y?\n", in_usChannelNo, in_usMarkerNo);

        /* Get the marker value */
        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString, "%lf", Out_dMarkerValue);
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_SelfTest
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function will read the specified marker(Y-Axis) value.
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_SelfTest(unsigned short in_usInstanceId,short *out_sStatus)
{	
    ViChar strMsg[20] = {0};

#ifndef	ZNLNETWORKANALYZER_WRP_SIMULATE
    status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, "*TST?\n", "%s", strMsg);
    if(status)
    {
        //ZNL_error_message(in_usInstanceId,status, strMsg);
        return DP_ZNL_NA_SELF_TST_FAILURE;
    }

    *out_sStatus = atoi(strMsg);
    *out_sStatus |= status;
#endif
    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_UserPreset
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the RF power output level. 
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_UserPreset(unsigned short in_usInstanceId)//Not Available command
{

    sprintf(m_szCmdString, ":SYST:PRES\n"); //SYST:PRES

    /* Setting the input power level */
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_PRESET_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_Calibration
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the RF power output level. 
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_Calibration(unsigned short in_usInstanceId, unsigned char in_ucPortNo, unsigned char in_ucCalTypeSel)
{ 

    int iOper = 0;
    unsigned char ucOPCCnt = 0;
    unsigned long lNumBytesRead = 0;

    unsigned char * pucReadData = NULL;
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, "SENS:CORR:COLL:METH refl3\n");

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);

        sprintf(m_szCmdString, "SENS:CORR:COLL:METH SPARSOLT\n");

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);

        if(in_ucCalTypeSel == DPZNL_2PORT_CAL)
        {
            sprintf(m_szCmdString, "SENS:CORR:PREF:ECAL:ORI ON\n");

            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

            if(status != VI_SUCCESS)
            {
                return DP_ZNL_NA_OPER_CMPLT_FAILURE;
            }
        }
        else
        {

            sprintf(m_szCmdString, "SENS:CORR:PREF:ECAL:ORI OFF\n");

            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

            if(status != VI_SUCCESS)
            {
                return DP_ZNL_NA_OPER_CMPLT_FAILURE;
            }

            sprintf(m_szCmdString, "SENS:CORR:PREF:ECAL:PMAP ECAL1,\'a1\'\n");

            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

            if(status != VI_SUCCESS)
            {
                return DP_ZNL_NA_OPER_CMPLT_FAILURE;
            }

        }

        QThread::msleep(100);

        //Measure in forward Direction
        sprintf(m_szCmdString, "SENS:CORR:COLL:ACQ ECAL1\n");

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(5000);

        while(!iOper)
        {
            /*wait for operation to complete*/

            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, "*OPC?\n");

            if(status != VI_SUCCESS)
            {
                return DP_ZNL_NA_OPER_CMPLT_FAILURE;
            }

            QThread::msleep(100);


            pucReadData = (unsigned char *)malloc(sizeof(unsigned char ) * 2048);
            //status = viRead(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, pucReadData, 2048, &lNumBytesRead);
            char *strData	       = NULL;
            sprintf(strData,"%s", pucReadData);

            iOper = atoi(strData);

            if(ucOPCCnt > 100)
                break;

            free(pucReadData);

            ucOPCCnt++;
        }

        QThread::msleep(100);

        /* Changed by Subeesh*/
        sprintf(m_szCmdString, ":OUTP OFF\n");

        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession,m_szCmdString);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_OPER_CMPLT_FAILURE;
        }

        QThread::msleep(100);
    }
    else
    {
        //sprintf(m_szCmdString, "SENS:CORR:COLL:METH refl3\n");
        sprintf(m_szCmdString, "SENS:CORR:COLL:METH FOPort1\n");

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_PORT_CAL_FAILURE;
        }

        QThread::msleep(100);

        sprintf(m_szCmdString, "SENS:CORR:COLL:METH RSHort\n");

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_PORT_CAL_FAILURE;
        }

        QThread::msleep(100);

        if(in_ucCalTypeSel == DPZNL_2PORT_CAL)
        {
            sprintf(m_szCmdString, "SENS:CORR:PREF:ECAL:ORI ON\n");

            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

            if(status != VI_SUCCESS)
            {
                return status;
            }
        }
        else
        {

            sprintf(m_szCmdString, "SENS:CORR:PREF:ECAL:ORI OFF\n");

            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

            if(status != VI_SUCCESS)
            {
                return status;
            }

            sprintf(m_szCmdString, "SENS:CORR:PREF:ECAL:PMAP ECAL1,\'a1\'\n");

            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

            if(status != VI_SUCCESS)
            {
                return status;
            }

        }

        QThread::msleep(100);

        //Measure in forward Direction
        sprintf(m_szCmdString, "SENS:CORR:COLL:ACQ ECAL1\n");

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return status;
        }

        sleep(5);

        while(!iOper)
        {
            /*wait for operation to complete*/
            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, "*OPC?\n");

            if(status != VI_SUCCESS)
            {
                return DP_ZNL_NA_OPER_CMPLT_FAILURE;
            }

            QThread::msleep(100);

            pucReadData = (unsigned char *)malloc(sizeof(unsigned char ) * 2048);
            //status = viRead(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, pucReadData, 2048, &lNumBytesRead);
            char *strData = NULL;
            sprintf(strData,"%s", pucReadData);

            iOper = atoi(strData);

            if(ucOPCCnt > 100)
                break;

            free(pucReadData);

            ucOPCCnt++;
        }

        QThread::msleep(100);
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_LoadStateFromFile
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To set the RF power output level. 
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_LoadStateFromFile(unsigned short in_usInstanceId, char* strFileName)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, ":MMEM:LOAD '%s.csa'\n", strFileName);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else{
		sprintf(m_szCmdString, "MMEM:LOAD '%s.csa'\n", strFileName);
       // sprintf(m_szCmdString, "MMEM:LOAD:STAT 1,'%s.znxml'\n", strFileName);
        //sprintf(m_szCmdString, "MMEM:LOAD:CKIT", strFileName);

        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_LOAD_FILE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_NumberOfTrace
// Author					: - 
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function puts the currently selected measurement trace into memory.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_NumberOfTrace(unsigned short in_usInstanceId, unsigned short in_usTraceNo)
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, "CALC:PAR:COUN %d\n", in_usTraceNo);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else{
        sprintf(m_szCmdString, "CALC:PAR:COUN %d\n", in_usTraceNo);

        /* Setting the input power level */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_NOF_TRACE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_StoreStateIntoFile
// Author					: - 
// Global Variables affected: NIL
// Created Date				: 19-Jan-2012 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To Store the state with calibration in a state file 
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_StoreStateIntoFile(unsigned short in_usInstanceId,char* in_csFileName)
{  
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, "MMEM:STOR:CSAR ""'%s.csa'""\n", in_csFileName);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    else{
        //Store  the data as
        //sprintf(m_szCmdString, "MMEM:STOR:CSAR ""'%s.csa'""\n", in_csFileName);
       // sprintf(m_szCmdString, "MMEM:LOAD:CKIT", in_csFileName);
	   sprintf(m_szCmdString, "MMEM:STOR '%s.csa'\n", in_csFileName);

        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_LOAD_FILE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

/*********************************************************************************
// Name						: ZNL_Trigger
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23  	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To Set the mode for Trigger
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_Trigger(unsigned short in_usInstanceId,unsigned char in_ucMode)	//Not Available command
{
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        if(in_ucMode)
        {
            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, "SENS1:SWE:MODE HOLD\n");
        }
        else
        {
            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, "SENS1:SWE:MODE CONT\n\n");
        }

    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        /** SENS:SWE:MODE CONT
            sense2:sweep:mode hold **/

        if(in_ucMode == DPZNL_CONTINUOUS)
        {
            sprintf(m_szCmdString, "SENS1:SWE:MODE CONT\n");

            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        }
        else if(in_ucMode == DPZNL_HOLD)
        {
            sprintf(m_szCmdString, "SENS1:SWE:MODE HOLD\n");
            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        }
        else
        {
            sprintf(m_szCmdString, "SENS1:SWE:MODE SINGLE\n");
            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        }

        if(status != VI_SUCCESS)
        {
            return status;
        }
    }
    else{
        if(in_ucMode)
        {
            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (char*)"SENS1:SWE:MODE HOLD\n");
        }
        else
        {
            status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (char*)"SENS1:SWE:MODE CONT\n\n");
        }
    }
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SWE_MODE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}


/*********************************************************************************
// Name						: ZNL_SetMarkerSearchType
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function will set marker search type.
*********************************************************************************/

short CZnlNetworkAnalyzer::DP_NA_SetMarkerSearchType(unsigned short in_usInstanceId,unsigned short in_usChannelNo, unsigned short in_usMarkerNo,  unsigned short in_usSearchType, double *Out_dMarkerValue, double *Out_dMarkerFreq)
{
    unsigned long lNumBytesRead = 0;
    unsigned char * pucReadData = NULL;
    char* strTemp =  NULL;
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == RS_SMB200A)
    {
        sprintf(m_szCmdString, "CALC%d:MARK%d ON\n", in_usChannelNo,in_usMarkerNo);
        /* Select the Marker */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_TYPE_FAILURE;
        }

        QThread::msleep(100);

        switch(in_usSearchType)
        {
        case DPZNL_MARKER_SEARCH_MAX:
            sprintf(m_szCmdString, "CALC%d:MARK%d:FUNC:EXEC MAX\n", in_usChannelNo,in_usMarkerNo);
            break;
        case DPZNL_MARKER_SEARCH_MIN:
            sprintf(m_szCmdString, "CALC%d:MARK%d:FUNC:EXEC MIN\n", in_usChannelNo,in_usMarkerNo);
            break;
        case DPZNL_MARKER_SEARCH_NPEAK:
            sprintf(m_szCmdString, "CALC%d:MARK%d:FUNC:EXEC NPE\n", in_usChannelNo,in_usMarkerNo);
            break;
        }

        /* Setting the Marker search type */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_TYPE_FAILURE;
        }

        QThread::msleep(100);

        /* Read the specified marker freq */
        sprintf(m_szCmdString, "CALC%d:MEAS%d:MARK%d:X?\n", in_usChannelNo, (in_usChannelNo + 1), in_usMarkerNo);
        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString, "%lf", Out_dMarkerFreq);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_READ_MARKER_FAILURE;
        }

        QThread::msleep(100);

        sprintf(m_szCmdString, "CALC%d:MEAS%d:MARK%d:Y?\n", in_usChannelNo, (in_usChannelNo + 1), in_usMarkerNo);
        /* Get the marker value */
        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString, "%lf", Out_dMarkerValue);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_VALUE_FAILURE;
        }
    }
    else
    {
        sprintf(m_szCmdString, "CALC%d:MEAS%d:MARK%d ON\n", 1, (in_usChannelNo), in_usMarkerNo);
        /* Select the Marker */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_TYPE_FAILURE;
        }

        QThread::msleep(100);


        switch(in_usSearchType)
        {
        case DPZNL_MARKER_SEARCH_MAX:
            sprintf(m_szCmdString, "CALC%d:MEAS%d:MARK%d:FUNC:EXEC MAX\n", 1, (in_usChannelNo), in_usMarkerNo);
            break;
        case DPZNL_MARKER_SEARCH_MIN:
            sprintf(m_szCmdString, "CALC%d:MEAS%d:MARK%d:FUNC:EXEC MIN\n", 1,(in_usChannelNo), in_usMarkerNo);
            break;
        case DPZNL_MARKER_SEARCH_NPEAK:
            sprintf(m_szCmdString, "CALC%d:MEAS%d:MARK%d:FUNC:EXEC NPE\n", 1,(in_usChannelNo),in_usMarkerNo);
            break;
        }

        /* Setting the Marker search type */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_TYPE_FAILURE;
        }

        QThread::msleep(100);

        /* Read the specified marker freq */
        sprintf(m_szCmdString, "CALC%d:MEAS%d:MARK%d:X?\n", 1, (in_usChannelNo), in_usMarkerNo);
        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString, "%lf", Out_dMarkerFreq);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_READ_MARKER_FAILURE;
        }

        QThread::msleep(100);

        sprintf(m_szCmdString, "CALC%d:MEAS%d:MARK%d:Y?\n", 1, (in_usChannelNo), in_usMarkerNo);
        /* Get the marker value */
        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString, "%lf", Out_dMarkerValue);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_MARKER_VALUE_FAILURE;
        }
    }

//    sprintf(m_szCmdString, "CALC%d:MARK%d:FUNC:EXEC\n", in_usChannelNo,in_usMarkerNo);
//    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
//    if(status != VI_SUCCESS)
//    {
//        return DP_ZNL_NA_MARKER_TYPE_FAILURE;
//    }


    return DP_ZNL_NA_CFG_SUCCESS;
}


/*********************************************************************************
// Name						: ZNL_SetSmoothing
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function will set the Smoothing.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetSmoothing(unsigned short in_usInstanceId, unsigned short in_usEnableDisable, unsigned short in_usPoints, float in_fAperture)
{


    if(in_usEnableDisable == 1)
    {
        sprintf(m_szCmdString, "CALC:SMO ON\n");

    }
    else if(in_usEnableDisable == 0)
    {
        sprintf(m_szCmdString, "CALC:SMO OFF\n");

    }
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

    if(status != VI_SUCCESS)
    {
        return DP_ZNL_WR_SMO_FAILURE;
    }
    QThread::msleep(100);

    sprintf(m_szCmdString, "CALC1:SMO:POIN %d\n", in_usPoints);
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

    if(status != VI_SUCCESS)
    {
        return status;
    }

    QThread::msleep(100);

    sprintf(m_szCmdString, "CALC1:SMO:APER %f\n", in_fAperture);

    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);

    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_APER_FAILURE;
    }


    return DP_ZNL_NA_CFG_SUCCESS;
}


/*********************************************************************************
// Name						: ZNL_SetSweepMode
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function will set the Sweep Mode.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetSweepMode(unsigned short in_usInstanceId, unsigned short in_usChannelNo, unsigned char in_ucMode)//Not Available command
{

    switch(in_ucMode)
    {
    case DPZNL_HOLD:
        sprintf(m_szCmdString, "SENS%d:SWE:MODE HOLD\n",in_usChannelNo);
        break;
    case DPZNL_CONTINUOUS:
        sprintf(m_szCmdString, "SENS%d:SWE:MODE CONT\n",in_usChannelNo);
        break;
    case DPZNL_SINGLE:
        sprintf(m_szCmdString, "SENS%d:SWE:MODE SING\n",in_usChannelNo);
        break;
    }

    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SWE_MODE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}
/********************** End of Network Analyzer functions **********************/


/*********************************************************************************
// Name						: ZNL_SetAveraging
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: This function will set the Smoothing.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetAveraging(unsigned short in_usInstanceId, unsigned short in_usEnableDisable, unsigned char  in_ucAvgMode, unsigned short in_usAvgPoints)
{
    char szAvgMode[4][15] = {"AUTO", "FLAT", "RED","MOV"};

    sprintf(m_szCmdString, "SENS1:AVER %s\n", in_usEnableDisable?"ON":"OFF");
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_AVG_FAILURE;
    }

    sprintf(m_szCmdString, "SENSE:AVER:MODE %s\n", szAvgMode[in_ucAvgMode]);
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_AVG_MODE_FAILURE;
    }

    sprintf(m_szCmdString, "SENS:AVER:COUN %d\n", in_usAvgPoints);
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_AVG_COUNT_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

//Function to change the Scale
/*********************************************************************************
// Name						: ZNL_SetScale
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23 	
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: Function to change the Scale.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_SetScale(unsigned short in_usInstanceId, unsigned char  in_ucTrace, double in_dScale)
{

    sprintf(m_szCmdString, ":DISP:WIND1:TRACE%d:Y:SCALE:PDIV %f\n",in_ucTrace, in_dScale);
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    if(status != VI_SUCCESS)
    {
        return DP_ZNL_NA_SCALE_FAILURE;
    }

    return DP_ZNL_NA_CFG_SUCCESS;

}
//Function to change the Scale
/*********************************************************************************
// Name						: ZNL_SetIFBandWidth
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: Function to change the Scale.
*********************************************************************************/
//short CZnlNetworkAnalyzer::DP_NA_SetIFBandWidth(unsigned short in_usInstanceId,double in_dIFbwId)
//{

//    sprintf(m_szCmdString, "SENS:BWID %f\n",in_dIFbwId);
//    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
//    if(status != VI_SUCCESS)
//    {
//        return DP_ZNL_NA_SCALE_FAILURE;
//    }

//    return DP_ZNL_NA_CFG_SUCCESS;

//}

//Function to change the Scale
/*********************************************************************************
// Name						: ZNL_SetScale
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: Function to Read the Trace Data.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_ReadTraceData(unsigned short in_usInstanceId, double in_dStartFreq, double in_dStopFreq, unsigned int in_uiNoofPoints, PSDPAGNA_WRP_TRACE_DATA out_pSTraceData)
{
    unsigned int uiReadBytesCnt = 0;
    unsigned int uiDataIdx = 0;
    char *pszTraceData = NULL;
    char *pszPhaseTraceData = NULL;
    char *pszPhase = NULL;
    char *pszMag = NULL;
    double dStepSize = 0.0;
    S16BIT s16RetVal = 0;

    QString strTemp;

    if(out_pSTraceData == NULL)
    {
        return  DP_ZNL_NA_SCALE_FAILURE;
    }

    dStepSize = (in_dStopFreq - in_dStartFreq)/(in_uiNoofPoints - 1);

    pszTraceData = (char *) malloc(in_uiNoofPoints * 20);

//    s16RetVal = DP_NA_SetDisplayFormat(in_usInstanceId, 1, 1);//LMAG
//    if(s16RetVal)
//    {
//        free(pszTraceData);
//        return s16RetVal;
//    }

    QThread::msleep(2000);
    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == DP_NA_N5241A)
    {
        sprintf(m_szCmdString, "CALC1:DATA? FDATA\n");
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            free(pszTraceData);
            return DP_ZNL_NA_SCALE_FAILURE;
        }
    }
    else if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {

        sprintf(m_szCmdString, "CALC1:MEAS%d:DATA:FDAT?\n", 1/*in_ucTraceNo*/);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            free(pszTraceData);
            return DP_ZNL_NA_SCALE_FAILURE;
        }
    }
    else
    {
        sprintf(m_szCmdString, "CALC1:DATA? FDAT\n");
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            free(pszTraceData);
            return DP_ZNL_NA_SCALE_FAILURE;
        }
    }

    status = viRead(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (ViPBuf)pszTraceData, (ViUInt32)(in_uiNoofPoints * 20), (ViPUInt32)&uiReadBytesCnt);
    if(status != VI_SUCCESS)
    {
        free(pszTraceData);
        return DP_ZNL_NA_SCALE_FAILURE;
    }

    pszMag = strtok(pszTraceData, ",");

    uiDataIdx = 0;
    while(pszMag != NULL)
    {
        if(uiDataIdx == in_uiNoofPoints)
        {
            break;
        }

        out_pSTraceData[uiDataIdx].m_dMag = atof(pszMag);
        out_pSTraceData[uiDataIdx].m_dFreq = in_dStartFreq + (uiDataIdx * dStepSize);

        pszMag = strtok(NULL, ",");
        uiDataIdx++;
    }

    if(pszMag != NULL)
    {
        free(pszMag);
    }
    free(pszTraceData);

#ifdef _PHASE_TRACE_DATA
    usleep(10000);

    pszPhaseTraceData = (char *) malloc(in_uiNoofPoints * 20);

    s16RetVal = DP_NA_SetDisplayFormat(in_usInstanceId, 2, 1);//phase
    if(s16RetVal)
    {
        return s16RetVal;
    }

    usleep(10000);

    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {

        sprintf(m_szCmdString, "CALC1:MEAS%d:DATA:FDAT?\n", in_ucTraceNo);
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            free(pszTraceData);
            return DP_ZNL_NA_SCALE_FAILURE;
        }
    }
    else
    {
        sprintf(m_szCmdString, "CALC1:DATA? FDAT\n");
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            free(pszPhaseTraceData);
            return DP_ZNL_NA_SCALE_FAILURE;
        }
    }

    for(U8BIT u8Retry = 0; u8Retry < 3; u8Retry++)
    {
        status = viRead(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (ViPBuf)pszPhaseTraceData, (ViUInt32)(in_uiNoofPoints * 20), (ViPUInt32)&uiReadBytesCnt);
        if(status != VI_SUCCESS)
        {
            usleep(1000);
            continue;
        }
        else
        {
            break;
        }
    }
    if(status != VI_SUCCESS)
    {
        free(pszPhaseTraceData);
        return DP_ZNL_NA_SCALE_FAILURE;
    }

    pszPhase = strtok(pszPhaseTraceData, ",");

    uiDataIdx = 0;
    while(pszPhase != NULL)
    {
        if(uiDataIdx == in_uiNoofPoints)
        {
            break;
        }

        out_pSTraceData[uiDataIdx].m_dFreq = in_dStartFreq + (uiDataIdx * dStepSize);
        out_pSTraceData[uiDataIdx].m_dPhase = atof(pszPhase);
        pszPhase = strtok(NULL, " ,");
        uiDataIdx++;
    }

    free(pszPhaseTraceData);
#endif


    return DP_ZNL_NA_CFG_SUCCESS;
}
//Function to change the Scale
/*********************************************************************************
// Name						: ZNL_SetScale
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: Function to Read the Trace Data.
*********************************************************************************/
short CZnlNetworkAnalyzer::DP_NA_ReadPhaseTraceData(unsigned short in_usInstanceId, double in_dStartFreq, double in_dStopFreq, unsigned int in_uiNoofPoints, PSDPAGNA_WRP_PHASETRACE_DATA out_pSPhaseTraceData)
{
    unsigned int uiReadBytesCnt = 0;
    unsigned int uiDataIdx = 0;
    char *pszTraceData = NULL;
    char *pszPhase = NULL;
    char *pszMag = NULL;
    double dStepSize = 0.0;

    QString strTemp;

    if(out_pSPhaseTraceData == NULL)
    {
        return  DP_ZNL_NA_SCALE_FAILURE;
    }

    dStepSize = (in_dStopFreq - in_dStartFreq)/(in_uiNoofPoints - 1);

    pszTraceData = (char *) malloc(in_uiNoofPoints * 20);

    DP_NA_SetDisplayFormat(in_usInstanceId, 2, 1);

    usleep(10000);
    sprintf(m_szCmdString, "CALC1:DATA? FDAT\n");
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    if(status != VI_SUCCESS)
    {
		free(pszTraceData);
        return DP_ZNL_NA_SCALE_FAILURE;
    }

    status = viRead(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (ViPBuf)pszTraceData, (ViUInt32)(in_uiNoofPoints * 20), (ViPUInt32)&uiReadBytesCnt);
    if(status != VI_SUCCESS)
    {
        free(pszTraceData);
        return DP_ZNL_NA_SCALE_FAILURE;
    }

    pszPhase = strtok(pszTraceData, ",");

    uiDataIdx = 0;
    while(pszPhase != NULL)
    {
        if(uiDataIdx == in_uiNoofPoints)
        {
            break;
        }

        out_pSPhaseTraceData[uiDataIdx].m_dFreq = in_dStartFreq + (uiDataIdx * dStepSize);
        out_pSPhaseTraceData[uiDataIdx].m_dPhase = atof(pszPhase);
        pszPhase = strtok(NULL, " ,");
        uiDataIdx++;
    }

    free(pszTraceData);

    return DP_ZNL_NA_CFG_SUCCESS;
}
short CZnlNetworkAnalyzer::DP_NA_ReadAttnTraceData(unsigned short in_usInstanceId, double in_dStartFreq, double in_dStopFreq, unsigned int in_uiNoofPoints, PSDPAGNA_WRP_ATTNTRACE_DATA out_pSAttnTraceData)
{
    unsigned int uiReadBytesCnt = 0;
    unsigned int uiDataIdx = 0;
    char *pszTraceData = NULL;
    char *pszPhase = NULL;
    char *pszMag = NULL;
    double dStepSize = 0.0;

    QString strTemp;

    if(out_pSAttnTraceData == NULL)
    {
        return  DP_ZNL_NA_SCALE_FAILURE;
    }

    dStepSize = (in_dStopFreq - in_dStartFreq)/(in_uiNoofPoints - 1);

    pszTraceData = (char *) malloc(in_uiNoofPoints * 20);

    DP_NA_SetDisplayFormat(in_usInstanceId, 1, 1);

    usleep(10000);
    sprintf(m_szCmdString, "CALC1:DATA? FDAT\n");
    status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
    if(status != VI_SUCCESS)
    {
        free(pszTraceData);
		return DP_ZNL_NA_SCALE_FAILURE;
    }

    status = viRead(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, (ViPBuf)pszTraceData, (ViUInt32)(in_uiNoofPoints * 20), (ViPUInt32)&uiReadBytesCnt);
    if(status != VI_SUCCESS)
    {
        free(pszTraceData);
		return DP_ZNL_NA_SCALE_FAILURE;
    }

    pszMag = strtok(pszTraceData, ",");

    uiDataIdx = 0;
    while(pszMag != NULL)
    {
        if(uiDataIdx == in_uiNoofPoints)
        {
            break;
        }

        out_pSAttnTraceData[uiDataIdx].m_dFreq = in_dStartFreq + (uiDataIdx * dStepSize);
        out_pSAttnTraceData[uiDataIdx].m_dMag = atof(pszMag);
        pszMag = strtok(NULL, " ,");
        uiDataIdx++;
    }
    free(pszTraceData);

    return DP_ZNL_NA_CFG_SUCCESS;
}

short CZnlNetworkAnalyzer::GetInstID(char *in_pcInstDetails, unsigned short *out_pusModel)
{

    unsigned char ucIdx = 0;
    bool bCmpRtn = 0;
    char *pszTok = NULL;
    char *pszModel = NULL;

    pszTok = strtok(in_pcInstDetails, ",");
    pszModel = strtok(NULL, " ,");

    for(ucIdx = 0; ucIdx < g_ucNACount;  ucIdx++)
    {
        bCmpRtn = strcasecmp(pszModel, m_InstCfg[ucIdx].m_szModel);
        if(bCmpRtn == 0)
        {
            *out_pusModel = m_InstCfg[ucIdx].m_usInstruementID;
            return DP_ZNL_NA_CFG_SUCCESS;
        }
    }

    return DP_ZNL_NA_INT_INVALID_MODEL;
}

short CZnlNetworkAnalyzer::DP_NA_ReadSweepTime(unsigned short in_usInstanceId, double *Out_dSweepTime)
{

    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        sprintf(m_szCmdString, "SENSe:SWEep:TIME?\n"); /*Set -- SENS:SWE:TIME 0.1*/

        /* Get the marker value */
        status = viQueryf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString, "%lf", Out_dSweepTime);
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}

short CZnlNetworkAnalyzer::DP_NA_SetSweepTime(unsigned short in_usInstanceId, double in_dSweepTime)
{

    if(m_arrSNAInstHandle[in_usInstanceId-1].m_usInstID == KY_E5080B)
    {
        sprintf(m_szCmdString, "SENS:SWE:TIME %lf\n", in_dSweepTime); /*Set -- SENS:SWE:TIME 0.1*/

        /* Get the marker value */
        status = viPrintf(m_arrSNAInstHandle[in_usInstanceId-1].m_viNASession, m_szCmdString);
        if(status != VI_SUCCESS)
        {
            return DP_ZNL_NA_SET_FREQ_FAILURE;
        }
    }

    return DP_ZNL_NA_CFG_SUCCESS;
}
