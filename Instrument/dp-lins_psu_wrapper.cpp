/*******************************************************************************
 File Name    : dp-lins_PSU_Wrapper.cpp
 Author       : Manikandan K
 Created Date : Mar 11,2019
 Description  : Test Thread function definitions.
*******************************************************************************/

/*******************************************************************************
 Company Name : DATA PATTERNS INDIA PRIVATE LIMITED
 Address      : No. H9,4th MainRoad,SIPCOT IT Park,Siruseri,Chennai-603103
 Email        : support@datapatterns.co.in
 Phone        : +91 44 47414000
 Fax          : +91 44 47414444
*******************************************************************************/

/******************************************************************************
 Copyright (c) 2019 DATA PATTERNS

 All rights reserved. These materials are confidential and proprietary to
 DATA PATTERNS and no part of these materials should be reproduced or published
 in any form by any means, electronic or mechanical, including photocopy, on any
 information storage or retrieval system, nor should the materials be disclosed
 to third parties without the explicit written authorization of DATA PATTERNS.

*******************************************************************************/


#include "dp-lins_psu_wrapper.h"
#include "QDebug"
#include "QThread"
//#include "/src/DP_UART_Wrapper.h"
#include <string.h>


CDP_PSU_Wrapper::CDP_PSU_Wrapper(unsigned char in_ucMode)
{
    m_ucIsDaisyChain = in_ucMode;
    m_fCurrentRating = 0.0;
    m_fVoltrating = 0.0;
}
/**************************************************************************
// Name						:  DP_GetAddress
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Get Device Address
**************************************************************************/
short CDP_PSU_Wrapper::DP_GetAddress(unsigned char *out_pucAddress)
{
    *out_pucAddress = m_ucAddress;

    return DP_PSU_SUCCESS;
}
/**************************************************************************
// Name						:  DP_GetDaisyChainMode
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Get Daisy Chain Mode
**************************************************************************/
short CDP_PSU_Wrapper::DP_GetDaisyChainMode(unsigned char *out_pucMode)
{
    *out_pucMode = m_ucIsDaisyChain;

    return DP_PSU_SUCCESS;
}
/**************************************************************************
// Name						:  ConfigCOM1Port
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  COM port Open
**************************************************************************/
short CDP_PSU_Wrapper::ConfigCOM1Port(QString in_qsComNo, unsigned int in_uiBaudRate, unsigned char in_ucStopBit, unsigned char in_ucDataBits, unsigned char in_ucParity)
{
    if(m_objqserialportUART.isOpen())
        m_objqserialportUART.close();

    //Set COM port name
    m_objqserialportUART.setPortName(in_qsComNo);

    //Set Baudrate
    m_objqserialportUART.setBaudRate(in_uiBaudRate);

    //Set Data Bits
    if(in_ucDataBits == QSerialPort::Data5)
    {
        m_objqserialportUART.setDataBits(QSerialPort::Data8);
    }
    else if(in_ucDataBits == QSerialPort::Data6)
    {
        m_objqserialportUART.setDataBits(QSerialPort::Data8);
    }
    else if(in_ucDataBits == QSerialPort::Data7)
    {
        m_objqserialportUART.setDataBits(QSerialPort::Data8);
    }
    else if(in_ucDataBits == QSerialPort::Data8)
    {
        m_objqserialportUART.setDataBits(QSerialPort::Data8);
    }
    else
    {
        m_objqserialportUART.setDataBits(QSerialPort::UnknownDataBits);
        return DP_PSU_INVALID_DATABITS;
    }

    //Set Parity Bit
    if(in_ucParity == QSerialPort::NoParity)
    {
        m_objqserialportUART.setParity(QSerialPort::NoParity);
    }
    else if(in_ucParity == QSerialPort::EvenParity)
    {
        m_objqserialportUART.setParity(QSerialPort::EvenParity);
    }
    else if(in_ucParity == QSerialPort::OddParity)
    {
        m_objqserialportUART.setParity(QSerialPort::OddParity);
    }
    else if(in_ucParity == QSerialPort::SpaceParity)
    {
        m_objqserialportUART.setParity(QSerialPort::SpaceParity);
    }
    else if(in_ucParity == QSerialPort::MarkParity)
    {
        m_objqserialportUART.setParity(QSerialPort::MarkParity);
    }
    else
    {
        m_objqserialportUART.setDataBits(QSerialPort::UnknownDataBits);
        return DP_PSU_INVALID_PARITY;
    }

    //Set stop bit
    if(in_ucStopBit == QSerialPort::OneStop)
    {
        m_objqserialportUART.setStopBits(QSerialPort::OneStop);
    }
    else if(in_ucStopBit == QSerialPort::OneAndHalfStop)
    {
        m_objqserialportUART.setStopBits(QSerialPort::OneAndHalfStop);
    }
    else if(in_ucStopBit == QSerialPort::TwoStop)
    {
        m_objqserialportUART.setStopBits(QSerialPort::TwoStop);
    }
    else
    {
        m_objqserialportUART.setStopBits(QSerialPort::UnknownStopBits);
        return DP_PSU_INVALID_DATABITS;
    }

    //Set flow control
    m_objqserialportUART.setFlowControl(QSerialPort::NoFlowControl);

    if(m_objqserialportUART.open(QIODevice::ReadWrite))
    {
//        qDebug()<<m_objqserialportUART.portName() + " Opened";
    }
    else
    {
//        qDebug()<<m_objqserialportUART.portName() + " Not Opened";
        return DP_PSU_COM_PORT_NOT_OPENED;
    }

    return DP_PSU_SUCCESS;
}

