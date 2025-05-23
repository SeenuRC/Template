#ifndef CDMMWRAPPER_H
#define CDMMWRAPPER_H

#include <windows.h>
#include "include/dp_types.h"
#include "stdio.h"
#include <QString>
#include <QSettings>
#include <QFileInfo>
#include <QApplication>
#include <QDebug>
#include "DP_UART_Wrapper.h"
#include "DP_APP_macros.h"

#define DP_DMM_SUCCESS                      0
#define DP_DMM_MIN_TRGDELAY                 0.0
#define DP_DMM_MAX_TRGDELAY                 3600.0
#define DP_DMM_MIN_TRIG_SAMPLE_COUNT        1
#define DP_DMM_MAX_TRIG_SAMPLE_COUNT        50000

#define DP_AUTO_RANGE_ENABLE                1
#define DP_AUTO_RANGE_DISABLE               0

#define DP_DMM_TRIGGER_BUS                  0
#define DP_DMM_TRIGGER_EXTERNAL             1
#define DP_DMM_TRIGGER_IMMEDIATE            2

#define DP_DMM_VOLTAGE_MEAS                 0
#define DP_DMM_RESISTANCE_MEAS              1

#define  DP_VALIDATE_HANDLE(in_hHandle)\
{\
    if(in_hHandle == INVALID_HANDLE_VALUE)\
    return DP_APP_INVALID_HANDLE;\
}

#define DP_VALIDATE_POINTER(out_pSpointer)\
{\
    if(!out_pSpointer)\
    return DP_APP_INVALID_POINTER;\
}

#define DP_DMM_VALIDATE_MEAS_TYPE(in_u8MeasType)\
{\
    if(in_u8MeasType > 1)\
    return DP_DMM_INVALID_MEAS_TYPE;\
}

#define DP_DMM_VALIDATE_AUTORANGE_ENDIS(in_u8AutoRangeEnDis)\
{\
    if(in_u8AutoRangeEnDis > 1)\
    return DP_DMM_INVALID_AUTORANGE_ENDIS;\
}

#define DP_DMM_VALIDATE_AUTOTRIGDELAY_ENDIS(in_u8AutoTrgDelayEnDis)\
{\
    if(in_u8AutoTrgDelayEnDis > 1)\
    return DP_DMM_INVALID_AUTOTRGDELAY_ENDIS;\
}

#define DP_DMM_VALIDATE_TRIG_TYPE(in_u8TriggerType)\
{\
    if(in_u8TriggerType > 2)\
    return DP_DMM_INVALID_TRIG_TYPE;\
}

#define DP_DMM_VALIDATE_TRIG_DELAY(in_fTrigDelay)\
{\
    if((in_fTrigDelay < DP_DMM_MIN_TRGDELAY) || (in_fTrigDelay < DP_DMM_MAX_TRGDELAY))\
    {\
        return DP_DMM_INVALID_TRGDELAY;\
    }\
}

#define DP_DMM_VALIDATE_TRIG_SAMPLE_COUNT(in_u32Count)\
{\
    if((in_u32Count < DP_DMM_MIN_TRIG_SAMPLE_COUNT) || (in_u32Count < DP_DMM_MIN_TRIG_SAMPLE_COUNT))\
    {\
        return DP_DMM_INVALID_TRG_SAMPLE_COUNT;\
    }\
}

typedef struct _S_DMM_DETAILS
{
    char m_szSerialNo[16];
    char m_szModel[16];
    char m_szManufacturer[64];
    char m_szRevision[16];

}S_DMM_DETAILS, *PS_DMM_DETAILS;

enum
{
    DP_DMM_INVALID_MEAS_TYPE = -999,
    DP_DMM_INVALID_AUTORANGE_ENDIS,
    DP_DMM_INVALID_AUTOTRGDELAY_ENDIS,
    DP_DMM_INVALID_TRIG_TYPE,
    DP_DMM_INVALID_TRGDELAY,
    DP_DMM_INVALID_TRG_SAMPLE_COUNT,

    DP_DMM_OPEN_FAILED,
    DP_DMM_INST_ID_MISMATCH,
    DP_DMM_CONFIG_FAILED,
    DP_DMM_MEASURE_FAILED,
    DP_DMM_CLOSE_FAILED,
    DP_DMM_GETID_FAILED,
    DP_DMM_SETTRIGGER_FAILED,
    DP_DMM_RESET_FAILED,
    DP_DMM_EXTTRIG_CONFIG_FAILED,
    DP_DMM_CLEAR_FAILED,
    DP_DMM_GET_CALIBDETAILS_FAILED
};

class CDMMWrapper
{
public:
    CDMMWrapper();

    bool m_bInitErrFlag;
    S8BIT s8DMMID[40];

    void ReadDMMInstrumentDetails();

    S32BIT DP_DMM_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize);

    S32BIT DP_DMM_Open(U8BIT in_u8PortNo, SDP_APP_UART_CONFIG in_SRS232Config, DP_DRV_HANDLE out_phHandle);

    S32BIT DP_DMM_Configure(DP_DRV_HANDLE in_hHandle, U8BIT in_u8MeasType, U8BIT in_u8AutoRangeEnDis, FSINGLE in_fRange, FSINGLE in_fResolution);

    S32BIT DP_DMM_Measure(DP_DRV_HANDLE in_hHandle, U32BIT in_u32TimeoutinMS, PFSINGLE out_pfMeasData);

    S32BIT DP_DMM_Close(DP_DRV_HANDLE in_hHandle);

    S32BIT DP_DMM_ReadDetails(DP_DRV_HANDLE in_hHandle, PS_DMM_DETAILS out_psDMMDetails);

    S32BIT DP_DMM_ConfigureTrigger(DP_DRV_HANDLE in_hHandle, U8BIT in_u8TriggerType);
    
    S32BIT DP_DMM_ExtTriggerConfig(DP_DRV_HANDLE in_hHandle, U8BIT in_u8AutoTrgDelayEnDis, FSINGLE in_fTrigDelay, U32BIT in_u32TrigCount, U32BIT in_u32NoofSamples);

    S32BIT DP_DMM_Reset(DP_DRV_HANDLE in_hHandle);

    S32BIT DP_DMM_Clear(DP_DRV_HANDLE in_hHandle);

    S32BIT DP_DMM_ReadCalibDetails(DP_DRV_HANDLE in_hHandle, PS8BIT out_ps8CalibDetails);
        
};


#endif // CDMMWRAPPER_H
