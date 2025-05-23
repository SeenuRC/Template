#ifndef DP_QSOCKETCOMM_STRUCTURE_H
#define DP_QSOCKETCOMM_STRUCTURE_H

#include "include/dp_types.h"
#include <QTime>
#include "winsock2.h"

//#define	DP_MAX_TCPMSG_SIZE		(20*(1024 * 1024)) /* 12 MB */
#define	DP_MAX_TCPMSG_SIZE              120000 /* 12 MB */
#define	DP_HEADER_SIZE                  4
#define	DP_FOOTER_SIZE                  4
#define	ESM_MAX_TCP_STACK_SIZE			(1*(1024*1024))
#define	ESM_TCPMSG_BUFFER_SIZE			(DP_MAX_TCPMSG_SIZE + 1024)
#define DP_RECEIVER_LRU_MAX_COUNT						2
#define DP_DRX_MAX_BOARD_COUNT						    2
#define	DP_MAX_REMOTEPROGRM_SIZE              100000 /* 10 MB */

#pragma pack(push, 1)
typedef struct
{
    char m_arrcBoardID[13];                                 /*!< Board ID */
    char m_arrcBoardFG[5];                                  /*!< Board FG */
    char m_arrcHWVersion[9];                                /*!< Hardware version */
    char m_arrcSWVersion[9];                                /*!< Software version */
    char m_arrcFPGAVersion[9];                              /*!< FPGA version */
    char m_arrcDDR4Size[10];                                /*!< DDR Size */
    char m_arrcNandFlashSize[10];                           /*!< NAND Flash Size */
    char m_arrcNorFlashSize[10];                            /*!< NOR Flash Size */
    char m_arrcMramSize[10];                                /*!< MRAM Size */
    char m_arrcEepromSize[10];                              /*!< EEPROM Size */
    unsigned int m_uiSerialNo;                              /*!< Serial No. */
    unsigned int m_uiChkSum;                                /*!< Calculated checksum */
}Sdpboardinfo, SBoardInfo, SDP_BOARDINFO;


/**
*      \struct  : SDP_FPGABRD_INFO
*      \brief   This structure contain FPGA board information details.
*
*/

typedef struct _SDPFPGABoardInfo
{
    unsigned short usBrdID;
    unsigned short usBrdVersion;
    unsigned short usTypeID;
    unsigned short usReleaseNum;

} SDP_FPGABRD_INFO;

typedef struct _SDPSPL0220_BOARD_INFO
{
    U16BIT m_u16BoardID;        /*!< It specifies the board ID */
    U16BIT m_u16BrdSerialNo;	/*!< It specifies the board serial number */
    U16BIT m_u16BrdVersion;     /*!< It specifies the board version */
    U8BIT m_u8Checksum;			/*!< It specifies the board information byte XOR checksum */

}SDPSPL0220_BOARD_INFO, *PSDPSPL0220_BOARD_INFO;

typedef struct _SFPGA_READ_WRITE
{
    unsigned long long m_ulRegOffset;
    unsigned long long m_ulData;
    int m_iTestStatus;
}SFPGA_READ_WRITE, *PSFPGA_READ_WRITE;

typedef struct _S_ERROR_MESSAGE
{
    int m_iErrNo;
    char m_arrcMessage[256];
}S_ERROR_MESSAGE, *PS_ERROR_MESSAGE;

/**
*      \struct  : SDiagnostic_info
*      \brief  	This structure contain SFP diagnoistic information details.
*
*/
typedef struct SDiagnostic_info
{
    float m_fTemperature;
    float m_fVoltage;
    float m_fCurrent;
    float m_fTxPower;
    float m_fRxPower;
}SDiagnostic_info;

typedef struct SSFP_TRS_DATA
{
    unsigned long long m_ulTRS1MSBData;
    unsigned long long m_ulTRS1LSBData;
    unsigned long long m_ulTRS2MSBData;
    unsigned long long m_ulTRS2LSBData;
}SSFP_TRS_DATA;

