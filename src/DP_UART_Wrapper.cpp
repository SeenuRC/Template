#include "include/dp_types.h"
#include "DP_UART_Wrapper.h"
#include <QSerialPortInfo>
#include <QVector>
#include "DP_datastructure.h"
#include "DP_APP_macros.h"
#include <windows.h>
#include <qdebug.h>

#define Q_OS_WINDOWS
#include <winsock2.h>
#include <wchar.h>


DP_DRV_HANDLE m_hHandle[256] = {NULL};
S32BIT m_s32hHandle[256] = {0};

void QPC_mDelay(unsigned int in_uiDelayinMs);
void QPC_uDelay(unsigned int in_uiDelayinUs);

#ifndef Q_OS_WINDOWS
static struct speedmap
{
    char *str; /*!< Baudrate */
    speed_t val; /*!< Line Speed */
} speedmap[] = {
{ (char *)"4800", B4800 }

, {(char *) "9600", B9600 }

  , { (char *)"19200", B19200 }

    , { (char *)"38400", B38400 }

      , { (char *)"57600", B57600 }

        , {(char *) "115200", B115200 }

          , {(char *) "921600", B921600 }

            , {(char *) "1000000", B1000000 }

              };

static struct termios g_Stios;
static int g_inspeeds = sizeof speedmap / sizeof speedmap[0];

#endif


/*********************** UART Communication API's *******************************/
/**
*\brief          This function is used to find the UART ports.
*
*\param[out]     out_pu8AvailPorts			 It specifies the pointer to get whether UART port available or not
*\param[out]     out_pu16DeviceID	         It specifies the pointer to hold the read Device ID
*\param[in]  	 out_pu8AvailablePorts  	 It specifies the pointer to hold the Available ports
*
*\retval	    ::DP_SUCCESS is returned upon success
*
*\par          Pre-requisite functions and it's sequence:
*              This function does not have any pre requisite function and can be called at any time needed
*
*\author      ANU R
*\date        02 June 2023
*/

S32BIT STDCALL UART_FindPorts(PU8BIT out_pu8AvailPorts, PU8BIT out_pu8AvailablePorts[])
{
    S32BIT s32PortCnt = 0;
    S32BIT s32Idx     = 0;
    QList<QSerialPortInfo> m_qliPort;

    /* Identifying available port counts */
    m_qliPort = QSerialPortInfo::availablePorts();
    s32PortCnt = (U8BIT )m_qliPort.count();

    for(s32Idx = 0; s32Idx < s32PortCnt; s32Idx++)
    {
        /* Copy available port names to output array */
        out_pu8AvailablePorts[s32Idx] = (PU8BIT) malloc(DP_APP_PORTNAME_LEN_MAX);
        strncpy((char *)out_pu8AvailablePorts[s32Idx], m_qliPort.at(s32Idx).portName().toStdString().c_str(), DP_APP_PORTNAME_LEN_MAX);
    }

    /* Copy available port counts to output pointer */
    *out_pu8AvailPorts = s32PortCnt;

    return DP_SUCCESS;
}
/**
*\brief			This function is used to internally to configure the opened UART port.
*
*\param[in] 	in_u16InstanceID	It specifies the UART port Instance.
*\param[in] 	in_SRS232Config 	It specifies the structure for UART configuration details
*
*\retval	 	::DP_APP_VALIDATE_INSTANCE is returned if the Instance ID is not available
*\retval	 	::DP_APP_VALIDATE_PORTCOUNT is returned if the Port No is not available
*\retval	 	::DP_APP_VALIDATE_BAUDRATE is returned if the Baurd rate is not available
*\retval	 	::DP_APP_VALIDATE_DATABITS is returned if the Data bits is not available
*\retval	 	::DP_APP_VALIDATE_PARITY is returned if the Parity is not available
*\retval	 	::DP_APP_VALIDATE_FLOWCONTROL is returned if the Flowcontrol is not available
*\retval	 	::DP_APP_VALIDATE_STOPBITS is returned if the stopBits is not available
*\retval		::DP_SUCCESS is returned upon success
*
* *\par Pre-requisite functions and it's sequence:
*   -# ::UART_FindPorts( )
*
*\author     ANU R
*\date       02 June 2023
*/

