#ifndef MACROS
#define MACROS

#include <string.h>
#include <error.h>

#define DATABASE        qApp->applicationDirPath()+"/config/configurations.db"
#define DB_CONN         "DB_Conn"
#define DB_SCRIPT       ":/script/script/dbscript.cfg"

#define DP_RFSS_7121_ZERO                0
#define DP_RFSS_7121_ONE                 1
#define DP_RFSS_7121_TWO                 2
#define DP_RFSS_7121_THREE               3

#define	DP_RFSS_7121_SUCCESS             0
#define DP_RFSS_7121_FAILURE            -1


#define	DP_RFSS_7121_PASS                0
#define DP_RFSS_7121_FAIL                1
#define DPESM1119_SWR_MODE             9
#define DPRFSS_SG_KY_IDX                3
#define DP_RFSS_7121_BASE0_REG                      0x00


#define DP_RFSS_7121_SPECTRUM_MODE						1
#define DP_RFSS_7121_NOISEFIGURE_MODE						219

#define DP_ATE_ZERO_F                   0.0f
#define DP_ATE_ZERO_D                   0.0

#define DP_RFSS_7121_APP_VERSION_MINOR     4
#define DP_RFSS_7121_APP_VERSION_MAJOR     0

#define APP_MAJOR_NUM                   1
#define APP_MINOR_NUM                   0

#define PRINTLOG_TYPE_SUCCESS           1
#define PRINTLOG_TYPE_FAILURE           2
#define PRINTLOG_TYPE_INFO              3

#define ANTENA_SELECTION                1
#define BITE_INTERNAL_SELECTION         2
#define BITE_EXTERNAL_SELECTION         3
#define BITE_INP_EXTERNAL_SELECTION     4
#define LO_SELECTION_26GHz              5
#define LO_SELECTION_31GHz              6
#define GAIN_EQL_SELECTION              7
#define BOARD_INFO_SECTOR              15
#define MAX_COLMN_INDEX                 3

#define BLI_BOARD_SEL              1
#define BLI_SECTOR_SEL              2
#define MODE_SEL              0
#define CABLE_SEL              3

#define DP_RFSS_7121_MAX_PORTS             255

#define DP_RFSS_7121_PAGE_SIZE             256

#define DP_ATE_RFOUT_ENABLE				1
#define DP_ATE_RFOUT_DISABLE			0

#define DP_PSU_ATE_ARR_SIZE             512

#define SDR_ATE_FAILURE                 -1
#define SDR_ATE_SUCCESS                 0

#define DP_RFSS_7121_PREAMPON             0
#define DPRFSS139_PREAMPOFF             1
#define DP_RFSS_7121_SWR_MODE             9

#define DP_RFSS_7121_ENABLE               1
#define DP_RFSS_7121_DISABLE              0
#define DP_RFSS_7121_CONTINOUS_MODE       3

#define DP_RFSS_7121_MAX_TESTCASES                 7
#define DP_RFSS_7121_MAX_CHANNELS                  3

#define DP_NA_MARKER_SEARCH_MAX                 0
#define DP_NA_MARKER_SEARCH_MIN                 1

#define DP_RFSS_7121_MIN_ATTEN_VALUE              0.0f
#define DP_RFSS_7121_MAX_ATTEN_VALUE              31.5f
#define  DPCRF6779_MIN_FREQ_RANGE                  2000  //MHz
#define  DPCRF6779_MAX_FREQ_RANGE                  40000  //MHz
#define  DPRFSS_7121_ADDR_MIN_VALUE                  0x0
#define  DPRFSS_7121_ADDR_MAX_VALUE                  0x7A

/******************  UART Macros  ******************/
#define DPRFSS7121_PORTNAME_LEN_MAX              6
#define DPRFSS7121_PORT_AVAILABLE                        1
#define DPRFSS7121_PORT_NOTAVAILABLE                     0

#define DP_RFSS_7121_MAX_COMM_PORTS			    256

#define DP_RFSS_7121_MIN_COMM_PORTS                1
#define	DP_RFSS_7121_DPSPL6847_MSG_HEADER_LEN						1
#define	DP_RFSS_7121_DPSPL6847_SIZEVAR_SIZE						1
#define	DP_RFSS_7121_DPSPL6847_ADDRESS_SIZE						1

/** 6847 FPGA Communication Register **/
#define DP_RFSS_7121_DPSPL6847_FPGA_START_FRAME							    0xA5
#define DP_RFSS_7121_DPSPL6847_FPGA_START_FRAME_BULK						0xA1