/**
*      \struct  : S_SFPBandWidth_RX
*      \brief  	This structure contain RX Bandwidth details.
*
*/
typedef struct S_SFPBandWidth_RX
{
     unsigned int m_ulRxPackets;
     unsigned int m_ulRxLostpackets;
     unsigned int m_ulRxDataCount;
     unsigned int m_ulRxPacketTime;
     unsigned int m_ulRxPacketSize;
     float m_pfTotalTime;
     float m_pfSpeed;
}S_SFPBandWidth_RX,*PS_SFPBandWidth_RX;
/**
*      \struct  : S_SFPBandWidth_TX
*      \brief   This structure get TX Bandwidth details.
*
*/
typedef struct _S_ATTENUATION_READWRITE
{
    float m_fAttenuationValue;
}S_ATTENUATION_READWRITE, *PS_ATTENUATION_READWRITE;
typedef struct S_SFPBandWidth_TX
{
        unsigned int m_uiTxPackets;
        unsigned int m_uiTxPacketSize;
        unsigned int m_uiTxPacketDelay;
}S_SFPBandwidth_TX;


/**************************************************** Socket Communication Structure **********************************/
typedef struct
{
    time_t m_GmtTime;
}S_DP_SET_SYSTEM_TIME;

typedef struct _STEMP_SENSOR
{
    float fHigherLimit;                                               /*!< Temperature sensor Higher Limit */
    float fLowerLimit;                                               /*!< Temperature sensor Lower limit */
}STEMP_SENSOR, *PSTEMP_SENSOR;

/**
 *\struct    struct SDP_Temp_Limits
 *\brief  	This structure contains the Temperature sensor Upper and lower limit
 *
 */
typedef struct _SDP_TEMP_LIMITS
{
    STEMP_SENSOR sTempSensor[2];
}SDP_TEMP_LIMITS, *PSDP_TEMP_LIMITS;

typedef struct
{
    char m_cErrMsg[250];
    char m_cStatus;
    int m_iHandle;
}SDEVICECONTEXT, *PSDEVICECONTEXT;


/**
 *	\struct : SErrInfo
 *	\brief This structure contains members to hold the ADM voltage limits.
 *
 */
typedef struct _SERRINFO
{
    char m_arrcTestName[30];								/*!<Test Case Name */
    unsigned int m_iIterNum;								/*!<Iteration Number */
    int m_iGloErrNo;										/*!<Global errno Value */
    char m_arrcDPErrString[512];							/*!<Error String */
}SERRINFO, *PSERRINFO;


/****** Communication Structure ******/



typedef struct _SSCRATCHPAD_TEST
{
    unsigned long long m_ulWriteData;
    unsigned long long m_ulReadData;
}SCRATCHPAD_TEST, *PSCRATCHPAD_TEST;

typedef struct _SFPGA_TEMPERATURE
{
    float m_fFPGATemperature;
}SFPGA_TEMPERATURE, *PSFPGA_TEMPERATURE;


typedef struct _SLOCAL_TEMPERATURE
{
    float m_fLocalTemperature;
}SLOCAL_TEMPERATURE, *PSLOCAL_TEMPERATURE;

typedef struct _S_PS_NORFLASH_WR_RD
{
    unsigned long long m_ulAddress;
    char m_arrcData[255];
    unsigned long long m_ulDataCnt;
}S_PS_NORFLASH_WR_RD, *PS_PS_NORFLASH_WR_RD;

typedef struct _S_PS_SD_CARD_SELFTEST
{
    char m_arrcFileName[100];
    char m_arrcDeviceName[10];
    unsigned int m_uiDataCount;
    unsigned char m_ucTestStatus;
}S_PS_SD_CARD_SELFTEST, *PS_PS_SD_CARD_SELFTEST;

