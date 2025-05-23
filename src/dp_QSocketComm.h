#ifndef CQSOCKETCOMM_H
#define CQSOCKETCOMM_H

#define __WINSOCKET__


#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QMutex>

#include "include/dp_types.h"
#include "dp_QSocketComm_Structure.h"
#include <QMessageBox>


/******** COMMANDS for TCP Communication *******************/

#define DP_CMD_BASE						100
#define DP_RES_BASE						100 //Response starts from 200 ==> DP_RES_BASE + DP_CMD_ID ==> DP_RES_BASE + (DP_CMD_BASE + DP_CMD_ID)

#define	DP_CMD_SIG_HANDLER                                      DP_CMD_BASE + 7
#define	DP_CMD_DEVICE_INIT                                      DP_CMD_BASE + 8
#define	DP_CMD_GET_DEVICE_DETAILS                               DP_CMD_BASE + 9
#define	DP_CMD_FPGA_WRITE_REG                                   DP_CMD_BASE + 10
#define	DP_CMD_FPGA_READ_REG                                    DP_CMD_BASE + 11
#define	DP_CMD_FPGA_SCRATCHPAD                                  DP_CMD_BASE + 12
#define	DP_CMD_PS_NORFLASH_WRITE                                DP_CMD_BASE + 13
#define	DP_CMD_PS_NORFLASH_READ                                 DP_CMD_BASE + 14
#define	DP_CMD_PS_NORFLASH_ERASE                                DP_CMD_BASE + 15
#define	DP_CMD_PS_NORFLASH_UPLOADBOARDINFO                      DP_CMD_BASE + 16
#define	DP_CMD_PS_NORFLASH_DATA_RETENSION                       DP_CMD_BASE + 17
#define	DP_CMD_PS_NORFLASH_CHECKSUMTEST                         DP_CMD_BASE + 18
#define	DP_CMD_PS_NANDFLASH_UPLOADBOARDINFO                     DP_CMD_BASE + 19
#define	DP_CMD_PS_NANDFLASH_DATA_RETENSION                      DP_CMD_BASE + 20
#define	DP_CMD_PS_NANDFLASH_CHECKSUM_TEST                       DP_CMD_BASE + 21
#define	DP_CMD_PS_SDCARD_SELFTEST                               DP_CMD_BASE + 22
#define	DP_CMD_PS_DDR4_ADDRESSBUSTEST                           DP_CMD_BASE + 24
#define	DP_CMD_PS_DDR4_DATABUSTEST                              DP_CMD_BASE + 23
#define	DP_CMD_PS_DDR4_DEVICETEST                               DP_CMD_BASE + 25
#define DP_CMD_READ_FPGA_TEMPERATURE                            DP_CMD_BASE + 26
#define	DP_CMD_BURST_MESSAGE                                    DP_CMD_BASE + 57
#define DP_CMD_EEPROM_WRITE                                     DP_CMD_BASE + 58
#define DP_CMD_EEPROM_READ                                      DP_CMD_BASE + 59
#define DP_CMD_PL_NORFLASH_READ_ID				               DP_CMD_BASE + 52
#define DP_CMD_PL_NORFLASH_SECTOR_ERASE                        DP_CMD_BASE + 53
#define DP_CMD_PL_NORFLASH_CHIP_ERASE                           DP_CMD_BASE + 30
#define DP_CMD_PL_NORFLASH_SINGLE_WRITE                         DP_CMD_BASE + 31
#define DP_CMD_PL_NORFLASH_SINGLE_READ                          DP_CMD_BASE + 32
#define DP_CMD_PL_NORFLASH_PAGE_WRITE                           DP_CMD_BASE + 33
#define DP_CMD_PL_NORFLASH_PAGE_READ                            DP_CMD_BASE + 34
#define DP_CMD_PL_NORFLASH_MEMORY_TEST                          DP_CMD_BASE + 35
#define	DP_CMD_PL_DDR4_DATABUSTEST                              DP_CMD_BASE + 36
#define	DP_CMD_PL_DDR4_ADDRESSBUSTEST                           	DP_CMD_BASE + 37
#define	DP_CMD_PL_DDR4_DEVICETEST                               DP_CMD_BASE + 38
#define DP_CMD_PL_PLL_SINGLE_WRITE                              DP_CMD_BASE + 39
#define DP_CMD_PL_PLL_SINGLE_READ                               DP_CMD_BASE + 40
#define DP_CMD_PL_PLL_PAGE_WRITE                                DP_CMD_BASE + 41
#define DP_CMD_PL_PLL_PAGE_READ                                 DP_CMD_BASE + 42
#define DP_CMD_PL_PLL_LOCK_STATUS                               DP_CMD_BASE + 43
#define DP_CMD_PL_PLL_SYNCRONIZATION                            	DP_CMD_BASE + 44
#define DP_CMD_PL_PPS_READ                                      DP_CMD_BASE + 45
#define DP_CMD_PL_READ_NEMA_DATA                                DP_CMD_BASE + 46
#define DP_CMD_SFP_LANE_STATUS                                  DP_CMD_BASE + 47
#define DP_CMD_SFP_PORT_STATUS                                  DP_CMD_BASE + 48
#define DP_CMD_SFP_DIAGNOSTICS_INFO                             DP_CMD_BASE + 49
#define DP_CMD_AURORA_TX_BANDWITH                               DP_CMD_BASE + 50
#define DP_CMD_AURORA_RX_BANDWITH                               DP_CMD_BASE + 51
#define DP_CMD_TRS_WRITE                                            DP_CMD_BASE + 60
#define DP_CMD_TRS_READ                                             DP_CMD_BASE + 61
#define DP_CMD_AURORATX_SEND                                     DP_CMD_BASE + 62
#define DP_CMD_AURORATX_RECIEVE                                   DP_CMD_BASE + 63
#define DP_CMD_AURORA_BANDWIDTH_RESET                                   DP_CMD_BASE + 64
#define DP_CMD_BURSTLOOPBACK                                        DP_CMD_BASE + 65
#define DP_CMD_IRIGB_MONITOR                						DP_CMD_BASE + 66
#define DP_CMD_TRSMAINCHANNEL_ENABLE                                     DP_CMD_BASE + 67
#define DP_CMD_TRSBACKUPCHANNEL_ENABLE                                     DP_CMD_BASE + 68
#define DP_CMD_TRS_BACKUPREAD                                             DP_CMD_BASE + 69
#define DP_CMD_LOCALTEMP_READ                                       DP_CMD_BASE+70
#define	DP_CMD_BURST_MESSAGE_READ                                    DP_CMD_BASE + 71
#define DP_CMD_TRS_FIFO_READ                                             DP_CMD_BASE + 72
#define DP_CMD_TRS_FIFO_READ                                             DP_CMD_BASE + 72
#define DP_CMD_CLOCK_MON                                             DP_CMD_BASE + 73
#define DP_CMD_ATTENUATION_WRITE								      DP_CMD_BASE + 74
#define DP_CMD_ATTENUATION_READ 								      DP_CMD_BASE + 75
#define DP_CMD_REMOTE_PROGRAM 								      DP_CMD_BASE + 76
#define DP_CMD_REMOTE_PROGRAMFILENAME 							DP_CMD_BASE + 77
#define DP_CMD_REMOTE_FORCEPROGRAM 							DP_CMD_BASE + 78
#define DP_CMD_IP_CONFIG 								              DP_CMD_BASE + 79
#define DP_CMD_IP_READBACK 								              DP_CMD_BASE + 80





