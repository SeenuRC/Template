#ifndef DP_DP_APP_MACROS
#define DP_DP_APP_MACROS
#include "../include/dp_types.h"
#include <stdio.h>


/*Printlog*/
#define PRINTLOG_TYPE_SUCCESS        1
#define PRINTLOG_TYPE_FAILURE        2
#define PRINTLOG_TYPE_INFO           3

#define DP_SUCCESS                                       0
#define DP_FAILURE                                      -1
#define		DP_SUCCESS								0
#define		DP_FAILURE								-1
#define		DP_SG_MODEL_CNT         2
#define		DP_SA_MODEL_CNT         2
#define		DP_NA_MODEL_CNT         2


#define DP_PORT_AVAILABLE                        1
#define DP_PORT_NOTAVAILABLE                     0

#define DP_MAX_PORTS                             255
#define DP_MIN_PORTS                             1

#define DP_TEST_PASS		0
#define DP_TEST_FAIL		1
#define DP_TEST_STOPPED		2
#define DP_ODTRM_TOTAL_CHANNELS 8

#define DP_NOT_REACHED      2
#define DP_REACHED          0

#define SET         1
#define RESET       0

/*User Privilages*/
#define DP_APP_ATE_ADMIN					0
#define DP_APP_ATE_NORMAL_USER			1
#define	DP_APP_DEVELOPER                  9876
#define	DP_APP_MAXATTEN                  31.5
#define	DP_APP_MAXCALATTEN               15

#define	DP_APP_DP_USER                    8967

/*User Mgmt */
#define DP_APP_ADDUSER					    0
#define DP_APP_UPDATEUSER					1
#define DP_APP_DELETEUSER					2

#define DP_APP_UART_STOP_BIT       1
#define DP_APP_UART_ENABLE_PARITY  1
#define DP_APP_UART_DISABLE_PARITY 0
#define DP_APP_UART_NONE_PARITY    0

#define DP_APP_PORTNAME_LEN_MAX              6
#define DP_APP_PORT_AVAILABLE                        1
#define DP_APP_PORT_NOTAVAILABLE                     0

#define DP_SA_ENABLE_CONTINOUS              1
#define DP_SA_DISABLE_CONTINOUS             0

#define DP_LOG_MODE                     1
#define DP_PHASE_MODE                   2
#define DP_SWR_MODE                     9
#define DP_SWEEPMODE                    0

#define DP_NA_MARKER_SEARCH_MAX         0
#define DP_NA_MARKER_SEARCH_MIN         1
#define DP_SPECTRUM_MODE				1
#define DP_NOISEFIGURE_MODE				219
#define DP_NA_TRIGGER_MODE_CONT         1
#define DP_NA_TRIGGER_MODE_HOLD         0
#define DP_NA_TRIGGER_MODE_SINGLE         2

#define DP_ATE_7343_ENABLE 1
#define DP_ATE_7343_DISABLE 0
#define DP_ATE_CONTINOUS_MODE 3
#define DP_SDR_HARM_COUNT 3

#define DP_CNT_ENABLE 1
#define DP_CNT_DISABLE 0
#define DP_DP_APP_CONTINOUS_MODE 3

#define OPT_DP_APP_TEST_CASE	15//later change

#define DP_APP_EIGHTY 80
#define DP_APP_INIT_VAL 0

#define DP_APP_ZERO                        0
#define DP_APP_ONE                         1
#define DP_APP_TWO                         2
#define DP_APP_THREE                       3
#define DP_APP_FOUR                        4
#define DP_APP_FIVE                        5
#define DP_APP_SIX                         6
#define DP_APP_SEVEN                       7
#define DP_APP_EIGHT                       8
#define DP_APP_NINE                        9
#define DP_APP_TEN                         10