typedef struct _S_PS_EEPROM_TEST
{
    unsigned char m_u8RegOffset;
    unsigned char m_u8Data;
}S_PS_EEPROMTEST, *PS_PS_EEPROMTEST;

typedef struct _S_PS_NORFLASH_TEST
{
    SBoardInfo m_SBoardInfo;
    unsigned char m_ucTestStatus;
}S_PS_NORFLASH_TEST, *PS_PS_NORFLASH_TEST;

typedef struct _S_PS_NADNFLASH_TEST
{
    SBoardInfo m_SBoardInfo;
    char 		  m_arrcFileName[100];
    unsigned char m_ucTestStatus;
}S_PS_NANDFLASH_TEST, *PS_PS_NANDFLASH_TEST;

typedef struct _S_PS_DDR4_TEST_STATUS
{
    unsigned char m_ucTestStatus;
}S_PS_DDR4_TEST_STATUS, *PS_PS_DDR4_TEST_STATUS;

typedef struct _S_PS_USB_SELF_TEST
{
    char m_arrcFileName[100];
    char m_arrcDeviceName[10];
    unsigned int m_uiDataCount;
    unsigned char m_ucTestStatus;
}S_PS_USB_SELF_TEST, *PS_PS_USB_SELF_TEST;

typedef struct _S_PL_NOR_FLASH_READ_ID
{
    unsigned long long m_ulDeviceID;
}S_PL_NOR_FLASH_READ_ID, *PS_PL_NOR_FLASH_READ_ID;


typedef struct _S_PL_NOR_FLASH_SECTOR_ERASE
{
    unsigned short m_usSectorNum;
    unsigned short m_usNoofSectors;
}S_PL_NOR_FLASH_SECTOR_ERASE, *PS_PL_NOR_FLASH_SECTOR_ERASE;

typedef struct _S_PL_NOR_FLASH_CHIP_ERASE
{
    unsigned char m_ucDieSelection;
    float 		  m_fTimeTaken;
}S_PL_NOR_FLASH_CHIP_ERASE, *PS_PL_NOR_FLASH_CHIP_ERASE;

typedef struct _S_PL_NOR_FLASH_SINGLE_WR_RD
{
    unsigned long long m_ulAddress;
    unsigned int  m_uiData;
}S_PL_NOR_FLASH_SINGLE_WR_RD, *PS_PL_NOR_FLASH_SINGLE_WR_RD;

typedef struct _S_PL_NOR_FLASH_PAGE_WR_RD
{
    unsigned int m_uiPageNo;
    unsigned short m_usDataCnt;
    unsigned char m_arrcData[256];
}S_PL_NOR_FLASH_PAGE_WR_RD, *PS_PL_NOR_FLASH_PAGE_WR_RD;

typedef struct _S_PLL_SINGLE_WR_RD
{
    unsigned int m_uiAddress;
    unsigned int m_uiData;
}S_PLL_SINGLE_WR_RD, *PS_PLL_SINGLE_WR_RD;

typedef struct _S_PLL_LOCK_STATUS
{
    unsigned char m_ucPLLLockStatus;
}S_PLL_LOCK_STATUS, *PS_PLL_LOCK_STATUS;

typedef struct _S_PPS_ON_OFF_TIME
{
    unsigned char m_ucPPSChannelSel; /* 1 - UART Channel, 2 - TTL Channel */
    unsigned long long m_ulONTime;
    unsigned long long m_ulOFFTime;
}S_PPS_ON_OFF_TIME, *PS_PPS_ON_OFF_TIME;

typedef struct _S_NEMA_DATA
{
    unsigned char m_arrcNEMAData[150];
}S_NEMA_DATA, *PS_NEMA_DATA;

typedef struct _S_SFP_LANE_STS
{
    U32BIT m_u32SFPLaneSts;
}S_SFP_LANE_STS, *PS_SFP_LANE_STS;

typedef struct _S_SFP_PORT_STS
{
    unsigned char m_arrcPortSts[20];
}S_SFP_PORT_STS, *PS_SFP_PORT_STS;