#define DP_RES_SIG_HANDLER                                      DP_RES_BASE + DP_CMD_SIG_HANDLER
#define DP_RES_DEVICE_INIT                                      DP_RES_BASE + DP_CMD_DEVICE_INIT
#define DP_RES_GET_DEVICE_DETAILS                               DP_RES_BASE + DP_CMD_GET_DEVICE_DETAILS
#define DP_RES_FPGA_WRITE_REG                                   DP_RES_BASE + DP_CMD_FPGA_WRITE_REG
#define DP_RES_FPGA_READ_REG                                    DP_RES_BASE + DP_CMD_FPGA_READ_REG
#define DP_RES_FPGA_SCRATCHPAD                                  DP_RES_BASE + DP_CMD_FPGA_SCRATCHPAD
#define DP_RES_PS_NORFLASH_WRITE                                DP_RES_BASE + DP_CMD_PS_NORFLASH_WRITE
#define DP_RES_PS_NORFLASH_READ                                 DP_RES_BASE + DP_CMD_PS_NORFLASH_READ
#define DP_RES_PS_NORFLASH_ERASE                                DP_RES_BASE + DP_CMD_PS_NORFLASH_ERASE
#define DP_RES_PS_NORFLASH_UPLOADBOARDINFO                      DP_RES_BASE + DP_CMD_PS_NORFLASH_UPLOADBOARDINFO
#define DP_RES_PS_NORFLASH_DATA_RETENSION                       DP_RES_BASE + DP_CMD_PS_NORFLASH_DATA_RETENSION
#define DP_RES_PS_NORFLASH_CHECKSUMTEST                         DP_RES_BASE + DP_CMD_PS_NORFLASH_CHECKSUMTEST
#define DP_RES_PS_NANDFLASH_UPLOADBOARDINFO                     DP_RES_BASE + DP_CMD_PS_NANDFLASH_UPLOADBOARDINFO
#define DP_RES_PS_NANDFLASH_DATA_RETENSION                      DP_RES_BASE + DP_CMD_PS_NANDFLASH_DATA_RETENSION
#define DP_RES_PS_NANDFLASH_CHECKSUM_TEST                       DP_RES_BASE + DP_CMD_PS_NANDFLASH_CHECKSUM_TEST
#define DP_RES_PS_SDCARD_SELFTEST                               DP_RES_BASE + DP_CMD_PS_SDCARD_SELFTEST
#define DP_RES_PS_DDR4_ADDRESSBUSTEST                           DP_RES_BASE + DP_CMD_PS_DDR4_ADDRESSBUSTEST
#define DP_RES_PS_DDR4_DATABUSTEST                              DP_RES_BASE + DP_CMD_PS_DDR4_DATABUSTEST
#define DP_RES_PS_DDR4_DEVICETEST                               DP_RES_BASE + DP_CMD_PS_DDR4_DEVICETEST
#define DP_RES_READ_FPGA_TEMPERATURE                            DP_RES_BASE + DP_CMD_READ_FPGA_TEMPERATURE
#define DP_RES_EEPROM_WRITE                                     DP_CMD_BASE + DP_CMD_EEPROM_WRITE
#define DP_RES_EEPROM_READ                                      DP_CMD_BASE + DP_CMD_EEPROM_READ
#define DP_RES_PL_NORFLASH_READ_ID								DP_RES_BASE + DP_CMD_PL_NORFLASH_READ_ID
#define DP_RES_PL_NORFLASH_SECTOR_ERASE							DP_RES_BASE + DP_CMD_PL_NORFLASH_SECTOR_ERASE
#define DP_RES_PL_NORFLASH_CHIP_ERASE							DP_RES_BASE + DP_CMD_PL_NORFLASH_CHIP_ERASE
#define DP_RES_PL_NORFLASH_SINGLE_WRITE							DP_RES_BASE + DP_CMD_PL_NORFLASH_SINGLE_WRITE
#define DP_RES_PL_NORFLASH_SINGLE_READ							DP_RES_BASE + DP_CMD_PL_NORFLASH_SINGLE_READ
#define DP_RES_PL_NORFLASH_PAGE_WRITE							DP_RES_BASE + DP_CMD_PL_NORFLASH_PAGE_WRITE
#define DP_RES_PL_NORFLASH_PAGE_READ							DP_RES_BASE + DP_CMD_PL_NORFLASH_PAGE_READ
#define DP_RES_PL_NORFLASH_MEMORY_TEST							DP_RES_BASE + DP_CMD_PL_NORFLASH_MEMORY_TEST
#define DP_RES_PL_DDR4_ADDRESSBUSTEST                           DP_RES_BASE + DP_CMD_PL_DDR4_ADDRESSBUSTEST
#define DP_RES_PL_DDR4_DATABUSTEST                              DP_RES_BASE + DP_CMD_PL_DDR4_DATABUSTEST
#define DP_RES_PL_DDR4_DEVICETEST                               DP_RES_BASE + DP_CMD_PL_DDR4_DEVICETEST
#define DP_RES_PL_PLL_SINGLE_WRITE								DP_RES_BASE + DP_CMD_PL_PLL_SINGLE_WRITE
#define DP_RES_PL_PLL_SINGLE_READ 								DP_RES_BASE + DP_CMD_PL_PLL_SINGLE_READ
#define DP_RES_PL_PLL_PAGE_WRITE								DP_RES_BASE + DP_CMD_PL_PLL_PAGE_WRITE
#define DP_RES_PL_PLL_PAGE_READ									DP_RES_BASE + DP_CMD_PL_PLL_PAGE_READ
#define DP_RES_PL_PLL_LOCK_STATUS								DP_RES_BASE + DP_CMD_PL_PLL_LOCK_STATUS
#define DP_RES_PL_PLL_SYNCRONIZATION							DP_RES_BASE + DP_CMD_PL_PLL_SYNCRONIZATION
#define DP_RES_PL_PPS_READ										DP_RES_BASE + DP_CMD_PL_PPS_READ
#define DP_RES_PL_READ_NEMA_DATA								DP_RES_BASE + DP_CMD_PL_READ_NEMA_DATA
#define DP_RES_SFP_LANE_STATUS									DP_RES_BASE + DP_CMD_SFP_LANE_STATUS
#define DP_RES_SFP_PORT_STATUS									DP_RES_BASE + DP_CMD_SFP_PORT_STATUS
#define DP_RES_SFP_DIAGNOSTICS_INFO								DP_RES_BASE + DP_CMD_SFP_DIAGNOSTICS_INFO
#define DP_RES_AURORA_TX_BANDWITH								DP_RES_BASE + DP_CMD_AURORA_TX_BANDWITH
#define DP_RES_AURORA_RX_BANDWITH								DP_RES_BASE + DP_CMD_AURORA_RX_BANDWITH
#define DP_RES_TRS_WRITE                                         DP_RES_BASE+DP_CMD_TRS_WRITE
#define DP_RES_TRS_READ                                          DP_RES_BASE+DP_CMD_TRS_READ
#define DP_RES_AURORATX_SEND                                     DP_RES_BASE + DP_CMD_AURORATX_SEND
#define DP_RES_AURORATX_RECIEVE                                    DP_RES_BASE + DP_CMD_AURORATX_RECIEVE
#define DP_RES_AURORA_BANDWIDTH_RESET                                    DP_RES_BASE + DP_CMD_AURORA_BANDWIDTH_RESET
#define DP_RES_BURSTLOOPBACK                                    DP_RES_BASE + DP_CMD_BURSTLOOPBACK
#define DP_RES_IRIGB_MONITOR                                    DP_RES_BASE + DP_CMD_IRIGB_MONITOR
#define DP_RES_TRSMAINCHANNEL_ENABLE                                     DP_RES_BASE + DP_CMD_TRSMAINCHANNEL_ENABLE
#define DP_RES_TRSBACKUPCHANNEL_ENABLE                                    DP_RES_BASE + DP_CMD_TRSBACKUPCHANNEL_ENABLE
#define DP_RES_TRS_BACKUPREAD                                            DP_RES_BASE + DP_CMD_TRS_BACKUPREAD
#define DP_RES_LOCALTEMP_READ                                            DP_RES_BASE + DP_CMD_LOCALTEMP_READ
#define DP_RES_BURST_MESSAGE_READ                                            DP_RES_BASE + DP_CMD_BURST_MESSAGE_READ
#define DP_RES_TRS_FIFO_READ                                            DP_RES_BASE + DP_CMD_TRS_FIFO_READ
#define DP_RES_CLOCK_MON                                            DP_RES_BASE + DP_CMD_CLOCK_MON
#define DP_RES_ATTENUATION_WRITE								      DP_RES_BASE + DP_CMD_ATTENUATION_WRITE
#define DP_RES_ATTENUATION_READ								      DP_RES_BASE + DP_CMD_ATTENUATION_READ
#define DP_RES_REMOTE_PROGRAM 								      DP_RES_BASE + DP_CMD_REMOTE_PROGRAM
#define DP_RES_REMOTE_PROGRAMFILENAME 							DP_RES_BASE + DP_CMD_REMOTE_PROGRAMFILENAME
#define DP_RES_REMOTE_FORCEPROGRAM 							DP_RES_BASE + DP_CMD_REMOTE_FORCEPROGRAM
#define DP_RES_IP_CONFIG 								              DP_RES_BASE + DP_CMD_IP_CONFIG
#define DP_RES_IP_READBACK 								              DP_RES_BASE + DP_CMD_IP_READBACK






