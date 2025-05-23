#include "DMMWrapper.h"

extern void *g_vpHandle;
CDMMWrapper::CDMMWrapper()
{
    //ReadDMMInstrumentDetails();
}

S32BIT CDMMWrapper::DP_DMM_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize)
{
    if(out_ps8ErrStringBuf == NULL)
        return  DP_APP_INVALID_PARAM;

    switch(in_s32ErrCode)
    {
    case DP_APP_INVALID_HANDLE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid UART Handle");
        break;

    case DP_APP_INVALID_POINTER:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid pointer");
        break;

    case DP_APP_RS232_INVALID_PORT_NO:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid RS232 Port Number");
        break;

    case DP_APP_RS232_PORT_BUSY:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RS232 Port is Busy");
        break;

    case DP_APP_RS232_PORT_OPEN:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RS232 Port Open Failed");
        break;

    case DP_APP_RS232_PORT_CLOSE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RS232 Port Close Failed");
        break;

    case DP_APP_WRITE_FAIL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "UART Write Failed");
        break;

    case DP_APP_READ_FAIL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "UART Read Failed");
        break;

    case DP_DMM_INVALID_MEAS_TYPE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid DMM Measurement Type");
        break;

    case DP_DMM_INVALID_AUTORANGE_ENDIS:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid DMM Auto Range Enable/Disable");
        break;

    case DP_DMM_INVALID_AUTOTRGDELAY_ENDIS:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid DMM Auto Trigger Delay Enable/Disable");
        break;

    case DP_DMM_INVALID_TRIG_TYPE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid DMM Trigger Type");
        break;

    case DP_DMM_INVALID_TRGDELAY:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid DMM Trigger Delay");
        break;

    case DP_DMM_INVALID_TRG_SAMPLE_COUNT:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid DMM Trigger or sample count");
        break;

    case DP_DMM_OPEN_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Open Failed");
        break;

    case DP_DMM_INST_ID_MISMATCH:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Instrument ID Mismatched");
        break;

    case DP_DMM_CONFIG_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Configuration Failed");
        break;

    case DP_DMM_MEASURE_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Measurement Failed");
        break;

    case DP_DMM_CLOSE_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Close Failed");
        break;

    case DP_DMM_GETID_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Get ID Failed");
        break;

    case DP_DMM_SETTRIGGER_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Trigger Configuration Failed");
        break;

    case DP_DMM_RESET_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Reset Failed");
        break;

    case DP_DMM_EXTTRIG_CONFIG_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM External Trigger Configuration Failed");
        break;

    case DP_DMM_CLEAR_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM clear Failed");
        break;

    case DP_DMM_GET_CALIBDETAILS_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DMM Get Calibration details Failed");
        break;

    default:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid Error Code !");
        break;
    }

    return DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to open the DMM in remote mode.
*
*\param[in]		in_hHandle		It specifies the UART port handle which obtained from UART open function
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_OPEN_FAILED is returned upon open failed
*
*\par Pre-requisite functions and it's sequence:
*   -# ::UART_FindPorts( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
#if 0
S32BIT CDMMWrapper::DP_DMM_Open(U8BIT in_u8PortNo, SDP_APP_UART_CONFIG in_SRS232Config, DP_DRV_HANDLE out_phHandle)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[20] = {0};
    S_DMM_DETAILS sRdDMMDetails;
    QString qstrDMMID;

    qstrDMMID.clear();
    memset(&sRdDMMDetails, 0, sizeof(S_DMM_DETAILS));

    s32Retval = UART_Open(in_SRS232Config, &out_phHandle);
    if(s32Retval)
    {
        return s32Retval;
    }

    s32Retval = UART_ConfigurePort(g_vpHandle, in_SRS232Config);
    if(s32Retval)
    {
        UART_Port_Close(g_vpHandle);
        return s32Retval;
    }

    s32Retval = DP_DMM_ReadDetails(g_vpHandle, &sRdDMMDetails);
    if(s32Retval)
    {
        UART_Port_Close(g_vpHandle);
        return s32Retval;
    }

    qstrDMMID.append(sRdDMMDetails.m_szManufacturer);
    qstrDMMID.append(',');
    qstrDMMID.append(sRdDMMDetails.m_szModel);
    qstrDMMID.append(',');
    qstrDMMID.append(sRdDMMDetails.m_szSerialNo);
    qstrDMMID.append(',');
    qstrDMMID.append(sRdDMMDetails.m_szRevision);

    qDebug() << qstrDMMID;

    //Add validation for Instrument ID
    if(strcmp(s8DMMID, qstrDMMID.toStdString().c_str()) != 0)
    {
        UART_Port_Close(g_vpHandle);
        return DP_DMM_INST_ID_MISMATCH;
    }

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));
    sprintf(s8TxBuffer, "SYSTem:REMote\n");

    s32Retval = UART_Write(g_vpHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        UART_Port_Close(g_vpHandle);
        return DP_DMM_OPEN_FAILED;
    }

    return DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to configure the DMM.