/**************************************************************************
// Name						:  DP_PSU_PowerRating
// Author					:  Manoj L
// Global Variables affected:  NIL
// Created Date				:  28-01-2021
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  To read the Power Supply details such as
                               Voltage and Current Ratings (*IDN query)
**************************************************************************/
short CDP_PSU_Wrapper:: DP_PSU_PowerRating(unsigned short in_usInstanceID, unsigned char in_ucPSUaddress, char* in_pzModelNum, float *out_Voltrating, float *out_CurrentRating)
{
    char szModelNum[15] = {0};
    short sRetVal = DP_PSU_ZERO;
    QString qsData ;
    int iLength = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};
    QStringList strTempList;
    QString strTemp;

    qsData.clear();
#if 1
    /*** Set Address for the Device ***/
    qsData = QString::asprintf("ADR %02d\r\n", in_ucPSUaddress);

    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = 3;

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));
    sRetVal = DP_PSU_ValidateACK(m_qbReadData);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }
    qsData.clear();
#endif
    /*** Identification Query ***/

    qsData = QString::asprintf("IDN?\r\n");

    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = 17;

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    strTemp = QString::asprintf("%s", arrucReadByte);
    strTempList = strTemp.split(",");
    qDebug()<<strTempList;
    strTemp = strTempList.at(1);

    if(strTemp.contains("GEN") == true)
    {
        strTemp = strTemp.mid(3, strTemp.length()-3);
        if(strTemp.contains("\r") == true)
        {
           strTemp.remove("\r");
        }
        strTempList = strTemp.split("-");
    }
    else if(strTemp.contains("ZUP") == true)
    {

    }
    else if(strTemp.contains("Z") == true)
    {

    }

    m_fVoltrating = strTempList.at(0).toFloat();
    m_fCurrentRating = strTempList.at(1).toFloat();

    sscanf((const char *)arrucReadByte,"%[^0-9]", szModelNum);
    if(strcmp(szModelNum, in_pzModelNum) != 0)
    {
        return DP_PSU_INVALID_SESSION_DEVICE;
    }

    /*** PSU RESET ***/
    sRetVal = DP_PSU_Reset(in_usInstanceID, DP_PSU_LOCAL_MODE, in_ucPSUaddress);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    qsData.clear();
    /*** Set Remote Mode ***/
    qsData = QString::asprintf("RMT 1\r");

    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = sizeof(arrucReadByte);
    memset(arrucReadByte,0,sizeof(arrucReadByte));

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    m_qbReadData.clear();
    m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));
    sRetVal = DP_PSU_ValidateACK(m_qbReadData);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    *out_Voltrating = m_fVoltrating;
    *out_CurrentRating  = m_fCurrentRating;

    return DP_PSU_SUCCESS;
}

