#include "dp_QSocketComm.h"
#include "macros.h"
#include "winsock2.h"
QMutex m_ObjETHMutexLock;

CQSocketComm::CQSocketComm(QObject *parent) : QObject(parent)
{

    m_u8TCPUDPConn = 0;
    m_u8connFlag = 0;
    m_pobjQTcpSocket = new QTcpSocket(this);
    m_pobjQUdpSocket = new QUdpSocket(this);

    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    connect(m_pobjQTcpSocket, &QTcpSocket::disconnected, this, &CQSocketComm::Slot_TargetConnectionStatus/*, Qt::UniqueConnection*/);
    connect(m_pobjQUdpSocket, &QTcpSocket::disconnected, this, &CQSocketComm::Slot_TargetConnectionStatus/*, Qt::UniqueConnection*/);
    connect(m_pobjQTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(ErrorHandler(QAbstractSocket::SocketError)));
    connect(m_pobjQUdpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(ErrorHandler(QAbstractSocket::SocketError)));
}

CQSocketComm::~CQSocketComm()
{
    DisConnectFromServer();
    delete m_pobjQTcpSocket;
    delete m_pobjQUdpSocket;
}

S16BIT CQSocketComm::ConnectToServer(QString in_qstrIpAddress, quint16 in_u16port, U8BIT in_u8ConnType)
{
    QHostAddress objIpAddress(in_qstrIpAddress);
    QString qstrErrMsg;
    S32BIT iRetVal = 0;
    WSADATA wsaData;
    fd_set Write, Err;
    TIMEVAL Timeout;
    Timeout.tv_sec = 2; /* Timeout in Sec */
    Timeout.tv_usec = 0; /* Timeout in MilliSec */

    Q_UNUSED(in_u8ConnType);

    sockaddr_in saSockAddr;
    qstrErrMsg.clear();
    if (QAbstractSocket::IPv4Protocol == objIpAddress.protocol()) {
        qDebug("Valid IPv4 Address.");
    } else if (QAbstractSocket::IPv6Protocol == objIpAddress.protocol()) {
        qDebug("Valid IPv6 address.");
    } else {
        qDebug("Unknown or invalid Address.");
        return ERR_IPADDR_INVALID;
    }

    if(in_u8ConnType == QUDP_CONN)
    {
        // UDP Communication Setup

        // Initialize WinSock
        iRetVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iRetVal != RETURN_SUCCESS)
        {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Windows Socket Startup failed");
            return RETURN_FAILURE;
        }

        // Fill sockaddr_in structure
        ZeroMemory(&saSockAddr, sizeof(sockaddr_in));
        saSockAddr.sin_family = AF_INET;
        saSockAddr.sin_port = htons(in_u16port);
        saSockAddr.sin_addr.s_addr = inet_addr((const char*)in_qstrIpAddress.toStdString().c_str());

        // Create a UDP socket
        m_sktConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (m_sktConnectSocket == INVALID_SOCKET)
        {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "UDP Socket Creation failed");
            WSACleanup();
            return RETURN_FAILURE;
        }

        // Set non-blocking mode
        unsigned long iMode = 1;
        int iResult = ioctlsocket(m_sktConnectSocket, FIONBIO, &iMode);
        if (iResult != NO_ERROR)
        {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "ioctlsocket failed with error: " + QString::number(iResult));
            closesocket(m_sktConnectSocket);
            WSACleanup();
            return RETURN_FAILURE;
        }

        // Attempt to send a datagram to the server
        const char* message = "Hello from UDP Client";
        iResult = sendto(m_sktConnectSocket, message, strlen(message), 0, (struct sockaddr*)&saSockAddr, sizeof(saSockAddr));
        if (iResult == SOCKET_ERROR)
        {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "sendto failed with error: " + QString::number(WSAGetLastError()));
            closesocket(m_sktConnectSocket);
            WSACleanup();
            return RETURN_FAILURE;
        }

        // Prepare for select call
        FD_ZERO(&Write);
        FD_ZERO(&Err);
        FD_SET(m_sktConnectSocket, &Write);
        FD_SET(m_sktConnectSocket, &Err);

        // Check if the socket is ready
        select(0, NULL, &Write, &Err, &Timeout);
        if (FD_ISSET(m_sktConnectSocket, &Write) == 0)
        {
            closesocket(m_sktConnectSocket);
            WSACleanup();
            return RETURN_FAILURE;
        }

        // Set back to blocking mode
        iMode = 0;
        iResult = ioctlsocket(m_sktConnectSocket, FIONBIO, &iMode);
        if (iResult != NO_ERROR)
        {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "ioctlsocket failed with error: " + QString::number(iResult));
            closesocket(m_sktConnectSocket);
            WSACleanup();
            return RETURN_FAILURE;
        }
    }
    else
    {
        // TCP Communication Setup
        FD_ZERO(&Write);
        FD_ZERO(&Err);

        // Fill sockaddr_in structure
        ZeroMemory(&saSockAddr, sizeof(sockaddr_in));

        iRetVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iRetVal != RETURN_SUCCESS) {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Windows Socket Startup failed");
            return RETURN_FAILURE;
        }

        saSockAddr.sin_family = AF_INET;
        saSockAddr.sin_port = htons(in_u16port);
        saSockAddr.sin_addr.s_addr = inet_addr((const char*)in_qstrIpAddress.toStdString().c_str());

        // Create a SOCKET for connecting to server
        m_sktConnectSocket = socket(saSockAddr.sin_family, SOCK_STREAM, 0);
        if (m_sktConnectSocket == INVALID_SOCKET) {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Socket Creation failed");
            WSACleanup();
            return RETURN_FAILURE;
        }

        // Set the socket in non-blocking mode
        unsigned long iMode = 1;
        int iResult = ioctlsocket(m_sktConnectSocket, FIONBIO, &iMode);
        if (iResult != NO_ERROR) {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "ioctlsocket failed with error: " + QString::number(iResult));
        }

        // Connect to server
        if (WSAConnect(m_sktConnectSocket, (struct sockaddr*)&saSockAddr, (int)sizeof(sockaddr_in), NULL, NULL, NULL, NULL) == false) {
            closesocket(m_sktConnectSocket);
            m_sktConnectSocket = INVALID_SOCKET;
        }

        if (m_sktConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            return RETURN_FAILURE;
        }

        // Restart the socket mode
        iMode = 0;
        iResult = ioctlsocket(m_sktConnectSocket, FIONBIO, &iMode);
        if (iResult != NO_ERROR) {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "ioctlsocket failed with error: " + QString::number(iResult));
            return RETURN_FAILURE;
        }

        FD_SET(m_sktConnectSocket, &Write);
        FD_SET(m_sktConnectSocket, &Err);

        // Check if the socket is ready
        select(0, NULL, &Write, &Err, &Timeout);
        if (FD_ISSET(m_sktConnectSocket, &Write) == 0) {
            return RETURN_FAILURE;
        }

    }

    m_u8connFlag = 1;

    return RETURN_SUCCESS;
}