/***********************************/
#define DP_APP_FPGA_START_FRAME							0xA7
#define DP_APP_FPGA_START_FRAME_BULK						0xA1
#define DP_APP_FPGA_TX_WRITE_SIZE_BYTE_MAX				4
#define	DP_APP_FPGA_MAX_RETRY_CNT						3
#define DP_APP_FPGA_TX_WRITE_SIZE_BYTE_MAX				4
#define DP_APP_FPGA_TX_READ_SIZE_BYTE_MAX				258
#define DP_APP_RESET_ENABLE_WRITE     0x1
#define DP_APP_RESET_DISABLE_WRITE    0x0
#define DP_APP_FPGA_SW_RX_SIZE 1

/****************************************/


#define DP_RDR_ZERO                  0
#define DP_RDR_ONE                   1
#define DP_RDR_TWO                   2
#define DP_RDR_THREE                 3
#define DP_RDR_FOUR                  4
#define DP_RDR_FIVE                  5
#define DP_RDR_SIX                   6
#define DP_RDR_SEVEN                 7
#define DP_RDR_EIGHT                 8
#define DP_RDR_NINE                  9
#define DP_RDR_TEN                   10

/***********************************DRx Registers End***********************************************/

#define PRINTLOG_TYPE_SUCCESS        1
#define PRINTLOG_TYPE_FAILURE        2
#define PRINTLOG_TYPE_INFO           3
#define DP_DP_APP_DISABLE              0
#define DP_DP_APP_ENABLE               1

#define DP_APP_ATE_CONTINOUS_MODE      2

#define DP_DP_APP_TEST_FAIL            1
#define DP_DP_APP_TEST_PASS            0
#define DP_DP_APP_TEST_STOPPED         2

#define DP_RES_SUCCESS               0
#define DP_RES_FAILURE              -1


#define DP_UART_COMM_ENA             1
#define DP_UDP_COMM_ENA              2

#define DP_DRV_FPGA_WR               1
#define DP_DRV_FPGA_RD               2

#define DP_APP_ZERO                0
#define DP_APP_ONE                 1
#define DP_APP_TWO                 2
#define DP_APP_THREE               3


#define DPRFSS_TOTAL_BANDS              4

#define DP_APP_MIN_PATHID_SA       1
#define DP_APP_MAX_PATHID_SA       100

#define	DP_APP_PASS                0
#define DP_APP_FAIL                1

#define DP_APP_SPECTRUM_MODE						1
#define DP_APP_NOISEFIGURE_MODE						219

#define DP_ATE_ZERO_F                   0.0f
#define DP_ATE_ZERO_D                   0.0

#define DP_APP_APP_VERSION_MINOR     4
#define DP_APP_APP_VERSION_MAJOR     0

#define APP_MAJOR_NUM                   1
#define APP_MINOR_NUM                   0

#define PRINTLOG_TYPE_SUCCESS           1
#define PRINTLOG_TYPE_FAILURE           2
#define PRINTLOG_TYPE_INFO              3

#define DP_APP_MAX_PORTS             255

#define DP_APP_PAGE_SIZE             256

#define DP_ATE_RFOUT_ENABLE				1
#define DP_ATE_RFOUT_DISABLE			0





#define DP_ENABLE               1
#define DP_DISABLE              0
#define DP_APP_CONTINOUS_MODE       3

#define DP_APP_MAX_TESTCASES                 10
#define DP_APP_MAX_CHANNELS                  3

#define DP_NA_MARKER_SEARCH_MAX                 0
#define DP_NA_MARKER_SEARCH_MIN                 1

#define DP_APP_MIN_ATTEN_VALUE              0.0f
#define DP_APP_MAX_ATTEN_VALUE              31.5f

#define DP_APP_MIN_PATHID                   0
#define DP_APP_MAX_PATHID                   0

/******************  UART Macros  ******************/
#define DP_APP_MAX_COMM_PORTS			    256
#define DP_APP_MIN_COMM_PORTS                1
#define DP_APP_PORT_AVAILABLE                1
#define DP_APP_PORT_NOTAVAILABLE             0