/**************************************************************************
// Name						:  DP_PSU_Open
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Initiating communication with Power supply
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_Open(unsigned char in_ucPortNo, unsigned long in_ulBaudRate, unsigned char in_ucStopBit, unsigned char in_ucDataBits, unsigned char in_ucParity, unsigned short *out_usInstanceID)
{
    S32BIT s32RetVal = DP_PSU_ZERO;
    U16BIT u16PSMInstanceID = 0;
    SDP_APP_UART_CONFIG SUARTConfigData;

    memset(&SUARTConfigData, 0 , sizeof(SDP_APP_UART_CONFIG));

    SUARTConfigData.m_u8PortNo = in_ucPortNo;
    SUARTConfigData.m_u32BaudRate   = in_ulBaudRate;
    SUARTConfigData.m_s8DataSize    = in_ucDataBits;
    SUARTConfigData.m_s8Parity      = in_ucParity;
    SUARTConfigData.m_s8FlowControl = QSerialPort::NoFlowControl;
    SUARTConfigData.m_s8StopBit     = in_ucStopBit;

    s32RetVal = UART_Open(SUARTConfigData, &u16PSMInstanceID);
    if(s32RetVal != DP_PSU_SUCCESS)
    {
        return DP_PSU_COM_PORT_NOT_OPENED;
    }

    *out_usInstanceID = u16PSMInstanceID;

    return DP_PSU_SUCCESS;
}
/**************************************************************************
// Name						:  DP_PSU_Close
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Close the communication with Power supply
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_Close(unsigned short in_usInstanceID, unsigned char ucAddress)
{
    QString qsData = "";
    int iLength = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    sRetVal = DP_PSU_Reset(in_usInstanceID, DP_PSU_LOCAL_MODE, ucAddress);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }
    /*** Close ***/
    qsData = QString::asprintf("RMT 0\r");
    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = sizeof(arrucReadByte);

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    m_qbReadData.clear();
    m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

    sRetVal = DP_PSU_ValidateACK(m_qbReadData);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    sRetVal = UART_Close(in_usInstanceID);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    return DP_PSU_SUCCESS;
}
/**************************************************************************
// Name						:  DP_PSU_Reset
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Reset of the Power supply
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_Reset(unsigned short in_usInstanceID, unsigned char in_ucLocalorGlobalReset, unsigned char ucAddress)
{
    QString qsData = "";
    int iLength = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    if(in_ucLocalorGlobalReset == DP_PSU_GLOBAL_MODE)
    {
        qsData = QString::asprintf("GRST\r");

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));
        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

    }
    else
    {
        if(m_ucIsDaisyChain == DP_PSU_DAISY_CHAIN_MODE)
        {
            qsData.clear();

            /*** Address of the Device ***/
            qsData = QString::asprintf("ADR %02d\r",ucAddress);

            strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

            iLength = strlen(arrcWriteByte);

            sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }

            iLength = sizeof(arrucReadByte);

            sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }

            m_qbReadData.clear();
            m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));
            sRetVal = DP_PSU_ValidateACK(m_qbReadData);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }
        }

        qsData.clear();
        /*** Reset Command ***/
        qsData = QString::asprintf("RST\r");

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));
        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
    }

    return DP_PSU_SUCCESS;
}
/**************************************************************************
// Name						:  DP_PSU_OV_UV_Config
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Set Over and Under Voltage
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_OV_UV_Config(unsigned short in_usInstanceID, unsigned char ucAddress, unsigned char in_ucSeriesType, float in_fOverVolt, float in_fUnderVolt)
{
    QString qsData;
    int iLength = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    if(m_ucIsDaisyChain == DP_PSU_DAISY_CHAIN_MODE)
    {
        qsData.clear();

        /*** Address of the Device ***/
        qsData = QString::asprintf("ADR %02d\r",ucAddress);

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
    }

    if((in_fOverVolt < 0 )||  (in_fOverVolt >= (m_fVoltrating * 1.05)) ||  (in_fUnderVolt > in_fOverVolt) /*|| (in_fOverVolt <= (in_fUnderVolt * 1.1))*/) /*Commented Due to high Power, Uncomment for other Power*/
    {
        return DP_PSU_INVALID_OVER_UNDER_VOLT_INPUT;
    }

    /*** Over Voltage Set ***/
    if(in_fOverVolt > 0 && in_fOverVolt < 10)
    {
        qsData = QString::asprintf("OVP 0%2.2f\r", in_fOverVolt);
    }
    else
    {
        qsData = QString::asprintf("OVP %.2f\r", in_fOverVolt);
    }

    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = sizeof(arrucReadByte);

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    m_qbReadData.clear();
    m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

    sRetVal = DP_PSU_ValidateACK(m_qbReadData);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    /*** Under Voltage Set ***/
    qsData.clear();

    if(in_ucSeriesType == DP_PSU_Z_SERIES)
    {
        if(in_fUnderVolt > 0 && in_fUnderVolt < 10)
        {
            qsData = QString::asprintf("UVP 0%2.2f\r", in_fUnderVolt);
        }
        else
        {
            qsData = QString::asprintf("UVP %.2f\r", in_fUnderVolt);
        }
    }
    else/*GEN-Series**/
    {
        if(in_fUnderVolt > 0 && in_fUnderVolt < 10)
        {
            qsData = QString::asprintf("UVL 0%2.2f\r", in_fUnderVolt);
        }
        else
        {
            qsData = QString::asprintf("UVL %.2f\r", in_fUnderVolt);
        }
    }

    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = sizeof(arrucReadByte);

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }
    m_qbReadData.clear();
    m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

    sRetVal = DP_PSU_ValidateACK(m_qbReadData);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    return DP_PSU_SUCCESS;
}
/**************************************************************************
// Name						:  DP_PSU_VoltCurrConfig
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  set Voltage and current
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_VoltCurrConfig(unsigned short in_usInstanceID, unsigned char ucAddress, unsigned char in_ucLocalorGlobalConfig, float in_fvoltage, float in_fcurrent)
{
    QString qsData = "";
    int iLength = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    if(in_fvoltage > m_fVoltrating || in_fcurrent > m_fCurrentRating)
    {
        return DP_PSU_INVALID_VOLT_CURR_INPUT;
    }

    if(in_ucLocalorGlobalConfig == DP_PSU_GLOBAL_MODE)
    {
        if(in_fvoltage > 0 && in_fvoltage < 10)
        {
            qsData = QString::asprintf("GPV %2.2f\r", in_fvoltage);
        }
        else
        {
            qsData = QString::asprintf("GPV %.2f\r", in_fvoltage);
        }

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        qsData.clear();

        qsData = QString::asprintf("GPC %1.3f\r", in_fcurrent);

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

    }
    else
    {
        if(m_ucIsDaisyChain == DP_PSU_DAISY_CHAIN_MODE)
        {
            qsData.clear();

            /*** Address of the Device ***/
            qsData = QString::asprintf("ADR %02d\r",ucAddress);

            strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

            iLength = strlen(arrcWriteByte);

            sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }

            iLength = sizeof(arrucReadByte);

            sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }

            m_qbReadData.clear();
            m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

            sRetVal = DP_PSU_ValidateACK(m_qbReadData);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }
        }

        qsData.clear();

        if(in_fvoltage > 0 && in_fvoltage < 10)
        {
            qsData = QString::asprintf("PV %2.2f\r", in_fvoltage);
        }
        else
        {
            qsData = QString::asprintf("PV %.2f\r", in_fvoltage);
        }

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        qsData.clear();

        if(in_fcurrent < DP_PSU_MIN_CURRENT_VAL || in_fcurrent > DP_PSU_MAX_CURRENT_VAL)
            return DP_PSU_INVALID_CURRENT;

        qsData = QString::asprintf("PC %1.3f\r", in_fcurrent);

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
    }

    return DP_PSU_SUCCESS;
}


