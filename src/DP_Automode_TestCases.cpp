#include <QFileDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "../Database/dp_config_struct.h"
#include "../Database/dp_config_read_wrap.h"
#include "mainwindow.h"

extern bool g_bAutoTestStartStop;
extern class CReportConfigWrapper *m_pobjReportWrapper;
extern CConfigParamReadWrapper *m_pobjReadConfigData;
#define _SIMULATION_INSTRUMENTS_
#define _SIMULATION_CONFIG_
extern MainWindow *p_objMainApp;
extern unsigned char g_ucPressYES_NO;
extern U16BIT g_u16InstanceID;
extern U16BIT g_u16PSMInstanceID;
extern unsigned char g_ucFGCount;

short CAutoTestThread ::Rx_Gain_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    U8BIT u8NetworkAnalyzerSel = 1;
    U16BIT u16Trace = 0;
    U8BIT u8Precision = 2;
    S32BIT sRetVal = 0;
    S32BIT s32Delay = 0;
    FDOUBLE dSweepTime = 0.0;
    QString strTemp = "";
    QString strActLog = "";
    QString strPortName = "";
    S8BIT szStateFilepath[512]   = {0};
    QString strStateFilePath    = "D:\\1";

    QString strCalibData = "";
    QStringList strlstCalibData = {""};
    QTextStream objTextStream;
    QFile objCalibCfg;
    QString strFileName = "";
    U16BIT usTrace = 0;
    U8BIT u8NAPortNo               = 1;
    U8BIT u8NAChannelNo               = 1;


    unsigned short in_iMarkerNo		 = DP_APP_ZERO;
    unsigned char in_ucMeasType		 = DP_APP_ZERO;
    double dMarkerFreq = 0.0f;
    double dMarkerValue              = 0.0f;
    unsigned short u16RFPathChNo[2] = {0};
    char szUserMsg[1024]            = {0};
    QString strTestName        = "";
    U32BIT u32TestCaseID       = DP_APP_ZERO;

    S_SBANDGANPAMODULE_RXGAINMEASUREMENT sRxGainMeasParam;
    S_RX_GAIN_MEASUREMENT sGainMeasRpt;
    memset(&sGainMeasRpt, 0, sizeof(S_RX_GAIN_MEASUREMENT));
    *out_bTestSts = true;
    sprintf(szStateFilepath, "%s", strStateFilePath.toStdString().c_str());

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sRxGainMeasParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Tx input return loss Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sprintf(szUserMsg, "Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser(szUserMsg);

    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(sRxGainMeasParam.m_dStartFrequency, sRxGainMeasParam.m_dStopFrequency, sRxGainMeasParam.m_dInputPower, sRxGainMeasParam.m_u16NoofPoints, strStateFilePath,1);
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }
    } // calib end
    try{
#ifndef _SIMULATION_INSTRUMENTS_
        //Recall State file
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NetworkAnalyzerSel, szStateFilepath);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to load state file in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        u16Trace = 3;//S11 trace

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DeleteAllTraces(u8NetworkAnalyzerSel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to delete all traces in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_CreateNewTrace(u8NetworkAnalyzerSel,u8NAPortNo,u16Trace);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to create new trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DisplayTrace(u8NetworkAnalyzerSel,u16Trace,u8NAPortNo,u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to display trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        //Select trace
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NetworkAnalyzerSel, sRxGainMeasParam.m_u16NoofPoints, u16Trace, u8NAPortNo,u8NAChannelNo);//trace, port, channel
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDisplayFormat(u8NetworkAnalyzerSel, DP_LOG_MODE, u8NAChannelNo);  // 1 ch no
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Display format in Network Analyzer");
            throw DP_TEST_FAIL;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Start Frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NetworkAnalyzerSel, (sRxGainMeasParam.m_dStartFrequency * 1e6), u8NAChannelNo);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Start Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Stop frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NetworkAnalyzerSel, (sRxGainMeasParam.m_dStopFrequency * 1e6), u8NAChannelNo);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Stop Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Power level in NA
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NetworkAnalyzerSel, sRxGainMeasParam.m_dInputPower, u8NAChannelNo);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to configure power Level in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set the RF Output Power ON
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NetworkAnalyzerSel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Turn ON RF in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        msleep(500);
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_SINGLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }
        strPortName = "S21";
        for(double dInputfreq = sRxGainMeasParam.m_dStartFrequency; dInputfreq <= sRxGainMeasParam.m_dStopFrequency; dInputfreq = dInputfreq + sRxGainMeasParam.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dMarkerFreq = dInputfreq;
            in_iMarkerNo    = 1;
            in_ucMeasType   = 1;

#ifndef _SIMULATION_INSTRUMENTS_
            //Set Marker Position
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetMarkerPosAndFreq(u8NetworkAnalyzerSel, in_iMarkerNo, dMarkerFreq*1e6, u8NAChannelNo);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to set marker position and Frequency in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_ReadMarkerValue(u8NetworkAnalyzerSel, in_iMarkerNo, in_ucMeasType, u8NAChannelNo, &dMarkerValue);
#else
            sRetVal = DP_APP_ZERO;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to read data at marker position in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            //                dMarkerValue = dMarkerValue * -1;
            sGainMeasRpt.m_dMeasuredGain = dMarkerValue;
            sGainMeasRpt.m_dInputFrequency = dInputfreq;
            sGainMeasRpt.m_dInputPower = sRxGainMeasParam.m_dInputPower;
            sGainMeasRpt.m_dExpMinimumGain = sRxGainMeasParam.m_dExpectedMinGain;
            sGainMeasRpt.m_dExpMaximumGain = sRxGainMeasParam.m_dExpectedMaxGain;

            strActLog = QString::asprintf("Channel Number : %d, Input Frequency : %0.3f MHz , Expected Minimum Gain : %0.2lf dB, Expected Maximum Gain : %0.2lf dB, Measured Gain : %0.2lf dB ",\
                                          sGainMeasRpt.m_u8ChannelNo, sGainMeasRpt.m_dInputFrequency,  sGainMeasRpt.m_dExpMinimumGain, sGainMeasRpt.m_dExpMaximumGain, sGainMeasRpt.m_dMeasuredGain );

            long lMeasGain	= (long)(dproundoff(sGainMeasRpt.m_dMeasuredGain, u8Precision) * pow(10, u8Precision));
            long lExpMinGain 	= (long)(dproundoff(sGainMeasRpt.m_dExpMinimumGain, u8Precision) * pow(10, u8Precision));
            long lExpMaxGain 	= (long)(dproundoff(sGainMeasRpt.m_dExpMaximumGain, u8Precision) * pow(10, u8Precision));

            if(lMeasGain < lExpMinGain || lMeasGain > lExpMaxGain)
            {
                sGainMeasRpt.m_u8Result = 1;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            }
            else
            {
                sGainMeasRpt.m_u8Result = 0;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
            }

            QVectRx_Gain_Measurement.push_back(sGainMeasRpt);
        }
        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NetworkAnalyzerSel);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to Power OFF in the Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        sRetVal = OnSafeState();
        if(sRetVal)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Gain  Measurement Test Completed");

    return DP_SUCCESS;
}

short CAutoTestThread ::Input_Return_Loss_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    QStringList strListDCAData;
    QStringList strListModeData;
    QStringList strListData;
    QString strModeFilePath;
    short sRetVal                    = DP_APP_ZERO;
    unsigned char ucPrecision        = 2;
    U16BIT usTrace                   = DP_APP_ZERO;
    double dMarkerValue              = 0.0f;
    QString strActLog                = "";
    QString strTemp                  = "";
    char szStateFilepath[512]        = {""};
    QString strStateFilepath         = "D:\\1"; /*to be changed*/
    QString strTestName              = "";
    char szUserMsg[1024]             = {0};
    U32BIT u32TestCaseID             = DP_APP_ZERO;
    unsigned short in_iMarkerNo		 = DP_APP_ZERO;
    unsigned char in_ucMeasType		 = DP_APP_ZERO;
    double dMarkerFreq = 0.0f;
    U8BIT u8NASel               = 1;
    U8BIT u8NAPortNo               = 1;
    U8BIT u8NAChannelNo               = 1;

    S_SBANDGANPAMODULE_INPUTRETURNLOSSMEASUREMENT sRxReturnLossMeasIPParam;
    S_INPUT_RETURN_LOSS sRxReturnLossMeasurement;
    *out_bTestSts = true;

    sprintf(szStateFilepath, "%s", strStateFilepath.toStdString().c_str());
    memset(&sRxReturnLossMeasurement, 0.0, sizeof(S_INPUT_RETURN_LOSS));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }
    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sRxReturnLossMeasIPParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Rx input return loss Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sprintf(szUserMsg, "Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser(szUserMsg);
    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(sRxReturnLossMeasIPParam.m_dStartFrequency,sRxReturnLossMeasIPParam.m_dStopFrequency, sRxReturnLossMeasIPParam.m_dInputPower, sRxReturnLossMeasIPParam.m_u16NoofPoints, strStateFilepath,1);
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }
    } // calib end
    try {
#ifndef _SIMULATION_INSTRUMENTS_
        //Recall State file
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NASel, szStateFilepath);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to load STate file in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        usTrace = 1;//S11 trace

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DeleteAllTraces(u8NASel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to delete all traces in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_CreateNewTrace(u8NASel,u8NAPortNo,u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to create new trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DisplayTrace(u8NASel,usTrace,u8NAPortNo,u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to display trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        //Select trace
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NASel, sRxReturnLossMeasIPParam.m_u16NoofPoints, usTrace,u8NAPortNo,u8NAChannelNo);//trace, port, channel
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDisplayFormat(u8NASel, DP_LOG_MODE, usTrace);  // 1 ch no
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Display format in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Start Frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NASel, (sRxReturnLossMeasIPParam.m_dStartFrequency * 1e6), u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Start Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Stop frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NASel, (sRxReturnLossMeasIPParam.m_dStopFrequency * 1e6), u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Stop Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Power level in NA
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NASel, sRxReturnLossMeasIPParam.m_dInputPower, u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Power Level in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set the RF Output Power ON
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NASel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Turn ON RF in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        msleep(500);
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_NA_TRIGGER_MODE_SINGLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        /*Use for max peak freq and data*/

        for(double dInputfreq = sRxReturnLossMeasIPParam.m_dStartFrequency; dInputfreq <= sRxReturnLossMeasIPParam.m_dStopFrequency; dInputfreq = dInputfreq + sRxReturnLossMeasIPParam.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dMarkerFreq = dInputfreq;
            in_iMarkerNo    = 1;
            in_ucMeasType   = 1;


#ifndef _SIMULATION_INSTRUMENTS_
            //Set Marker Position
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetMarkerPosAndFreq(u8NASel, in_iMarkerNo, dMarkerFreq*1e6, usTrace);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to set marker position and Frequency in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_ReadMarkerValue(u8NASel, in_iMarkerNo, in_ucMeasType, usTrace, &dMarkerValue);
#else
            sRetVal = DP_APP_ZERO;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to read data at marker position in Network Analyzer");
                throw DP_TEST_FAIL;
            }


            dMarkerValue = dMarkerValue * -1;

            sRxReturnLossMeasurement.m_dInputFrequency      = dInputfreq;
            sRxReturnLossMeasurement.m_dInputPower          = sRxReturnLossMeasIPParam.m_dInputPower;
            sRxReturnLossMeasurement.m_dExpMaxReturnloss    = sRxReturnLossMeasIPParam.m_dExpectedMaxReturnLoss;
            sRxReturnLossMeasurement.m_dMeasReturnLoss      = dproundoff(dMarkerValue, ucPrecision);

            if(dMarkerValue >= sRxReturnLossMeasurement.m_dExpMaxReturnloss)
            {
                strActLog = QString::asprintf("Channel No : %d, Input Frequency : %lf , Measured Frequency : %lf MHz, Input Power : %lf dBm with Measured Input Return Loss : %lf",sRxReturnLossMeasurement.m_u8ChannelNo,sRxReturnLossMeasurement.m_dInputFrequency, dMarkerFreq, sRxReturnLossMeasIPParam.m_dInputPower, dMarkerValue);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
                sRxReturnLossMeasurement.m_u8Result = DP_TEST_PASS;
            }
            else
            {
                strActLog = QString::asprintf("Channel No : %d,Input Frequency : %lf , Measured Frequency : %lf MHz, Input Power : %lf dBm with Measured Input Return Loss : %lf",sRxReturnLossMeasurement.m_u8ChannelNo,sRxReturnLossMeasurement.m_dInputFrequency, dMarkerFreq, sRxReturnLossMeasIPParam.m_dInputPower, dMarkerValue);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                sRxReturnLossMeasurement.m_u8Result = DP_TEST_FAIL;
                *out_bTestSts = false;
            }

            QVect_InputReturnLoss_Measurement.push_back(sRxReturnLossMeasurement);
        }

        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {

#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NASel);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Power OFF in the Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        }
        sRetVal = OnSafeState();
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    return DP_SUCCESS;
}

short CAutoTestThread ::Output_Return_Loss_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    QStringList strListDCAData;
    QStringList strListModeData;
    QStringList strListData;
    QString strModeFilePath;
    short sRetVal                    = DP_APP_ZERO;
    unsigned char ucPrecision        = 2;
    U16BIT usTrace                   = DP_APP_ZERO;
    double dMarkerValue              = 0.0f;
    QString strActLog                = "";
    QString strTemp                  = "";
    char szStateFilepath[512]        = {""};
    QString strStateFilepath         = "D:\\1"; /*to be changed*/
    QString strTestName              = "";
    char szUserMsg[1024]             = {0};
    U32BIT u32TestCaseID             = DP_APP_ZERO;
    unsigned short in_iMarkerNo		 = DP_APP_ZERO;
    unsigned char in_ucMeasType		 = DP_APP_ZERO;
    double dMarkerFreq = 0.0f;
    U8BIT u8NASel               = 1;
    U8BIT u8NAPortNo               = 1;
    U8BIT u8NAChannelNo               = 1;

    S_SBANDGANPAMODULE_OUTPUTRETURNLOSSMEASUREMENT sTxReturnLossMeasIPParam;
    S_OUTPUT_RETURN_LOSS sTxReturnLossMeasurement;
    *out_bTestSts = true;

    sprintf(szStateFilepath, "%s", strStateFilepath.toStdString().c_str());
    memset(&sTxReturnLossMeasurement, 0.0, sizeof(S_OUTPUT_RETURN_LOSS));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sTxReturnLossMeasIPParam);
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Tx input return loss Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sprintf(szUserMsg, "Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser(szUserMsg);

    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(sTxReturnLossMeasIPParam.m_dStartFrequency, sTxReturnLossMeasIPParam.m_dStopFrequency, sTxReturnLossMeasIPParam.m_dInputPower, sTxReturnLossMeasIPParam.m_u16NoofPoints, strStateFilepath,1);
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }
    } // calib end
    try{
#ifndef _SIMULATION_INSTRUMENTS_
        //Recall State file
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NASel, szStateFilepath);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to load STate file in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        usTrace = 4;


#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DeleteAllTraces(u8NASel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to delete all traces in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_CreateNewTrace(u8NASel,1,usTrace);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to create new trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DisplayTrace(u8NASel,usTrace,u8NAPortNo,u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to display trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        //Select trace
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NASel, sTxReturnLossMeasIPParam.m_u16NoofPoints, usTrace,u8NAPortNo,u8NAChannelNo);//trace, port, channel
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        usleep(100);

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDisplayFormat(u8NASel, DP_LOG_MODE, u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Display format in Network Analyzer");
            throw DP_TEST_FAIL;
        }
        usleep(480);


#ifndef _SIMULATION_INSTRUMENTS_
        //Set Start Frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NASel, (sTxReturnLossMeasIPParam.m_dStartFrequency * 1e6), u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Start Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Stop frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NASel, (sTxReturnLossMeasIPParam.m_dStopFrequency * 1e6), u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Stop Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Power level in NA
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NASel, sTxReturnLossMeasIPParam.m_dInputPower, u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Power Level in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        usleep(50);

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        //Set the RF Output Power ON
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NASel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Turn ON RF in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        msleep(500);
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_NA_TRIGGER_MODE_SINGLE);//continuous
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        for(double dInputfreq = sTxReturnLossMeasIPParam.m_dStartFrequency; dInputfreq <= sTxReturnLossMeasIPParam.m_dStopFrequency; dInputfreq = dInputfreq + sTxReturnLossMeasIPParam.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dMarkerFreq = dInputfreq;
            in_iMarkerNo    = 1;
            in_ucMeasType   = 1;

#ifndef _SIMULATION_INSTRUMENTS_
            //Set Marker Position
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetMarkerPosAndFreq(u8NASel, in_iMarkerNo, dMarkerFreq*1e6, u8NAChannelNo);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to set marker position and Frequency in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_ReadMarkerValue(u8NASel, in_iMarkerNo, in_ucMeasType, u8NAChannelNo, &dMarkerValue);
#else
            sRetVal = DP_APP_ZERO;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to read data at marker position in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            dMarkerValue = dMarkerValue * -1;

            sTxReturnLossMeasurement.m_dInputFrequency      = dInputfreq;
            sTxReturnLossMeasurement.m_dInputPower          = sTxReturnLossMeasIPParam.m_dInputPower;
            sTxReturnLossMeasurement.m_dExpMaxReturnloss    = sTxReturnLossMeasIPParam.m_dExpectedMaxReturnLoss;
            sTxReturnLossMeasurement.m_dMeasReturnLoss      = dproundoff(dMarkerValue, ucPrecision);

            if(dMarkerValue > sTxReturnLossMeasurement.m_dExpMaxReturnloss)
            {
                strActLog = QString::asprintf("Channel No : %d , Input Frequency : %lf , Measured Frequency : %lf MHz, Input Power : %lf dBm with Measured Gain : %lf",sTxReturnLossMeasurement.m_u8ChannelNo, sTxReturnLossMeasurement.m_dInputFrequency, dMarkerFreq, sTxReturnLossMeasIPParam.m_dInputPower, dMarkerValue);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
                sTxReturnLossMeasurement.m_u8Result = DP_TEST_PASS;
            }
            else
            {
                strActLog = QString::asprintf("Channel No : %d , Input Frequency : %lf , Measured Frequency : %lf MHz, Input Power : %lf dBm with Measured Gain : %lf",sTxReturnLossMeasurement.m_u8ChannelNo, sTxReturnLossMeasurement.m_dInputFrequency, dMarkerFreq, sTxReturnLossMeasIPParam.m_dInputPower, dMarkerValue);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                sTxReturnLossMeasurement.m_u8Result = DP_TEST_FAIL;
                *out_bTestSts = false;
            }

            QVect_OutputReturnLoss_Measurement.push_back(sTxReturnLossMeasurement);
        }
        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NASel);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Power OFF in the Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        }
        sRetVal = OnSafeState();
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    return DP_SUCCESS;
}

short CAutoTestThread ::Rx_Isolation_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    U8BIT u8NetworkAnalyzerSel = 1;
    U16BIT u16Trace = 0;
    U8BIT u8Precision = 2;
    S32BIT sRetVal = 0;
    S32BIT s32Delay = 0;
    FDOUBLE dSweepTime = 0.0;
    QString strTemp = "";
    QString strActLog = "";
    QString strPortName = "";
    S8BIT szStateFilepath[512]   = {0};
    QString strStateFilePath    = "D:\\1";

    QString strCalibData = "";
    QStringList strlstCalibData = {""};
    QTextStream objTextStream;
    QFile objCalibCfg;
    QString strFileName = "";
    U16BIT usTrace = 0;

    unsigned short in_iMarkerNo		 = DP_APP_ZERO;
    unsigned char in_ucMeasType		 = DP_APP_ZERO;
    double dMarkerFreq = 0.0f;
    double dMarkerValue              = 0.0f;
    unsigned short u16RFPathChNo[2] = {0};
    char szUserMsg[1024]            = {0};
    QString strTestName        = "";
    U32BIT u32TestCaseID       = DP_APP_ZERO;

    double dMeasuredGain[9];

    S_SBANDGANPAMODULE_RXISOLATIONMEASUREMENT sRxIsoMeasParam;
    S_RX_ISOLATION_MEASUREMENT sIsoMeasRpt;
    memset(&sIsoMeasRpt, 0, sizeof(S_RX_ISOLATION_MEASUREMENT));
    *out_bTestSts = true;
    sprintf(szStateFilepath, "%s", strStateFilePath.toStdString().c_str());

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sRxIsoMeasParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Tx input return loss Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sprintf(szUserMsg, "Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser(szUserMsg);

    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(sRxIsoMeasParam.m_dStartFrequency, sRxIsoMeasParam.m_dStopFrequency, sRxIsoMeasParam.m_dInputPower, sRxIsoMeasParam.m_u16NoofPoints, strStateFilePath,1);
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }
    } // calib end
    try{
#ifndef _SIMULATION_INSTRUMENTS_
        //Recall State file
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NetworkAnalyzerSel, szStateFilepath);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to load state file in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        u16Trace = 3;//S11 trace

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DeleteAllTraces(u8NetworkAnalyzerSel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to delete all traces in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_CreateNewTrace(u8NetworkAnalyzerSel,1,u16Trace);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to create new trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DisplayTrace(u8NetworkAnalyzerSel,u16Trace,1,1);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to display trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        //Select trace
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NetworkAnalyzerSel, sRxIsoMeasParam.m_u16NoofPoints, u16Trace, 1, 1);//trace, port, channel
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDisplayFormat(u8NetworkAnalyzerSel, DP_LOG_MODE, 1);  // 1 ch no
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Display format in Network Analyzer");
            throw DP_TEST_FAIL;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Start Frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NetworkAnalyzerSel, (sRxIsoMeasParam.m_dStartFrequency * 1e6), 1);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Start Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Stop frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NetworkAnalyzerSel, (sRxIsoMeasParam.m_dStopFrequency * 1e6), 1);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Stop Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Power level in NA
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NetworkAnalyzerSel, sRxIsoMeasParam.m_dInputPower, 1);//Channel number
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to configure power Level in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set the RF Output Power ON
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NetworkAnalyzerSel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Turn ON RF in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        msleep(500);
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_SINGLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        strTemp = QString::asprintf("Idle Mode");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp);

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }
        msleep(500);
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_SINGLE);//continuous
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }
        int iSpot = 0;

        for(double dInputfreq = sRxIsoMeasParam.m_dStartFrequency; dInputfreq <= sRxIsoMeasParam.m_dStopFrequency; dInputfreq = dInputfreq + sRxIsoMeasParam.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dMarkerFreq = dInputfreq;
            in_iMarkerNo    = 1;
            in_ucMeasType   = 1;

#ifndef _SIMULATION_INSTRUMENTS_
            //Set Marker Position
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetMarkerPosAndFreq(u8NetworkAnalyzerSel, in_iMarkerNo, dMarkerFreq*1e6, 1);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to set marker position and Frequency in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            msleep(100);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_ReadMarkerValue(u8NetworkAnalyzerSel, in_iMarkerNo, in_ucMeasType, 1, &dMarkerValue);
#else
            sRetVal = DP_APP_ZERO;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to read data at marker position in Network Analyzer");
                throw DP_TEST_FAIL;
            }
            dMeasuredGain[iSpot] = dMarkerValue;
            strActLog = QString::asprintf("Channel Number : %d, Input Frequency : %0.3f MHz , Measured Gain in Idle Mode : %0.2lf dB ",sIsoMeasRpt.m_u8ChannelNo, dInputfreq, dMeasuredGain[iSpot] );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strActLog);
            iSpot++;
            //QVectRxIsolation_Measurement.push_back(sIsoMeasRpt);
        }

        strTemp = QString::asprintf("Rx Mode Running");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp);
        iSpot = 0;

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }
        msleep(200);
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_SINGLE);//continuous
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        for(double dInputfreq = sRxIsoMeasParam.m_dStartFrequency; dInputfreq <= sRxIsoMeasParam.m_dStopFrequency; dInputfreq = dInputfreq + sRxIsoMeasParam.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dMarkerFreq = dInputfreq;
            in_iMarkerNo    = 1;
            in_ucMeasType   = 1;

#ifndef _SIMULATION_INSTRUMENTS_
            //Set Marker Position
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetMarkerPosAndFreq(u8NetworkAnalyzerSel, in_iMarkerNo, dMarkerFreq*1e6, 1);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to set marker position and Frequency in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            msleep(200);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_ReadMarkerValue(u8NetworkAnalyzerSel, in_iMarkerNo, in_ucMeasType, 1, &dMarkerValue);
#else
            sRetVal = DP_APP_ZERO;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to read data at marker position in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            sIsoMeasRpt.m_dMeasureGainIdleMode = dMeasuredGain[iSpot];
            sIsoMeasRpt.m_dMeasureGainRxMode = dMarkerValue;
            sIsoMeasRpt.m_dInputFrequency = dInputfreq;
            sIsoMeasRpt.m_dInputPower = sRxIsoMeasParam.m_dInputPower;
            sIsoMeasRpt.m_dExpIsolation = sRxIsoMeasParam.m_dExpMaxIsolation;

            sIsoMeasRpt.m_dCalcIsolation = dMeasuredGain[iSpot] - dMarkerValue;
            if(sIsoMeasRpt.m_dCalcIsolation < sIsoMeasRpt.m_dExpIsolation)
            {
                strActLog = QString::asprintf("Channel Number : %d,Input Frequency : %0.3f MHz , Measured Gain in Rx Mode : %0.2lf dB , Calculated isolation : %f dB",sIsoMeasRpt.m_u8ChannelNo, sIsoMeasRpt.m_dInputFrequency,sIsoMeasRpt.m_dMeasureGainRxMode,sIsoMeasRpt.m_dCalcIsolation);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
            }
            else
            {
                strActLog = QString::asprintf("Channel Number : %d,Input Frequency : %0.3f MHz , Measured Gain in Rx Mode : %0.2lf dB , Calculated isolation : %f dB",sIsoMeasRpt.m_u8ChannelNo, sIsoMeasRpt.m_dInputFrequency,sIsoMeasRpt.m_dMeasureGainRxMode,sIsoMeasRpt.m_dCalcIsolation);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            }

            QVectRxIsolation_Measurement.push_back(sIsoMeasRpt);
            iSpot++;
        }

        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NetworkAnalyzerSel);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to Power OFF in the Network Analyzer");
            throw DP_TEST_FAIL;
        }

        sRetVal = OnSafeState();
        if(sRetVal)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Rx Isolation Test Completed");

    return DP_SUCCESS;
}

short CAutoTestThread ::Tx_Output_Power_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    short sRetVal								= DP_APP_ZERO;
    double dPwrMtr_XStart						= DP_APP_ZERO;
    double dPwrMtr_Xscalestep							= DP_APP_ZERO;
    double dPwrMtr_YScale						= DP_APP_ZERO;
    double dPwrMtr_Trigger						= DP_APP_ZERO;
    double dPwrMtr_YScalestep					= DP_APP_ZERO;
    double dPwrMtr_Marker_1						= DP_APP_ZERO;
    double dPwrMtr_Marker_2						= DP_APP_ZERO;
    double dRFFreq								= DP_APP_ZERO;
    double dMeasPwrMarker						= DP_APP_ZERO;
    U8BIT u8PMSel               = 1;
    U8BIT u8SGSel               = 1;
    QStringList qslstTRPselection    ;
    QStringList qslstTRPconfig    ;
    QStringList qslstSBOBconfig    ;
    QTextStream objFileStream;
    QStringList qstrList;
    QString qsTemp                              ="";
    QString qsTempMsg                           = "";
    QStringList qsPulseconfig;
    QFile qfConfigFile;
    QStringList qsPulsewidconfig;
    QStringList qsChannellist;
    QString cErrCode                            = "key not found";
    QStringList qlistrReportData;
    QString strTestName        = "";
    U32BIT u32TestCaseID       = DP_APP_ZERO;
    QString strActLog          = "";
    unsigned short u16RFPathChNo[2] = {0};
    QString strTemp            = "";
    U8BIT u8Address             = 1;
    FSINGLE fVoltage            = 0.0;
    FSINGLE fCurrent            = 0.0;
    double dCalibPower;
    double dSGinPower;
    double m_dMeasPathLoss;

    S_SBANDGANPAMODULE_TXOUTPUTPOWERMEASUREMENT STxOutputPow;
    S_TX_OUTPUT_POWER_MEASUREMENT sOutputPowMeasReport;
    *out_bTestSts = true;
    memset(&sOutputPowMeasReport,DP_APP_ZERO,sizeof (S_TX_OUTPUT_POWER_MEASUREMENT));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID,1,&STxOutputPow);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Tx Output Power Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    dPwrMtr_YScale = STxOutputPow.m_dYScale;
    dPwrMtr_YScalestep = STxOutputPow.m_dYMax;
    dPwrMtr_Trigger = -10;
    dPwrMtr_XStart = STxOutputPow.m_dXStart * 1e3;
    dPwrMtr_Xscalestep = STxOutputPow.m_dXScaleStep * 1e3;
    dPwrMtr_Marker_1 = STxOutputPow.m_dMarker1;
    dPwrMtr_Marker_2 = STxOutputPow.m_dMarker2;