/************************************************* Macros **************************************************************/
#define RETURN_SUCCESS                  0
#define RETURN_FAILURE                  -1
#define ERR_BASE_NUM                    -999
#define QTCP_CONN                       0
#define QUDP_CONN                       1
#define INIT_ZERO                       0
#define DP_ERROR_BUFF_SIZE              256
#define	DP_MSGLEN_SIZE				sizeof(unsigned long)
#define DP_COMM_CMD_GROUP0                      100
#define	DP_RES_SET_ESM_SYSTEM_TIME              74
#define	DP_CMD_SET_ESM_SYSTEM_TIME              73
#define	DP_MSGIDS_SIZE				4
#define	DP_MSGSTS_SIZE				2
#define	DP_MSG_RESERVED_SIZE		2
#define DP_RX_HEADER_STATE			0
#define DP_RX_MSGSTS_STATE			1
#define DP_RX_MSGLEN_STATE			2
#define DP_RX_MSGID_STATE			3
#define DP_RX_MSGDATA_STATE			4
#define DP_RX_FOOTER_STATE			5

#define DP_RX_BYTE_COUNT			1

#define	DP_CMD_RECEIVED				1
#define	DP_CMD_NOTRECED				0

#define DPSPL0220_DDR4_TEST_SUCCESS     1
#define DPSPL0220_DDR4_TEST_FAILURE     0

