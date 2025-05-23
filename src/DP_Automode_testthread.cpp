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

*******************************************************************************/
#include "DP_Automode_testthread.h"
extern bool g_bAutoTestStartStop;
extern CReportConfigWrapper *m_pobjReportWrapper;
extern MainWindow *p_objMainApp;
unsigned char ucFailCnt[30];
unsigned char ucSuccessCnt[30];

/*********************************************************************************************************
// Name						:	 Start()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    14/06/2023
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to start the thread for perform the auto mode test case
*************************************************************************************************************/

void CAutoTestThread :: Start(SAUTOTESTCONFIG in_STestConfig)
{
    if(p_objMainApp->m_bIsTestActive == false)
    {
        p_objMainApp->m_bIsTestActive = true;

        m_STestConfig.m_ucTestSelCnt      = in_STestConfig.m_ucTestSelCnt;
        m_STestConfig.m_ucMajorCycCnt     = in_STestConfig.m_ucMajorCycCnt;
        m_STestConfig.m_uiDelayBwnTest    = in_STestConfig.m_uiDelayBwnTest;
        m_STestConfig.m_uiDelayBwnMajCyc  = in_STestConfig.m_uiDelayBwnMajCyc;
        m_STestConfig.m_uiDelayBwnMinCyc  = in_STestConfig.m_uiDelayBwnMinCyc;

        memcpy(m_STestConfig.m_ucTestCaseSel, in_STestConfig.m_ucTestCaseSel, sizeof(m_STestConfig.m_ucTestCaseSel));

        start(QThread :: HighPriority);//thread begin
    }
}
/*********************************************************************************************************
// Name						:	 run()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    14/06/2023
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to start the thread for perform the auto mode test case
*************************************************************************************************************/
void CAutoTestThread :: run()
{
    unsigned char ucTestCnt			= 0;
    unsigned char ucChNo            = 0;
    unsigned short usMajorCnt		= 0;
    unsigned short usTotMajorCnt	= 0;
    short sRetVal                   = 0;
    QString strTemp1                = "";
    QString strTestName             = "";

    sRetVal = m_pobjReportWrapper->InsertMainExec(1, "SBAND GaN PA Module");
    if(sRetVal != DP_SUCCESS)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, "Unable to Insert Records into Main Execution");
        return;
    }

    usTotMajorCnt = p_objMainApp->m_pobjAutoTestCases->m_usTotMajorCnt;

    memset(m_uiAutoTestSuccessCnt, 0, sizeof(m_uiAutoTestSuccessCnt));
    memset(m_uiAutoTestFailCnt, 0, sizeof(m_uiAutoTestFailCnt));

    memset(ucFailCnt, 0, sizeof(ucFailCnt));
    memset(ucSuccessCnt, 0, sizeof(ucSuccessCnt));

    p_objMainApp->m_pobjAutoTestCases->m_ulOverallProg = 0;

    /* Major Cycle Progress Bar */

    emit sig_SetMajorCycleProgressbarRange(0, usTotMajorCnt);

    ucChNo = 1;

    for(usMajorCnt = 0; usMajorCnt < usTotMajorCnt ; usMajorCnt++)		/* Majaor Cycle Count Loop*/
    {
        if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts != true)
        {
            p_objMainApp->m_pobjAutoTestCases->bTestExecSts = false;
            emit sig_ChangeStartStopEnDis(0);
            strTemp1 = QString::asprintf("%s", "Current Test Execution Stopped");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp1);
            p_objMainApp->m_pobjAutoTestCases->m_ucTestCnt = 0;
            return;
        }
        if(usMajorCnt != 0)
        {
            sleep((p_objMainApp->m_pobjAutoTestCases->m_uiMajCyclDelay) * 1); /*Delay(ms) between each Majaor Cycle */
        }

        m_pobjReportWrapper->setMajorCycleNo((usMajorCnt+1));

        for(ucTestCnt = 0; ucTestCnt < p_objMainApp->m_pobjAutoTestCases->m_ucTestCnt ; ucTestCnt++)
        {
            if(!p_objMainApp->m_pobjAutoTestCases->bTestExecSts)
            {
                p_objMainApp->m_pobjAutoTestCases->bTestExecSts =false;
                emit sig_ChangeStartStopEnDis(0);
                strTemp1 = QString::asprintf("%s", "Current Test Execution Stopped");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp1);
                p_objMainApp->m_pobjAutoTestCases->m_ucTestCnt = 0;
                return;
            }

            if(ucTestCnt != 0)
            {
                sleep((p_objMainApp->m_pobjAutoTestCases->m_uiDelay) * 1);//Delay between each testcase
            }

            strTestName = p_objMainApp->qsliAutoTestCaseList.at(p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt]-1);

            switch(p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt])
            {

            case DP_APP_RX_RETURN_LOSS_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Input_Return_Loss_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_RFIO_RETURN_LOSS_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Rx_Isolation_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;

            case DP_APP_TX_INPUT_RETURN_LOSS_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Output_Return_Loss_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_RX_GAIN_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Rx_Gain_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_NOISE_FIGURE_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Noise_Figure_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_TX_OUTPUT_POWER_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Tx_Output_Power_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_TX_HARMONICS_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Tx_Harmonics_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_POWER_DROOP_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Power_Droop_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_PULSE_RISE_FALL_TIME_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::RF_Pulse_Rise_Fall_Time_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_POWER_TUNING_MEASUREMENT:

                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Power_Tuning_Measurement, ucChNo))
                {
                    return ;
                }
                break;

              case DP_APP_RX_OUTOFBAND_REJECTION_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Rx_OutOfBand_Rejection_Measuremnt, ucChNo))
                {
                    return ;
                }
                break;

            case DP_APP_PHASENOISE_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::PhaseNoiseMeasurement, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_GAIN_MEASUREMENT_SA:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::RxGainMeasuremnt_SA, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_IMAGE_REJECTION_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::ImageRejection, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_P1dB_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Receiver1dBMeasurement, ucChNo))
                {
                    return ;
                }
                break;            
            case DP_APP_VSWR_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::VSWRMeasurement, ucChNo))
                {
                    return ;
                }
                break;

            case DP_APP_SPURIOUS_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::RxSpuriousMeasuremnt, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_BITE_ISOLATION_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::RFBITEIsolation, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_BLANK_ISOLATION_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::RFBlankIsolation, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_FILTER_REJECTION_MEASUREMENT:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::FilterRejection, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_PHASESHIFTER_CALIBRATION:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Tx_Phaseshifter_Calibration, ucChNo))
                {
                    return ;
                }
                break;
            case DP_APP_PHASESHIFTER_VALIDATION:
                if(PerformTest(p_objMainApp->m_pobjAutoTestCases->m_ucarrCycleCnt[ucTestCnt], usMajorCnt, strTestName,  p_objMainApp->m_pobjAutoTestCases->m_ucarrTestID[ucTestCnt], ucTestCnt, &CAutoTestThread::Tx_Phaseshifter_Validation, ucChNo))
                {
                    return ;
                }
                break;
            default:
                break;
            }/*End of the switch statement*/

        }/*End of testcase loop*/

        emit sig_UpdateMajorProgressBar(usMajorCnt+1);

    }/* End of Major cycle count for loop */
    emit sig_ChangeStartStopEnDis(0);
    p_objMainApp->m_pobjAutoTestCases->bTestExecSts = false;
    p_objMainApp->m_pobjAutoTestCases->m_ucTestCnt = 0;
    return;
}