#ifndef _SIMULATION_INSTRUMENTS_
    /* SigGen Preset for RF */
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SGSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to preset signal Generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /*Config Power meter*/
    sRetVal = p_objMainApp->m_objPMWrap.DP_PMeter_Preset(u8PMSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to preset the Power Meter");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    /*Function to preset function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Reset(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /*Config Power meter*/
    sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_Config(u8PMSel,3.1,0,(dPwrMtr_XStart), (dPwrMtr_Xscalestep/1e6), dPwrMtr_YScalestep, dPwrMtr_YScale, dPwrMtr_Trigger, dPwrMtr_Marker_1, dPwrMtr_Marker_2);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to configure the Power Meter");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    msleep(200);



#ifndef _SIMULATION_CONFIG_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Config(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance,DP_FG_CH1,DP_FG_WAVEFORM_PULSE_SIGLENT,STxOutputPow.m_dPulsePeriod,STxOutputPow.m_dPulseWidth,STxOutputPow.m_dHighLevel, STxOutputPow.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Configure Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    /*Function to configure waveform shape in function generator for all channels*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_ConfigureWaveformShape(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_WAVEFORM_PULSE_SIGLENT);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulse(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1,0/*in_dDutyCycle*/, STxOutputPow.m_dPulsePeriod);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulseWidth(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, STxOutputPow.m_dPulseWidth);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    /*Function to configure impedance in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_SetImpedance(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_IMPEDANCE_INFINITY, DP_FG_IMPEDANCE_AUTO);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Set Impedance in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgHighLowLvl(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, STxOutputPow.m_dHighLevel, STxOutputPow.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Configure Offset voltage Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    /*Function to enable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_EN);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Enable Output Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    try{
        for(double dInputfreq = STxOutputPow.m_dStartFrequency; dInputfreq <= STxOutputPow.m_dStopFrequency; dInputfreq = dInputfreq + STxOutputPow.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dRFFreq = dInputfreq;

#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(1, dInputfreq , STxOutputPow.m_dInputPower, &dSGinPower, &m_dMeasPathLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the RF stimulus path calibration data ");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strActLog);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF( u8SGSel, (dRFFreq * 1.0e6),  dSGinPower/*STxOutputPow.m_dInputPower*/); //add cable loss
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to configure frequency and power level in signal Generator");
                throw DP_TEST_FAIL;
            }
            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled( u8SGSel, DP_ATE_RFOUT_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Signal Generator  RF OUT Failed");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            msleep(100);
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PwrMtr_ReadPeakPower(u8PMSel,&dMeasPwrMarker);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to Read Output Power in RF Power Meter for Signal Power Measurement Test");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, DP_ATE_RFOUT_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Signal Generator 1 RF OFF Failed");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = ApplyCalibData(2, dRFFreq , dMeasPwrMarker, &dCalibPower, &sOutputPowMeasReport.m_dOutputCableLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Signal Generator 1 RF OFF Failed");
                throw DP_TEST_FAIL;
            }

            sOutputPowMeasReport.m_dRFfreq = dInputfreq;
            sOutputPowMeasReport.m_dInputPower = STxOutputPow.m_dInputPower;
            sOutputPowMeasReport.m_dExpectedOutputPower = STxOutputPow.m_dExpMaxOutPower;
            sOutputPowMeasReport.m_dMeasuredOutputPower = dMeasPwrMarker;
            sOutputPowMeasReport.m_dMeasPowerWatt = dpdbmtoWatt(dCalibPower);
            sOutputPowMeasReport.m_dCalculatedOutputPower = dCalibPower;

            if((dCalibPower > (STxOutputPow.m_dExpMinOutPower)))
            {
                strActLog = QString::asprintf("Channel No : %d, Frequency : %f MHz,Observed Power:%0.4lf dBm, Calculated Power : %0.4lf dBm, Measured Power in Watts : %lf", sOutputPowMeasReport.m_u8ChannelNo, sOutputPowMeasReport.m_dRFfreq, sOutputPowMeasReport.m_dMeasuredOutputPower,dCalibPower, sOutputPowMeasReport.m_dMeasPowerWatt);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
                sOutputPowMeasReport.m_u8Result = DP_TEST_PASS;
            }
            else
            {
                strActLog = QString::asprintf("Channel No : %d, Frequency : %f MHz,Observed Power:%0.4lf dBm, Calculated Power : %0.4lf dBm, Measured Power in Watts : %lf", sOutputPowMeasReport.m_u8ChannelNo, sOutputPowMeasReport.m_dRFfreq, sOutputPowMeasReport.m_dMeasuredOutputPower,dCalibPower, sOutputPowMeasReport.m_dMeasPowerWatt);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                sOutputPowMeasReport.m_u8Result = DP_TEST_FAIL;
                *out_bTestSts = false;
            }
            QVectTx_OutputPower_Measurement.push_back(sOutputPowMeasReport);
        }
        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, DP_ATE_RFOUT_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s","Unable to disable signal Generator");
            throw DP_TEST_FAIL;
        }
        /*Function to Disable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_DIS);
#else
        sRetVal = 0;
#endif
        if(sRetVal != 0)
        {
            *out_bTestSts = true;
            strTemp = QString::asprintf("%s","Unable to Enable Output Function generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        sRetVal = OnSafeState();
        if(sRetVal)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    return DP_SUCCESS;
}

short CAutoTestThread ::Tx_Harmonics_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    QStringList strListModeData;
    QStringList strListData;
    QString strModeFilePath;
    U32BIT u32TestCaseID            = DP_APP_ZERO;
    int u32CfgNumber                = DP_APP_ZERO;
    short sRetVal				    = DP_APP_ZERO;
    double dHarAmpl[10]			    = {DP_APP_ZERO};
    double dSAoutPower			    = DP_APP_ZERO;
    double dMeasPwr                 = DP_APP_ZERO;
    double dSGinPower               = DP_APP_ZERO;
    QString qsTempMsg			    = "";
    QString strTestName             = "";
    double dOutputCableLoss         = 0.0;
    QString strActLog                = "";
    U8BIT u8SGSel               = 1;
    U8BIT u8SASel               = 1;
    unsigned short u16RFPathChNo[2] = {0};

    S_SBANDGANPAMODULE_TXHARMONICSMEASUREMENT sHarmonicRejectionIPParam;
    S_HARMONIC_MEASUREMENT sHarmonicRejectionReport;
    *out_bTestSts = true;
    memset(&sHarmonicRejectionReport, DP_APP_ZERO, sizeof(S_HARMONIC_MEASUREMENT));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }
    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    u32CfgNumber  = 1;
    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, u32CfgNumber, &sHarmonicRejectionIPParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Rx input return loss Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }


#ifndef _SIMULATION_INSTRUMENTS_
    //SA Noise Figure Mode
    sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ModeConfig(u8SASel, 1);
#else
    sRetVal = 0 ;
#endif
    if(sRetVal)
    {
        strActLog = QString::asprintf("%s","Unable to configure Noise Figure mode in spectrum analyzer for Noise Figure measurement ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Preset in RF SG
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SGSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strActLog = QString::asprintf("%s", "Unable to Configure Preset in RF Signal Generator ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(u8SASel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strActLog = QString::asprintf("%s","Unable to Configure Preset in RF Signal Analyzer ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    /*Function to preset function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Reset(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strActLog = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    /*Function to configure waveform shape in function generator for all channels*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_ConfigureWaveformShape(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_WAVEFORM_PULSE_SIGLENT);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strActLog = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulse(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1,0/*in_dDutyCycle*/, sHarmonicRejectionIPParam.m_dPulsePeriod);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strActLog = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulseWidth(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, sHarmonicRejectionIPParam.m_dPulseWidth);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strActLog = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }
    /*Function to configure impedance in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_SetImpedance(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_IMPEDANCE_INFINITY, DP_FG_IMPEDANCE_AUTO);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strActLog = QString::asprintf("%s","Unable to Set Impedance in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgHighLowLvl(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, sHarmonicRejectionIPParam.m_dHighLevel,sHarmonicRejectionIPParam.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strActLog = QString::asprintf("%s","Unable to Configure Offset voltage Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    /*Function to enable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_EN);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strActLog = QString::asprintf("%s","Unable to Enable Output Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    try{
        for(double dInputfreq = sHarmonicRejectionIPParam.m_dStartFrequency; dInputfreq <= sHarmonicRejectionIPParam.m_dStopFrequency; dInputfreq = dInputfreq + sHarmonicRejectionIPParam.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(sHarmonicRejectionIPParam.m_u16RFInputPathID, dInputfreq , sHarmonicRejectionIPParam.m_dInputPower, &dSGinPower, &sHarmonicRejectionReport.m_dMeasPathLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the RF stimulus path calibration data ");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strActLog);
                return DP_FAILURE;
            }

            // Configure SigGen freq and power for RF
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SGSel, dInputfreq *1e6, dSGinPower);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure frequency and power level in signal generator");
                throw DP_TEST_FAIL;
            }

            /*Set Span to 0 */
#ifndef _SIMULATION_INSTRUMENTS_
            // Config Center Freq and Span in SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SASel, dInputfreq *1e6, sHarmonicRejectionIPParam.m_dSpan *1e9);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure center frequency and span in signal analyzer");
                throw DP_TEST_FAIL;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            // Config  RBW , VBW in SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SASel, 0, sHarmonicRejectionIPParam.m_dRBW * 1e3, 0, sHarmonicRejectionIPParam.m_dVBW * 1e3, 0, sHarmonicRejectionIPParam.m_dSweepTime);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure RBW, VBW and sweep time in signal analyzer");
                throw DP_TEST_FAIL;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            // Config Ref Level In SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SASel, sHarmonicRejectionIPParam.m_dReferenceLevel);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure reference level in signal analyzer");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigVideoTrigger(u8SASel,sHarmonicRejectionIPParam.m_dVideoTrigLevel/*-10*/,0,0);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure reference level in signal analyzer");
                throw DP_TEST_FAIL;
            }
            msleep(500);
#ifndef _SIMULATION_INSTRUMENTS_
            // Enable Rf out in RF SG
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, true);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to enable RF output in RF signal generator ");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Enable continuous run
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SASel, DP_CNT_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer ");
                throw DP_TEST_FAIL;
            }
            msleep(500);
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SASel, DP_CNT_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer ");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Read MArker Frequency
            sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SASel, sHarmonicRejectionIPParam.m_dMarkerPos*1e-1, &dMeasPwr);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to read marker frequency in spectrum analyzer for Harmonics Measurement");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib DATA for SA
            sRetVal = ApplyCalibData(sHarmonicRejectionIPParam.m_u16RFOutputPathID, dInputfreq, dMeasPwr, &dSAoutPower, &dOutputCableLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the SA path calibration data for Harmonics Measurement");
                throw DP_TEST_FAIL;
            }

            //Read Harmonic Value
            dHarAmpl[0]= dSAoutPower;

            /////////////////////////////////////Configure Second Harmonic frequency///////////////////////////////////////////////

#ifndef _SIMULATION_INSTRUMENTS_
            // Config Center Freq and Span in SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SASel, (2 * 1e6* dInputfreq), sHarmonicRejectionIPParam.m_dSpan *1e9);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure center frequency and span in signal analyzer");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Config  RBW , VBW in SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SASel, 0, 3/*sHarmonicRejectionIPParam.m_dRBW*/ * 1e6, 0, /*sHarmonicRejectionIPParam.m_dVBW*/ 1* 1e6, 0, sHarmonicRejectionIPParam.m_dSweepTime);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure RBW, VBW and sweep time in signal analyzer");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Config Ref Level In SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SASel, sHarmonicRejectionIPParam.m_dReferenceLevel2ndOrder);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure reference level Harmonics Measurement ");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigVideoTrigger(u8SASel,sHarmonicRejectionIPParam.m_dVideoTrigLevel2ndOrder/*-45*/,0,0);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure reference level in signal analyzer");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Enable continuous run
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SASel, DP_CNT_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer ");
                throw DP_TEST_FAIL;
            }


#ifndef _SIMULATION_INSTRUMENTS_
            msleep(500);
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SASel, DP_CNT_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer ");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Read MArker Frequency
            sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SASel, sHarmonicRejectionIPParam.m_dMarkerPos*1e-1, &dMeasPwr);
#else
            sRetVal = 0;
#endif

            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to read marker frequency in spectrum analyzer for Harmonics Measurement");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib DATA for SA
            sRetVal = ApplyCalibData(sHarmonicRejectionIPParam.m_u16RFOutputPathID, (2 * dInputfreq), dMeasPwr, &dSAoutPower, &dOutputCableLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the SA path calibration data for harmonics Measurement");
                throw DP_TEST_FAIL;
            }

            //Read Harmonic Value
            dHarAmpl[1] = dSAoutPower;

            //        msleep(1000);

            /////////////////////////////////////Configure third Harmonic frequency///////////////////////////////////////////////

#ifndef _SIMULATION_INSTRUMENTS_
            // Config Center Freq and Span in SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SASel, (3 * 1e6 * dInputfreq), sHarmonicRejectionIPParam.m_dSpan *1e9);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure center frequency and span in Harmonics Measurement ");
                throw DP_TEST_FAIL;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigVideoTrigger(u8SASel,sHarmonicRejectionIPParam.m_dVideoTrigLevel3rdOrder,0,0);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to configure reference level in signal analyzer");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Enable continuous run
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SASel, DP_CNT_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer ");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            msleep(500);
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SASel, DP_CNT_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer ");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Read MArker Frequency
            sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SASel, sHarmonicRejectionIPParam.m_dMarkerPos*1e-1, &dMeasPwr);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to read marker frequency in spectrum analyzer for Harmonics Measurement");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib DATA for SA
            sRetVal = ApplyCalibData(sHarmonicRejectionIPParam.m_u16RFOutputPathID, (3 * dInputfreq), dMeasPwr, &dSAoutPower, &dOutputCableLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the SA path calibration data for harmonics Measurement");
                throw DP_TEST_FAIL;
            }

            //Read Harmonic Value
            dHarAmpl[2] = dSAoutPower;
            msleep(100);

#ifndef _SIMULATION_INSTRUMENTS_
            // Disable RF Out in SG
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, false);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to disable RF output in RF signal generator for harmonics Measurement");
                throw DP_TEST_FAIL;
            }

            sHarmonicRejectionReport.m_dInputfreq = dInputfreq;
            sHarmonicRejectionReport.m_d2ndHarmFreq = 2 * dInputfreq;
            sHarmonicRejectionReport.m_d3rdHarmFreq = 3 * dInputfreq;
            sHarmonicRejectionReport.m_dInputPower = sHarmonicRejectionIPParam.m_dInputPower;
            sHarmonicRejectionReport.m_dExpMin2ndHarmonicRejection = sHarmonicRejectionIPParam.m_dExpectedHarmonicsRejection;
            sHarmonicRejectionReport.m_dExpMin3rdHarmonicRejection = sHarmonicRejectionIPParam.m_dExpectedHarmonicsRejection;
            sHarmonicRejectionReport.m_dFundamentalPwr = dproundoff(dHarAmpl[0], 2);
            sHarmonicRejectionReport.m_d2ndHarmPwr = dproundoff(dHarAmpl[1], 2);
            sHarmonicRejectionReport.m_d3rdHarmPwr = dproundoff(dHarAmpl[2], 2);
            sHarmonicRejectionReport.m_d2ndHarmonicRejection = dproundoff((sHarmonicRejectionReport.m_dFundamentalPwr - dHarAmpl[1]), 2);
            sHarmonicRejectionReport.m_d3rdHarmonicRejection = dproundoff((sHarmonicRejectionReport.m_dFundamentalPwr - dHarAmpl[2]), 2);

            qsTempMsg = QString::asprintf("Channel No: %d, Input Freqency: %0.2f MHz, Fundamental Power:%0.2f dBm,2nd Harmonics Power:%0.2f dBm,3rd Harmonics Power:%0.2f dBm, Exp. Harmonics Rejection: %0.2f dBc, Measured 2nd Harmonics Rejection: %0.2f dBc, Measured 3rd Harmonics Rejection: %0.2f dBc", sHarmonicRejectionReport.m_u8ChannelNo, dInputfreq, sHarmonicRejectionReport.m_dFundamentalPwr,sHarmonicRejectionReport.m_d2ndHarmPwr,sHarmonicRejectionReport.m_d3rdHarmPwr, sHarmonicRejectionReport.m_dExpMin2ndHarmonicRejection, sHarmonicRejectionReport.m_d3rdHarmonicRejection, sHarmonicRejectionReport.m_d3rdHarmonicRejection);

            if(sHarmonicRejectionReport.m_d2ndHarmonicRejection > sHarmonicRejectionReport.m_dExpMin2ndHarmonicRejection)
            {
                sHarmonicRejectionReport.m_u8Result = DP_TEST_PASS;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, qsTempMsg);
            }
            else
            {
                *out_bTestSts = false;
                sHarmonicRejectionReport.m_u8Result = DP_TEST_FAIL;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, qsTempMsg);
            }

            if(g_bAutoTestStartStop == false)
            {
                sHarmonicRejectionReport.m_u8Result  = 2;
                QVect_Harmonics_Measurement.push_back(sHarmonicRejectionReport);
                sRetVal = OnSafeState();
                if(sRetVal)
                {
                    strActLog = QString::asprintf("%s","Unable to insert to database");
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                    return DP_FAILURE;
                }
                qsTempMsg = QString::asprintf("Harmonics Measurement Test stopped");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, qsTempMsg);
                return DP_TEST_STOPPED;
            }

            QVect_Harmonics_Measurement.push_back(sHarmonicRejectionReport);
        }
        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        // Enable Rf out in RF SG
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, false);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strActLog = QString::asprintf("%s","Unable to disable RF output in RF signal generator ");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }
        /*Function to Disable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_DIS);
#else
        sRetVal = 0;
#endif
        if(sRetVal != 0)
        {
            *out_bTestSts = true;
            strActLog = QString::asprintf("%s","Unable to Enable Output Function generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }
        sRetVal = OnSafeState();
        if(sRetVal)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    qsTempMsg = QString::asprintf("Harmonics Measurement Test Completed");
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, qsTempMsg);

    return DP_SUCCESS;
}

short CAutoTestThread ::RF_Pulse_Rise_Fall_Time_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    short sRetVal								= DP_APP_ZERO;
    unsigned char ucChnLoop						= DP_APP_ZERO;
    double dPwrMtr_RT_XStart					= DP_APP_ZERO;
    double dPwrMtr_RT_XScale					= DP_APP_ZERO;
    double dPwrMtr_RT_YStart					= DP_APP_ZERO;
    double dPwrMtr_RT_YScale					= DP_APP_ZERO;
    double dPwrMtr_FT_XStart					= DP_APP_ZERO;
    double dPwrMtr_FT_XScale					= DP_APP_ZERO;
    double dPwrMtr_FT_YStart					= DP_APP_ZERO;
    double dPwrMtr_FT_YScale					= DP_APP_ZERO;
    double dPwrMtr_Trigger						= DP_APP_ZERO;
    double dPwrMtr_Marker_1						= DP_APP_ZERO;
    double dPwrMtr_Marker_2						= DP_APP_ZERO;
    double dExpRTFT								= DP_APP_ZERO;
    double dMeasRiseTime						= DP_APP_ZERO;
    double dMeasFallTime						= DP_APP_ZERO;
    double dSGinPower;
    double m_dMeasPathLoss;
    double m_dMeasRise[100] = {0};
    double m_dExpRise[150];
    QStringList qslstTRPselection    ;
    QStringList qslstTRPconfig    ;
    QStringList qslstSBOBconfig    ;
    unsigned short uscount = 0;

    U8BIT u8SGSel               = 1;
    U8BIT u8PMSel               = 1;

    QString qsTemp                              = "";
    QString qsTempMsg = "";
    QTextStream objFileStream;
    QStringList qstrList;
    QStringList qsChannellist;

    unsigned short u16RFPathChNo[2] = {0};
    QString strTemp                  = "";
    QString strActLog                = "";
    U32BIT u32TestCaseID             = DP_APP_ZERO;
    QString strTestName              = "";
    QString qsConfigFilepath;
    QString qsConfigcsvFilepath;
    QStringList qsPulseconfig;
    QFile qfConfigFile;
    QStringList qsPulsewidconfig;
    QString cErrCode							= "Key Not Found";
    QStringList qlistrReportData;
    S_SBANDGANPAMODULE_RFPULSERISEORFALLTIMETEST STxPulseRTFT;
    S_RF_PULSE_RISE_FALL_TIME_MEAS sPulseRiseFallTimeMeasReport;

    memset(&sPulseRiseFallTimeMeasReport, DP_APP_ZERO, sizeof(S_RF_PULSE_RISE_FALL_TIME_MEAS));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &STxPulseRTFT);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Rx input return loss Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    dPwrMtr_RT_XStart = STxPulseRTFT.m_dXStartRT * 1e-3;
    dPwrMtr_RT_XScale = STxPulseRTFT.m_dXScaleRT * 1e-9;
    dPwrMtr_RT_YStart = STxPulseRTFT.m_dYMaxRT;
    dPwrMtr_RT_YScale = STxPulseRTFT.m_dYScaleRT;

    dPwrMtr_FT_XStart = STxPulseRTFT.m_dXStartFT * 1e3;
    dPwrMtr_FT_XScale = STxPulseRTFT.m_dXScaleFT * 1e-9;
    dPwrMtr_FT_YScale = STxPulseRTFT.m_dYMaxFT;
    dPwrMtr_FT_YStart = STxPulseRTFT.m_dYScaleFT;

    dPwrMtr_Trigger = -10;
    dPwrMtr_Marker_1 = 0;
    dPwrMtr_Marker_2 = 0;

    /* SigGen Preset for RF */
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SGSel);	//siggen1
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#else
    sRetVal = 0;
#endif

#ifndef _SIMULATION_INSTRUMENTS_
    /*Config Power meter*/
    sRetVal = p_objMainApp->m_objPMWrap.DP_PMeter_Preset(u8PMSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to preset the Power Meter");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }


    /* Tx Pulse RiseTime Measurement */
#ifndef _SIMULATION_INSTRUMENTS_
    /*Config Power meter*/
    sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_Config(u8PMSel,3.1,0,(dPwrMtr_RT_XStart), dPwrMtr_RT_XScale, dPwrMtr_RT_YStart, dPwrMtr_RT_YScale, dPwrMtr_Trigger, dPwrMtr_Marker_1, dPwrMtr_Marker_2);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to configure the Power Meter");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    msleep(200);
    /*Function to preset function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Reset(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#if 0
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Config(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance,DP_FG_CH1,DP_FG_WAVEFORM_PULSE_SIGLENT,sPowerDroopIPParam.m_dPulsePeriod,sPowerDroopIPParam.m_dPulseWidth,sPowerDroopIPParam.m_dHighLevel, sPowerDroopIPParam.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Configure Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#if 1
    /*Function to configure waveform shape in function generator for all channels*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_ConfigureWaveformShape(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_WAVEFORM_PULSE_SIGLENT);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulse(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1,0/*in_dDutyCycle*/, STxPulseRTFT.m_dPulsePeriod);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulseWidth(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, STxPulseRTFT.m_dPulseWidth);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    /*Function to configure impedance in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_SetImpedance(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_IMPEDANCE_INFINITY, DP_FG_IMPEDANCE_AUTO);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Set Impedance in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgHighLowLvl(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, STxPulseRTFT.m_dHighLevel, STxPulseRTFT.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Configure Offset voltage Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#endif

    /*Function to enable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_EN);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Enable Output Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    try{

        strTemp = QString::asprintf("Rise Time Test in Progress");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO,strTemp);
        for(double dInputfreq = STxPulseRTFT.m_dStartFrequency; dInputfreq <= STxPulseRTFT.m_dStopFrequency; dInputfreq = dInputfreq + STxPulseRTFT.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(1, dInputfreq , STxPulseRTFT.m_dInputPower, &dSGinPower, &m_dMeasPathLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the RF stimulus path calibration data ");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strActLog);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF( u8SGSel, (dInputfreq * 1.0e6),  dSGinPower/*sPowerDroopIPParam.m_dInputPower*/); //add cable loss
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to configure frequency and power level in signal Generator");
                throw DP_TEST_FAIL;
            }
            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            /* Enable Output for RF_SigGen */
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled( u8SGSel, DP_ATE_RFOUT_ENABLE);	//siggen2
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Signal Generator RF OUT Failed");
                throw DP_TEST_FAIL;
            }
#else
            sRetVal = 0;
#endif

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            msleep(100);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(500);
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PwrMtr_MeasurPulseRiseTime(u8PMSel,&dMeasRiseTime);
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to measure the Pulse rise time");
                throw DP_TEST_FAIL;
            }
#else
            sRetVal = 0;
#endif


#ifndef _SIMULATION_INSTRUMENTS_
            /* Disable Output for SigGen */
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled( u8SGSel, DP_ATE_RFOUT_DISABLE);
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Signal Generator RF OFF Failed");
                throw DP_TEST_FAIL;
            }
#else
            sRetVal = 0;
#endif

            m_dExpRise[uscount]= STxPulseRTFT.m_dExpectedRiseFallTime;
            m_dMeasRise[uscount] = dMeasRiseTime;

            if(((m_dMeasRise[uscount] * 1e9) < STxPulseRTFT.m_dExpectedRiseFallTime))
            {
                strTemp = QString::asprintf("Channel : %d, Measure Rise Time Test Pass at Frequency : %f MHz, Measured Rise Time : %.2lf ns",sPulseRiseFallTimeMeasReport.m_u8ChannelNo, dInputfreq,m_dMeasRise[uscount] * 1e9);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,strTemp);
                sPulseRiseFallTimeMeasReport.m_u8Result = 0;
            }
            else
            {
                strTemp = QString::asprintf("Channel : %d, Measure Rise Time Test Pass at Frequency : %f MHz, Measured Rise Time : %.2lf ns",sPulseRiseFallTimeMeasReport.m_u8ChannelNo, dInputfreq,m_dMeasRise[uscount] * 1e9);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,strTemp);
                *out_bTestSts = true;
                sPulseRiseFallTimeMeasReport.m_u8Result = 1;
            }
            //QVectTx_PulseRiseFall_Measurement.push_back(sPulseRiseFallTimeMeasReport);
            uscount++;
        }
        uscount = 0;

        strTemp = QString::asprintf("Fall Time Test in Progress");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO,strTemp);

        /* Tx Pulse FallTime Measurement */
#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_Slope(u8PMSel, 0);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = true;
            strTemp = QString::asprintf("%s","Unable to configure the Power Meter Slope");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        for(double dInputfreq = STxPulseRTFT.m_dStartFrequency; dInputfreq <= STxPulseRTFT.m_dStopFrequency; dInputfreq = dInputfreq + STxPulseRTFT.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(1, dInputfreq , STxPulseRTFT.m_dInputPower, &dSGinPower, &m_dMeasPathLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the RF stimulus path calibration data ");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strActLog);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF( u8SGSel, (dInputfreq * 1.0e6),  dSGinPower/*sPowerDroopIPParam.m_dInputPower*/); //add cable loss
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to configure frequency and power level in signal Generator");
                throw DP_TEST_FAIL;
            }
            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            /* Enable Output for RF_SigGen */
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled( u8SGSel, DP_ATE_RFOUT_ENABLE);	//siggen2
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Signal Generator RF OUT Failed");
                throw DP_TEST_FAIL;
            }
#else
            sRetVal = 0;
#endif

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            msleep(100);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            msleep(500);
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PwrMtr_MeasurPulseFallTime(u8PMSel,&dMeasFallTime);
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("Unable to Measure the Pulse Fall time");
                throw DP_TEST_FAIL;
            }
#else
            sRetVal = 0;
#endif
            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            /* Disable Output for SigGen */
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled( u8SGSel, DP_ATE_RFOUT_DISABLE);
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("Signal Generator RF OFF Failed");
                throw DP_TEST_FAIL;
            }
#else
            sRetVal = 0;
#endif
            sPulseRiseFallTimeMeasReport.m_dInputFrequency = dInputfreq;
            sPulseRiseFallTimeMeasReport.m_dInputPower = STxPulseRTFT.m_dInputPower;
            sPulseRiseFallTimeMeasReport.m_dExpRiseFallTime = STxPulseRTFT.m_dExpectedRiseFallTime;
            sPulseRiseFallTimeMeasReport.m_dMeasRiseTime = m_dMeasRise[uscount]*1e9;
            sPulseRiseFallTimeMeasReport.m_dMeasFallTime = dMeasFallTime *1e9;

            if(((dMeasFallTime * 1e9) < STxPulseRTFT.m_dExpectedRiseFallTime))
            {
                strTemp = QString::asprintf("Channel : %d, Measure Fall Time Test Pass at Frequency : %f MHz, Measured Fall Time : %.2lf ns", sPulseRiseFallTimeMeasReport.m_u8ChannelNo, sPulseRiseFallTimeMeasReport.m_dInputFrequency, sPulseRiseFallTimeMeasReport.m_dMeasFallTime);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,strTemp);
                sPulseRiseFallTimeMeasReport.m_u8Result = 0;
            }
            else
            {
                strTemp = QString::asprintf("Channel : %d, Measure Fall Time Test Pass at Frequency : %f MHz, Measured Fall Time : %.2lf ns", sPulseRiseFallTimeMeasReport.m_u8ChannelNo, sPulseRiseFallTimeMeasReport.m_dInputFrequency, sPulseRiseFallTimeMeasReport.m_dMeasFallTime);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,strTemp);
                *out_bTestSts = true;
                sPulseRiseFallTimeMeasReport.m_u8Result = 1;
            }
            QVect_RF_PulseRise_Measurement.push_back(sPulseRiseFallTimeMeasReport);
            uscount++;
        }

        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, DP_ATE_RFOUT_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s","Unable to disable signal Generator");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_DIS);