void CQSocketComm::ErrorHandler(QAbstractSocket::SocketError socketError)
{
#ifndef __WINSOCKET__
    if(m_u8TCPUDPConn == QTCP_CONN)
    {
        qDebug() << "Socket Error: " << socketError;
        qDebug() << "Error String: " << m_pobjQTcpSocket->errorString();
    }
    else
    {
        qDebug() << "Socket Error: " << socketError;
        qDebug() << "Error String: " << m_pobjQUdpSocket->errorString();
    }
#endif
}

S16BIT CQSocketComm::ReadPacketFromServer()
{
    QByteArray qbArrReadResp;
    if(m_u8TCPUDPConn == QTCP_CONN)
    {
        m_pobjQTcpSocket->waitForReadyRead(10 * 1000);
        if(m_pobjQTcpSocket->state() == QAbstractSocket::ConnectedState)
        {
            qbArrReadResp = m_pobjQTcpSocket->readAll();
            if(qbArrReadResp.size())
            {
                qDebug("Recieving Packet from Target Successfull");
            }
            else
            {
                qDebug() << "ReadPacketFromServer()" << m_pobjQTcpSocket->errorString();
                return QTCP_ERR_RECIEVE_PACKET;
            }
        }
        else
        {
            qDebug() << "ReadPacketFromServer(): " << m_pobjQTcpSocket->errorString();
            return QTCP_ERR_CONN_STATE;
        }
    }
    else if (m_u8TCPUDPConn == QUDP_CONN)
    {
            qDebug() << "UDP Read Connection";
    }
    else
    {
        qDebug("ReadPacketFromServer(): Unknown Connection Type");
        return ERR_UNKNOWN_CONN;
    }
    return RETURN_SUCCESS;
}