S32BIT STDCALL UART_Open(SDP_APP_UART_CONFIG in_SRS232Config, PU16BIT out_pu16InstanceID)
{
    S32BIT s32RetVal = DP_APP_ZERO;

    s32RetVal = UART_PORT_Open(in_SRS232Config.m_u8PortNo, out_pu16InstanceID);
    if(s32RetVal != DP_SUCCESS)
    {
        return s32RetVal;
    }
    else
    {
        s32RetVal = UART_ConfigurePort(*out_pu16InstanceID, in_SRS232Config);
        if(s32RetVal != DP_SUCCESS)
        {
            return s32RetVal;
        }
    }
    return DP_SUCCESS;
}


S32BIT STDCALL UART_StreamBuffClear(U16BIT in_u16InstanceID)
{
#ifdef Q_OS_WINDOWS
    PurgeComm(m_hHandle[in_u16InstanceID-1], PURGE_TXCLEAR);
    PurgeComm(m_hHandle[in_u16InstanceID-1], PURGE_RXCLEAR);
#endif
    return DP_SUCCESS;
}


/* WINDOWS STANDARD UART COMMUNICATION*/
S32BIT STDCALL UART_PORT_Open(U8BIT in_u8PortNo, PU16BIT out_pu16InstanceID)
{
    S32BIT s32RetVal = DP_SUCCESS;
    char strPortName1[10] = {0};
    wchar_t strPortName[10] = {0};
    unsigned char ucInstanceID = 0;
    QString qsdeviceName = "";
    char carrPortName[20] = {0};

    QList<QSerialPortInfo>  qliPort;
    qliPort = QSerialPortInfo::availablePorts();

    // DPRDR8122_VALIDATE_POINTER(out_phHandle);
    // No need to validate in_strPortName. If it is incorrect then DPRDR8122_UART_Open will return failure.
#ifdef Q_OS_WINDOWS
//    strcpy(strPortName1, (const char*)qliPort.at(in_u8PortNo).portName().toStdString().c_str());
    sprintf(strPortName1, "//./%s",(const char*)qliPort.at(in_u8PortNo).portName().toStdString().c_str());

    std::mbstowcs(strPortName, strPortName1, 10);
#else
    strcpy(carrPortName, (const char*)qliPort.at(in_u8PortNo).portName().toStdString().c_str());
    qsdeviceName = QString::fromLatin1(carrPortName);
    qsdeviceName.sprintf("/dev/%s",qsdeviceName.toStdString().c_str());
#endif
    ucInstanceID++;

#ifdef Q_OS_WINDOWS
    m_hHandle[in_u8PortNo] = CreateFile(strPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
#else
    m_s32hHandle[ucInstanceID-1] = open((const char*)qsdeviceName.toStdString().c_str(), O_RDWR|O_NDELAY);
#endif

#ifdef Q_OS_WINDOWS
    if(m_hHandle[in_u8PortNo] == INVALID_HANDLE_VALUE)
    {
        if(GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            s32RetVal = DP_APP_RS232_INVALID_PORT_NO;
        }
        if(GetLastError() == ERROR_ACCESS_DENIED)
        {
            s32RetVal = DP_APP_RS232_PORT_BUSY;
        }
        s32RetVal = DP_APP_RS232_PORT_OPEN;
    }
#else
    /* Open the device */
    if(m_s32hHandle[ucInstanceID-1] < 0)
    {
        printf("Error No : %d\n", errno);
        perror("UART Open : ");
        //        return DP_APP_UART_ERR_OPEN;
    }
#endif
    *out_pu16InstanceID = in_u8PortNo + 1;

    return s32RetVal;
    return DP_SUCCESS;
}


S32BIT STDCALL UART_ConfigurePort(U16BIT in_u16InstanceID, SDP_APP_UART_CONFIG in_SRS232Config)
{
    S32BIT s32RetVal = 0;
    U8BIT ucErrFlag = 0;
#ifdef Q_OS_WINDOWS
    DCB PortDCB;
#endif
    int32_t sRetVal = DP_SUCCESS;
    QString qsBaudrateVal = "";
    QByteArray qbarrBaudrate;
    // DPRDR8122_VALIDATE_HANDLE(in_hHandle);

    // No need to validate input parameters because these are hard-coded values.
    // Initialize the DCBlength member
#ifdef Q_OS_WINDOWS
    PortDCB.DCBlength = sizeof(DCB);
    s32RetVal = GetCommState(m_hHandle[in_u16InstanceID-1], &PortDCB);
    DP_LIB_RETURN_ON_ERR(s32RetVal, ucErrFlag);

    if(ucErrFlag != 1)
    {
        PortDCB.BaudRate		= in_SRS232Config.m_u32BaudRate;
        PortDCB.ByteSize		= in_SRS232Config.m_s8DataSize;;
        PortDCB.Parity			= in_SRS232Config.m_s8Parity;
        PortDCB.StopBits		= in_SRS232Config.m_s8StopBit;
        PortDCB.fBinary			= TRUE;
        PortDCB.fDsrSensitivity = false;
        PortDCB.fParity			= FALSE;
        PortDCB.fOutX			= false;
        PortDCB.fInX			= false;
        PortDCB.fNull			= false;
        PortDCB.fAbortOnError	= FALSE;
        PortDCB.fOutxCtsFlow	= FALSE;
        PortDCB.fOutxDsrFlow	= false;
        PortDCB.fDtrControl		= 0;
        PortDCB.fRtsControl		= 0;
        PortDCB.fTXContinueOnXoff = 1;
        PortDCB.EofChar			= 0;
        PortDCB.fErrorChar      = 1;
        PortDCB.ErrorChar		= 63;
        PortDCB.XonLim			= 2048;
        PortDCB.XoffLim			= 512;
        PortDCB.XonChar			= 17;
        PortDCB.XoffChar		= 19;
        PortDCB.EvtChar			= 0;
        PortDCB.wReserved		= 0;
        PortDCB.fDummy2			= 0;
        PortDCB.wReserved1		= 0;
        PortDCB.DCBlength 		= sizeof(DCB);

        s32RetVal = SetCommState(m_hHandle[in_u16InstanceID-1], &PortDCB);
        DP_LIB_RETURN_ON_ERR(s32RetVal, ucErrFlag);
        if(ucErrFlag != 1)
        {
            if((s32RetVal = UART_SetCommunicationTimeouts(in_u16InstanceID, \
                                                                    DP_APP_UART_RDINTERVAlTIMEOUT, DP_APP_UART_RDTOTAlTIMEOUTMULTIPLIER, DP_APP_UART_RDTOTAlTIMEOUTCONST, \
                                                                    DP_APP_UART_WRTOTAlTIMEOUTMULTIPLIER, DP_APP_UART_WRTOTAlTIMEOUTCONST)))
                DP_LIB_RETURN_ON_ERR(s32RetVal, ucErrFlag);
        }
        SetupComm(m_hHandle[in_u16InstanceID-1], 70000, 10000 );//Related to FIFO(Software FIFO)
    }
    /* If port is ready return success; else throw an error */
    return DP_SUCCESS;

#else

    // char_t *pcdevice = DP_APP_PTR_INIT;
    speed_t speed;

    /* Validating input parameters */
    if(in_SRS232Config.m_s32EnaParity > DP_APP_UART_ENA_PARITY)
    {
        return DP_APP_ERR_INVALID_PARITY;
    }

    if(in_SRS232Config.m_s32EnaParity)
    {
        if(in_SRS232Config.m_s8Parity > DP_APP_UART_ODD_PARITY)
        {
            return DP_APP_ERR_INVALID_PARITY;
        }
    }

    if((in_SRS232Config.m_s8StopBit < DP_APP_UART_ONE_STOPBIT) || (in_SRS232Config.m_s8StopBit > DP_APP_UART_TWO_STOPBIT))
    {
        return DP_APP_ERR_INVALID_STOPBITS;
    }
    if(in_SRS232Config.m_u32BaudRate == NULL)
    {
        return DP_APP_ERR_INVALID_BAUDRATE;
    }
    qsBaudrateVal = QString::number(in_SRS232Config.m_u32BaudRate);
    qbarrBaudrate = qsBaudrateVal.toUtf8();
    const char * pcbaudrate = qbarrBaudrate.constData();
    speed = DP_APP_UART_MapSpeed((char*)pcbaudrate);
    if(speed == DP_APP_ERR_INVALID_BAUDRATE)
        return DP_APP_ERR_INVALID_BAUDRATE;

    //speed = B921600;  //921600
    printf("baud rate : %d\n", speed);
    /* Set file status flags to zero */
    if(fcntl(m_s32hHandle[in_u16InstanceID-1], F_SETFL, 0) < 0)
        return DP_APP_UART_ERR_FILE_CTRL;

    /* Get the parameter associated with the object referenced by fd */
    if(tcgetattr(m_s32hHandle[in_u16InstanceID-1], &g_Stios) < 0)
    {
        perror("Failed in uart : ");
        printf("ErrNo : %d", errno);
        return DP_APP_UART_ERR_GET_ATTR;
    }

    /* Set the input line speed */
    if (cfsetispeed(&g_Stios, speed) < 0)
        return DP_APP_UART_ERR_SET_INPUT_SPEED;

    /* Set the output line speed */
    if (cfsetospeed(&g_Stios, speed) < 0)
        return DP_APP_UART_ERR_SET_OUTPUT_SPEED;

    g_Stios.c_cflag |= ( CLOCAL | CREAD );

    /* Disabling parity check */
    if(!in_SRS232Config.m_s32EnaParity)
    {
        if(in_SRS232Config.m_s8StopBit == DP_APP_UART_ONE_STOPBIT)
        {
            g_Stios.c_cflag &= ~(PARENB | CSTOPB);
            g_Stios.c_cflag |= CS8;
        }
        else
        {
            g_Stios.c_cflag &= ~(PARENB);
            g_Stios.c_cflag |= (CS8 | CSTOPB);
        }
    }

    /* Setting even parity */
    else if((in_SRS232Config.m_s32EnaParity == DP_APP_UART_ENA_PARITY) && (in_SRS232Config.m_s8Parity == DP_APP_UART_EVEN_PARITY))
    {
        if(in_SRS232Config.m_s8StopBit == DP_APP_UART_ONE_STOPBIT)
        {
            g_Stios.c_cflag |= (PARENB | CS7);
            g_Stios.c_cflag &= ~(PARODD | CSTOPB);
        }
        else
        {
            g_Stios.c_cflag |= (PARENB | CS7 | CSTOPB);
            g_Stios.c_cflag &= ~(PARODD);
        }
    }

    /* Setting odd parity */
    else if((in_SRS232Config.m_s32EnaParity == DP_APP_UART_ENA_PARITY) && (in_SRS232Config.m_s8Parity == DP_APP_UART_ODD_PARITY))
    {
        if(in_SRS232Config.m_s8StopBit == DP_APP_UART_ONE_STOPBIT)
        {
            g_Stios.c_cflag |= (PARENB | CS7 | PARODD);
            g_Stios.c_cflag &= ~(PARODD | CSTOPB);
        }
        else
            g_Stios.c_cflag |= (PARENB | CS7 | CSTOPB | PARODD);
    }


    /* Open the pcdevice...Enable the port with raw data mode */
    g_Stios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);


    /* Enable parity checking and disabling software flow control */
    g_Stios.c_iflag &= ~(INPCK | ISTRIP);
    g_Stios.c_iflag &= ~(IXOFF | IXON | IXANY);
    g_Stios.c_iflag &= ~(INLCR | ICRNL);


    /* Enabling output filtering option with raw data */
    g_Stios.c_oflag &= ~(OPOST);
    g_Stios.c_oflag &= ~(ONLCR | OCRNL);

    g_Stios.c_cc[VTIME] = in_SRS232Config.m_u32TimeOut;
    g_Stios.c_cc[VMIN] = DP_APP_VARINIT;

    /* Set the new attribute for the serial port */
    if (tcsetattr(m_s32hHandle[in_u16InstanceID-1], TCSANOW, &g_Stios) < 0)
    {
        perror("Failed in uart : ");
        printf("ErrNo : %d", errno);
        return DP_APP_UART_ERR_SET_ATTR;
    }
    return s32RetVal;
#endif
}