*
*\param[in]		in_hHandle          It specifies the UART port handle which obtained from UART open function
*\param[in]     in_u8MeasType       It specifies the measurement type (0 - voltage, 1 - Resistance)
*\param[in]     in_u8AutoRangeEnDis It specifies the auto range enable/disable (0 - disable, 1 - Enable)
*\param[in]     in_fRange           It specifies the range
*\param[in]     in_fResolution      It specifies the resolution
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_CONFIG_FAILED is returned upon configuration failure
*
*\par Pre-requisite functions and it's sequence:
*   -# ::DP_DMM_Open( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
S32BIT CDMMWrapper::DP_DMM_Configure(DP_DRV_HANDLE in_hHandle, U8BIT in_u8MeasType, U8BIT in_u8AutoRangeEnDis, FSINGLE in_fRange, FSINGLE in_fResolution)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[48] = {0};

    DP_VALIDATE_HANDLE(in_hHandle);
    DP_DMM_VALIDATE_MEAS_TYPE(in_u8MeasType);
    DP_DMM_VALIDATE_AUTORANGE_ENDIS(in_u8AutoRangeEnDis);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));
    if(in_u8MeasType == 0)//DC voltage
    {
        if(in_u8AutoRangeEnDis == 0)//range disable
        {
            sprintf(s8TxBuffer, "CONFigure:VOLTage:DC %f,%f\n", in_fRange, in_fResolution);
            s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
            if(s32Retval)
            {
                return DP_DMM_CONFIG_FAILED;
            }
        }
        else//<function>:RANGe:AUTO {OFF|ON}
        {
            sprintf(s8TxBuffer, "CONFigure:VOLTage:DC \n");
            s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
            if(s32Retval)
            {
                return DP_DMM_CONFIG_FAILED;
            }

            sprintf(s8TxBuffer, "VOLTage:DC:RANGe:AUTO ON\n");
            s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
            if(s32Retval)
            {
                return DP_DMM_CONFIG_FAILED;
            }

            /*If auto range is enabled no need to configure the resolution*/
            /*sprintf(s8TxBuffer, "VOLTage:DC:RESolution %f\n", in_fResolution);
            s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
            if(s32Retval)
            {
                return DP_DMM_CONFIG_FAILED;
            }*/
        }
    }
    else//resistance
    {
        if(in_u8AutoRangeEnDis == 0)//range disable
        {
            sprintf(s8TxBuffer, "CONFigure:RESistance %f,%f\n", in_fRange, in_fResolution);
            s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
            if(s32Retval)
            {
                return DP_DMM_CONFIG_FAILED;
            }
        }
        else//<function>:RANGe:AUTO {OFF|ON}
        {
            sprintf(s8TxBuffer, "CONFigure:RESistance \n");
            s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
            if(s32Retval)
            {
                return DP_DMM_CONFIG_FAILED;
            }

            sprintf(s8TxBuffer, "RESistance:RANGe:AUTO ON\n");
            s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
            if(s32Retval)
            {
                return DP_DMM_CONFIG_FAILED;
            }

            /*If auto range is enabled no need to configure the resolution*/
            /*sprintf(s8TxBuffer, "RESistance:RESolution %f\n", in_fResolution);
            s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
            if(s32Retval)
            {
                return DP_DMM_CONFIG_FAILED;
            }*/
        }
    }

    return DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to measure voltage and resistance from the DMM.
