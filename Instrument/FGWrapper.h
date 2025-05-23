#ifndef CFGWRAPPER_H
#define CFGWRAPPER_H
#include "visatype.h"
#include "../include/dp_types.h"
#include "visa.h"
#include <QFileInfo>
#include <QSettings>
#include <QStringList>
#include <QApplication>
#include <QDebug>
typedef struct _FGInsconfg
{
    char m_szManufacturer[64];
    char m_szmodel[32];
    unsigned short m_usInstID;
}SFGInsconfg;

typedef struct _SFGInstHandle
{
    ViSession m_viFGSession;
    unsigned short m_usInstID;
    U8BIT m_u8Instance;
}SFGInstHandle;

/*FG Model ID*/
#define TKX_AFG31102        1 /*Tektronix Function generator AFG31102*/
#define SDG2042X            2 /*SIGLENT*/

/*Generic Macros*/
#define DP_FG_CNFG_SUCCESS 0
#define DP_FG_MAX_INST_NO  32
#define DP_FG_ZERO          0

/*FG Channel Selection Macros*/
#define DP_FG_CH1       1
#define DP_FG_CH2       2
#define DP_FG_ALLCHN    3

/*FG Output Macros*/
#define DP_FG_OUTPUT_EN   1
#define DP_FG_OUTPUT_DIS  0

/*FG Polarity Macros*/
#define DP_FG_NORMAL_POLARITY   0
#define DP_FG_INVERSE_POLARITY  1

/*FG Impedance Mode Selection Macros*/
#define DP_FG_IMPEDANCE_AUTO         0
#define DP_FG_IMPEDANCE_CONFIGURABLE 1

/*FG Impedance Automode Selection Macros*/
#define DP_FG_IMPEDANCE_MIN      0 /*1 ohm*/
#define DP_FG_IMPEDANCE_MAX      1/*10 kilo ohm*/
#define DP_FG_IMPEDANCE_INFINITY 2/*Greater than 10 kilo ohm*/

#define DP_FG_IMPEDANCE_HZ      0 /*High (For Siglent instrument)*/

/*FG Pulse Hold Macros*/
#define DP_FG_HOLD_PULSE_WIDTH 0
#define DP_FG_HOLD_PULSE_DUTY  1

/*FG Burst Cycle Macros*/
#define DP_FG_BURST_CYCLE_AUTO         0
#define DP_FG_BURST_CYCLE_CONFIGURABLE 1

/*FG Burst Cycle Auto Macros*/
#define DP_FG_BURST_CYCLE_MIN       0
#define DP_FG_BURST_CYCLE_MAX       1
#define DP_FG_BURST_CYCLE_INFINITY  2

/*FG Burst Idle Mode Selection Macros*/
#define DP_FG_BURST_IDLE_START       0
#define DP_FG_BURST_IDLE_DC          1
#define DP_FG_BURST_IDLE_STOP        2

/*FG Frequency Mode Selection Macros*/
#define DP_FG_FREQ_CW       0
#define DP_FG_FREQ_FIXED    1
#define DP_FG_FREQ_SWEEP    2

/*FG Sweep Mode Macros*/
#define DP_FG_SWEEP_AUTO      0
#define DP_FG_SWEEP_MANUAL    1

/*FG Waveform Shape Macros*/
#define DP_FG_WAVEFORM_SIN          0
#define DP_FG_WAVEFORM_SQUARE       1
#define DP_FG_WAVEFORM_PULSE        2
#define DP_FG_WAVEFORM_RAMP         3
#define DP_FG_WAVEFORM_PRNOISE      4
#define DP_FG_WAVEFORM_DC           5
#define DP_FG_WAVEFORM_SINC         6
#define DP_FG_WAVEFORM_GAUSSIAN     7
#define DP_FG_WAVEFORM_LORENTZ      8
#define DP_FG_WAVEFORM_ERISE        9
#define DP_FG_WAVEFORM_EDECAY       10
#define DP_FG_WAVEFORM_HAVERSINE    11
#define DP_FG_WAVEFORM_EMEMORY1     12
#define DP_FG_WAVEFORM_EMEMORY2     13
#define DP_FG_WAVEFORM_EFILE        14


#define DP_FG_WAVEFORM_PULSE_SIGLENT        0
typedef  unsigned char session;
typedef unsigned long attr;


///*DP Function Generator Error Codes*///