S32BIT STDCALL UART_StreamWrite(U16BIT in_u16InstanceID, PU8BIT in_pu8Data, U16BIT in_u8DataCnt)
{
    short s32RetVal = 0;
    unsigned long uiDataCount = 0;
    unsigned char ucTXFlag = 0;
    unsigned char ucRetryCount = 0;
#ifdef Q_OS_WINDOWS
    DWORD dwErrorFlags;
    COMSTAT ComStat;
#endif

    /*validations*/
    //    DPRDR8122_VALIDATE_HANDLE( m_hHandle[in_u16InstanceID-1]);
    //    DPRDR8122_VALIDATE_POINTER(in_pu8Data);
#ifdef Q_OS_WINDOWS
    for(ucRetryCount = 0; ucRetryCount < 3; ucRetryCount++)
    {
        //Discards all characters from the output or input buffer of a specified communications resource.
        PurgeComm( m_hHandle[in_u16InstanceID-1], PURGE_TXCLEAR);
        PurgeComm( m_hHandle[in_u16InstanceID-1], PURGE_RXCLEAR);
        uiDataCount = 0;

        ClearCommError(m_hHandle[in_u16InstanceID-1], &dwErrorFlags, &ComStat);
        uiDataCount = ComStat.cbInQue;

        s32RetVal = WriteFile( m_hHandle[in_u16InstanceID-1], in_pu8Data, in_u8DataCnt, &uiDataCount, NULL);
        DP_LIB_RETURN_ON_ERR(s32RetVal, ucTXFlag);
        if((s32RetVal != 0) || (uiDataCount != in_u8DataCnt))
        {
            continue;
        }

        break;
    }
    if(ucRetryCount > 3)
    {
        s32RetVal = DP_APP_WRITE_FAIL;
    }

#else
    if(in_pu8Data == NULL)
    {
        return DP_APP_UART_ERR_NULL_PTR;
    }

    if(s32TxDataSz > DP_APP_UART_MAX_DATA_SIZE)
    {
        return DP_APP_UART_INV_DATA_LENGTH;
    }
    /* Transmit the data */
    s32RetVal = write(m_s32hHandle[in_u16InstanceID-1], in_pu8Data, in_u8DataCnt);
    if(s32RetVal < 0)
    {
        return DP_APP_UART_ERR_WRITE;
    }
    usleep(10000);
#endif
    return s32RetVal;
}