#define DP_RFSS_7121_DPSPL6847_FPGA_SW_RX_SIZE				1
#define DP_RFSS_7121_DPSPL6847_FPGA_SR_RX_SIZE              4
#define DP_RFSS_7121_DPSPL6847_FPGA_BW_RX_SIZE				1
#define DP_RFSS_7121_DPSPL6847_FPGA_BR_RX_SIZE              258

#define DP_RFSS_7121_PORT_AVAILABLE                1
#define DP_RFSS_7121_PORT_NOTAVAILABLE             0

#define DP_RFSS_7121_UART_RDINTERVAlTIMEOUT			  0xFFFFFFFF
#define DP_RFSS_7121_UART_RDTOTAlTIMEOUTMULTIPLIER       1
#define DP_RFSS_7121_UART_RDTOTAlTIMEOUTCONST            5
#define DP_RFSS_7121_UART_WRTOTAlTIMEOUTMULTIPLIER       1
#define DP_RFSS_7121_UART_WRTOTAlTIMEOUTCONST            5

#define DP_RFSS_7121_DPSPL6847_BRDID_REG          0x0
#define DP_RFSS_7121_DPSPL6847_TYPEID_REG         0x0
#define DP_RFSS_7121_BAND_CONFIG_REG              0x0
#define DP_RFSS_7121_DCA_SEL_REG                  0x0
#define DP_RFSS_7121_DCA_ATTEN_CONFIG_REG         0x0

#define DP_RFSS_7121_DPSPL6847_BRD_ID             0x6847
#define DP_RFSS_7121_DPSPL6847_TYPE_ID            0

#define DPRFSS7121_WRITE                          1
#define DPRFSS7121_READ                           2

#define DP_RFSS_7121_BASE2_REG 0x20
#define DP_RFSS_7121_BASE1_REG 0x10
#define DPRFSS7121_TEMPSENSOR_CONFIG_ADDR              0x09
#define DPRFSS7121_TEMPSENSOR_ADDR                (DP_RFSS_7121_BASE1_REG + 0xC)
#define DPRFSS7121_TEMPSENSOR_DUMMY_ADDR           (DP_RFSS_7121_BASE0_REG + 0xE)
#define DPRFSS7121_TEMP_EXT_LSB_ADDR                   0x10
#define DPRFSS7121_TEMP_EXT_ADDR                   0x1

#define TOTAL_NO_DATA_2to18GHz          16
#define TOTAL_NO_DATA_18to40GHz         22
#define TOTAL_NO_OF_BOARD               12
#define TOTAL_NO_2to18_BITE_DATA        1600
#define TOTAL_NO_18to40_BITE_DATA        2200
#define TOTAL_NO_LO_DATA                62
#define TOTAL_NO_GAIN_EQL_DATA          22

#define BITE_INTERNAL_2to18SECTOR_SELECTION             133
#define BITE_INTERNAL_18to40_BLI_SECTOR_SELECTION       136
#define BITE_INTERNAL_18to40_ADF_SECTOR_SELECTION       137

#define BITE_EXTERNAL_2to18SECTOR_SELECTION             134
#define BITE_EXTERNAL_18to40_SECTOR_SELECTION           138

#define BITE_EXT_SOURCE_2to18SECTOR_SELECTION             135
#define BITE_EXT_SOURCE_18to40_BLI_SECTOR_SELECTION       139
#define BITE_EXT_SOURCE_18to40_ADF_SECTOR_SELECTION       140



#define LO_26GHz_SECTOR_SELECTION                  141
#define LO_31GHz_SECTOR_SELECTION                  142
#define GAIN_EQL_SECTOR_SELECTION                  143


#define TOTAL_NO_BITE_DATA 3800
#define RF_CH1_START_ADDR 0x10
#define RF_CH1_END_ADDR 0x0
#define TOTAL_NO_DATA 0

#define Twoto18GHz_START_ADDR   0x0
#define SECTOR_SIZE 65536

#define DP_SG_MODEL_CNT 3
#define DP_SA_MODEL_CNT 3
#define DP_NA_MODEL_CNT 2


#define ADF_CH1_END_ADDR 0x0
#define ADF_CH1_START_ADDR 0x0

#define START_18to40_ADDR 0x10
#define END_18to40_ADDR 0x10