enum
{
  DP_FG_OPEN_FAILURE = -999,
  DP_FG_INT_ID_READ_FAILED,
  DP_FG_INT_INVALID_MODEL,
  DP_FG_INVALID_FG_CNT_FORMAT,
  DP_FG_COUNT_NOT_EXISTS,
  DP_FG_RESET_FAILURE,
  DP_FG_INVALID_CH_SEL,
  DP_FG_INVALID_INST_NUM,  
  DP_FG_WAVEFORM_SHAPE_CFG_FAILURE,
  DP_FG_INVALID_WAVEFORM_SHAPE_CFG,
  DP_FG_OUTPUT_EN_DIS_FAILURE,
  DP_FG_POLARITY_CFG_FAILURE,
  DP_FG_IMPEDANCE_CFG_FAILURE,
  DP_FG_INVALID_IMPEDANCE_SEL,
  DP_FG_DUTYCYCLE_CFG_FAILURE,
  DP_FG_INVALID_DCYCLE,
  DP_FG_ERR_INVALID_PARAM,
  DP_FG_PULSE_DELAY_CFG_FAILURE,
  DP_FG_PULSE_PERIOD_CFG_FAILURE,
  DP_FG_PULSE_WIDTH_CFG_FAILURE,
  DP_FG_TRIGGER_MODE_CFG_FAILURE,
  DP_FG_HIGH_LVL_CFG_FAILURE,
  DP_FG_LOW_LVL_CFG_FAILURE,
  DP_FG_HIGH_LOW_LVL_CFG_FAILURE,
  DP_FG_OFFSET_CFG_FAILURE,
  DP_FG_CLOSE_FAILURE,
  DP_FG_PULSE_HOLD_FAILURE,
  DP_FG_AM_SOURCE_SEL_FAILURE,
  DP_FG_AM_OUT_ENDIS_FAILURE,
  DP_FG_BURST_MODE_CFG_FAILURE,
  DP_FG_BURST_CYCLE_CFG_FAILURE,
  DP_FG_INVALID_BURST_MODE_SEL,
  DP_FG_BURST_EN_DIS_FAILURE,
  DP_FG_BURST_IDLE_CFG_FAILURE,
  DP_FG_INVALID_BURST_IDLE_MODE_SEL,
  DP_FG_CENTER_FREQ_CFG_FAILURE,
  DP_FG_FREQ_CFG_FAILURE,
  DP_FG_FREQ_MODE_CFG_FAILURE,
  DP_FG_INVALID_FREQ_MODE_SEL,
  DP_FG_SPAN_CFG_FAILURE,
  DP_FG_START_STOP_CFG_FAILURE,
  DP_FG_HOLD_TIME_CFG_FAILURE,
  DP_FG_SWEEP_MODE_CFG_FAILURE,
  DP_FG_SWEEP_TIME_CFG_FAILURE,
  DP_FG_VOLTAGE_CFG_FAILURE,
  DP_FG_IP_FILE_NOT_EXISTS,

};



class CFGWrapper
{
public:
    CFGWrapper();

    ViStatus m_viStatus;
    SFGInsconfg m_arrSFGInsconfg[32];
    SFGInstHandle m_arrSFGInstHandle[32];

    unsigned char m_ucAvailFGCount;


    /*Generic*/
    S16BIT Open(unsigned char in_u8InstNo, char *pLogicalName);
    S16BIT GetInstID(char *in_pcInstDetails, unsigned short *out_pusModel);
    S16BIT GetInstrumentDetails();
    S16BIT DP_FG_Reset(U8BIT in_u8InstNo);
    S16BIT DP_FG_Close(U8BIT in_u8InstNo);
    S16BIT DP_FG_ConfigureWaveformShape(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8ShapeSel);
    S16BIT DP_FG_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT U16BufSize);

    /*Pulse Functions*/
    S16BIT DP_FG_CfgPulseDutyCycle(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dDutyCycle);
    S16BIT DP_FG_CfgPulseDelay(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dDelay);
    S16BIT DP_FG_CfgPulsePeriod(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dPulsePeriod);
    S16BIT DP_FG_CfgPulseWidth(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dPulseWidth);
    S16BIT DP_FG_CfgPulseHold(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bHoldSel);
    S16BIT DP_FG_CfgPulse(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dDutyCycle, FDOUBLE in_dPulsePeriod);

    /*Voltage Functions*/
    S16BIT DP_FG_CfgHighLevel(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dHighLevelVal);    
    S16BIT DP_FG_CfgLowLevel(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dLowLevelVal);
    S16BIT DP_FG_CfgHighLowLvl(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dHighLevelVal, FDOUBLE in_dLowLevelVal);
    S16BIT DP_FG_CfgOffsetVoltage(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dOffsetVal);
    S16BIT DP_FG_SetDCVoltage(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dVoltage);

    /*Burst Functions*/
    S16BIT DP_FG_CfgBurstMode(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bBurstModeSel);
    S16BIT DP_FG_CfgBurstCycles(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8BurstModeSel, U32BIT in_u32NoOfCycles);
    S16BIT DP_FG_EnableBurstMode(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bBurstEnDis);
    S16BIT DP_FG_CfgBurstIdle(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8BurstModeSel);

    /*Frequency Functions*/
    S16BIT DP_FG_CfgCenterFrequency(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dCenterFrequency);
    S16BIT DP_FG_CfgFrequency(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bFreqModeSel, FDOUBLE in_dFrequency);
    S16BIT DP_FG_CfgFrequencyMode(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8FreqModeSel);
    S16BIT DP_FG_ConfigureSpan(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dCenterFrequency);
    S16BIT DP_FG_CfgStartStopFreq(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dStartFreq, FDOUBLE in_dStopFreq);

    /*Sweep Functions*/
    S16BIT DP_FG_CfgSweepHoldTime(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dHoldTime);
    S16BIT DP_FG_CfgSweepMode(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bSweepModeSel);
    S16BIT DP_FG_CfgSweepTime(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dSweepTime);

    /*Output Functions*/
    S16BIT DP_FG_CfgTriggerMode(U8BIT in_u8InstNo, bool in_bTriggerMode);
    S16BIT DP_FG_EnableOutput(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bEnDisOutput);
    S16BIT DP_FG_SetPolarity(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bPolaritySel);
    S16BIT DP_FG_SetImpedance(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8ImpedanceSel, FDOUBLE in_dImpedance);


    /*SIGLENT Functions*/
    S16BIT DP_FG_Config(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8ShapeSel,FDOUBLE in_dPeriod, FDOUBLE in_dWidth, FDOUBLE in_dHighLevel,FDOUBLE in_dLowLevel);
};

#endif // CFGWRAPPER_H