S32BIT STDCALL UART_StreamRead(U16BIT in_u16InstanceID, U16BIT in_ucRxDataLen, U32BIT in_u32TimeOut, PU8BIT out_pucBuffData)
{
    short s32RetVal = DP_SUCCESS;
    unsigned long uiDataCount = 0;
    unsigned char ucRXFlag = 0;
    unsigned char ucRetryCount = 1;
    unsigned short  u16Loop = 0;

    S32BIT s32RxDataSz = 0;
#ifdef Q_OS_WINDOWS
    DWORD dwErrorFlags;
    COMSTAT ComStat;
#endif

    s32RxDataSz = in_ucRxDataLen;

    //    DPRDR8122_VALIDATE_HANDLE(m_hHandle[in_u16InstanceID]);
    //    DPRDR8122_VALIDATE_POINTER(in_pucBuffData);
    //    DPRDR8122_VALIDATE_POINTER(out_pucBuffData);
#ifdef Q_OS_WINDOWS
    for(ucRetryCount = 0; ucRetryCount < 3; ucRetryCount++)
    {
        for(u16Loop = 0; u16Loop < in_u32TimeOut; u16Loop++)
        {

            ClearCommError(m_hHandle[in_u16InstanceID-1], &dwErrorFlags, &ComStat);
            uiDataCount = ComStat.cbInQue;
            if(uiDataCount >= in_ucRxDataLen)
			{
                break;
            }
            QPC_mDelay(1);
        }

//        if(uiDataCount < in_ucRxDataLen)
//        {
//            continue;
//        }
        in_ucRxDataLen = uiDataCount;

        uiDataCount = 0;
        s32RetVal = ReadFile(m_hHandle[in_u16InstanceID-1], out_pucBuffData, in_ucRxDataLen, &uiDataCount, 0);
        DP_LIB_RETURN_ON_ERR(s32RetVal, ucRXFlag);
        if((ucRetryCount  > 3) || (s32RetVal < 0))
        {
            s32RetVal = DP_APP_READ_FAIL;
        }
		
		break;
    }
#else
    if(s32RxDataSz > DP_APP_UART_MAX_DATA_SIZE)
    {
        return DP_APP_UART_INV_DATA_LENGTH;
    }

    /* Receive the data */
    s32RetVal = read(m_s32hHandle[in_u16InstanceID-1], out_pucBuffData, in_ucRxDataLen);
    if(s32RetVal < 0)
    {
        return DP_APP_UART_ERR_READ;
    }
#endif
    return s32RetVal;
}