typedef struct _S_SFP_DIAGNOSTIC_INFO
{
    SDiagnostic_info m_sarrSDiagnostic_info[20];
}S_SFP_DIAGNOSTIC_INFO, *PS_SFP_DIAGNOSTIC_INFO;


/**
*\struct	_SDPRDR8188_1G_ETHERNET_CONFIG
*\brief		This structure contains members to hold the 1G Ethernet device configuration details
*
*		The 1G Ethernet device configuration details includes the MAC address, broadcast address, subnet mask address, gateway address, source IP address, destination IP address and port number.
*/
typedef struct _SDPRDR8161_1G_ETHERNET_CONFIG
{
    U32BIT  m_u32IPv4Address;       /*!< This parameter specifies the  IPv4 address. It's contains 4 bytes */
    U8BIT   m_arru8MACAddress[6];       /*!< This parameter specifies the MAC address. It's contains 6 bytes (eg: 00082D583801) */
    U32BIT  m_u32SubnetMaskAddress;     /*!< This parameter specifies the subnet mask address. It's contains 4 bytes */
    U32BIT  m_u32GatewayAddress;        /*!< This parameter specifies the gateway address. It's contains 4 bytes */
    U16BIT  m_u16PortNo;            	/*!< This parameter specifies the Tx/Rx port number */
    U16BIT  m_u16DestPortNo;            	/*!< This parameter specifies the Tx/Rx port number */
    U32BIT  m_u32DestIPv4Address;       /*!< This parameter specifies the destination IPv4 address. It's contains 4 bytes */


}SDPRDR8161_1G_ETHERNET_CONFIG, *PSDPRDR8161_1G_ETHERNET_CONFIG;

typedef struct _SSDPRDR8161_1G_ETHERNET_FPGA_CONFIG
{
    unsigned int m_uiEnableChannel;
    SDPRDR8161_1G_ETHERNET_CONFIG m_sEthernetIPConfig;
}SDPRDR8161_1G_ETHERNET_FPGA_CONFIG, *PSDPRDR8161_1G_ETHERNET_FPGA_CONFIG;

typedef struct _SSDPRDR8161_1G_ETHERNET_FPGA_READBACK
{
    unsigned int m_uiEnableChannel;
    SDPRDR8161_1G_ETHERNET_CONFIG m_sEthernetIPConfig;
}SDPRDR8161_1G_ETHERNET_FPGA_READBACK, *PSDPRDR8161_1G_ETHERNET_FPGA_READBACK;
#if 0
typedef struct SIRIG_B_info
{
        unsigned char m_ucSeconds;
        unsigned char m_ucMinutes;
        unsigned char m_ucHours;
        unsigned short m_usTotalDays;
        unsigned char m_ucYear;
        unsigned long long  m_ulOverallSeconds;
        unsigned long long m_ulStatus;
}SIRIGBinfo;
#endif

typedef struct _S_IRIG_B_info
{
    unsigned char m_ucSeconds;
    unsigned char m_ucMinutes;
    unsigned char m_ucHours;
    unsigned short m_usTotalDays;
    unsigned char m_ucYear;
  unsigned int  m_ulOverallSeconds;
    unsigned int m_ulStatus;

}SIRIGBinfo, *PSIRIGBinfo ;
/**
*\struct	_SDPSPL5838_ETH_BW_TXINFO
*\brief		This structure contains members to hold the  10G Aurora device Tx bandwidth information
*
*		The 10G  Aurora Ethernet Tx bandwidth details includes the packet size, packet delay time in micro sec and number of packets.
*/
typedef struct _SDPRDR8161_AURORA_BW_TXINFO
{
    unsigned int m_uiEnableChannel;
    S_SFPBandwidth_TX m_sSFPTxBandwith;
}SDPRDR8161_AURORA_BW_TXINFO, *PSDPRDR8161_AURORA_BW_TXINFO;

