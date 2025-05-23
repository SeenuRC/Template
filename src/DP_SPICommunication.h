#ifndef DP_SPICOMMUNICATION_H
#define DP_SPICOMMUNICATION_H

#include "mainwindow.h"
#include "libMPSSE_spi.h"

#define SPIChannelConfig_t SPI_ChannelConfig

typedef struct _SLMX2028_CONFIG
{
    U8BIT m_u8Address;
    U16BIT m_u16Data;
}SLMX2028_CONFIG, *PSLMX2028_CONFIG;

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
S32BIT STDCALL DP_SPI_FindPorts(PU8BIT out_arru8AvailablePorts);

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
S32BIT STDCALL DP_SPI_GetPortInfo(U8BIT in_u8PortNo, FT_DEVICE_LIST_INFO_NODE *out_pSPIChannelinfo);

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
S32BIT STDCALL DP_SPI_PortOpen(U8BIT in_u8PortNo, DP_DRV_HANDLE out_vpHandle);

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
S32BIT STDCALL DP_SPI_PortClose(DP_DRV_HANDLE in_vpHandle);

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
S32BIT STDCALL DP_SPI_PortConfigure(DP_DRV_HANDLE in_vpHandle, SPI_ChannelConfig *out_pSSPIConfig);

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

S32BIT STDCALL DP_LMX2028RegWrite(DP_DRV_HANDLE in_hHandle, PSLMX2028_CONFIG in_pSPLLConfig, U8BIT in_u8CfgCnt);

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
S32BIT STDCALL DP_LMX2028RegRead(DP_DRV_HANDLE in_hHandle, PSLMX2028_CONFIG inout_pSPLLConfig, U8BIT in_u8CfgCnt);

#endif // DP_SPICOMMUNICATION_H
