#ifndef DP_RDR_7343_DATASTRUCTURE
#define DP_RDR_7343_DATASTRUCTURE

#include "../include/dp_types.h"
#include "../Instrument/visa.h"
#include "../Instrument/visatype.h"
#include <QString>
#include <QVector>
#pragma pack(push,1)

typedef struct _SDPSPL6985_UART_CONFIG
{
    U8BIT   m_u8PortNo;
    U8BIT	m_s8DataSize;		/*!< UART Data Size */
    U8BIT	m_s8Parity;		/*!< UART Parity */		/*!< UART Parity */
    U8BIT	m_s8StopBit;		/*!< UART Stop Bit */
    U8BIT   m_s8FlowControl;
    U32BIT	m_u32BaudRate;		/*!< UART Baud Rate */
    U32BIT	m_u32TimeOut;		/*!< UART Baud Rate */
    S32BIT m_s32EnaParity;
    S32BIT m_s32Timeout;

} SDP_APP_UART_CONFIG, *PSDP_APP_UART_CONFIG;


/**
*\struct 	: _DP_RS422_WRITE_PKT_
*\brief  	: This structure contains members details to frame single write command packet to FPGA
*
*This structure contains members details to frame single write command packet to FPGA
*
*/
typedef struct _DP_RS422_WRITE_PKT_
{
    unsigned char	ucHeader;
    unsigned char   ucAddress_LSB;
    unsigned char   ucAddress_MSB;
    unsigned char	ucData_LSB;
    unsigned char	ucData_MSB;
    unsigned char	ucChecksum;
}DP_APP_FPGA_Write_PKT;

/**
*\struct 	: _DP_RS422_READ_PKT_
*\brief  	: This structure contains members details to frame single read command packet to FPGA
*
*This structure contains members details to frame single read command packet to FPGA
*
*/
typedef struct _DP_RS422_READ_PKT_
{
    unsigned char	ucHeader;
    unsigned char	ucAddress;
    unsigned char	ucChecksum;
}DP_APP_FPGA_Read_PKT;

/**
*\struct 	: _DP_RS422_BULK_WRITE_PKT_
*\brief  	: This structure contains members details to frame bulk write command packet to FPGA
*
*This structure contains members details to frame bulk write command packet to FPGA*/

typedef struct
{
    char m_szServerIP[50];
    char m_szUID[50];
    char m_szPassword[50];
    char m_szDatabase[50];
    char m_szPortNo[50];
    char m_szODBC_DSNName[50];
    char m_szODBCDriverVer[50];
    char m_szSystemName[50];
} S_DATABASE_CONNECTIVITY;

typedef struct _SDEVICEDETAILS
{
    char            m_arrcIPAddr[20];
    char            m_arrcInsName[50];
    unsigned char    m_uiInstance;
    unsigned char    m_uiStatus;
} SDEVICEDETAILS, *PSDEVICEDETAILS;

typedef struct
{
    unsigned int m_uiPrivilege;
    unsigned char m_ucTestSel;
    bool m_bSysConfig;
    char m_strCurrentUser[50];
    char m_carrCurrentPwd[50];
    char m_strUserID[50];
} S_ATE_GLOBAL_USER_INFO;


typedef struct
{
    /* ATE INSTRUMENTS DETECTION STS*/
    bool m_bSigGen[4];
    bool m_bSA[2];
    bool m_bPM[2];
    bool m_bNA;
    bool m_bPSU;
    bool m_b3PSU;
    bool m_bDSO[2];
    bool m_bRFPM;
    bool m_bFG[2];
    bool m_bNFA;
    bool m_bPG;
    bool m_bAA;

    /*IP or Port Address*/
    char m_cSGInfo[2][50];
    char m_cSAInfo[2][50];
    char m_cNAInfo[2][50];
    char m_cPMInfo[2][50];
    char m_cFGInfo[2][50];
    char m_cDSOInfo[2][50];

} S_ATE_COTS_INST_STS;

typedef struct
{
    ViSession	m_viRFSG[4];
    ViSession	m_viRFSA[2];
    ViSession   m_viRFNA[2];
    ViSession	m_viDSO;
    ViSession	m_viZUPPS;
    ViSession   m_viRFPM;
    ViSession	m_viPG;
    ViSession	m_viNFA;
} S_ATE_HANDLE;

