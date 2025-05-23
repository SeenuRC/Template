#include "DSOwrapper.h"
#include "include/dp_types.h"
#include "visa.h"
#include "visatype.h"
#include <float.h>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <windows.h>
#include <QApplication>
#include <QStringList>




CDSO_Wrapper::CDSO_Wrapper()
{
    vistatus = 0;

    memset(m_arrDSOInstHandle, 0, sizeof(m_arrDSOInstHandle));

    ReadDSOInstrumentDetails();
}

/**
*\brief			This function is used to get Oscilloscope Instrument Details from INI File.
*
*\retval		::DP_DSO_FILE_OPEN_FAILURE  is returned if the file open failure.
*\retval		::DP_DSO_CNT_READ_FAILURE   is returned if the Oscilloscope count read failure.
*\retval		::DP_DSO_MANUFAUCHER_READ_FAILURE   is returned if the Oscilloscope Manufaucher read failure.
*\retval		::DP_DSO_MODEL_READ_FAILURE   is returned if the Oscilloscope Model Id read failure.
*\retval		::DP_DSO_INS_ID_READ_FAILURE   is returned if the Oscilloscope Instrument Id read failure.
*\retval		::DP_DSO_SUCCESS is returned if the get Oscilloscope Instrument Details Success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::ReadDSOInstrumentDetails()
{
    S8BIT szSecKey[128]		= {0};
    QString strTemp = "";
    QString strTempPath             = "";
    QString strFilePath             = "";
    QSettings *ptrInstDetailsFile = NULL;

    strTempPath = qApp->applicationDirPath();

    strFilePath = strTempPath + "/RFInstDetails.ini";
    QFileInfo objfileInfo(strFilePath);
    if (objfileInfo.exists() && objfileInfo.isFile())
    {
        ptrInstDetailsFile = new(QSettings)(strFilePath, QSettings::IniFormat);
    }
    else
    {
        return DP_DSO_FILE_OPEN_FAILURE;
    }

    sprintf(szSecKey , "INST_CONFIG/TOT_DSO_COUNT");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_DSO_CNT_READ_FAILURE;
    }

    m_u8AvailDSOCount = strTemp.toUInt(0,10);

    for(U8BIT u8Idx = 0; u8Idx < m_u8AvailDSOCount; u8Idx++)
    {
        sprintf(szSecKey, "INST_CONFIG/DSO_%d_MANUFACTURER", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_DSO_MANUFAUCHER_READ_FAILURE;
        }

        strcpy(m_InstDSOCfg[u8Idx].szManufacturer, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/DSO_%d_MODEL", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_DSO_MODEL_READ_FAILURE;
        }

        strcpy(m_InstDSOCfg[u8Idx].szModel, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/DSO_%d_ID", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_DSO_INS_ID_READ_FAILURE;
        }

        m_InstDSOCfg[u8Idx].usInstID = strTemp.toUInt(0,10);
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to get Oscilloscope Instrument Id.
*
*\param[in] 	in_pcInstDetails                It specifies the value of the Instrument Details.
*\param[out] 	out_pusModel                    It specifies the retrieved value of the Oscilloscope Instrument Id.
*\retval		::DP_DSO_GET_INS_ID_FAILURE     is returned if the Get Instrument Id read failure.
*\retval		::DP_DSO_SUCCESS is returned    if the get Oscilloscope Instrument Id Success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::GetInstID(PS8BIT in_pcInstDetails, U16BIT *out_pusModel)
{
    U8BIT ucIdx = 0;
    bool bCmpRtn = 0;
    char *pszModel = NULL;

    pszModel = strtok(in_pcInstDetails, ",");
    pszModel = strtok(NULL, " ,");

    for(ucIdx = 0; ucIdx < m_u8AvailDSOCount;  ucIdx++)
    {
        bCmpRtn = strcasecmp(pszModel, m_InstDSOCfg[ucIdx].szModel);

        if (bCmpRtn == 0)
        {
            *out_pusModel = m_InstDSOCfg[ucIdx].usInstID;
            return DP_DSO_SUCCESS;
        }
    }

    return DP_DSO_GET_INS_ID_FAILURE;
}

/**
*\brief			This function is used to get the error message.
*
*\param[in] 	in_s32ErrCode                   It specifies the value of the Error code.
*\param[in] 	in_u16BufSize                   It specifies the value of the size of the error message.
*\param[out] 	out_ps8ErrStringBuf             It specifies the retrieved value of the error message.
*\retval		::DP_DSO_SUCCESS                is returned if the get the error message success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize)
{
    if(out_ps8ErrStringBuf == NULL)
        return  DP_DSO_INVALID_PARAM;

    switch(in_s32ErrCode)
    {
    case DP_VI_DSO_OPEN_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize, "ViOpen Failed");
        break;

    case DP_VI_DSO_CLOSE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"ViClose Failed!!");
        break;

    case DP_DSO_RESET_FAILED:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Reset Failed!!");
        break;

    case DP_DSO_INVALID_PARAM:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Frequency Start Failed !!");
        break;

    case DP_DSO_INVALID_INS_ID:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid Instrument Id");
        break;

    case DP_DSO_INVALID_MEAS_SEL:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid Measurement Selection");
        break;

    case DP_DSO_FILE_OPEN_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"File Open Failure");
        break;

    case DP_DSO_CNT_READ_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"DSO Instrument Count Read Failed");
        break;

    case DP_DSO_MODEL_READ_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"DSO Model Read Failed");
        break;

    case DP_DSO_MANUFAUCHER_READ_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"DSO Manufaucher Read failed");
        break;

    case DP_DSO_INS_ID_READ_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"DSO Instrument Id Read Failed");
        break;

    case DP_DSO_GET_INS_ID_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Get Instrument ID failed");
        break;

    case DP_DSO_SIGLE_RUN_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"DSO Single Run failed");
        break;

    case DP_ERR_DSO_INVALID_PARAM:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"DSO Invalied Parameter");
        break;

    case DP_DSO_CHAN_CONFIG_FAILED:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Channel Failed");
        break;

    case DP_DSO_SET_MEASUREMENT_TYPE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Set Measurement type Failed");
        break;

    case DP_DSO_SET_CHAN_SKEW_VAL_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Set Channel SKEW value Failed");
        break;

    case DP_DSO_CONFIG_ACQ_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Acquisition Mode Failed");
        break;

    case DP_DSO_SET_SAMP_RATE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Set Sampling Rate Failed");
        break;

    case DP_DSO_READ_PHASE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Phase Value Failed");
        break;

    case DP_DSO_READ_PEAK_VAL_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Peak Value Failed");
        break;

    case DP_DSO_READ_PULSE_WIDTH_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Pulse width Failed");
        break;

    case DP_DSO_MEASUREMENT_VAL_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Measurement Failed");
        break;

    case DP_DSO_MEAS_STATISTICS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Measure Statistics Failed");
        break;

    case DP_DSO_SET_TRIG_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Set Trigger Mode Failed");
        break;

    case DP_DSO_SET_TRIG_SRC_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Set Trigger Source Failed");
        break;

    case DP_DSO_SET_TRIG_TYPE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Set Trigger type Failed");
        break;

    case DP_DSO_CHAN_NO_INVALID:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"DSO Channel Number Invalid");
        break;

    case DP_DSO_INVALID_PROBE_VAL:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid Probe attentuation value");
        break;

    default:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid Error Code !");
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Open the Oscilloscope.
*
*\param[in] 	in_ucDSONo                      It specifies the value of the Oscilloscope number.
*\param[in] 	pLogicalName                    It specifies the value of the Oscilloscope Logical name.
*\retval		::DP_VI_DSO_OPEN_FAILURE        is returned if the Oscilloscope Open failure.
*\retval		::DP_DSO_GET_INS_ID_FAILURE     is returned if the get Instrument Id failure.
*\retval		::DP_DSO_INVALID_INS_ID         is returned if the Instrument Id is invalid.
*\retval		::DP_DSO_SUCCESS                is returned if the Oscilloscope Open success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_Open(U8BIT in_ucDSONo, PS8BIT pLogicalName)
{
    ViSession defaultDSORM = 0;
    ViSession viDSOSession = 0;
    U16BIT usInstID = 0;
    unsigned int uiReadCnt = 0;
    char szInstDetails [256] = {0};

    try
    {
        vistatus =  viOpenDefaultRM(&defaultDSORM);
        if(vistatus != 0)
        {
            return DP_VI_DSO_OPEN_FAILURE;
        }

        vistatus = viOpen(defaultDSORM, pLogicalName, VI_NULL, VI_NULL, &viDSOSession);
        if(vistatus != 0)
        {
            vistatus = viOpenDefaultRM(&defaultDSORM);
            if(vistatus != 0)
            {
                return DP_VI_DSO_OPEN_FAILURE;
            }

            vistatus = viOpen(defaultDSORM, pLogicalName, VI_NULL, VI_NULL, &viDSOSession);
            if(vistatus)
            {
                return DP_VI_DSO_OPEN_FAILURE;
            }
        }

        /*Read Instrument Type*/
        vistatus = viPrintf(viDSOSession, (char*)"*IDN?\n");
        if(vistatus)
        {
            return DP_DSO_GET_INS_ID_FAILURE;
        }

        vistatus = viBufRead(viDSOSession, (ViPBuf)szInstDetails, 256, (ViPUInt32)&uiReadCnt); // Read the Identification of Oscilloscope
        if(vistatus)
        {
            return DP_DSO_GET_INS_ID_FAILURE;
        }

        /*Read the Model ID*/
       vistatus = GetInstID(szInstDetails, &usInstID);

       if(vistatus != 0)
       {
           viClose(viDSOSession);

           return DP_DSO_INVALID_INS_ID;
       }
       else
       {
           m_arrDSOInstHandle[in_ucDSONo-1].m_viDSOSession = viDSOSession;
           m_arrDSOInstHandle[in_ucDSONo-1].m_usInstID = usInstID;
           m_arrDSOInstHandle[in_ucDSONo-1].m_u8Instance = in_ucDSONo;
       }
    }
    catch(...)
    {
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Close the Oscilloscope.
*
*\param[in] 	in_u8InstNo                      It specifies the value of the Instrument number.
*\retval		::DP_VI_DSO_CLOSE_FAILURE        is returned if the Oscilloscope Close failure.
*\retval		::DP_DSO_SUCCESS                is returned if the Oscilloscope Close success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_Close(U8BIT in_u8InstNo)
{
    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    vistatus = viClose(in_u8InstNo);  //Close the Oscilloscope
    if(vistatus)
    {
        return DP_VI_DSO_CLOSE_FAILURE;
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Preset the Oscilloscope.
*
*\param[in] 	in_u8InstNo                 It specifies the value of the Instrument number.
*\retval		::DP_DSO_RESET_FAILED       is returned if the Oscilloscope Preset failure.
*\retval		::DP_DSO_SUCCESS            is returned if the Oscilloscope Preset success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_Preset(U8BIT in_u8InstNo)
{
    ViChar viBuffer[256] = {0};

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    memset(viBuffer, 0, sizeof(viBuffer));

    strcpy(viBuffer, "*RST\n");  // Preset the Oscilloscope

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer);
    if(vistatus)
    {
        return DP_DSO_RESET_FAILED;
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Channel configuration.
*
*\param[in] 	in_u8InstNo                     It specifies the value of the Instrument number.
*\param[in] 	in_u8ChNo                       It specifies the value of the Channel number.
*\param[in] 	in_fVerScalePerDiv              It specifies the value of the Veritical Scale per Division.
*\param[in] 	in_dHoriScalePerDiv             It specifies the value of the Horizationtal Scale per Division.
*\param[in] 	in_fOffset                      It specifies the value of the Offset.
*\retval		::DP_DSO_CHAN_NO_INVALID        is returned if the Channel number is invalid.
*\retval		::DP_DSO_CHAN_CONFIG_FAILED     is returned if the channel number configuration failure.
*\retval		::DP_DSO_SUCCESS                is returned if the Channel configuration success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_ChannelConfig(U8BIT in_u8InstNo, U8BIT in_u8ChNo, FDOUBLE in_dVerScalePerDiv, FDOUBLE in_dHoriScalePerDiv, FDOUBLE in_dOffset, FDOUBLE in_dfProbeAttenValue)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer, 0, sizeof(viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChNo < DP_DSO_MIN_CHANNEL  || in_u8ChNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(in_dfProbeAttenValue < DP_DSO_MIN_PROBE_VAL  || in_dfProbeAttenValue > DP_DSO_MAX_PROBE_VAL)
    {
        return DP_DSO_INVALID_PROBE_VAL;
    }

    sprintf(viBuffer,"PROBe%d:SETup:ATTenuation:MANual %lf\n", in_u8ChNo, in_dfProbeAttenValue); //Probe Selection Cahennel Value  10000
    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_CHAN_CONFIG_FAILED;
    }

     //CHANnel<m>:STATe <State>  //Channel ON Or OFF
    sprintf(viBuffer,"CHAN%d:STATe ON\n", in_u8ChNo);
    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_CHAN_CONFIG_FAILED;
    }

    sprintf(viBuffer,"CHAN%d:SCAL %lf\n",in_u8ChNo, in_dVerScalePerDiv); // set the vertical scale per division
    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_CHAN_CONFIG_FAILED;
    }

    sprintf(viBuffer,":TIMebase:SCALe %lf\n", in_dHoriScalePerDiv); // set the Horizontal scale per division // It Can be Taken Second value

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_CHAN_CONFIG_FAILED;
    }

    sprintf(viBuffer,"CHANnel%d:OFFSet %lf\n", in_u8ChNo, in_dOffset); // set the Off set of the channel
    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_CHAN_CONFIG_FAILED;
    }
    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Acquisition type configuration.