*
*\param[in]		in_hHandle          It specifies the UART port handle which obtained from UART open function
*\param[in]     in_u32TimeoutinMS   It specifies the timeout in milliseconds
*\param[out]    out_pfMeasData      It specifies the measured data
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_MEASURE_FAILED is returned upon measurement failure
*
*\par Pre-requisite functions and it's sequence:
*   -# ::DP_DMM_Open( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
S32BIT CDMMWrapper::DP_DMM_Measure(DP_DRV_HANDLE in_hHandle, U32BIT in_u32TimeoutinMS, PFSINGLE out_pfMeasData)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[48] = {0};
    S8BIT s8RxBuffer[48] = {0};
    U32BIT u32ReadDataCnt = 0;

    DP_VALIDATE_HANDLE(in_hHandle);
    DP_VALIDATE_POINTER(out_pfMeasData);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));
    memset(s8RxBuffer, 0, sizeof(s8RxBuffer));

    sprintf(s8TxBuffer, "INITiate\n");
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_MEASURE_FAILED;
    }

    /*Add Delay to complete measurement*/
    Sleep(100);

    sprintf(s8TxBuffer, "FETCh?\n");
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_MEASURE_FAILED;
    }

    s32Retval = UART_Read(in_hHandle, s8RxBuffer, sizeof(s8RxBuffer), in_u32TimeoutinMS, &u32ReadDataCnt);
    if(s32Retval)
    {
        return DP_DMM_MEASURE_FAILED;
    }

    *out_pfMeasData = atof(s8RxBuffer);


//    qDebug() << "Read Data" << *out_pfMeasData << s8RxBuffer;
    return DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to close the DMM.
*
*\param[in]		in_hHandle		It specifies the UART port handle which obtained from UART open function
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_CLOSE_FAILED is returned upon close failure
*
*\par Pre-requisite functions and it's sequence:
*   -# ::DP_DMM_Open( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
S32BIT CDMMWrapper::DP_DMM_Close(DP_DRV_HANDLE in_hHandle)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[20] = {0};

    DP_VALIDATE_HANDLE(in_hHandle);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));
    sprintf(s8TxBuffer, "SYSTem:LOCal\n");

    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_CLOSE_FAILED;
    }

    s32Retval = UART_Port_Close(in_hHandle);
    if(s32Retval)
    {
        return s32Retval;
    }

    return DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to read the DMM ID.
*
*\param[in]		in_hHandle		It specifies the UART port handle which obtained from UART open function
*\param[out]	out_ps8DMMID	It specifies the read ID
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_GETID_FAILED is returned upon read ID failure
*
*\par Pre-requisite functions and it's sequence:
*   -# ::DP_DMM_Open( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
S32BIT CDMMWrapper::DP_DMM_ReadDetails(DP_DRV_HANDLE in_hHandle, PS_DMM_DETAILS out_psDMMDetails)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[20] = {0};
    S8BIT s8RxBuffer[35] = {0};
    U32BIT u32Timeout = 0;
    U32BIT u32ReadDataCnt = 0;
    QString strDMMID = "";

    strDMMID.clear();
    DP_VALIDATE_HANDLE(in_hHandle);
    DP_VALIDATE_POINTER(out_psDMMDetails);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));
    memset(s8RxBuffer, 0, sizeof(s8RxBuffer));

    sprintf(s8TxBuffer, "*IDN?\n");
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_GETID_FAILED;
    }

    u32Timeout = 500;
    s32Retval = UART_Read(in_hHandle, s8RxBuffer, sizeof(s8RxBuffer), u32Timeout, &u32ReadDataCnt);
    if(s32Retval)
    {
        return DP_DMM_GETID_FAILED;
    }

    strDMMID = QString::asprintf("%s", s8RxBuffer);
    qDebug() << strDMMID;

    strDMMID.chop(2);

    // Split the QString into a QStringList
    QStringList objqstrList = strDMMID.split(',');  // ',' is the delimiter

    // Output each part for demonstration
    for (const QString &part : objqstrList)
    {
        qDebug() << part;
    }

    strcpy(out_psDMMDetails->m_szManufacturer, objqstrList.at(0).toStdString().c_str());
    strcpy(out_psDMMDetails->m_szModel, objqstrList.at(1).toStdString().c_str());
    strcpy(out_psDMMDetails->m_szSerialNo, objqstrList.at(2).toStdString().c_str());
    strcpy(out_psDMMDetails->m_szRevision, objqstrList.at(3).toStdString().c_str());

    return DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to set trigger in the DMM.