short CAutoTestThread::PerformTest(unsigned char in_ucTotMinorCyleCnt, unsigned short in_usMajorCnt, QString in_strTestName,  unsigned char in_ucTestID, unsigned char in_ucTestIdx, short (CAutoTestThread::*fnPtr)(bool *, char *), unsigned char in_ucChNo)
{
    bool bTstSts               = 0;
    short sRetVal              = 0;
    QString strTemp1           = "";

    char szTestName[128];
    //    int iRetVal = 0;


    emit sig_SetMinorCycleProgressbarRange(0, in_ucTotMinorCyleCnt);
    for(unsigned char ucCycleCnt = 0; ucCycleCnt < in_ucTotMinorCyleCnt; ucCycleCnt++)
    {
        if(ucCycleCnt != 0)
        {
            sleep(p_objMainApp->m_pobjAutoTestCases->m_uiMinorCyclDelay * 1); // Delay(ms) between each minor cycle
        }

        m_pobjReportWrapper->setMinorCycleNo(ucCycleCnt+1);

        if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts!=true)
        {
            emit sig_ChangeStartStopEnDis(0);
            strTemp1 = QString::asprintf("%s Stopped [Minor Cycle No.: %d, Major Cycle No.: %d]", in_strTestName.toStdString().c_str(), (ucCycleCnt + 1),(in_usMajorCnt + 1));
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp1);
            return DP_SUCCESS;
        }
        /*current cycle number*/
        //  objAutoTestThread->SetCurrCycleNo(ucCycleCnt + 1);
        p_objMainApp->m_pobjAutoTestCases->m_ulOverallProg++;
        strTemp1 = QString::asprintf("%s Started [Minor Cycle No.:%d, Major Cycle No.:%d]", in_strTestName.toStdString().c_str(), (ucCycleCnt + 1), (in_usMajorCnt + 1));
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp1);

        sprintf(szTestName, "%s", in_strTestName.toStdString().c_str());
        bTstSts = false;
        sRetVal = (this->*fnPtr)(&bTstSts, szTestName);
        if(sRetVal)
        {
            if((g_bAutoTestStartStop != true) && (sRetVal == DP_TEST_STOPPED))
            {
                emit sig_ChangeStartStopEnDis(0);
                strTemp1 = QString::asprintf("%s Stopped [Minor Cycle No.:%d, Major Cycle No.:%d]", in_strTestName.toStdString().c_str(), (ucCycleCnt + 1),(in_usMajorCnt + 1));
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp1);
                return DP_SUCCESS;
            }
            else
            {

                ucFailCnt[in_ucTestID - 1]++;
                m_uiAutoTestFailCnt[in_ucTestID-1]++;
                emit sig_UpdateUI(ucSuccessCnt[in_ucTestID - 1], m_uiAutoTestSuccessCnt[in_ucTestID-1], ucFailCnt[in_ucTestID - 1], m_uiAutoTestFailCnt[in_ucTestID-1], in_ucTestID, p_objMainApp->m_pobjAutoTestCases->m_ulOverallProg, ucCycleCnt + 1);
                p_objMainApp->m_pobjAutoTestCases->bTestExecSts = false;
                emit sig_ChangeStartStopEnDis(0);
                strTemp1 = QString::asprintf("%s Aborted due to previous error [Minor Cycle No.:%d, Major Cycle No.:%d]", in_strTestName.toStdString().c_str(), (ucCycleCnt + 1),(in_usMajorCnt + 1));
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp1);
                return DP_SUCCESS;
            }
        }
        if(bTstSts)
        {
            m_uiAutoTestSuccessCnt[in_ucTestID-1]++;
            ucSuccessCnt[in_ucTestID - 1]++;
        }
        else
        {
            m_uiAutoTestFailCnt[in_ucTestID-1]++;
            ucFailCnt[in_ucTestID - 1]++;
        }

        emit sig_UpdateUI(ucSuccessCnt[in_ucTestID - 1], m_uiAutoTestSuccessCnt[in_ucTestID-1], ucFailCnt[in_ucTestID - 1], m_uiAutoTestFailCnt[in_ucTestID-1], in_ucTestID, p_objMainApp->m_pobjAutoTestCases->m_ulOverallProg, ucCycleCnt + 1);

        strTemp1 = QString::asprintf("%s Completed [Minor Cycle count:%d, Major Cycle count:%d] ", in_strTestName.toStdString().c_str(), (ucCycleCnt + 1),(in_usMajorCnt +1));
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp1);
    }

    return DP_SUCCESS;
}

