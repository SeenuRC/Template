#include "ctestcasewrapper.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlRecord>

extern MainWindow *p_objMainApp;
extern CReportConfigWrapper *m_pobjReportWrapper;

CTestCaseWrapper::CTestCaseWrapper()
{

}

void CTestCaseWrapper::DP_SG_SA_GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg)
{
    switch(in_s32ErrVal)
    {
    case DP_ERR_SIG_PRESET:
        sprintf(out_pcErrMsg, "Unable to Configure Preset in RF Signal Generator");
        break;
    case DP_ERR_SPECTRUM_PRESET:
        sprintf(out_pcErrMsg, "Unable to Configure Preset in Specturum Analyzer");
        break;
    case DP_ERR_SA_REFLEVEL:
        sprintf(out_pcErrMsg, "Unable to configure reference level");
        break;
    case DP_ERR_SA_RBW_VBW:
        sprintf(out_pcErrMsg, "Unable to configure RBW, VBW and sweep time");
        break;
    case DP_ERR_STIM_PATHID:
        sprintf(out_pcErrMsg, "Unable to Read the RF stimulus path calibration data");
        break;
    case DP_ERR_SA_SPAN_CF:
        sprintf(out_pcErrMsg, "Unable to configure center frequency and span");
        break;
    case DP_ERR_FREQ_POWER:
        sprintf(out_pcErrMsg, "Unable to configure frequency and power level in signal generator");
        break;
    case DP_ERR_RF_ENABLE_OUT:
        sprintf(out_pcErrMsg, "Unable to enable RF output in RF signal generator");
        break;
    case DP_ERR_SA_ENA_CNT:
        sprintf(out_pcErrMsg, "Unable to enable the continous run in spectrum analyzer");
        break;
    case DP_ERR_SA_DIS_CNT:
        sprintf(out_pcErrMsg, "Unable to disable the continous run in spectrum analyzer");
        break;
    case DP_ERR_SA_ACTIVE_MARKER:
        sprintf(out_pcErrMsg, "Unable to read marker frequency in spectrum analyzer");
        break;
    case DP_ERR_RF_DISABLE_OUT:
        sprintf(out_pcErrMsg, "Unable to disable RF output in RF signal generator");
        break;
    case DP_ERR_MEAS_PATHID:
        sprintf(out_pcErrMsg, "Unable to Read the Measurement path calibration data");
        break;
    default:
        sprintf(out_pcErrMsg, "Invalid Error");
        break;
    }
}

/**
*\brief          This function is used to find the one time initialization for SG and SA
*
*\param[in]  	 in_sInOutParam.m_dRefLevel  	 It specifies the referece level
* *\param[in]  	 in_sInOutParam.m_dRbw           It specifies the rbw
*\param[in]  	 in_sInOutParam.m_dVbw           It specifies the VBW
*\param[in]  	 in_sInOutParam.m_dSweepTime  	 It specifies the sweep time
*
*\retval	    ::DP_SUCCESS is returned upon success
*
*
*\author      Parthipan A
*\date        02 June 2023
*/

S32BIT CTestCaseWrapper::DP_SG_SA_Initialization(S_SG_SA_INOUT_PARAMETERS in_sInOutParam)
{
    S32BIT s32RetVal    = 0;

    // Configure Preset in RF SG
    s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_Preset(in_sInOutParam.m_u8SigGenSel);
    if(s32RetVal)
    {
        return DP_ERR_SIG_PRESET;
    }

    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(in_sInOutParam.m_u8SpecSel);
    if(s32RetVal < 0)
    {
        return DP_ERR_SPECTRUM_PRESET;
    }

    QThread::msleep(1000);

    // Config Ref Level In SA
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(in_sInOutParam.m_u8SpecSel, in_sInOutParam.m_dRefLevel);
    if(s32RetVal)
    {
        return DP_ERR_SA_REFLEVEL;
    }

    // Config  RBW , VBW in SA
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(in_sInOutParam.m_u8SpecSel, 0, (in_sInOutParam.m_dRbw * 1e3), 0, (in_sInOutParam.m_dVbw * 1e3), true, in_sInOutParam.m_dSweepTime);
    if(s32RetVal)
    {
        return DP_ERR_SA_RBW_VBW;
    }

    return DP_SUCCESS;
}