#else
        sRetVal = 0;
#endif
        if(sRetVal != 0)
        {
            *out_bTestSts = true;
            strTemp = QString::asprintf("%s","Unable to Enable Output Function generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        sRetVal = OnSafeState();
        if(sRetVal)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    return DP_SUCCESS;
}

short CAutoTestThread ::Power_Droop_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    short sRetVal								= DP_APP_ZERO;
    double dPwrMtr_XStart						= DP_APP_ZERO;
    double dPwrMtr_Xscalestep							= DP_APP_ZERO;
    double dPwrMtr_YScale						= DP_APP_ZERO;
    double dPwrMtr_Trigger						= DP_APP_ZERO;
    double dPwrMtr_YScalestep					= DP_APP_ZERO;
    double dPwrMtr_Marker_1						= DP_APP_ZERO;
    double dPwrMtr_Marker_2						= DP_APP_ZERO;
    double dRFFreq								= DP_APP_ZERO;
    double dMeasPwrMarker1						= DP_APP_ZERO;
    double dMeasPwrMarker2						= DP_APP_ZERO;
    U8BIT u8PMSel               = 1;
    U8BIT u8SGSel               = 1;
    QStringList qslstTRPselection    ;
    QStringList qslstTRPconfig    ;
    QStringList qslstSBOBconfig    ;
    QTextStream objFileStream;
    QStringList qstrList;
    QString qsTemp                              ="";
    QString qsTempMsg                           = "";
    QStringList qsPulseconfig;
    QFile qfConfigFile;
    QStringList qsPulsewidconfig;
    QStringList qsChannellist;
    QString cErrCode                            = "key not found";
    QStringList qlistrReportData;
    QString strTestName        = "";
    U32BIT u32TestCaseID       = DP_APP_ZERO;
    QString strActLog          = "";
    unsigned short u16RFPathChNo[2] = {0};
    QString strTemp            = "";

    S_SBANDGANPAMODULE_POWERDROOPMEASUREMENT sPowerDroopIPParam;
    S_POWERDROOP_MEASUREMENT  sPowerDroopReport;
    *out_bTestSts = true;
    memset(&sPowerDroopReport,DP_APP_ZERO,sizeof (S_POWERDROOP_MEASUREMENT));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID,1,&sPowerDroopIPParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Tx Output Power Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    dPwrMtr_YScale = sPowerDroopIPParam.m_dYScale;
    dPwrMtr_YScalestep = sPowerDroopIPParam.m_dYScaleMax;
    dPwrMtr_Trigger = sPowerDroopIPParam.m_dTrigger;

    dPwrMtr_XStart = sPowerDroopIPParam.m_dXScaleStart * 1e3;
    dPwrMtr_Xscalestep = sPowerDroopIPParam.m_dXScaleStep * 1e3;
    dPwrMtr_Marker_1 = sPowerDroopIPParam.m_dMarker1;
    dPwrMtr_Marker_2 = sPowerDroopIPParam.m_dMarker2;

#ifndef _SIMULATION_INSTRUMENTS_
    /* SigGen Preset for RF */
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SGSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to preset signal Generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /*Config Power meter*/
    sRetVal = p_objMainApp->m_objPMWrap.DP_PMeter_Preset(u8PMSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to preset the Power Meter");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /*Config Power meter*/
    sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_Config(u8PMSel,3.1,0,(dPwrMtr_XStart), (dPwrMtr_Xscalestep/1e6), dPwrMtr_YScalestep, dPwrMtr_YScale, dPwrMtr_Trigger, dPwrMtr_Marker_1, dPwrMtr_Marker_2);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to configure the Power Meter");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    msleep(200);

    /*Function to preset function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Reset(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_CONFIG_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Config(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance,DP_FG_CH1,DP_FG_WAVEFORM_PULSE_SIGLENT,sPowerDroopIPParam.m_dPulsePeriod,sPowerDroopIPParam.m_dPulseWidth,sPowerDroopIPParam.m_dHighLevel, sPowerDroopIPParam.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Configure Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#if 1
    /*Function to configure waveform shape in function generator for all channels*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_ConfigureWaveformShape(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_WAVEFORM_PULSE_SIGLENT);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulse(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1,0/*in_dDutyCycle*/, sPowerDroopIPParam.m_dPulsePeriod);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulseWidth(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, sPowerDroopIPParam.m_dPulseWidth);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    /*Function to configure impedance in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_SetImpedance(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_IMPEDANCE_INFINITY, DP_FG_IMPEDANCE_AUTO);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Set Impedance in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgHighLowLvl(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, sPowerDroopIPParam.m_dHighLevel, sPowerDroopIPParam.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Configure Offset voltage Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#endif

    /*Function to enable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_EN);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        strTemp = QString::asprintf("%s","Unable to Enable Output Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    try{

        for(double dInputfreq = sPowerDroopIPParam.m_dStartFrequency; dInputfreq <= sPowerDroopIPParam.m_dStopFrequency; dInputfreq = dInputfreq + sPowerDroopIPParam.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dRFFreq = dInputfreq;
            double dSGinPower;
            double m_dMeasPathLoss;
#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(1, dInputfreq , sPowerDroopIPParam.m_dInputPower, &dSGinPower, &m_dMeasPathLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the RF stimulus path calibration data ");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strActLog);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF( u8SGSel, (dRFFreq * 1.0e6),  dSGinPower/*sPowerDroopIPParam.m_dInputPower*/); //add cable loss
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to configure frequency and power level in signal Generator");
                throw DP_TEST_FAIL;
            }
            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled( u8SGSel, DP_ATE_RFOUT_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Signal Generator  RF OUT Failed");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PwrMtr_ReadMarkerPower(u8PMSel,1,&dMeasPwrMarker1);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to Read Output Power in RF Power Meter for Signal Power Measurement Test");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PwrMtr_ReadMarkerPower(u8PMSel,2,&dMeasPwrMarker2);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to Read Output Power in RF Power Meter for Signal Power Measurement Test");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, DP_ATE_RFOUT_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Signal Generator 1 RF OFF Failed");
                throw DP_TEST_FAIL;
            }

            sPowerDroopReport.m_dRFInputFreq = dInputfreq;
            sPowerDroopReport.m_dInputPower = sPowerDroopIPParam.m_dInputPower;
            sPowerDroopReport.m_dExpMaxPowerDroop = sPowerDroopIPParam.m_dExpectedPowerDroop;
            sPowerDroopReport.m_dMeasPowerMarker1 = dMeasPwrMarker1;
            sPowerDroopReport.m_dMeasPowerMarker2 = dMeasPwrMarker2;
            sPowerDroopReport.m_dCalPowerDroop = dproundoff((dMeasPwrMarker1 - dMeasPwrMarker2), 2);

            qsTempMsg = QString::asprintf("CH : %d, I/P Freqency: %0.3f MHz,  I/P : %0.2f dBm, Exp. Max. Power Droop : %0.2f dB, Measured Power Droop : %0.2f dB",sPowerDroopReport.m_u8ChannelNo, sPowerDroopReport.m_dRFInputFreq, sPowerDroopReport.m_dInputPower, sPowerDroopReport.m_dExpMaxPowerDroop, sPowerDroopReport.m_dCalPowerDroop);
            if(sPowerDroopReport.m_dCalPowerDroop < sPowerDroopReport.m_dExpMaxPowerDroop)
            {
                sPowerDroopReport.m_u8Result = DP_TEST_PASS;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, qsTempMsg);
            }
            else
            {
                *out_bTestSts = false;
                sPowerDroopReport.m_u8Result = DP_TEST_FAIL;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, qsTempMsg);
            }
            QVect_PowerDroop_Measurement.push_back(sPowerDroopReport);
        }
        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, DP_ATE_RFOUT_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s","Unable to disable signal Generator");
            throw DP_TEST_FAIL;
        }
        /*Function to Disable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_DIS);
#else
        sRetVal = 0;
#endif
        if(sRetVal != 0)
        {
            *out_bTestSts = true;
            strTemp = QString::asprintf("%s","Unable to Enable Output Function generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        sRetVal = OnSafeState();
        if(sRetVal)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    return DP_SUCCESS;
}

short CAutoTestThread ::Noise_Figure_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    U8BIT u8RFChnNumber             = 0;
    U8BIT u8IFChnNumber             = 0;
    U32BIT u32TestCaseID        = 0;
    U32BIT u32CfgNumber         = 0;
    U32BIT u32NoOfPoints = 0;
    S16BIT sRetVal               = 0;
    S8BIT szStateFileName[512] = {0};
    S8BIT szUserMsg[512]         = {0};
    FDOUBLE dStartFreq = 0.0;
    FDOUBLE dStopFreq = 0.0;
    FDOUBLE dSpotFreq = 0.0;
    FDOUBLE dStepFreq = 0.0;
    FDOUBLE dNoiseFig[161]    				= {0.0f};
    FDOUBLE dNoiseFigTemp[161]    			= {0.0f};
    FDOUBLE dGainValue[161]					= {0.0f};
    QString strFilePath		   				= "";
    QString strTemp                     = "";
    QString strActLog           = "";
    QString strTestName         = "";
    U8BIT u8SASel               = 1;
    U8BIT u8Precision = 2;

    unsigned short u16RFPathChNo[2] = {0};
    *out_bTestSts 							= 1;
    S_SBANDGANPAMODULE_NOISEFIGUREMEASUREMENT sNoiseMeasurement;
    S_NOISEFIGURE_MEASUREMENT sNoiseMeasurementReport;
    memset(&sNoiseMeasurementReport, 0 ,sizeof(S_NOISEFIGURE_MEASUREMENT));

    if(in_szTestname == NULL)
    {
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read test case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    //Spectrum Preset
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(u8SASel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to reset the spectrum analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //SA Noise Figure Mode
    sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ModeConfig(u8SASel, 219);
#else
    sRetVal = 0 ;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to configure Noise Figure mode in spectrum analyzer for Noise Figure measurement ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //        sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_PreAmpOnOFF(u8SASel, u8PreAmpSel);
#else
    sRetVal = 0 ;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to configure Pre Amplifier Selection in spectrum analyzer for Noise Figure measurement ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sNoiseMeasurement);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s",  "Unable to read Config data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    dStartFreq      = sNoiseMeasurement.m_dStartFrequency;
    dStopFreq       = sNoiseMeasurement.m_dStopFrequency;
    u32NoOfPoints   = sNoiseMeasurement.m_u16NoofPoints;


#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_NFConfigBW(u8SASel,2/*in_dBandWidth in MHz*/);
#else
    sRetVal = 0 ;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to Configure NF Bandwidth");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sprintf(szUserMsg,"Do you wish to do Noise Source Calibration ?\nIf YES then Connect Noise Source with SA and then Click on Yes Button\nIf NO then Connect Noise Source with INPUT PORT and then Click on No Button");
    emit sig_QusToUserContinueSkip(szUserMsg);
    if(g_ucPressYES_NO)
    {
        g_ucPressYES_NO = 0;

        /*TODO Read from Calib caonfig Table data*/
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Noise Source Calibration in progress");

        /*Amplifer*/
#ifndef _SIMULATION_INSTRUMENTS_
        // Config the DUT setup in SA
        sRetVal = p_objMainApp->m_objSAWrapper.AgSA_AmpDUTSetupConfig(u8SASel, (dStartFreq * 1e6), (dStopFreq *1e6), ((dStopFreq - dStartFreq) / (u32NoOfPoints-1))*1e6, u32NoOfPoints,1);
#else
        sRetVal = 0 ;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to configure the Down Convertion setup in spectrum analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        //Calibrate noise source
        sRetVal = p_objMainApp->m_objSAWrapper.Calibration_With_NoiseSoure(u8SASel, 10000);
#else
        sRetVal = 0 ;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to calibrate noise source for Noise figure measurement ");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        sprintf(szStateFileName, "GaN_%d_%dMHz", (U32BIT)(dStartFreq + 0.1), (U32BIT)(dStopFreq+0.1));
#ifndef _SIMULATION_INSTRUMENTS_
        //Calibrate noise source
        sRetVal = p_objMainApp->m_objSAWrapper.DP_SA_StoreStateIntoFile(u8SASel, szStateFileName);
#else
        sRetVal = 0 ;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to store the calibration data into state file for Noise figure measurement ");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Noise Source Calibration completed");
    }
    sprintf(szUserMsg, "Connect Noise Source in RF IN Port and IF OUT to Signal Analyzer.\n1. Press Continue button to start testing \n2. Press Skip button to skip");
    emit sig_QusToUserContinueSkip(szUserMsg);
    if(g_ucPressYES_NO == 1)
    {
        sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, u32CfgNumber, &sNoiseMeasurement);
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s", "Unable to read config data");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
        /*Recall state File */
        sprintf(szStateFileName, "GaN_%d_%dMHz", (U32BIT)(dStartFreq + 0.1), (U32BIT)(dStopFreq+0.1));
#ifndef _SIMULATION_INSTRUMENTS_
        //Calibrate noise source
        sRetVal = p_objMainApp->m_objSAWrapper.DP_SA_LoadStateFromFile(u8SASel, szStateFileName);
#else
        sRetVal = 0 ;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to read the calibration data into state file for Noise figure measurement ");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
        dStartFreq      = sNoiseMeasurement.m_dStartFrequency;
        dStopFreq       = sNoiseMeasurement.m_dStopFrequency;
        u32NoOfPoints   = sNoiseMeasurement.m_u16NoofPoints;
        sNoiseMeasurementReport.m_dExpNoiseFig = sNoiseMeasurement.m_ExpectedNoiseFigure;

        //Add Configuration

        /*Read Noise Figure*/
#ifndef _SIMULATION_INSTRUMENTS_
        //Read noise figure Value
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadNF(u8SASel, dNoiseFig, dGainValue, dNoiseFigTemp);
#else
        sRetVal = 0 ;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to Read noise figure Value for Noise figure measurement ");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        for(U16BIT u16SpotIdx = 0; u16SpotIdx < u32NoOfPoints; u16SpotIdx++)
        {
            //sNoiseMeasurementReport.m_dMeasGainValue = dGainValue[u16SpotIdx];
            sNoiseMeasurementReport.m_dMeasNoiseFig = dNoiseFig[u16SpotIdx];
            dStepFreq = (dStopFreq - dStartFreq) / (u32NoOfPoints - 1);

            dSpotFreq = dStartFreq + (dStepFreq * u16SpotIdx);
            sNoiseMeasurementReport.m_dRFfreq = dSpotFreq;
            //Result
            strActLog = QString::asprintf("Channel Number : %d, I/P Frequency: %0.3f MHz,  Exp. Max Noise Figure: %0.2f dB, Measured Noise Figure: %0.2f dB, Measured Gain: %0.2f dB", sNoiseMeasurementReport.m_u8ChannelNo, sNoiseMeasurementReport.m_dRFfreq, sNoiseMeasurementReport.m_dExpNoiseFig, sNoiseMeasurementReport.m_dMeasNoiseFig,dGainValue[u16SpotIdx]);

            long lMeasNoisFig	= (long)(dproundoff(sNoiseMeasurementReport.m_dMeasNoiseFig,  u8Precision));
            long lExpMaxNoisFig 	= (long)(dproundoff(sNoiseMeasurementReport.m_dExpNoiseFig, u8Precision));

            if(lMeasNoisFig < lExpMaxNoisFig)
            {
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
                sNoiseMeasurementReport.m_u8Result = DP_TEST_PASS;
            }
            else
            {
                sNoiseMeasurementReport.m_u8Result = DP_TEST_FAIL;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                *out_bTestSts = 0;
            }
            QVect_NoiseFig_Measurement.push_back(sNoiseMeasurementReport);
        }/*No. of Points end*/

        if(g_bAutoTestStartStop == false)
        {
            sNoiseMeasurementReport.m_u8Result = DP_TEST_STOPPED;
            QVect_NoiseFig_Measurement.push_back(sNoiseMeasurementReport);
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to report record insert into database");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
#ifndef _SIMULATION_CONFIG_
            sRetVal = p_objMainApp->obj_TRM_wrapper->DP_SPL_6768_Write_Mode_Selection_Phase_calculation(DP_SPL_6768_IDLE, DP_SPL_6768_NORMAL, DP_SPL_6768_DIRECT);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to configure mode selection");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            strTemp = QString::asprintf("%s","Noise Figure Measurement Test stopped");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_TEST_STOPPED;
        }

    }
    else
    {
        *out_bTestSts = 0;
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Noise Figure Measurement Test stopped");
        return DP_TEST_STOPPED;
    }

    sRetVal = OnSafeState();
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to report record insert into database");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    return DP_SUCCESS;
}

short CAutoTestThread ::Power_Tuning_Measurement(bool *out_bTestSts, char *in_szTestname)
{
    short sRetVal								= DP_APP_ZERO;
    double dPwrMtr_XStart						= DP_APP_ZERO;
    double dPwrMtr_Xscalestep							= DP_APP_ZERO;
    double dPwrMtr_YScale						= DP_APP_ZERO;
    double dPwrMtr_Trigger						= DP_APP_ZERO;
    double dPwrMtr_YScalestep					= DP_APP_ZERO;
    double dPwrMtr_Marker_1						= DP_APP_ZERO;
    double dPwrMtr_Marker_2						= DP_APP_ZERO;
    double dRFFreq								= DP_APP_ZERO;
    double dMeasPwrMarker						= DP_APP_ZERO;
    U8BIT u8PMSel               = 1;
    U8BIT u8SGSel               = 1;
    QStringList qslstTRPselection    ;
    QStringList qslstTRPconfig    ;
    QStringList qslstSBOBconfig    ;
    QTextStream objFileStream;
    QStringList qstrList;
    QString qsTemp                              ="";
    QString qsTempMsg                           = "";
    QStringList qsPulseconfig;
    QFile qfConfigFile;
    QStringList qsPulsewidconfig;
    QStringList qsChannellist;
    QString cErrCode                            = "key not found";
    QStringList qlistrReportData;
    QString strTestName        = "";
    U32BIT u32TestCaseID       = DP_APP_ZERO;
    QString strActLog          = "";
    unsigned short u16RFPathChNo[2] = {0};
    QString strTemp            = "";
    U8BIT u8Address             = 1;
    FSINGLE fVoltage            = 0.0;
    FSINGLE fCurrent            = 0.0;
    double dCalibPower;
    double dOutputCableLoss;
    QString qsConfigcsvFilepath = qApp->applicationDirPath()+"/Application/INPUT_FILES/RFTxOutputPower.csv";

    S_SBANDGANPAMODULE_POWERTUNINGMEASUREMENT SPowerTuning;
    S_POWER_TUNING_MEASUREMENT SPowerTuningReport;
    *out_bTestSts = true;
    memset(&SPowerTuningReport,DP_APP_ZERO,sizeof (S_TX_OUTPUT_POWER_MEASUREMENT));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID,1,&SPowerTuning);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Rx input return loss Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    dPwrMtr_YScale = SPowerTuning.m_dYScale;  /*Ymax*/
    dPwrMtr_YScalestep = SPowerTuning.m_dYMax;
    dPwrMtr_Trigger = -10;

    dPwrMtr_XStart = (-SPowerTuning.m_dPulseWidth * 1e3 * 0.2);
    dPwrMtr_Xscalestep = SPowerTuning.m_dPulseWidth * 1e3 * 0.2;
    dPwrMtr_Marker_1 = (SPowerTuning.m_dPulseWidth) * 0.3;
    dPwrMtr_Marker_2 = (SPowerTuning.m_dPulseWidth) * 0.4;


#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->DP_PSU_Reset(g_u16PSMInstanceID, DP_PSU_LOCAL_MODE, 1);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    /*Power Supply ON*/
    sRetVal = PowerON();
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    /* SigGen Preset for RF */
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SGSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        PowerOff();
        strTemp = QString::asprintf("%s", "Unable to preset signal Generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /*Config Power meter*/
    sRetVal = p_objMainApp->m_objPMWrap.DP_PMeter_Preset(u8PMSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to preset the Power Meter");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /*Config Power meter*/
    sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_Config(u8PMSel,3.1,0,(dPwrMtr_XStart), (dPwrMtr_Xscalestep/1e6), dPwrMtr_YScalestep, dPwrMtr_YScale, dPwrMtr_Trigger, dPwrMtr_Marker_1, dPwrMtr_Marker_2);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to configure the Power Meter");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    msleep(200);
    //    p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_usInstID = 1;
    /*Function to preset function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Reset(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_CONFIG_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_Config(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance,DP_FG_CH1,DP_FG_WAVEFORM_PULSE_SIGLENT,SPowerTuning.m_dPulsePeriod,SPowerTuning.m_dPulseWidth,SPowerTuning.m_dHighLevel, SPowerTuning.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to Configure Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#if 1
    /*Function to configure waveform shape in function generator for all channels*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_ConfigureWaveformShape(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_WAVEFORM_PULSE_SIGLENT);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to Reset Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulse(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1,0/*in_dDutyCycle*/, SPowerTuning.m_dPulsePeriod);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgPulseWidth(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, SPowerTuning.m_dPulseWidth);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to set pulse width in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    /*Function to configure impedance in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_SetImpedance(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_IMPEDANCE_INFINITY, DP_FG_IMPEDANCE_AUTO);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to Set Impedance in Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_CfgHighLowLvl(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, SPowerTuning.m_dHighLevel, SPowerTuning.m_dLowLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to Configure Offset voltage Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#endif

    /*Function to enable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_EN);
#else
    sRetVal = 0;
#endif
    if(sRetVal != 0)
    {
        *out_bTestSts = true;
        PowerOff();
        strTemp = QString::asprintf("%s","Unable to Enable Output Function generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    try{
        //Add Configuration
        for(double dInputfreq = SPowerTuning.m_dStartFrequency; dInputfreq <= SPowerTuning.m_dStopFrequency; dInputfreq = dInputfreq + SPowerTuning.m_dStepFrequency)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dRFFreq = dInputfreq;

            double dSGinPower;
            double m_dMeasPathLoss;
#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(1, dInputfreq , SPowerTuning.m_dInputPower, &dSGinPower, &m_dMeasPathLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s","Unable to Read the RF stimulus path calibration data ");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strActLog);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF( u8SGSel, (dRFFreq * 1.0e6),  dSGinPower/*SPowerTuning.m_dInputPower*/); //add cable loss
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to configure frequency and power level in signal Generator");
                throw DP_TEST_FAIL;
            }
            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled( u8SGSel, DP_ATE_RFOUT_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Signal Generator  RF OUT Failed");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                PowerOff();
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(SPowerTuning.m_dReadPeakDelay);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PowerMeter_FreeRun(u8PMSel,DP_PWRMETER_FREERUN_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {

                *out_bTestSts = true;
                PowerOff();
                strTemp = QString::asprintf("%s","Unable to Configure Trigger Auto Mode in RF Power Meter");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objPMWrap.DP_PwrMtr_ReadPeakPower(u8PMSel,&dMeasPwrMarker);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Unable to Read Output Power in RF Power Meter for Signal Power Measurement Test");
                throw DP_TEST_FAIL;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->DP_PSU_Measure_Ouput(g_u16PSMInstanceID,u8Address,&fVoltage,&fCurrent);
#else
            sRetVal = 0;
#endif
            if(sRetVal != 0)
            {
                strTemp = QString::asprintf("%s","PSU Measure Output Failed");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, DP_ATE_RFOUT_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Signal Generator 1 RF OFF Failed");
                throw DP_TEST_FAIL;
            }

            SPowerTuning.m_dRFInputPathID = 2;
#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = ApplyCalibData(SPowerTuning.m_dRFInputPathID, dRFFreq , dMeasPwrMarker, &dCalibPower, &dOutputCableLoss);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s","Signal Generator 1 RF OFF Failed");
                throw DP_TEST_FAIL;
            }



            SPowerTuningReport.m_dFrequency = dInputfreq;
            SPowerTuningReport.m_dInputPower = SPowerTuning.m_dInputPower;
            SPowerTuningReport.m_dMeasuredOutputPower = dMeasPwrMarker;
            SPowerTuningReport.m_dMeasPowerWatt = dpdbmtoWatt(dCalibPower);
            SPowerTuningReport.m_dCalculatedOutputPower = dCalibPower;
            SPowerTuningReport.m_dCurrent = fCurrent;
            SPowerTuningReport.m_dPulseWidth = SPowerTuning.m_dPulseWidth;
            SPowerTuningReport.m_dPulsePeriod = SPowerTuning.m_dPulsePeriod;

            /*Efficiency (%) = ((Calculated output power(W)*(Pulsewidth/Pulseperiod))/(50*DC current))*100*/
            SPowerTuningReport.m_dEfficiency = (((SPowerTuningReport.m_dMeasPowerWatt * (SPowerTuning.m_dPulseWidth / SPowerTuning.m_dPulsePeriod)) / (50.0 * fCurrent)) * 100.0);
            if((dCalibPower > (SPowerTuning.m_dExpMinOutPower)))
            {
                strActLog = QString::asprintf("Channel No : %d, Frequency : %f MHz,Observed Power:%0.4lf dBm, Calculated Power : %0.4lf dBm, Measured Power in Watts : %lf, Efficiency : %0.2f", SPowerTuningReport.m_u8ChannelNo, SPowerTuningReport.m_dFrequency, SPowerTuningReport.m_dMeasuredOutputPower,dCalibPower, SPowerTuningReport.m_dMeasPowerWatt,SPowerTuningReport.m_dEfficiency);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
                SPowerTuningReport.m_u8Result = DP_TEST_PASS;
            }
            else
            {
                strActLog = QString::asprintf("Channel No : %d, Frequency : %f MHz,Observed Power:%0.4lf dBm, Calculated Power : %0.4lf dBm, Measured Power in Watts : %lf, Efficiency : %0.2f", SPowerTuningReport.m_u8ChannelNo, SPowerTuningReport.m_dFrequency, SPowerTuningReport.m_dMeasuredOutputPower,dCalibPower, SPowerTuningReport.m_dMeasPowerWatt,SPowerTuningReport.m_dEfficiency);
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                SPowerTuningReport.m_u8Result = DP_TEST_FAIL;
                *out_bTestSts = false;
            }
            QVect_PowerTuning_Measurement.push_back(SPowerTuningReport);
        }

        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SGSel, DP_ATE_RFOUT_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s","Unable to disable signal Generator");
            throw DP_TEST_FAIL;
        }
        /*Function to Disable output in function generator*/
#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objFGWrapper.DP_FG_EnableOutput(p_objMainApp->m_objFGWrapper.m_arrSFGInstHandle[0].m_u8Instance, DP_FG_CH1, DP_FG_OUTPUT_DIS);
#else
        sRetVal = 0;
