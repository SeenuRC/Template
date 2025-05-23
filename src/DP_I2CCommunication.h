#ifndef DP_I2CCOMMUNICATION_H
#define DP_I2CCOMMUNICATION_H

#include "../include/dp_types.h"
#include <windows.h>

#include "DP_APP_macros.h"
#include "../include/libMPSSE_i2c.h"

#define MAX_CHANNEL_CNT		5

/****************** I2C ********************/
/****** Validation Data *******/
#define DP_MAX_PORTS                             255
#define DP_MIN_PORTS                             1

#define DP_PORT_AVAILABLE                        1
#define DP_PORT_NOTAVAILABLE                     0

#define DP_VALIDATE_ADDR_MAX_VALUE               0x7A

#define I2C_CLOCK_FACTOR			           1000
/**********************************I2C Macro************************************/
#define DPCRF6594_I2C_SLAVE_ADD			     0x27

#define DPCRF6594_INPUT_BANK_1			     0x08
#define DPCRF6594_INPUT_BANK_2			     0x09
#define DPCRF6594_INPUT_BANK_3			     0x0A
#define DPCRF6594_INPUT_BANK_4			     0x0B
#define DPCRF6594_INPUT_BANK_5			     0x0C

#define DPCRF6594_OUTPUT_BANK_1			     0x18
#define DPCRF6594_OUTPUT_BANK_2			     0x19
#define DPCRF6594_OUTPUT_BANK_3			     0x1A
#define DPCRF6594_OUTPUT_BANK_4			     0x1B
#define DPCRF6594_OUTPUT_BANK_5			     0x1C

/******************************Manual Function*********************************/
#define DPCRF6594_IO_RESET  			     42

 typedef ChannelConfig_t I2C_ChannelConfig;

typedef struct _S_FTDeviceList {
    ULONG Flags;
    ULONG Type;
    ULONG ID;
    ULONG LocId;
    char SerialNumber[16];
    char Description[64];
    DP_DRV_HANDLE ftHandle;
}S_FTDEVICELIST;

typedef struct SDP_I2C_PORTDETAILS
{
    U32BIT m_u32TotalDevices;
    U8BIT m_u8OpenFlag;
    U8BIT m_u8PortCurIndex;
    U8BIT m_u8ClockSelIndex;
    S8BIT m_arrs8SerialNum[50];
    U8BIT m_u8SlaveAddr;

    unsigned char m_u8SerialPortIndex;

    bool	bIsPortOpen;
    unsigned char ucClockSelIndex;
    unsigned char ucSlaveAddr;

    unsigned char u8InputMode;
    unsigned char u8InputStrobe;

}SDP_I2C_PORTDETAILS, *PSDP_I2C_PORTDETAILS;
/******************************* Library Functions *******************************************/
/**
*\brief		 	This function is used to get error message for specified error code value
*
*\param[in]  	in_s32ErrCode		It specifies the error code value
*\param[out] 	out_ps8ErrStringBuf	It specifies the pointer to the error string
*\param[in]  	in_u16BufSize		It specifies the error message length
*
*\retval	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_ERR_INVALID_POINTER is returned if the specified output error message pointer is null
*
*\par Pre-requisite Functions and it's Sequence:
*			This function does not have any pre-requisite function and can be called at any time if required. Usually this function will be called whenever the library function returns with error.
*/
S32BIT STDCALL  DP_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize);

/**
*\brief		 This function is used to find the I2C ports.
*
*\param[out]  out_pu32TotalDevicesFound	 It specifies the pointer to get total devices found
*
*\retval	 ::DP_SUCCESS is returned upon success
*\retval	 ::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*
*\post		 ::DP_GetErrorMessage
*
*\author
*\date
*/
S32BIT STDCALL  DP_I2C_FindTotalDevices(PU32BIT out_pu32TotalDevicesFound);

/**
*\brief			This function is used to open the I2C.
*
*\param[in] 	in_u8DeviceNo    It specifies device number.
*\param[out] 	out_phHandle	  It specifies  the pointer to get the port handle
*
*\retval		::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*\retval		::DPCRF6779_ERR_DEVICE_BUSY is returned if the the device is already opened
*\retval		::DP_SUCCESS is returned upon success
*
*\author
*\date
*/
S32BIT STDCALL  DP_I2C_Open(U8BIT in_u8DeviceNo, DP_DRV_HANDLE out_phHandle);

/**
*\brief         This function is used to close the opened port.
*
*\param[in] 	in_hHandle	It specifies the I2C handle which obtained from open function
*
*\retval		::DPCRF6779_ERR_DEVICE_BUSY Specified device is already open
*\retval	 	::DPCRF6779_ERR_INVALID_HANDLE is returned if the device handle is invalid or null
*\retval		::DP_SUCCESS is returned upon success
*
*\pre		 	::DPCRF6779_Open
*
*\author
*\date
*/
S32BIT STDCALL  DP_I2C_Close(DP_DRV_HANDLE in_hHandle);