*
*\param[in] 	in_u8InstNo                         It specifies the value of the Instrument number.
*\param[in] 	in_u8ChNo                           It specifies the value of the Channel number.
*\param[in] 	u8AcquisitionType                   It specifies the value of the Acquisition type.
*\param[in] 	in_dSamplingRate                    It specifies the value of the sampling rate.
*\retval		::DP_DSO_CHAN_NO_INVALID            is returned if the Channel number is invalid.
*\retval		::DP_DSO_CONFIG_ACQ_MODE_FAILURE    is returned if the Configure acquisition mode failure.
*\retval		::DP_DSO_SET_SAMP_RATE_FAILURE      is returned if the Set the Sampling rate failure.
*\retval		::DP_DSO_SUCCESS                    is returned if the Acquisition type configuration success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_ConfigureAcquisitionType(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8AcquisitionType, FDOUBLE in_dSamplingRate)
{
    ViChar viBuffer[256] = {0};
    QStringList qslAcqMode;
    QStringList qsl3004AcqMode;

    qslAcqMode.clear();
    qsl3004AcqMode.clear();

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    memset(viBuffer , 0 , sizeof (viBuffer));

    qslAcqMode << "ETIMe" << "RTIMe" << "PDETect" << "HRESolution" << "SEGMented" << "SEGPdetect" << "SEGHres";
    qsl3004AcqMode << "SAMPle" << "PDETect" << "HRESolution" << "ENVelope" << "AVERage";

    if(in_u8ChNo < DP_DSO_MIN_CHANNEL  || in_u8ChNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer, ":ACQuire:MODE %s\n", qslAcqMode.at(u8AcquisitionType).toStdString().c_str()); //Set the Acquisition Mode
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        if(u8AcquisitionType < 3)
        {
            sprintf(viBuffer, "CHANnel%d:TYPE  %s\n", in_u8ChNo, qsl3004AcqMode.at(u8AcquisitionType).toStdString().c_str());  //Set the channel Acquisition type
        }
        else
        {
            sprintf(viBuffer, "CHANnel%d:ARIThmetics  %s\n", in_u8ChNo, qsl3004AcqMode.at(u8AcquisitionType).toStdString().c_str()); //Set the channel Acquisition type
        }
    }

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer);
    if(vistatus)
    {
        return DP_DSO_CONFIG_ACQ_MODE_FAILURE;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer, ":CHANNEL%d:DISPLAY ON\n", in_u8ChNo);  //Enable the Channel Display State

        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer);
        if(vistatus)
        {
            return DP_DSO_CONFIG_ACQ_MODE_FAILURE;
        }
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer, ":ACQuire:SRATe:ANALog %lf\n", in_dSamplingRate);
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"ACQuire:POINts:VALue %lf\n", in_dSamplingRate);
    }

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer);  // Set the Sampling rate
    if(vistatus)
    {
        return DP_DSO_SET_SAMP_RATE_FAILURE;
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Acquisition record configuration.
*
*\param[in] 	in_u8InstNo                         It specifies the value of the Instrument number.
*\param[in] 	in_u32MinNumPts                     It specifies the value of the Number of points.
*\param[in] 	dAcqStartTime                       It specifies the value of the Acquisition Start time.
*\retval		::DP_DSO_CONFIG_ACQ_MODE_FAILURE    is returned if the Configure acquisition mode failure.
*\retval		::DP_DSO_SET_SAMP_RATE_FAILURE      is returned if the Set the Sampling rate failure.
*\retval		::DP_DSO_SUCCESS                    is returned if the Acquisition Record configuration success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_ConfigureAcquisitionRecord(U8BIT in_u8InstNo, U32BIT in_u32NumPts, FDOUBLE dAcqStartTime)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer, 0, sizeof(viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u32NumPts == DP_DSO_ZERO)
    {
        sprintf(viBuffer,"ACQuire:POINts:AUTO ON\n");  //Set Acquisition AUTO enable
    }
    else
    {
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
        {
            sprintf(viBuffer, ":ACQuire:POINts:ANALog %u\n", in_u32NumPts);  // Set the Number of Points in Acquisition
        }
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
        {
            sprintf(viBuffer, "ACQuire:MEMory:MODE MANual\n"); // Set the Acquisition manual mode

            vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer);
            if(vistatus)
            {
                return DP_DSO_CONFIG_ACQ_MODE_FAILURE;
            }

            sprintf(viBuffer, "TIMebase:ROLL:MTIMe %lf\n", dAcqStartTime); //Set the Acquisition Start time
            vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer);
            if(vistatus)
            {
                return DP_DSO_SET_SAMP_RATE_FAILURE;
            }

            sprintf(viBuffer, "ACQuire:POINts:VALue %u\n", in_u32NumPts); //Set the Number of Acquisition points
        }
    }

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer);
    if(vistatus)
    {
        return DP_DSO_SET_SAMP_RATE_FAILURE;
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Set Phase measurement type.
*
*\param[in] 	in_u8InstNo                             It specifies the value of the Instrument number.
*\param[in] 	in_u8RefchNo                            It specifies the value of the Reference Channel number.
*\param[in] 	in_u8OtherchNo                          It specifies the value of the Other Channel number.
*\param[in] 	in_fSkew                                It specifies the value of the Skew.
*\retval		::DP_DSO_CHAN_NO_INVALID                is returned if the Channel number is invalid.
*\retval		::DP_DSO_SET_CHAN_SKEW_VAL_FAILURE      is returned if the Set channel skew value failure.
*\retval		::DP_DSO_SET_MEASUREMENT_TYPE_FAILURE   is returned if the Set Measurement type failure.
*\retval		::DP_DSO_SUCCESS                       is returned if the Set Phase measurement type success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_SetPhaseMeasurementType(U8BIT in_u8InstNo, U8BIT in_u8MeasurementSel, U8BIT in_u8RefchNo, U8BIT in_u8OtherchNo, FSINGLE in_fSkew)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer, 0, sizeof(viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8RefchNo < DP_DSO_MIN_CHANNEL  || in_u8RefchNo > DP_DSO_MAX_CHANNEL || in_u8OtherchNo < DP_DSO_MIN_CHANNEL  || in_u8OtherchNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(in_u8MeasurementSel < DP_DSO_ONE || in_u8MeasurementSel > DP_DSO_EIGHT)
    {
        return DP_DSO_INVALID_MEAS_SEL;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"CHANnel%d:SKEW %f\n", in_u8RefchNo, in_fSkew); // Set the Channel SKEW value
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer,":CALibrate:SKEW CHANnel%d,%f\n", in_u8RefchNo, in_fSkew);  // Set the Channel SKEW value
    }

    vistatus = viPrintf(in_u8InstNo, viBuffer);
    if(vistatus)
    {
        return DP_DSO_SET_CHAN_SKEW_VAL_FAILURE;
    }

    memset(viBuffer, 0, sizeof(viBuffer));

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer,":MEASure:PHASe CHANnel%d,CHANnel%d\n", in_u8RefchNo, in_u8OtherchNo); //Set the Measure Reference & Other Channel Number
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:ENABle ON", in_u8MeasurementSel);  //Enable the Measurement
        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
        }

        sprintf(viBuffer,"MEASurement%d:MAIN PHASE", in_u8MeasurementSel);  //Set the Phase Measurement

        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
        }

        sprintf(viBuffer,"MEASurement%d:SOURce RE%d,CH%d\n", in_u8MeasurementSel, in_u8RefchNo, in_u8OtherchNo); // Set the Source for Measurement
    }

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Read Phase Difference.
*
*\param[in] 	in_u8InstNo                          It specifies the value of the Instrument number.
*\param[in] 	in_u32NoofSamples                    It specifies the value of the Number of samples.
*\param[in] 	in_u32Delay                          It specifies the value of the Delay.
*\param[out] 	Out_dPhaseDiffVal                    It specifies the retrieved value of the Phase differnce value.
*\retval		::DP_DSO_READ_PHASE_FAILURE          is returned if the Read phase difference failure.
*\retval		::DP_DSO_SUCCESS                     is returned if the Read phase difference success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_Read_Phasediff(U8BIT in_u8InstNo, U8BIT u8MeasurementSel, U32BIT in_u32NoofSamples, U32BIT in_u32Delay, FDOUBLE *Out_dPhaseDiffVal)
{
    ViChar viBuffer[256] = {0};
    double dPhaseDiffVal = 0.0;
    double dAvgPhase = 0.0;
    U32BIT iLoop = 0;

    memset(viBuffer, 0, sizeof (viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    for(iLoop = 0; iLoop < in_u32NoofSamples; iLoop++)
    {
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
        {
            sprintf(viBuffer,":MEASure:PHASe?\n");  // Read the Phase
        }
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
        {
            sprintf(viBuffer,"MEASurement%d:RESult:ACTual? PHASE\n", u8MeasurementSel);  // Read the Phase
        }

        vistatus = viQueryf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer, "%lf", dPhaseDiffVal);
        if(vistatus)
        {
            return DP_DSO_READ_PHASE_FAILURE;
        }

        dAvgPhase += dPhaseDiffVal;

        Sleep(in_u32Delay);
    }

    dAvgPhase = (dAvgPhase/in_u32NoofSamples);

    *Out_dPhaseDiffVal = dAvgPhase;

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Read Phase Difference.
*
*\param[in] 	in_u8InstNo                          It specifies the value of the Instrument number.
*\param[out] 	out_sDSOMeasStats                    It specifies the retrieved value of the Statistics.
*\retval		::DP_DSO_MEAS_STATISTICS_FAILURE     is returned if the Read Statistics failure.
*\retval		::DP_DSO_SUCCESS                     is returned if the Read Statistics success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_Measure_Statistics(U8BIT in_u8InstNo, U8BIT u8MeasurementSel, PS_DP_DSO_MEASSTATISTICS out_sDSOMeasStats)
{
    ViChar viBuffer[256] = {0};
    char szResult[512];
    QString strMeasStat;
    QList<QString> qsListMeasStats;

    memset(szResult, 0, sizeof(szResult));

    memset(viBuffer, 0, sizeof(viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    sprintf(viBuffer,":MEASure:STATistics ON\n"); //Set Measurement Statistics ON
    vistatus = viPrintf(in_u8InstNo,viBuffer);
    if(vistatus)
    {
        return DP_DSO_MEAS_STATISTICS_FAILURE;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        memset(viBuffer, 0, sizeof(viBuffer));
        sprintf(viBuffer,":MEASure:SENDvalid ON\n");  //Enable The result state
        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_MEAS_STATISTICS_FAILURE;
        }
    }

    memset(viBuffer, 0, sizeof(viBuffer));

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer,"MEASure:RESults?\n"); //Read the Statistics
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:STATistics:VALue:ALL?\n", u8MeasurementSel); //Read the Statistics
    }
    vistatus = viQueryf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer, "%s", szResult);
    if(vistatus)
    {
        return DP_DSO_MEAS_STATISTICS_FAILURE;
    }

    strMeasStat = szResult;

    qsListMeasStats = strMeasStat.split(",");

    out_sDSOMeasStats->dCurrMeas = qsListMeasStats.at(1).toFloat();
    out_sDSOMeasStats->ucResultState = qsListMeasStats.at(2).toFloat();
    out_sDSOMeasStats->dMinPhase = qsListMeasStats.at(3).toFloat();
    out_sDSOMeasStats->dMaxphase = qsListMeasStats.at(4).toFloat();
    out_sDSOMeasStats->dMean = qsListMeasStats.at(5).toFloat();
    out_sDSOMeasStats->dStndDeviation = qsListMeasStats.at(6).toFloat();
    out_sDSOMeasStats->u32Count = qsListMeasStats.at(7).toInt();

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Run Control.
*
*\param[in] 	in_u8InstNo                          It specifies the value of the Instrument number.
*\retval		::DP_DSO_SIGLE_RUN_FAILURE           is returned if the Single run failure.
*\retval		::DP_DSO_SUCCESS                     is returned if the RUN Control success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_SCOPE_RunControl(U8BIT in_u8InstNo, U8BIT in_u8ConRunOrStop)
{
    ViChar viBuffer[256] = {0};

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    memset(viBuffer, 0, sizeof(viBuffer));

    if(in_u8ConRunOrStop == DP_DSO_CON_RUN_START)  //Continuious Run
    {
        sprintf(viBuffer,"RUN\n");
        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SIGLE_RUN_FAILURE;
        }
    }
    else
    {
        sprintf(viBuffer,"STOP\n");
        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SIGLE_RUN_FAILURE;
        }
    }

    return DP_DSO_SUCCESS;
}

