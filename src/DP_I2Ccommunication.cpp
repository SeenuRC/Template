#include "windows.h"
#include "DP_APP_macros.h"
#include "DP_I2CCommunication.h"


/**
*\brief		 This function is used to find the I2C ports.
*
*\param[out]  out_pu32TotalDevicesFound	 It specifies the pointer to get total devices found
*
*\retval	 ::DP_SUCCESS is returned upon success
*\retval	 ::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*
*\pre
*\post		 ::DP_GetErrorMessage
*
*\author
*\date
*/
S32BIT STDCALL DP_I2C_FindTotalDevices(PU32BIT out_pu32TotalDevicesFound)
{
    S32BIT s32RetVal                = DP_SUCCESS;
    uint32 uiNoofChannels           = 0;

    DP_VALIDATE_POINTER(out_pu32TotalDevicesFound);

    /*Get total number of connected devices*/
    s32RetVal = I2C_GetNumChannels(&uiNoofChannels);
    *out_pu32TotalDevicesFound = uiNoofChannels;

    return s32RetVal;
}

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
S32BIT STDCALL DP_I2C_Open(U8BIT in_u8DeviceNo, DP_DRV_HANDLE out_phHandle)
{
    S32BIT s32RetVal            = DP_SUCCESS;
    FT_HANDLE hFTHandle         = 0;

    DP_VALIDATE_POINTER(out_phHandle);
    // No need to validate in_strPortName. If it is incorrect then DPRDR8122_I2C_Open will return failure.

    s32RetVal = I2C_OpenChannel(in_u8DeviceNo, &hFTHandle);
    if(s32RetVal == DP_SUCCESS)
    {
        memcpy(out_phHandle, &hFTHandle, sizeof(DP_DRV_HANDLE));
    }

    return s32RetVal;
}

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
S32BIT STDCALL DP_I2C_Close(DP_DRV_HANDLE in_hHandle)
{
    S32BIT s32RetVal        = DP_SUCCESS;

    DP_VALIDATE_HANDLE(in_hHandle);

    s32RetVal = I2C_CloseChannel(in_hHandle);

    return s32RetVal;
}

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
S32BIT STDCALL DP_I2C_ConfigurePort(DP_DRV_HANDLE in_hHandle, I2C_ChannelConfig *in_pSI2CConfig)
{
    S32BIT s32RetVal        = DP_SUCCESS;

    DP_VALIDATE_HANDLE(in_hHandle);

    s32RetVal = I2C_InitChannel(in_hHandle, in_pSI2CConfig);

    return s32RetVal;
}

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
S32BIT STDCALL DP_I2C_GetChannelInfo(U8BIT in_u8DeviceNo, FT_DEVICE_LIST_INFO_NODE *out_pI2CChannelinfo)
{
    S32BIT s32RetVal        = DP_SUCCESS;

    s32RetVal = I2C_GetChannelInfo(in_u8DeviceNo, out_pI2CChannelinfo);

    return s32RetVal;
}

/**
*\brief	 	This function is used to write data to given offset.
*
*\param[in]  	in_hHandle      It specifies the I2C port handle which obtained from I2C open function
*\param[in]  	in_u8SlaveAddr  It specifies the slave address
*\param[in]  	in_u8Address    It specifies the register address
*\param[in]  	in_u8Data       It specifies the data to be written
*
*\retval 	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*\retval	 	::DPCRF6779_ERR_INVALID_OFFSET is returned if offset is out of limit
*\retval	 	::DPCRF6779_ERR_INVALID_WRITEDATA is returned if write data is out of limit
*
*\pre		 	::DPRDR8122_Open
*\post		 	::DP_GetErrorMessage
*
*\author
*\date
*/
S32BIT STDCALL DP_I2C_WriteReg(DP_DRV_HANDLE in_hHandle, U8BIT in_u8SlaveAddr, U8BIT in_u8Address, U8BIT in_u8Data)
{
    S32BIT s32RetVal                = DP_SUCCESS;
    U8BIT ucRS232WriteFrame[2]      = {0};
    uint32 u32BytestoTransfer       = 0;
    uint32 u32BytesTransfered       = 0;

    DP_VALIDATE_HANDLE(in_hHandle);

    u32BytestoTransfer = 2;
    ucRS232WriteFrame[0] = in_u8Address;
    ucRS232WriteFrame[1] = in_u8Data;

    s32RetVal = I2C_DeviceWrite(in_hHandle, in_u8SlaveAddr, u32BytestoTransfer, ucRS232WriteFrame, &u32BytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT);
    if(s32RetVal != DP_SUCCESS)
    {
        return DP_I2C_WRITE_FAIL;
    }

    if((u32BytestoTransfer != u32BytesTransfered) && (s32RetVal == DP_SUCCESS))
    {
        s32RetVal = DP_I2C_WRITE_FAIL;
    }

    return s32RetVal;
}

/**
*\brief     	This function is used to read from the given offset.
*
*\param[in]		 in_hHandle		 It specifies the I2C port handle which obtained from I2C open function
*\param[in]  	 in_u8SlaveAddr  It specifies the slave address
*\param[in]  	 in_u8Address    It specifies the register address
*\param[out]  	 out_pu8Data     It specifies the pointer data to be written
*
*\retval	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*\retval	 	::DPCRF6779_ERR_INVALID_OFFSET is returned if offset is out of limit
*
*\pre		 	::DPCRF6779_Open
*\post		 	::DP_GetErrorMessage
*
*\author
*\date
*/
S32BIT STDCALL DP_I2C_ReadReg(DP_DRV_HANDLE in_hHandle, U8BIT in_u8SlaveAddr, U8BIT in_u8Address, PU8BIT out_pu8Data)
{
    S32BIT s32RetVal            = DP_SUCCESS;
    uint32 u32BytestoTransfer   = 0;
    uint32 u32BytesTransfered   = 0;

    DP_VALIDATE_HANDLE(in_hHandle);
    DP_VALIDATE_POINTER(out_pu8Data);

    u32BytestoTransfer = 1;

    s32RetVal = I2C_DeviceWrite(in_hHandle, in_u8SlaveAddr, u32BytestoTransfer, &in_u8Address, &u32BytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT | I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE);
    if(s32RetVal != DP_SUCCESS)
    {
        return DP_I2C_WRITE_FAIL;
    }

    if((u32BytestoTransfer != u32BytesTransfered) && (s32RetVal == DP_SUCCESS))
    {
        return DP_I2C_WRITE_FAIL;
    }

    u32BytestoTransfer = 1;

    s32RetVal = I2C_DeviceRead(in_hHandle, in_u8SlaveAddr, u32BytestoTransfer, out_pu8Data, &u32BytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT | I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE);
    if((u32BytestoTransfer != u32BytesTransfered) && (s32RetVal == DP_SUCCESS))
    {
        s32RetVal = DP_I2C_READ_FAIL;
    }

    return s32RetVal;
}