/**
*\brief			This function is used to configure the opened I2C port.
*
*\param[in] 	in_hHandle	It specifies the I2C port handle which obtained from I2C open function
*\param[in] 	in_pSI2CConfig It specifies the I2C configuration details
*
*\retval		::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*\retval		::DPCRF6779_ERR_DEVICE_BUSY Specified device is already open
*\retval	 	::DPCRF6779_ERR_INVALID_HANDLE is returned if the device handle is invalid or null
*\retval		::DP_SUCCESS is returned upon success
*
*\author
*\date
*/
S32BIT STDCALL  DP_I2C_ConfigurePort(DP_DRV_HANDLE in_hHandle, I2C_ChannelConfig *in_pSI2CConfig);

/**
*\brief			This function is used to configure the opened I2C port.
*
*\param[in] 	in_hHandle	It specifies the I2C port handle which obtained from I2C open function
*\param[out] 	out_pI2CChannelinfo It specifies the I2C configuration details
*
*\retval		::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*\retval		::DPCRF6779_ERR_DEVICE_BUSY Specified device is already open
*\retval	 	::DPCRF6779_ERR_INVALID_HANDLE is returned if the device handle is invalid or null
*\retval		::DP_SUCCESS is returned upon success
*
*\author
*\date
*/
S32BIT STDCALL  DP_I2C_GetChannelInfo(U8BIT in_u8DeviceNo, FT_DEVICE_LIST_INFO_NODE *out_pI2CChannelinfo);

/**
*\brief     	This function is used to read from the given offset.
*
*\param[in]		 in_hHandle		 It specifies the I2C port handle which obtained from I2C open function
*\param[in]  	 in_u8SlaveAddr  It specifies the slave address
*\param[in]  	 in_u8Address    It specifies the register address
*\param[out]  	 out_pu8Data     It specifies the pointer data to be written
*
*\retval	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_ERR_INVALID_HANDLE is returned if the I2C handle is null.
*\retval	 	::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null.
*
*\pre		 	::DP_I2C_Open
*\post		 	::DP_GetErrorMessage
*
*\author
*\date
*/
S32BIT STDCALL  DP_I2C_ReadReg(DP_DRV_HANDLE in_hHandle, U8BIT in_u8SlaveAddr, U8BIT in_u8Address, PU8BIT out_pu8Data);

/**
*\brief	 	This function is used to write data to given offset.
*
*\param[in]  	in_hHandle      It specifies the I2C port handle which obtained from I2C open function
*\param[in]  	in_u8SlaveAddr  It specifies the slave address
*\param[in]  	in_u8Address    It specifies the register address
*\param[in]  	in_u8Data       It specifies the data to be written
*
*\retval 	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_ERR_INVALID_HANDLE is returned if the I2C handle is null.
*
*\pre		 	::DP_I2C_Open
*\post		 	::DP_GetErrorMessage
*
*\author
*\date
*/
S32BIT STDCALL  DP_I2C_WriteReg(DP_DRV_HANDLE in_hHandle, U8BIT in_u8SlaveAddr, U8BIT in_u8Address, U8BIT in_u8Data);

/**
*\brief     	This function is used to read data from .csv file from the given file path for channel configuration.
*
*\param[in]  	in_pu8FilePath    It specifies the register address
*
*\retval	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*
*\post		 	::DP_GetErrorMessage
*
*\author        Venkatesh S
*\date          27/08/2024
*/
S32BIT STDCALL  DP_Read_Channel_Config(PS8BIT in_ps8FilePath);

/**
*\brief     	This function is used to read configure channel through I2C Communication.
*
*\param[in]  	in_hHandle      It specifies the I2C port handle which obtained from I2C open function.
*\param[in]  	in_u8ChannelNo  It specifies the Channel number to be configured.
*
*\retval	 	::DP_SUCCESS is returned upon success.
*\retval	 	::DP_ERR_INVALID_HANDLE is returned if the I2C handle is null.
*\retval	 	::DP_ERR_INVALID_IOEXP_CHNO is returned if the configuration channel if out of range.
*
*\pre		 	::DP_Read_Channel_Config
*\pre		 	::DP_I2C_Open
*\post		 	::DP_GetErrorMessage
*
*\author        Venkatesh S
*\date          27/08/2024
*/
S32BIT STDCALL  DP_Configure_Channel(DP_DRV_HANDLE in_hHandle, U8BIT in_u8ChannelNo);

#endif // DP_I2CCOMMUNICATION_H
