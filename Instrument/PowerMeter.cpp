#include "PowerMeter.h"
#include <stdio.h>
#include <qdebug.h>
#include <QFileInfo>
#include <QSettings>
#include <QThread>
#include <QApplication>

//#define _SIMULATION_ 1

//sPMInstCfg m_InstPMCfg[32];
//sPMInstHandle m_arrPMInstHandle[16];
extern unsigned char g_ucPMCount;

CPMWrapper::CPMWrapper()
{
    defaultPMRM = 0;
    g_viRFPM[2] = 0;
    viPM = 0;
    vistatus = 0;
    m_usTotalInstances = 0;
    m_bOpenStatus = 0;
    arrcErrStr[255] = {0};
    viRFPMSession[9] = {0};

  //  ReadPMInstrumentDetails();

    memset(m_arrPMInstHandle, 0, sizeof(m_arrPMInstHandle));

}
/**************************************************************************
// Name						:	 Open()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    26-03-09
// Revision Date			:
// Reason for Revising		:
// Description				:	To DP_PMeter_Open RF signal gen.
**************************************************************************/
short CPMWrapper::Open(U8BIT in_ucPMNo, PS8BIT pLogicalName)
{
    ViSession defaultPMRM = 0;
    ViSession viPMSession = 0;
    U16BIT usInstID = 0;
    unsigned int uiReadCnt = 0;
    char arripadress[100]={0};
    char szInstDetails [256] = "";

    try
    {
        //sprintf(arripadress, "ASRL%s::INSTR", pLogicalName);

        vistatus =  viOpenDefaultRM(&defaultPMRM);
        if(vistatus != 0)
        {
            return DP_VI_PM_OPEN_FAILURE;
        }

        vistatus = viOpen(defaultPMRM, pLogicalName, VI_NULL, VI_NULL, &viPMSession);
        if(vistatus != 0)
        {
            vistatus = viOpenDefaultRM(&defaultPMRM);
            if(vistatus != 0)
            {
                return DP_VI_PM_OPEN_FAILURE;
            }
            //sprintf(arripadress, "TCPIP0::%s::inst0::INSTR", pLogicalName);
            vistatus = viOpen(defaultPMRM, pLogicalName, VI_NULL, VI_NULL, &viPMSession );
            if(vistatus)
            {
                return DP_VI_PM_OPEN_FAILURE;
            }
        }

        /*Read Instrument Type*/
        vistatus = viPrintf(viPMSession, (char*)"*IDN?\n");
        if(vistatus)
        {
            return DP_PMETER_INT_ID_READ_FAILED;
        }

        vistatus = viBufRead(viPMSession, (ViPBuf)szInstDetails, 256,(ViPUInt32)&uiReadCnt);
        if(vistatus)
        {
            return DP_PMETER_INT_ID_READ_FAILED;
        }

        /*Read the Model ID*/
       vistatus = GetInstID(szInstDetails, &usInstID);

       if(vistatus != 0)
       {
           viClose(viPMSession);

           return DP_PMETER_INT_INVALID_MODEL;
       }
       else
       {
           m_arrPMInstHandle[in_ucPMNo-1].m_viPMSession = viPMSession;
           m_arrPMInstHandle[in_ucPMNo-1].m_usInstID = usInstID;
       }
    }

    catch(...)
    {
    }

    return DP_POWMTR_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_Preset()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function preset the power meter.
******************************************************************************************************/
S16BIT CPMWrapper::DP_PMeter_Preset(U8BIT in_ucPMNo)
{
    vistatus = viPrintf(m_arrPMInstHandle[in_ucPMNo-1].m_viPMSession, (char*)"*RST\n");
    if (vistatus)
    {
        return DP_PMETER_RESET_FAILED;
    }

    return DP_PWRMETER_SUCCESS;
}

/***************************************************************************************************
// Name						:	 Close()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function Close the power meter
******************************************************************************************************/
S16BIT CPMWrapper::Close(U8BIT in_ucPMNo)
{
#ifndef _SIMULATION_
    vistatus = viClose(m_arrPMInstHandle[in_ucPMNo - 1].m_viPMSession);
    if(vistatus != 0)
    {
        return DP_PMETER_CLS_FAILURE;
    }
#endif
    return DP_PWRMETER_SUCCESS;
}

/***************************************************************************************************
// Name						:	 GetInstID()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to get instrument id for power meter
******************************************************************************************************/
S16BIT CPMWrapper::GetInstID(PS8BIT in_pcInstDetails, U16BIT *out_pusModel)
{
    U8BIT ucIdx = 0;
    bool bCmpRtn = 0;
    char *pszTok = NULL;
    char *pszModel = NULL;

    pszTok = strtok(in_pcInstDetails, ",");
    pszModel = strtok(NULL, " ,");

    for(ucIdx = 0; ucIdx < g_ucPMCount;  ucIdx++)
    {
        bCmpRtn = strcasecmp(pszModel, m_InstCfg[ucIdx].m_PM_model);

        if (bCmpRtn == 0)
        {
            *out_pusModel = m_InstCfg[ucIdx].usInstID;
            return DP_POWMTR_SUCCESS;
        }
    }

    free(pszTok);
    pszTok = NULL;

    return DP_PMETER_INT_INVALID_MODEL;
}

/***************************************************************************************************
// Name						:	 ReadPMInstrumentDetails()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to Read the power meter instrument details
******************************************************************************************************/
S16BIT CPMWrapper::ReadPMInstrumentDetails()
{
    S8BIT szSecKey[128]		= {0};
    QString strTemp = "";
    QString strIPConfig = "";
    QString strTempPath             = "";
    QString strFilePath             = "";
    QSettings *ptrInstDetailsFile = NULL;
    U8BIT ucPMCount = 0;

    strTempPath = qApp->applicationDirPath();
    strFilePath = strTempPath + "/Input_Files/RFInstDetails.ini";

    QFileInfo objfileInfo(strFilePath);
    if (objfileInfo.exists() && objfileInfo.isFile())
    {
        ptrInstDetailsFile = new(QSettings)(strFilePath, QSettings::IniFormat);
    }
    else
    {
        m_bInitErrFlag = 1;
        return -1;
    }

    sprintf(szSecKey , "INST_CONFIG/TOT_PM_COUNT");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return -1;
    }

    ucPMCount = strTemp.toUInt(0,10);

    for(U8BIT u8Idx = 0; u8Idx < ucPMCount; u8Idx++)
    {
        sprintf(szSecKey, "INST_CONFIG/PM_%d_MANUFACTURER", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            m_bInitErrFlag = 1;
            return -1;
        }

        strcpy(m_InstCfg[u8Idx].m_PM_Manufacturer, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/PM_%d_MODEL", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            m_bInitErrFlag = 1;
            return -1;
        }

        strcpy(m_InstCfg[u8Idx].m_PM_model, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/PM_%d_ID", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            m_bInitErrFlag = 1;
            return -1;
        }

        m_InstCfg[u8Idx].usInstID = strTemp.toUInt(0,10);
    }

    m_bInitErrFlag = 0;
    return DP_POWMTR_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_PMeter_GetErrorMessage()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to get the power meter error message
******************************************************************************************************/
S16BIT CPMWrapper::DP_PMeter_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize)
{
    if(out_ps8ErrStringBuf == NULL)
        return  DP_ERR_PM_INVALID_PARAM;

    switch(in_s32ErrCode)
    {
    case DP_VI_PM_OPEN_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize, "ViOpen Failed");
        break;
    case DP_PMETER_INT_INVALID_MODEL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Power Meter Invalid Model!");
        break;
    case DP_PMETER_INT_ID_READ_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Power Meter Model Number Read Failure!");
        break;
    case DP_PMETER_RESET_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Power Meter Model Number Read Failure!");
        break;
    case DP_PMETER_CLS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"ViClose Failed!!");
        break;
    case DP_PMETER_FREE_RUN_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Continuous measurement mode Failed");
        break;
    case DP_PMETER_GET_MARKER_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Getting Marker Power Failed");
        break;
    case DP_PMETER_READ_POWER_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Peak Mode Setting Failed");
        break;
    case DP_PMETER_CNFG_FREQUENCY_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Frequency Failed !!");
        break;
    case DP_PMETER_CNFG_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Mode Configuration Failed");
        break;
    case DP_PMETER_TRIG_CHANGE_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Trigger Mode Change Failed");
        break;
    case  DP_PMETER_TRIG_LEVEL_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Automatic Trigger Mode Setting Failed");
        break;
    case DP_PMETER_UPPER_LIMIT_CONFG_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring UpperLimit Failed");
        break;
    case DP_PMETER_TRACE_SETUP_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring UpperLimit Failed");
        break;
    case DP_PMETER_CONFG_OFFSET_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Offset Failed");
        break;
    case DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Measurement Mode Failed");
        break;
    case DP_PMETER_DISPLAY_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Display Failed");
        break;
    case DP_PMETER_CONFG_DELAY_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Delay Failed");
        break;
    case DP_PMETER_SWEEP_TIME_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Sweep Time Failed");
        break;
    case DP_PMETER_CLOSING_INSTRUMENTS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Closing All instruments Failed");
        break;
    case DP_PMETER_CONFG_RANGE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Range Failed");
        break;
    case DP_PMETER_CONFG_AVG_COUNT_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Average Count Failed");
        break;
    case DP_PMETER_CONFG_CHANNEL_ENABLE_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Channel Enable Mode Failed");
        break;
    case DP_PMETER_CONFG_DUTY_CYLCLE_CORRECT_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Duty Cycle Correction Failed");
        break;
    case DP_PMETER_CONFG_CORRECT_FREQ_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Correct Frequency Mode Failed");
        break;
    case DP_PMETER_CONFG_REF_OSCILLATOR_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Ref Oscillator Mode Failed");
        break;
    case DP_PMETER_TRIG_SOUR_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Trigger Source Mode Failed");
        break;
    case DP_PMETER_CONFG_ZERO_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Zero Configuation Mode Failed");
        break;
    case DP_PMETER_GET_CHANNEL_NAME_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Getting Channel Name Failed");
        break;
    case DP_PMETER_CONFG_AUTO_ENABLE_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Auto Enable Mode Failed");
        break;
    case DP_PMETER_GET_DURATION_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Getting Duration Mode Failed");
        break;
    case DP_PMETER_OPERATION_COMP_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Operation Completion Failed");
        break;
    case DP_PMETER_TRIG_HOLD_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Trigger Hold Mode Failed");
        break;
    case DP_PMETER_TRIG_DELETE_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Trigger Delete MOde Failed");
        break;
    case DP_PMETER_OUT_TRIG_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Output Trigger MOde Failed");
        break;
    case DP_PMETER_SELF_TEST_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Self Test MOde Failed");
        break;
    case DP_PMETER_ABORT_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Abort Mode Failed");
        break;
    case DP_PMETER_MEASUREMENT_COMPLETE_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Measurement Completion Mode Failed");
        break;
    case DP_PMETER_FETCH_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Fetch Mode Failed");
        break;
    case DP_PMETER_READ_CHANNEL_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Read Channel Mode Failed");
        break;
    case DP_PMETER_READ_PARAMETER_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Read Parameter Failed");
        break;
    case DP_PMETER_CALLIBRATE_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Auto Enable Mode Failed");
        break;
    case  DP_PMETER_MEASUREMENT_PULSE_PERIOD_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Measuring Pulse Period Mode Failed");
        break;
    case DP_PMETER_GET_TRANS_POS_DUR_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Getting Transition Positive Duration Mode Failed");
        break;
    case DP_PMETER_GET_TRANS_NEG_DUR_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Getting Transition Negative Duration Mode Failed");
        break;
    case DP_PMETER_OPER_COM_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Operation Completion Failure");
        break;
    default:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,"Invalid Error Code !");
    }

   return DP_POWMTR_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_PowerMeter_Config()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to configure the power meter