S16BIT CQSocketComm::DisConnectFromServer()
{
    int iRetVal = 0;
#ifndef __WINSOCKET__
    if(m_pobjQTcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        m_pobjQTcpSocket->disconnectFromHost();
        emit sig_MsgToPrintActlog(PRINTLOG_TYPE_INFO, "TCP Connection Disconnected from Server.");
    }

    if(m_pobjQUdpSocket->state() == QAbstractSocket::ConnectedState)
    {
        m_pobjQUdpSocket->disconnectFromHost();
        emit sig_MsgToPrintActlog(PRINTLOG_TYPE_INFO, "UDP Connection Disconnected from Server.");
    }
#else

    iRetVal = shutdown(m_sktConnectSocket, SD_BOTH); //Shutdown the both transmit and receive operation
    if(iRetVal != RETURN_SUCCESS)
    {
        emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Connection Shutdown failure");
    }

    iRetVal = closesocket(m_sktConnectSocket);
    if(iRetVal != RETURN_SUCCESS)
    {
        emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Socket close failure");
    }

    WSACleanup();
#endif
    m_u8connFlag = 0;

    return RETURN_SUCCESS;
}

S16BIT CQSocketComm::Slot_TargetConnectionStatus()
{
    QString qstrMsg;
    qstrMsg.clear();
#ifndef __WINSOCKET__
    if(m_u8TCPUDPConn == QTCP_CONN)
    {
        if(m_pobjQTcpSocket->state() != QAbstractSocket::ConnectedState)
        {
            qDebug("TCP Connection to Server Closed");
            qstrMsg = "TCP Connection to Server Closed";
        }
    }
    else
    {
        if(m_pobjQUdpSocket->state() != QAbstractSocket::ConnectedState)
        {
            qDebug("UDP Connection to Server Closed");
            qstrMsg = "UDP Connection to Server Closed";
        }
    }
    m_u8connFlag = 0;

    emit sig_ServerDisconnected(qstrMsg);
#endif
    return RETURN_SUCCESS;
}


void CQSocketComm::GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_ps8ErrMsg)
{
    switch(in_s32ErrVal)
    {
    case QTCP_ERR_CONN_FAILED:
        sprintf(out_ps8ErrMsg, "QSocketComm: QTCP Socket Open Error!");
        break;
    case QUDP_ERR_CONN_FAILED:
        sprintf(out_ps8ErrMsg, "QSocketComm: QUDP Socket Open Error!");
        break;
    case ERR_IPADDR_INVALID:
        sprintf(out_ps8ErrMsg, "QSocketComm: IP Address Invalid!");
        break;
    case ERR_UNKNOWN_CONN:
        sprintf(out_ps8ErrMsg, "QSocketComm: Unknown Connection Type!");
        break;
    case QTCP_ERR_SEND_PACKET:
        sprintf(out_ps8ErrMsg, "QSocketComm: QTCP Send Packet Error!");
        break;
    case QUDP_ERR_SEND_PACKET:
        sprintf(out_ps8ErrMsg, "QSocketComm: QUDP Send Packet Error!");
        break;
    case QTCP_ERR_RECIEVE_PACKET:
        sprintf(out_ps8ErrMsg, "QSocketComm: QTCP Recieve Packet Error!");
        break;
    case QUDP_ERR_RECIEVE_PACKET:
        sprintf(out_ps8ErrMsg, "QSocketComm: QUDP Recieve Packet Error!");
        break;
    case QTCP_ERR_CONN_STATE:
        sprintf(out_ps8ErrMsg, "QSocketComm: QTCP Connection State Error!");
        break;
    case QUDP_ERR_CONN_STATE:
        sprintf(out_ps8ErrMsg, "QSocketComm: QUDP Connection State Error!");
        break;
    default:
        sprintf(out_ps8ErrMsg, "QSocketComm: Unknown Error Value: %d", in_s32ErrVal);
        break;
    }
}

short CQSocketComm::DP_EndianSwap(DP_SET_SYSTEM_TIME *pESMsystemTime)
{
    if(pESMsystemTime == NULL)
    {
        return -1;
    }
    pESMsystemTime->m_GmtTime = htonl(pESMsystemTime->m_GmtTime);

    return RETURN_SUCCESS;
}

void CQSocketComm::DP_Set0214TimeandDate()
{

    time_t UTC_Time;
    unsigned long ulRxMessageSize = 0;
    unsigned long ulTxMessageSize = 0;
    short sRetVal = 0;
    char arrcData[DP_ERROR_BUFF_SIZE] = {0};

    S_COMMAND_FROM_HOST                      SCmdFromEcmAtp;
    S_RESPONSE_TO_HOST						SResToEcmAtp;
#if 0
    if(m_pobjQTcpSocket->waitForConnected() == false)
    {
        this->GetErrorMessage(1, arrcData);
        emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, arrcData);
        QMessageBox::information(nullptr, "QSocketComm Wrapper", arrcData);
        return;
    }