#endif
        if(sRetVal != 0)
        {
            *out_bTestSts = true;
            strTemp = QString::asprintf("%s","Unable to Enable Output Function generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /*Power Supply Turning Off*/
        sRetVal = PowerOff();
#else
        sRetVal = 0;
#endif
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        sRetVal = OnSafeState();
        if(sRetVal)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    return DP_SUCCESS;
}

short CAutoTestThread ::Rx_OutOfBand_Rejection_Measuremnt(bool *out_bTestSts, char *in_szTestname)
{
    U8BIT u8NetworkAnalyzerSel = 1;
    U16BIT u16Trace = 0;
    U8BIT u8Precision = 2;
    S32BIT sRetVal = 0;
    S32BIT s32Delay = 0;
    FDOUBLE dSweepTime = 0.0;
    QString strTemp = "";
    QString strActLog = "";
    QString strPortName = "";
    S8BIT szStateFilepath[512]   = {0};
    QString strStateFilePath    = "D:\\2";

    QString strCalibData = "";
    QStringList strlstCalibData = {""};
    QTextStream objTextStream;
    QFile objCalibCfg;
    QString strFileName = "";
    U16BIT usTrace = 0;
    U8BIT u8NAPortNo               = 1;
    U8BIT u8NAChannelNo               = 1;


    unsigned short in_iMarkerNo		 = DP_APP_ZERO;
    unsigned char in_ucMeasType		 = DP_APP_ZERO;
    double dMarkerFreq = 0.0f;
    double dMarkerValue              = 0.0f;
    double dGainforRef = 0.0f;
    unsigned short u16RFPathChNo[2] = {0};
    char szUserMsg[1024]            = {0};
    QString strTestName        = "";
    U32BIT u32TestCaseID       = DP_APP_ZERO;

    S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION sRxOutOfBandMeasParam;
    S_RX_OUTOFBAND_REJECTION_MEASUREMENT sRxOutOfBandMeasRpt;
    memset(&sRxOutOfBandMeasRpt, 0, sizeof(S_RX_OUTOFBAND_REJECTION_MEASUREMENT));
    *out_bTestSts = true;
    sprintf(szStateFilepath, "%s", strStateFilePath.toStdString().c_str());

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sRxOutOfBandMeasParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Tx input return loss Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sprintf(szUserMsg, "Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser(szUserMsg);

    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(sRxOutOfBandMeasParam.m_dCalibStartFrequency, sRxOutOfBandMeasParam.m_dCalibStopFrequency, sRxOutOfBandMeasParam.m_dInputPower, sRxOutOfBandMeasParam.m_u16NumberOfPoints, strStateFilePath,1);
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }
    } // calib end
    try{
#ifndef _SIMULATION_INSTRUMENTS_
        //Recall State file
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NetworkAnalyzerSel, szStateFilepath);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to load state file in Network Analyzer");
            throw DP_TEST_FAIL;
        }

        u16Trace = 3;//S11 trace

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DeleteAllTraces(u8NetworkAnalyzerSel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to delete all traces in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_CreateNewTrace(u8NetworkAnalyzerSel,u8NAPortNo,u16Trace);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to create new trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DisplayTrace(u8NetworkAnalyzerSel,u16Trace,u8NAPortNo,u8NAChannelNo);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to display trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        //Select trace
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NetworkAnalyzerSel, sRxOutOfBandMeasParam.m_u16NumberOfPoints, u16Trace, u8NAPortNo,u8NAChannelNo);//trace, port, channel
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDisplayFormat(u8NetworkAnalyzerSel, DP_LOG_MODE, u8NAChannelNo);  // 1 ch no
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Display format in Network Analyzer");
            throw DP_TEST_FAIL;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Start Frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NetworkAnalyzerSel, (sRxOutOfBandMeasParam.m_dCalibStartFrequency * 1e6), u8NAChannelNo);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Start Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Stop frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NetworkAnalyzerSel, (sRxOutOfBandMeasParam.m_dCalibStopFrequency * 1e6), u8NAChannelNo);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Stop Frequency in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Power level in NA
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NetworkAnalyzerSel, sRxOutOfBandMeasParam.m_dInputPower, u8NAChannelNo);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to configure power Level in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set the RF Output Power ON
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NetworkAnalyzerSel);
#else
        sRetVal = DP_APP_ZERO;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Turn ON RF in Network Analyzer");
            throw DP_TEST_FAIL;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        msleep(500);
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_SINGLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            throw DP_TEST_FAIL;
        }

      //Add Configuration

        for(U32BIT u32FreqTblIdx = 0; u32FreqTblIdx < sRxOutOfBandMeasParam.m_vectCONFIGURATIONTABLE.size(); u32FreqTblIdx++)
        {
            if(p_objMainApp->m_pobjAutoTestCases->bTestExecSts == false)
            {
                break;
            }
            dMarkerFreq = sRxOutOfBandMeasParam.m_vectCONFIGURATIONTABLE[u32FreqTblIdx].m_dFrequency;
            in_iMarkerNo    = 1;
            in_ucMeasType   = 1;

#ifndef _SIMULATION_INSTRUMENTS_
            //Set Marker Position
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetMarkerPosAndFreq(u8NetworkAnalyzerSel, in_iMarkerNo, dMarkerFreq*1e6, u8NAChannelNo);
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to set marker position and Frequency in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            msleep(1000);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal=p_objMainApp->m_objNAWrap.DP_NA_ReadMarkerValue(u8NetworkAnalyzerSel, in_iMarkerNo, in_ucMeasType, u8NAChannelNo, &dMarkerValue);
#else
            sRetVal = DP_APP_ZERO;
#endif
            if(sRetVal < 0)
            {
                strTemp = QString::asprintf("%s", "Unable to read data at marker position in Network Analyzer");
                throw DP_TEST_FAIL;
            }

            //                dMarkerValue = dMarkerValue * -1;
            sRxOutOfBandMeasRpt.m_dObservedGain = dMarkerValue;
            sRxOutOfBandMeasRpt.m_dInputFrequency = dMarkerFreq;
            sRxOutOfBandMeasRpt.m_dInputPower = sRxOutOfBandMeasParam.m_dInputPower;
            sRxOutOfBandMeasRpt.m_dExpRejection = sRxOutOfBandMeasParam.m_vectCONFIGURATIONTABLE[u32FreqTblIdx].m_dExpectedRejection;

            if(dMarkerFreq == 3100)
            {
                sRxOutOfBandMeasRpt.m_dCalculatedRejection = 0;
                dGainforRef = sRxOutOfBandMeasRpt.m_dObservedGain;
            }
            else
            {
                sRxOutOfBandMeasRpt.m_dCalculatedRejection = dGainforRef - sRxOutOfBandMeasRpt.m_dObservedGain;
            }

            strActLog = QString::asprintf("Channel Number : %d, Input Frequency : %0.3f MHz , Expected Rejection : %0.2lf dB, Observed Gain : %0.2lf dB, Calculated Rejection : %0.2lf dB ",sRxOutOfBandMeasRpt.m_u8ChannelNo, sRxOutOfBandMeasRpt.m_dInputFrequency,  sRxOutOfBandMeasRpt.m_dExpRejection, sRxOutOfBandMeasRpt.m_dObservedGain, sRxOutOfBandMeasRpt.m_dCalculatedRejection);
            if(sRxOutOfBandMeasRpt.m_dCalculatedRejection >= sRxOutOfBandMeasRpt.m_dExpRejection)
            {
                sRxOutOfBandMeasRpt.m_u8Result = 0;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
            }
            else
            {

                sRxOutOfBandMeasRpt.m_u8Result = 1;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            }
            QVect_RxOutofBand_Measurement.push_back(sRxOutOfBandMeasRpt);
        }

        throw DP_TEST_PASS;
    } catch (S32BIT s32Exception) {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NetworkAnalyzerSel);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to Power OFF in the Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        sRetVal = OnSafeState();
        if(sRetVal)
        {
            *out_bTestSts = false;
            strActLog = QString::asprintf("%s","Unable to insert report records to database");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }
    }
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Rx Out Of Band Rejection Test Completed");

    return DP_SUCCESS;
}

short CAutoTestThread ::PhaseNoiseMeasurement(bool *out_bTestSts, char* in_szTestname)
{
    short sRetVal                   = 0;
    U32BIT u32TestCaseID            = 0;
    U8BIT u8RFChnNumber             = 0;
    double dSGinPower               = 0;
    U8BIT u8SigGenSel               = 1;
    U8BIT u8SpecAnSel               = 1;
    U8BIT u8ChIdx                   = 1;
    FDOUBLE dStimulasPathloss       = 0;
    FDOUBLE dMeasuredPathloss       = 0;
    unsigned char ucPrecision       = 2;
    QString strActLog               = "";
    QString strConnecterMsg         = "";
    QString strConnecter            = "";
    QString strErrorMsg = "";
    FDOUBLE dOutputFreq = 0.0;

    double dRefLevel = 0.0;
    double dRBW = 0.0;
    double dVBW = 0.0;
    double dSweepTime = 0.0;
    double dSpan = 0.0;
    double dPeakFreq = 0.0;
    double dMarkerAmp = 0.0;

    QString qstr_testname = "";

    unsigned short u16OChannelNo[2] = {0};

    *out_bTestSts = 1;

    S_MODULENAME_PHASENOISEMEASUREMENT s_Param_PhaseNoiseMeasurement; /*for input parameters from database*/

    S_PHASE_NOISE_MEASUREMENT s_Rpt_PhaseNoiseMeasurement; /*For report to database*/

    if(in_szTestname == NULL)
    {
        return DP_APP_ERR_INVALID_PARAM;
    }

    qstr_testname = QString::asprintf("%s", in_szTestname);

    /*Read Test case ID*/
    sRetVal = m_pobjReportWrapper->ReadTestCaseID(qstr_testname.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s", "Unable to read Test Case ID ");
        emit Sig_Fail_State(out_bTestSts, strErrorMsg);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Preset in RF SG
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SigGenSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure Preset in RF Signal Generator");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Reset in RF SA
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(u8SpecAnSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure Reset in Spectrum Analyzer");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }


#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Phase Noise mode in RF SA
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ModeConfig(u8SpecAnSel, PHASE_NOISE_MODE);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure center frequency and Span in Spectrum Analyzer");
        throw DP_TEST_FAIL;
    }

    /*Read Test case configuration parameter*/
    memset(&s_Rpt_PhaseNoiseMeasurement,0, sizeof(S_PHASE_NOISE_MEASUREMENT));
    s_Param_PhaseNoiseMeasurement.m_vectFREQUENCYTABLE.clear();
    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &s_Param_PhaseNoiseMeasurement);
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s", "Unable to read configuration data");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

    s_Rpt_PhaseNoiseMeasurement.m_dInputPower = s_Param_PhaseNoiseMeasurement.m_dInputPower;
    s_Rpt_PhaseNoiseMeasurement.m_dExpectedPhaseNoiseLevel = s_Param_PhaseNoiseMeasurement.m_dExpectedMinPhaseNoise;

    dRefLevel = s_Param_PhaseNoiseMeasurement.m_dReferenceLevel;
    dRBW = s_Param_PhaseNoiseMeasurement.m_dRBW;
    dVBW = s_Param_PhaseNoiseMeasurement.m_dVBW;
    dSweepTime = s_Param_PhaseNoiseMeasurement.m_dSweepTime;
    dSpan = s_Param_PhaseNoiseMeasurement.m_dSpan;

    strConnecterMsg = QString::asprintf("Connect LO1 Output Channel-%d Port to Spectrum Analyzer\n1. Press Continue button to start testing \n2. Press Skip button to skip", u8RFChnNumber);
    emit sig_QusToUserContinueSkip((char*)strConnecterMsg.toStdString().c_str());
    strConnecterMsg = QString::asprintf("Channel-%d Measurement started", u8RFChnNumber);
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strConnecterMsg);

    if(g_ucPressYES_NO == 1)
    {
        try
        {
            for(U32BIT u32FreqTblIdx = 0; u32FreqTblIdx < s_Param_PhaseNoiseMeasurement.m_vectFREQUENCYTABLE.size(); u32FreqTblIdx++)
            {
                dOutputFreq = s_Param_PhaseNoiseMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency;
#ifndef _SIMULATION_INSTRUMENTS_
                // Apply Calib Data for RF Stimulas Path
                sRetVal = ApplyCalibData(s_Param_PhaseNoiseMeasurement.m_s8StimulasPathID , s_Param_PhaseNoiseMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency, s_Param_PhaseNoiseMeasurement.m_dInputPower , &dSGinPower, &dStimulasPathloss);
#else
                sRetVal = 0;
#endif
                if(sRetVal)
                {
                    strErrorMsg = QString::asprintf("Unable to Read the RF stimulus path calibration data ");
                    throw DP_TEST_FAIL;
                }

#ifndef _SIMULATION_INSTRUMENTS_
                sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel, (s_Param_PhaseNoiseMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency * 1e6), dSGinPower);
#else
                sRetVal = 0;
#endif
                if(sRetVal)
                {
                    strErrorMsg = QString::asprintf("%s","Unable to configure frequency and power level in signal generator");
                    throw DP_TEST_FAIL;
                }


#ifndef _SIMULATION_INSTRUMENTS_
                // Configure Phase Noise in RF SA
                sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigPNMeasurement(u8SpecAnSel, (dOutputFreq * 1e6), (s_Param_PhaseNoiseMeasurement.m_dPNStartFrequency * 1e6), (s_Param_PhaseNoiseMeasurement.m_dPNStopFrequency * 1e6),  dRefLevel, (dSpan * 1e6));
#else
                sRetVal = 0;
#endif
                if(sRetVal)
                {
                    strErrorMsg = QString::asprintf("%s","Unable to Configure center frequency and Span in Spectrum Analyzer");
                    throw DP_TEST_FAIL;
                }

#ifndef _SIMULATION_INSTRUMENTS_
                // Enable Rf out in RF SG
                sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, true);
#else
                sRetVal = 0;
#endif
                if(sRetVal)
                {
                    strErrorMsg = QString::asprintf("%s","Unable to disable RF output");
                    throw DP_TEST_FAIL;
                }

#ifndef _SIMULATION_INSTRUMENTS_
                // Configure Start and Stop frequency in RF SA
                sRetVal = p_objMainApp->m_objSAWrapper.Read_PhaseNoise(u8SpecAnSel, 1, 100, &dMarkerAmp);
#else
                sRetVal = 0;
#endif
                if(sRetVal)
                {
                    strErrorMsg = QString::asprintf("%s","Unable to Configure RBW, VBW and Sweep time in Spectrum Analyzer");
                    emit Sig_Fail_State( out_bTestSts,strErrorMsg);
                    return DP_FAILURE;
                }

                s_Rpt_PhaseNoiseMeasurement.m_s8ConnectorID = u8RFChnNumber;
                s_Rpt_PhaseNoiseMeasurement.m_dInputFrequency = s_Param_PhaseNoiseMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency;
                s_Rpt_PhaseNoiseMeasurement.m_dInputPower = s_Param_PhaseNoiseMeasurement.m_dInputPower;
                s_Rpt_PhaseNoiseMeasurement.m_dExpectedOutputFrequency = dOutputFreq;
                s_Rpt_PhaseNoiseMeasurement.m_dOutputFrequency = dOutputFreq;
                s_Rpt_PhaseNoiseMeasurement.m_dObservedPhaseNoiseLevel = dMarkerAmp;
                s_Rpt_PhaseNoiseMeasurement.m_dStimulasPathLoss = dStimulasPathloss;

                dproundoff(s_Rpt_PhaseNoiseMeasurement.m_dObservedPhaseNoiseLevel , ucPrecision);
                dproundoff(dStimulasPathloss, ucPrecision);

                long lPhaseNoise 		= (long)(dproundoff(s_Rpt_PhaseNoiseMeasurement.m_dObservedPhaseNoiseLevel, ucPrecision));
                long lExpMin            = (long)(dproundoff(s_Rpt_PhaseNoiseMeasurement.m_dExpectedPhaseNoiseLevel, ucPrecision));

                //Result
                strActLog = QString::asprintf("CH - %d , I/P Freqency: %0.3f MHz,  I/P Power: %0.2f dBm, Exp. Phase Noise : %0.2f dBc, Meas. Phase Noise : %0.2f dBc"\
                                              ,s_Rpt_PhaseNoiseMeasurement.m_s8ConnectorID , s_Rpt_PhaseNoiseMeasurement.m_dInputFrequency , s_Rpt_PhaseNoiseMeasurement.m_dInputPower, s_Rpt_PhaseNoiseMeasurement.m_dExpectedPhaseNoiseLevel, s_Rpt_PhaseNoiseMeasurement.m_dObservedPhaseNoiseLevel);

                if(lPhaseNoise > lExpMin)
                {
                    s_Rpt_PhaseNoiseMeasurement.m_u8Result = DP_TEST_FAIL;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                    *out_bTestSts = 0;
                }
                else
                {
                    s_Rpt_PhaseNoiseMeasurement.m_u8Result = DP_TEST_PASS;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
                }

                if(g_bAutoTestStartStop == false)
                {
                    s_Rpt_PhaseNoiseMeasurement.m_u8Result = DP_TEST_STOPPED;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "LO1 Output Power Measurement Test stopped");
                    throw DP_TEST_STOPPED;
                }

                QVectRx_PhaseNoise_Measurement.push_back(s_Rpt_PhaseNoiseMeasurement);
            }//Qvector Size end
            strConnecterMsg = QString::asprintf("Channel-%d Measurement Completed", u8RFChnNumber);
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strConnecterMsg);
            throw DP_TEST_PASS;
        }
        catch(S32BIT s32Exception)
        {
#ifndef _SIMULATION_INSTRUMENTS_
            // Enable Rf out in RF SG
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, false);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("%s","Unable to disable RF output");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Configure Phase Noise mode in RF SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ModeConfig(u8SpecAnSel, SPECTRUM_MODE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("%s","Unable to Configure center frequency and Span in Spectrum Analyzer");
                throw DP_TEST_FAIL;
            }


            sRetVal = OnSafeState();
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("%s","Unable to insert report records to database");
                emit Sig_Fail_State( out_bTestSts,strErrorMsg);
                return DP_FAILURE;
            }

            if(s32Exception == DP_TEST_FAIL)
            {
                emit Sig_Fail_State(out_bTestSts, strErrorMsg);
                return s32Exception;
            }
            else if(s32Exception == DP_TEST_STOPPED)
            {
                return s32Exception;
            }
        }

    }//Question user end
    else
    {
        *out_bTestSts = 0;
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "LO1 Output Power Measurement Test stopped");
        return DP_TEST_STOPPED;
    }

    sRetVal = OnSafeState();
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to insert report records to database");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

    p_objMainApp->m_bIsTestActive  = 0;

    return DP_SUCCESS;
}

short CAutoTestThread::RxGainMeasuremnt_SA(bool *out_bTestSts, char *in_szTestname)
{
    U8BIT u8RFChnNumber             = 0;
    U8BIT u8IFChnNumber             = 0;
    U32BIT u32TableStructsize = 0;
    U8BIT u8RFDCA1 = 0;
    U32BIT u32TestCaseID        = 0;
    U32BIT u32CfgNumber         = 0;
    U8BIT u8BandSel  = 0;
    U8BIT u8GainEqPathSel  = 0;
    U8BIT u8AtteBandSel = 0;
    FDOUBLE dIFFreq   =   0.0f;
    short sRetVal               = 0;
    double dMeasPwr             = 0.0f;
    double dSAoutPower          = 0.0f;
    double dSGinPower           = 0.0f;
    char szErrMsg[512]          = {0};
    QString strActLog           = "";
    QString strTestName         = "";
    QString strConnecterMsg     = "";
    QString strFilename         = "";
    QString strFileData         = "";
    QStringList strCalibDataList = {""};

    *out_bTestSts = 1;
    S_MODULENAME_GAINMEASUREMENTSA sNBRFLRUGainMeasurement;

    _S_RX_GAIN_MEASUREMENT_SA sRxRepGainMeasurement;
    memset(&sRxRepGainMeasurement, 0, sizeof(_S_RX_GAIN_MEASUREMENT_SA));

    if(in_szTestname == NULL)
    {
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read Test Case ID ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sNBRFLRUGainMeasurement);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read configuration data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Preset in RF SG
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(sNBRFLRUGainMeasurement.m_u8SigGenSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to Configure Preset in RF Signal Generator for Gain Measurement");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
    // Config Ref Level In SA
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(sNBRFLRUGainMeasurement.m_u8SpecSel, sNBRFLRUGainMeasurement.m_dRefLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to configure reference level Gain Measurement ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
#ifndef _SIMULATION_INSTRUMENTS_
    // Config  RBW , VBW in SA
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(sNBRFLRUGainMeasurement.m_u8SpecSel, 0, (sNBRFLRUGainMeasurement.m_dRbw * 1e3), 0, (sNBRFLRUGainMeasurement.m_dVbw* 1e3), true, sNBRFLRUGainMeasurement.m_dSweepTime);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to configure RBW, VBW and sweep time in Gain Measurement");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    u32TableStructsize = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.size();

    for(U32BIT u32Idx = 0; u32Idx < u32TableStructsize ; u32Idx++)
    {
        U8BIT u8TotBand = 0;
        bool bIsEdgeFreq = false;
        FDOUBLE dIPFreq = 0.0;

        sRxRepGainMeasurement.m_dExpMaxGain         = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.at(u32Idx).m_dExpMaxGain;
        sRxRepGainMeasurement.m_dExpMinGain         = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.at(u32Idx).m_dExpMinGain;
        sRxRepGainMeasurement.m_dRf_inputfreq       = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.at(u32Idx).m_dInputFreq;
        sRxRepGainMeasurement.m_dRf_inputPower      = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.at(u32Idx).m_dInputPower;
        u8BandSel                                   = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.at(u32Idx).m_u8BandSel;
        u8AtteBandSel                                   = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.at(u32Idx).m_u8GainEqPathSel;/*Temperarly I am using this variable as a atteBandSel*/
        dIFFreq = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.at(u32Idx).m_dIFFrequency;
        dIPFreq = sNBRFLRUGainMeasurement.m_vectGAIN_CFG_DETAILS.at(u32Idx).m_dRFDCA_2;/*Temperarly I am using this variable as a Band cofigure center Frequency*/

#ifndef _SIMULATION_INSTRUMENTS_
        // Apply Calib Data for RF Stimulas Path
        sRetVal = ApplyCalibData(sNBRFLRUGainMeasurement.m_u16StimulusPathId, sRxRepGainMeasurement.m_dRf_inputfreq , sRxRepGainMeasurement.m_dRf_inputPower , &dSGinPower, &sRxRepGainMeasurement.m_dRFCableloss);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to Read the RF stimulus path calibration data ");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Config Center Freq and Span in SA
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(sNBRFLRUGainMeasurement.m_u8SpecSel, (dIFFreq * 1e6), (sNBRFLRUGainMeasurement.m_dSpan * 1e6));
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to configure center frequency and span in Gain Measurement ");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        // Configure SigGen freq and power for RF
#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(sNBRFLRUGainMeasurement.m_u8SigGenSel, (sRxRepGainMeasurement.m_dRf_inputfreq * 1e6), dSGinPower);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to configure frequency and power level in signal generator for Gain Measurement Measurement");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        // Enable Rf out in RF SG
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(sNBRFLRUGainMeasurement.m_u8SigGenSel, true);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to disable RF output in RF signal generator for Gain Measurement ");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Enable continuous run
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(sNBRFLRUGainMeasurement.m_u8SpecSel, DP_ATE_7343_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer Gain Measurement");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(sNBRFLRUGainMeasurement.m_u32delay);   // Delay to Settle
#ifndef _SIMULATION_INSTRUMENTS_
        // Disable continuous run
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(sNBRFLRUGainMeasurement.m_u8SpecSel, DP_ATE_7343_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to disable the continous run in spectrum analyzer for Gain Measurement");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        // Read MArker Frequency
        sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(sNBRFLRUGainMeasurement.m_u8SpecSel, (sNBRFLRUGainMeasurement.m_u8MarkerPos * 1e6), &dMeasPwr);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to read marker frequency in spectrum analyzer for Gain Measurement");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        // Disable RF Out in SG
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(sNBRFLRUGainMeasurement.m_u8SigGenSel, false);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to disable RF output in RF signal generator for Gain Measurement");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Apply Calib DATA for SA
        sRetVal = ApplyCalibData(sNBRFLRUGainMeasurement.m_u16MeasurementPathID, dIFFreq, dMeasPwr, &dSAoutPower, &sRxRepGainMeasurement.m_dMeasCableloss);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to Read the SA path calibration data for Gain Measurement");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
        sRxRepGainMeasurement.m_dMesured_Power = dMeasPwr;
        sRxRepGainMeasurement.m_dCalculated_Power	 = dSAoutPower;
        sRxRepGainMeasurement.m_dCalculated_Gain   =	dSAoutPower - sRxRepGainMeasurement.m_dRf_inputPower;

        //Result
        strActLog = QString::asprintf("CH : %d, I/P Freqency: %0.3f MHz,  I/P : %0.2f dBm, Exp. Min. Gain : %0.2f dB, Exp. Max. Gain : %0.2f dB, Meas. O/P Power : %0.2f dBm, Calc. Gain : %0.2f dB",sRxRepGainMeasurement.m_s32ChannelNo, sRxRepGainMeasurement.m_dRf_inputfreq, sRxRepGainMeasurement.m_dRf_inputPower, sRxRepGainMeasurement.m_dExpMinGain, sRxRepGainMeasurement.m_dExpMaxGain, sRxRepGainMeasurement.m_dCalculated_Power, sRxRepGainMeasurement.m_dCalculated_Gain);
        dproundoff(sRxRepGainMeasurement.m_dMesured_Power, sNBRFLRUGainMeasurement.m_u8Precision);
        dproundoff(sRxRepGainMeasurement.m_dMeasCableloss, sNBRFLRUGainMeasurement.m_u8Precision);
        dproundoff(sRxRepGainMeasurement.m_dRFCableloss, sNBRFLRUGainMeasurement.m_u8Precision);
        dproundoff(sRxRepGainMeasurement.m_dCalculated_Power, sNBRFLRUGainMeasurement.m_u8Precision);

        long lGain 		= (long)(dproundoff(sRxRepGainMeasurement.m_dCalculated_Gain, sNBRFLRUGainMeasurement.m_u8Precision)   * pow(10, sNBRFLRUGainMeasurement.m_u8Precision));
        long lExpMin 	= (long)(dproundoff(sRxRepGainMeasurement.m_dExpMinGain, sNBRFLRUGainMeasurement.m_u8Precision) * pow(10, sNBRFLRUGainMeasurement.m_u8Precision));
        long lExpMax 	= (long)(dproundoff(sRxRepGainMeasurement.m_dExpMaxGain, sNBRFLRUGainMeasurement.m_u8Precision) * pow(10, sNBRFLRUGainMeasurement.m_u8Precision));

        if(( lGain < lExpMin) || (lGain > lExpMax) )
        {
            sRxRepGainMeasurement.m_s32Result = DP_APP_FAIL;
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            *out_bTestSts = 0;
        }
        else
        {
            sRxRepGainMeasurement.m_s32Result = DP_APP_PASS;
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
        }

        //Adding to array
        if(g_bAutoTestStartStop == false)
        {
            sRxRepGainMeasurement.m_s32Result = 2;
            QVectRx_Gain_Measurement_SA.push_back(sRxRepGainMeasurement);
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to report record insert into database");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Gain Measurement Test stopped");
            return DP_DP_APP_TEST_STOPPED;
        }

        QVectRx_Gain_Measurement_SA.push_back(sRxRepGainMeasurement);
    }//Qvector Size end

    sRetVal = OnSafeState();
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to report record insert into database");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    return DP_SUCCESS;
}

short CAutoTestThread ::Receiver1dBMeasurement(bool *out_bTestSts, char *in_szTestname)
{
    U8BIT u8AtteBandSel = 0;
    U8BIT u8GainEqPathSel           = 0;
    U8BIT u8RFChnNumber             = 0;
    U8BIT u8IFChnNumber             = 0;
    U8BIT u8BandSel                 = 0;
    U8BIT u8SigGenSel               = 1;
    U8BIT u8SpecAnaSel              = 1;
    U32BIT u32TableStructsize = 0;
    U32BIT u32TestCaseID        = 0;
    U32BIT u32CfgNumber         = 0;
    U8BIT u8Is1dBCompReached    = 0;
    short sRetVal               = 0;
    FDOUBLE dMeasPwr             = 0.0f;
    FDOUBLE dSAoutPower          = 0.0f;
    FDOUBLE dSGinPower           = 0.0f;
    FDOUBLE dIFFreq = 0.0f;
    FDOUBLE dCenterFrequency = 0.0f;
    char szErrMsg[512]          = {0};
    U8BIT u8IsRefGain   = 0;
    FDOUBLE dGainDiff            = 0.0f;
    FDOUBLE dExpMinGain          = 0.0f;
    FDOUBLE dExpMaxGain          = 0.0f;
    U64BIT ulPrevFreq    = 0;
    QString strActLog           = "";
    QString strTestName         = "";
    QString strConnecterMsg     = "";
    *out_bTestSts = 1;

    S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT sReceiver1dBCompressionPoint;

    S_ONEDBCOMPRESSIONPOINT s1dBCompressionPointReport;
    memset(&s1dBCompressionPointReport, 0, sizeof(S_ONEDBCOMPRESSIONPOINT));

    if(in_szTestname == NULL)
    {
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read Test Case ID ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sReceiver1dBCompressionPoint);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read configuration data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Preset in RF SG
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SigGenSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to Configure Preset in RF Signal Generator for Receiver 1dB compression point validation");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(u8SpecAnaSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s","Unable to Configure Preset in Signal Analyzer for Receiver 1dB compression point validation");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
    // Config Ref Level In SA
    sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SpecAnaSel, sReceiver1dBCompressionPoint.m_dRefLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to configure reference level for Receiver 1dB compression point validation");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Config  RBW , VBW in SA
    sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SpecAnaSel, 0, (sReceiver1dBCompressionPoint.m_dRBW * 1e3), 0, (sReceiver1dBCompressionPoint.m_dVBW * 1e3), true, sReceiver1dBCompressionPoint.m_dSweepTime);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to configure RBW, VBW and sweep time for Receiver 1dB compression point validation");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }


    u32TableStructsize = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.size();

    for(U32BIT u32Idx = 0; u32Idx < u32TableStructsize ; u32Idx++)
    {
        dExpMaxGain = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_dExpMaxGain;
        dExpMinGain = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_dExpMinGain;
        s1dBCompressionPointReport.m_dRFInptFreq = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_dFrequency;
        s1dBCompressionPointReport.m_dRFpwr = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_dInputPower;
        u8BandSel = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_u8BandSel;
        u8IsRefGain = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_u8IsRefGain;
        u8AtteBandSel                                   = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_u8GainEqPathSel;/*Temperarly I am using this variable as a atteBandSel*/
        dCenterFrequency = sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_dCenterFreq;
        dIFFreq =  sReceiver1dBCompressionPoint.m_vectCFGDETAILS.at(u32Idx).m_dIFFrequency;

        if( (ulPrevFreq == (U64BIT)(s1dBCompressionPointReport.m_dRFInptFreq /* 1000000.0*/)) && (u8Is1dBCompReached == 1) )
        {
            continue;
        }

        u8Is1dBCompReached = 0;

#ifndef _SIMULATION_INSTRUMENTS_
        // Config Center Freq and Span in SA
        sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnaSel, (dIFFreq * 1e6), (sReceiver1dBCompressionPoint.m_dSpan * 1e6));
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to configure center frequency and span for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        /*Configuration*/

#ifndef _SIMULATION_INSTRUMENTS_
        // Apply Calib Data for RF Stimulas Path
        sRetVal = ApplyCalibData(sReceiver1dBCompressionPoint.m_u16StimulusPathId, s1dBCompressionPointReport.m_dRFInptFreq, s1dBCompressionPointReport.m_dRFpwr, &dSGinPower, &s1dBCompressionPointReport.m_dRFCableloss);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to Read the RF stimulus path calibration data for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        // Configure SigGen freq and power for RF
#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel, (s1dBCompressionPointReport.m_dRFInptFreq * 1e6), dSGinPower);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to configure frequency and power level in signal generator for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        // Enable Rf out in RF SG
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, true);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to disable RF output in RF signal generator for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Enable continuous run
        sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ATE_7343_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(sReceiver1dBCompressionPoint.m_u64Delay);

#ifndef _SIMULATION_INSTRUMENTS_
        // Disable continuous run
        sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ATE_7343_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to disable the continous run in spectrum analyzer for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Read MArker Frequency
        sRetVal =   p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SpecAnaSel, (dIFFreq * 1e6), &dMeasPwr);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to read marker frequency in spectrum analyzer for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Disable RF Out in SG
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, false);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to disable RF output in RF signal generator for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Apply Calib DATA for SA
        sRetVal = ApplyCalibData(sReceiver1dBCompressionPoint.m_u16MeasurementPathId, dIFFreq, dMeasPwr, &dSAoutPower, &s1dBCompressionPointReport.m_dMeasCableloss);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to Read the SA path calibration data for Receiver 1dB compression point validation");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        s1dBCompressionPointReport.m_dMeasActPwr = dSAoutPower;

        if(u8IsRefGain == 1)
        {
            s1dBCompressionPointReport.m_dRefGain = dSAoutPower - s1dBCompressionPointReport.m_dRFpwr;
            s1dBCompressionPointReport.m_dCalcGain = dSAoutPower - s1dBCompressionPointReport.m_dRFpwr;
        }
        else if(u8IsRefGain == 0)
        {
            s1dBCompressionPointReport.m_dCalcGain = dSAoutPower - s1dBCompressionPointReport.m_dRFpwr;

            dGainDiff = s1dBCompressionPointReport.m_dRefGain - s1dBCompressionPointReport.m_dCalcGain;

            if( dGainDiff >= 1.0)
            {
                u8Is1dBCompReached = 1;
            }
        }

        s1dBCompressionPointReport.m_s32ChannelNo = u8RFChnNumber;
        if(u8IsRefGain == 0)
        {
            strActLog = QString::asprintf("CH No. : %d I/P Frequency : %0.2f MHz I/P Power : %0.2f dBm  Measured Power : %0.2f dBm Calc. Gain : %0.2f dB", \
                                          s1dBCompressionPointReport.m_s32ChannelNo, s1dBCompressionPointReport.m_dRFInptFreq, s1dBCompressionPointReport.m_dRFpwr, s1dBCompressionPointReport.m_dMeasActPwr, s1dBCompressionPointReport.m_dCalcGain);
        }
        else
        {
            strActLog = QString::asprintf("CH No. : %d I/P Frequency : %0.2f MHz I/P Power : %0.2f dBm Reference Gain : %0.2f dB, Measured Power : %0.2f dBm, Calc. Gain : %0.2f dB", \
                                          s1dBCompressionPointReport.m_s32ChannelNo, s1dBCompressionPointReport.m_dRFInptFreq, s1dBCompressionPointReport.m_dRFpwr, s1dBCompressionPointReport.m_dRefGain, s1dBCompressionPointReport.m_dMeasActPwr, s1dBCompressionPointReport.m_dCalcGain);
        }

        long lCalGain = (long)(dproundoff(s1dBCompressionPointReport.m_dRefGain, sReceiver1dBCompressionPoint.m_u8Precision) * pow(10, sReceiver1dBCompressionPoint.m_u8Precision));
        long lExpMinGain = (long)(dproundoff(dExpMinGain, sReceiver1dBCompressionPoint.m_u8Precision) * pow(10, sReceiver1dBCompressionPoint.m_u8Precision));
        long lExpMaxGain = (long)(dproundoff(dExpMaxGain, sReceiver1dBCompressionPoint.m_u8Precision) * pow(10, sReceiver1dBCompressionPoint.m_u8Precision));

        dproundoff(s1dBCompressionPointReport.m_dMeasCableloss, sReceiver1dBCompressionPoint.m_u8Precision);
        dproundoff(s1dBCompressionPointReport.m_dRFCableloss, sReceiver1dBCompressionPoint.m_u8Precision);

        if( (lCalGain < lExpMinGain) || (lCalGain > lExpMaxGain))
        {
            s1dBCompressionPointReport.m_s32Result = DP_APP_FAIL;
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            *out_bTestSts = 0;
        }
        else
        {
            if(u8Is1dBCompReached == 0)
            {
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strActLog);
            }
            s1dBCompressionPointReport.m_s32Result = DP_APP_PASS;
        }

        if(u8Is1dBCompReached)
        {
            s1dBCompressionPointReport.m_d1dBComPoint = s1dBCompressionPointReport.m_dRFpwr;
            strActLog = QString::asprintf("1dB Compression point reached at input power : %lf (dBm), Measured Power : %0.2f dBm, Calc. Gain : %0.2f dB", s1dBCompressionPointReport.m_dRFpwr,  s1dBCompressionPointReport.m_dMeasActPwr, s1dBCompressionPointReport.m_dCalcGain);
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strActLog);
        }

        if(g_bAutoTestStartStop == false)
        {
            s1dBCompressionPointReport.m_s32Result  = DP_DP_APP_TEST_STOPPED;
            QVectReceiver1dBCompressionPoint.push_back(s1dBCompressionPointReport);
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to report record insert into database");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Receiver 1dB compression point validation Test stopped");
            return DP_DP_APP_TEST_STOPPED;
        }
        QVectReceiver1dBCompressionPoint.push_back(s1dBCompressionPointReport);

        ulPrevFreq = (U64BIT)(s1dBCompressionPointReport.m_dRFInptFreq/* * 1000000.0*/);

    }/*Qvector size end*/


    sRetVal = OnSafeState();
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to report record insert into database");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    return DP_SUCCESS;
}