enum{
        DP_ERR_SIG_PRESET = -3999,
    DP_ERR_SPECTRUM_PRESET,
    DP_ERR_SA_REFLEVEL,
    DP_ERR_SA_RBW_VBW,
    DP_ERR_STIM_PATHID,
    DP_ERR_SA_SPAN_CF,
    DP_ERR_FREQ_POWER,
    DP_ERR_RF_ENABLE_OUT,
    DP_ERR_SA_ENA_CNT,
    DP_ERR_SA_DIS_CNT,
    DP_ERR_SA_ACTIVE_MARKER,
    DP_ERR_RF_DISABLE_OUT,
    DP_ERR_MEAS_PATHID
};

enum DP_APP_AUTOTESTCASE_LIST
{
    DP_APP_RX_RETURN_LOSS_MEASUREMENT = 1,
    DP_APP_TX_INPUT_RETURN_LOSS_MEASUREMENT,
    DP_APP_RX_GAIN_MEASUREMENT,
    DP_APP_NOISE_FIGURE_MEASUREMENT,
    DP_APP_TX_OUTPUT_POWER_MEASUREMENT,
    DP_APP_TX_HARMONICS_MEASUREMENT,
    DP_APP_POWER_DROOP_MEASUREMENT,
    DP_APP_PULSE_RISE_FALL_TIME_MEASUREMENT,
    DP_APP_RFIO_RETURN_LOSS_MEASUREMENT,
    DP_APP_POWER_TUNING_MEASUREMENT,
    DP_APP_RX_OUTOFBAND_REJECTION_MEASUREMENT,
    DP_APP_PHASENOISE_MEASUREMENT,
    DP_APP_GAIN_MEASUREMENT_SA,
    DP_APP_IMAGE_REJECTION_MEASUREMENT,
    DP_APP_P1dB_MEASUREMENT,
    DP_APP_VSWR_MEASUREMENT,
    DP_APP_SPURIOUS_MEASUREMENT,
    DP_APP_BITE_ISOLATION_MEASUREMENT,
    DP_APP_BLANK_ISOLATION_MEASUREMENT,
    DP_APP_FILTER_REJECTION_MEASUREMENT,
    DP_APP_PHASESHIFTER_VALIDATION,
    DP_APP_PHASESHIFTER_CALIBRATION
};
enum{
    DP_APP_INVALID_HANDLE = -9999,	/*!<Invalid Handle*/
    DP_APP_INVALID_POINTER,
    DP_APP_INVALID_PARAM,
    DP_APP_RS232_INVALID_PORT_NO,
    DP_APP_RS232_PORT_BUSY,
    DP_APP_RS232_PORT_OPEN,
    DP_APP_RS232_GETCOMMSTATE,
    DP_APP_RS232_SETCOMMSTATE,
    DP_APP_RS232_GETCOMMTIMEOUT,
    DP_APP_RS232_SETCOMMTIMEOUT,
    DP_APP_RS232_PORT_CLOSE,
    DP_APP_WRITE_FAIL,
    DP_APP_READ_FAIL,
    DP_APP_ERR_INCORRECT_SIZE,
    DP_APP_DPSPL6847_INCORRECT_BRDID,
    DP_APP_DPSPL6847_INCORRECT_TYPEID,
    DP_APP_INVALID_RF_BITE_PATHSEL,
    DP_APP_INVALID_FILTER_BYPASS_PATHSEL,
    DP_APP_ERR_BAND_CONFIG_FAIL,
    DP_APP_ERR_INVALID_DCA_SEL,
    DP_APP_ERR_INVALID_ATTEN_DATA_LIMIT,
    DP_APP_ERR_SET_ATTEN_FAIL,
    DP_APP_ERR_INVALID_HANDLE ,
    DP_APP_ERR_INVALID_POINTER,
    DP_APP_ERR_INVALID_PARAM,
    DP_APP_ERR_QUERY_EXE,
    DP_APP_ERR_COM_PORT_NOT_OPENED,
    DP_APP_ERR_READ_PORT_INFO,
    DP_APP_SPI_PORT_BUSY,
    DP_APP_SPI_ERR_CONFIG_PORT,
    DP_APP_SPI_WRITE_FAIL,
    DP_APP_SPI_READ_FAIL,
    DP_APP_ERR_RS232_INVALID_PORT_NO,
    DP_APP_ERR_RS232_PORT_BUSY,
    DP_APP_ERR_OPEN,
    DP_APP_ERR_INVALID_SOCKET,
    DP_APP_SOCKET_CREATE_FAIL,
    DP_APP_ERR_CLOSE,
    DP_APP_CONN_SHUTDOWN,
    DP_APP_ERR_INVALID_HEADER,
    DP_APP_ERR_INVALID_PARITY,
    DP_APP_ERR_INVALID_TIMEOUT,
    DP_APP_ERR_INVALID_CRC,
    DP_APP_ERR_INVALID_RESPKT,
    DP_APP_ERR_INVALID_FOOTER,
    DP_APP_ERR_INVALID_CHECKSUM,
    DP_APP_ERR_INVALID_DATALEN,
    DP_APP_ERR_INVALID_PKT,
    DP_APP_ERR_STATUS_READ,
    DP_APP_ERR_STATUS_TIMEOUT,
    DP_APP_ERR_WRITE_FAIL,
    DP_APP_ERR_BRD_RESET,
    DP_APP_ERR_DEV_DETAILS,
    DP_APP_ERR_FPGA_DETAILS,
    DP_APP_ERR_HEADER_MISMATCH,
    DP_APP_CRC_ERROR,
    DP_APP_DATA_NA_ERROR,
    DP_APP_MSGKIND_ERROR,
    DP_APP_CMD_NOTOK_ERROR,
    DP_APP_READ_ACK_ERROR,
    DP_APP_OP_LOAD_ERROR,
    DP_APP_CMD_ID_ERROR,
    DP_APP_MSG_COUNT_ERROR,
    DP_APP_RESP_ERROR,
    DP_APP_RWR_ERR_BRD_DET_PATH,
    DP_APP_ERR_INVALID_CHANNEL,
    DP_APP_ERR_DB_SERVER_IP,
    DP_APP_ERR_DB_UID,
    DP_APP_ERR_DB_PASSWORD,
    DP_APP_ERR_DB_NAME,
    DP_APP_ERR_PORT_NO,
    DP_APP_ERR_SYSTEM_NAME,
    DP_APP_ERR_OPEN_DB,
    DP_APP_ERR_ENTER_DETAIL,
    DP_APP_ERR_UN_PW,
    DP_APP_ERR_USER_DETAIL,
    DP_APP_ERR_INSERT_UD,
    DP_APP_ERR_USER_ID,
    DP_APP_ERR_INSERT_TR,
    DP_APP_ERR_DB_FILE_PATH,
    DP_APP_INSERT_DETAIL_ERR,
    DP_APP_ERR_VERIFY_DETAILS,
    DP_APP_ERR_READ_USER_DETAILS,
    DR_RDR_7343_ERR_TR,
    DP_APP_ERR_UPDATE_USER_DETAILS,
    DP_APP_ERR_CONN_DATABASE,
    DP_APP_MARIADB_SERVICE_ERROR,
    DP_APP_ERR_ETH_SEND_ERR,
    DP_APP_ERR_ETH_RECV_ERR,
    DP_APP_SOCKET_BIND_ERROR,
    DP_APP_SOCKET_CONN_ERROR,
    DP_APP_TEST_SEL_ERR,
    DP_APP_ERR_SCRATCH_PAD_RD,
    DP_APP_ERR_SCRATCH_PAD_WR,
    DP_APP_ERR_MODE_SEL,
    E_DP_UART_COM_PORT_NOT_OPENED  ,
    E_DP_UART_CMD_NOT_TRANSMITED   ,
    E_DP_UART_INVALID_BAUDRATE     ,
    E_DP_UART_INVALID_DATABITS     ,
    E_DP_UART_INVALID_PARITY       ,
    E_DP_UART_INVALID_FLOW_CONTROL ,
    E_DP_UART_TIMEOUT              ,
    E_DP_UART_INVALID_CHECKSUM     ,
    E_DP_UART_READ_TIMEOUT         ,
    E_DP_UART_RETURNED_FAILURE     ,
    E_DP_UART_INVALID_DATA_LENGTH  ,
    E_DP_UART_INVALID_HEADER       ,
    E_DP_UART_INVALID_UART_TIMEOUT ,
    E_DP_UART_INVALID_ADDRESS      ,
    E_DP_FRAM_ERROR                ,
    E_DP_PARITY_ERROR              ,
    E_DP_CHK_SUM_ERROR             ,
    E_DP_TIME_OUT_ERROR            ,
    E_DP_UART_INVALID_POINTER      ,
    E_DP_DATA_CHK_SUM_ERROR        ,
    E_DP_HEADER_MISMATCH_ERROR     ,
    E_DP_DATA_WRITE_FAILURE        ,
    E_DP_DATA_READ_FAILURE,
    DP_I2C_WRITE_FAIL,
    DP_I2C_READ_FAIL
};

