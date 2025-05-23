#include "src/macros.h"
#include "src/mainwindow.h"
#include "DP_SPICommunication.h"

/**
*\brief			This function is used to find the SPI port's.
*
*\param[out] 	out_arru8AvailablePorts	  It specifies the pointer to get the data of available Port's
*
*\retval		::DP_ERR_COM_PORT_NOT_OPENED is returned if the Finding ports Failed
*\retval		::DP_SUCCESS is returned upon success
*
*\post		 	::DP_GetErrorMessage
*
*/
S32BIT STDCALL DP_SPI_FindPorts(PU8BIT out_arru8AvailablePorts)
{
    S32BIT s32RetVal                    = DP_SUCCESS;
    U16BIT usPortCount                  = DP_APP_ZERO;
    DWORD u32TotalDevices               = DP_APP_ZERO;

    FT_DEVICE_LIST_INFO_NODE S_SPI_Channelinfo;

    memset(out_arru8AvailablePorts, DP_PORT_NOTAVAILABLE, DP_MAX_PORTS);

    Init_libMPSSE();

    s32RetVal = SPI_GetNumChannels(&u32TotalDevices);
    if(s32RetVal != DP_SUCCESS)
    {
        return DP_APP_ERR_COM_PORT_NOT_OPENED;
    }
    else
    {
        for(usPortCount=0; usPortCount < u32TotalDevices; usPortCount++)
        {

            if(SPI_GetChannelInfo(usPortCount,&S_SPI_Channelinfo)== DP_SUCCESS)
            {
                out_arru8AvailablePorts[usPortCount] = DP_PORT_AVAILABLE;
            }
        }
    }

    return DP_SUCCESS;
}

/**
*\brief			This function is used to get the opened SPI port information.
*
*\param[in] 	in_u8PortNo	        It specifies the SPI port number which obtained from SPI open function
*\param[in] 	out_pSPIChannelinfo It specifies the SPI configuration details
*
*\retval		::DP_VALIDATE_POINTER is returned if the specified output total devices pointer is null
*\retval		::DP_ERR_READ_PORT_INFO is return if the Port Information Read Failed
*\retval		::DP_SUCCESS is returned upon success
*
*\post		 	::DP_GetErrorMessage
*
*/
S32BIT STDCALL DP_SPI_GetPortInfo(U8BIT in_u8PortNo, FT_DEVICE_LIST_INFO_NODE *out_pSPIChannelinfo)
{
    S32BIT s32RetVal = DP_SUCCESS;

    DP_VALIDATE_POINTER(out_pSPIChannelinfo);

    s32RetVal = SPI_GetChannelInfo(in_u8PortNo, out_pSPIChannelinfo);
    if(s32RetVal != DP_SUCCESS)
    {
        s32RetVal = DP_APP_ERR_READ_PORT_INFO;
    }

    return s32RetVal;
}

/**
*\brief			This function is used to open the SPI port.
*
*\param[in] 	in_u8PortNo   It specifies SPI port number.
*\param[in] 	in_vpHandle   It specifies the SPI port Instance which is to be opened from SPI Find Ports function
*
*\retval		::DP_VALIDATE_POINTER is returned if the specified output total devices pointer is null
*\retval		::DP_RS232_PORT_BUSY is returned if the specified device is already open
*\retval		::DP_SUCCESS is returned upon success
*
*\post		 	::DP_GetErrorMessage
*
*/
S32BIT STDCALL DP_SPI_PortOpen(U8BIT in_u8PortNo, DP_DRV_HANDLE out_vpHandle)
{
    S32BIT s32RetVal = DP_SUCCESS;

    FT_HANDLE hFTHandle = 0;

    DP_VALIDATE_POINTER(out_vpHandle);

    s32RetVal = SPI_OpenChannel(in_u8PortNo, &hFTHandle);
    if(s32RetVal != DP_SUCCESS)
    {
        s32RetVal = DP_APP_SPI_PORT_BUSY;
    }
    else
    {
        memcpy(out_vpHandle, &hFTHandle, sizeof(DP_DRV_HANDLE));
    }

    return s32RetVal;
}

/**
*\brief         This function is used to close the opened SPI port.
*
*\param[in] 	in_vpHandle	        It specifies the SPI port Handle which obtained from SPI open function
*
*\retval		::DP_ERR_DEV_CLOSE is returned if the Specified device is already closed
*\retval	 	::DP_VALIDATE_HANDLE is returned if the device handle is invalid or null
*\retval		::DP_SUCCESS  is returned upon success
*
*\par Pre-requisite Functions and it's Sequence:
*				-# ::DP_SPI_FindPorts( )
*				-# ::DP_SPI_PortOpen( )
*
*\post		 	::DP_GetErrorMessage
*
*/
S32BIT STDCALL DP_SPI_PortClose(DP_DRV_HANDLE in_vpHandle)
{
    S32BIT s32RetVal = DP_SUCCESS;

    DP_VALIDATE_HANDLE(in_vpHandle);

    s32RetVal = SPI_CloseChannel(in_vpHandle);
    if(s32RetVal != DP_SUCCESS)
    {
        s32RetVal = DP_APP_ERR_CLOSE;
    }

    return s32RetVal;
}

