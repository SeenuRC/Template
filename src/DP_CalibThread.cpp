/*******************************************************************************
 Company Name : DATA PATTERNS INDIA PRIVATE LIMITED
 appendress      : No. H9,4th MainRoad,SIPCOT IT Park,Siruseri,Chennai-603103
 Email        : support@datapatterns.co.in
 Phone        : +91 44 47414000
 Fax          : +91 44 47414444
*******************************************************************************/

/******************************************************************************
 Copyright (c) 2019 DATA PATTERNS

All rights reserved. These materials are confidential and proprietary to
        DATA PATTERNS and no part of these materials should be reproduced or published
        in any form by any means, electronic or mechanical, including photocopy, on any
                                                           inaasprintfion storage or retrieval system, nor should the materials be disclosed
            to third parties without the explicit written authorization of DATA PATTERNS.

                ****************************************************************************/

#include "DP_CalibThread.h"

extern class CReportConfigWrapper *m_pobjReportWrapper;
extern MainWindow *p_objMainApp;
extern CConfigParamReadWrapper *m_pobjReadConfigData;

U8BIT g_ucCalibthread;
U8BIT g_ucTotaltestSel;
U32BIT g_uiTotRowCount;
U16BIT g_ucEquipment;
U8BIT g_ucarrTestrecord[11];
QString g_strlist[48];
bool bTestExecSts;
U8BIT g_ucTestStopped;

#define AUTOENABLE 0

CalibTestThread::CalibTestThread(QObject *parent): QThread(parent)
{
    g_ucCalibthread = 0;

    connect(this,&CalibTestThread::sig_MsgToPrintlog, ((MainWindow *)this->parent()), &MainWindow::Printlog);
}
/*********************************************************************************************************
// Name						:	 Start()
// Author					:	 Venkatesh S
// Global Variables affected:	 NA
// Created Date				:    23/08/2023
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to start the thread for perform the auto mode test case
*************************************************************************************************************/

void CalibTestThread :: Start()
{
    if(g_ucCalibthread == 0)
    {
        g_ucCalibthread = 1;

        start(QThread :: HighPriority);//thread begin
    }
}
/*********************************************************************************************************
// Name						:	 Stop()
// Author					:	 Venkatesh S
// Global Variables affected:	 NA
// Created Date				:    23/08/2023
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to Stop the thread for perform the auto mode test case
*************************************************************************************************************/