/**
*\brief          This function is used to find the one time initialization for SA only
*
*\param[in]  	 in_sInOutParam.m_dRefLevel  	 It specifies the referece level
* *\param[in]  	 in_sInOutParam.m_dRbw           It specifies the rbw
*\param[in]  	 in_sInOutParam.m_dVbw           It specifies the VBW
*\param[in]  	 in_sInOutParam.m_dSweepTime  	 It specifies the sweep time
*
*\retval	    ::DP_SUCCESS is returned upon success
*
*
*\author      Parthipan A
*\date        02 June 2023
*/
S32BIT CTestCaseWrapper::DP_SA_Initialization(S_SG_SA_INOUT_PARAMETERS in_sInOutParam)
{
    S32BIT s32RetVal    = 0;

    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_Reset(in_sInOutParam.m_u8SpecSel);
    if(s32RetVal < 0)
    {
        return DP_ERR_SPECTRUM_PRESET;
    }

    QThread::msleep(1000);

    // Config Ref Level In SA
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigLevel(in_sInOutParam.m_u8SpecSel, in_sInOutParam.m_dRefLevel);
    if(s32RetVal)
    {
        return DP_ERR_SA_REFLEVEL;
    }

    // Config  RBW , VBW in SA
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigSweepCoupling(in_sInOutParam.m_u8SpecSel, 0, (in_sInOutParam.m_dRbw * 1e3), 0, (in_sInOutParam.m_dVbw * 1e3), true, in_sInOutParam.m_dSweepTime);
    if(s32RetVal)
    {
        return DP_ERR_SA_RBW_VBW;
    }

    return DP_SUCCESS;
}

/**
*\brief          This function is used to find the one time initialization for Sg and SA
*
*\param[in]  	 in_sInOutParam.m_dInputFreq  	 It specifies the input frequency
* *\param[in]  	 in_sInOutParam.m_u16StimulusPathId           It specifies the stimulus path ID
*\param[in]  	 in_sInOutParam.m_dCenterFreq           It specifies the center frequency
*\param[in]  	 in_sInOutParam.m_dSpan  	 It specifies the span
* *\param[in]  	 in_sInOutParam.m_dMarkerPos  	 It specifies the marker pos
*\param[in]  	 in_sInOutParam.m_u16MeasurementPathID  	 It specifies the measurement path id
*\param[out]  	 in_sInOutParam.m_dSGinPower  	 It specifies the siggen power
*\param[out]  	 in_sInOutParam.m_dRFCableloss  	 It specifies the RF cable loss
*\param[out]  	 in_sInOutParam.m_dMesured_Power  	 It specifies the Measured power
*\param[out]  	 in_sInOutParam.m_dCalculated_Power  	 It specifies the calculated power
* *\param[out]   in_sInOutParam.m_dMeasCableloss  	 It specifies the measured cable loss
*
*
*\retval	    ::DP_SUCCESS is returned upon success
*
*
*\author      Parthipan A
*\date        02 June 2023
*/
S32BIT CTestCaseWrapper::DP_SG_SA_Config_for_ChannelMeasurement(PS_SG_SA_INOUT_PARAMETERS inout_psInOutParam)
{
    S32BIT s32RetVal        = 0;
    FDOUBLE dSGinPower      = 0.0;
    FDOUBLE dMeasPwr        = 0.0;
    FDOUBLE dRFCableloss    = 0.0;
    FDOUBLE dSAoutPower     = 0.0;
    FDOUBLE dIFCableloss    = 0.0;

    // Apply Calib Data for RF Stimulas Path
    s32RetVal = ApplyCalibData(inout_psInOutParam->m_u16StimulusPathId, inout_psInOutParam->m_dInputFreq , inout_psInOutParam->m_dInputPower , &dSGinPower, &dRFCableloss);
    if(s32RetVal)
    {
        return DP_ERR_STIM_PATHID;
    }

    // Config Center Freq and Span in SA
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(inout_psInOutParam->m_u8SpecSel, (inout_psInOutParam->m_dCenterFreq * 1e6), (inout_psInOutParam->m_dSpan * 1e6));
    if(s32RetVal)
    {
        return DP_ERR_SA_SPAN_CF;
    }

    // Configure SigGen freq and power for RF
    s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigRF(inout_psInOutParam->m_u8SigGenSel, (inout_psInOutParam->m_dInputFreq * 1e6), dSGinPower);
    if(s32RetVal)
    {
        return DP_ERR_FREQ_POWER;
    }


    // Enable Rf out in RF SG
    s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(inout_psInOutParam->m_u8SigGenSel, true);
    if(s32RetVal)
    {
        return DP_ERR_RF_ENABLE_OUT;
    }

    // Enable continuous run
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(inout_psInOutParam->m_u8SpecSel, DP_ATE_7343_ENABLE);
    if(s32RetVal)
    {
        return DP_ERR_SA_ENA_CNT;
    }

    QThread::msleep(inout_psInOutParam->m_u64delay);

    // Disable continuous run
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(inout_psInOutParam->m_u8SpecSel, DP_ATE_7343_DISABLE);
    if(s32RetVal)
    {
        return DP_ERR_SA_DIS_CNT;
    }

    // Read MArker Frequency
    s32RetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(inout_psInOutParam->m_u8SpecSel, (inout_psInOutParam->m_dMarkerPos * 1e6), &dMeasPwr);
    if(s32RetVal)
    {
        return DP_ERR_SA_ACTIVE_MARKER;
    }

    // Disable RF Out in SG
    s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(inout_psInOutParam->m_u8SigGenSel, false);
    if(s32RetVal)
    {
        return DP_ERR_RF_DISABLE_OUT;
    }

    // Apply Calib DATA for SA
    s32RetVal = ApplyCalibData(inout_psInOutParam->m_u16MeasurementPathID, inout_psInOutParam->m_dCenterFreq, dMeasPwr, &dSAoutPower, &dIFCableloss);
    if(s32RetVal)
    {
        return DP_ERR_MEAS_PATHID;
    }

   inout_psInOutParam->m_dSGinPower =  dSGinPower;
   inout_psInOutParam->m_dRFCableloss = dRFCableloss;
   inout_psInOutParam->m_dMesured_Power =  dMeasPwr;
   inout_psInOutParam->m_dCalculated_Power =  dSAoutPower;
   inout_psInOutParam->m_dMeasCableloss =  dIFCableloss;

    return DP_SUCCESS;
}