/**
*\brief			This function is used to configure the opened SPI port.
*
*\param[in] 	in_vpHandle	     It specifies the SPI port Handle which obtained from SPI open function
*\param[in] 	out_pSSPIConfig  It specifies the SPI configuration details
*
*\retval		::DP_VALIDATE_POINTER is returned if the specified output total devices pointer is null
*\retval		::DP_VALIDATE_HANDLE is returned if the instance ID is invalid
*\retval	 	::DP_ERR_CONFIG_PORT is returned upon failure
*\retval		::DP_SUCCESS is returned upon success
*
* *\par Pre-requisite Functions and it's Sequence:
*				-# ::DP_SPI_FindPorts( )
*				-# ::DP_SPI_PortOpen( )
*
*\post		 	::DP_GetErrorMessage
*
*/
S32BIT STDCALL DP_SPI_PortConfigure(DP_DRV_HANDLE in_vpHandle, SPI_ChannelConfig *out_pSSPIConfig)
{
    S32BIT s32RetVal = DP_SUCCESS;

    DP_VALIDATE_HANDLE(in_vpHandle);
    DP_VALIDATE_POINTER(out_pSSPIConfig);

    s32RetVal = SPI_InitChannel(in_vpHandle, out_pSSPIConfig);
    if(s32RetVal != DP_SUCCESS)
    {
        s32RetVal = DP_APP_SPI_ERR_CONFIG_PORT;
    }

    return s32RetVal;
}

/**
* \brief	 	This function is used to write data to given address through SPI Communication.
*
*\param[in]  	in_hHandle       It specifies the UART port handle which obtained from UART open function
*\param[in]  	in_u16Address    It specifies the register address value
*\param[in]  	in_u8Data        It specifies the data to be written
*
*\retval 	 	::DP_SUCCESS is returned upon success
*\retval	 	::DPCRF6779_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*\retval	 	::DPCRF6779_ERR_INVALID_OFFSET is returned if offset is out of limit
*\retval	 	::DPCRF6779_ERR_INVALID_WRITEDATA is returned if write data is out of limit
*\retval        ::DP_SPI_WRITE_FAIL is returned if SPI Write failed
*
*\par Pre-requisite Functions and it's Sequence:
*				-# ::DP_SPI_FindPorts( )
*				-# ::DP_SPI_PortOpen( )
*               -# ::DP_SPI_PortConfigure( )
*
*\post		 	::DPCRF6779_GetErrorMessage
*
*\author        Anoop Thulaseedharan
*\date          04 January 2022
*/

S32BIT STDCALL DP_LMX2028RegWrite(DP_DRV_HANDLE in_hHandle, PSLMX2028_CONFIG in_pSPLLConfig, U8BIT in_u8CfgCnt)
{
    U8BIT ucSPIWriteFrame[3]            = {DP_APP_ZERO};
    S32BIT s32RetVal                    = DP_APP_ZERO;
    U32BIT u32BytestoTransfer           = DP_APP_ZERO;
    DWORD ulBytesTransfered             = DP_APP_ZERO;

    /*Write*/
    DP_VALIDATE_HANDLE(in_hHandle);
    DP_VALIDATE_POINTER(in_pSPLLConfig);

    /*
    * Enable the Chip Select and Write Register Address into the Slave SPI Device
    */
    
    for(U16BIT u16CfgIdx = 0; u16CfgIdx < in_u8CfgCnt; u16CfgIdx++)
    {
        u32BytestoTransfer = 3;
        /*FIXME MSB First to be checked and updated*/
        ucSPIWriteFrame[0] = (U8BIT)((in_pSPLLConfig[u16CfgIdx].m_u8Address) & 0x7F);   // Address
        ucSPIWriteFrame[1] = (U8BIT)((in_pSPLLConfig[u16CfgIdx].m_u16Data >> 8) & 0xFF);// MSB Data
        ucSPIWriteFrame[2] = (U8BIT)((in_pSPLLConfig[u16CfgIdx].m_u16Data) & 0xFF);     // LSB Data

        s32RetVal = SPI_Write(in_hHandle, ucSPIWriteFrame, u32BytestoTransfer, &ulBytesTransfered, (SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE|SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE));
        if(s32RetVal != DP_SUCCESS)
        {
            return s32RetVal;
        }

        if((u32BytestoTransfer != ulBytesTransfered) && (s32RetVal == DP_SUCCESS))
        {
            return DP_APP_SPI_WRITE_FAIL;
        }
    }

    return s32RetVal;
}