/**************************************************************************
// Name						:  DP_PSU_Readback_Config
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Read Back Configuration
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_Readback_Config(unsigned short in_usInstanceID, unsigned char ucAddress, float *out_pfVoltage, float *out_pfCurrent, float *out_pfUnderVoltage, float *out_pfOverVoltage)
{
    QString qsData;
    int iLength = 0;
    float fMeasVolt = 0;
    float fMeasCurr = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    if(out_pfVoltage == NULL || out_pfCurrent == NULL || out_pfUnderVoltage == NULL)
    {
        return DP_PSU_INVALID_PTR;
    }

    if(m_ucIsDaisyChain == DP_PSU_DAISY_CHAIN_MODE)
    {
        qsData.clear();

        /*** Address of the Device ***/
        qsData = QString::asprintf("ADR %02d\r",ucAddress);

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
    }

    qsData = QString::asprintf("DVC?\r");
    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = sizeof(arrucReadByte);

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    QByteArray databuf;
    databuf = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

    sscanf(databuf.data(),"%f,%f,%f,%f,%f,%f\r",&fMeasVolt,out_pfVoltage,&fMeasCurr,out_pfCurrent,out_pfUnderVoltage,out_pfOverVoltage);

    return DP_PSU_SUCCESS;
}
/**************************************************************************
// Name						:  DP_PSU_Output
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  OutPut ON/OFF
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_Output(unsigned short in_usInstanceID, unsigned char ucAddress, unsigned char in_ucLocalorGlobalOutput, unsigned char in_ucOnOff)
{
    QString qsData;
    int iLength = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    if(in_ucLocalorGlobalOutput == DP_PSU_GLOBAL_MODE)
    {
        qsData.clear();

        if(in_ucOnOff == DP_PSU_OUTPUT_ON)
        {
            qsData = QString::asprintf("GOUT 1\r");
        }
        else if(in_ucOnOff == DP_PSU_OUTPUT_OFF)
        {
            qsData = QString::asprintf("GOUT 0\r");
        }

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
    }
    else
    {
        if(m_ucIsDaisyChain == DP_PSU_DAISY_CHAIN_MODE)
        {
            qsData.clear();

            /*** Address of the Device ***/
            qsData = QString::asprintf("ADR %02d\r",ucAddress);

            strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

            iLength = strlen(arrcWriteByte);

            sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }

            iLength = sizeof(arrucReadByte);

            sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }
            m_qbReadData.clear();
            m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

            sRetVal = DP_PSU_ValidateACK(m_qbReadData);
            if(sRetVal != DP_PSU_SUCCESS)
            {
                return sRetVal;
            }
        }

        qsData.clear();

        if(in_ucOnOff == DP_PSU_OUTPUT_ON)
        {
            qsData = QString::asprintf("OUT 1\r");
        }
        else if(in_ucOnOff == DP_PSU_OUTPUT_OFF)
        {
            qsData = QString::asprintf("OUT 0\r");
        }

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
        m_qbReadData.clear();
        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        sRetVal = DP_PSU_ValidateACK(m_qbReadData);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }
    }

    return DP_PSU_SUCCESS;

}