short CAutoTestThread ::VSWRMeasurement(bool *out_bTestSts, char *in_szTestname)
{
    U8BIT u8IFChnNumber = 0;
    U8BIT u8RFChnNumber = 0;
    U8BIT u8BandSel         = 0;
    U8BIT u8GainEqPathSel  = 0;
    U8BIT u8AtteBandSel = 0;
    U8BIT u8NetworkAnalyzerSel  = 1;
    U8BIT u8NoofTrace           = 0;
    U8BIT u8Precision   = 2;
    U8BIT u8PortNum     = 0;
    S16BIT sRetVal               = 0;
    U16BIT u16Trace      = 0;
    U32BIT u32TableStructsize   = 0;
    U32BIT u32TestCaseID        = 0;
    U32BIT u32CfgNumber         = 0;
    FDOUBLE dMarkerMaxData       = 0.0f;
    FDOUBLE dMarkerMinData       = 0.0f;
    FDOUBLE dMarkerMaxFreq       = 0.0f;
    FDOUBLE dMarkerMinFreq       = 0.0f;
    FDOUBLE dBandCenterFrequency = 0.0f;
    S8BIT szStateFilepath[512]   = {0};
    S8BIT szErrMsg[512]          = {0};
    QString strActLog           = "";
    QString strTestName         = "";
    QString strStateFilePath    = "D:\\NBRFLRU_CALIB_DATA";

    S_MODULENAME_VSWRMEASUREMENT sVSWRMeasurement;
    S_VSWR_MEASUREMENT sVSWRMeasurementReport;

    memset(&sVSWRMeasurementReport, 0, sizeof(S_VSWR_MEASUREMENT));

    strcpy(szStateFilepath, strStateFilePath.toStdString().c_str());

    if(in_szTestname == NULL)
    {
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read Test Case ID ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sVSWRMeasurement);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read configuration data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    strTemp = QString::asprintf("Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser((char *)strTemp.toStdString().c_str());
    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(sVSWRMeasurement.m_dStartFrequency, sVSWRMeasurement.m_dStopFrequency, sVSWRMeasurement.m_dInputPower, sVSWRMeasurement.m_dDataPoints, strStateFilePath,1);
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

    } // calib end

#ifndef _SIMULATION_INSTRUMENTS_
    //Recall State file
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NetworkAnalyzerSel, szStateFilepath);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to load state file in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sleep(1);

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Datapoints
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetIFBandWidth(u8NetworkAnalyzerSel, 10000.0);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to set IF Band width in Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Power level in NA
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NetworkAnalyzerSel, sVSWRMeasurement.m_dInputPower, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to configure power Level in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    msleep(100);

    u32TableStructsize = sVSWRMeasurement.m_vectCFGDETAIL.size();

    for(U32BIT u32IDx = 0; u32IDx < u32TableStructsize; u32IDx++)
    {
        sVSWRMeasurementReport.m_dinputPower = sVSWRMeasurement.m_dInputPower;
        sVSWRMeasurementReport.m_dExpectedVSWR = sVSWRMeasurement.m_vectCFGDETAIL.at(u32IDx).m_dExpMaxVswr;
        sVSWRMeasurement.m_dStartFrequency = sVSWRMeasurement.m_vectCFGDETAIL.at(u32IDx).m_dStartFreq;
        sVSWRMeasurement.m_dStopFrequency = sVSWRMeasurement.m_vectCFGDETAIL.at(u32IDx).m_dStopFreq;
        u8BandSel = sVSWRMeasurement.m_vectCFGDETAIL.at(u32IDx).m_u8BandSel;
        dBandCenterFrequency = sVSWRMeasurement.m_vectCFGDETAIL.at(u32IDx).m_dBandFreq;
        u8AtteBandSel =sVSWRMeasurement.m_vectCFGDETAIL.at(u32IDx).m_u8AttBandSel;

        sVSWRMeasurementReport.m_s32StartFreq = sVSWRMeasurement.m_dStartFrequency;
        sVSWRMeasurementReport.m_s32StopFreq = sVSWRMeasurement.m_dStopFrequency;

        for(u8NoofTrace = 0; u8NoofTrace < 1; u8NoofTrace++)
        {

            if(u8NoofTrace == 0)
            {
                u16Trace = 1;//S11 trace
            }
            else
            {
                u16Trace = 4;//S22 trace
            }

#ifndef _SIMULATION_INSTRUMENTS_
            //Select trace
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NetworkAnalyzerSel, sVSWRMeasurement.m_dDataPoints, u16Trace, 1, 1);//trace, port, channel
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(100);

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDisplayFormat(u8NetworkAnalyzerSel, DP_SWR_MODE, u16Trace);  // 1 ch no
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to config Display format in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(480);
#ifndef _SIMULATION_INSTRUMENTS_
            //Set Start Frequency
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NetworkAnalyzerSel, (sVSWRMeasurement.m_dStartFrequency * 1e6), 1);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to config Start Frequency in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            //Set Stop frequency
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NetworkAnalyzerSel, (sVSWRMeasurement.m_dStopFrequency * 1e6), 1);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to config Stop Frequency in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            if(u32IDx == 0)
            {
                msleep(3000);
            }

            /**TODO Configure */


#ifndef _SIMULATION_INSTRUMENTS_
            //Set Power level in NA
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NetworkAnalyzerSel, sVSWRMeasurement.m_dInputPower, 1);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to config Power Level in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(50);

#ifndef _SIMULATION_INSTRUMENTS_
            //Set Trigger On
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }


#ifndef _SIMULATION_INSTRUMENTS_
            //Set the RF Output Power ON
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NetworkAnalyzerSel);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to Turn ON RF in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
            //Set Trigger On
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_HOLD);//continuous
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetMarkerSearchType(u8NetworkAnalyzerSel, u16Trace, 1, DP_NA_MARKER_SEARCH_MAX, &dMarkerMaxData, &dMarkerMaxFreq);  //id , channel no, marker no, marker 1 - peak , marker data
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to read Max. Marker Value in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetMarkerSearchType(u8NetworkAnalyzerSel, u16Trace, 1 ,DP_NA_MARKER_SEARCH_MIN, &dMarkerMinData, &dMarkerMinFreq);  //id , channel no, marker no, marker 1 - peak , marker data
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s", "Unable to read Min. Marker Value in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            //Power Off
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NetworkAnalyzerSel);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strActLog = QString::asprintf("%s", "Unable to Power OFF in the Network Analyzer" );
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            if(u8NoofTrace == 0)
            {
                sVSWRMeasurementReport.m_dInputPortVSWR = dMarkerMaxData;
                sVSWRMeasurementReport.m_dInputPortMaxFreq  = dMarkerMaxFreq/1000000;
            }
            else
            {
                sVSWRMeasurementReport.m_dOutputPortVSWR = dMarkerMaxData;
                sVSWRMeasurementReport.m_dOutputPortMaxFreq = dMarkerMaxFreq/1000000;
            }
        }//trace Loop

        long lRetIPPortloss 	= (long)(dproundoff(sVSWRMeasurementReport.m_dInputPortVSWR, u8Precision) * pow(10, u8Precision));
        long lReOPPortloss 	= (long)(dproundoff(sVSWRMeasurementReport.m_dOutputPortVSWR, u8Precision) * pow(10, u8Precision));
        long lExpIPPortMax 	= (long)(dproundoff(sVSWRMeasurementReport.m_dExpectedVSWR, u8Precision) * pow(10, u8Precision));

        strActLog = QString::asprintf("Ch No. : %d, Port : %s, Start Freq.: %0.3f MHz, Stop Freq.: %0.3f MHz, Exp. Max. Return Loss: %0.2f, Measured Return Loss : [ Input: %0.2f @ %0.3f MHz, Output.: %0.2f @ %0.3f MHz]",\
                                      sVSWRMeasurementReport.m_s32ChannelNo, ((u8NoofTrace == 0)? "Input" : "Output"), sVSWRMeasurement.m_dStartFrequency, sVSWRMeasurement.m_dStopFrequency, sVSWRMeasurement.m_dExpMaxRetLoss, sVSWRMeasurementReport.m_dInputPortVSWR, sVSWRMeasurementReport.m_dInputPortMaxFreq,sVSWRMeasurementReport.m_dOutputPortVSWR, sVSWRMeasurementReport.m_dOutputPortMaxFreq);


        if(lRetIPPortloss <= lExpIPPortMax /*|| lReOPPortloss <= lExpIPPortMax*/)
        {
            sVSWRMeasurementReport.m_s32Result = DP_APP_PASS;
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
        }
        else
        {
            sVSWRMeasurementReport.m_s32Result = DP_APP_FAIL;
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            *out_bTestSts = false;
        }

        if(g_bAutoTestStartStop == false)
        {
            sVSWRMeasurementReport.m_s32Result = DP_DP_APP_TEST_STOPPED;
            QVectVSWRMeasurement.push_back(sVSWRMeasurementReport);
            strTemp = QString::asprintf("%s","VSWR Measurement test stopped");
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to insert to database");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            return DP_DP_APP_TEST_STOPPED;
        }
        QVectVSWRMeasurement.push_back(sVSWRMeasurementReport);
    }/*QVector size end*/


    sRetVal = OnSafeState();
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to report record insert into database");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    return DP_SUCCESS;
}

short CAutoTestThread::RxSpuriousMeasuremnt(bool *out_bTestSts, char *in_szTestname)
{
    short sRetVal                   = 0;
    U32BIT u32TestCaseID            = 0;
    U8BIT u8RFChnNumber             = 0;
    double dSGinPower               = 0;
    double dOutputPower              = 0;
    U8BIT u8SigGenSel               = 1;
    U8BIT u8SpecAnSel               = 1;
    U8BIT u8ChIdx                   = 1;
    FDOUBLE dStimulasPathloss       = 0;
    FDOUBLE dMeasuredPathloss       = 0;
    FDOUBLE dOutputFreq             = 0;
    unsigned char ucPrecision       = 2;
    QString strActLog               = "";
    QString strConnecterMsg         = "";
    QString strConnecter            = "";
    QString strErrorMsg = "";

    double dRefLevel = 0.0;
    double dRBW = 0.0;
    double dVBW = 0.0;
    double dSweepTime = 0.0;
    double dSpan = 0.0;
    double dPeakFreq = 0.0;
    double dMarkerAmp = 0.0;
    double dSpurOffset = 0.0;
    float fMeasSpurPwr = 0.0;
    float dCenterFreq = 0.0;

    QString qstr_testname = "";

    *out_bTestSts = 1;

    S_MODULENAME_SPURIOUSMEASUREMENT s_Param_SpuriousMeasurement; /*for input parameters from database*/

    S_SPURIOUS_MEASUREMENT s_Rpt_SpuriousMeasurement; /*For report to database*/

    if(in_szTestname == NULL)
    {
        return DP_APP_ERR_INVALID_PARAM;
    }

    qstr_testname = QString::asprintf("%s", in_szTestname);

    /*Read Test case ID*/
    sRetVal = m_pobjReportWrapper->ReadTestCaseID(qstr_testname.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s", "Unable to read Test Case ID ");
        emit Sig_Fail_State(out_bTestSts, strErrorMsg);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Preset in RF SG
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SigGenSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure Preset in RF Signal Generator");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Reset in RF SA
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(u8SpecAnSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure Reset in Spectrum Analyzer");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

    /*Read Test case configuration parameter*/
    memset(&s_Rpt_SpuriousMeasurement,0, sizeof(S_SPURIOUS_MEASUREMENT));
    s_Param_SpuriousMeasurement.m_vectFREQUENCYTABLE.clear();
    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &s_Param_SpuriousMeasurement);
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s", "Unable to read configuration data");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

    s_Rpt_SpuriousMeasurement.m_dInputPower = s_Param_SpuriousMeasurement.m_dInputPower;
    s_Rpt_SpuriousMeasurement.m_dExpMinSpurious = s_Param_SpuriousMeasurement.m_dExpectedMinSpurious;

    dRefLevel = s_Param_SpuriousMeasurement.m_dReferenceLevel;
    dRBW = s_Param_SpuriousMeasurement.m_dRBW;
    dVBW = s_Param_SpuriousMeasurement.m_dVBW;
    dSweepTime = s_Param_SpuriousMeasurement.m_dSweepTime;
    dSpan = s_Param_SpuriousMeasurement.m_dSpan;

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure reference Level in RF SA
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SpecAnSel,dRefLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure Preset in RF Signal Generator");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure center frequency and Span in RF SA
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnSel, (dOutputFreq * 1e6), (dSpan * 1e6));
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure center frequency and Span in Spectrum Analyzer");
        emit Sig_Fail_State(out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Peak Threshold in RF SA
    sRetVal = p_objMainApp->m_objSAWrapper.AgSA_SetThersHold(u8SpecAnSel,0,0);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure center frequency and Span in Spectrum Analyzer");
        emit Sig_Fail_State(out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure RBW, VBW, Sweep time in RF SA
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SpecAnSel, 0, (dRBW * 1e3) , 0, (dVBW * 1e3), 0, dSweepTime);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to Configure RBW, VBW and Sweep time in Spectrum Analyzer");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }
    try
    {
        for(U32BIT u32FreqTblIdx = 0; u32FreqTblIdx < s_Param_SpuriousMeasurement.m_vectFREQUENCYTABLE.size(); u32FreqTblIdx++)
        {
            dOutputFreq = s_Param_SpuriousMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency;
#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(s_Param_SpuriousMeasurement.m_s8StimulasPathID , s_Param_SpuriousMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency, s_Param_SpuriousMeasurement.m_dInputPower , &dSGinPower, &dStimulasPathloss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("Unable to Read the RF stimulus path calibration data ");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel, (s_Param_SpuriousMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency *1e6), dSGinPower);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("%s","Unable to configure frequency and power level in signal generator");
                throw DP_TEST_FAIL;
            }

            dCenterFreq = s_Param_SpuriousMeasurement.m_dCenterFrequency;

#ifndef _SIMULATION_INSTRUMENTS_
            // Configure center frequency and Span in RF SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnSel, (dCenterFreq * 1e6), (dSpan * 1e6));
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("%s","Unable to Configure center frequency and Span in Spectrum Analyzer");
                emit Sig_Fail_State(out_bTestSts,strErrorMsg);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Enable Rf out in RF SG
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, true);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("%s","Unable to disable RF output");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            //Enable continous in RF SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnSel, DP_SA_ENABLE_CONTINOUS);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("%s","Unable to Configure enable continous in Spectrum Analyzer");
                throw DP_TEST_FAIL;
            }
            Sleep(s_Param_SpuriousMeasurement.m_u16Delay);

#ifndef _SIMULATION_INSTRUMENTS_
            //Disable continous in RF SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnSel, DP_SA_DISABLE_CONTINOUS);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("%s","Unable to Configure disable continous in Spectrum Analyzer");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
#ifdef _MARKER_SEARCH_
            //Read Peak value
            sRetVal= p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadPeakValue(u8SpecAnSel, &dPeakFreq,&dMarkerAmp);
#else
            // Read Marker Frequency
            sRetVal=p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SpecAnSel,(dOutputFreq * 1e6),&dMarkerAmp);
#endif
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                strErrorMsg = QString::asprintf("%s","Unable to read marker or peak from Spectrum Analyzer");
                throw DP_TEST_FAIL;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Measured Path
            sRetVal = ApplyCalibData(s_Param_SpuriousMeasurement.m_s8MeasuredPathID , dOutputFreq, dMarkerAmp , &dOutputPower, &dMeasuredPathloss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strErrorMsg = QString::asprintf("Unable to Read the RF Measured path calibration data ");
                throw DP_TEST_FAIL;
            }

            s_Rpt_SpuriousMeasurement.m_dInputFrequency = s_Param_SpuriousMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency;
            s_Rpt_SpuriousMeasurement.m_dOutputFrequency = dOutputFreq;
            s_Rpt_SpuriousMeasurement.m_dOutputPower = dMarkerAmp;
            s_Rpt_SpuriousMeasurement.m_dStimulasPathLoss = dStimulasPathloss;
            s_Rpt_SpuriousMeasurement.m_dMeasuredPathLoss = dMeasuredPathloss;
            s_Rpt_SpuriousMeasurement.m_dSpan = dSpan;

            //Actual Peak
            strActLog = QString::asprintf("I/P Freqency: %0.3f MHz,  I/P Power: %0.2f dBm, Meas. O/P Frequency : %0.2f dBm ,Meas. O/P Power : %0.2f dBm"\
                                          ,s_Param_SpuriousMeasurement.m_vectFREQUENCYTABLE[u32FreqTblIdx].m_dInputFrequency , s_Rpt_SpuriousMeasurement.m_dInputPower, s_Rpt_SpuriousMeasurement.m_dOutputFrequency , s_Rpt_SpuriousMeasurement.m_dOutputPower);
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strActLog);


            FDOUBLE dSpurOff = 0;
            FDOUBLE ObsSpurPwr = 0;

            while(dSpurOff <= s_Param_SpuriousMeasurement.m_dFrequencyTolerance && dSpurOff >= (-s_Param_SpuriousMeasurement.m_dFrequencyTolerance))
            {
#ifdef _MARKER_SEARCH_
                //Read next Peak
                sRetVal =  p_objMainApp->m_objSAWrapper.AgSA_ReadNextPeakValue(u8SpecAnSel, &fMeasSpurPwr, &dSpurOffset);
#else
                sRetVal = 0;
#endif
                if(sRetVal)
                {
                    strErrorMsg = QString::asprintf("%s","Unable to read next peak from Spectrum Analyzer");
                    throw DP_TEST_FAIL;
                }
                dSpurOff = dOutputFreq  - (dSpurOffset/ 1e6);
            }
            dSpurOff = 0;

            ObsSpurPwr = fMeasSpurPwr;
            s_Rpt_SpuriousMeasurement.m_dObservedspurious = s_Rpt_SpuriousMeasurement.m_dOutputPower - fMeasSpurPwr;
            s_Rpt_SpuriousMeasurement.m_dObservedOffsetFreq = dSpurOffset / 1e6;

            dproundoff(dStimulasPathloss, ucPrecision);

            long lSpuriousOffset 		= (long)(dproundoff(s_Rpt_SpuriousMeasurement.m_dObservedspurious, ucPrecision)   * pow(10, ucPrecision));
            long lExpMin                = (long)(dproundoff(s_Rpt_SpuriousMeasurement.m_dExpMinSpurious , ucPrecision) * pow(10, ucPrecision));

            //Result
            if(s_Rpt_SpuriousMeasurement.m_dObservedOffsetFreq !=  dOutputFreq)
            {
                strActLog = QString::asprintf("I/P Freqency: %0.3f MHz,  I/P Power: %0.2f dBm, Exp. Min. Spurious Offset : %0.2f dB, Meas. Spurious Frequency : %0.2f MHz , Meas. Spurious Power : %0.2f dBm , Meas. Spurious Offset : %0.2f dBc"\
                                              ,s_Rpt_SpuriousMeasurement.m_dInputFrequency , s_Rpt_SpuriousMeasurement.m_dInputPower, s_Rpt_SpuriousMeasurement.m_dExpMinSpurious, s_Rpt_SpuriousMeasurement.m_dObservedOffsetFreq , ObsSpurPwr, s_Rpt_SpuriousMeasurement.m_dObservedspurious);

                if(lSpuriousOffset < lExpMin)
                {
                    s_Rpt_SpuriousMeasurement.m_u8Result = DP_TEST_FAIL;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                    *out_bTestSts = 0;
                }
                else
                {
                    s_Rpt_SpuriousMeasurement.m_u8Result = DP_TEST_PASS;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
                }
            }
            else
            {
                s_Rpt_SpuriousMeasurement.m_dObservedOffsetFreq = 0;
                s_Rpt_SpuriousMeasurement.m_dObservedspurious = 0;
                s_Rpt_SpuriousMeasurement.m_u8Result = DP_TEST_PASS;
                strActLog = QString::asprintf("I/P Freqency: %0.3f MHz,  I/P Power: %0.2f dBm, No Pead Found"\
                                              ,s_Rpt_SpuriousMeasurement.m_dInputFrequency , s_Rpt_SpuriousMeasurement.m_dInputPower);

                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
            }

            if(g_bAutoTestStartStop == false)
            {
                s_Rpt_SpuriousMeasurement.m_u8Result = DP_TEST_STOPPED;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "LO1 Spurious Measurement Test stopped");
                throw DP_TEST_STOPPED;
            }
            QVectRx_Spurious_Measurement.push_back(s_Rpt_SpuriousMeasurement);
        }//Qvector Size end
        throw DP_TEST_PASS;
    }
    catch(S32BIT s32Exception)
    {
#ifndef _SIMULATION_INSTRUMENTS_
        // Enable Rf out in RF SG
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, false);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strErrorMsg = QString::asprintf("%s","Unable to disable RF output");
            throw DP_TEST_FAIL;
        }

        sRetVal = OnSafeState();
        if(sRetVal)
        {
            strErrorMsg = QString::asprintf("%s","Unable to insert report records to database");
            emit Sig_Fail_State( out_bTestSts,strErrorMsg);
            return DP_FAILURE;
        }

        if(s32Exception == DP_TEST_FAIL)
        {
            emit Sig_Fail_State(out_bTestSts, strErrorMsg);
            return s32Exception;
        }
        else if(s32Exception == DP_TEST_STOPPED)
        {
            return s32Exception;
        }

    }

    sRetVal = OnSafeState();
    if(sRetVal)
    {
        strErrorMsg = QString::asprintf("%s","Unable to insert report records to database");
        emit Sig_Fail_State( out_bTestSts,strErrorMsg);
        return DP_FAILURE;
    }
    return DP_SUCCESS;
}