S16BIT CDSO_Wrapper::DP_SCOPE_RunSingle(U8BIT in_u8InstNo)
{
    ViChar viBuffer[256] = {0};

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    sprintf(viBuffer,"SINGle\n");
    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_SIGLE_RUN_FAILURE;
    }

    return DP_DSO_SUCCESS;

}

/**
*\brief			This function is used to Set Peak measurement type.
*
*\param[in] 	in_u8InstNo                             It specifies the value of the Instrument number.
*\param[in] 	in_u8RefchNo                            It specifies the value of the Reference Channel number.
*\param[in] 	in_u8OtherchNo                          It specifies the value of the Other Channel number.
*\retval		::DP_DSO_CHAN_NO_INVALID                is returned if the Channel number is invalid.
*\retval		::DP_DSO_SET_MEASUREMENT_TYPE_FAILURE   is returned if the Set Peak Measurement type failure.
*\retval		::DP_DSO_SUCCESS                        is returned if the Set Peak measurement type success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_SetPeakMeasurementType(U8BIT in_u8InstNo, U8BIT in_u8MeasurementSel, U8BIT in_u8ChannelNo)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer, 0, sizeof(viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChannelNo < DP_DSO_MIN_CHANNEL  || in_u8ChannelNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(in_u8MeasurementSel < DP_DSO_ONE  || in_u8MeasurementSel > DP_DSO_EIGHT)
    {
        return DP_DSO_INVALID_MEAS_SEL;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer,":MEASure:VPP CHANnel%d\n", in_u8ChannelNo); //Set the Peak Measurement Source
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:ENABle ON", in_u8MeasurementSel); //Set the Measurement Enable ON
        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
        }

        sprintf(viBuffer,"MEASurement%d:MAIN PEAK", in_u8MeasurementSel); //Set the Peak Measurement

        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
        }

        sprintf(viBuffer,"MEASurement%d:SOURce CH%d\n", in_u8MeasurementSel, in_u8ChannelNo); //Set the Source for Peak Measurement
    }

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Read Phase Difference.
*
*\param[in] 	in_u8InstNo                          It specifies the value of the Instrument number.
*\param[in] 	in_u8ChNo                            It specifies the value of the channel number.
*\param[out] 	Out_dPhaseDiffVal                    It specifies the retrieved value of the Peak.
*\retval		::DP_DSO_READ_PEAK_VAL_FAILURE       is returned if the Read phase difference failure.
*\retval		::DP_DSO_SUCCESS                     is returned if the Read phase difference success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_Read_PeakValue(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8MeasurementSel, FDOUBLE *Out_fdPeakValue)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer , 0 , sizeof (viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChNo < DP_DSO_MIN_CHANNEL  || in_u8ChNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer,":MEASure:VPP? CHANnel%d\n", in_u8ChNo); // Read the Peak Value
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:RESult:ACTual? PEAK\n", u8MeasurementSel); // Read the Peak Value
    }

    vistatus = viQueryf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer, "%lf", Out_fdPeakValue);
    if(vistatus)
    {
        return DP_DSO_READ_PEAK_VAL_FAILURE;
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Set Pulse Width measurement type.
*
*\param[in] 	in_u8InstNo                             It specifies the value of the Instrument number.
*\param[in] 	in_u8ChannelNo                          It specifies the value of the Channel number.
*\retval		::DP_DSO_CHAN_NO_INVALID                is returned if the Channel number is invalid.
*\retval		::DP_DSO_SET_MEASUREMENT_TYPE_FAILURE   is returned if the Set Pulse width Measurement type failure.
*\retval		::DP_DSO_SUCCESS                        is returned if the Set Pulse width measurement type success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_SetPulseWidthMeasType(U8BIT in_u8InstNo, U8BIT in_u8ChannelNo, U8BIT in_u8MeasurementSel, U8BIT in_u8SetMeasType)
{
    ViChar viBuffer[256] = {0};
    QString qstrWidthtype = "";

    memset(viBuffer, 0, sizeof(viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChannelNo < DP_DSO_MIN_CHANNEL  || in_u8ChannelNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(in_u8MeasurementSel < DP_DSO_ONE  || in_u8MeasurementSel > DP_DSO_EIGHT)
    {
        return DP_DSO_INVALID_MEAS_SEL;
    }

    if(in_u8SetMeasType == DP_DSO_POS_PULUEWIDTH)
    {
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
        {
            qstrWidthtype = "PWIDth";
        }
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
        {
            qstrWidthtype = "PPWidth";
        }
    }
    else
    {
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
        {
            qstrWidthtype = "NWIDth";
        }
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
        {
            qstrWidthtype = "NPWidth";
        }
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer,":MEASure:%s CHANnel%d\n", qstrWidthtype.toStdString().c_str(), in_u8ChannelNo); //Set the Source for Pulse measurement
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:ENABle ON", in_u8MeasurementSel); // Set Measurement Enable ON
        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
        }

        sprintf(viBuffer,"MEASurement%d:MAIN %s", in_u8MeasurementSel, qstrWidthtype.toStdString().c_str()); // Set the Measurement Pulse Width

        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
        }

        sprintf(viBuffer,":MEASurement%d:SOURce CH%d\n", in_u8MeasurementSel, in_u8ChannelNo); // Set the Source for Pulse width measurement
    }

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
    }

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Read Pulse Width.
*
*\param[in] 	in_u8InstNo                          It specifies the value of the Instrument number.
*\param[in] 	in_u8ChanNo                          It specifies the value of Channel Number.
*\param[in] 	in_u8MeasType                        It specifies the value of Measurement type.
*\param[out] 	Out_fdPulseValue                     It specifies the retrieved value of the pulse width value.
*\retval		::DP_DSO_CHAN_NO_INVALID             is returned if the Invalid channel number.
*\retval		::DP_DSO_READ_PULSE_WIDTH_FAILURE    is returned if the Read pulse width failure.
*\retval		::DP_DSO_SUCCESS                     is returned if the Read phase difference success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_Read_PulseWidth(U8BIT in_u8InstNo, U8BIT in_u8ChanNo, U8BIT u8MeasurementSel, U8BIT in_u8MeasType, FDOUBLE *Out_fdPulseWidthValue)
{
    ViChar viBuffer[256] = {0};
    QString qstrWidthtype = "";

    memset(viBuffer , 0 , sizeof (viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChanNo < DP_DSO_MIN_CHANNEL  || in_u8ChanNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(in_u8MeasType == DP_DSO_POS_PULUEWIDTH)
    {
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
        {
            qstrWidthtype = "PWIDth";
        }
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
        {
            qstrWidthtype = "PPWidth";
        }
    }
    else
    {
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
        {
            qstrWidthtype = "NWIDth";
        }
        if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
        {
            qstrWidthtype = "NPWidth";
        }
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer,":MEASure:%s? CHANnel%d\n", qstrWidthtype.toStdString().c_str(), in_u8ChanNo);  // Read the Pulse width
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:RESult:ACTual? %s\n", u8MeasurementSel, qstrWidthtype.toStdString().c_str()); // Read the Pulse width
    }

    vistatus = viQueryf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer, "%lf", Out_fdPulseWidthValue);
    if(vistatus)
    {
        return DP_DSO_READ_PULSE_WIDTH_FAILURE;
    }

    *Out_fdPulseWidthValue = *Out_fdPulseWidthValue * 1000; // Convert the Seconds to milliseconds

    return DP_DSO_SUCCESS;
}

/**
*\brief			This function is used to Set Trigger mode & Level.
*
*\param[in] 	in_u8InstNo                             It specifies the value of the Instrument number.
*\param[in] 	in_u8ChannelNo                          It specifies the value of the Channel number.
*\param[in] 	in_u8Mode                               It specifies the value of the Trigger Mode.
*\param[in] 	u8TriggerSrc                            It specifies the value of the trigger source.
*\param[in] 	u8TriggerType                           It specifies the value of the Trigger type.
*\param[in] 	in_dTriggerLevel                        It specifies the value of the Trigger level.
*\retval		::DP_DSO_CHAN_NO_INVALID                is returned if the Channel number is invalid.
*\retval		::DP_DSO_SET_TRIG_MODE_FAILURE          is returned if the Set Peak Measurement type failure.
*\retval		::DP_DSO_SET_TRIG_SRC_FAILURE           is returned if the Set Peak Measurement type failure.
*\retval		::DP_DSO_SET_TRIG_TYPE_FAILURE          is returned if the Set Peak Measurement type failure.
*\retval		::DP_DSO_SUCCESS                        is returned if the Set Peak measurement type success.

*\author
*\date
*/
S16BIT CDSO_Wrapper::DP_Scope_SetTriggerModeLevel(U8BIT in_u8InstNo, U8BIT in_u8ChannelNo, U8BIT in_u8Mode, U8BIT in_u8TriggerSrc, U8BIT in_u8TriggerType, FDOUBLE in_dTriggerLevel, U8BIT in_u8EdgeCoupling)
{
   ViChar viBuffer[256] = {0};

   QStringList qslTgrMode;
   QStringList qslTgrSrc;
   QStringList qslTgrTypeSel;
   QStringList qslTgrCoupling;

   qslTgrMode.clear();
   qslTgrSrc.clear();
   qslTgrTypeSel.clear();

   if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
   {
       return DP_DSO_INVALID_INS_NO;
   }

   if(in_u8ChannelNo < DP_DSO_ONE || in_u8ChannelNo > DP_DSO_FIVE)  //in_u8ChannelNo - 5 is External Trigger in RS_RTM3004
   {
       return DP_DSO_CHAN_NO_INVALID;
   }

   memset(viBuffer , 0 , sizeof (viBuffer));

   qslTgrMode << "EDGE" << "PWIDth" << "RUNT" << "PATTern" << "STATe" << "TIMeout" << "GLITch" << "DELay" << "SEQuence" << "SHOLd" << "WINDow" << "ADVanced" << "TV" << "COMM" << "TRANsition";
   qslTgrSrc << "CH1" << "CH2" << "CH3" << "CH4" << "EXTernanalog" << "LINE";
   qslTgrTypeSel << "EDGE" << "WIDTh" << "RISetime" << "LOGic" << "LINE" << "TV" << "RUNT";
   qslTgrCoupling << "DC" << "AC" << "LFReject";

   if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
   {
       if(in_u8ChannelNo > DP_DSO_FOUR)
       {
           return DP_DSO_CHAN_NO_INVALID;
       }

       sprintf(viBuffer,":TRIGger:AND:ENABle ON\n"); //Set Trigger Enable ON

       vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
       if(vistatus)
       {
           return DP_DSO_SET_TRIG_MODE_FAILURE;
       }
   }

   if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
   {
       sprintf(viBuffer,":TRIGger:MODE %s\n", qslTgrMode.at(in_u8Mode).toStdString().c_str());  // Set the Trigger mode
   }
   if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
   {
       if(in_u8Mode == 0)
       {
           sprintf(viBuffer,"TRIGger:A:MODE AUTO\n");// Set the Trigger mode AUTO
       }
       else
       {
           sprintf(viBuffer,"TRIGger:A:MODE NORMal\n"); // Set the Trigger Mode NORMAL
       }
   }

   vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
   if(vistatus)
   {
       return DP_DSO_SET_TRIG_MODE_FAILURE;
   }

   if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
   {
       sprintf(viBuffer,":TRIGger:AND:SOURce CHANnel%d\n", in_u8ChannelNo);
   }
   if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
   {
       sprintf(viBuffer,"TRIGger:A:SOURce %s\n", qslTgrSrc.at(in_u8TriggerSrc).toStdString().c_str()); //Set the Trigger source
   }

   vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);  //Set the Trigger source
   if(vistatus)
   {
       return DP_DSO_SET_TRIG_SRC_FAILURE;
   }

   if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
   {
       sprintf(viBuffer,"TRIGger:A:TYPE %s\n", qslTgrTypeSel.at(in_u8TriggerType).toStdString().c_str()); // Set the Trigger Type
       vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
       if(vistatus)
       {
           return DP_DSO_SET_TRIG_TYPE_FAILURE;
       }
   }

   if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
   {
       sprintf(viBuffer,":TRIGger:LEVel CHANnel%d, %.0lf\n", in_u8ChannelNo, in_dTriggerLevel);
   }
   if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
   {
       sprintf(viBuffer,"TRIGger:A:LEVel%d:VALue %.0lf\n", in_u8ChannelNo, in_dTriggerLevel);
   }
   vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);  // Set the Trigger level
   if(vistatus)
   {
       return DP_DSO_SET_TRIG_TYPE_FAILURE;
   }

   if(in_u8TriggerType == 0)  //Only using EDGE Type //Coupling Are DC ,AC, LFReject
   {
       sprintf(viBuffer,"TRIGger:A:EDGE:COUPling %s\n", qslTgrCoupling.at(in_u8EdgeCoupling).toStdString().c_str());
       vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
       if(vistatus)
       {
           return DP_DSO_SET_TRIG_TYPE_FAILURE;
       }
   }

   return DP_DSO_SUCCESS;
}