/**************************************************************************
// Name						:  DP_PSU_Measure_Ouput
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Get Measured Output Voltage and current
**************************************************************************/

short CDP_PSU_Wrapper::DP_PSU_Measure_Ouput(unsigned short in_usInstanceID, unsigned char ucAddress, float *out_fVoltage, float *out_fCurrent)
{
    qint64 qi64Size = 0;
    QString qsData;
    int iLength = 0;
    float fSetVolt = 0;
    float fSetCurr = 0;
    float fOVP = 0;
    float fUVL = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    if(out_fVoltage == NULL || out_fCurrent == NULL)
    {
        return DP_PSU_INVALID_PTR;
    }

    qsData.clear();
    if(m_ucIsDaisyChain == DP_PSU_DAISY_CHAIN_MODE)
    {
        /*** Address of the Device ***/
        qsData = QString::asprintf("ADR %02d\r",ucAddress);

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        if(strcmp(m_qbReadData.data(),"OK\r") != 0)
        {
            return DP_PSU_ACK_NOT_RECEIVED;
        }

        m_qbReadData.clear();
        qsData.clear();
    }

    qsData = QString::asprintf("DVC?\r");

    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = sizeof(arrucReadByte);

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    QByteArray databuf;
    databuf = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

    sscanf(databuf.data(),"%f,%f,%f,%f,%f,%f\r",out_fVoltage,&fSetVolt,out_fCurrent,&fSetCurr,&fOVP,&fUVL);

//    qDebug("%f,%f,%f,%f,%f,%f\n", *out_fVoltage, fSetVolt, *out_fCurrent, fSetCurr, fOVP, fUVL);

    return DP_PSU_SUCCESS;
}