S32BIT CTestCaseWrapper::DP_SA_Config_for_ChannelMeasurement(PS_SG_SA_INOUT_PARAMETERS inout_psInOutParam)
{
    S32BIT s32RetVal        = 0;
    FDOUBLE dMeasPwr        = 0.0;
    FDOUBLE dSAoutPower     = 0.0;
    FDOUBLE dIFCableloss    = 0.0;

    // Config Center Freq and Span in SA
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ConfigFreqCenterSpan(inout_psInOutParam->m_u8SpecSel, (inout_psInOutParam->m_dCenterFreq * 1e6), (inout_psInOutParam->m_dSpan * 1e6));
    if(s32RetVal)
    {
        return DP_ERR_SA_SPAN_CF;
    }

    // Enable continuous run
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(inout_psInOutParam->m_u8SpecSel, DP_ATE_7343_ENABLE);
    if(s32RetVal)
    {
        return DP_ERR_SA_ENA_CNT;
    }

    QThread::msleep(inout_psInOutParam->m_u64delay);

    // Disable continuous run
    s32RetVal = p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ContRun(inout_psInOutParam->m_u8SpecSel, DP_ATE_7343_DISABLE);
    if(s32RetVal)
    {
        return DP_ERR_SA_DIS_CNT;
    }

    // Read MArker Frequency
    s32RetVal =  p_objMainApp->m_objSAWrapper.DP_Enum_SpecAn_ReadActiveMarker(inout_psInOutParam->m_u8SpecSel, (inout_psInOutParam->m_dMarkerPos * 1e6), &dMeasPwr);
    if(s32RetVal)
    {
        return DP_ERR_SA_ACTIVE_MARKER;
    }


    // Apply Calib DATA for SA
    s32RetVal = ApplyCalibData(inout_psInOutParam->m_u16MeasurementPathID, inout_psInOutParam->m_dCenterFreq, dMeasPwr, &dSAoutPower, &dIFCableloss);
    if(s32RetVal)
    {
        return DP_ERR_MEAS_PATHID;
    }

    inout_psInOutParam->m_dMesured_Power    =  dMeasPwr;
    inout_psInOutParam->m_dCalculated_Power =  dSAoutPower;
    inout_psInOutParam->m_dMeasCableloss    =  dIFCableloss;

    return DP_SUCCESS;
}

S32BIT CTestCaseWrapper::DP_Exit(U8BIT in_u8SigGenSel)
{
    S32BIT s32RetVal        = 0;

    // Disable RF Out in SG
    s32RetVal = p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(in_u8SigGenSel, false);
    if(s32RetVal)
    {
        return DP_ERR_RF_DISABLE_OUT;
    }

    return DP_SUCCESS;
}

short  CTestCaseWrapper :: ApplyCalibData(unsigned int in_uiPathID, double in_dFrequency, double in_dUncalibPower, double *out_dCalibPower, double *out_dCableloss)
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

    strQuery.sprintf("SELECT * FROM calib_cabloss WHERE PathID = %d AND (RadioFreq BETWEEN %f AND %f)",in_uiPathID, in_dFrequency - 0.000001, in_dFrequency + 0.000001);

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
        strQuery.sprintf("SELECT * FROM calib_cabloss_rpt WHERE PathID = %d AND RadioFreq < %f ORDER BY RadioFreq DESC LIMIT 1", in_uiPathID, in_dFrequency);
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
        strQuery.sprintf("SELECT * FROM calib_cabloss_rpt WHERE PathID = %d AND RadioFreq > %f ORDER BY RadioFreq ASC LIMIT 1",in_uiPathID, in_dFrequency);
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

    *out_dCalibPower = in_dUncalibPower + dCableLoss;
    *out_dCableloss = dCableLoss;
    return 0;
}