S16BIT CDSO_Wrapper::DP_Scope_SetTimePeriodMeasType(U8BIT in_u8InstNo, U8BIT in_u8ChannelNo, U8BIT in_u8MeasurementSel)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer, 0, sizeof(viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChannelNo < DP_DSO_MIN_CHANNEL  || in_u8ChannelNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        //sprintf(viBuffer,":MEASure:%s CHANnel%d\n", qstrWidthtype.toStdString().c_str(), in_u8ChannelNo);
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:ENABle ON", in_u8MeasurementSel); // Set Measurement Enable ON
        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
        }

        sprintf(viBuffer,"MEASurement%d:MAIN PERiod", in_u8MeasurementSel); // Set the Measurement Time Period

        vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
        if(vistatus)
        {
            return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
        }

        sprintf(viBuffer,":MEASurement%d:SOURce CH%d\n", in_u8MeasurementSel, in_u8ChannelNo); // Set the Source for Pulse width measurement
    }

    vistatus = viPrintf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession,viBuffer);
    if(vistatus)
    {
        return DP_DSO_SET_MEASUREMENT_TYPE_FAILURE;
    }

    return DP_DSO_SUCCESS;
}


S16BIT CDSO_Wrapper::DP_Scope_Read_TimePeriod(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8MeasurementSel, FDOUBLE *Out_fdTimePeriodValue)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer , 0 , sizeof (viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChNo < DP_DSO_MIN_CHANNEL  || in_u8ChNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == KS_DSOX2012A)
    {
        sprintf(viBuffer,":MEASure:VPP? CHANnel%d\n", in_u8ChNo);
    }
    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:RESult:ACTual? PERiod\n", u8MeasurementSel);
    }

    vistatus = viQueryf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer, "%lf", Out_fdTimePeriodValue); // Read the
    if(vistatus)
    {
        return DP_DSO_READ_PEAK_VAL_FAILURE;
    }

    *Out_fdTimePeriodValue = *Out_fdTimePeriodValue * 1000; // Convert the Seconds to milliseconds

    return DP_DSO_SUCCESS;
}