#define DPSPL5838_01_PRINTLOG_TYPE_SUCCESS   1
#define DPSPL5838_01_PRINTLOG_TYPE_FAILURE   2
#define DPSPL5838_01_PRINTLOG_TYPE_INFO      3

#define DPSPL0220_SDCARD_TEST_SUCCESS   1
#define DPSPL0220_SDCARD_TEST_FAILURE   0


#define DP_CMD_DEFAULT_TIMEOUT_MS       1000
#define DP_CMD_IRIGB_TIMEOUT_MS       1000
#define DP_CMD_TRS_TIMEOUT_MS       10000000



/**********************************************Command and Response ID for DP-SPL-5853**************************************/
#define DP_DPSPL5853_CMD_GROUP1                      101
#define	DP_CMD_5853_WRITE_REG              101
#define	DP_RES_5853_WRITE_REG              102

#define	DP_CMD_5853_READ_REG                    103
#define	DP_RES_5853_READ_REG                    104

#define	DP_CMD_5853_BULKWRITE_REG               105
#define	DP_RES_5853_BULKWRITE_REG               106

#define	DP_CMD_5853_BULKREAD_REG              107
#define	DP_RES_5853_BULKREAD_REG              108
/******************************************************************************************************************/

/************************************************* Enumuration *****************************************************/
enum QSocketErrMessage{
    QTCP_ERR_CONN_FAILED        =   ERR_BASE_NUM,
    QUDP_ERR_CONN_FAILED,
    ERR_IPADDR_INVALID,
    ERR_UNKNOWN_CONN,
    QTCP_ERR_SEND_PACKET,
    QUDP_ERR_SEND_PACKET,
    QTCP_ERR_CONN_STATE,
    QUDP_ERR_CONN_STATE,
    QTCP_ERR_RECIEVE_PACKET,
    QUDP_ERR_RECIEVE_PACKET,
    DP_ESM_APP_ETH_TIMEOUT
};