#endif
    /*Set Group Id and Unit Id for Version */
    SCmdFromEcmAtp.m_SCmdIds.m_u16GroupId = DP_COMM_CMD_GROUP0;
    SCmdFromEcmAtp.m_SCmdIds.m_u16UnitId =  DP_CMD_GET_DEVICE_DETAILS;

    time(&UTC_Time);
    memcpy(&SCmdFromEcmAtp.m_UCmdMessage.m_sSystemDateTime.m_GmtTime, &UTC_Time, sizeof(UTC_Time));

    ulTxMessageSize = sizeof(DP_SET_SYSTEM_TIME);

#ifdef DP_ENDIAN_SWAP
    sRetVal = DP_ESM_EndianSwap(&SCmdFromEcmAtp.m_UCmdMessage.S_Cmd_SetESM_System_Time);
    if(sRetVal)
    {
        QMessageBox::information(nullptr, "EndianSwap", arrcData, MB_ICONERROR);
        return;
    }
#endif
    sRetVal = DP_TCPCmdTrans(/*m_pobjQTcpSocket, */&SCmdFromEcmAtp, ulTxMessageSize, &SResToEcmAtp, &ulRxMessageSize, DP_CMD_DEFAULT_TIMEOUT_MS);
    if(sRetVal == RETURN_SUCCESS)
    {
        if((SResToEcmAtp.m_SResIds.m_u16GroupId == DP_COMM_CMD_GROUP0) && (SResToEcmAtp.m_SResIds.m_u16UnitId == DP_RES_SET_ESM_SYSTEM_TIME))
        {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_SUCCESS, "System date/time configuration success");
        }
        else
        {
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Invalid Group/Unit ID of Response");
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "System date/time configuration failure");
            return ;
        }
    }
    else
    {
        emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "System date/time configuration failure");
        emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, arrcData);
        return ;
    }

}