*
*\param[in]		in_hHandle		    It specifies the UART port handle which obtained from UART open function
*\param[in]	    in_u8TriggerType	It specifies the trigger type(0-Bus, 1-External, 2- Immediate)
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_SETTRIGGER_FAILED is returned upon set trigger failure
*
*\par Pre-requisite functions and it's sequence:
*   -# ::DP_DMM_Open( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
S32BIT CDMMWrapper::DP_DMM_ConfigureTrigger(DP_DRV_HANDLE in_hHandle, U8BIT in_u8TriggerType)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[40] = {0};

    DP_VALIDATE_HANDLE(in_hHandle);
    DP_DMM_VALIDATE_TRIG_TYPE(in_u8TriggerType);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));

    if(in_u8TriggerType == 0)
    {
        sprintf(s8TxBuffer, "TRIGger:SOURce BUS\n");
    }
    else if(in_u8TriggerType == 1)
    {
        sprintf(s8TxBuffer, "TRIGger:SOURce EXTernal\n");
    }
    else
    {
        sprintf(s8TxBuffer, "TRIGger:SOURce IMMediate\n");
    }

    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_SETTRIGGER_FAILED;
    }

    return  DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to configure external trigger parameters in the DMM.
*
*\param[in]		in_hHandle              It specifies the UART port handle which obtained from UART open function
*\param[in]     in_u8AutoRangeEnDis     It specifies the auto trigger delay enable/disable (0 - disable, 1 - Enable)
*\param[in]     in_fTrigDelay           It specifies the trigger delay in seconds (Min - 0 to Max - 3600 seconds)
*\param[in]     in_u32TrigCount         It specifies the trigger count (Min - 1 to Max - 50,000)
*\param[in]     in_u32SampleCnt         It specifies the trigger sample count (Min - 1 to Max - 50,000)
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_EXTTRIG_CONFIG_FAILED is returned upon external trigger configuration failure
*
*\par Pre-requisite functions and it's sequence:
*   -# ::DP_DMM_Open( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
S32BIT CDMMWrapper::DP_DMM_ExtTriggerConfig(DP_DRV_HANDLE in_hHandle, U8BIT in_u8AutoTrgDelayEnDis, FSINGLE in_fTrigDelay, U32BIT in_u32TrigCount, U32BIT in_u32SampleCnt)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[40] = {0};

    DP_VALIDATE_HANDLE(in_hHandle);
    DP_DMM_VALIDATE_AUTOTRIGDELAY_ENDIS(in_u8AutoTrgDelayEnDis);
    DP_DMM_VALIDATE_TRIG_DELAY(in_fTrigDelay);
    DP_DMM_VALIDATE_TRIG_SAMPLE_COUNT(in_u32TrigCount);
    DP_DMM_VALIDATE_TRIG_SAMPLE_COUNT(in_u32SampleCnt);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));

    if(in_u8AutoTrgDelayEnDis == 0)//Disable
    {
        sprintf(s8TxBuffer, "TRIGger:DELay %f", in_fTrigDelay);
    }
    else
    {
        sprintf(s8TxBuffer, "TRIGger:DELay:AUTO ON\n");
    }
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_EXTTRIG_CONFIG_FAILED;
    }

    /*TRIGger:COUNt {<value>|MINimum|MAXimum|INFinite}*/
    sprintf(s8TxBuffer, "TRIGger:COUNt %d\n", in_u32TrigCount);
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_EXTTRIG_CONFIG_FAILED;
    }

    /*SAMPle:COUNt {<value>|MINimum|MAXimum}*/
    sprintf(s8TxBuffer, "SAMPle:COUNt %d\n", in_u32SampleCnt);
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_EXTTRIG_CONFIG_FAILED;
    }

    return DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to reset the DMM.
*
*\param[in]		in_hHandle              It specifies the UART port handle which obtained from UART open function
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_RESET_FAILED is returned upon DMM reset failure
*
*\par Pre-requisite functions and it's sequence:
*   -# ::DP_DMM_Open( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
S32BIT CDMMWrapper::DP_DMM_Reset(DP_DRV_HANDLE in_hHandle)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[20] = {0};

    DP_VALIDATE_HANDLE(in_hHandle);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));

    sprintf(s8TxBuffer, "*RST\n");
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_RESET_FAILED;
    }

    return DP_DMM_SUCCESS;
}