#define START_BITE_18to40_ADDR 0x464
#define END_BITE_18to40_ADDR 0x464
#define END_BITE_2to18_ADDR 0x464
#define START_BITE_2to18_ADDR 0x464
#define DPRFSS7121_FILE_WRITE                            1
#define DPRFSS7121_FILE_READ                             2

enum DP_RFSS_7121_AUTOTESTCASE_LIST
{
    DP_RFSS_7121_GAIN_MEASUREMENT = 0,
    DP_RFSS_7121_HARMONICS_MEASUREMENT,
    DP_RFSS_7121_P1DB_MEASUREMENT,
    DP_RFSS_7121_NOISEFIGURE_MEASUREMENT,
    DP_RFSS_7121_BITE_POWER_MEASUREMENT,
    DP_RFSS_7121_PHASE_AMPLITUDE_RETENTION,
    DP_RFSS_7121_RF_BITE_ISOLATION,
    DP_RFSS_7121_RF_BLANK_ISOLATION,
    DP_RFSS_7121_FILTER_REJECTION,
    DP_RFSS_7121_VSWR_MEASUREMENT,
};

enum{
    DPRFSS7121_INVALID_HANDLE = -999,	/*!<Invalid Handle*/
    DPRFSS7121_INVALID_POINTER,
    DPRFSS7121_RS232_PORT_BUSY,
    DPRFSS7121_BRD_RD_FAILED,
    DPRFSS7121_BRD_TYPE_ID_FAILED,
    DPRFSS7121_RS232_PORT_OPEN,
    DPRFSS7121_RS232_GETCOMMSTATE,
    DPRFSS7121_RS232_SETCOMMSTATE,
    DPRFSS7121_RS232_GETCOMMTIMEOUT,
    DPRFSS7121_RS232_SETCOMMTIMEOUT,
    DPRFSS7121_RS232_PORT_CLOSE,
    DPRFSS7121_UART_BUFFCLR_FAIL,
    DPRFSS7121_WRITE_FAIL,
    DPRFSS7121_READ_FAIL,
    DP_RFSS_7121_ERR_CHKSUM,
    DPRFSS7121_ERR_INCORRECT_SIZE,
    DP_RFSS_7121_ERR_UART_TIMEOUT,
    DPRFSS7121_DPSPL6847_INCORRECT_BRDID,
    DPRFSS7121_DPSPL6847_INCORRECT_TYPEID,
    DPRFSS7121_INVALID_RF_BITE_PATHSEL,
    DPRFSS7121_INVALID_FILTER_BYPASS_PATHSEL,
    DPRFSS7121_ERR_BAND_CONFIG_FAIL,
    DPRFSS7121_ERR_INVALID_DCA_SEL,
    DPRFSS7121_ERR_INVALID_ATTEN_DATA_LIMIT,
    DPRFSS7121_ERR_SET_ATTEN_FAIL,
    DPRFSS7121_ERR_WRITE_FAIL,                        //Write Failed
    DPRFSS7121_ERR_READ_FAIL,                         //Read  Failed
    DPRFSS7121_ERR_FREQUENCY,                         //Read  Failed
    DPRFSS7121_ERR_VCO_WRITE,
    DPRFSS7121_ERR_DIVIDER_WRITE,
    DPRFSS7121_ERR_NDIVIDER_WRITE,
    DPRFSS7121_ERR_DENOMINATOR_R38_WRITE,
    DPRFSS7121_ERR_DENOMINATOR_R39_WRITE,
    DPRFSS7121_ERR_NDIV_FRACT_MSB_WRITE,
    DPRFSS7121_ERR_NDIV_FRACT_LSB_WRITE,
    DPRFSS7121_ERR_PLL_OP_ENABLE,
    DPRFSS7121_ERR_PLL_LOCKSTS,
    DPRFSS7121_ERR_INIT_FAILED,
    DPRFSS7121_ERR_PLL_RESET,
    DPRFSS7121_ERR_PLL_RESET_RELEASE,
    DPRFSS7121_ERR_INIT_VCO_CALIB,
    DPRFSS7121_I2C_WRITE_FAIL,
    DPRFSS7121_I2C_READ_FAIL,
    DPRFSS7121_TEMP_INIT_FAILED,
    DPRFSS7121_TEMP_RT_MSB_READ_FAIL,
    DPRFSS7121_TEMP_RT_LSB_READ_FAIL,
    DPRFSS7121_TEMP_WRITE_FAIL,
    DPRFSS7121_TEMP_LT_READ_FAIL

};