/**************************************************************************
// Name						:  DP_PSU_ReadStatus
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Read the status
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_ReadStatus(unsigned short in_usInstanceID, unsigned char ucAddress, S_DP_PSU_STATUS *out_pSReadStatusVal)
{
    qint64 qi64Size = 0;
    QString qsData;
    int iLength = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};


    if(out_pSReadStatusVal == NULL)
    {
        return DP_PSU_INVALID_PTR;
    }

    qsData.clear();

    if(m_ucIsDaisyChain == DP_PSU_DAISY_CHAIN_MODE)
    {
        /*** Address of the Device ***/
        qsData = QString::asprintf("ADR %02d\r",ucAddress);

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        if(strcmp(m_qbReadData.data(),"OK\r") != 0)
        {
            return DP_PSU_ACK_NOT_RECEIVED;
        }

        m_qbReadData.clear();
        qsData.clear();
    }

    qsData = QString::asprintf("STT?\r");

    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = sizeof(arrucReadByte);

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

    sscanf(m_qbReadData.data(),"MV(%f),PV(%f),MC(%f),PC(%f),SR(%hu),FR(%hu)\r",&out_pSReadStatusVal->fMeasuredVoltage,&out_pSReadStatusVal->fSetVoltage,&out_pSReadStatusVal->fMeasuredCurrent,&out_pSReadStatusVal->fSetCurrent,&out_pSReadStatusVal->usStatusRegister,&out_pSReadStatusVal->usFaultRegister);

    /*** ERROR CHECKING TO BE IMPLEMENTED ***/

    return DP_PSU_SUCCESS;

}
/**************************************************************************
// Name						:  DP_PSU_Command_Transmit
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Command Transmit
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_Command_Transmit(unsigned short in_usInstanceID, QString in_qsWriteData, int in_iDataSize,unsigned char ucAddress)
{
    QString qsData ;
    int iLength = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    if(m_ucIsDaisyChain == DP_PSU_DAISY_CHAIN_MODE)
    {
        /*** Address of the Device ***/
        qsData = QString::asprintf("ADR %02d\r",ucAddress);

        strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

        iLength = strlen(arrcWriteByte);

        sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        iLength = sizeof(arrucReadByte);

        sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
        if(sRetVal != DP_PSU_SUCCESS)
        {
            return sRetVal;
        }

        m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

        if(strcmp(m_qbReadData.data(),"OK\r") != 0)
        {
            return DP_PSU_ACK_NOT_RECEIVED;
        }
        m_qbReadData.clear();
        qsData.clear();
    }

    in_qsWriteData += "\r";

    strcpy(arrcWriteByte, (const char *)in_qsWriteData.toStdString().c_str());

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, in_iDataSize);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    return DP_PSU_SUCCESS;
}

