#ifndef PMWRAPPER_H
#define PMWRAPPER_H
#include "include/dp_types.h"
#include "visa.h"
#include "visatype.h"
#include<QString>
#include<QThread>

#define PMP4416A   0
#define PMN1911A   1
#define PMENRX     3

#define  DP_PWRMETER_SUCCESS         0
#define  DP_PWRMeter_FAILURE         1

#define DP_PWRMETER_FREERUN_ENABLE				1
#define DP_PWRMETER_FREERUN_DISABLE			0

#pragma pack(push, 1)

typedef struct _PMInstCfg
{
    char m_PM_Manufacturer[64];
    char m_PM_model[32];
    U16BIT usInstID;
}sPMInstCfg;

typedef struct _SPMInstHandle
{
    ViSession m_viPMSession;
    U16BIT m_usInstID;
}sPMInstHandle;
#pragma pack(pop)

enum{
    DP_POWMTR_SUCCESS = 0,
    DP_VI_PM_OPEN_FAILURE = -999,
    DP_PMETER_INT_INVALID_MODEL,
    DP_PMETER_INT_ID_READ_FAILED,
    DP_PMETER_RESET_FAILED,
    DP_PMETER_CLS_FAILURE,
    DP_PMETER_FREE_RUN_FAILURE,
    DP_ERR_PM_INVALID_PARAM,
    DP_PMETER_READ_POWER_FAILURE,
    DP_PMETER_GET_MARKER_FAILURE,
    DP_PMETER_CNFG_MODE_FAILURE,
    DP_PMETER_TRIG_CHANGE_MODE_FAILURE,
    DP_PMETER_TRIG_LEVEL_MODE_FAILURE,
    DP_PMETER_SET_MEASUREMENT_MODE_FAILURE,
    DP_PMETER_UPPER_LIMIT_CONFG_FAILURE,
    DP_PMETER_TRACE_SETUP_FAILURE,
    DP_PMETER_CONFG_MEASUREMENT_MODE_FAILURE,
    DP_PMETER_DISPLAY_FAILURE,
    DP_PMETER_CONFG_OFFSET_FAILURE,
    DP_PMETER_CONFG_DELAY_FAILURE,
    DP_PMETER_SWEEP_TIME_FAILURE,
    DP_PMETER_CNFG_FREQUENCY_FAILURE,
    DP_PMETER_CLOSING_INSTRUMENTS_FAILURE,
    DP_PMETER_CONFG_RANGE_FAILURE,
    DP_PMETER_CONFG_AVG_COUNT_FAILURE,
    DP_PMETER_CONFG_CHANNEL_ENABLE_MODE_FAILURE,
    DP_PMETER_CONFG_DUTY_CYLCLE_CORRECT_FAILURE,
    DP_PMETER_CONFG_CORRECT_FREQ_MODE_FAILURE,
    DP_PMETER_CONFG_REF_OSCILLATOR_MODE_FAILURE,
    DP_PMETER_TRIG_SOUR_MODE_FAILURE,
    DP_PMETER_CONFG_ZERO_MODE_FAILURE,
    DP_PMETER_GET_CHANNEL_NAME_FAILURE,
    DP_PMETER_CONFG_AUTO_ENABLE_MODE_FAILURE,
    DP_PMETER_CALLIBRATE_MODE_FAILURE,
    DP_PMETER_READ_PARAMETER_FAILURE,
    DP_PMETER_READ_CHANNEL_FAILURE,
    DP_PMETER_FETCH_MODE_FAILURE,
    DP_PMETER_MEASUREMENT_COMPLETE_MODE_FAILURE,
    DP_PMETER_ABORT_MODE_FAILURE,
    DP_PMETER_SELF_TEST_MODE_FAILURE,
    DP_PMETER_TRIG_DELETE_MODE_FAILURE,
    DP_PMETER_TRIG_HOLD_MODE_FAILURE,
    DP_PMETER_OUT_TRIG_MODE_FAILURE,
    DP_PMETER_OPERATION_COMP_MODE_FAILURE,
    DP_PMETER_GET_DURATION_FAILURE,
    DP_PMETER_MEASUREMENT_PULSE_PERIOD_MODE_FAILURE,
    DP_PMETER_GET_TRANS_POS_DUR_FAILURE,
    DP_PMETER_GET_TRANS_NEG_DUR_FAILURE,
    DP_PMETER_OPER_COM_FAILURE
};