typedef struct _SDPRDR8161_AURORA_TX_SEND
{
    unsigned char ucChannelNo;
    unsigned char ucDataCount;
    unsigned char m_uiTxPackets[4096];

}SDPRDR8161_AURORA_TX_SEND, *PSDPRDR8161_AURORA_TX_SEND;

typedef struct _SDPRDR8161_AURORA_TX_RECIEVE
{
    unsigned char ucChannelNo;
    unsigned char ucDataCount;
    unsigned char m_uiTxPackets[4096];

}SDPRDR8161_AURORA_TX_RECIEVE, *PSDPRDR8161_AURORA_TX_RECIEVE;


/**
*\struct	_SDPSPL5838_ETH_BW_RXINFO
*\brief		This structure contains members to hold the 10G Aurora device Rx bandwidth information
*
*		The 10G Aurora Rx bandwidth details includes the number of packets received, number of packets success, number of packets failed, number of packets user sent, over all time, speed in mbps.
*/
typedef struct _SDPRDR8161_AURORA_BW_RXINFO
{
    unsigned int m_uiEnableChannel;
    unsigned char m_u8busystatus;
    S_SFPBandWidth_RX m_sSFPRxBandwith;
}SDPRDR8161_AURORA_BW_RXINFO, *PSDPRDR8161_AURORA_BW_RXINFO;

typedef struct _S_DP_RDR_RES_BURST_MSG{
    unsigned short m_usCmdMsgID;
    unsigned short m_usSybSysSel[4];
    unsigned int m_uiarrRSGUres[5];
    unsigned int m_uiarrCCMUFres[5];
    unsigned int m_uiarrCMMres[5];
    unsigned int m_uiDTMStartFreq;
    unsigned int m_uiDTMStopFreq;
    unsigned int m_uiDTMStepFreq;
    unsigned int m_uiarrDTMPhaseOffset[8];
    unsigned char m_ucbandwidth;
    unsigned int m_uiPulseWdt;
    unsigned char m_uctransmissiononoff;
    unsigned char m_ucwaveformselection;
    unsigned int m_uiarrDTM6999ReservedField[5];
    unsigned int m_uiarrSTPU0221ReservedField[5];
    unsigned int m_uiarrCSPU5847ReservedField[5];
    unsigned int m_uiarrSPU5844ReservedField[5];
    unsigned int m_uiarrDRMDDSPhaseOffset[4];
    unsigned int m_uiarrDRMDigitalAttenuation[4];
    unsigned int m_uiarrDRM1006ReservedField[4];
    unsigned char m_ucReserved[1167];
}S_DP_RDR_RES_BURST_MSG;


typedef struct _S_DEVICE_OPEN_STS
{
    unsigned char m_ucFPGASts;
    unsigned char m_ucEEPROMSts;
    unsigned char m_ucTempSenSts;
    unsigned char m_ucADMDevSts;
    unsigned char m_ucNORDevSts;
    unsigned char m_ucMRAMSts;
}S_DEVICE_OPEN_STS, *PS_DEVICE_OPEN_STS;

typedef struct
{
    time_t m_GmtTime;
}DP_SET_SYSTEM_TIME;

typedef struct
{
    U8BIT           m_u8BarSelection;
    U8BIT           m_u8BoardNo;
    U8BIT           m_u8szReserved[2];
    unsigned long long   m_ulFPGAOffset;
    unsigned long long   m_ulFPGAData;

}S_CMD_FPGA_WRITE;

typedef struct
{
    U8BIT           m_u8clockmonitorselection;


}S_CMD_CLOCK_MONSTATUS;

typedef struct
{
    U8BIT           m_u8clockmonitorstatus;


}S_RES_CLOCK_MONSTATUS;
typedef struct
{
    U8BIT           m_u8BarSelection;
    U8BIT           m_u8BoardNo;
    U8BIT           m_u8szReserved[2];
    unsigned long long   m_ulFPGAOffset;

}S_CMD_FPGA_READ;

