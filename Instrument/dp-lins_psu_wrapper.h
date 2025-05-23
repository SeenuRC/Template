#ifndef CDP_PSU_WRAPPER_H
#define CDP_PSU_WRAPPER_H

#include <QSerialPort>
#include "./src/DP_UART_Wrapper.h"

typedef struct {
    float fMeasuredVoltage;
    float fSetVoltage;
    float fMeasuredCurrent;
    float fSetCurrent;
    unsigned short usStatusRegister;
    unsigned short usFaultRegister;
}S_DP_PSU_STATUS;

class CDP_PSU_Wrapper
{
private:
    unsigned char m_ucAddress;
    unsigned char m_ucIsDaisyChain;

public:
    QSerialPort m_objqserialportUART;

#define DP_PSU_SUCCESS                                  0
#define DP_PSU_FAIL                                     1

#define DP_PSU_ZERO                                     0
#define DP_PSU_MAX_LEN_READ                             50

#define DP_PSU_GLOBAL_MODE                              2
#define DP_PSU_LOCAL_MODE                               0

#define DP_PSU_OUTPUT_ON                                1
#define DP_PSU_OUTPUT_OFF                               0

#define DP_PSU_DAISY_CHAIN_MODE                         1
#define DP_PSU_NORMAL_MODE                              0

#define DP_PSU_MIN_CURRENT_VAL                          0
#define DP_PSU_MAX_CURRENT_VAL                          12

#define DP_PSU_DELAY_FOR_CONF                           1500

#define DP_PSU_Z_SERIES                                 0
#define DP_PSU_GEN_SERIES                               1

#define DP_PSU_ERR_NO                                   -1000
#define DP_PSU_COM_PORT_NOT_OPENED                      DP_PSU_ERR_NO + 1
#define DP_PSU_CMD_NOT_TRANSMITED                       DP_PSU_ERR_NO + 2
#define DP_PSU_INVALID_BAUDRATE                         DP_PSU_ERR_NO + 3
#define DP_PSU_INVALID_DATABITS                         DP_PSU_ERR_NO + 4
#define DP_PSU_INVALID_PARITY                           DP_PSU_ERR_NO + 5
#define DP_PSU_INVALID_SESSION_DEVICE                   DP_PSU_ERR_NO + 6
#define DP_PSU_ACK_NOT_RECEIVED                         DP_PSU_ERR_NO + 7
#define DP_PSU_INVALID_CURRENT                          DP_PSU_ERR_NO + 8
#define DP_PSU_INVALID_PTR                              DP_PSU_ERR_NO + 9
#define DP_PSU_INVALID_OVER_UNDER_VOLT_INPUT            DP_PSU_ERR_NO + 10
#define DP_PSU_INVALID_VOLT_CURR_INPUT                  DP_PSU_ERR_NO + 11
#define DP_PSU_ERROR_E01                                DP_PSU_ERR_NO + 12
#define DP_PSU_ERROR_E02                                DP_PSU_ERR_NO + 13
#define DP_PSU_ERROR_E04                                DP_PSU_ERR_NO + 14
#define DP_PSU_ERROR_E06                                DP_PSU_ERR_NO + 15
#define DP_PSU_ERROR_E07                                DP_PSU_ERR_NO + 16
#define DP_PSU_ERROR_E08                                DP_PSU_ERR_NO + 17
#define DP_PSU_ERROR_C01                                DP_PSU_ERR_NO + 18
#define DP_PSU_ERROR_C02                                DP_PSU_ERR_NO + 19
#define DP_PSU_ERROR_C03                                DP_PSU_ERR_NO + 20
#define DP_PSU_ERROR_C04                                DP_PSU_ERR_NO + 21
#define DP_PSU_ERROR_C05                                DP_PSU_ERR_NO + 22
#define DP_PSU_INVALID_FLOW_CONTROL                     DP_PSU_ERR_NO + 23

    QString m_qsErrMsg;
    QByteArray m_qbReadData;
    QString m_qsReadData;

    float m_fVoltrating;
    float m_fCurrentRating;

    CDP_PSU_Wrapper(unsigned char in_ucMode = DP_PSU_DAISY_CHAIN_MODE);

    short DP_GetAddress(unsigned char *out_pucAddress);
    short DP_GetDaisyChainMode(unsigned char *out_pucMode);
    short DP_PSU_Open(unsigned char in_ucPortNo, unsigned long in_ulBaudRate, unsigned char in_ucStopBit, unsigned char in_ucDataBits, unsigned char in_ucParity, unsigned short *out_usInstanceID);
    short DP_PSU_Reset(unsigned short in_usInstanceID, unsigned char in_ucLocalorGlobalReset, unsigned char ucAddress);
    short DP_PSU_OV_UV_Config(unsigned short in_usInstanceID, unsigned char ucAddress, unsigned char in_ucSeriesType, float in_fOverVolt, float in_fUnderVolt);
    short DP_PSU_VoltCurrConfig(unsigned short in_usInstanceID, unsigned char ucAddress, unsigned char in_ucLocalorGlobalConfig, float in_fvoltage, float in_fcurrent);
    short DP_PSU_Readback_Config(unsigned short in_usInstanceID, unsigned char ucAddress, float *out_pfVoltage, float *out_pfCurrent, float *out_pfUnderVoltage, float *out_pfOverVoltage );
    short DP_PSU_Output(unsigned short in_usInstanceID, unsigned char ucAddress, unsigned char in_ucLocalorGlobalOutput, unsigned char in_ucOnOff);
    short DP_PSU_Measure_Ouput(unsigned short in_usInstanceID, unsigned char ucAddress, float *out_fVoltage, float *out_fCurrent);
    short DP_PSU_ReadStatus(unsigned short in_usInstanceID, unsigned char ucAddress, S_DP_PSU_STATUS *out_pSReadStatusVal);
    short DP_PSU_GetErrorMessage(short in_sErrorCode, QString *out_pqsErrMsg);
    short DP_PSU_ValidateACK(QByteArray qbaErrorCode);
    short DP_PSU_Command_Transmit(unsigned short in_usInstanceID, QString in_qsWriteData, int in_iDataSize, unsigned char ucAddress);
    short DP_PSU_Command_Reception(unsigned short in_usInstanceID, int in_iDataSizeToRead, QByteArray *out_pqbaWriteData);
    short ConfigCOM1Port(QString in_qsComNo = "COM1", unsigned int in_uiBaudRate = 9600, unsigned char in_ucStopBit = 1, unsigned char in_ucDataBits = 8, unsigned char in_ucParity = 0);
    short DP_PSU_PowerRating(unsigned short in_usInstanceID, unsigned char in_ucPSUaddress, char *in_pzModelNum, float *out_Voltrating, float *out_CurrentRating);
    short SetFOLDEnable(unsigned short in_usInstanceID, bool in_bEnbDis);
    short DP_PSU_Close(unsigned short in_usInstanceID, unsigned char ucAddress);
};

#endif // DP_PSU_WRAPPER_H