void CalibTestThread :: stop()
{
    if(isRunning())
    {
        g_ucCalibthread = 0;

        terminate();
        msleep(500);

        if(isRunning())
        {
            terminate();
        }

        quit();
    }

}
/*********************************************************************************************************
// Name						:	 run()
// Author					:	 Venkatesh S
// Global Variables affected:	 NA
// Created Date				:    23/08/2023
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to start the thread for perform the auto mode test case
*************************************************************************************************************/
void CalibTestThread ::run()
{
    S8BIT szTempMsg[200]                           = {0};
      U8BIT ucCompletedCount                          = 0;
    U8BIT ucResult                                  = 0;
    U8BIT u8SigGenSel                               = 1;
    U8BIT u8SpecAnSel                               = 1;
    S16BIT s32RetVal                                = 0;
    S32BIT s32SpotCount                             = 0;
    S32BIT s32IndvCount                             = 0;
    U32BIT iuRow                                    = 0;
    U32BIT u32TestCaseID                            = 0;
    float fPeakValue                                = 0.0f;
    FDOUBLE dRBW                                    = 0.0;
    FDOUBLE dRefLevel                               = 0.0;
    FDOUBLE dSpan                                   = 0.0;
    FDOUBLE dVBW                                    = 0.0;
    FDOUBLE dSweepTime                              = 0.0;
    FDOUBLE dMarkerPos                              = 0.0;
    FDOUBLE dStepLoop                               = 0.0;
    FDOUBLE dMeasPwr                                = 0.0;
    QString strMsgHeader                            = "";
    QString strActLog                               = "";
    QString strMessage                              = "";
    QString time_format;
    QString strSysTime;
    QDateTime m_DateTime;
    QStringList qsChannellist                   = {};
    QByteArray arr_BytArr;

    time_format = "yyyy-MM-dd HH:mm:ss";
    m_DateTime = QDateTime::currentDateTime();
    strSysTime = m_DateTime.toString(time_format);

    if(g_ucCalibthread)
    {
        for(iuRow = 0; iuRow < g_uiTotRowCount; iuRow++ )
        {
            s32IndvCount = 0;
            emit Sig_IndTest_ProgBar(s32IndvCount);
            if(g_ucarrTestrecord[iuRow])
            {
                bTestExecSts = 1;

                S_SBANDGANPAMODULE_CABLELOSSCALIBRATION s_cableloss_calib;
                S_CALIB_CABLOSS_RPT s_cableloss_rpt;
                memset(&s_cableloss_rpt,0,sizeof(S_CALIB_CABLOSS_RPT));

                s32RetVal = m_pobjReportWrapper->ReadTestCaseID("Cable Loss Calibration", &u32TestCaseID);
                if(s32RetVal)
                {
                    sprintf(szTempMsg,"Unable to Test Case ID from database");
                    g_ucCalibthread = 0;
                    bTestExecSts = 0;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,szTempMsg);
                    emit sig_ThreadStop();
                    return;
                }

                s32RetVal= m_pobjReadConfigData->ReadConfigData(u32TestCaseID, (iuRow+1), &s_cableloss_calib);//to be updated
                if(s32RetVal < 0)
                {
                    sprintf(szTempMsg,"Unable to read configuration from database");
                    g_ucCalibthread = 0;
                    bTestExecSts = 0;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,szTempMsg);
                    emit sig_ThreadStop();
                    return;
                }

                dSpan = s_cableloss_calib.m_dSpan;
                dRBW  = s_cableloss_calib.m_dRBW;
                dVBW  = s_cableloss_calib.m_dVBW;
                dSweepTime = s_cableloss_calib.m_s8SweepTime;
                dRefLevel  = s_cableloss_calib.m_dReferenceLevel;

                s_cableloss_rpt.m_s32PathID = s_cableloss_calib.m_s8PathID;
                s_cableloss_rpt.m_dAmp = s_cableloss_calib.m_dInputPower;

                //Configure Preset in RF SG
#if AUTOENABLE
                s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SigGenSel);
#endif
                if(s32RetVal)
                {
                    sprintf(szTempMsg,"Unable to Configure Preset in RF Signal Generator for Cable loss calibration");
                    g_ucCalibthread = 0;
                    bTestExecSts = 0;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,szTempMsg);
                    emit sig_ThreadStop();
                    return;
                }

                // Config Center Freq, Ref Level and  Span in SA
#if AUTOENABLE
                s32RetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SpecAnSel, dRefLevel);
#endif
                if(s32RetVal)
                {
                    sprintf(szTempMsg,"Unable to configure reference level Cable loss calibration");
                    g_ucCalibthread = 0;
                    bTestExecSts = 0;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,szTempMsg);
                    emit sig_ThreadStop();
                    return;
                }

                // Config  RBW , VBW in SA
#if AUTOENABLE
                s32RetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SpecAnSel, true, (dRBW * (10e2)), true, (dVBW * (10e2)), true, dSweepTime);
#endif
                if(s32RetVal)
                {
                    sprintf(szTempMsg,"Unable to configure RBW,VBW and sweep time in Cable loss calibration");
                    bTestExecSts = 0;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,szTempMsg);
                    emit sig_ThreadStop();
                    return;
                }
                try
                {
                    for(dStepLoop = s_cableloss_calib.m_dStartFrequency ; dStepLoop <= s_cableloss_calib.m_dStopFrequency; dStepLoop += s_cableloss_calib.m_dStepFrequency) /*update freq from ini file*/
                    {
                        // Config  Frequency and span in SA
#if AUTOENABLE
                        s32RetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnSel, (dStepLoop * 1e6), (dSpan * (10e5)));
#endif
                        if(s32RetVal)
                        {
                            sprintf(szTempMsg,"Unable to configure center frequency and span in Cable loss calibration");
                            throw -1;
                        }

                        // Configure SigGen freq and power for RF
#if AUTOENABLE
                        s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel, ( dStepLoop * 1e6) , s_cableloss_calib.m_dInputPower);
#endif
                        if(s32RetVal)
                        {
                            sprintf(szTempMsg,"Unable to configure frequency and power level in signal generator");
                            throw -1;
                        }

                        // Enable Rf out in RF SG
#if AUTOENABLE
                        s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, true);
#endif
                        if(s32RetVal)
                        {
                            sprintf(szTempMsg,"Unable to disable RF output in signal generator");
                            throw -1;
                        }

                        // Enable continuous run