typedef struct _S_DP_RDR_CMD_MSG{
    unsigned short m_usCmdMsgID;
    unsigned short m_usSybSysSel[4];
    unsigned int m_uiarrRSGUres[5];
    unsigned int m_uiarrCCMUFres[5];
    unsigned int m_uiarrCMMres[5];
    unsigned int m_uiDTMStartFreq;
    unsigned int m_uiDTMStopFreq;
    unsigned int m_uiDTMStepFreq;
    unsigned int m_uiarrDTMPhaseOffset[8];
    unsigned char m_ucbandwidth;
    unsigned int m_uiPulseWdt;
    unsigned char m_uctransmissiononoff;
    unsigned char m_ucwaveformselection;
    unsigned int m_uiarrDTM6999ReservedField[5];
    unsigned int m_uiarrSTPU0221ReservedField[5];
    unsigned int m_uiarrCSPU5847ReservedField[5];
    unsigned int m_uiarrSPU5844ReservedField[5];
    unsigned int m_uiarrDRMDDSPhaseOffset[4];
    unsigned int m_uiarrDRMDigitalAttenuation[4];
    unsigned int m_uiarrDRM1006ReservedField[4];
    unsigned char m_ucReserved[1167];
}S_DP_RDR_CMD_MSG;

typedef struct
{
    U8BIT           m_u8BChannelSelection;


}S_CMD_BURSTLOOPBACK;

typedef struct
{
    U8BIT           m_u8BChannelSelection;



}S_CMD_TRSFIFOREAD;



typedef struct
{
    unsigned char           m_u8TRSData[1450];
    unsigned int ucDataCount;
    unsigned long long ulLSBData[4096];
 unsigned long long ulMSBData[4096];
    unsigned long long ulNanosecond[4096];
   unsigned int uiNanoDifference[4096];


}S_RES_TRSFIFOREADDATA;

typedef struct _S_DP_RDR_RES_MSG{
    unsigned short m_usResMsgID;
    unsigned short m_usReserved;
    unsigned char m_ucReserved[1336];
    unsigned short m_usarrRespAck[30];
}S_DP_RDR_RES_MSG;


typedef struct _S_DP_RDR_REGMON_CMD_{
    unsigned short m_usCmdMsgID;
    unsigned short m_usSybSysSel[4];
    unsigned int m_uiRegisterOffset;
    unsigned short m_usTxByteCnt;
    union
    {
        unsigned char m_u8TxRegVal[1024];
        unsigned short m_u16TxRegVal[512];
        unsigned int m_uiTxRegVal[256];
    }uTxData;
    unsigned char m_ucReserved[360];
}S_DP_RDR_REGMON_CMD;

typedef struct _S_DPRDR_REGMON_RES_{
    unsigned short m_usResMsgID;
    unsigned short m_usResSybSysSel[4];
    unsigned short m_usRxByteCnt;
    union
    {
        unsigned char m_u8RxRegVal[1024];
        unsigned short m_u16RxRegVal[512];
        unsigned int m_uiRxRegVal[256];
    }uRxData;
    unsigned char m_ucReserved[364];
}S_DPRDR_REGMON_RES;

typedef struct
{
    U16BIT      m_u16GroupId;
    U16BIT      m_u16UnitId;
}S_GRP_UNT_ID;

typedef struct
{
    float						fFirmwareVersion;
    float						fDriverVersion;
    float						fFPGAVersion_DRx[DP_RECEIVER_LRU_MAX_COUNT][DP_DRX_MAX_BOARD_COUNT];
    float						fRF_FrontEnd_Version;

}S_RES_VERSION_DETAILS;


typedef struct
{
    unsigned int						m_sTRSMSBData;
    unsigned int						m_sTRSLSBData;

}S_TRSWRITE;