short CQSocketComm::DP_TCPCmdTrans(/*QTcpSocket* ClientSocket,*/
                            LPVOID pInputParam,
                            unsigned long in_ulInputParamLen,
                            LPVOID pOutputParam,
                            unsigned long *pOutputParamLen,
                            unsigned long in_ulTimeOut)
{
    static S_DP_TX_TCP_PACKET	SECM_TxTcpPacket;
    static S_DP_RX_TCP_PACKET	SECM_RxTcpPacket;

    static S_COMMAND_FROM_HOST	SCommandFromECM;

    static char cTransmitBuffer[ESM_TCPMSG_BUFFER_SIZE];

    unsigned long ulTxPacketLength = 0;

    S_COMMAND_FROM_HOST *pucInParam;
    S_RESPONSE_TO_HOST  *pucOutParam;

    int sRetVal = RETURN_FAILURE;
    int iResult = RETURN_SUCCESS;

    // Copy the Param Data to the message
    pucInParam = (S_COMMAND_FROM_HOST *)pInputParam;

    memcpy(&SCommandFromECM, pucInParam, (DP_MSGIDS_SIZE  + in_ulInputParamLen));
    if(SCommandFromECM.m_SCmdIds.m_u16GroupId && SCommandFromECM.m_SCmdIds.m_u16UnitId) /* Group ID and Unit ID 0 for receiving Reports from WBSPU */
    {
        SECM_TxTcpPacket.m_u8TCPHeader[0]	=	0xAA;
        SECM_TxTcpPacket.m_u8TCPHeader[1]	=	0xAB;
        SECM_TxTcpPacket.m_u8TCPHeader[2]	=	0xBA;
        SECM_TxTcpPacket.m_u8TCPHeader[3]	=	0xBB;

        SECM_TxTcpPacket.m_u8TCPFooter[0]	=	0xCC;
        SECM_TxTcpPacket.m_u8TCPFooter[1]	=	0xCD;
        SECM_TxTcpPacket.m_u8TCPFooter[2]	=	0xDC;
        SECM_TxTcpPacket.m_u8TCPFooter[3]	=	0xDD;

        // Validate the input parameter length
        if(in_ulInputParamLen > ESM_MAX_TCP_STACK_SIZE)
        {
            qDebug() << QString::number(in_ulInputParamLen);
            return RETURN_FAILURE;
        }

        if(in_ulInputParamLen)
        {
            if(pInputParam == NULL)
            {
                qDebug() << "FAIL : Input Parameter Null";
                emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Input Parameter Null");
                return RETURN_FAILURE;
            }
        }

        SECM_TxTcpPacket.m_SCommandFromHost.m_SCmdIds.m_u16GroupId = htons(SCommandFromECM.m_SCmdIds.m_u16GroupId);
        SECM_TxTcpPacket.m_SCommandFromHost.m_SCmdIds.m_u16UnitId = htons(SCommandFromECM.m_SCmdIds.m_u16UnitId);

        SECM_TxTcpPacket.m_ulMessageLength = in_ulInputParamLen;
//        SECM_TxTcpPacket.m_ulMessageLength = 0;
        //qDebug() << "Size of Input Packet:" << in_ulInputParamLen;
        memcpy(SECM_TxTcpPacket.m_SCommandFromHost.m_UCmdMessage.m_arru8TxData, SCommandFromECM.m_UCmdMessage.m_arru8TxData, in_ulInputParamLen);

        DP_FrameTCP_TXBuffer(cTransmitBuffer, &SECM_TxTcpPacket, &ulTxPacketLength);

        m_ObjETHMutexLock.lock();

#ifndef __WINSOCKET__
        if(!(m_pobjQTcpSocket->waitForConnected()))
        {
            qDebug() << "Server Not Connected";
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Server Not Connected");
            return RETURN_FAILURE;
        }
#endif

        if(ulTxPacketLength > ESM_MAX_TCP_STACK_SIZE)
        {
            unsigned long ulTxBufferCount = 0, ulTransmitSize = 0;
            while(ulTxBufferCount < ulTxPacketLength)
            {
                if((ulTxBufferCount + ESM_MAX_TCP_STACK_SIZE) <=  ulTxPacketLength)
                {
                    ulTransmitSize = ESM_MAX_TCP_STACK_SIZE;
#ifndef __WINSOCKET__
                    iResult = m_pobjQTcpSocket->write(&cTransmitBuffer[ulTxBufferCount], ulTransmitSize);
#else
                    iResult = send( m_sktConnectSocket, &cTransmitBuffer[ulTxBufferCount], ulTransmitSize, 0 );
#endif
                }
                else
                {
                    ulTransmitSize = (ulTxPacketLength - ulTxBufferCount);
#ifndef __WINSOCKET__
                    iResult = m_pobjQTcpSocket->write(&cTransmitBuffer[ulTxBufferCount], ulTransmitSize);
#else
                    iResult = send( m_sktConnectSocket, &cTransmitBuffer[ulTxBufferCount], ulTransmitSize, 0 );
#endif
                }

                ulTxBufferCount += ulTransmitSize;
                if (iResult == SOCKET_ERROR)
                {
                    m_ObjETHMutexLock.unlock();
#ifndef __WINSOCKET__
                   qDebug() << "send failed with error: " << iResult;
                    m_pobjQTcpSocket->disconnectFromHost();
#else
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(m_sktConnectSocket);
                    WSACleanup();
#endif
                    return DP_ESM_APP_ETH_TIMEOUT;
                }
            }

        }
        else
        {
#ifndef __WINSOCKET__
            iResult = m_pobjQTcpSocket->write(cTransmitBuffer, ulTxPacketLength);
#else
            iResult = send(m_sktConnectSocket, cTransmitBuffer, ulTxPacketLength, 0);
#endif
            if (iResult == SOCKET_ERROR)
            {
                m_ObjETHMutexLock.unlock();
#ifndef __WINSOCKET__
                   qDebug() << "send failed with error: " << iResult;
                    m_pobjQTcpSocket->disconnectFromHost();
#else
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(m_sktConnectSocket);
                    WSACleanup();
#endif
                return DP_ESM_APP_ETH_TIMEOUT;
            }
        }
#ifndef __WINSOCKET__

        if (iResult == SOCKET_ERROR)
        {
            m_pobjQTcpSocket->disconnectFromHost();
            qDebug() << "send failed";
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Send failed in Transmit");
            return RETURN_FAILURE;
        }

        iResult = m_pobjQTcpSocket->waitForBytesWritten(1000);

        if (iResult == SOCKET_ERROR)
        {
            m_pobjQTcpSocket->disconnectFromHost();
            qDebug() << "send failed";
            emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Send failed in Transmit");
            return RETURN_FAILURE;
        }
#endif

    }
#if 1
    sRetVal = DP_TCPClientResRev(&SECM_RxTcpPacket, in_ulTimeOut);
    if(sRetVal == RETURN_SUCCESS)
    {
        sRetVal = SECM_RxTcpPacket.m_s16MsgStatus;

        *pOutputParamLen = SECM_RxTcpPacket.m_ulMessageLength;

        //Copy the Param Data to the message
        pucOutParam = (S_RESPONSE_TO_HOST *)pOutputParam;

        memcpy(&pucOutParam->m_SResIds, &SECM_RxTcpPacket.m_SResponseToHost.m_SResIds, (DP_MSGIDS_SIZE));
        memcpy(&pucOutParam->m_UResMessage.m_arru8RxData, &SECM_RxTcpPacket.m_SResponseToHost.m_UResMessage.m_arru8RxData, (*pOutputParamLen));
    }
    else
    {
        emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE, "Response Recieved Error");
    }