//Board Detection
typedef struct _BRD_DETECT
{

    unsigned char m_ucNAType;
    unsigned char m_ucAAOpenSts;
    unsigned char m_ucNAMode;
    unsigned char m_ucSGCount;
    unsigned char m_ucSACount;
    unsigned char m_ucNACount;
    unsigned char m_ucPMCount;
    unsigned char m_ucFGCount;
    unsigned char m_ucDSOCount;

    char m_szPSUPortNo;
    char m_sz3PSUPortNo;
    char m_szNFAModel[256];

    char m_szSGLName[8][256];
    char m_szSGModel[8][256];
    char m_szSALName[2][256];
    char m_szNALName[2][256];
    char m_szSAModel[2][256];
    char m_szPMLName[2][256];
    char m_szPMModel[2][256];
    char m_szDSOLName[2][256];
    char m_szDSOModel[256];
    char m_szFGLName[2][256];
    char m_szFGModel[256];
    char m_szPGModel[256];
    char m_szPGLName[256];
    char m_szPSUModel[256];
    char m_sz3PSULName[256];
    char m_sz3PSUModel[256];
    char m_szNAConstStr[256];
    char m_szNFAConstStr[256];
    char m_szDSOIpAddr[256];
} S_BRD_DETECTION;


typedef struct
{
    unsigned char m_ucTestSelCnt;
    unsigned char m_ucMajorCycCnt;
    unsigned char m_ucMinorCycCnt[10];
    unsigned char m_ucTestCaseSel[10];
    unsigned int m_uiDelayBwnTest;
    unsigned int m_uiDelayBwnMajCyc;
    unsigned int m_uiDelayBwnMinCyc;
    QString m_strTestName[30];

} SAUTOTESTCONFIG;


typedef struct
{
    unsigned char m_ucResult;
    unsigned char m_ucChannelNo;
    unsigned char m_ucSGSel;
    unsigned char m_ucSGSel_1;
    unsigned char m_ucSASel;
    unsigned char m_ucBrdSel;
    unsigned short m_usSpotNo;
    double m_dRxDCA1;
    double m_dRxDCA2;
    double m_dRFBiteAtten;
    double m_dRFFrequency;
    double m_dADFFrequency;
    double m_dRFPwr;
    double m_dRFCableLoss;
    double m_dADFCableLoss;
    double m_dRFPwrinSG;
    double m_dADFPwrinSG;
    double m_dMeasPwrinSA;
    double m_dIFCableLoss;
    double m_dCalcPwr;
    double m_dExpMinGain;
    double m_dExpMaxGain;
    double m_dCalcGain;
    char m_szInpConnName[256];
    char m_szOutConnName[256];
    char m_szConnector[256];
} S_GAIN_MEASUREMENT;

typedef struct _S_RFSSU_1_GAINMEASUREMENT_SPOTDETAILS
{
    FDOUBLE m_dRFInpFreq;
    FDOUBLE m_dIFDA;
    FDOUBLE m_dIFCALDA;
    U8BIT m_u8GainEql;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dExpMinGain;
    FDOUBLE m_dExpMaxGain;
    QString m_strInpPort;
    QString m_strOutPort;
    U16BIT m_u16BandSel;
    FDOUBLE m_dRFDA;
} S_RFSSU_1_GAINMEASUREMENT_SPOTDETAILS, *PS_RFSSU_1_GAINMEASUREMENT_SPOTDETAILS;



typedef struct _S_SG_SA_INOUT_PARAMETERS_
{
    U8BIT m_u8SigGenSel;
    U8BIT m_u8SpecSel;
    U16BIT m_u16StimulusPathId;
    U16BIT m_u16MeasurementPathID;
    FDOUBLE m_dSpan;
    FDOUBLE m_dRbw;
    FDOUBLE m_dVbw;
    FDOUBLE m_dSweepTime;
    FDOUBLE m_dRefLevel;
    FDOUBLE m_dMarkerPos;
    FDOUBLE m_dInputFreq;
    FDOUBLE m_dCenterFreq;
    FDOUBLE m_dInputPower;
    U64BIT m_u64delay;
    FDOUBLE m_dSGinPower;
    FDOUBLE m_dRFCableloss;
    FDOUBLE m_dMesured_Power;
    FDOUBLE m_dMeasCableloss;
    FDOUBLE m_dCalculated_Power;
} S_SG_SA_INOUT_PARAMETERS, *PS_SG_SA_INOUT_PARAMETERS;


//Global Structure
typedef struct
{
    char		m_carrOutputPath[256];
    S_BRD_DETECTION m_sBrdDetectionConfig;
    S_ATE_COTS_INST_STS sCOTSInstInfo;
    S_ATE_HANDLE sHandles;
    S_ATE_GLOBAL_USER_INFO sGlobalUserInfo;
} SGlobalStucture;


//////////////************end***************/////////////////
#pragma pack(pop)

#endif // DP_RDR_7343_DATASTRUCTURE