/**
* \brief     	This function is used to read from the given address through SPI Communication.
*
*\param[in]		in_hHandle			It specifies the UART port handle which obtained from UART open function
*\param[in]	 	in_u16Address 		It specifies the register address value
*\param[out] 	out_pu8Data     	It specifies the pointer to get read data
*
*\retval	 	::DP_SUCCESS is returned upon success
*\retval	 	::DPCRF6779_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*\retval	 	::DPCRF6779_ERR_INVALID_OFFSET is returned if offset is out of limit
*\retval        ::DP_SPI_WRITE_FAIL is returned if SPI Write failed
*\retval        ::DP_SPI_READ_FAIL is returned if SPI Read Failed
*
*\par Pre-requisite Functions and it's Sequence:
*				-# ::DP_SPI_FindPorts( )
*				-# ::DP_SPI_PortOpen( )
*               -# ::DP_SPI_PortConfigure( )
*
*\post		 	::DPCRF6779_GetErrorMessage
*
*\author        Anoop Thulaseedharan
*\date          04 January 2022
*/
S32BIT STDCALL DP_LMX2028RegRead(DP_DRV_HANDLE in_hHandle, PSLMX2028_CONFIG inout_pSPLLConfig, U8BIT in_u8CfgCnt)
{
    U8BIT ucSPIWriteFrame[3]            = {DP_APP_ZERO};
    U8BIT ucSPIReadData[2]              = {DP_APP_ZERO};
    S32BIT s32RetVal                    = DP_APP_ZERO;
    U32BIT u32BytestoTransfer           = DP_APP_ZERO;
    DWORD ulBytesTransfered             = DP_APP_ZERO;

    /*Write*/
    DP_VALIDATE_HANDLE(in_hHandle);
    DP_VALIDATE_POINTER(inout_pSPLLConfig);

    /*
    * Enable the Chip Select and Write Register Address into the Slave SPI Device
    */

    for(U16BIT u16CfgIdx = 0; u16CfgIdx < in_u8CfgCnt; u16CfgIdx++)
    {
//        u32BytestoTransfer = 3;

//        ucSPIWriteFrame[0] = (U8BIT)(inout_pSPLLConfig[u16CfgIdx].m_u8Address | 0x80);  // Address

//        s32RetVal = SPI_ReadWrite(in_hHandle, ucSPIWriteFrame, ucSPIReadData, u32BytestoTransfer, &ulBytesTransfered, (SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE|SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE));
//        if(s32RetVal != DP_SUCCESS)
//        {
//            return s32RetVal;
//        }

//        if((u32BytestoTransfer != ulBytesTransfered) && (s32RetVal == DP_SUCCESS))
//        {
//            return DP_SPI_WRITE_FAIL;
//        }

         u32BytestoTransfer = 1;

         ucSPIWriteFrame[0] = (U8BIT)(inout_pSPLLConfig[u16CfgIdx].m_u8Address | 0x80);  // Address
       // ucSPIWriteFrame[0] = 1;
        s32RetVal = SPI_Write(in_hHandle, ucSPIWriteFrame, u32BytestoTransfer, &ulBytesTransfered, (SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE));
        if(s32RetVal != DP_SUCCESS)
        {
            return s32RetVal;
        }

        if((u32BytestoTransfer != ulBytesTransfered) && (s32RetVal == DP_SUCCESS))
        {
            return DP_APP_SPI_WRITE_FAIL;
        }

        u32BytestoTransfer = 2;

        s32RetVal = SPI_Read(in_hHandle, ucSPIReadData, u32BytestoTransfer, &ulBytesTransfered, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
        if(s32RetVal != DP_SUCCESS)
        {
            return s32RetVal;
        }

        if((u32BytestoTransfer != ulBytesTransfered) && (s32RetVal == DP_SUCCESS))
        {
            return DP_APP_SPI_READ_FAIL;
        }
        else
        {
             /* FIXME MSB First to be checked and updated*/
            /*LSB data available in 2nd byte */
            inout_pSPLLConfig[u16CfgIdx].m_u16Data = ucSPIReadData[1] |  (ucSPIReadData[0] << 8);
            //memcpy(&inout_pSPLLConfig[u16CfgIdx].m_u16Data, ucSPIReadData, 2);
        }
    }

    return s32RetVal;
}
