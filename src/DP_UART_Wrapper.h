#ifndef DP_APP_UART_H
#define DP_APP_UART_H

#include "include\dp_types.h"
#include "DP_datastructure.h"
typedef unsigned long DWORD;

#define DP_APP_UART_RDINTERVAlTIMEOUT			  0xFFFFFFFF
#define DP_APP_UART_RDTOTAlTIMEOUTMULTIPLIER       1
#define DP_APP_UART_RDTOTAlTIMEOUTCONST            1
#define DP_APP_UART_WRTOTAlTIMEOUTMULTIPLIER       1
#define DP_APP_UART_WRTOTAlTIMEOUTCONST            1

/**
*\brief         This function is used to find the UART ports.
*
*\param[out]    out_pu8AvailPortCnt		 It specifies the pointer to hold the available port count
*\param[in]  	out_pu8AvailablePorts  	 It specifies the pointer to hold the Available ports
*
*\retval	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_APP_ERR_INVALID_POINTER is returned if the specified output total devices pointer is null
*
*\par            Pre-requisite functions and it's sequence:
*                This function does not have any pre requisite function and can be called at any time needed
*
*\author     
*\date       01 Aug 2023
*/
S32BIT STDCALL UART_FindPorts(PU8BIT out_pu8AvailPortCnt, PU8BIT out_pu8AvailablePorts[]);

/**
*\brief			This function is used to internally to configure the opened UART port.
*
*\param[in] 	in_SRS232Config         It specifies the structure for UART configuration details
*\param[out] 	out_pu16InstanceID  	It specifies the pointer to get UART port Instance.
*
*\retval		::DP_SUCCESS is returned upon success
*
* *\par Pre-requisite functions and it's sequence:
*   -# ::UART_FindPorts( )
*
*\author     
*\date       01 Aug 2023
*/
S32BIT STDCALL UART_Open(SDP_APP_UART_CONFIG in_SRS232Config, PU16BIT out_pu16InstanceID);


S32BIT STDCALL UART_PORT_Open(U8BIT in_u8PortNo, PU16BIT out_pu16InstanceID);

S32BIT STDCALL UART_ConfigurePort(U16BIT in_u16InstanceID, SDP_APP_UART_CONFIG in_SRS232Config);
/**
*\brief         This function is used to close the opened UART port.
*
*\param[in]  	in_u16InstanceID    It specifies the UART port Instance which obtained from UART open function
*
*\retval		::DP_SUCCESS is returned upon success
*
*\par Pre-requisite functions and it's sequence:
*   -# ::UART_Open( )
*
*/
S32BIT STDCALL UART_Close(U16BIT in_u16InstanceID);

/**
*\brief			This function is used to clear the stream buffers
*
*\param[in]  	in_u16InstanceID    It specifies the UART port Instance which obtained from UART open function
*
*\retval		::DP_SUCCESS is returned upon success
*\retval		::DP_APP_UART_BUFFCLR_FAIL is returned upon failure
*
* *\par Pre-requisite functions and it's sequence:
*   -# ::UART_Open( )
*
*\author     
*\date       01 Aug 2023
*/
S32BIT STDCALL UART_StreamBuffClear(U16BIT in_u16InstanceID);

/**
*\brief	 	This function is used to write stream data.
*
*\param[in]  	in_u16InstanceID    It specifies the UART port Instance which obtained from UART open function
*\param[in]  	in_pu8WriteBuff     It specifies the data to be written
*\param[in]  	in_u16DataCnt    	It specifies the data count to be written in bytes
*
*\retval 	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_APP_ERR_UART_WRITE is returned if Uart fails to write
*
*\par Pre-requisite functions and it's sequence:
*   -# ::UART_Open( )
*
*\author     
*\date       01 Aug 2023
*/
S32BIT STDCALL UART_StreamWrite(U16BIT in_u16InstanceID, PU8BIT in_pu8WriteBuff, U16BIT in_u16DataCnt);

/**
*\brief     	This function is used to read stream data.
*
*\param[in]		in_u16InstanceID     It specifies the UART port Instance which obtained from UART open function
*\param[in]  	in_u16DataCnt  		 It specifies the data count to be read in bytes
*\param[in]  	in_u32TimeOut    	It specifies the time out
*\param[out] 	out_pu8ReadBuff		 It specifies the pointer to get read data
*
*\retval 	 	::DP_SUCCESS is returned upon success
*\retval	 	::DP_APP_ERR_UART_Read is returned if Uart fails to Read
*\retval	 	::DP_APP_ERR_UART_TIMEOUT is returned if data is not read in specified time
*
*\par Pre-requisite functions and it's sequence:
*   -# ::UART_Open( )
*
*\author     
*\date       01 Aug 2023
*/
S32BIT STDCALL UART_StreamRead(U16BIT in_u16InstanceID, U16BIT in_u16DataCnt, U32BIT in_u32TimeOut, PU8BIT out_pu8ReadBuff);


S32BIT STDCALL UART_SetCommunicationTimeouts( U16BIT in_u16InstanceID, DWORD in_dwReadIntervalTimeout, \
                                                                       DWORD in_dwReadTotalTimeoutMultiplier, DWORD in_dwReadTotalTimeoutConstant, \
                                                                       DWORD in_dwWriteTotalTimeoutMultiplier, DWORD in_dwWriteTotalTimeoutConstant);
#endif // DP_APP_UART_H