#ifdef Q_OS_WINDOWS
S32BIT STDCALL UART_SetCommunicationTimeouts( U16BIT in_u16InstanceID, DWORD in_dwReadIntervalTimeout, \
                                                                       DWORD in_dwReadTotalTimeoutMultiplier, DWORD in_dwReadTotalTimeoutConstant, \
                                                                       DWORD in_dwWriteTotalTimeoutMultiplier, DWORD in_dwWriteTotalTimeoutConstant)
{
    U8BIT ucErrFlag = 0;
    COMMTIMEOUTS CommTimeouts;
    S32BIT s32RetVal = DP_SUCCESS;

    // No need to validate input parameters because these are hard-coded values.

    s32RetVal = GetCommTimeouts(m_hHandle[in_u16InstanceID-1], &CommTimeouts);
    DP_LIB_RETURN_ON_ERR(s32RetVal, ucErrFlag);

    if(ucErrFlag != 1)
    {
        CommTimeouts.ReadIntervalTimeout = in_dwReadIntervalTimeout;
        CommTimeouts.ReadTotalTimeoutConstant = in_dwReadTotalTimeoutConstant;
        CommTimeouts.ReadTotalTimeoutMultiplier = in_dwReadTotalTimeoutMultiplier;
        CommTimeouts.WriteTotalTimeoutConstant = in_dwWriteTotalTimeoutConstant;
        CommTimeouts.WriteTotalTimeoutMultiplier = in_dwWriteTotalTimeoutMultiplier;

        s32RetVal = SetCommTimeouts(m_hHandle[in_u16InstanceID-1], &CommTimeouts);
    }

    return s32RetVal;
}
#else
/* LINUX STANDARD UART COMMUNICATION*/