class CPMWrapper
{
private :

    QString  m_IVIRFPMOpenStatus;
    U16BIT m_usTotalInstances;
    bool m_bOpenStatus;
    char arrcErrStr[256];
    bool m_bInitErrFlag;
    
public:
    ViSession viRFPMSession[10];
    ViSession defaultPMRM, g_viRFPM[3], viPM;
    ViStatus vistatus;

    sPMInstCfg m_InstCfg[2];
    sPMInstHandle m_arrPMInstHandle[16];
    QThread *obj_Sleep;

    CPMWrapper();
    S16BIT GetInstID(PS8BIT in_pcInstDetails, U16BIT  *Out_usModel);
    S16BIT Open(U8BIT in_ucPMNo, PS8BIT pLogicalName);
    S16BIT Close(U8BIT in_ucPMNo);
    S16BIT ReadPMInstrumentDetails();
    S16BIT DP_PMeter_Preset(U8BIT in_ucPMNo);
    S16BIT DP_PMeter_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT U16BufSize);

    ViStatus DP_PowerMeter_Config(ViSession in_uiVi, double in_dFreq, double in_dOffset,double in_dXStart,double in_dXScal,double in_dYMax,double in_dYScal,double in_dTriggerLvl,double in_dMarker1Time,double in_dMarker2Time);
    ViStatus DP_PwrMtr_ConfigPowerDroop(ViSession in_uiVi, double in_dFreq, double in_dOffset,double in_dXStart,double in_dXScal,double in_dYMax,double in_dYScal,double in_dTriggerLvl,double in_dMarker1Time,double in_dMarker2Time);
    /*Rise Time */
    ViStatus DP_PowerMeter_Config1(ViSession in_uiVi, double in_dFreq, double in_dOffset, double in_dXStart,double in_dXScal,double in_dYMax,double in_dYScal,double in_dTriggerLvl,double in_dMarker1Time,double in_dMarker2Time);
    /*Fall Time*/
    ViStatus DP_PowerMeter_Config2(ViSession in_uiVi, double in_dFreq, double in_dOffset,double in_dXStart,double in_dXScal,double in_dYMax,double in_dYScal,double in_dTriggerLvl,double in_dMarker1Time,double in_dMarker2Time);
    ViStatus DP_PwrMtr_MarkerConfig(ViSession in_uiVi,U8BIT in_ucTriggerSrcType ,double in_dMarker1Time,double in_dMarker2Time);

    S16BIT DP_PwrMtr_SetFrequency(ViSession in_uiVi, double in_dFreq);
    ViStatus DP_PwrMtr_Trig_AutoMode(ViSession in_uiVi);
    ViStatus DP_PowerMeter_FreeRun(ViSession in_uiVi, bool in_EnaDis);
    ViStatus DP_PwrMtr_ReadMarkerPower(ViSession in_uiVi,U8BIT usMarkerNo,double *out_pdPower);
    ViStatus DP_PwrMtr_ReadPeakPower(ViSession in_uiVi, double *out_pdPower);
    S16BIT DP_PwrMtr_MeasurPulseRiseTime(ViSession in_uiVi, double *out_pdMeasRiseTime);
    S16BIT DP_PwrMtr_MeasurPulseFallTime(ViSession in_uiVi, double *out_pdMeasFallTime);
    ViStatus DP_PowerMeter_Slope(ViSession in_uiVi, bool in_PosNeg);

    S16BIT DP_PwrMtr_WaitforOperationComplete(ViSession in_uiVi);
    S16BIT DP_PwrMtr_Calibration(ViSession in_uiVi);
};

#endif // PMWRAPPER_H