/**
*\brief     	This function is used to clear the output buffer of the DMM.
*
*\param[in]		in_hHandle              It specifies the UART port handle which obtained from UART open function
*
*\retval	 	::DP_DMM_SUCCESS is returned upon success
*\retval	 	::DP_DMM_CLEAR_FAILED is returned upon DMM clear failure
*
*\par Pre-requisite functions and it's sequence:
*   -# ::DP_DMM_Open( )
*
*\author     Kruthika T
*\date       17 April 2024
*/
S32BIT CDMMWrapper::DP_DMM_Clear(DP_DRV_HANDLE in_hHandle)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[20] = {0};

    DP_VALIDATE_HANDLE(in_hHandle);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));

    sprintf(s8TxBuffer, "*CLS\n");
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_CLEAR_FAILED;
    }

    return DP_DMM_SUCCESS;
}


void CDMMWrapper::ReadDMMInstrumentDetails()
{
    S8BIT szSecKey[128]		= {0};
    QString strTemp = "";
    QString strTempPath             = "";
    QString strID = "";
    QString strFilePath             = "";
    QSettings *ptrInstDetailsFile = NULL;

    memset(s8DMMID, 0, sizeof(s8DMMID));

    strTempPath = qApp->applicationDirPath();
    strFilePath = strTempPath + "/Config/" + "DMMInstDetails.ini";
    QFileInfo objfileInfo(strFilePath);
    if (objfileInfo.exists() && objfileInfo.isFile())
    {
        ptrInstDetailsFile = new(QSettings)(strFilePath, QSettings::IniFormat);
    }
    else
    {
        m_bInitErrFlag = 1;
        return;
    }

    sprintf(szSecKey, "INST_CONFIG/DMM_ID");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        delete ptrInstDetailsFile;
        return;
    }


    strID.append(strTemp.toStdString().c_str());
    strID.append(',');
    sprintf(szSecKey, "INST_CONFIG/DMM_MODEL");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        delete ptrInstDetailsFile;
        return;
    }

    strID.append(strTemp.toStdString().c_str());
    strID.append(',');
    sprintf(szSecKey, "INST_CONFIG/DMM_SERIALNO");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        delete ptrInstDetailsFile;
        return;
    }


    strID.append(strTemp.toStdString().c_str());
    strID.append(',');
    sprintf(szSecKey, "INST_CONFIG/DMM_REVISION");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        delete ptrInstDetailsFile;
        return;
    }

    strID.append(strTemp.toStdString().c_str());
//    qDebug() << strID;
    strcpy(s8DMMID, strID.toStdString().c_str());

    m_bInitErrFlag = 0;
    delete ptrInstDetailsFile;
    return;
}


S32BIT CDMMWrapper::DP_DMM_ReadCalibDetails(DP_DRV_HANDLE in_hHandle, PS8BIT out_ps8CalibDetails)
{
    S32BIT s32Retval = 0;
    S8BIT s8TxBuffer[20] = {0};
    S8BIT s8RxBuffer[35] = {0};
    U32BIT u32Timeout = 0;
    U32BIT u32ReadDataCnt = 0;

    DP_VALIDATE_HANDLE(in_hHandle);
    DP_VALIDATE_POINTER(out_ps8CalibDetails);

    memset(s8TxBuffer, 0, sizeof(s8TxBuffer));
    memset(s8RxBuffer, 0, sizeof(s8RxBuffer));

    sprintf(s8TxBuffer, "*IDN?\n");
    s32Retval = UART_Write(in_hHandle, s8TxBuffer, strlen(s8TxBuffer));
    if(s32Retval)
    {
        return DP_DMM_GET_CALIBDETAILS_FAILED;
    }

    u32Timeout = 500;
    s32Retval = UART_Read(in_hHandle, s8RxBuffer, sizeof(s8RxBuffer), u32Timeout, &u32ReadDataCnt);
    if(s32Retval)
    {
        return DP_DMM_GET_CALIBDETAILS_FAILED;
    }

    memcpy(out_ps8CalibDetails, s8RxBuffer, u32ReadDataCnt);
    return DP_DMM_SUCCESS;
}
#endif