short CAutoTestThread::ImageRejection(bool *out_bTestSts, char *in_szTestname)
{
    U8BIT u8GainEqPathSel  = 0;
    U8BIT u8AtteBandSel = 0;
    U8BIT u8IFChnNumber = 0;
    U8BIT u8RFChnNumber = 0;
    U8BIT u8SigGenSel               = 1;
    U8BIT u8SpecAnaSel               = 1;
    U32BIT u32TableStructsize = 0;
    U32BIT u32TestCaseID        = 0;
    U32BIT u32CfgNumber         = 0;
    short sRetVal               = 0;
    double dMeasPwr             = 0.0f;
    double dSAoutPower          = 0.0f;
    double dSGinPower           = 0.0f;
    double dCenterFreq          = 0.0f;
    double dIFFreq = 0.0f;
    char szErrMsg[512]          = {0};
    QString strActLog           = "";
    QString strTestName         = "";
    QString strConnecterMsg = "";
    U8BIT u8Imagecnt                   = 0;
    U8BIT u8BandSel = 0;
    double arrdFreq[4]              = {0.0f};
    *out_bTestSts = 1;

    S_MODULENAME_IMAGEREJECTION sImageRej;

    S_IMAGE_REJECTION sImageRejReport;
    memset(&sImageRejReport, 0, sizeof(S_IMAGE_REJECTION));

    if(in_szTestname == NULL)
    {
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read Test Case ID ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Preset in RF SG
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SigGenSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to Configure Preset in RF Signal Generator for Image Rejection");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(u8SpecAnaSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s","Unable to Configure Preset in RF Signal Analyzer ");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID,1,&sImageRej);
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to read configuration data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Config Ref Level In SA
    sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SpecAnaSel, sImageRej.m_dRefLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to configure reference level Image Rejection");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Config  RBW , VBW in SA
    sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SpecAnaSel, 0, (sImageRej.m_dRBW * 1.0e3), 0, (sImageRej.m_dVBW* 1.0e3), 1, sImageRej.m_dSweepTime);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to configure RBW,VBW and sweep time in Image Rejection");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    u32TableStructsize = sImageRej.m_vectCFGDETAILS.size();

    for(U32BIT u32Idx = 0; u32Idx < u32TableStructsize ; u32Idx++)
    {
        sImageRejReport.m_dExpMinRej = sImageRej.m_vectCFGDETAILS.at(u32Idx).m_dExpMinRej;
        sImageRejReport.m_dImageFreq = sImageRej.m_vectCFGDETAILS.at(u32Idx).m_dImageFrequency;
        sImageRejReport.m_dRfFrequency = sImageRej.m_vectCFGDETAILS.at(u32Idx).m_dFrequency;
        arrdFreq[0] =  sImageRejReport.m_dRfFrequency;
        arrdFreq[1] = sImageRejReport.m_dImageFreq;
        sImageRejReport.m_dInputPower = sImageRej.m_vectCFGDETAILS.at(u32Idx).m_dInputPower;
        u8BandSel = sImageRej.m_vectCFGDETAILS.at(u32Idx).m_u8BandSel;
        dCenterFreq = sImageRej.m_vectCFGDETAILS.at(u32Idx).m_dRFDCA_2;/*Temperarly I am using this variable as a Band cofigure center Frequency*/
        u8AtteBandSel                                    = sImageRej.m_vectCFGDETAILS.at(u32Idx).m_u8GainEqPathSel;/*Temperarly I am using this variable as a Atte band Sel*/
        dIFFreq  = sImageRej.m_vectCFGDETAILS.at(u32Idx).m_dIFFrequency;

        sprintf(sImageRejReport.m_szRFIFPort, "RFIN-%d - IFOUT-%d", u8RFChnNumber, u8IFChnNumber);

        if( (u8BandSel == 1) && (u8RFChnNumber < 4))
        {
            sprintf(sImageRejReport.m_szBandDetails, "2 - 18 GHz Path");
        }
        else if( (u8BandSel == 2) && (u8RFChnNumber < 4))
        {
            sprintf(sImageRejReport.m_szBandDetails, "2 - 9 GHz Path");
        }
        else
        {
            sprintf(sImageRejReport.m_szBandDetails, "NA");
        }

        /*Configure here*/

#ifndef _SIMULATION_INSTRUMENTS_
        // Config Center Freq and  Span in SA
        sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnaSel, (dIFFreq * 1e6), (sImageRej.m_dSpan * 1e6));
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s","Unable to configure center frequency and span in Image Rejection");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        for(u8Imagecnt = 0; u8Imagecnt < 2; u8Imagecnt++)
        {
#ifndef  _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(sImageRej.m_u16StimulusPathId, arrdFreq[u8Imagecnt], sImageRejReport.m_dInputPower, &dSGinPower, &sImageRejReport.m_dRFCableloss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to Read the stimulus path calibration data for Image Rejection");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Configure SigGen freq and power for RF
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel, (arrdFreq[u8Imagecnt] * 1e6) , dSGinPower);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to configure frequency and power level in signal generator for  Image Rejection");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            // Enable Rf out in RF SG
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, true);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to disable RF output in RF signal generator for Image Rejection");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Enable continuous run
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ATE_7343_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to enable the continous run in spectrum analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            msleep(sImageRej.m_u64Delay); // Delay to Settle
#ifndef _SIMULATION_INSTRUMENTS_
            // Disable continuous run
            sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ATE_7343_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to disable the continous run in spectrum analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            // Read MArker Frequency
            sRetVal =   p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SpecAnaSel, (0 * 1e6), &dMeasPwr);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to read marker frequency in spectrum analyze");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Disable RF Out in SG
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, false);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to disable RF output in RF signal generator for Image Rejection ");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef  _SIMULATION_INSTRUMENTS_
            // Apply Calib DATA for Measurement Path ID
            sRetVal = ApplyCalibData(sImageRej.m_u16MeasurementPathId, dIFFreq, dMeasPwr, &dSAoutPower, &sImageRejReport.m_dRFCableloss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp = QString::asprintf("%s","Unable to Read the Measurement PathID  for Image Rejection");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            if(u8Imagecnt == 0)
            {
                sImageRejReport.m_dMeasActpwr  = dSAoutPower;
            }
            else
            {
                sImageRejReport.m_s32ImageNo	= u8Imagecnt;
                sImageRejReport.m_dRfFrequency	= arrdFreq[0];
                sImageRejReport.m_dImageFreq	= arrdFreq[u8Imagecnt];
                sImageRejReport.m_dExpMinRej    = sImageRej.m_dExpImageRej;
                sImageRejReport.m_dImagePwr     = dSAoutPower;
                sImageRejReport.m_dImageRej     = sImageRejReport.m_dMeasActpwr - sImageRejReport.m_dImagePwr;

                strActLog = QString::asprintf("CH No. : %d,  I/P Frequency : %0.3f MHz, I/P Power : %0.2f dBm, Image Freq. : %0.3f MHz, Meas. IF power @ Actual I/P : %0.2f dBm, Meas. IF power @ Image I/P : %0.2f dBm, Exp. Min. Image Rejection : %0.2f dBc, Calc. Image Rejection : %0.2f dBc", \
                                              sImageRejReport.m_s32ChannelNo, arrdFreq[0], sImageRejReport.m_dInputPower, arrdFreq[u8Imagecnt], sImageRejReport.m_dMeasActpwr, sImageRejReport.m_dImagePwr, sImageRejReport.m_dExpMinRej, sImageRejReport.m_dImageRej);
                dproundoff(sImageRejReport.m_dMeasActpwr, sImageRej.m_u8Precision);
                dproundoff(sImageRejReport.m_dImagePwr, sImageRej.m_u8Precision);
                long lImageRej 		= (long)(dproundoff(sImageRejReport.m_dImageRej, sImageRej.m_u8Precision)   * pow(10, sImageRej.m_u8Precision));
                long lExpMinRej		= (long)(dproundoff(sImageRejReport.m_dExpMinRej, sImageRej.m_u8Precision)   * pow(10, sImageRej.m_u8Precision));

                if( lImageRej < lExpMinRej)
                {
                    sImageRejReport.m_s32Result = DP_APP_FAIL;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                    *out_bTestSts = 0;
                }
                else
                {
                    sImageRejReport.m_s32Result = DP_APP_PASS;
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
                }

                if(g_bAutoTestStartStop == false)
                {
                    sImageRejReport.m_s32Result = DP_DP_APP_TEST_STOPPED;
                    QVectImageRejection.push_back(sImageRejReport);
                    sRetVal = OnSafeState();
                    if(sRetVal)
                    {
                        strTemp = QString::asprintf("%s","Unable to report record insert into database");
                        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                        return DP_FAILURE;
                    }
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Image Rejection Measurement Test stopped");
                    return DP_DP_APP_TEST_STOPPED;
                }

                QVectImageRejection.push_back(sImageRejReport);
            }
        }//Image Count end
    } // Qvector size ends

    sRetVal = OnSafeState();
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s","Unable to report record insert into database");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    return DP_SUCCESS;
}

short CAutoTestThread ::RFBITEIsolation( bool *out_bTestSts, char* in_szTestname)
{
    short sRetVal					= DP_APP_ZERO;
    double dMeasurePwr              = DP_APP_ZERO;
    double dMeasIsolation			= DP_APP_ZERO;
    double dSGIPPower               = 0.0;
    double dTempStimCableLoss       = 0.0;
    double dTempMeasCableLoss       = 0.0;
    double dSAoutPower              = 0.0;
    U32BIT u32Idx                   = DP_APP_ZERO;
    U32BIT u32TableStructsize       = DP_APP_ZERO;
    QString strTemp					= "";
    QString strTestName             = "";
    U32BIT u32TestCaseID            = DP_APP_ZERO;
    FSINGLE     fAtten                          = 0.0;

    U8BIT u8SigGenSel               = 1;
    U8BIT u8SpecAnaSel               = 1;

    QList<FDOUBLE> objAntInPowerList;
    S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT SIsoBtwRFBITEIPParam;
    S_MODULEID_BITEISOLATION_MEAS SIsoBtwRFBITE;  /* structure for report generation */

    *out_bTestSts = true;
    memset(&SIsoBtwRFBITE, DP_APP_ZERO, sizeof(S_MODULEID_BITEISOLATION_MEAS));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);
    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &SIsoBtwRFBITEIPParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp= QString::asprintf("%s", "Unable to Read Configuration Data for RF-BITE Isolation Measurement");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Preset SigGEN */
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SigGenSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp= QString::asprintf("%s", "Unable to Preset the Signal Generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Reset Spectrum */
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(u8SpecAnaSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to Preset the Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    msleep(100);
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ModeConfig(u8SpecAnaSel, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal != DP_SUCCESS)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Spectrum Analyzer Mode Configuration Failed");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Config Sweep in Spectrum */
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SpecAnaSel, false, (SIsoBtwRFBITEIPParam.m_dRBW * 1e3), false, (SIsoBtwRFBITEIPParam.m_dVBW * 1e3), true, SIsoBtwRFBITEIPParam.m_dSweepTime);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp= QString::asprintf("%s", "Unable to configure the Sweep in Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Config Reference Level in Spectrum */
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SpecAnaSel, SIsoBtwRFBITEIPParam.m_dReferenceLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to configure the reference level in Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Config center frequency, span*/
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnaSel,  (500 * 1.0e6),  (1 * 1.0e6));
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to configure the center & span in Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /*Run hold*/
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ENABLE);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to enable sweep in Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    msleep(500);
#ifndef _SIMULATION_INSTRUMENTS_
    /* Config Reference Level in Spectrum */
    sRetVal = p_objMainApp->m_objSAWrapper.AgSA_PreAmpEnableDisable(u8SpecAnaSel, 0);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to configure the reference level in Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }


    u32TableStructsize = SIsoBtwRFBITEIPParam.m_vectRFBITEISOLATIONDATA.size();
    objAntInPowerList.clear();
    /*Loop to Iterate for all the Frequencies */
    for(u32Idx = 0; u32Idx < u32TableStructsize ; u32Idx++)
    {
        SIsoBtwRFBITE.m_dInputfreq = SIsoBtwRFBITEIPParam.m_vectRFBITEISOLATIONDATA.at(u32Idx).m_dInputFreq;
        SIsoBtwRFBITE.m_dInputPower = SIsoBtwRFBITEIPParam.m_vectRFBITEISOLATIONDATA.at(u32Idx).m_dInputPower;
        SIsoBtwRFBITE.m_dExpIsolation = SIsoBtwRFBITEIPParam.m_vectRFBITEISOLATIONDATA.at(u32Idx).m_dExpIso;

        //Add Configuration

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = ApplyCalibData(SIsoBtwRFBITEIPParam.m_u8StimuluspathId, SIsoBtwRFBITEIPParam.m_vectRFBITEISOLATIONDATA.at(u32Idx).m_dInputFreq, SIsoBtwRFBITEIPParam.m_vectRFBITEISOLATIONDATA.at(u32Idx).m_dInputPower, &dSGIPPower, &dTempStimCableLoss);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to apply cable loss");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Config RF freq */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel , (SIsoBtwRFBITE.m_dInputfreq * 1.0e6),  dSGIPPower);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to configure frequency and power in Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Config center frequency, span*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnaSel,  (SIsoBtwRFBITE.m_dInputfreq * 1.0e6),  (SIsoBtwRFBITEIPParam.m_dSpan * 1.0e6));
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to configure the center frequency and span in Spectrum Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* RF Out Enable */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, DP_ATE_RFOUT_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to RF OUT Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(100);

#ifndef _SIMULATION_INSTRUMENTS_
        /*Run hold*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to config hold in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(SIsoBtwRFBITEIPParam.m_u16Delay);

#ifndef _SIMULATION_INSTRUMENTS_
        /*Run hold*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to config hold in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SpecAnaSel, (SIsoBtwRFBITE.m_dInputfreq * 1.0e6),  &dMeasurePwr);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to read power at Marker position in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Disable RF Output */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled( u8SigGenSel, DP_ATE_RFOUT_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Disable the RF in Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = ApplyCalibData(SIsoBtwRFBITEIPParam.m_u8MeasurepathID, SIsoBtwRFBITE.m_dInputfreq, dMeasurePwr, &dSAoutPower, &dTempMeasCableLoss);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to apply cable loss");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        SIsoBtwRFBITE.m_dStimulusPathloss = dTempStimCableLoss;
        SIsoBtwRFBITE.m_dMeasuredPathloss = dTempMeasCableLoss;
        SIsoBtwRFBITE.m_dSetPower = dSGIPPower;
        dSAoutPower = dproundoff(dSAoutPower, 2);
        SIsoBtwRFBITE.m_dMeasured_Power = dSAoutPower;
        dMeasIsolation = SIsoBtwRFBITE.m_dInputPower - SIsoBtwRFBITE.m_dMeasured_Power;
        SIsoBtwRFBITE.m_dMeasIsolation	= dproundoff(dMeasIsolation, 2);

        strTemp = QString::asprintf("Input Freqency: %0.3f MHz, Input Power: %0.2f dBm, Exp. Min. Isolation : %0.2f dB,  Measured Power @ RF-IN %d : %0.2f dBm, Measured Isolation : %0.2f dB",\
                                    SIsoBtwRFBITE.m_dInputfreq, SIsoBtwRFBITE.m_dInputPower, SIsoBtwRFBITE.m_dExpIsolation, SIsoBtwRFBITE.m_s32ChannelNo, SIsoBtwRFBITE.m_dMeasured_Power, SIsoBtwRFBITE.m_dMeasIsolation);

        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp);

        if(SIsoBtwRFBITE.m_dMeasIsolation >= SIsoBtwRFBITE.m_dExpIsolation)
        {
            SIsoBtwRFBITE.m_s32Result = DP_APP_PASS;
        }
        else
        {
            *out_bTestSts = false;
            SIsoBtwRFBITE.m_s32Result = DP_APP_FAIL;
        }

        if(g_bAutoTestStartStop == false)
        {
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                *out_bTestSts = false;
                strTemp = QString::asprintf("%s","Unable to insert to database");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            return DP_DP_APP_TEST_STOPPED;
        }

        qvBiteIsolationData.push_back(SIsoBtwRFBITE);

    }//Loop End
    /* Print to Report */
    sRetVal = OnSafeState();
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s","Unable to insert to database");
        emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    strTemp = QString::asprintf("%s","RF BITE Isolation Measurement Test Completed");
    emit sig_MsgToPrintlog( PRINTLOG_TYPE_INFO, strTemp);

    return DP_SUCCESS;
}

short CAutoTestThread ::RFBlankIsolation( bool *out_bTestSts, char* in_szTestname)
{
    short sRetVal			   = DP_APP_ZERO;
    double dMeasurePwr         = DP_APP_ZERO;
    double dMeasIsolation	   = DP_APP_ZERO;
    double dSGIPPower          = 0.0;
    double dTempStimCableLoss  = 0.0;
    double dTempMeasCableLoss  = 0.0;
    double dSAoutPower         = 0.0;
    U32BIT u32Idx              = DP_APP_ZERO;
    QString strTemp			   = "";
    U32BIT u32TableStructsize  = 0;
    QString strTestName        = "";
    QString strUserMsg         = "";
    U32BIT u32TestCaseID       = DP_APP_ZERO;
    FSINGLE     fAtten                          = 0.0;
    U8BIT u8SigGenSel               = 1;
    U8BIT u8SpecAnaSel               = 1;
    double dInputFreq;
    double  dInputPower;
    double dExpMinIsolation;
    QList<FDOUBLE> objBypassPowerList;
    S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT SIsoBtwRFBLANKIPParam;
    S_MODULEID_BLANKING_ISOLATION_MEAS SIsoBtwBlanking;  /* structure for report generation */

    *out_bTestSts = true;
    memset(&SIsoBtwBlanking, DP_APP_ZERO, sizeof(S_MODULEID_BLANKING_ISOLATION_MEAS));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &SIsoBtwRFBLANKIPParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to Read Configuration Data for RF-BLANK Isolation Measurement");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Preset SigGEN */
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SigGenSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to Preset the Signal Generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Reset Spectrum */
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(u8SpecAnaSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to Preset the Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(500);

    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ModeConfig(u8SpecAnaSel, 1);
    if(sRetVal != DP_SUCCESS)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Spectrum Analyzer Mode Configuration Failed");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Config Sweep in Spectrum */
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SpecAnaSel, 0, (SIsoBtwRFBLANKIPParam.m_dRBW * 1e3), 0, (SIsoBtwRFBLANKIPParam.m_dVBW * 1e3), 1, SIsoBtwRFBLANKIPParam.m_dSweepTime);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to configure the Sweep in Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /* Config Reference Level in Spectrum */
    sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SpecAnaSel, SIsoBtwRFBLANKIPParam.m_dReferenceLevel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to configure the reference level in Spectrum Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    u32TableStructsize = SIsoBtwRFBLANKIPParam.m_vectRFBLANKISOLATIONDATA.size();
    objBypassPowerList.clear();

    /*Loop to Iterate for all the Frequencies */
    for(u32Idx = 0; u32Idx < u32TableStructsize ; u32Idx++)
    {

        dInputFreq = SIsoBtwRFBLANKIPParam.m_vectRFBLANKISOLATIONDATA.at(u32Idx).m_dInputFreq;
        dInputPower = SIsoBtwRFBLANKIPParam.m_vectRFBLANKISOLATIONDATA.at(u32Idx).m_dInputPower;
        dExpMinIsolation = SIsoBtwRFBLANKIPParam.m_vectRFBLANKISOLATIONDATA.at(u32Idx).m_dExpIso;


#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = ApplyCalibData(SIsoBtwRFBLANKIPParam.m_u8StimuluspathID, dInputFreq, dInputPower, &dSGIPPower, &dTempStimCableLoss);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to apply cable loss");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Config RF freq */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel , (dInputFreq * 1.0e6),  dSGIPPower);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to configure frequency and power in Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Config center frequency, span*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnaSel,  (dInputFreq * 1.0e6),  (SIsoBtwRFBLANKIPParam.m_dSpan * 1.0e6));
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to configure the center frequency and span in Spectrum Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* RF Out Enable */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, DP_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to RF OUT Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /*Run hold*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to config hold in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(SIsoBtwRFBLANKIPParam.m_u16Delay);

#ifndef _SIMULATION_INSTRUMENTS_
        /*Run hold*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to config hold in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SpecAnaSel, 0,  &dMeasurePwr);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to read power at Marker position in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Disable RF Output */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, DP_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Disable the RF in Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = ApplyCalibData(SIsoBtwRFBLANKIPParam.m_u8MeasurepathID, dInputFreq, dMeasurePwr, &dSAoutPower, &dTempStimCableLoss);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to apply cable loss");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        objBypassPowerList.append(dMeasurePwr);

        //Add Configuration

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = ApplyCalibData(SIsoBtwRFBLANKIPParam.m_u8StimuluspathID, dInputFreq, dInputPower, &dSGIPPower, &dTempStimCableLoss);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to apply cable loss");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Config RF freq */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel , (dInputFreq * 1.0e6),  dSGIPPower);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to configure frequency and power in Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Config center frequency, span*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnaSel,  (dInputFreq * 1.0e6),  (SIsoBtwRFBLANKIPParam.m_dSpan * 1.0e6));
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to configure the center frequency and span in Spectrum Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* RF Out Enable */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, DP_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to RF OUT Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        //msleep(2000);

#ifndef _SIMULATION_INSTRUMENTS_
        /*Run hold*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to config hold in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(SIsoBtwRFBLANKIPParam.m_u16Delay);

#ifndef _SIMULATION_INSTRUMENTS_
        /*Run hold*/
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to config hold in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(u8SpecAnaSel, 0,  &dMeasurePwr);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to read power at Marker position in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /* Disable RF Output */
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, DP_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Disable the RF in Signal Generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = ApplyCalibData(SIsoBtwRFBLANKIPParam.m_u8MeasurepathID, dInputFreq, dMeasurePwr, &dSAoutPower, &dTempMeasCableLoss);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to apply cable loss");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        SIsoBtwBlanking.m_dStimulusPathLoss = dTempStimCableLoss;
        SIsoBtwBlanking.m_dMeasuredPathLoss = dTempMeasCableLoss;
        SIsoBtwBlanking.m_dSetPower = dSGIPPower;
        SIsoBtwBlanking.m_dInputfreq = dInputFreq;
        SIsoBtwBlanking.m_dInputPower = dInputPower;
        SIsoBtwBlanking.m_dExpIsolation = dExpMinIsolation;
        SIsoBtwBlanking.m_dMeasBypassPower = dproundoff(objBypassPowerList.at(u32Idx), 2);
        SIsoBtwBlanking.m_dMeasBlankingPower = dproundoff(dSAoutPower, 2);
        dMeasIsolation = ( objBypassPowerList.at(u32Idx) - dSAoutPower );
        SIsoBtwBlanking.m_dMeasIsolation	= dproundoff(dMeasIsolation, 2);

        strTemp = QString::asprintf("Input Freqency: %0.3f MHz, Input Power: %0.2f dBm, Measured Bypass Power: %0.02f dBm, Measured Blanking Power: %0.02f dBm, Exp. Isolation : %0.2f dB, Measured Isolation : %0.2f dB",\
                                     SIsoBtwBlanking.m_dInputfreq, SIsoBtwBlanking.m_dInputPower, SIsoBtwBlanking.m_dMeasBypassPower, SIsoBtwBlanking.m_dMeasBlankingPower, SIsoBtwBlanking.m_dExpIsolation, SIsoBtwBlanking.m_dMeasIsolation);

        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp);

        if(SIsoBtwBlanking.m_dMeasIsolation >= SIsoBtwBlanking.m_dExpIsolation)
        {
            SIsoBtwBlanking.m_s32Result = DP_APP_PASS;
        }
        else
        {
            *out_bTestSts = false;
            SIsoBtwBlanking.m_s32Result = DP_APP_FAIL;
        }

        if(g_bAutoTestStartStop == false)
        {
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                *out_bTestSts = false;
                strTemp = QString::asprintf("%s","Unable to insert to database");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            return DP_DP_APP_TEST_STOPPED;
        }

        qvBlankingIsolationData.push_back(SIsoBtwBlanking);
    }
    /* Print to Report */
    sRetVal = OnSafeState();
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s","Unable to insert to database");
        emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    strTemp = QString::asprintf("%s","RF Blanking Isolation Measurement Test Completed");
    emit sig_MsgToPrintlog( PRINTLOG_TYPE_INFO, strTemp);

    return DP_SUCCESS;
}
#if 0
short CAutoTestThread::CHN_CHN_Isolation(bool *out_bTestSts, char* in_szTestname)
{
    unsigned char ucPrecision   = 0;
    short sRetVal               = 0;
    U32BIT u32TableStructsize = 0;
    U32BIT u32Idx  = 0;

    double dMeasPwr             = 0.0f;
    double dMeasFreq            = 0.0f;
    double dSAoutPower          = 0.0f;
    double dSGinPower[2]        = {0.0f};
    double dCenterFreq          = 0.0f;
    char *PszMessage            = NULL;

    QStringList strConnectName;
    QString strFilePath         = "";
    QString strActLog           = "";

    char szErrMsg[512]          = {0};
    double dRF_stimulusPathID = 0.0f;
    double dRF_MeasPathID = 0.0f;
    unsigned char ucCfgNoBLIIdx[4] = {5,6,7,8};
    unsigned char ucCfgNoADFIdx[4] = {9,10,11,12};
    unsigned char uc2to18GHz_Inp_PathID[4] = {1,2,3,4};
    unsigned char uc18to40GHz_Inp_PathID[4] = {5,6,7,8};
    unsigned char uc18to40GHz_ADF_Inp_PathID[4] = {9,10,11,12};
    unsigned char uc2to18GHz_Out_PathID[4] = {13,14,15,16};
    unsigned char ucADF_Out_PathID[4] = {17,18,19,20};

    U8BIT u8SigGenSel               = 1;
    U8BIT u8SpecAnaSel               = 1;

    *out_bTestSts = 0;
    QString strTempString           = "";
    QString strInpcnname            = "";
    U32BIT u32TestCaseID            =0;
    U32BIT u32CfgNumber             = 0;
    QString strTestName             = "";
    QString strOutpcnname           = "";
    QString strTempFreq             = "";

    S_CHANNELTOCHANNELISOLATION sParamIsolation;
    S_CH_CH_ISOLATION_REPO sRxRepIsolation;
    S_ACT_CH_RESULT sRxActCHRes;
    QVector <S_ACT_CH_RESULT> QVectActCHRes;

    memset(&sRxRepIsolation, 0, sizeof(S_CH_CH_ISOLATION_REPO));

    strTestName = QString::asprintf("%s", in_szTestname);
    p_objMainApp->m_bIsTestActive = true;

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = ReadConfigData(u32TestCaseID, u32CfgNumber, sParamIsolation);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to Read Configuration Data for RF-BLANK Isolation Measurement");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    // Configure Preset in RF SG
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(u8SigGenSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp.sprintf("%s","Unable to Configure Preset in RF Signal Generator");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    u32TableStructsize = sParamIsolation.m_vectTABLE_CONFIG.size();

    for(u32Idx = 0; u32Idx < u32TableStructsize ; u32Idx++)
    {
        sRxRepIsolation.m_dExpMinActPwr      = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dExpMinPwr;
        sRxRepIsolation.m_dExpMaxActPwr      = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dExpMaxPwr;
        sRxRepIsolation.m_dRf_inputfreq      = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dRFInpFreq;
        sRxRepIsolation.m_dRf_inputPower     = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dInputPower;
        sRxRepIsolation.m_dExpMinIsolation   = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dExpMinIso;

#ifndef _SIMULATION_INSTRUMENTS_
        // Apply Calib Data for RF Stimulas Path
        sRetVal = ApplyCalibData(dRF_stimulusPathID, sRxRepIsolation.m_dRf_inputfreq, sRxRepIsolation.m_dRf_inputPower, &dSGinPower[0], &sRxActCHRes.m_dRFCableloss);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to Read the RF stimulus path calibration data");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Config Center Freq and Span in SA
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnaSel, (sRxRepIsolation.m_dRf_inputfreq * 1e9), (sParamIsolation.m_dSpan * 1e6));
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to configure center frequency and span");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Config Ref Level In SA
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SpecAnaSel, sParamIsolation.m_s32Reference_Level);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to configure reference level");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        // Config  RBW , VBW in SA
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SpecAnaSel, 0, (sParamIsolation.m_dRBW * 1e3), 0, (sParamIsolation.m_dRBW * 1e3), 1, 0);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to configure RBW,VBW and sweep time");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

// TODO Configuration


#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel, (sRxRepIsolation.m_dRf_inputfreq * 1e9), dSGinPower[0]);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to configure frequency and power level in signal generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Enable Rf out in RF SG
        sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, true);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to disable RF output in RF signal generator");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Enable continuous run
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ATE_7343_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to enable the continous run in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(sParamIsolation.m_ddelay);   // Delay to Settle
#ifndef _SIMULATION_INSTRUMENTS_
        // Disable continuous run
        sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ATE_7343_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to disable the continous run in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
#ifndef _SIMULATION_INSTRUMENTS_
        // Read MArker Frequency
        sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadPeakValue(u8SpecAnaSel, &dMeasFreq , &dMeasPwr);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to read marker frequency in spectrum analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        // Apply Calib DATA for SA
        sRetVal = ApplyCalibData(dRF_MeasPathID, dCenterFreq, dMeasPwr, &dSAoutPower, &sRxActCHRes.m_dMeasCableloss);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp.sprintf("%s","Unable to Read the SA path calibration data");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        sRxActCHRes.m_dMesured_ActPower             = dMeasPwr;
        sRxActCHRes.m_dCalculated_ActPower          = dSAoutPower;

        //Result
        strActLog.sprintf(" I/P Freqency: %0.3f GHz, Exp. Min. ActPower : %0.2f dBm, Exp. Max. Actpower : %0.2f dBm, Meas. O/P Power : %0.2f dBm, Calc. Power : %0.2f dBm", sRxRepIsolation.m_dRf_inputfreq, sRxRepIsolation.m_dExpMinActPwr, sRxRepIsolation.m_dExpMaxActPwr, sRxRepIsolation.m_dMesured_ActPower, sRxRepIsolation.m_dCalculated_ActPower);

        if(( sRxRepIsolation.m_dCalculated_ActPower < sRxRepIsolation.m_dExpMinActPwr) || (sRxRepIsolation.m_dCalculated_ActPower > sRxRepIsolation.m_dExpMaxActPwr) )
        {
            sRxRepIsolation.m_s32Result = DP_DP_APP_TEST_FAIL;
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
            *out_bTestSts = 1;
        }
        else
        {
            sRxRepIsolation.m_s32Result = DP_DP_APP_TEST_PASS;
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
        }

        //Adding to array
        if(g_bAutoTestStartStop == false)
        {
            sRxRepIsolation.m_s32Result = 2;
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to insert to database");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Isolation Measurement Test stopped");
            return DP_TEST_STOPPED;
        }

        QVectActCHRes.push_back(sRxActCHRes);

    }