#define APP_GET_ERROR_STR(szErrMsg) {\
        if (s32RetVal != 0)\
        {\
                if(s32RetVal > 0)\
                {\
                        strncpy(szErrMsg, strerror(s32RetVal), sizeof(szErrMsg));\
                }\
                else\
                {\
                        DP_RFSS_7121_GetErrorMessage(s32RetVal, szErrMsg, sizeof(szErrMsg));\
                }\
        }\
}

#define DP_RFSS_7121_CELLWIDGET_IPADDR(table, row, col, IPAddr){\
    QWidget *cellWidget = new QWidget();\
    QLineEdit *lineEdit = new QLineEdit(this);\
    QHBoxLayout *HBoxLayout = new QHBoxLayout();\
    \
    lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);\
    \
    lineEdit->setStyleSheet("border-top-left-radius: 5px;border-bottom-left-radius: 5px;border-top-right-radius: 5px;border-bottom-right-radius: 5px;");\
    \
    /*Validation for IP Address*/\
    DP_RFSS_7121_LINEEDIT_IPADDR_VALIDATOR(lineEdit);\
    lineEdit->setText(IPAddr);\
    lineEdit->setEnabled(false);\
    lineEdit->setStyleSheet("QLineEdit { background-color: rgb(75, 75, 75); background-color: rgb(25, 25, 25);    background-color: rgb(222, 236, 250); }");\
    \
    HBoxLayout->setContentsMargins(2,0,2,0);\
    HBoxLayout->setSpacing(10);\
    HBoxLayout->addWidget(lineEdit);\
    \
    cellWidget->setLayout(HBoxLayout);\
    \
    table->setCellWidget(row,col,cellWidget);\
}

#define DP_RFSS_7121_LINEEDIT_IPADDR_VALIDATOR(LineEdit){\
    QString ipRange = "(([ 0]+)|([ 0]*[0-9] *)|([0-9][0-9] )|([ 0][0-9][0-9])|(1[0-9][0-9])|([2][0-4][0-9])|(25[0-5]))";\
    QRegExp ipRegex ("^" + ipRange+ ipRange+ ipRange + ipRange + "$");\
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);\
    LineEdit->setValidator(ipValidator);\
    LineEdit->setInputMask("000.000.000.000");\
}

#define DP_RFSS_7121_CELLWIDGET_IMAGE(table, row, col, ImagePath){\
    QLabel *stslabel = new QLabel();\
    QWidget *cellWidget = new QWidget();\
    QHBoxLayout *layout = new QHBoxLayout();\
\
    stslabel->setPixmap(QPixmap(ImagePath));\
    stslabel->setScaledContents(true);\
    stslabel->setMinimumSize(20, 20);\
    stslabel->setMaximumSize(20, 20);\
    \
    layout->setContentsMargins(0,0,0,0);\
    layout->addWidget(stslabel);\
    \
    cellWidget->setLayout(layout);\
    table->setCellWidget(row, col, cellWidget);\
}


#define  DP_RFSS_7121_VALIDATE_HANDLE(in_hHandle)\
{\
    if(in_hHandle == INVALID_HANDLE_VALUE)\
    return DPRFSS7121_INVALID_HANDLE;\
}