#define DP_SPL_6768_FPGA_BASE_ADDR0_MIN												 0x0
#define		DP_SPL_6768_FPGA_BASE_ADD6_MAX                                           0x6FF

#define DP_SPL_6768_FPGA_DATA_MIN                                                    0x0
#define DP_SPL_6768_FPGA_DATA_MAX                                                    0xFFFF

#define TRM_RD_WR_RETRYCNT                                       5
#define TRM_RD_WR_DELAY_MS                                      10

#define DP_VALIDATE_POINTER(out_pSDeviceDetails)\
{\
    if(!out_pSDeviceDetails)\
    return DP_APP_INVALID_POINTER;\
}
#define DP_VALIDATE_HANDLE(in_hHandle)\
{\
        if(in_hHandle == NULL)\
            return DP_APP_INVALID_HANDLE;\
}

#define DP_LIB_RETURN_ON_ERR(s32RetVal, ErrFlag)\
{\
    S16BIT s16Erc = 0;\
    if(s32RetVal == 0)\
    {\
        s32RetVal = GetLastError();\
        if((s32RetVal & 0x20000000) == 0x20000000)\
        {\
            s16Erc = (S16BIT)(s32RetVal & 0xFFFF);\
            s32RetVal = s16Erc;\
        }\
        ErrFlag = 1;\
    }\
    else\
    {\
        s32RetVal = 0;\
    }\
}

#define  DP_APP_VALIDATE_HANDLE(in_hHandle)\
{\
    if(!in_hHandle)\
    return DP_APP_INVALID_HANDLE;\
    }

#define DP_APP_VALIDATE_POINTER(pAddress)\
{\
    if(!pAddress)\
    return DP_APP_INVALID_POINTER;\
    }

//FPGA Address validation
#define DP_SPL_6768_TRM_VALIDATE_ADDRESS(Variable)\
{\
    if((Variable < DP_SPL_6768_FPGA_BASE_ADDR0_MIN) || (Variable > DP_SPL_6768_FPGA_BASE_ADD6_MAX))\
{\
    return E_DP_SPL_6768_TRM_ADDRESS_INVALID;\
    }\
    }
//FPGA DATA validation
#define DP_SPL_6768_TRM_VALIDATE_DATA(Variable)\
{\
    if((Variable < DP_SPL_6768_FPGA_DATA_MIN) || (Variable > DP_SPL_6768_FPGA_DATA_MAX))\
{\
    return E_DP_SPL_6768_TRM_DATA_INVALID;\
    }\
    }


#endif // DP_DP_APP_MACROS