#endif
    m_ObjETHMutexLock.unlock();
    return sRetVal;
}

int CQSocketComm::DP_ReadFromServer(/*QTcpSocket *in_pobjClientSocket, */char *cRXData, unsigned long ulReadByteCount,unsigned long ulWaitMilliSeconds)
{
    unsigned long ulRetVal = 0;
    S32BIT s32Result = 0;
 #ifndef __WINSOCKET__
    ulRetVal = m_pobjQTcpSocket->bytesAvailable();
    if(ulRetVal < ulReadByteCount)
    {
        /** waitForReadyRead is a blocking Funtion cause socket notifer acces 1 socket with 2 thread warning on run **/
        m_pobjQTcpSocket->waitForReadyRead(ulWaitMilliSeconds);
    }
    s32Result = m_pobjQTcpSocket->read(cRXData, ulReadByteCount);
#else
    DP_SetTimeOut(m_sktConnectSocket, ulWaitMilliSeconds);
    s32Result = recv(m_sktConnectSocket, cRXData, ulReadByteCount, 0);
    DP_SetTimeOut(m_sktConnectSocket, 0);
#endif
    return s32Result;
}

short CQSocketComm::DP_TCPClientResRev(/*QTcpSocket *ClientSocket, */S_DP_RX_TCP_PACKET *in_SECM_RxTcpPacket, unsigned short in_usTimeOut)
{

    unsigned short usRXMessageStatus	= DP_CMD_NOTRECED;
    unsigned char ucRxCmdState			= DP_RX_HEADER_STATE;
    char cRXData = 0;
    unsigned short usCmdIndex = 0;

    unsigned char	ucTCPHeader[DP_HEADER_SIZE];
    unsigned char	ucTCPFooter[DP_HEADER_SIZE];
    char arrcRXMegLen[DP_MSGLEN_SIZE];
    static char	cReceiveBuffer[ESM_TCPMSG_BUFFER_SIZE];

    short sMsgStatus = 0;
    short sReserved = 0;
    unsigned long ulMessageLen = 0;
    unsigned short usMessageId = 0;

    int iResult = 0;

    unsigned long ulMessageWordCounter = 0;

    ucTCPHeader[0]	=	0xEE;
    ucTCPHeader[1]	=	0xEF;
    ucTCPHeader[2]	=	0xFE;
    ucTCPHeader[3]	=	0xFF;

    ucTCPFooter[0]	=	0xFF;
    ucTCPFooter[1]	=	0xFE;
    ucTCPFooter[2]	=	0xEF;
    ucTCPFooter[3]	=	0xEE;

    while(usRXMessageStatus != DP_CMD_RECEIVED)
    {
        cRXData = 0;

        switch(ucRxCmdState)
        {
            case DP_RX_HEADER_STATE:

                iResult = DP_ReadFromServer(&cRXData, DP_RX_BYTE_COUNT,in_usTimeOut);

                if (iResult == DP_RX_BYTE_COUNT)
                {
                    if((unsigned char)cRXData == ucTCPHeader[usCmdIndex])
                    {
                        usCmdIndex++;
                    }
                    else if((unsigned char)cRXData == ucTCPHeader[0])
                    {
                        usCmdIndex = 1;
                    }
                    else
                    {
                        usCmdIndex = 0;
                    }

                    if(usCmdIndex)
                    {
                        in_SECM_RxTcpPacket->m_u8szTCPHeader[usCmdIndex-1] = (unsigned char)cRXData;
                    }

                    if(usCmdIndex >= DP_HEADER_SIZE)
                    {
                        ucRxCmdState = DP_RX_MSGSTS_STATE;
                        usCmdIndex = 1;
                    }
                }
                else
                {
                    qDebug() << "\n RES - TIME OUT - HEADER\n";
                    emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE,  "RES - TIME OUT - HEADER");
                    return DP_ESM_APP_ETH_TIMEOUT;
                }
                iResult = 0;

                break;

            case DP_RX_MSGSTS_STATE:
                sMsgStatus = 0;

                iResult = DP_ReadFromServer((char *)&sMsgStatus, sizeof(sMsgStatus),in_usTimeOut);

                if (iResult == sizeof(sMsgStatus))
                {
                    in_SECM_RxTcpPacket->m_s16MsgStatus = htons(sMsgStatus);
                    ucRxCmdState = DP_RX_MSGLEN_STATE;
                    usCmdIndex = 1;
                }
                else
                {
                    qDebug() << "\n RES - TIME OUT - STATUS\n";
                    emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE,  "RES - TIME OUT - STATUS 1");
                    return DP_ESM_APP_ETH_TIMEOUT;
                }

                iResult = DP_ReadFromServer((char *)&sMsgStatus, sizeof(sReserved), in_usTimeOut);

                if (iResult == sizeof(sReserved))
                {
                    in_SECM_RxTcpPacket->m_s16Reserved = htons(sReserved);
                    ucRxCmdState = DP_RX_MSGLEN_STATE;
                    usCmdIndex = 1;
                }
                else
                {
                    qDebug() << "\n RES - TIME OUT - STATUS\n";
                    emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE,  "RES - TIME OUT - STATUS 2");
                    return DP_ESM_APP_ETH_TIMEOUT;
                }

                iResult = 0;
                break;

            case DP_RX_MSGLEN_STATE:

                memset(arrcRXMegLen,0,DP_MSGLEN_SIZE);

                ulMessageLen = 0;

                iResult = DP_ReadFromServer((char *)&ulMessageLen, DP_MSGLEN_SIZE,in_usTimeOut);

                if (iResult == DP_MSGLEN_SIZE)
                {
                    in_SECM_RxTcpPacket->m_ulMessageLength = 0;
                    in_SECM_RxTcpPacket->m_ulMessageLength = htonl(ulMessageLen);
                    ucRxCmdState = DP_RX_MSGID_STATE;
                    usCmdIndex = 1;

                }
                else
                {
                    qDebug() << "\n RES - TIME OUT - MSGLEN\n";
                    emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE,  "RES - TIME OUT - MSGLEN");
                    return DP_ESM_APP_ETH_TIMEOUT;
                }
                iResult = 0;
                break;

            case DP_RX_MSGID_STATE:

                usMessageId = 0;

                iResult = DP_ReadFromServer((char *)&usMessageId, sizeof(usMessageId),in_usTimeOut);

                if (iResult == sizeof(usMessageId))
                {
                    if(usCmdIndex)
                    {
                        in_SECM_RxTcpPacket->m_SResponseToHost.m_SResIds.m_u16GroupId = htons(usMessageId);
                        usCmdIndex = 0;
                    }
                    else
                    {
                        in_SECM_RxTcpPacket->m_SResponseToHost.m_SResIds.m_u16UnitId = htons(usMessageId);
                        usCmdIndex = 0;

                        if(in_SECM_RxTcpPacket->m_ulMessageLength)
                        {
                            ucRxCmdState = DP_RX_MSGDATA_STATE;
                        }
                        else
                        {
                            ucRxCmdState = DP_RX_FOOTER_STATE;
                        }
                    }
                }
                else
                {
                    qDebug() << "\n RES - TIME OUT - MSGID\n";
                    emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE,  "RES - TIME OUT - MSGID");
                    return DP_ESM_APP_ETH_TIMEOUT;
                }
                break;



            case DP_RX_MSGDATA_STATE:

                memset(&cReceiveBuffer, 0, sizeof(char) * in_SECM_RxTcpPacket->m_ulMessageLength);

                iResult = 0;
                ulMessageWordCounter = 0;
                while(iResult != SOCKET_ERROR)
                {

                    iResult = DP_ReadFromServer((char *)&cReceiveBuffer[ulMessageWordCounter], (in_SECM_RxTcpPacket->m_ulMessageLength - ulMessageWordCounter), in_usTimeOut);

                    if(iResult > 0)
                    {
                        ulMessageWordCounter += iResult;
                        if(ulMessageWordCounter >= in_SECM_RxTcpPacket->m_ulMessageLength)
                        {
                            break;
                        }
                    }
                }

                if(iResult == SOCKET_ERROR)
                {
                    usRXMessageStatus = DP_CMD_NOTRECED;
                    ucRxCmdState = DP_RX_HEADER_STATE;
                    return DP_ESM_APP_ETH_TIMEOUT;

                }

                cReceiveBuffer[ulMessageWordCounter] = '\0';

                if((unsigned long)ulMessageWordCounter == in_SECM_RxTcpPacket->m_ulMessageLength)
                {
                    memcpy(&in_SECM_RxTcpPacket->m_SResponseToHost.m_UResMessage.m_arru8RxData, (unsigned char*)cReceiveBuffer, in_SECM_RxTcpPacket->m_ulMessageLength);
                    memset(cReceiveBuffer,0,in_SECM_RxTcpPacket->m_ulMessageLength);
                    ucRxCmdState = DP_RX_FOOTER_STATE;
                    usCmdIndex = 0;
                }
                else
                {
                    qDebug() << "\n RES - TIME OUT - MSGDATA\n";
                    emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE,  "RES - TIME OUT - MSGDATA");
                    usRXMessageStatus = DP_CMD_NOTRECED;
                    ucRxCmdState = DP_RX_HEADER_STATE;
                    return DP_ESM_APP_ETH_TIMEOUT;
                }
                iResult = 0;
                break;

            case DP_RX_FOOTER_STATE:

                iResult = DP_ReadFromServer((char *)&cRXData, DP_RX_BYTE_COUNT,in_usTimeOut);

                if (iResult == DP_RX_BYTE_COUNT)
                {

                    if((unsigned char)cRXData == ucTCPFooter[usCmdIndex])
                    {
                        usCmdIndex++;
                    }
                    else if((unsigned char)cRXData == ucTCPFooter[0])
                    {
                        usCmdIndex = 1;
                    }
                    else
                    {
                        usCmdIndex = 0;
                    }

                    if(usCmdIndex)
                    {
                        in_SECM_RxTcpPacket->m_u8szTCPFooter[usCmdIndex-1] = (unsigned char)cRXData;
                    }

                    if(usCmdIndex >= DP_FOOTER_SIZE)
                    {
                        ucRxCmdState = DP_RX_HEADER_STATE;
                        usRXMessageStatus = DP_CMD_RECEIVED;
                        usCmdIndex = 0;
                    }
                }
                else
                {
                    qDebug() << "\n RES - TIME OUT - FOOTER\n";
                    emit sig_MsgToPrintActlog(PRINTLOG_TYPE_FAILURE,  "RES - TIME OUT - FOOTER");
                    return DP_ESM_APP_ETH_TIMEOUT;
                }
                iResult = 0;
                break;

            default:
                break;
        }

    }

    return RETURN_SUCCESS;

}