typedef struct
{
    unsigned long long						m_sTRSMAINMSBData;
    unsigned long long						m_sTRSMAINLSBData;
    unsigned long long						m_sTRSBACKUPMSBData;
    unsigned long long						m_sTRSBACKUPLSBData;

}S_TRSREAD;

typedef struct
{
     char						m_zRemoteProgramFileName[512];
}S_REMOTEPRGRMFILENAME;

typedef struct
{
    U32BIT                              m_u32DataCount;
    U8BIT                                   m_arru8TxData[512 * 1024];
}S_REMOTEPRGRM;

typedef union _U_CMD_MSG_
{
    S_DP_SET_SYSTEM_TIME						m_sSystemDateTime;
    SFPGA_READ_WRITE							m_sFPGAWriteRead;
    SCRATCHPAD_TEST								m_sScratchPadTest;
    S_PS_NORFLASH_WR_RD							m_sNORFlashWriteRead;
    S_PS_NORFLASH_TEST 							m_sNORFlashTest;
    S_PS_NANDFLASH_TEST 						m_sNANDFlashTest;
    S_PS_SD_CARD_SELFTEST						m_sSDCardSelfTest;
    S_PS_USB_SELF_TEST					 		m_sUSBSelfTest;
    S_PL_NOR_FLASH_CHIP_ERASE 					m_sNORFlashChipErase;
    S_PL_NOR_FLASH_SECTOR_ERASE 			   	m_SFlashSectorErase;
    S_PL_NOR_FLASH_SINGLE_WR_RD					m_SFlashSingleWrRd;
    S_PL_NOR_FLASH_PAGE_WR_RD 					m_SFlashPageWrRd;
    S_PLL_SINGLE_WR_RD 							m_sPLLWriteRead;
    S_PPS_ON_OFF_TIME 							m_sPPS_ON_OFF_Time;
    S_PS_EEPROMTEST                             m_sEEPROMTest;
    S_DP_RDR_CMD_MSG                            m_sBurstCommand;
    SDPRDR8161_AURORA_BW_TXINFO              m_sAurorTxBandwidth;
    S_TRSWRITE                               m_sTRSWrite;
    SDPRDR8161_AURORA_TX_SEND                   m_sAuroraTXSend;
    S_CMD_BURSTLOOPBACK                         m_sBurstLoopbacksel;
    S_CMD_TRSFIFOREAD						m_sTRSFIFOChannelSel;
    S_CMD_CLOCK_MONSTATUS                   m_sClockMonSelection;
    S_DP_RDR_REGMON_CMD                         m_sRegCommand;
    S_ATTENUATION_READWRITE					m_sAttenuationWrite;
    S_REMOTEPRGRMFILENAME                   m_sRemoteProgramFileName;
    S_REMOTEPRGRM                    m_sRemoteProgram  ;
    SDPRDR8161_1G_ETHERNET_FPGA_CONFIG     m_sEthernet_IPFPGAConfig;
    SDPRDR8161_1G_ETHERNET_FPGA_READBACK     m_sEthernet_IPREADBACK;
    U8BIT                                   m_arru8TxData[DP_MAX_TCPMSG_SIZE];
}U_CMD_MESSAGE;



typedef struct
{
    S_GRP_UNT_ID            m_SCmdIds;
    U_CMD_MESSAGE           m_UCmdMessage;
    S_ERROR_MESSAGE			m_sErrMsg;


}S_COMMAND_FROM_HOST;

typedef struct _S_CMD_PACKET_
{
    U8BIT               m_u8TCPHeader[DP_HEADER_SIZE];
    unsigned int		m_ulMessageLength;
    S_COMMAND_FROM_HOST	m_SCommandFromHost;
    U8BIT               m_u8TCPFooter[DP_FOOTER_SIZE];

}S_DP_TX_TCP_PACKET;
/**********************************************************************************************************/