#define DP_RFSS_7121_VALIDATE_POINTER(out_pSDeviceDetails)\
{\
    if(!out_pSDeviceDetails)\
    return DPRFSS7121_INVALID_POINTER;\
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

#define DP_RFSS_7121_VALIDATE_RF_BITE_PATHSEL(in_u8PathSelection)\
{\
    if(in_u8PathSelection > DP_RFSS_7121_ONE)\
    return DPRFSS7121_INVALID_RF_BITE_PATHSEL;\
}

#define DP_RFSS_7121_VALIDATE_FILTER_BYPASS_PATHSEL(in_u8FilterPathSel)\
{\
    if(in_u8PathSelection > DP_RFSS_7121_ONE)\
    return DPRFSS7121_INVALID_FILTER_BYPASS_PATHSEL;\
}

#define DP_RFSS_7121_VALIDATE_DCA_SEL(in_u8DCASelection)\
{\
    if((in_u8DCASelection < DP_RFSS_7121_ONE) || (in_u8DCASelection > DP_RFSS_7121_THREE))\
    return DPRFSS7121_ERR_INVALID_DCA_SEL;\
}

#define DP_RFSS_7121_VALIDATE_ATTEN_DATA_LIMIT(in_fDCAAttenuation)\
{\
    if((in_fDCAAttenuation < DP_RFSS_7121_MIN_ATTEN_VALUE) || (in_u8DCASelection > DP_RFSS_7121_MAX_ATTEN_VALUE))\
    return DPRFSS7121_ERR_INVALID_ATTEN_DATA_LIMIT;\
}
/* RFSSU Macros*/
#define DP_USER_FLASH                   0
#define DP_CONFIG_FLASH                 1
#define DP_FLASH_WR                  1
#define DP_GUI_DISABLE                  0
#define DP_GUI_ENABLE                   1
#define DP_GUI_DISABLE                  0
#define DP_SEC_ERASE                    1
#define DP_CHIP_ERASE                    2

#define DPRFSSU_ZERO                    0
#define DP_RFSS_7121_BASE3_REG                      0x30
#define DPRFSSU_BPI_DATA_WRITE_REG              (DP_RFSS_7121_BASE3_REG + 0x1)
#define DPRFSSU_BPI_DATA_READ_REG               (DP_RFSS_7121_BASE3_REG + 0x2)
#define DPRFSSU_BPI_BUSY_STS_READ_REG           (DP_RFSS_7121_BASE3_REG + 0x5)
#define DPRFSSU_BPI_CTRL_WR_REG                 (DP_RFSS_7121_BASE3_REG + 0x0)
#define DPRFSSU_BPI_BIT_CTRL_WR_REG             (DP_RFSS_7121_BASE3_REG + 0x0)
#define DPRFSSU_FLASH_ADD_LSB_REG				(DP_RFSS_7121_BASE3_REG + 0x3)
#define DPRFSSU_FLASH_ADD_MSB_REG				(DP_RFSS_7121_BASE3_REG + 0x4)
#define DP_RFSS_7121_MODE_REG1                  (DP_RFSS_7121_BASE3_REG + 0x8)
#define DP_RFSS_7121_MODE_REG2                  (DP_RFSS_7121_BASE3_REG + 0x9)
#define DP_RFSS_7121_2to18GHz_DCA_REG           (DP_RFSS_7121_BASE3_REG + 0xE)
#define DP_RFSS_7121_BITE_DCA_REG               (DP_RFSS_7121_BASE3_REG + 0x6)

#define DPRDR8122_CONFIG_FLASH_MAX_DATA            0XFF
#define DPRDR8122_CONFIG_FLASH_MAX_DATA_COUNT      512
#define DPRDR8122_CONFIG_FLASH_MAX_ADDR            0x1FFFFFF
#define DPRDR8122_CONFIG_FLASH_MAX_SECTORS         542
#define DPRDR8122_CONFIG_FLASH_MAX_PAGES           131072

#define DP_RFSS_7121_ENABLE_ZERO_DCA                    (DP_RFSS_7121_BASE3_REG + 0xA)
#define DP_RFSS_7121_ENABLE_FLASH_REG                   (DP_RFSS_7121_BASE3_REG + 0xF)

#define CONFIG_FLASH_ID_DATA_COUNT_REG              2
#define CONFIG_FLASH_RD_FIFO_CLR_REG                0x1


#define CONFIG_FLASH_DUMMY_DATA                     1


#define CONFIG_FLASH_WRITE_FIFO_CLEAR_DATA           1

#define CONFIG_FLASH_WRDS_CMD                       0x4
#define CONFIG_FLASH_WREB_CMD                       0x6
#define CONFIG_FLASH_READ_INS                       0x3
#define CONFIG_FLASH_PAGE_PRGRM_CMD                 0x2

#define CONFIG_FLASH_SECTOR_ERASE_WREB_CMD         0x06
#define CONFIG_FLASH_READ_STS_CMD                   0x5
#define CONFIG_FLASH_BUSY_READ_REG                  0x54
#define CONFIG_FLASH_CHIP_ERASE_CMD                       0X60

#define ENABLE_ZERO_DCA                          0x1
#define DISABLE_ZERO_DCA                         0x0

#define ENABLE_FLASH                        0x1
#define DISABLE_FLASH                       0x0


#define CONFIG_FLASH_RD_FLASHID_CMD                 0x9F
#define DP_RFSS_7121_BASE1_REG                      0x10

#define DPRFSSU_CONFIG_FLASH_ADD_LSB_REG				(DP_RFSS_7121_BASE1_REG + 0x3)
#define DPRFSSU_CONFIG_FLASH_ADD_MSB_REG				(DP_RFSS_7121_BASE1_REG + 0x2)
#define CONFIG_FLASH_FIFO_DATA_WRITE_REG                (DP_RFSS_7121_BASE1_REG + 0x5)
#define CONFIG_FLASH_SINGLE_DATA_WRITE_REG              (DP_RFSS_7121_BASE1_REG + 0x1)
#define CONFIG_FLASH_INSTRUCTION_REG                    (DP_RFSS_7121_BASE1_REG + 0x0)
#define CONFIG_FLASH_INPUT_DATA_CNT_REG                 (DP_RFSS_7121_BASE1_REG + 0x6)
#define CONFIG_FLASH_DATA_READ_REG                      (DP_RFSS_7121_BASE1_REG + 0x4)
#define CONFIG_FLASH_WRITE_FIFO_CLR_REG                 (DP_RFSS_7121_BASE1_REG + 0x7)
#define CONFIG_FLASH_READ_FIFO_DATA_REG                 (DP_RFSS_7121_BASE1_REG + 0x8)
#define CONFIG_FLASH_FIFO_DATA_COUNT_REG                (DP_RFSS_7121_BASE1_REG + 0x6)
#define CONFIG_FLASH_SPI_STS_REG                        (DP_RFSS_7121_BASE1_REG + 0xE)

#define DP_RDR_7121_PLL_CONFIG_REG                      (DP_RFSS_7121_BASE2_REG + 0x1)
#define DP_RDR_7121_BITE_DCA_CONFIG_REG                 (DP_RFSS_7121_BASE3_REG + 0x6)
#define DP_RDR_7121_PLL_CONFIG_CONTROL_REG              (DP_RFSS_7121_BASE2_REG + 0x0)
#define DP_RDR_7121_PLL_MANUAL_CNFG_DATA                    1
#define DP_RDR_7121_PLL_LOCK_STS_REG                      (DP_RFSS_7121_BASE2_REG + 0x5)
#define DP_RDR_7121_26GHz_PLL_LOCK_STS_REG                (DP_RFSS_7121_BASE3_REG + 0xB)
#define DP_RDR_7121_31GHz_PLL_LOCK_STS_REG                (DP_RFSS_7121_BASE3_REG + 0xC)
#define CONFIG_FLASH_MIN_CONFIGFLASH_64KB_SECTORS   1
#define CONFIG_FLASH_MAX_CONFIGFLASH_64KB_SECTORS   542
#define CONFIG_FLASH_MAX_CONFIGFLASH_4KB_SECTORS    32
#define CONFIG_FLASH_MIN_CONFIGFLASH_4KB_SECTORS    1


#define DPRDR8122_USER_FLASH_MAX_DATA            0XFFFF
#define DPRDR8122_USER_FLASH_MAX_ADDR            0x1FFFFFF
#define DPRDR8122_USER_FLASH_MAX_SECTORS         512
#define DPRDR8122_USER_FLASH_MAX_PAGES           1048576
#define DPRDR8122_CONFIG_FLASH_MAX_SECTORSIZE   131072.0
#define DPRFSSU_FLASH_MAX_SECTOR_NUMBER         512
#define DPRFSSU_FLASH_SECTOR_SIZE               65536

#define CONFIG_FLASH_64KB_SECTOR_ERASE_CMD     0xD8
#define CONFIG_FLASH_32KB_SECTOR_ERASE_CMD     0x52
#define CONFIG_FLASH_4KB_SECTOR_ERASE_CMD      0x20



#define DPRFSSU_FLASH_RD_VENDORID_ADD_CMD     0x0000
#define DPRFSSU_FLASH_RD_DEVICEID1_ADD_CMD    0x0001
#define DPRFSSU_FLASH_RD_DEVICEID2_ADD_CMD    0x000E
#define DPRFSSU_FLASH_RD_DEVICEID3_ADD_CMD    0x000F
#define DPRFSSU_FLASH_FIFO_ENABLE_CMD         0x0001
#define DPRFSSU_FLASH_FIFO_DISABLE_CMD        0x0000
#define DPRFSSU_SUCCESS 0
enum{

    FLASHID_RD  = 0,
    FLASH_SINGLE_WRITE,
    FLASH_SINGLE_READ,
    FLASH_FILE_WRITE,
    FLASH_FILE_READ,
    FLASH_SECTOR_ERASE,
    FLASH_CHIP_ERASE,
    FLASH_WR_BRDINFO,
    FLASH_RD_BRDINFO,
    FLASH_CHCKSUM_TEST,
    FLASH_MEM_TEST,
    FLASH_PROGRAM,
    FLASH_GETCHECKSUM

};
enum{
    DPRFSSU_ERR_FLASH_SET_ADDRESS  = 100,
    DPRFSSU_ERR_FLASH_PAGE_WRITE_FAIL,
    DPRFSSU_ERR_SECTOR_ERASE_FAIL,
    DPRFSSU_ERR_FLASH_WRITE_FAIL,
    DPRFSSU_ERR_USER_FLASH_BUSY,
    DPRFSSU_ERR_FLASH_READID_FAIL,
    DPRFSSU_ERR_FLASH_PAGE_READ_FAIL,
    DPRFSSU_ERR_INVALID_HANDLE,
    DPRFSSU_ERR_INVALID_POINTER,
    DPRFSSU_ERR_INVALID_FLASH_ADDRESS,
    DPRFSSU_ERR_INVALID_FLASH_PAGENO,
    DPSPL1003_ERR_INVALID_FLASH_DATACNT,
    DPSPL1003_ERR_INVALID_SECTOR_NO,
    DPSPL1003_ERR_INVALID_NUMOF_SECTORS,
    DPRFSSU_ERR_FLASH_READ_FAIL,
    DPRFSSU_ERR_INVALID_FLASH_DATACNT,
    DPRFSSU_ERR_INVALID_SECTOR_NO,
    DPRFSSU_ERR_FLASH_CHIP_ERASE_FAIL,
    DPRFSSU_ERR_FLASH_TIMEOUT,
    DPRFSSU_ERR_FLASH_READID,
    DPRFSSU_CONFIG_FLASH_SPI_READ_FAILED,
    DPRFSSU_CONFIG_FLASH_BUSY,
    CONFIG_FLASH_ERR_INVALID_NOOFSECTORS
};
#define DP_RETURN_DB_SUCCESS 0
#define DP_RETURN_DB_FAILURE -1
enum{
    DP_ERR_DB_NOT_OPENED = -1999,
    DP_ERR_INVALID_TEST_ID,
    DP_ERR_INVALID_CFN_NUM,
    DP_ERR_QUERY_EXE,
    DP_ERR_FILE_OPEN,
};
#define  DPRFSSU_VALIDATE_HANDLE(in_hHandle)\
{\
    if(!in_hHandle)\
        return DPRFSSU_ERR_INVALID_HANDLE;\
}

#define DPRFSSU_VALIDATE_POINTER(pAddress)\
{\
    if(!pAddress)\
        return DPRFSSU_ERR_INVALID_POINTER;\
}
#define DPRFSSU_VALIDATE_USER_FLASH_ADDRESS(in_u32Address)\
{\
    if(in_u32Address > DPRDR8122_USER_FLASH_MAX_ADDR)\
    {\
        return DPRFSSU_ERR_INVALID_FLASH_ADDRESS;\
    }\
}
#define DPRFSSU_VALIDATE_FLASH_PAGENO(in_u32PageNo)\
{\
    if((in_u32PageNo < 1) && (in_u32PageNo > DPRDR8122_USER_FLASH_MAX_PAGES))\
    {\
        return DPRFSSU_ERR_INVALID_FLASH_PAGENO;\
    }\
}
#define DPRFSSU_VALIDATE_CONFIG_FLASH_PAGENO(in_u32PageNo)\
{\
    if((in_u32PageNo < 1) && (in_u32PageNo > DPRDR8122_CONFIG_FLASH_MAX_PAGES))\
    {\
        return DPRFSSU_ERR_INVALID_FLASH_PAGENO;\
    }\
}
#define DPRFSSU_VALIDATE_FLASH_DATACOUNT(in_u16DataCount)\
{\
    if((in_u16DataCount < 1) && (in_u16DataCount > DPRDR8122_CONFIG_FLASH_MAX_DATA_COUNT))\
    {\
        return DPRFSSU_ERR_INVALID_FLASH_DATACNT;\
    }\
}
#define DPRFSSU_VALIDATE_CONFIG_FLASH_DATACOUNT(in_u16DataCount)\
{\
    if((in_u16DataCount < 1) && (in_u16DataCount > DPRDR8122_CONFIG_FLASH_MAX_DATA_COUNT))\
    {\
        return DPRFSSU_ERR_INVALID_FLASH_DATACNT;\
    }\
}

#define DPRFSSU_FLASH_SECTOR_NO(in_u16SectorNo)\
{\
    if((in_u16SectorNo < 1) && (in_u16SectorNo > DPRFSSU_FLASH_MAX_SECTOR_NUMBER))\
    {\
        return DPRFSSU_ERR_INVALID_SECTOR_NO;\
    }\
}

#define DPRFSSU_FLASH_SECTOT_COUNT(in_u16SectorNo ,in_u16NoOfSector)\
{\
    if(((in_u16SectorNo - 1) + in_u16NoOfSector) > DPRFSSU_FLASH_MAX_SECTOR_NUMBER)\
    {\
        return DPSPL1003_ERR_INVALID_NUMOF_SECTORS;\
    }\
}

#define VALIDATE_CONFIGFLASH_64KB_NOOFSECTORS_RETURN_ON_ERR(SectorNo, NoOfSectors)\
        {\
            if((NoOfSectors < CONFIG_FLASH_MIN_CONFIGFLASH_64KB_SECTORS) || (NoOfSectors > CONFIG_FLASH_MAX_CONFIGFLASH_64KB_SECTORS))\
                return CONFIG_FLASH_ERR_INVALID_NOOFSECTORS;\
            if(((SectorNo - 1) + NoOfSectors) > CONFIG_FLASH_MAX_CONFIGFLASH_64KB_SECTORS)\
                return CONFIG_FLASH_ERR_INVALID_NOOFSECTORS;\
        }

#define VALIDATE_CONFIGFLASH_64KB_SECTORS_RETURN_ON_ERR(SectorNo)\
        {\
            if((SectorNo < CONFIG_FLASH_MIN_CONFIGFLASH_64KB_SECTORS) || (SectorNo > CONFIG_FLASH_MAX_CONFIGFLASH_64KB_SECTORS))\
                return CONFIG_FLASH_ERR_INVALID_NOOFSECTORS;\
        }

#define VALIDATE_CONFIGFLASH_32KB_NOOFSECTORS_RETURN_ON_ERR(SectorNo, NoOfSectors)\
        {\
            if((NoOfSectors < DPCRF6516_MIN_CONFIGFLASH_32KB_SECTORS) || (NoOfSectors > DPCRF6516_MAX_CONFIGFLASH_32KB_SECTORS))\
                return DPCRF6516_ERR_INVALID_NOOFSECTORS;\
            if(((SectorNo - 1) + NoOfSectors) > DPCRF6516_MAX_CONFIGFLASH_32KB_SECTORS)\
                return DPCRF6516_ERR_INVALID_NOOFSECTORS;\
        }

#define VALIDATE_CONFIGFLASH_32KB_SECTORS_RETURN_ON_ERR(SectorNo)\
        {\
            if((SectorNo < DPCRF6516_MIN_CONFIGFLASH_32KB_SECTORS) || (SectorNo > DPCRF6516_MAX_CONFIGFLASH_32KB_SECTORS))\
                return DPCRF6516_ERR_INVALID_NOOFSECTORS;\
        }

#define VALIDATE_CONFIGFLASH_4KB_NOOFSECTORS_RETURN_ON_ERR(SectorNo, NoOfSectors)\
        {\
            if((NoOfSectors < CONFIG_FLASH_MIN_CONFIGFLASH_4KB_SECTORS) || (NoOfSectors > CONFIG_FLASH_MAX_CONFIGFLASH_4KB_SECTORS))\
                return CONFIG_FLASH_ERR_INVALID_NOOFSECTORS;\
            if(((SectorNo - 1) + NoOfSectors) > CONFIG_FLASH_MAX_CONFIGFLASH_4KB_SECTORS)\
                return CONFIG_FLASH_ERR_INVALID_NOOFSECTORS;\
        }
#define VALIDATE_CONFIGFLASH_4KB_SECTORS_RETURN_ON_ERR(SectorNo)\
        {\
            if((SectorNo < CONFIG_FLASH_MIN_CONFIGFLASH_4KB_SECTORS) || (SectorNo > CONFIG_FLASH_MAX_CONFIGFLASH_4KB_SECTORS))\
                return CONFIG_FLASH_ERR_INVALID_NOOFSECTORS;\
        }

#endif // MACROS