speed_t STDCALL DP_APP_UART_MapSpeed(PS8BIT in_pcBaudRate)
{
    struct speedmap *pSsmp = speedmap, *pSesmp = &speedmap[g_inspeeds];
    int iBaudCmpVal = 0;
    /* Map the baud rate defined in the structure */
    while(pSsmp < pSesmp)
    {
        iBaudCmpVal = strcmp(in_pcBaudRate, pSsmp->str);
        if( iBaudCmpVal == 0)
        {
            /* Return the line speed value corresponding to the baud rate */
            return (pSsmp->val);
        }

        pSsmp++;
    }

    return DP_APP_ERR_INVALID_BAUDRATE;
}
#endif


S32BIT STDCALL UART_Close(U16BIT in_u16InstanceID)
{
    S32BIT s32RetVal = DP_SUCCESS;
    //    DPRDR8122_VALIDATE_HANDLE(in_hHandle);
#ifdef Q_OS_WINDOWS
    s32RetVal = CloseHandle(m_hHandle[in_u16InstanceID-1]);
    if(s32RetVal != 0)
    {
        s32RetVal = DP_SUCCESS;
    }

    if(s32RetVal != DP_SUCCESS)
    {
        //        return DPRDR8122_RS232_PORT_CLOSE;
    }
    if(m_s32hHandle[in_u16InstanceID-1] != 0)
    {
        CloseHandle((HANDLE)m_s32hHandle[in_u16InstanceID-1]);
    }
#else
    if(m_s32hHandle[in_u16InstanceID-1] != 0)
    {
        close(m_s32hHandle[in_u16InstanceID-1]);
    }
#endif
    return s32RetVal;
    return DP_SUCCESS;
}


void QPC_mDelay(unsigned int in_uiDelayinMs)
{
#ifdef Q_OS_WINDOWS
    LARGE_INTEGER liStartingTime, liEndingTime, liElapsedTime;
    LARGE_INTEGER liFrequency;

    QueryPerformanceFrequency(&liFrequency);
    QueryPerformanceCounter(&liStartingTime);

    do
    {
        QueryPerformanceCounter(&liEndingTime);
        liElapsedTime.QuadPart = liEndingTime.QuadPart - liStartingTime.QuadPart;
        liElapsedTime.QuadPart *= 1000;
        liElapsedTime.QuadPart /= liFrequency.QuadPart;
    }while(liElapsedTime.QuadPart  < in_uiDelayinMs);
#else
/*Add linux Delay*/
#endif
}

void QPC_uDelay(unsigned int in_uiDelayinUs)
{
#ifdef Q_OS_WINDOWS
    LARGE_INTEGER liStartingTime, liEndingTime, liElapsedTime;
    LARGE_INTEGER liFrequency;

    QueryPerformanceFrequency(&liFrequency);
    QueryPerformanceCounter(&liStartingTime);

    do
    {

        QueryPerformanceCounter(&liEndingTime);
        liElapsedTime.QuadPart = liEndingTime.QuadPart - liStartingTime.QuadPart;
        liElapsedTime.QuadPart *= 1000000;
        liElapsedTime.QuadPart /= liFrequency.QuadPart;

    }while(liElapsedTime.QuadPart  < in_uiDelayinUs);
#else
    /*Add linux Delay*/
#endif
}