/**************************************************************************
// Name						:  DP_PSU_Command_Transmit
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Command Transmit
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_Command_Reception(unsigned short in_usInstanceID, int in_iDataSizeToRead, QByteArray *out_pqbaWriteData)
{
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    if(out_pqbaWriteData == NULL)
    {
        return DP_PSU_INVALID_PTR;
    }

    sRetVal = UART_StreamRead(in_usInstanceID, in_iDataSizeToRead, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }
    *out_pqbaWriteData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

    return DP_PSU_SUCCESS;
}

/**************************************************************************
// Name						:  SetFOLDEnable
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Set Fold Enable or Disble for Protection
**************************************************************************/

short CDP_PSU_Wrapper:: SetFOLDEnable(unsigned short in_usInstanceID, bool in_bEnbDis)
{
    QString qsData = "";
    qint64 qi64Size = 0;
    int iLength = 0;
    short sRetVal = 0;
    unsigned int uiTimeout = 100;
    char arrcWriteByte[256] = {0};
    unsigned char arrucReadByte[DP_PSU_MAX_LEN_READ] = {0};

    qsData = QString::asprintf("FLD %d\r",in_bEnbDis);

    strcpy(arrcWriteByte, (const char *)qsData.toStdString().c_str());

    iLength = strlen(arrcWriteByte);

    sRetVal =  UART_StreamWrite(in_usInstanceID, (unsigned char *)arrcWriteByte, iLength);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    iLength = sizeof(arrucReadByte);

    sRetVal = UART_StreamRead(in_usInstanceID, iLength, uiTimeout, arrucReadByte);
    if(sRetVal != DP_PSU_SUCCESS)
    {
        return sRetVal;
    }

    m_qbReadData = QByteArray::fromRawData((const char *)arrucReadByte, sizeof(arrucReadByte));

    if(strcmp(m_qbReadData.data(),"OK\r") != 0)
    {
        return DP_PSU_ACK_NOT_RECEIVED;
    }
    m_qbReadData.clear();

    return DP_PSU_SUCCESS;
}
/**************************************************************************
// Name						:  DP_PSU_ErrorCodeMessage
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Read the status
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_ValidateACK(QByteArray qbaErrorCode)
{
    qDebug()<<qbaErrorCode.data();
    if(strcmp(qbaErrorCode.data(),"E01\r") == 0)
    {
        return DP_PSU_ERROR_E01;
    }
    else if(strcmp(qbaErrorCode.data(),"E02\r") == 0)
    {
        return DP_PSU_ERROR_E02;
    }
    else if(strcmp(qbaErrorCode.data(),"E04\r") == 0)
    {
        return DP_PSU_ERROR_E04;
    }
    else if(strcmp(qbaErrorCode.data(),"E06\r") == 0)
    {
        return DP_PSU_ERROR_E06;
    }
    else if(strcmp(qbaErrorCode.data(),"E07\r") == 0)
    {
        return DP_PSU_ERROR_E07;
    }
    else if(strcmp(qbaErrorCode.data(),"E08\r") == 0)
    {
        return DP_PSU_ERROR_E08;
    }
    else if(strcmp(qbaErrorCode.data(),"C01\r") == 0)
    {
        return DP_PSU_ERROR_C01;
    }
    else if(strcmp(qbaErrorCode.data(),"C02\r") == 0)
    {
        return DP_PSU_ERROR_C02;
    }
    else if(strcmp(qbaErrorCode.data(),"C03\r") == 0)
    {
        return DP_PSU_ERROR_C03;
    }
    else if(strcmp(qbaErrorCode.data(),"C04\r") == 0)
    {
        return DP_PSU_ERROR_C04;
    }
    else if(strcmp(qbaErrorCode.data(),"C05\r") == 0)
    {
        return DP_PSU_ERROR_C05;
    }
    else if(strcmp(m_qbReadData.data(),"OK\r") != 0)
    {
        return DP_PSU_ACK_NOT_RECEIVED;
    }
    else if(strcmp(m_qbReadData.data(),"OK\r") == 0)
    {
        return DP_PSU_SUCCESS;
    }

    return DP_PSU_SUCCESS;
}

/**************************************************************************
// Name						:  DP_PSU_GetErrorMessage
// Author					:  Manikandan K
// Global Variables affected:  NIL
// Created Date				:  17-01-2020
// Revision Date			:  NIL
// Reason for Revising		:  NIL
// Description				:  Error String return
**************************************************************************/
short CDP_PSU_Wrapper::DP_PSU_GetErrorMessage(short in_sErrorCode, QString *out_pqsErrMsg)
{
    switch(in_sErrorCode)
    {
    case DP_PSU_COM_PORT_NOT_OPENED:
    {
        *out_pqsErrMsg = QString::asprintf("Port Not Opened");
        break;
    }
    case DP_PSU_CMD_NOT_TRANSMITED:
    {
        *out_pqsErrMsg = QString::asprintf("Data Not Transmitted");
        break;
    }
    case DP_PSU_INVALID_BAUDRATE:
    {
        *out_pqsErrMsg = QString::asprintf("Invalid Baud Rate");
        break;
    }
    case DP_PSU_INVALID_DATABITS:
    {
        *out_pqsErrMsg = QString::asprintf("Invalid Data Bits");
        break;
    }
    case DP_PSU_INVALID_PARITY:
    {
        *out_pqsErrMsg = QString::asprintf("Invalid Parity");
        break;
    }
    case DP_PSU_INVALID_SESSION_DEVICE:
    {
        *out_pqsErrMsg = QString::asprintf("Invalid Device");
        break;
    }
    case DP_PSU_ACK_NOT_RECEIVED:
    {
        *out_pqsErrMsg = QString::asprintf("Acknowledgement not received");
        break;
    }
    case DP_PSU_INVALID_CURRENT:
    {
        *out_pqsErrMsg = QString::asprintf("Invalid Input Current");
        break;
    }
    case DP_PSU_INVALID_PTR:
    {
        *out_pqsErrMsg = QString::asprintf("Invalid Pointer");
        break;
    }
    case DP_PSU_INVALID_OVER_UNDER_VOLT_INPUT:
    {
        *out_pqsErrMsg = QString::asprintf("Invalid Over or Under Voltage");
        break;
    }
    case DP_PSU_INVALID_VOLT_CURR_INPUT:
    {
        *out_pqsErrMsg = QString::asprintf("Invalid Voltage or Current input");
        break;
    }
    case DP_PSU_ERROR_E01:
    {
        *out_pqsErrMsg = QString::asprintf("Program voltage (PV) is programmed above acceptable range");
        break;
    }
    case DP_PSU_ERROR_E02:
    {
        *out_pqsErrMsg = QString::asprintf(" programming output voltage below under voltage setting");
        break;
    }
    case DP_PSU_ERROR_E04:
    {
        *out_pqsErrMsg = QString::asprintf("Over voltage is programmed below acceptable range");
        break;
    }
    case DP_PSU_ERROR_E06:
    {
        *out_pqsErrMsg = QString::asprintf("Under Voltage value is programmed above the programmed output voltage");
        break;
    }
    case DP_PSU_ERROR_E07:
    {
        *out_pqsErrMsg = QString::asprintf("Programming the Output to ON during a latched fault shut down");
        break;
    }
    case DP_PSU_ERROR_E08:
    {
        *out_pqsErrMsg = QString::asprintf("Cannot execute command via Advanced Slave Parallel mode");
        break;
    }
    case DP_PSU_ERROR_C01:
    {
        *out_pqsErrMsg = QString::asprintf("Illegal command or query");
        break;
    }
    case DP_PSU_ERROR_C02:
    {
        *out_pqsErrMsg = QString::asprintf("Missing parameter");
        break;
    }
    case DP_PSU_ERROR_C03:
    {
        *out_pqsErrMsg = QString::asprintf("Illegal parameter");
        break;
    }
    case DP_PSU_ERROR_C04:
    {
        *out_pqsErrMsg = QString::asprintf("Checksum error");
        break;
    }
    case DP_PSU_ERROR_C05:
    {
        *out_pqsErrMsg = QString::asprintf("Setting out of range");
        break;
    }
    default:
    {
        *out_pqsErrMsg = QString::asprintf("Unknown Error");
        break;
    }
    }
    return DP_PSU_SUCCESS;
}