S16BIT CDSO_Wrapper::DP_Scope_Read_RMSAmplitudeValue(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8MeasurementSel, FDOUBLE *Out_fdPeakValue)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer , 0 , sizeof (viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChNo < DP_DSO_MIN_CHANNEL  || in_u8ChNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:RESult:ACTual? RMS\n", u8MeasurementSel); // Read the Mean Value
    }

    vistatus = viQueryf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer, "%lf", Out_fdPeakValue);
    if(vistatus)
    {
        return DP_DSO_READ_PEAK_VAL_FAILURE;
    }

    return DP_DSO_SUCCESS;
}

S16BIT CDSO_Wrapper::DP_Scope_Read_TopLevelAmplitude(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8MeasurementSel, FDOUBLE *Out_fdPeakValue)
{
    ViChar viBuffer[256] = {0};

    memset(viBuffer , 0 , sizeof (viBuffer));

    if(in_u8InstNo < DP_DSO_ONE || in_u8InstNo > DP_DSO_SIXTEEN)
    {
        return DP_DSO_INVALID_INS_NO;
    }

    if(in_u8ChNo < DP_DSO_MIN_CHANNEL  || in_u8ChNo > DP_DSO_MAX_CHANNEL)
    {
        return DP_DSO_CHAN_NO_INVALID;
    }

    if(m_arrDSOInstHandle[in_u8InstNo - 1].m_usInstID == RS_RTM3004)
    {
        sprintf(viBuffer,"MEASurement%d:RESult:ACTual? HIGH\n", u8MeasurementSel); // Read the top level Value
    }

    vistatus = viQueryf(m_arrDSOInstHandle[in_u8InstNo - 1].m_viDSOSession, viBuffer, "%lf", Out_fdPeakValue);
    if(vistatus)
    {
        return DP_DSO_READ_PEAK_VAL_FAILURE;
    }

    return DP_DSO_SUCCESS;
}