******************************************************************************************************/
ViStatus CPMWrapper::DP_PowerMeter_Config(ViSession in_uiVi, double in_dFreq, double in_dOffset,double in_dXStart,double in_dXScal,double in_dYMax,double in_dYScal,double in_dTriggerLvl,double in_dMarker1Time,double in_dMarker2Time)
{
    ViStatus viSts     = 0;
    ViChar Buffer[256] = {0};
    FDOUBLE dXScale = 0.0;

    memset(Buffer, 0, sizeof(Buffer));

    sprintf(Buffer,"TRIG:SOUR IMM\r\n");
//    qDebug() << Buffer;
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRIG_SOUR_MODE_FAILURE;
    }

    viSts = DP_PwrMtr_WaitforOperationComplete(in_uiVi);
    if(viSts)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
         sprintf(Buffer, "DISP:WIND2:SEL2\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "DISP:WIND1:SEL1\r\n");
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_CNFG_MODE_FAILURE;
    }

    viSts = DP_PwrMtr_WaitforOperationComplete(in_uiVi);
    if(viSts)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    sprintf(Buffer,"CONF1\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_CNFG_MODE_FAILURE;
    }

    viSts = DP_PwrMtr_WaitforOperationComplete(in_uiVi);
    if(viSts)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    sprintf(Buffer,"CONF2\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_CNFG_MODE_FAILURE;
    }

    viSts = DP_PwrMtr_WaitforOperationComplete(in_uiVi);
    if(viSts)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "TRIG:SOUR INT\r\n");

        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "TRIG:ALL:SOUR EXT\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }
        viSts = DP_PwrMtr_WaitforOperationComplete(in_uiVi);
        if(viSts)
        {
            return DP_PMETER_OPER_COM_FAILURE;
        }

        /*Trig Mode*/
        sprintf(Buffer, "TRIG:ALL:MODE NORM\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }

        viSts = DP_PwrMtr_WaitforOperationComplete(in_uiVi);
        if(viSts)
        {
            return DP_PMETER_OPER_COM_FAILURE;
        }


        sprintf(Buffer,"CALC:TYPE PULS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_DISPLAY_FAILURE;
        }
    }
    obj_Sleep->msleep(100);

    sprintf(Buffer, "INIT1:CONT ON\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_FREE_RUN_FAILURE;
    }
    obj_Sleep->msleep(100);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:Y:TOP:DBM %lf\r\n",in_dYMax);
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:LIM:UPP %.1lf\r\n",in_dYMax);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_UPPER_LIMIT_CONFG_FAILURE;
    }
    obj_Sleep->msleep(100);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:Y:SPAN:DBM %lf\r\n",in_dYScal);
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS:TRAC:Y:SCAL:PDIV %1f\r\n",in_dYScal);
    }
    else if (m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:TRAC:LIM:LOW %.1lf\r\n",in_dYScal);
    }
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }
    obj_Sleep->msleep(100);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "CALC:TRAC:X:LEFT %lf\r\n",in_dXStart);
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:OFFS:TIME %lfus\r\n",in_dXStart);
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:OFFS:TIME %.0lfms\r\n",in_dXStart);
    }
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }
    obj_Sleep->msleep(100);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "CALC:TRAC:X:LENG %lf\r\n",in_dXScal);
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:X:SCAL:PDIV %0.9lf\r\n", in_dXScal);
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:TIME %.0lfms\r\n", in_dXScal);
    }
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }
    obj_Sleep->msleep(100);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "DISPLAY:TRAC:MEAS:POW:PULS:TOP:STAT 1\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
        }
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:CORR:GAIN2:STAT ON\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }
    obj_Sleep->msleep(100);

    /*Offset*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, (char*)"SENS1:CORR:OFFS %f\r\n", in_dOffset);
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, (char*)"SENS1:CORR:GAIN2 %.0lf\r\n", in_dOffset);
    }    
    if(viSts)
    {
        return DP_PMETER_CONFG_OFFSET_FAILURE;
    }
    obj_Sleep->msleep(100);

   /*Trigger Level*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "TRIG:LEV %0.2lfDBM\r\n",in_dTriggerLvl);
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "TRIG:LEV %0.2lfDBM\r\n",in_dTriggerLvl);
//        sprintf(Buffer, "TRIG:SEQ:LEV:AUTO 1\r\n");
    }



    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return  DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
    }
    obj_Sleep->msleep(100);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        sprintf(Buffer, "DISP:WIND2:FORM TRAC\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(500);
    }    

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {/*Default : AVG*/
        /*Measurement - feed 1 */
        sprintf(Buffer, "CALC1:FEED1 \"POW:PEAK\"\r\n");

        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(100);
        /*Measurement - feed 1 */
        sprintf(Buffer, "CALC1:FEED2 \"POW:PEAK\"\r\n");
        qDebug() << Buffer;
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(100);

        /*Measurement - feed 2 */
        sprintf(Buffer, "CALC2:FEED1 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(100);

        /*Measurement - feed 2 */
        sprintf(Buffer, "CALC2:FEED2 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(100);

        if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
        {
            sprintf(Buffer, "DISP:WIND2:FORM TRAC\r\n");
        }
        if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
        {
            sprintf(Buffer, "DISP:WIND1:FORM TRACE\r\n");
        }

        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(100);

    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        /*Marker 1 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK1:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(100);

        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK1:POS:TIME %lf\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(100);

        /*Marker 2 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK2:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(100);

        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK2:POS:TIME %lf\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        /*Marker 1*/
        sprintf(Buffer, "SENS1:SWE1:OFFS:TIME %.7lfms\r\n",in_dMarker1Time);

        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(100);

        sprintf(Buffer, "SENS1:SWE1:TIME %.7lfms\r\n",in_dMarker2Time);

        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(100);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {

        sprintf(Buffer, "SENS1:FREQ %lf\r\n",in_dFreq*1e9);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);

        if(viSts)
        {
            return DP_PMETER_CNFG_FREQUENCY_FAILURE;
        }

    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:FREQ %.0lfHz\r\n",in_dFreq);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CNFG_FREQUENCY_FAILURE;
        }
        obj_Sleep->msleep(100);
    }
    obj_Sleep->msleep(100);
    /*Display type*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:DET:FUNC NORM\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }
    else if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "DISPLAY:TRAC:MEAS:POW:PULS:TOP:STAT 1\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
        }
    }    
    return DP_POWMTR_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_PwrMtr_ConfigPowerDroop()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to configure the Power droop in power meter
******************************************************************************************************/
ViStatus CPMWrapper::DP_PwrMtr_ConfigPowerDroop(ViSession in_uiVi, double in_dFreq, double in_dOffset, double in_dXStart, double in_dXScal, double in_dYMax, double in_dYScal, double in_dTriggerLvl, double in_dMarker1Time, double in_dMarker2Time)
{
    ViStatus viSts     = 0;
    ViChar Buffer[256] = {0};
    FDOUBLE dXScale = 0.0;

    sprintf(Buffer,"*RST\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return  DP_PMETER_RESET_FAILED;
    }

    obj_Sleep->msleep(500);

    sprintf(Buffer,"*CLS\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_CLS_FAILURE;
    }

    obj_Sleep->msleep(500);
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
         strcpy(Buffer, "DISP:WIND2:SEL2\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        strcpy(Buffer, "DISP:WIND2:SEL1\r\n");
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    {
        return DP_PMETER_CNFG_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);


    sprintf(Buffer,"TRIG:SOUR IMM\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return viSts;
    }

    obj_Sleep->msleep(500);

    sprintf(Buffer,"CONF1\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_CNFG_MODE_FAILURE;
    }

    sprintf(Buffer,"CONF2\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return viSts;
    }
    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        strcpy(Buffer, "TRIG:SOUR INT\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        strcpy(Buffer, "TRIG:ALL:SOUR EXT\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);

        /*Trig Mode*/
        strcpy(Buffer, "TRIG:ALL:MODE NORM\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }

        obj_Sleep->msleep(500);

        strcpy(Buffer,"CALC:TYPE TRAC\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_DISPLAY_FAILURE;
        }
    }

    obj_Sleep->msleep(500);

    strcpy(Buffer, "INIT1:CONT ON\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_FREE_RUN_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:Y:TOP:DBM %lf\r\n",in_dYMax);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:LIM:UPP %0.1lf\r\n",in_dYMax);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_DISPLAY_FAILURE;
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:Y:SPAN:DBM %lf\r\n",in_dYScal);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:Y:SCAL:PDIV %1f\r\n",in_dYScal);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:TRAC:LIM:LOW %0.1lf\r\n",in_dYScal);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_DISPLAY_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "CALC:TRAC:X:LEFT %lf\r\n",in_dXStart);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:OFFS:TIME %1fus\r\n",in_dXStart);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS:TRAC:OFFS:TIME %.0lfus\r\n",in_dXStart);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_DISPLAY_FAILURE;
    }

    dXScale = in_dXScal * 1e-9;
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "CALC:TRAC:X:LENG %lf\r\n",in_dXScal);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:X:SCAL:PDIV  %0.9lf\n",dXScale);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS:TRAC:TIME %.0lf\n",in_dXScal);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_DISPLAY_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        strcpy(Buffer, "DISPLAY:TRAC:MEAS:POW:PULS:TOP:STAT 1\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
        }
    }

    /*Offset Enable*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        strcpy(Buffer, "SENS1:CORR:GAIN2:STAT ON\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    obj_Sleep->msleep(500);

    /*Offset*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, (char*)"SENS1:CORR:OFFS %f\r\n", in_dOffset);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, (char*)"SENS1:CORR:GAIN2 %.0lf\r\n", in_dOffset);
    }

    if(viSts)
    {
        return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

   /*Trigger Level*/
    sprintf(Buffer, "TRIG:LEV %.0lfDBM\r\n",in_dTriggerLvl);
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return  DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        /*Measurement - feed 1 */
        strcpy(Buffer, "CALC1:FEED1 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        /*Measurement - feed 2 */
        strcpy(Buffer, "CALC1:FEED2 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        obj_Sleep->msleep(500);

        /*Measurement - feed 1 */
        strcpy(Buffer, "CALC2:FEED1 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        /*Measurement - feed 2 */
        strcpy(Buffer, "CALC2:FEED2 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        obj_Sleep->msleep(500);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        /*Marker 1 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK1:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK1:POS:TIME %lf\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*Marker 2 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK2:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK2:POS:TIME %lf\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        strcpy(Buffer, "DISP:WIND2:FORM TRAC\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        strcpy(Buffer, "DISP:WIND2:FORM TRACE\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    /*Marker 1*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        sprintf(Buffer, "SENS1:SWE1:OFFS:TIME %0.2lfns\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:SWE1:OFFS:TIME:%0.2lfns\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    obj_Sleep->msleep(500);

    /*Marker 2*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        sprintf(Buffer, "SENS1:SWE1:TIME %0.2lfus\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:SWE1:TIME %0.2lfus\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    obj_Sleep->msleep(500);

    sprintf(Buffer, "SENS1:FREQ %.0lfGHz\r\n",in_dFreq/1000000000.0);
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return viSts;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        strcpy(Buffer, "SENS1:DET:FUNC NORM\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        strcpy(Buffer, "DISPLAY:TRAC:MEAS:POW:PULS:TOP:STAT 1\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
        }
    }

    return DP_POWMTR_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_PowerMeter_Config1()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to configure Rise time in power meter
******************************************************************************************************/
ViStatus CPMWrapper::DP_PowerMeter_Config1(ViSession in_uiVi, double in_dFreq, double in_dOffset, double in_dXStart, double in_dXScal, double in_dYMax, double in_dYScal, double in_dTriggerLvl, double in_dMarker1Time, double in_dMarker2Time)
{
    ViStatus viSts     = 0;
    ViChar Buffer[256] = {0};
    FDOUBLE dXScale = 0.0;

    sprintf(Buffer,"*RST\r\n");
    viSts = viPrintf(viPM, Buffer);
    if(viSts)
    {
        return viSts;
    }

    sprintf(Buffer,"*CLS\r\n");
    viSts = viPrintf(viPM, Buffer);
    if(viSts)
    {
        return viSts;
    }

    obj_Sleep->msleep(500);

    sprintf(Buffer,"CONF1\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_CNFG_MODE_FAILURE;
    }

    sprintf(Buffer,"CONF2\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_CNFG_MODE_FAILURE;
    }
    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        strcpy(Buffer, "TRIG:SOUR INT\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        strcpy(Buffer, "TRIG:ALL:SOUR EXT\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);

        /*Trig Mode*/
        strcpy(Buffer, "TRIG:ALL:MODE NORM\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }

        obj_Sleep->msleep(500);

        strcpy(Buffer,"CALC:TYPE PULS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_DISPLAY_FAILURE;
        }
    }

    obj_Sleep->msleep(500);

    strcpy(Buffer, "INIT1:CONT ON\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_FREE_RUN_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:Y:TOP:DBM %lf\r\n",in_dYMax);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:LIM:UPP %.1lf\r\n",in_dYMax);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_UPPER_LIMIT_CONFG_FAILURE;
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:Y:SPAN:DBM %lf\r\n",in_dYScal);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:Y:SCAL:PDIV %1f\r\n",in_dYScal);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:TRAC:LIM:LOW %.1lf\r\n",in_dYScal);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "CALC:TRAC:X:LEFT %lf\r\n",in_dXStart);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:OFFS:TIME %lfus\r\n",in_dXStart);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:OFFS:TIME %.0lfus\r\n",in_dXStart);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }

    dXScale = in_dXScal * 1e-9;
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "CALC:TRAC:X:LENG %lf\r\n",in_dXScal);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:X:SCAL:PDIV  %0.9lf\n", dXScale);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:TIME %.0lf\n", in_dXScal);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        strcpy(Buffer, "DISPLAY:TRAC:MEAS:POW:PULS:TOP:STAT 1\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
        }
    }

    /*Offset Enable*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        strcpy(Buffer, "SENS1:CORR:GAIN2:STAT ON\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    /*Offset*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, (char*)"SENS1:CORR:OFFS %f\r\n", in_dOffset);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, (char*)"SENS1:CORR:GAIN2 %f\r\n", in_dOffset);
    }

    if(viSts)
    {
        return DP_PMETER_CONFG_OFFSET_FAILURE;
    }

    /*Trigger Level*/
    sprintf(Buffer, "TRIG:LEV %0.2lfDBM\r\n",in_dTriggerLvl);
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return  DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        /*Measurement - feed 1 */
        strcpy(Buffer, "CALC1:FEED1 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        /*Measurement - feed 1 */
        strcpy(Buffer, "CALC1:FEED2 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        obj_Sleep->msleep(500);

        /*Measurement - feed 2 */
        strcpy(Buffer, "CALC2:FEED1 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        /*Measurement - feed 2 */
        strcpy(Buffer, "CALC2:FEED2 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        /*Marker 1 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK1:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK1:POS:TIME %lf\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*Marker 2 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK2:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK2:POS:TIME %lf\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        strcpy(Buffer, "DISP:WIND2:FORM TRAC\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(500);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        strcpy(Buffer, "DISP:WIND1:FORM TRACE\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(500);
    }

    /*Marker 1*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        sprintf(Buffer, "SENS1:SWE1:OFFS:TIME %0.2lfns\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        obj_Sleep->msleep(500);

        sprintf(Buffer, "SENS1:SWE1:TIME %0.2lfus\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        obj_Sleep->msleep(500);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "SENS1:FREQ %0.2lfGHz\r\n",in_dFreq/1000000000.0);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CNFG_FREQUENCY_FAILURE;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:FREQ %0.2lfHz\r\n",in_dFreq);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CNFG_FREQUENCY_FAILURE;
        }
    }

    obj_Sleep->msleep(500);

    /*Display type*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        strcpy(Buffer, "SENS1:DET:FUNC NORM\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        strcpy(Buffer, "DISPLAY:TRAC:MEAS:TRAN:POS:DUR 1\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
        }
    }
    obj_Sleep->msleep(500);

    return DP_POWMTR_SUCCESS;

}

/***************************************************************************************************
// Name						:	 DP_PowerMeter_Config2()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to configure Fall time in power meter
******************************************************************************************************/
ViStatus CPMWrapper::DP_PowerMeter_Config2(ViSession in_uiVi, double in_dFreq, double in_dOffset, double in_dXStart, double in_dXScal, double in_dYMax, double in_dYScal, double in_dTriggerLvl, double in_dMarker1Time, double in_dMarker2Time)
{
    ViStatus viSts     = 0;
    ViChar Buffer[256] = {0};
    FDOUBLE dXScale = 0.0;

    sprintf(Buffer,"TRIG:SOUR IMM\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRIG_SOUR_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        strcpy(Buffer, "DISP:WIND2:SEL2\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        strcpy(Buffer, "DISP:WIND2:SEL1\r\n");
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    {
        return DP_PMETER_CNFG_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        strcpy(Buffer, "TRIG:SOUR INT\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        strcpy(Buffer, "TRIG:ALL:SOUR EXT\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);

        /*Trig Mode*/
        strcpy(Buffer, "TRIG:ALL:MODE NORM\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
        }

        obj_Sleep->msleep(500);


        strcpy(Buffer,"CALC:TYPE PULS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_DISPLAY_FAILURE;
        }
    }

    obj_Sleep->msleep(500);

    strcpy(Buffer, "INIT1:CONT ON\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_FREE_RUN_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:Y:TOP:DBM %lf\r\n",in_dYMax);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:LIM:UPP %.1lf\r\n",in_dYMax);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_UPPER_LIMIT_CONFG_FAILURE;
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:Y:SPAN:DBM %lf\r\n",in_dYScal);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:Y:SCAL:PDIV %1f\r\n",in_dYScal);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:TRAC:LIM:LOW %.1lf\r\n",in_dYScal);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "CALC:TRAC:X:LEFT %lf\r\n",in_dXStart);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:OFFS:TIME %lfus\r\n",in_dXStart);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:OFFS:TIME %.0lfus\r\n",in_dXStart);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }

    dXScale = in_dXScal * 1e-9;
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        sprintf(Buffer, "CALC:TRAC:X:LENG %lf\r\n",in_dXScal);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:X:SCAL:PDIV  %0.9lf\n", dXScale);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        sprintf(Buffer, "SENS1:TRAC:TIME %.0lf\n", in_dXScal);
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        strcpy(Buffer, "DISPLAY:TRAC:MEAS:POW:PULS:TOP:STAT 1\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
        }
    }

    /*Offset Enable*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        strcpy(Buffer, "SENS1:CORR:GAIN2:STAT ON\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    /*Offset*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, (char*)"SENS1:CORR:OFFS %f\r\n", in_dOffset);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, (char*)"SENS1:CORR:GAIN2 %f\r\n", in_dOffset);
    }

    if(viSts)
    {
        return DP_PMETER_CONFG_OFFSET_FAILURE;
    }

    /*Trigger Level*/
    sprintf(Buffer, "TRIG:LEV %0.2lfDBM\r\n",in_dTriggerLvl);
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return  DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        /*Measurement - feed 1 */
        strcpy(Buffer, "CALC1:FEED1 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        /*Measurement - feed 1 */
        strcpy(Buffer, "CALC1:FEED2 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        obj_Sleep->msleep(500);

        /*Measurement - feed 2 */
        strcpy(Buffer, "CALC2:FEED1 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        /*Measurement - feed 2 */
        strcpy(Buffer, "CALC2:FEED2 \"POW:PEAK\"\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        /*Marker Mode*/
        sprintf(Buffer, "CALC:DMOD MARK\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*Marker 1 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK1:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK1:POS:TIME %lf\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*Marker 2 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK2:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK2:POS:TIME %lf\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        strcpy(Buffer, "DISP:WIND2:FORM TRAC\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(500);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        strcpy(Buffer, "DISP:WIND1:FORM TRACE\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(500);
    }

    /*Marker 1*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:SWE1:OFFS:TIME %0.2lfns\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        obj_Sleep->msleep(500);

        sprintf(Buffer, "SENS1:SWE1:TIME %0.2lfus\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }

        obj_Sleep->msleep(500);
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "SENS1:FREQ %0.2lfGHz\r\n",in_dFreq/1000000000.0);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CNFG_FREQUENCY_FAILURE;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:FREQ %0.2lfHz\r\n", in_dFreq);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CNFG_FREQUENCY_FAILURE;
        }
    }

    obj_Sleep->msleep(500);

    /*Display type*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)//Keysight wrapper
    {
        strcpy(Buffer, "SENS1:DET:FUNC NORM\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
    }

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        strcpy(Buffer, "DISPLAY:TRAC:MEAS:TRAN:NEG:DUR 1\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE;
        }
    }

    obj_Sleep->msleep(500);

    return DP_POWMTR_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_PwrMtr_SetFrequency()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to set frequency in power meter
******************************************************************************************************/
S16BIT CPMWrapper::DP_PwrMtr_SetFrequency(ViSession in_uiVi, double in_dFreq)
{
    ViStatus viSts     = 0;
    ViChar Buffer[256] = {0};

    memset(Buffer, 0, sizeof(Buffer));

    sprintf(Buffer, "SENS1:FREQ %.0lfHz\r\n", in_dFreq);
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_CNFG_FREQUENCY_FAILURE;
    }

    qDebug() << Buffer;

    obj_Sleep->msleep(500);

    return 0;
}

/***************************************************************************************************
// Name						:	 DP_PwrMtr_MarkerConfig()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to configure the marker in power meter
******************************************************************************************************/
ViStatus CPMWrapper:: DP_PwrMtr_MarkerConfig(ViSession in_uiVi, U8BIT in_ucTriggerSrcType ,double in_dMarker1Time,double in_dMarker2Time)
{
    ViStatus viSts    = 0;
    ViChar Buffer[256] = {0};

    strcpy(Buffer, "SENS1:TRAC:MEAS:POW:AVG?\r\n");//not available
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_READ_POWER_FAILURE;
    }

    obj_Sleep->msleep(500);

    in_ucTriggerSrcType = 0;

    if(in_ucTriggerSrcType == 0)
    {
        strcpy(Buffer,"TRIG:SOUR EXT\r\n");
    }
    else
    {
        strcpy(Buffer,"TRIG:SOUR INT\r\n");
    }

    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRIG_CHANGE_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    strcpy(Buffer, "TRIG:DEL 1ms\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRIG_DELETE_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    strcpy(Buffer, "TRIG:HOLD 10us\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRIG_HOLD_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    strcpy(Buffer, "OUTP:TRIG ON\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_OUT_TRIG_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    strcpy(Buffer, "TRIG:ALL:DEL 49.5us\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_TRACE_SETUP_FAILURE;
    }

    obj_Sleep->msleep(500);

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)//RS wrapper
    {
        /*Marker 1 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK1:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK1:POS:TIME %lf\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*Marker 2 ON*/
        sprintf(Buffer, "DISP:TRAC:MARK2:MODE MEAS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }
        obj_Sleep->msleep(500);
        /*MARKER POSITION*/
        sprintf(Buffer, "DISP:TRAC:MARK2:POS:TIME %lf\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer);
        if(viSts)
        {
            return DP_PMETER_TRIG_LEVEL_MODE_FAILURE;
        }

        obj_Sleep->msleep(500);
    }

    /*Marker 1*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A  || m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:SWE1:OFFS:TIME %0.2lfns\r\n",in_dMarker1Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(500);
    }

    /*Marker 2*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A ||  m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "SENS1:SWE1:TIME %0.2lfus\r\n",in_dMarker2Time);
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return viSts;
        }
        obj_Sleep->msleep(500);
    }

    strcpy(Buffer, "INIT:CONT ON\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_FREE_RUN_FAILURE;
    }

    obj_Sleep->msleep(500);

    strcpy(Buffer, "*OPC\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_OPERATION_COMP_MODE_FAILURE;
    }

    obj_Sleep->msleep(500);

    return DP_POWMTR_SUCCESS;
}
/***************************************************************************************************
// Name						:	 DP_PwrMtr_MeasurPulseRiseTime()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to measure pulse rise time in power meter
******************************************************************************************************/
S16BIT CPMWrapper::DP_PwrMtr_MeasurPulseRiseTime(ViSession in_uiVi, double *out_pdMeasRiseTime)
{
    QString csFormat;
    ViChar Buffer[256] = {NULL};
    ViStatus viSts    = 0;

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        strcpy(Buffer, "CALC:TRAC:MEAS:TRAN:POS:DUR?\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        strcpy(Buffer, "TRAC1:MEAS:TRAN1:POS:DUR?\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {

    }

    viSts = viQueryf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer, "%lf\n", out_pdMeasRiseTime);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_MEASUREMENT_PULSE_PERIOD_MODE_FAILURE;
    }

    return DP_POWMTR_SUCCESS;
}
/***************************************************************************************************
// Name						:	 DP_PwrMtr_MeasurPulseFallTime()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to measure pulse fall time in power meter
******************************************************************************************************/
S16BIT CPMWrapper::DP_PwrMtr_MeasurPulseFallTime(ViSession in_uiVi, double *out_pdMeasFallTime)
{
    QString csFormat;
    ViChar Buffer[256] = {NULL};
    ViStatus viSts    = 0;

    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        strcpy(Buffer, "CALC:TRAC:MEAS:TRAN:NEG:DUR?\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        strcpy(Buffer, "TRAC:MEAS:TRAN:NEG:DUR?\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {

    }

    viSts = viQueryf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer, "%lf\n", out_pdMeasFallTime);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_MEASUREMENT_PULSE_PERIOD_MODE_FAILURE;
    }

    return DP_POWMTR_SUCCESS;
}

ViStatus CPMWrapper::DP_PwrMtr_Trig_AutoMode(ViSession in_uiVi)
{
    ViStatus viSts = 0;
    ViChar Buffer[256] = {0};

    /*Continuous triggering*/
    strcpy(Buffer, "INIT1:CONT ON\r\n");
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts)
    {
        return DP_PMETER_FREE_RUN_FAILURE;
    }

    return DP_POWMTR_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_PowerMeter_FreeRun()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to enable or disable the free run in power meter
******************************************************************************************************/
ViStatus CPMWrapper::DP_PowerMeter_FreeRun(ViSession in_uiVi, bool in_EnaDis)
{
    ViStatus viSts = 0;
    ViChar Buffer[256] = {0};

    memset(Buffer, 0, sizeof(Buffer));

    if(in_EnaDis == true)
    {
        /*Continuous triggering*/
        sprintf(Buffer, "INIT1:CONT ON\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_FREE_RUN_FAILURE;
        }
    }
    else
    {
        /*Stop triggering*/
        sprintf(Buffer, "INIT1:CONT OFF\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_FREE_RUN_FAILURE;
        }
    }

    qDebug() << Buffer;

    return DP_POWMTR_SUCCESS;
}


ViStatus CPMWrapper::DP_PowerMeter_Slope(ViSession in_uiVi, bool in_PosNeg)
{
    ViStatus viSts = 0;
    ViChar Buffer[256] = {0};

    memset(Buffer, 0, sizeof(Buffer));
    /*Slope */
    if(in_PosNeg == true)
    {
        /*Continuous triggering*/
        sprintf(Buffer, "TRIG:SEQ:SLOP POS\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_FREE_RUN_FAILURE;
        }
    }
    else
    {
        /*Stop triggering*/
        sprintf(Buffer, "TRIG:SEQ:SLOP NEG\r\n");
        viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
        if(viSts)
        {
            return  DP_PMETER_FREE_RUN_FAILURE;
        }
    }

    qDebug() << Buffer;

    return DP_POWMTR_SUCCESS;
}
/***************************************************************************************************
// Name						:	 DP_PwrMtr_ReadMarkerPower()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to Read Marker power in power meter
******************************************************************************************************/
ViStatus CPMWrapper::DP_PwrMtr_ReadMarkerPower(ViSession in_uiVi, U8BIT usMarkerNo, double *out_pdPower)
{
    ViStatus viSts = 0;
    ViChar Buffer[256] = {0};

    /*Read Marker Power*/
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        sprintf(Buffer, "SENS1:SWE1:MARK%d:POW?\r\n",usMarkerNo);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:MARK%d:YPOS?\r\n",usMarkerNo);
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
       // sprintf(Buffer, "SENS1:SWE%d:OFFS:TIME?\r\n",usMarkerNo);
        //
    }

    viSts = viQueryf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, Buffer, "%lf\r\n", out_pdPower);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_READ_POWER_FAILURE;
    }

    return DP_POWMTR_SUCCESS;
}
/***************************************************************************************************
// Name						:	 DP_PwrMtr_ReadPeakPower()
// Author					:
// Global Variables affected:	 NA
// Created Date				:
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to Read Peek power in power meter
******************************************************************************************************/
ViStatus CPMWrapper::DP_PwrMtr_ReadPeakPower(ViSession in_uiVi, double *out_pdPower)
{
    ViStatus viSts = 0;
    ViChar Buffer[256] = {0};

//    memset(Buffer, 0, sizeof(Buffer));
//    sprintf(Buffer, "INIT1:CONT OFF\r\n");
//    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
//    if(viSts)
//    {
//        return DP_PMETER_FREE_RUN_FAILURE;
//    }

//    qDebug() << Buffer;

    memset(Buffer, 0, sizeof(Buffer));

    /* Get the marker value */
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMN1911A)
    {
        sprintf(Buffer, "MEAS1:POW:AC?\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMP4416A)
    {
        sprintf(Buffer, "MEAS:POW:AC?\r\n");
    }
    if(m_arrPMInstHandle[in_uiVi-1].m_usInstID == PMENRX)
    {
        sprintf(Buffer, "CALC:TRAC:MEAS:POW:PULS:TOP?\r\n");
    }

    qDebug() << Buffer;

    viSts = viQueryf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer, "%lf\r\n", out_pdPower);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_READ_POWER_FAILURE;
    }

    obj_Sleep->msleep(300);
    return DP_POWMTR_SUCCESS;
}

S16BIT CPMWrapper::DP_PwrMtr_WaitforOperationComplete(ViSession in_uiVi)
{
    ViStatus viSts    = 0;
    S32BIT iOpc = 0;
    S32BIT iTempRetCount = 0;

    while(!iOpc)
    {
        viSts = viQueryf(m_arrPMInstHandle[in_uiVi-1].m_viPMSession, (char*)"*OPC?\n", "%d", &iOpc);
        if(viSts != VI_SUCCESS)
        {
            return DP_PMETER_OPER_COM_FAILURE;
        }

        if(iTempRetCount >= 200)
            break;
        iTempRetCount++;
    }

    return DP_POWMTR_SUCCESS;
}

S16BIT CPMWrapper::DP_PwrMtr_Calibration(ViSession in_uiVi)
{
    ViStatus viSts = 0;
    ViChar Buffer[256] = {0};

    sprintf(Buffer, "OUTP:ROSC:STAT 1\r\n");  //PoWer Reference ON
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    obj_Sleep->msleep(300);

    sprintf(Buffer, "CAL1:ZERO:AUTO ONCE\r\n");  //Power Meter Zero
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    obj_Sleep->msleep(4000);

    sprintf(Buffer, "CAL1:AUTO ONCE\r\n");  //PoWer Meter Calibration
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    obj_Sleep->msleep(6000);

    sprintf(Buffer, "CAL1:RCAL 1\r\n");  //PoWer Meter ZERO / CAL lockout Facility
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    obj_Sleep->msleep(300);

    sprintf(Buffer, "OUTP:ROSC:STAT 0\r\n");  //PoWer Reference OFF
    viSts = viPrintf(m_arrPMInstHandle[in_uiVi - 1].m_viPMSession, Buffer);
    if(viSts != VI_SUCCESS)
    {
        return DP_PMETER_OPER_COM_FAILURE;
    }

    return DP_POWMTR_SUCCESS;
}