#define DP_ERR_BASE 							-2000
#define DP_E_UART_PKT_WRITE						(DP_ERR_BASE + 1)
#define DP_E_UART_RESP_TIMEOUT						(DP_ERR_BASE + 2)
#define DP_E_FILE_EXIST						(DP_ERR_BASE + 3)
#define DP_E_FILE_CREATION_FAILED						(DP_ERR_BASE + 4)
#define DP_E_FILE_WRITE_FAILED						(DP_ERR_BASE + 5)

#pragma pack(push,1)
typedef struct
{
    char cvar;
}SDP_SAMPLE;
#pragma pack(pop)

class CQSocketComm : public QObject
{
    Q_OBJECT
public:
    explicit CQSocketComm(QObject *parent = nullptr);
    ~CQSocketComm();

    U8BIT m_u8connFlag;

    SOCKET m_sktConnectSocket;

    void DP_Set0214TimeandDate();

    S16BIT DP_SetTimeOut(SOCKET in_sktConnectSocket,unsigned long in_ulMilliSeconds);

    S16BIT ConnectToServer(QString in_qstrIpAddress, quint16 in_u16port, U8BIT in_u8ConnType);

    void GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_ps8ErrMsg);
    S16BIT ReadPacketFromServer();

    S16BIT DisConnectFromServer();

    int DP_ReadFromServer(/*QTcpSocket *in_pobjClientSocket, */char *cRXData, unsigned long ulReadByteCount, unsigned long ulWaitMilliSeconds);

    short DP_TCPCmdTrans(/*QTcpSocket *ClientSocket, */LPVOID pInputParam, unsigned long in_ulInputParamLen, LPVOID pOutputParam, unsigned long *pOutputParamLen, unsigned long in_ulTimeOut);

    short DP_TCPClientResRev(/*QTcpSocket *ClientSocket, */S_DP_RX_TCP_PACKET *in_SECM_RxTcpPacket, unsigned short in_usTimeOut);

    short DP_FrameTCP_TXBuffer(char *cTransmitBuffer, S_DP_TX_TCP_PACKET *SECM_TxTcpPacket, unsigned long *ulTxPacketLength);

    void dp_SocketGetErrMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg);
public slots:
    S16BIT Slot_TargetConnectionStatus();

    void ErrorHandler(QAbstractSocket::SocketError socketError);

    short DP_EndianSwap(DP_SET_SYSTEM_TIME *pESMsystemTime);

private:
    quint8      m_u8TCPUDPConn;
    QTcpSocket  *m_pobjQTcpSocket;
    QUdpSocket  *m_pobjQUdpSocket;

signals:
    void sig_MsgToPrintActlog(U8BIT, QString);

    void sig_ServerDisconnected(QString);

};

#endif // CQSOCKETCOMM_H