void CAutoTestThread::TestPerformance(unsigned char in_usTestCase, unsigned char in_ucMajorCycle,  unsigned char in_ucMinorCycle, bool in_TestSts)
{
    QStringList qsliTestCaseList;
    QString strTemp = "";
    qsliTestCaseList.clear();
    qsliTestCaseList << "Gain Measurement" <<  "Harmonics Measurement" << "Spurious Measurement" << "1 dB Compression Point Measurement" <<"BITE Power Measurement" << "VSWR Measurement" <<"Noise figure Measurement"  <<"Channel to Channel Isolation" <<"RF to BITE Isolation"<<"RF to Blanking Isolation" ;

    if(g_bAutoTestStartStop != true)
    {
        emit sig_AutoModeGUIEnDis();
        strTemp = QString::asprintf("%s Test Stopped (Minor Cycle count: %d) (Major Cycle count: %d)", qsliTestCaseList[in_usTestCase].toStdString().c_str(), (in_ucMinorCycle + 1),(in_ucMajorCycle + 1));
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp);
        return;
    }

    if(in_TestSts == true)
    {
        strTemp = QString::asprintf("%s Test Failed (Minor Cycle count:%d) (Major Cycle count:%d) ", qsliTestCaseList[in_usTestCase].toStdString().c_str(), (in_ucMinorCycle + 1),(in_ucMajorCycle +1));
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        emit sig_AutoModeGUIUpdate(in_usTestCase, in_TestSts);
    }
    else
    {
        strTemp = QString::asprintf("%s Test Completed Successfully (Minor Cycle count:%d) (Major Cycle count:%d) ", qsliTestCaseList[in_usTestCase].toStdString().c_str(), (in_ucMinorCycle + 1),(in_ucMajorCycle +1));
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strTemp);
        emit sig_AutoModeGUIUpdate(in_usTestCase, in_TestSts);
    }
    qsliTestCaseList.clear();
    return;
}