//    emit Sig_get_Portname();

    QString strTemp;
    strTemp.clear();

    strTemp = QString::asprintf("Connect Input Cable to the Isolation Connector %s",p_objMainApp->qstr_Inp_portname.toStdString().c_str());
     emit sig_QusToUser((char*)(strTemp.toStdString().c_str()));
    if(g_ucPressYES_NO)
    {
        for(u32Idx = 0; u32Idx < u32TableStructsize ; u32Idx++)
        {
            strcpy(sRxRepIsolation.m_szoutpCnname, p_objMainApp->qstr_Inp_portname.toStdString().c_str());
            sRxRepIsolation.m_dExpMinActPwr      = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dExpMinPwr;
            sRxRepIsolation.m_dExpMaxActPwr      = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dExpMaxPwr;
            sRxRepIsolation.m_dRf_inputfreq      = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dRFInpFreq;
            sRxRepIsolation.m_dRf_inputPower     = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dInputPower;
            sRxRepIsolation.m_dExpMinIsolation   = sParamIsolation.m_vectTABLE_CONFIG.at(u32Idx).m_dExpMinIso;

#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib Data for RF Stimulas Path
            sRetVal = ApplyCalibData(dRF_stimulusPathID, sRxRepIsolation.m_dRf_inputfreq, sRxRepIsolation.m_dRf_inputPower, &dSGinPower[0], &sRxRepIsolation.m_dRFCableloss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to Read the RF stimulus path calibration data");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Config Center Freq and Span in SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(u8SpecAnaSel, (sRxRepIsolation.m_dRf_inputfreq * 1e9), (sParamIsolation.m_dSpan * 1e6));
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to configure center frequency and span");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Config Ref Level In SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(u8SpecAnaSel, sParamIsolation.m_s32Reference_Level);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to configure reference level");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            // Config  RBW , VBW in SA
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(u8SpecAnaSel, 0, (sParamIsolation.m_dRBW * 1e3), 0, (sParamIsolation.m_dRBW * 1e3), 1, 0);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to configure RBW,VBW and sweep time");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(u8SigGenSel, (sRxRepIsolation.m_dRf_inputfreq * 1e9), dSGinPower[0]);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to configure frequency and power level in signal generator for Gain Measurement Measurement");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Enable Rf out in RF SG
            sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, true);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to disable RF output in RF signal generator for Gain Measurement ");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Enable continuous run
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ATE_7343_ENABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to enable the continous run in spectrum analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(sParamIsolation.m_ddelay);   // Delay to Settle
#ifndef _SIMULATION_INSTRUMENTS_
            // Disable continuous run
            sRetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(u8SpecAnaSel, DP_ATE_7343_DISABLE);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to disable the continous run in spectrum analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
#ifndef _SIMULATION_INSTRUMENTS_
            // Read MArker Frequency
            sRetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadPeakValue(u8SpecAnaSel, &dMeasFreq , &dMeasPwr);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to read marker frequency in spectrum analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            // Apply Calib DATA for SA
            sRetVal = ApplyCalibData(dRF_MeasPathID, dCenterFreq, dMeasPwr, &dSAoutPower, &sRxRepIsolation.m_dMeasCableloss);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                strTemp.sprintf("%s","Unable to Read the SA path calibration data");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            sRxRepIsolation.m_dMesured_ActPower      = QVectActCHRes.at(u32Idx).m_dMesured_ActPower;
            sRxRepIsolation.m_dCalculated_ActPower   = QVectActCHRes.at(u32Idx).m_dCalculated_ActPower;
            sRxRepIsolation.m_dCalcIsolatepwr        = QVectActCHRes.at(u32Idx).m_dCalculated_ActPower - dSAoutPower;
            sRxRepIsolation.m_dCalculated_IsolationPower = dSAoutPower;

            //Result
            strActLog.sprintf(" I/P Freqency: %0.3f GHz, Meas. Act.CH. Power : %0.2f dBm, Meas. Isolation CH. Power : %0.2f dBm, Calc. Isolation : %0.2f dBc", sRxRepIsolation.m_dRf_inputfreq, sRxRepIsolation.m_dCalculated_ActPower, dSAoutPower, sRxRepIsolation.m_dCalcIsolatepwr);

            if(( sRxRepIsolation.m_dCalculated_ActPower < sRxRepIsolation.m_dExpMinActPwr) || (sRxRepIsolation.m_dCalculated_ActPower > sRxRepIsolation.m_dExpMaxActPwr))
            {
                sRxRepIsolation.m_s32Result = DP_DP_APP_TEST_FAIL;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
                *out_bTestSts = 1;
            }
            else
            {
                sRxRepIsolation.m_s32Result = DP_DP_APP_TEST_PASS;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strActLog);
            }

            //Adding to array
            if(g_bAutoTestStartStop == false)
            {
                sRxRepIsolation.m_s32Result = 2;
                QVectCHtoCHIsolation.push_back(sRxRepIsolation);
                sRetVal = OnSafeState();
                if(sRetVal)
                {
                    strTemp.sprintf("%s","Unable to insert to database");
                    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                    return DP_FAILURE;
                }
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, "Isolation Measurement Test stopped");
                return DP_TEST_STOPPED;
            }
            QVectCHtoCHIsolation.push_back(sRxRepIsolation);

        }
    }
#ifndef _SIMULATION_INSTRUMENTS_
    // Disable RF Out in SG
    sRetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(u8SigGenSel, false);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp.sprintf("%s","Unable to disable RF output in RF signal generator for Gain Measurement");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = OnSafeState();
    if(sRetVal)
    {
        strTemp.sprintf("%s","Unable to insert to database");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }


    return DP_SUCCESS;
}
#endif
short CAutoTestThread ::FilterRejection( bool *out_bTestSts, char* in_szTestname)
{
    short sRetVal			  = DP_APP_ZERO;
    double dNotchPB2MinMag    = DP_APP_ZERO;
    double dNotchPB1MinMag    = DP_APP_ZERO;
    double dCalNotchPBMinMag  = DP_APP_ZERO;
    QString strFilePath       = "";
    QString strTemp           = "";
    char szStateFilepath[512] = {0};
    QString strStateFilepath  = "D:\\DUALFEA_CALIB_DATA";
    QString strTestName       = "";
    U32BIT u32TestCaseID      = DP_APP_ZERO;
    U32BIT u32CfgNumber       = DP_APP_ZERO;
    U16BIT usTraceNo          = 3;
    double dStepFreq          = 0;
    double dMarkerMaxData     = 0;
    double dMarkerMaxFreq     = 0;
    double dPBMaxMag       = 0;
    double d500_2000MaxMag = 0;
    double dPBMaxFreq      = 0;
    U8BIT u8SearchType = 0;
    double dPBStartFreq       = 0;
    double dPBStopFreq        = 0;
    double dOBStartFreq       = 0;
    double dOBStopFreq        = 0;
    double dMarkerFreq        = 0;
    int iMarkerNo             = 0;
    double Out_dMarkerValue   = 0;
    char szUserMsg[1024]            = {0};
    U8BIT u8NASel               = 1;
    S_MODULEID_FILTER_REJECTION sFilterRejection;
    S_MODULENAME_FILTERREJECTIONMEASUREMENT sFilterRejMeasIPParam;

    *out_bTestSts = true;

    memset(&sFilterRejection, 0, sizeof(S_MODULEID_FILTER_REJECTION));

    if(in_szTestname == NULL)
    {
        *out_bTestSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    sprintf(szStateFilepath, "%s", strStateFilepath.toStdString().c_str());
    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp= QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sFilterRejMeasIPParam);
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp= QString::asprintf("%s", "Unable to Read Filter Rejection Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    /*Ask User to change Channels at output*/
    sprintf(szUserMsg, "Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser(szUserMsg);


    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(300, 2200, sFilterRejMeasIPParam.m_dInputPower, sFilterRejMeasIPParam.m_u16DataPoints, strStateFilepath,1);
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
    }


#ifndef _SIMULATION_INSTRUMENTS_
    //Recall State file
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NASel, szStateFilepath);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to load State file in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set power level in NA
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NASel, sFilterRejMeasIPParam.m_dInputPower, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to set power level in Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NASel, sFilterRejMeasIPParam.m_u16DataPoints, usTraceNo, 1, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to select the trace");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDisplayFormat(u8NASel, DP_LOG_MODE, usTraceNo);  // 1 ch no
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to config Display format in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(480);
    //Add Configuration

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Start Frequency
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NASel, (sFilterRejMeasIPParam.m_dPBStartFrequency * 1e6), 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to set Start Frequency Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Stop frequency
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NASel, (sFilterRejMeasIPParam.m_dPBStopFrequency * 1e6), 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to set Stop Frequency Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Datapoints
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDataPoints(u8NASel, sFilterRejMeasIPParam.m_u16DataPoints, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        strTemp = QString::asprintf("%s", "Unable to set Data points in Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
    /*Set Trigger Off*/
    sRetVal=p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_CNT_ENABLE);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to Trigger ON in NA");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
    /*Set the RF Output Power ON*/
    sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NASel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to enable RF output in NA");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
    /*Set Trigger On*/
    sRetVal=p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_CNT_DISABLE);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to Trigger ON in NA");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetMarkerSearchType(u8NASel, usTraceNo, 1, DP_NA_MARKER_SEARCH_MAX, &dMarkerMaxData, &dMarkerMaxFreq);  //id , channel no, marker no, marker 1 - peak , marker data
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to read Min. Marker Value in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    dPBMaxMag = dMarkerMaxData;
    d500_2000MaxMag = dMarkerMaxData;
    dPBMaxFreq = dMarkerMaxFreq;

    sFilterRejection.m_dMaxGain500_2000MHz = d500_2000MaxMag;

    msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NASel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s", "Unable to Turn RF OFF in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    /*HPF and LPF Rejection*/
    for(U8BIT u8Idx = 0; u8Idx < 2; u8Idx++)
    {
        if(g_bAutoTestStartStop == false)
        {
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                *out_bTestSts = false;
                strTemp = QString::asprintf("%s","Unable to insert to database");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            return DP_DP_APP_TEST_STOPPED;
        }

        if(u8Idx == 0)//HPF Rejection
        {
#ifndef _SIMULATION_INSTRUMENTS_
            //Add HPF Config
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                *out_bTestSts = false;
                strTemp = QString::asprintf("%s", "Unable to confiure Dual FEA Path and band");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            dOBStartFreq = sFilterRejMeasIPParam.m_dHPFOBStartFrequency;
            dOBStopFreq = sFilterRejMeasIPParam.m_dHPFOBStopFrequency;
            dMarkerFreq = sFilterRejMeasIPParam.m_dHPFRejectionFrequency;
        }
        else//LPF Rejection
        {
            if(g_bAutoTestStartStop == false)
            {
                break;
            }

#ifndef _SIMULATION_INSTRUMENTS_
            //Add LPF Config
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                *out_bTestSts = false;
                strTemp = QString::asprintf("%s", "Unable to confiure Dual FEA Path and band");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            dOBStartFreq = sFilterRejMeasIPParam.m_dLPFOBStartFrequency;
            dOBStopFreq = sFilterRejMeasIPParam.m_dLPFOBStopFrequency;
            dMarkerFreq = sFilterRejMeasIPParam.m_dLPFRejectionFrequency;
        }

        msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Start Frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NASel, (dOBStartFreq * 1e6), 1);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to set Start Frequency Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Stop frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NASel, (dOBStopFreq * 1e6), 1);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to set Stop Frequency Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Datapoints
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDataPoints(u8NASel, sFilterRejMeasIPParam.m_u16DataPoints, 1);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s", "Unable to set Data points in Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
        /*Set Trigger Off*/
        sRetVal=p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_CNT_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Trigger ON in NA");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /*Set the RF Output Power ON*/
        sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NASel);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to enable RF output in NA");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
        /*Set Trigger On*/
        sRetVal=p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_CNT_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Trigger ON in NA");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        iMarkerNo = 1;
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Marker Position
        sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetMarkerPosAndFreq(u8NASel, iMarkerNo, (dMarkerFreq * 1e6), usTraceNo);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to set marker position in NA");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal=p_objMainApp->m_objNAWrap.DP_NA_ReadMarkerValue(u8NASel, iMarkerNo, DP_LOG_MODE, usTraceNo, &Out_dMarkerValue);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to read data at marker position in NA");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NASel);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Turn RF OFF in Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        if(u8Idx == 0)
        {
            sFilterRejection.m_dHPFLoss = Out_dMarkerValue;
            sFilterRejection.m_dHPFRejection = (dPBMaxMag - sFilterRejection.m_dHPFLoss);
        }
        else
        {
            sFilterRejection.m_dNotchLoss = Out_dMarkerValue;
            sFilterRejection.m_dNotchRejection = (dPBMaxMag - sFilterRejection.m_dNotchLoss);
        }
    }

    msleep(500);

    for(U8BIT u8BandSel = 0; u8BandSel < 2; u8BandSel++)
    {
        if(g_bAutoTestStartStop == false)
        {
            sRetVal = OnSafeState();
            if(sRetVal)
            {
                *out_bTestSts = false;
                strTemp = QString::asprintf("%s","Unable to insert to database");
                emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }
            return DP_DP_APP_TEST_STOPPED;
        }

        if(u8BandSel == 0)//1.02 - 1.1 GHz
        {
            u8SearchType = DP_NA_MARKER_SEARCH_MIN;
            dPBStartFreq = sFilterRejMeasIPParam.m_dNotchRejectionStartFrequency;
            dPBStopFreq = sFilterRejMeasIPParam.m_dNotchRejectionStopFrequency;

            /*Notch Rejection*/
#ifndef _SIMULATION_INSTRUMENTS_
            //Add Notch Config
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                *out_bTestSts = false;
                strTemp = QString::asprintf("%s", "Unable to confiure Dual FEA Path and band");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

            msleep(500);
        }
        else
        {

            if(g_bAutoTestStartStop == false)
            {
                sRetVal = OnSafeState();
                if(sRetVal)
                {
                    *out_bTestSts = false;
                    strTemp = QString::asprintf("%s","Unable to insert to database");
                    emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
                    return DP_FAILURE;
                }
                return DP_DP_APP_TEST_STOPPED;
            }

            u8SearchType = DP_NA_MARKER_SEARCH_MAX;
            dPBStartFreq = sFilterRejMeasIPParam.m_dNotchPBStartFrequency;
            dPBStopFreq = sFilterRejMeasIPParam.m_dNotchPBStopFrequency;

            /*Notch Rejection*/
#ifndef _SIMULATION_INSTRUMENTS_
            //Add Notch Config
#else
            sRetVal = 0;
#endif
            if(sRetVal < 0)
            {
                *out_bTestSts = false;
                strTemp = QString::asprintf("%s", "Unable to confiure Dual FEA Path and band");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                return DP_FAILURE;
            }

        }

        msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Start Frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NASel, (dPBStartFreq * 1e6), 1);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to set Start Frequency Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Stop frequency
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NASel, (dPBStopFreq * 1e6), 1);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to set Stop Frequency Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Set Datapoints
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDataPoints(u8NASel, sFilterRejMeasIPParam.m_u16DataPoints, 1);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            strTemp = QString::asprintf("%s", "Unable to set Data points in Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /*Set Trigger Off*/
        sRetVal=p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_CNT_ENABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Trigger ON in NA");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        /*Set the RF Output Power ON*/
        sRetVal=p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NASel);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to enable RF output in NA");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
        /*Set Trigger On*/
        sRetVal=p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NASel, DP_CNT_DISABLE);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Trigger ON in NA");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        msleep(500);

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetMarkerSearchType(u8NASel, usTraceNo, 1, u8SearchType, &dMarkerMaxData, &dMarkerMaxFreq);  //id , channel no, marker no, marker 1 - peak , marker data
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to read Min. Marker Value in Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

        if(u8BandSel == 0)
        {
            sFilterRejection.m_dLPFLoss = dMarkerMaxData;
        }
        else
        {
            dPBMaxMag = dMarkerMaxData;
            sFilterRejection.m_dMaxGain10_18GHz = dPBMaxMag;
            sFilterRejection.m_dLPFRejection = (dPBMaxMag - sFilterRejection.m_dLPFLoss);
        }

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NASel);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTestSts = false;
            strTemp = QString::asprintf("%s", "Unable to Turn RF OFF in Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

    }


    sFilterRejection.m_dInputPower = sFilterRejMeasIPParam.m_dInputPower;
    sFilterRejection.m_dExpRejection = sFilterRejMeasIPParam.m_dExpectdMinmumFilterRejection;

    if((sFilterRejection.m_dLPFRejection > sFilterRejection.m_dExpRejection) && (sFilterRejection.m_dHPFRejection > sFilterRejection.m_dExpRejection) && (sFilterRejection.m_dNotchRejection > sFilterRejection.m_dExpRejection))
    {
        sFilterRejection.m_s32Result = DP_APP_PASS;
        strTemp = QString::asprintf("Calculated HPF rejection: %0.2lf, Calculated LPF rejection: %0.2lf, Calculated notch rejection: %0.2lf, Result:%s",  sFilterRejection.m_dHPFRejection, sFilterRejection.m_dLPFRejection, sFilterRejection.m_dNotchRejection, "Pass");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strTemp);
    }
    else
    {
        sFilterRejection.m_s32Result = DP_APP_FAIL;
        *out_bTestSts = false;
        strTemp = QString::asprintf(" Calculated HPF rejection: %0.2lf, Calculated LPF rejection: %0.2lf, Calculated notch rejection: %0.2lf, Result:%s",  sFilterRejection.m_dHPFRejection, sFilterRejection.m_dLPFRejection, sFilterRejection.m_dNotchRejection, "Fail");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
    }

    qvFilterRejectionMeasData.push_back(sFilterRejection);

    /* Print to Report */
    sRetVal = OnSafeState();
    if(sRetVal)
    {
        *out_bTestSts = false;
        strTemp = QString::asprintf("%s","Unable to insert to database");
        emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE,strTemp);
        return DP_FAILURE;
    }

    strTemp = QString::asprintf("%s","Filter Rejection Measurement Test Completed");
    emit sig_MsgToPrintlog( PRINTLOG_TYPE_INFO, strTemp);

    return DP_SUCCESS;
}

short CAutoTestThread::Tx_Phaseshifter_Validation(bool *out_bTstSts, char *in_szTestname)
{
    QStringList strListDCAData;
    U8BIT u8LatchEnable             = DP_APP_ZERO;
    QStringList strListModeData;
    QStringList strListData;
    QString strModeFilePath;
    U8BIT u8Index                   = DP_APP_ZERO;
    QStringList strListDPSData;
    short sRetVal                   = DP_APP_ZERO;
    unsigned char ucPrecision       = 2;
    U16BIT usTrace                  = DP_APP_ZERO;
    QString strActLog               = "";
    QString strTemp                 = "";
    char szStateFilepath[512]       = {""};
    QString strStateFilepath        = "D:\\NA_CALIB_DATA";
    QString strTestName             = "";
    char szUserMsg[1024]            = {0};
    U32BIT u32TestCaseID            = DP_APP_ZERO;
    U32BIT u32TableStructsize       = DP_APP_ZERO;
    U32BIT u32Idx                   = DP_APP_ZERO;
    U16BIT u16AttValue              = DP_APP_ZERO;
    U8BIT u8RXEnable = 0;
    PSDPAGNA_WRP_PHASETRACE_DATA pSPhaseTracedata;
    U8BIT u8NetworkAnalyzerSel = 1;

    S_MODULENAME_TXPHASESHIFTERVALIDATION sTxPhaseShifterValIPparam;
    S_MODULEID_TX_PHASESHIFTER_VALIDATION sTxPhaseShifterValidation;
    *out_bTstSts = true;

    sprintf(szStateFilepath, "%s", strStateFilepath.toStdString().c_str());
    memset(&sTxPhaseShifterValidation, 0.0, sizeof(S_MODULEID_TX_PHASESHIFTER_VALIDATION));

    if(in_szTestname == NULL)
    {
        *out_bTstSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTstSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sTxPhaseShifterValIPparam);
    if(sRetVal)
    {
        *out_bTstSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Tx attenuation Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sprintf(szUserMsg, "Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser(szUserMsg);

    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(sTxPhaseShifterValIPparam.m_dStartFrequency, sTxPhaseShifterValIPparam.m_dStopFrequency, sTxPhaseShifterValIPparam.m_dInputPower, sTxPhaseShifterValIPparam.m_u16NoOfPoints, strStateFilepath,1/*Trace*/);
        if(sRetVal < 0)
        {
            *out_bTstSts = false;
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

    } // calib end
    pSPhaseTracedata = (PSDPAGNA_WRP_PHASETRACE_DATA) malloc(sizeof(SDPAGNA_WRP_PHASETRACE_DATA) * sTxPhaseShifterValIPparam.m_u16NoOfPoints);
    if(pSPhaseTracedata == NULL)
    {
        *out_bTstSts = false;
        strTemp= QString::asprintf("%s", "Memory Allocation Failed");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    memset(pSPhaseTracedata, 0, (sizeof(SDPAGNA_WRP_PHASETRACE_DATA) * sTxPhaseShifterValIPparam.m_u16NoOfPoints));

#ifndef _SIMULATION_INSTRUMENTS_
    //Recall State file
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NetworkAnalyzerSel, szStateFilepath);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to load State file in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    usleep(1000);

#ifndef _SIMULATION_INSTRUMENTS_
    //Select trace
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NetworkAnalyzerSel, sTxPhaseShifterValIPparam.m_u16NoOfPoints, usTrace, 1, 1);//trace, port, channel
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    usleep(100);
    usTrace = 2;//S21 trace

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal=p_objMainApp->m_objNAWrap.DP_NA_DisplayTrace(u8NetworkAnalyzerSel, usTrace, 1, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to display trace in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    usleep(100);
#ifndef _SIMULATION_INSTRUMENTS_
    //Set Power level in NA
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NetworkAnalyzerSel, (sTxPhaseShifterValIPparam.m_dInputPower), 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to config Power Level in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    usleep(100);
#ifndef _SIMULATION_INSTRUMENTS_
    //Set Trigger On
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }


#ifndef _SIMULATION_INSTRUMENTS_
    //Set Trigger On
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_HOLD);//Hold
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set the RF Output Power ON
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NetworkAnalyzerSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to Turn ON RF in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }

    usleep(2000);
#ifndef _SIMULATION_INSTRUMENTS_
    //Set Trigger On
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }
    u32TableStructsize = sTxPhaseShifterValIPparam.m_vectPHASEDATA.size();
    for(u32Idx = 0; u32Idx < u32TableStructsize ; u32Idx++)
    {

//        sTxPhaseShifterValidation.m_u8ChannelNo = m_pobjAutoTest->m_ucChannelNo;
        sTxPhaseShifterValidation.m_dInputPower = sTxPhaseShifterValIPparam.m_dInputPower;
        sTxPhaseShifterValidation.m_dExpMinPhase = sTxPhaseShifterValIPparam.m_vectPHASEDATA.at(u32Idx).m_dExpectedMinimumPhaseShift;
        sTxPhaseShifterValidation.m_dExpMaxPhase = sTxPhaseShifterValIPparam.m_vectPHASEDATA.at(u32Idx).m_dExpectedMaximumPhaseShift;
        sTxPhaseShifterValidation.m_dPhase = sTxPhaseShifterValIPparam.m_vectPHASEDATA.at(u32Idx).m_dPhaseShift;

       /*Add Configuration*/
#if 0
        if(m_u8ChNSel == 0)//ch1
        {
            u8Index = 1;
            u8LatchEnable = 0x1;   /*DCA Latch Enable*/
        }
        else if(m_u8ChNSel == 1)//ch2
        {
            u8Index = 2;
            u8LatchEnable = 0x2;
        }
        else//both
        {
            u8Index = 3;
            u8LatchEnable = 0x3;
        }
        UpdateModeData(u8Index, u8RXEnable);
        UpdateFileAttenuationData(u8LatchEnable,u8Index);
        UpdatePhaseData(sTxPhaseShifterValidation.m_dPhase);
#endif
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTstSts = false;
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            free(pSPhaseTracedata);
            return DP_FAILURE;
        }


#ifndef _SIMULATION_INSTRUMENTS_
        //Set Trigger On
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_HOLD);//Hold
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTstSts = false;
            strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            free(pSPhaseTracedata);
            return DP_FAILURE;
        }

        U8BIT u8TotalAttenIdx = sTxPhaseShifterValIPparam.m_vectPHASEDATA.count();
        FDOUBLE dMeasPhase[u8TotalAttenIdx][sTxPhaseShifterValIPparam.m_u16NoOfPoints];

#ifndef _SIMULATION_INSTRUMENTS_
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_ReadPhaseTraceData(u8NetworkAnalyzerSel, sTxPhaseShifterValIPparam.m_dStartFrequency, sTxPhaseShifterValIPparam.m_dStopFrequency, sTxPhaseShifterValIPparam.m_u16NoOfPoints, pSPhaseTracedata);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTstSts = false;
            strTemp = QString::asprintf("%s", "Unable to read trace data in Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            free(pSPhaseTracedata);
            return DP_FAILURE;
        }

        for(S32BIT u32DataIdx = 0; u32DataIdx < sTxPhaseShifterValIPparam.m_u16NoOfPoints; u32DataIdx++)
        {
            dMeasPhase[u32Idx][u32DataIdx] = pSPhaseTracedata[u32DataIdx].m_dPhase;

            sTxPhaseShifterValidation.m_dMeasuredPhase = dMeasPhase[u32Idx][u32DataIdx];
            sTxPhaseShifterValidation.m_dInputFrequency = pSPhaseTracedata[u32DataIdx].m_dFreq;
            long lCalPhase	= (long)(dproundoff(dMeasPhase[u32Idx][u32DataIdx], ucPrecision) * pow(10, ucPrecision));
            //sTxPhaseShifterValidation.m_dExpMinPhase 	= (long)(DP_CRF_7385_dproundoff(sTxPhaseShifterValidation.m_dExpMinPhase, ucPrecision) * pow(10, ucPrecision));
            //sTxPhaseShifterValidation.m_dExpMaxPhase 	= (long)(DP_CRF_7385_dproundoff(sTxPhaseShifterValidation.m_dExpMaxPhase, ucPrecision) * pow(10, ucPrecision));
            strTemp = QString::asprintf("IP Frequency: %lf, applied phase shift : %.1lf, Measured Phase : %.3lf degree",sTxPhaseShifterValidation.m_dInputFrequency,sTxPhaseShifterValidation.m_dPhase, sTxPhaseShifterValidation.m_dMeasuredPhase);

            if((lCalPhase < sTxPhaseShifterValidation.m_dExpMinPhase) || (lCalPhase > sTxPhaseShifterValidation.m_dExpMaxPhase))
            {
                sTxPhaseShifterValidation.m_u8Result = DP_TEST_FAIL;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                *out_bTstSts = false;
            }
            else
            {
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strTemp);
                sTxPhaseShifterValidation.m_u8Result = DP_TEST_PASS;
            }

            QVectTxPhaseShiftMeasurement.push_back(sTxPhaseShifterValidation);
        }
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NetworkAnalyzerSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to read trace data in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }

    free(pSPhaseTracedata);

#if 0
    u8Index = 0;//Idle Mode
    u8LatchEnable = 0x7;
    u16AttValue = (31.5 * 2);
    UpdateModeData(u8Index, u8RXEnable);
    UpdateAttValidationData(u8LatchEnable,u16AttValue);