#if AUTOENABLE
                        s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnSel, DP_SA_ENABLE_CONTINOUS);
#endif
                        if(s32RetVal)
                        {
                            sprintf(szTempMsg,"Unable to enable the continous run in spectrum analyzer");
                            throw -1;
                        }
                        msleep(500);

                        // Disable continuous run
#if AUTOENABLE
                        s32RetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnSel, DP_SA_DISABLE_CONTINOUS);
#endif
                        if(s32RetVal)
                        {
                            sprintf(szTempMsg,"Unable to disable the continous run in spectrum analyzer");
                            throw -1;
                        }

#ifdef PEAK_VALUE_ENABLED
                        //Read Peak value
#if AUTOENABLE
                        fPeakValue = dMeasPwr;
                        s32RetVal= p_objMainApp->m_objSAWrapper.AgSA_ReadPeakValue(u8SpecAnSel, &fPeakValue);
                        dMeasPwr = fPeakValue;
#endif
#else
                        // Read Marker Frequency
#if AUTOENABLE
                        s32RetVal=p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SpecAnSel,(dMarkerPos * 1e6),&dMeasPwr);
#endif
#endif
                        if(s32RetVal)
                        {
                            sprintf(szTempMsg,"Unable to read marker frequency in spectrum analyze");
                            throw -1;
                        }

                        // Disable RF Out in SG
#if AUTOENABLE
                        s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, false);
#endif
                        if(s32RetVal)
                        {
                            sprintf(szTempMsg,"Unable to disable RF output in signal generator");
                            throw -1;
                        }

                        sprintf(s_cableloss_rpt.m_szPathName,"%s",g_strlist[iuRow].toStdString().c_str());

                        s_cableloss_rpt.m_dPathLoss = s_cableloss_calib.m_dInputPower - dMeasPwr;

                        strActLog = QString::asprintf("Path ID: %d, Input Freqency: %0.3f MHz, Power: %0.2f dBm, Exp.Min.Power: %0.2f dBm, Exp. Max.Power: %0.2f dBm, Measured Power:%0.2f dBm, Cable loss: %0.2f dBm", s_cableloss_calib.m_s8PathID,dStepLoop, s_cableloss_calib.m_dInputPower, s_cableloss_calib.m_dExpMinLoss, s_cableloss_calib.m_dExpMaxLoss, dMeasPwr, s_cableloss_rpt.m_dPathLoss);

                        msleep(100);

                        /* Checking Calib Power Limit */
                        if((s_cableloss_rpt.m_dPathLoss <  s_cableloss_calib.m_dExpMinLoss) || (s_cableloss_rpt.m_dPathLoss >  s_cableloss_calib.m_dExpMaxLoss))
                        {
                            ucResult = DP_TEST_FAIL;
                            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,strActLog);
                        }
                        else
                        {
                            ucResult = DP_TEST_PASS;
                            emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,strActLog);
                        }
                        emit Sig_IndTest_ProgBar(s32SpotCount);

                        s_cableloss_rpt.m_dRadioFreq = dStepLoop;
                        s_cableloss_rpt.m_dPathLoss = s_cableloss_rpt.m_dPathLoss;
                        strcpy(s_cableloss_rpt.m_szST,strSysTime.toStdString().c_str());

                        QVectCalibration.push_back(s_cableloss_rpt);

                        s32IndvCount++;
                        emit Sig_IndTest_ProgBar(s32IndvCount);
                    }
                }catch (...) {
#ifndef _SIMULATION_
                    // Enable Rf out in RF SG
                    s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, false);
#else
                    s32RetVal = 0;
#endif
                    if(s32RetVal)
                    {
                        sprintf(szTempMsg,"Unable to disable RF output");
                        throw -1;
                    }

                    g_ucCalibthread = 0;
                    bTestExecSts = 0;
                    QVectCalibration.push_back(s_cableloss_rpt);
                    OnSafeState();
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,szTempMsg);

                    return;

                }
                OnSafeState();// test selected ends
            }
            emit Sig_TotTest_ProgBar((ucCompletedCount+=1));
        }
        g_ucCalibthread = 0;
    }
    emit sig_ThreadStop();
}

void CalibTestThread::OnSafeState()
{
    S32BIT s32RetVal      = 0;

    if(QVectCalibration.size() > 0)
    {
        s32RetVal = m_pobjReportWrapper->Insert_calib_cabloss_rpt_Record(QVectCalibration,"Cable loss Report");
        QVectCalibration.clear();
    }
}