short CQSocketComm::DP_FrameTCP_TXBuffer(char *cTransmitBuffer, S_DP_TX_TCP_PACKET *SECM_TxTcpPacket, unsigned long *ulTxPacketLength)
{

    unsigned long ulBufferIndex = 0, ulMessageLength = 0;

    ulMessageLength = SECM_TxTcpPacket->m_ulMessageLength;

    SECM_TxTcpPacket->m_ulMessageLength = htonl(SECM_TxTcpPacket->m_ulMessageLength);

    memcpy(&cTransmitBuffer[ulBufferIndex],&SECM_TxTcpPacket->m_u8TCPHeader, DP_HEADER_SIZE);

    ulBufferIndex = ulBufferIndex + DP_HEADER_SIZE;

    memcpy(&cTransmitBuffer[ulBufferIndex],&SECM_TxTcpPacket->m_ulMessageLength,DP_MSGLEN_SIZE);

    ulBufferIndex = ulBufferIndex + DP_MSGLEN_SIZE;

    memcpy(&cTransmitBuffer[ulBufferIndex],&SECM_TxTcpPacket->m_SCommandFromHost.m_SCmdIds,sizeof(S_GRP_UNT_ID));

    ulBufferIndex = ulBufferIndex + sizeof(S_GRP_UNT_ID);

    memcpy(&cTransmitBuffer[ulBufferIndex],&SECM_TxTcpPacket->m_SCommandFromHost.m_UCmdMessage.m_arru8TxData, ulMessageLength);

    ulBufferIndex = ulBufferIndex + ulMessageLength;

    memcpy(&cTransmitBuffer[ulBufferIndex], &SECM_TxTcpPacket->m_u8TCPFooter, DP_FOOTER_SIZE);

    ulBufferIndex = ulBufferIndex + DP_FOOTER_SIZE;

    *ulTxPacketLength = ulBufferIndex;

    return RETURN_SUCCESS;
}


short CQSocketComm::DP_SetTimeOut(SOCKET in_sktConnectSocket,unsigned long in_ulMilliSeconds)
{
    setsockopt(in_sktConnectSocket, SOL_SOCKET, SO_RCVTIMEO,(const char *)&in_ulMilliSeconds, sizeof(int));

    return RETURN_SUCCESS;
}


void CQSocketComm::dp_SocketGetErrMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg)
{
    switch(in_s32ErrVal)
    {
    case 1:
        sprintf(out_pcErrMsg, " are Not Found");
        break;
    default:
        /** Unknown Error Message **/
        sprintf(out_pcErrMsg, "dp_QSocketComm : Unknown Error exists : %d", in_s32ErrVal);
        break;
    }
}