#endif
    sRetVal = OnSafeState();
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s","Unable to insert to database");
        emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    return DP_SUCCESS;
}
short CAutoTestThread::Tx_Phaseshifter_Calibration(bool *out_bTstSts, char *in_szTestname)
{
    QStringList strListDCAData;
    U8BIT u8LatchEnable             = DP_APP_ZERO;
    QStringList strListModeData;
    QStringList strListData;
    QString strModeFilePath;
    U8BIT u8Index              = DP_APP_ZERO;
    QStringList strListDPSData;
    short sRetVal              = DP_APP_ZERO;
    unsigned char ucPrecision  = 2;
    U16BIT usTrace             = DP_APP_ZERO;
    QString strActLog          = "";
    QString strTemp            = "";
    char szStateFilepath[512]  = {""};
    QString strStateFilepath   = "D:\\NA_CALIB_DATA";
    QString strTestName        = "";
    char szUserMsg[1024]       = {0};
    U32BIT u32TestCaseID       = DP_APP_ZERO;
    S16BIT s16MarkerNo         = DP_APP_ZERO;
    U8BIT u8MeasType           = DP_APP_ZERO;
    FDOUBLE dMarkerFreq        = DP_APP_ZERO;
    FDOUBLE dPhaseVariation    = DP_APP_ZERO;
    S16BIT s16phase            = DP_APP_ZERO;
    FDOUBLE dMarkerValue       = DP_APP_ZERO;
    U8BIT u8RXEnable = 0;
     U8BIT u8NetworkAnalyzerSel = 1;
    PSDPAGNA_WRP_PHASETRACE_DATA pSPhaseTracedata;

    S_MODULENAME_TXPHASESHIFTERCALIBRATION sTxPhaseShifterCalibValIPparam;
    S_MODULEID_TX_PHASESHIFTER_CALIBRATION sTxPhaseShifterCalibValidation;
    *out_bTstSts = true;

    sprintf(szStateFilepath, "%s", strStateFilepath.toStdString().c_str());
    memset(&sTxPhaseShifterCalibValidation, 0.0, sizeof(S_MODULEID_TX_PHASESHIFTER_CALIBRATION));

    if(in_szTestname == NULL)
    {
        *out_bTstSts = false;
        return DP_ERR_INVALID_PARAM;
    }

    strTestName = QString::asprintf("%s", in_szTestname);

    sRetVal = m_pobjReportWrapper->ReadTestCaseID(strTestName.toStdString().c_str(), &u32TestCaseID);
    if(sRetVal)
    {
        *out_bTstSts = false;
        strActLog = QString::asprintf("%s", "Unable to read Test Case ID");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sRetVal = m_pobjReadConfigData->ReadConfigData(u32TestCaseID, 1, &sTxPhaseShifterCalibValIPparam);
    if(sRetVal)
    {
        *out_bTstSts = false;
        strActLog = QString::asprintf("%s", "Unable to Read Tx attenuation Configuration Data");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strActLog);
        return DP_FAILURE;
    }

    sprintf(szUserMsg, "Do you want to perform calibration ? \n If Yes, Connect the cable between Network Analyzer Port A and E-Cal Kit port A and the Press on Yes button\n If No, then Press on No button to continue testing");
    emit sig_QusToUser(szUserMsg);

    if(g_ucPressYES_NO)
    {
        sRetVal = PerformNACalib(sTxPhaseShifterCalibValIPparam.m_dStartFrequency, sTxPhaseShifterCalibValIPparam.m_dStopFrequency, sTxPhaseShifterCalibValIPparam.m_dInputPower, sTxPhaseShifterCalibValIPparam.m_u16NoOfPoints, strStateFilepath,1/*Trace Selection*/);
        if(sRetVal < 0)
        {
            *out_bTstSts = false;
            strTemp = QString::asprintf("%s", "Unable to Perform Network Analyzer Calibration");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

    } // calib end
    pSPhaseTracedata = (PSDPAGNA_WRP_PHASETRACE_DATA) malloc(sizeof(SDPAGNA_WRP_PHASETRACE_DATA) * sTxPhaseShifterCalibValIPparam.m_u16NoOfPoints);
    if(pSPhaseTracedata == NULL)
    {
        *out_bTstSts = false;
        strTemp= QString::asprintf("%s", "Memory Allocation Failed");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    memset(pSPhaseTracedata, 0, (sizeof(SDPAGNA_WRP_PHASETRACE_DATA) * sTxPhaseShifterCalibValIPparam.m_u16NoOfPoints));

#ifndef _SIMULATION_INSTRUMENTS_
    //Recall State file
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_LoadStateFromFile(u8NetworkAnalyzerSel, szStateFilepath);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to load State file in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    usleep(1000);

#ifndef _SIMULATION_INSTRUMENTS_
    //Select trace
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NetworkAnalyzerSel, sTxPhaseShifterCalibValIPparam.m_u16NoOfPoints, usTrace, 1, 1);//trace, port, channel
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    usleep(100);
    usTrace = 2;//S21 trace

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal=p_objMainApp->m_objNAWrap.DP_NA_DisplayTrace(u8NetworkAnalyzerSel, usTrace, 1, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to display trace in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    usleep(100);
#ifndef _SIMULATION_INSTRUMENTS_
    //Set Power level in NA
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NetworkAnalyzerSel, (sTxPhaseShifterCalibValIPparam.m_dInputPower), 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to config Power Level in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }


#ifndef _SIMULATION_INSTRUMENTS_
    //Set the RF Output Power ON
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NetworkAnalyzerSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to Turn ON RF in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }

    usleep(2000);
#ifndef _SIMULATION_INSTRUMENTS_
    //Set Trigger On
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }


#ifndef _SIMULATION_INSTRUMENTS_
    //Set Trigger On
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_HOLD);//Hold
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    /*Format change to phase*/
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDisplayFormat(u8NetworkAnalyzerSel, 2, 1);//continuous
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to set Display format in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }
    for(S16BIT s16Frequency = sTxPhaseShifterCalibValIPparam.m_dStartFrequency; s16Frequency <= sTxPhaseShifterCalibValIPparam.m_dStopFrequency; s16Frequency += sTxPhaseShifterCalibValIPparam.m_dStepFrequency)
    {

        s16MarkerNo = 2;
        u8MeasType = 2;
        dMarkerFreq = s16Frequency;

        s16phase = 0;
#ifndef _SIMULATION_INSTRUMENTS_
        //Set Marker Position
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetMarkerPosAndFreq(u8NetworkAnalyzerSel, s16MarkerNo,dMarkerFreq*1e6,1);
#else
        sRetVal = 0;
#endif
        if(sRetVal)
        {
            *out_bTstSts = false;
            strTemp = QString::asprintf("%s", "Unable to set Marker position in Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            free(pSPhaseTracedata);
            return DP_FAILURE;
        }
        usleep(100);

        dPhaseVariation = 6;
        do
        {
#if 0
            if(s16phase < 0)
            {
                s16phase = 355 + s16phase;
            }

            if(m_u8ChNSel == 0)//ch1
            {
                u8Index = 1;
                u8LatchEnable = 0x1;   /*DCA Latch Enable*/
            }
            else if(m_u8ChNSel == 1)//ch2
            {
                u8Index = 2;
                u8LatchEnable = 0x2;
            }
            else//both
            {
                u8Index = 3;
                u8LatchEnable = 0x3;
            }


            UpdatePhaseData(u8Index);
            UpdateModeData(u8Index, u8RXEnable);
            UpdateFileAttenuationData(u8LatchEnable,u8Index);
            UpdatePhaseData(u8Index);
#endif
#ifndef _SIMULATION_INSTRUMENTS_
            //Set Trigger On
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_CONT);//continuous
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                *out_bTstSts = false;
                strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                free(pSPhaseTracedata);
                return DP_FAILURE;
            }


#ifndef _SIMULATION_INSTRUMENTS_
            //Set Trigger On
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, DP_NA_TRIGGER_MODE_HOLD);//Hold
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                *out_bTstSts = false;
                strTemp = QString::asprintf("%s", "Unable to config Trigger in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                free(pSPhaseTracedata);
                return DP_FAILURE;
            }

            //FDOUBLE dMeasPhase[u8TotalAttenIdx][sTxPhaseShifterCalibValIPparam.m_u16NoOfPoints];

//            sTxPhaseShifterCalibValidation.m_u8ChannelNo = m_pobjAutoTest->m_ucChannelNo;
            sTxPhaseShifterCalibValidation.m_dInputFrequency = sTxPhaseShifterCalibValIPparam.m_dStartFrequency;
            sTxPhaseShifterCalibValidation.m_dInputPower = sTxPhaseShifterCalibValIPparam.m_dInputPower;

#ifndef _SIMULATION_INSTRUMENTS_
            sRetVal = p_objMainApp->m_objNAWrap.DP_NA_ReadMarkerValue(u8NetworkAnalyzerSel, s16MarkerNo,u8MeasType,1,&dMarkerValue);
#else
            sRetVal = 0;
#endif
            if(sRetVal)
            {
                *out_bTstSts = false;
                strTemp = QString::asprintf("%s", "Unable to read marker value in Network Analyzer");
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
                free(pSPhaseTracedata);
                return DP_FAILURE;
            }
            usleep(100);
            strTemp = QString::asprintf("IP Frequency: %lf, Applied phase: %d, Measured Phase : %.3lf Deg",dMarkerFreq,s16phase,dMarkerValue);
            if(dMarkerValue <= dPhaseVariation && dMarkerValue >= -dPhaseVariation)
            {
                sTxPhaseShifterCalibValidation.m_u8Result = DP_TEST_PASS;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,strTemp);
            }
            else
            {
                *out_bTstSts = false;
                sTxPhaseShifterCalibValidation.m_u8Result = DP_TEST_FAIL;
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,strTemp);
            }
            for(U32BIT u32DataIdx = 0; u32DataIdx < sTxPhaseShifterCalibValIPparam.m_u16NoOfPoints; u32DataIdx++)
            {
                //dMeasPhase[0][u32DataIdx] = pSPhaseTracedata[u32DataIdx].m_dPhase;
                strTemp = QString::asprintf("IP Frequency: %lf, applied phase shift : %.1lf, Measured Phase : %.3lf degree",pSPhaseTracedata[u32DataIdx].m_dFreq, sTxPhaseShifterCalibValIPparam.m_dInputPower, pSPhaseTracedata[u32DataIdx].m_dPhase);

                sTxPhaseShifterCalibValidation.m_dInputFrequency = pSPhaseTracedata[u32DataIdx].m_dFreq;
                //sTxPhaseShifterCalibValidation.m_dMeasuredPhase = dMeasPhase[0][u32DataIdx];
                //long lCalPhase	= (long)(DP_CRF_7385_dproundoff(dMeasPhase[0][u32DataIdx], ucPrecision) * pow(10, ucPrecision));
                sTxPhaseShifterCalibValidation.m_dExpMinPhase 	= (long)(dproundoff(sTxPhaseShifterCalibValidation.m_dExpMinPhase, ucPrecision) * pow(10, ucPrecision));
                sTxPhaseShifterCalibValidation.m_dExpMaxPhase 	= (long)(dproundoff(sTxPhaseShifterCalibValidation.m_dExpMaxPhase, ucPrecision) * pow(10, ucPrecision));

                QVectTxPhaseShiftCALMeasurement.push_back(sTxPhaseShifterCalibValidation);
                s16phase = s16phase - dMarkerValue;

            }
        }while(dMarkerValue > dPhaseVariation || dMarkerValue < -dPhaseVariation);

        strTemp = QString::asprintf("%lf,%.3lf,%lf\n",dMarkerFreq,s16phase,dMarkerValue);
    }
#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NetworkAnalyzerSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s", "Unable to read trace data in Network Analyzer");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        free(pSPhaseTracedata);
        return DP_FAILURE;
    }

    free(pSPhaseTracedata);
    u8Index = 0;//Idle Mode
    u8LatchEnable = 0x7;
    U16BIT u16AttValue = (31.5 * 2);

#if 0

    UpdateModeData(u8Index, u8RXEnable);
    UpdateAttValidationData(u8LatchEnable,u16AttValue);
#endif
    sRetVal = OnSafeState();
    if(sRetVal)
    {
        *out_bTstSts = false;
        strTemp = QString::asprintf("%s","Unable to insert to database");
        emit sig_MsgToPrintlog( PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    return DP_SUCCESS;
}

double CAutoTestThread::dpdbmtoWatt(double in_ddbm)
{
    return pow(10,in_ddbm/10)/1000;
}

double CAutoTestThread :: dproundoff(double in_dData, unsigned char in_ucPrecision)
{
    long lData                      = DP_APP_ZERO;
    double dData                    = DP_APP_ZERO;
    double dMulFact                 = DP_APP_ZERO;
    
    dMulFact =  pow(10, in_ucPrecision);
    
    dData = (in_dData * dMulFact);
    lData = (long)dData;
    
    if((dData - lData) >= 0.5 )
    {
        lData += 1;
    }
    
    return (((double)lData)/dMulFact);
}

short  CAutoTestThread::OnSafeState()
{
    short sRetVal = 0;
    if(QVectRx_Gain_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_rx_gain_measurement_Record(QVectRx_Gain_Measurement, "Gain Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectRx_Gain_Measurement.clear();
    }
    else if(QVect_Harmonics_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_harmonic_measurement_Record(QVect_Harmonics_Measurement, "Harmonics Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVect_Harmonics_Measurement.clear();
    }
    else if(QVect_NoiseFig_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_noisefigure_measurement_Record(QVect_NoiseFig_Measurement, "Noise Figure Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVect_NoiseFig_Measurement.clear();
    }
    else if(QVect_PowerDroop_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_powerdroop_measurement_Record(QVect_PowerDroop_Measurement, "Power Droop Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVect_PowerDroop_Measurement.clear();
    }
    else if(QVect_RF_PulseRise_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_rf_pulse_rise_fall_time_meas_Record(QVect_RF_PulseRise_Measurement, "RF Pulse/Rise Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVect_RF_PulseRise_Measurement.clear();
    }
    else if(QVect_InputReturnLoss_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_input_return_loss_Record(QVect_InputReturnLoss_Measurement, "Input Return Loss Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVect_InputReturnLoss_Measurement.clear();
    }
    else if(QVect_OutputReturnLoss_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_output_return_loss_Record(QVect_OutputReturnLoss_Measurement, "Output Return Loss Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVect_OutputReturnLoss_Measurement.clear();
    }
    else if(QVectRxIsolation_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_rx_isolation_measurement_Record(QVectRxIsolation_Measurement, "Rx Isolation Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectRxIsolation_Measurement.clear();
    }
    else if(QVectTx_OutputPower_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_tx_output_power_measurement_Record(QVectTx_OutputPower_Measurement, "Tx Output Power Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectTx_OutputPower_Measurement.clear();
    }
    else if(QVect_PowerTuning_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_power_tuning_measurement_Record(QVect_PowerTuning_Measurement, "Power Tuning Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVect_PowerTuning_Measurement.clear();
    }
    else if(QVect_RxOutofBand_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_rx_outofband_rejection_measurement_Record(QVect_RxOutofBand_Measurement, "Rx Out Of Band Rejection Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVect_RxOutofBand_Measurement.clear();
    }
    else if(QVectRx_PhaseNoise_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_phase_noise_measurement_Record(QVectRx_PhaseNoise_Measurement, "Phase Noise Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectRx_PhaseNoise_Measurement.clear();
    }
    else if(QVectImageRejection.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_image_rejection_Record(QVectImageRejection, "Image  Rejection Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectImageRejection.clear();
    }
    else if(QVectReceiver1dBCompressionPoint.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_onedbcompressionpoint_Record(QVectReceiver1dBCompressionPoint, "P1dB Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectReceiver1dBCompressionPoint.clear();
    }
    else if(QVectRx_Attenuator_Validation.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_rx_attenuator_validation_Record(QVectRx_Attenuator_Validation, "Attenuator Validation Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectRx_Attenuator_Validation.clear();
    }
    else if(QVectRx_Gain_Measurement_SA.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_rx_gain_measurement_sa_Record(QVectRx_Gain_Measurement_SA, "Gain Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectRx_Gain_Measurement_SA.clear();
    }
    else if(QVectVSWRMeasurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_vswr_measurement_Record(QVectVSWRMeasurement, "VSWR Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectVSWRMeasurement.clear();
    }
    else if(QVectRx_Spurious_Measurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_spurious_measurement_Record(QVectRx_Spurious_Measurement, "Spurious Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectRx_Spurious_Measurement.clear();
    }
    else if(qvBiteIsolationData.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_moduleid_biteisolation_meas_Record(qvBiteIsolationData, "BITE Isolation Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        qvBiteIsolationData.clear();
    }
    else if(qvBlankingIsolationData.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_moduleid_blanking_isolation_meas_Record(qvBlankingIsolationData, "Blanking Isolation Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        qvBlankingIsolationData.clear();
    }
    else if(qvFilterRejectionMeasData.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_moduleid_filter_rejection_Record(qvFilterRejectionMeasData, "Filter Rejection Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        qvFilterRejectionMeasData.clear();
    }
    else if(QVectTxPhaseShiftMeasurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_moduleid_tx_phaseshifter_validation_Record(QVectTxPhaseShiftMeasurement, "Phase Shift Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectTxPhaseShiftMeasurement.clear();
    }
    else if(QVectTxPhaseShiftCALMeasurement.size() > 0)
    {
        sRetVal = m_pobjReportWrapper->Insert_moduleid_tx_phaseshifter_calibration_Record(QVectTxPhaseShiftCALMeasurement, "Phase Shift Calibration Measurement Test");
        if(sRetVal)
        {
            return DP_FAILURE;
        }
        QVectTxPhaseShiftCALMeasurement.clear();
    }
    else
    {
        ;
    }
    return 0;
}

double CAutoTestThread :: Atten_Roundoff(double in_dData, unsigned char in_ucPrecision)
{
    long lData = 0;
    double dData = 0.0;
    double dMulFact = 0.0;

    dMulFact =  pow(10, in_ucPrecision);

    dData = (in_dData * dMulFact);
    lData = (long)dData;

    if((dData - lData) > 0.5 )
    {
        dData = round(in_dData);
    }
    else if((dData - lData) < 0.5 )
    {
        dData = round(in_dData);
    }
    else if((dData - lData) == 0.5)
    {
        dData = in_dData;
    }

    return dData;
}

void CAutoTestThread::GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg)
{
    switch(in_s32ErrVal)
    {
    case DP_ERR_DB_NOT_OPENED_1:
        sprintf(out_pcErrMsg, "DB : Error in Databse Connection");
        break;
    case DP_ERR_INVALID_TEST_ID_1:
        sprintf(out_pcErrMsg, "DB : Invalid Test ID");
        break;
    case DP_ERR_INVALID_CFN_NUM_1:
        sprintf(out_pcErrMsg, "DB : Invalid Configuration Number");
        break;
    case DP_ERR_QUERY_EXE_1:
        sprintf(out_pcErrMsg, "DB : Error in Query execution");
        break;
    case DP_ERR_FILE_OPEN_1:
        sprintf(out_pcErrMsg, "DB : FIle Open faild");
        break;
    default:
        sprintf(out_pcErrMsg, "DB : Unknown Error");
        break;
    }
}

short  CAutoTestThread :: ApplyCalibData(unsigned int in_uiPathID, double in_dFrequency, double in_dUncalibPower, double *out_dCalibPower, double *out_dCableloss)
{

    QString strQuery                = "";
    bool bEquFlag                   = true;
    bool bMinFlag                   = true;
    bool bMaxFlag                   = true;
    bool bRetVal                    = true;
    double dMinFreq                 = DP_APP_ZERO;
    double dMaxFreq                 = DP_APP_ZERO;
    double dMinPwr                  = DP_APP_ZERO;
    double dMaxPwr                  = DP_APP_ZERO;
    double dSlope                   = DP_APP_ZERO;
    double dConst                   = DP_APP_ZERO;
    double dCableLoss               = DP_APP_ZERO;


    if(in_uiPathID < DP_APP_MIN_PATHID_SA || in_uiPathID > DP_APP_MAX_PATHID_SA)
    {
        return DP_APP_FAIL;
    }

    if(in_dFrequency <= 0.0)
    {
        return DP_APP_FAIL	;
    }

    QSqlQuery query(m_pobjReportWrapper->m_objDataBase);

    strQuery= QString::asprintf("SELECT * FROM calib_cabloss_rpt WHERE PathID = %d AND (RadioFreq BETWEEN %f AND %f)",in_uiPathID, in_dFrequency - 0.000001, in_dFrequency + 0.000001);

    if(!query.exec(strQuery))
    {
        qWarning("%s", query.lastError().text().toLocal8Bit().data());
        bEquFlag = false;
        return PRINTLOG_TYPE_FAILURE;
    }
    else
    {

        if(query.next() == true)
        {
            dCableLoss = query.value(6).toDouble();

        }

        bEquFlag = true;
    }

    //if frequency is not available
    if(!bEquFlag)
    {

        //select minimum range
        strQuery= QString::asprintf("SELECT * FROM calib_cabloss_rpt WHERE PathID = %d AND RadioFreq < %f ORDER BY RadioFreq DESC LIMIT 1", in_uiPathID, in_dFrequency);
        query.prepare(strQuery);
        bRetVal = query.exec();
        if(!bRetVal)
        {
            qWarning("%s", query.lastError().text().toLocal8Bit().data());
            bMinFlag = false;
            return DP_APP_FAIL;
        }
        else
        {
            bMinFlag = true;
            bMinFlag = true;
            QSqlRecord m_Record = query.record();
            while(query.next())
            {
                dMinPwr             = m_Record.value(6).toDouble();
            }
            while(query.next())
            {
                dMinFreq            = m_Record.value(4).toDouble();
            }

        }

        // Select maximunm range
        strQuery= QString::asprintf("SELECT * FROM calib_cabloss_rpt WHERE PathID = %d AND RadioFreq > %f ORDER BY RadioFreq ASC LIMIT 1",in_uiPathID, in_dFrequency);
        query.prepare(strQuery);
        bRetVal = query.exec();
        if(!bRetVal)
        {
            qWarning("%s", query.lastError().text().toLocal8Bit().data());
            bMinFlag = false;
            return DP_APP_FAIL;
        }
        else
        {
            bMinFlag = true;
            QSqlRecord m_Record = query.record();
            while(query.next())
            {
                dMaxPwr            = m_Record.value(6).toDouble();
            }
            while(query.next())
            {
                dMaxFreq           = m_Record.value(4).toDouble();
            }

        }
        //if both flags are set
        if(bMinFlag && bMaxFlag)
        {
            //equation of straight line to be performed (y = mx + c)

            //To find slope(m = (y2 - y1)/(x2 - x1))
            dSlope = (dMaxPwr - dMinPwr)/(dMaxFreq - dMinFreq);

            //To find constant(c = y1 - mx1) or (c = y2 - mx2)
            dConst = dMinPwr - (dSlope * dMinFreq);

            //To find unknown power (y = mx + c)
            dCableLoss = (dSlope * in_dFrequency) + dConst;
        }

        else
        {
            return DP_APP_FAIL;
        }
    }

    query.finish();

    *out_dCalibPower = in_dUncalibPower - dCableLoss;
    *out_dCableloss = dCableLoss;
    return 0;
}

S32BIT CAutoTestThread::PerformNACalib(FDOUBLE in_dStartFreq, FDOUBLE in_dStopFreq, FDOUBLE in_dInputPower, U32BIT in_u32DataPoints, QString in_strCalibStateFile, U8BIT in_u8TraceSel)
{
    S32BIT sRetVal = 0;
    U16BIT u8NetworkAnalyzerSel = 1;
    U16BIT u16Trace = 0;
    QString strActLog = "";
    QString strTemp = "";
    char  szStateFilePath[512] = {0};

    sprintf(szStateFilePath, "%s", in_strCalibStateFile.toStdString().c_str());

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_UserPreset(u8NetworkAnalyzerSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to Preset Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    msleep(1000);
#ifndef _SIMULATION_INSTRUMENTS_
    //Power Off
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NetworkAnalyzerSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strActLog = QString::asprintf("%s", "Unable to Power OFF in the Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DeleteAllTraces(u8NetworkAnalyzerSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strActLog = QString::asprintf("%s", "Unable to Delete all traces in Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    for(U8BIT u8TraceIdx = 0; u8TraceIdx < 4; u8TraceIdx++)
    {
        if(0 == ((in_u8TraceSel >> u8TraceIdx) & 1))
        {
            continue;
        }

        if(u8TraceIdx == 0)
        {
            u16Trace = 1;//S11 trace
        }
        else if(u8TraceIdx == 1)
        {
            u16Trace = 2;//S12 trace
        }
        else if(u8TraceIdx == 2)
        {
            u16Trace = 3;//S21 trace
        }
        else if(u8TraceIdx == 3)
        {
            u16Trace = 4;//S22 trace
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Create new trace
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_CreateNewTrace(u8NetworkAnalyzerSel,( u16Trace), 1);  //id , chn no , portno
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strActLog = QString::asprintf("%s", "Unable to create new trace in Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Display trace
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_DisplayTrace(u8NetworkAnalyzerSel, (u16Trace), 1, 1);  // id , Trace no ,port no , channel no
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strActLog = QString::asprintf("%s", "Unable to Display trace in Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
    }/*Trace end*/

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Start Frequency
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStartFrequency(u8NetworkAnalyzerSel, (in_dStartFreq * 1e6), 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to set Start Frequency Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Stop frequency
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetStopFrequency(u8NetworkAnalyzerSel, (in_dStopFreq * 1e6), 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to set Stop Frequency Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set power level in NA
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerLevel(u8NetworkAnalyzerSel, in_dInputPower, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to set power level in Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Datapoints
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetIFBandWidth(u8NetworkAnalyzerSel, 10000.0);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to set IF Band width in Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Datapoints
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetDataPoints(u8NetworkAnalyzerSel, in_u32DataPoints, 1);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to set Data points in Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Select trace
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SelectTrace(u8NetworkAnalyzerSel, in_u32DataPoints, (u16Trace), 1, 1);  // Trace No  port No , chn no
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to select trace in Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerON(u8NetworkAnalyzerSel);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to power ON in the Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

#ifndef _SIMULATION_INSTRUMENTS_
    //Set trigger
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, 1);  // ID , 0 - HOLD, 1- Continious
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to Turn ON Trigger in the Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }
    msleep(300);


    strTemp = QString::asprintf("Perform 2-Port Calibration in network analyzer \n1.After Completion of calibration press OK button\n2. If you wan to cancel Press Cancel button ");
    emit sig_QusToUser((char*)strTemp.toStdString().c_str());

    if(g_ucPressYES_NO)
    {
#ifndef _SIMULATION_INSTRUMENTS_
        //Power Off
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_SetPowerOFF(u8NetworkAnalyzerSel);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Power OFF in the Network Analyzer" );
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }

#ifndef _SIMULATION_INSTRUMENTS_
        //Save State File
        sRetVal = p_objMainApp->m_objNAWrap.DP_NA_StoreStateIntoFile(u8NetworkAnalyzerSel, szStateFilePath);
#else
        sRetVal = 0;
#endif
        if(sRetVal < 0)
        {
            strTemp = QString::asprintf("%s", "Unable to Store State File path of Network Analyzer");
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
            return DP_FAILURE;
        }
    }
    else
    {
        strTemp = QString::asprintf("%s", "Calibration Stopped");
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp);
        return DP_SUCCESS;
    }
    sleep(1);

#ifndef _SIMULATION_INSTRUMENTS_
    //Set Trigger Off
    sRetVal = p_objMainApp->m_objNAWrap.DP_NA_Trigger(u8NetworkAnalyzerSel, 0);
#else
    sRetVal = 0;
#endif
    if(sRetVal < 0)
    {
        strTemp = QString::asprintf("%s", "Unable to Turn OFF Trigger in the Network Analyzer" );
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strTemp);
        return DP_FAILURE;
    }

    strTemp = "Network analyzer calibration completed";
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp);

    return DP_SUCCESS;
}

S32BIT CAutoTestThread::PowerON()
{
    U8BIT u8Address             = 1;
    U8BIT u8SeriesType          = 0;

    S16BIT s16RetVal            = 0;
    FSINGLE fVoltage            = 50.0;
    FSINGLE fCurrent            = 11.0;
    FSINGLE fVoltageRD          = 0.0;
    FSINGLE fCurrentRD          = 0.0;
    FSINGLE m_fOverVoltageRD    = 0.0;
    FSINGLE m_fUnderVoltageRD   = 0.0;

    QString qsPSUError          = "";
    QString qstrModel           = "";

    qstrModel = QString::asprintf("%s",p_objMainApp->m_pobjCommunication->m_szModel);
    if(qstrModel.contains("GEN"))
    {
        u8SeriesType = DP_PSU_GEN_SERIES;
    }
    else
    {
        u8SeriesType = DP_PSU_Z_SERIES;
    }
    s16RetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->DP_PSU_OV_UV_Config(g_u16PSMInstanceID,u8Address, u8SeriesType, p_objMainApp->m_pobjCommunication->m_fVoltrating, 0.0);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Unable to configure Over and Under Voltage");
        return DP_FAILURE;
    }

    s16RetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->DP_PSU_VoltCurrConfig(g_u16PSMInstanceID,u8Address,DP_PSU_LOCAL_MODE, fVoltage, fCurrent);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Unable to configure Voltage and Current");
        return DP_FAILURE;
    }

    s16RetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->DP_PSU_OV_UV_Config(g_u16PSMInstanceID,u8Address,u8SeriesType, p_objMainApp->m_pobjCommunication->m_fOverVoltage, p_objMainApp->m_pobjCommunication->m_fUnderVoltage);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Unable to configure Over and Under Voltage");
        return DP_FAILURE;
    }

    s16RetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->DP_PSU_Readback_Config(g_u16PSMInstanceID,u8Address,&fVoltageRD,&fCurrentRD,&m_fOverVoltageRD,&m_fUnderVoltageRD);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Unable to Reaback Configuration");
        return DP_FAILURE;
    }

    if(fVoltage != fVoltageRD || fCurrent != fCurrentRD)
    {
        qsPSUError = "Voltage and Current not configured properly";
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,qsPSUError);
        return DP_FAILURE;
    }

    if(p_objMainApp->m_pobjCommunication->m_fOverVoltage != m_fOverVoltageRD || p_objMainApp->m_pobjCommunication->m_fUnderVoltage != m_fUnderVoltageRD)
    {
        qsPSUError = "Over Voltage and Over Current not configured properly";
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,qsPSUError);
        return DP_FAILURE;
    }

    s16RetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->SetFOLDEnable(g_u16PSMInstanceID, true);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Enable Fold failed");
        return DP_FAILURE;
    }

    s16RetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->DP_PSU_Output(g_u16PSMInstanceID,u8Address,DP_PSU_LOCAL_MODE,DP_PSU_OUTPUT_ON);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Enable PSU Out Failed");
        return DP_FAILURE;
    }

    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,"PSU Power ON Success");
    return DP_SUCCESS;
}

S32BIT CAutoTestThread::PowerOff()
{
    U8BIT u8Address             = 1;
    S16BIT s16RetVal            = 0;

    QString qsPSUError          = "";

    s16RetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->DP_PSU_Output(g_u16PSMInstanceID,u8Address,DP_PSU_LOCAL_MODE,DP_PSU_OUTPUT_OFF);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Disable PSU Out Failed");
        return DP_FAILURE;
    }

    s16RetVal = p_objMainApp->m_pobjCommunication->m_obj_CPSUWrapper->SetFOLDEnable(g_u16PSMInstanceID, false);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Disable Fold failed");
        return DP_FAILURE;
    }
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,"PSU Power OFF Success");
    return DP_SUCCESS;
}