typedef union _U_RESPONSE_MSG_
{
    S_RES_VERSION_DETAILS						m_SResVersionDetails;
    S_DEVICE_OPEN_STS							m_sDeviceOpenSts;
    SDP_FPGABRD_INFO							m_sFPGABoardDetails;
    SFPGA_READ_WRITE							m_sFPGAWriteRead;
    SCRATCHPAD_TEST								m_sScratchPadTest;
    SFPGA_TEMPERATURE 							m_sReadFPGATemp;
    S_PS_NORFLASH_WR_RD							m_sNORFlashWriteRead;
    S_PS_NORFLASH_TEST 							m_sNORFlashTest;
    S_PS_NANDFLASH_TEST 						m_sNANDFlashTest;
    S_PS_SD_CARD_SELFTEST						m_sSDCardSelfTest;
    S_PS_EEPROMTEST                             m_sEEPROMTest;
    S_PS_DDR4_TEST_STATUS 						m_sDDR4TestSts;
 S_PS_USB_SELF_TEST				        m_sUSBSelfTest;
    S_PL_NOR_FLASH_READ_ID					m_sNorFlashDeviceID;
    S_PL_NOR_FLASH_CHIP_ERASE 				m_sNORFlashChipErase;
    S_PL_NOR_FLASH_SINGLE_WR_RD				m_SFlashSingleWrRd;
    S_PL_NOR_FLASH_PAGE_WR_RD 				m_SFlashPageWrRd;
    S_PLL_SINGLE_WR_RD 						m_sPLLWriteRead;
    S_PLL_LOCK_STATUS  						m_sPLLLockStatus;
    S_PPS_ON_OFF_TIME 						m_sPPS_ON_OFF_Time;
    S_NEMA_DATA 							m_sNEMA_Data;
    S_SFP_LANE_STS  						m_sSFPLaneStatus;
    S_SFP_PORT_STS  						m_sSFP_Port_Status;
    S_SFP_DIAGNOSTIC_INFO 					m_sSFP_Diagnostic_Info;
    S_DP_RDR_RES_MSG                            m_sBurstResponse;
    SDPRDR8161_AURORA_BW_RXINFO                m_sRxBandwidth;
    S_DPRDR_REGMON_RES                          m_sRegResponse;
    SDPRDR8161_AURORA_TX_RECIEVE                m_sAuroraTxRecieve;
    S_DP_RDR_RES_BURST_MSG                      m_sBurstLoopbackResponse;
    SIRIGBinfo                                  m_sIRIGBData;
    S_TRSREAD                                   m_sTRSRead;
    SLOCAL_TEMPERATURE                          m_sLocalTemperatureRead;
    S_RES_TRSFIFOREADDATA							m_sTRSFIFORead;
    S_RES_CLOCK_MONSTATUS                           m_sClockMonStatus;
    S_ATTENUATION_READWRITE					m_sAttenuationRead;
    SDPRDR8161_1G_ETHERNET_FPGA_READBACK     m_sEthernet_IPREADBACK;

    U8BIT                                       m_arru8RxData[DP_MAX_TCPMSG_SIZE];
}U_RES_MESSAGE;

/***************************************Structure for ECM Response*****************************************/
typedef struct
{
    S_GRP_UNT_ID	m_SResIds;
    U_RES_MESSAGE	m_UResMessage;
}S_RESPONSE_TO_HOST;

typedef struct _S_RESPONSE_PACKET_
{
    U8BIT               m_u8szTCPHeader[DP_HEADER_SIZE];
    S16BIT				m_s16MsgStatus;
    S16BIT				m_s16Reserved;
    unsigned int		m_ulMessageLength;
    S_RESPONSE_TO_HOST	m_SResponseToHost;
    U8BIT               m_u8szTCPFooter[DP_FOOTER_SIZE];

}S_DP_RX_TCP_PACKET;
/**********************************************************************************************************/

#pragma pack(pop)
#endif // DP_QSOCKETCOMM_STRUCTURE_H
