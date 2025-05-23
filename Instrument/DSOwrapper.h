#include "visa.h"
#include "include/dp_types.h"
#include <string.h>

/* Instrument Id*/
#define RS_RTM3004     1  /*R&S Instrument Model Number - RTM3004*/
#define KS_DSOX2012A   2  /*KeySight Instrument Model Number - DSOX2012A*/

#define DP_DSO_ZERO         0
#define DP_DSO_ONE          1
#define DP_DSO_TWO          2
#define DP_DSO_THREE		3
#define DP_DSO_FOUR         4
#define DP_DSO_FIVE         5
#define DP_DSO_EIGHT        8
#define DP_DSO_SIXTEEN         16

/*Channel Min & Max*/
#define DP_DSO_MIN_CHANNEL         1
#define DP_DSO_MAX_CHANNEL         4

/*Probe Attentuation Value*/
#define DP_DSO_MIN_PROBE_VAL       0.001
#define DP_DSO_MAX_PROBE_VAL       10000

/*Pulse Width*/
#define DP_DSO_POS_PULUEWIDTH      0
#define DP_DSO_NEG_PULUEWIDTH      1

/*Continuious Run Start & Stop*/
#define DP_DSO_CON_RUN_STOP        0
#define DP_DSO_CON_RUN_START       1

/*KS_DSOX2012A Acquisition Mode Selection*/
#define DP_DSO_ACQ_ETIME_MODE           0
#define DP_DSO_ACQ_RTIME_MODE           1
#define DP_DSO_ACQ_PDETECT_MODE         2
#define DP_DSO_ACQ_HRESOLUTION_MODE     3
#define DP_DSO_ACQ_SEGMENTED_MODE       4
#define DP_DSO_ACQ_SEGPDETECT_MODE      5
#define DP_DSO_ACQ_SEGHRES_MODE         6

/*RS_RTM3004 Acquisition Mode Selection*/
#define DP_DSO_RTM3004_ACQ_SAMPLE_MODE          0
#define DP_DSO_RTM3004_ACQ_PDETECT_MODE         1
#define DP_DSO_RTM3004_ACQ_HRESOLUTION_MODE     2
#define DP_DSO_RTM3004_ACQ_ENVELOPE_MODE        3
#define DP_DSO_RTM3004_ACQ_AVERAGE_MODE         4

/* Trigger Source Selection */
#define DP_DSO_TGR_SRC_CHAN1_SEL          0
#define DP_DSO_TGR_SRC_CHAN2_SEL          1
#define DP_DSO_TGR_SRC_CHAN3_SEL          2
#define DP_DSO_TGR_SRC_CHAN4_SEL          3
#define DP_DSO_TGR_SRC_EXTERNAL_SEL       4
#define DP_DSO_TGR_SRC_LINE_SEL           5

/* Trigger Coupling Selection */
#define DP_DSO_TGR_COUP_DC_SEL            0
#define DP_DSO_TGR_COUP_AC_SEL            1
#define DP_DSO_TGR_COUP_LFREJECT_SEL      2

/* Trigger Type Selection */
#define DP_DSO_TGR_TYPE_EDGE_SEL          0
#define DP_DSO_TGR_TYPE_WIDTH_SEL         1
#define DP_DSO_TGR_TYPE_RISETIME_SEL      2
#define DP_DSO_TGR_TYPE_LOGIC_SEL         3
#define DP_DSO_TGR_TYPE_LINE_SEL          4
#define DP_DSO_TGR_TYPE_TV_SEL            5
#define DP_DSO_TGR_TYPE_RUNT_SEL          6

/* RS_RTM3004 Trigger Mode Selection */
#define DP_DSO_TGR_AUTO_MODE              0
#define DP_DSO_TGR_NORMAL_MODE            1

/* KS_DSOX2012A Trigger Mode Selection */
#define DP_DSO_TGR_EDGE_MODE              0
#define DP_DSO_TGR_PWIDTH_MODE            1
#define DP_DSO_TGR_RUNT_MODE              2
#define DP_DSO_TGR_PATTERN_MODE           3
#define DP_DSO_TGR_STATE_MODE             4
#define DP_DSO_TGR_TIMEOUT_MODE           5
#define DP_DSO_TGR_GLITCH_MODE            6
#define DP_DSO_TGR_DELAY_MODE             7
#define DP_DSO_TGR_SEQUENCE_MODE          8
#define DP_DSO_TGR_SHOLD_MODE             9
#define DP_DSO_TGR_WINDOW_MODE            10
#define DP_DSO_TGR_ADVANCED_MODE          11
#define DP_DSO_TGR_TV_MODE                12
#define DP_DSO_TGR_COMM_MODE              13
#define DP_DSO_TGR_TRANSITION_MODE        14

enum{
    DP_DSO_SUCCESS = 0,
    DP_VI_DSO_OPEN_FAILURE = -999,
    DP_VI_DSO_CLOSE_FAILURE,
    DP_DSO_RESET_FAILED,
    DP_DSO_INVALID_PARAM,
    DP_DSO_INVALID_INS_NO,
    DP_DSO_INVALID_INS_ID,
    DP_DSO_FILE_OPEN_FAILURE,
    DP_DSO_CNT_READ_FAILURE,
    DP_DSO_MODEL_READ_FAILURE,
    DP_DSO_MANUFAUCHER_READ_FAILURE,
    DP_DSO_INS_ID_READ_FAILURE,
    DP_DSO_GET_INS_ID_FAILURE,
    DP_DSO_RESET_FAILURE,
    DP_DSO_CLS_FAILURE,
    DP_DSO_SIGLE_RUN_FAILURE,
    DP_ERR_DSO_INVALID_PARAM,
    DP_DSO_CHAN_CONFIG_FAILED,
    DP_DSO_SET_MEAS_TYPE_FAILURE,
    DP_DSO_SET_MEASUREMENT_TYPE_FAILURE,
    DP_DSO_SET_CHAN_SKEW_VAL_FAILURE,
    DP_DSO_CONFIG_ACQ_MODE_FAILURE,
    DP_DSO_SET_SAMP_RATE_FAILURE,
    DP_DSO_READ_PHASE_FAILURE,
    DP_DSO_READ_PEAK_VAL_FAILURE,
    DP_DSO_READ_PULSE_WIDTH_FAILURE,
    DP_DSO_MEASUREMENT_VAL_FAILURE,
    DP_DSO_MEAS_STATISTICS_FAILURE,
    DP_DSO_SET_TRIG_MODE_FAILURE,
    DP_DSO_SET_TRIG_SRC_FAILURE,
    DP_DSO_SET_TRIG_TYPE_FAILURE,
    DP_DSO_CHAN_NO_INVALID,
    DP_DSO_INVALID_MEAS_SEL,
    DP_DSO_INVALID_PROBE_VAL
};

/**
*\struct    S_DP_DSO_INFO
*\brief  	This structure contains members for DSO details
*
*/
typedef struct _S_DP_DSO_INFO
{
    bool m_bDSO;
    ViSession m_viDSO;
    U16BIT m_usInstID;
    float m_fAmp;
    double m_dtime;
    double m_dSamplingRate;
    char m_cDSOInfo[2][150];
}S_DP_DSO_INFO, *PS_DP_DSO_INFO;


/**
*\struct    sDSOInstCfg
*\brief  	This structure contains Oscilloscope Instrument Configuration Details
*
*/
typedef struct _DSOInstCfg
{
    char szManufacturer[64];
    char szModel[32];
    U16BIT usInstID;
}sDSOInstCfg;

/**
*\struct    sDSOInstHandle
*\brief  	This structure contains Oscilloscope Instrument Details
*
*/
typedef struct _SDSOInstHandle
{
    ViSession m_viDSOSession;
    U16BIT m_usInstID;
    U8BIT m_u8Instance;
}sDSOInstHandle;


/**
*\struct    S_DP_DSO_INFO
*\brief  	This structure contains members for DSO details
*
*/
typedef struct _S_DP_DSO_MEASSTATISTICS
{
    double dCurrMeas;
    double dMinPhase;
    double dMaxphase;
    double dMean;
    double dStndDeviation;
    double Range;
    unsigned int u32Count;
    unsigned char ucResultState;

}S_DP_DSO_MEASSTATISTICS, *PS_DP_DSO_MEASSTATISTICS;


class CDSO_Wrapper
{
private :

    ViStatus vistatus;

public :


    CDSO_Wrapper();

    sDSOInstHandle m_arrDSOInstHandle[16];
    sDSOInstCfg m_InstDSOCfg[32];
    U8BIT m_u8AvailDSOCount;

    /*Generic*/
    S16BIT DP_Scope_Open(U8BIT in_ucDSONo, PS8BIT pLogicalName);
    S16BIT DP_Scope_Close(U8BIT in_u8InstNo);
    S16BIT DP_Scope_Preset(U8BIT in_u8InstNo);
    S16BIT DP_Scope_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize);
    S16BIT ReadDSOInstrumentDetails();
    S16BIT GetInstID(PS8BIT in_pcInstDetails, U16BIT  *Out_usModel);

    /*Configuration*/
    S16BIT DP_Scope_ChannelConfig(U8BIT in_u8InstNo, U8BIT in_u8ChNo, FDOUBLE in_dVerScalePerDiv, FDOUBLE in_dHoriScalePerDiv, FDOUBLE in_dOffset, FDOUBLE in_dfProbeAttenValue);
    S16BIT DP_Scope_ConfigureAcquisitionType(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8AcquisitionType, FDOUBLE in_dSamplingRate);
    S16BIT DP_Scope_ConfigureAcquisitionRecord(U8BIT in_u8InstNo, U32BIT in_u32NumPts, FDOUBLE dAcqStartTime);
    S16BIT DP_Scope_SetPhaseMeasurementType(U8BIT in_u8InstNo, U8BIT in_u8MeasurementSel, U8BIT in_u8RefchNo, U8BIT in_u8OtherchNo, FSINGLE in_fSkew);
    S16BIT DP_Scope_SetPeakMeasurementType(U8BIT in_u8InstNo, U8BIT in_u8MeasurementSel, U8BIT in_u8ChannelNo);
    S16BIT DP_Scope_SetPulseWidthMeasType(U8BIT in_u8InstNo, U8BIT in_u8ChannelNo, U8BIT in_u8MeasurementSel, U8BIT in_u8SetMeasType);
    S16BIT DP_Scope_SetTriggerModeLevel(U8BIT in_u8InstNo, U8BIT in_u8ChannelNo, U8BIT in_u8Mode, U8BIT in_u8TriggerSrc, U8BIT in_u8TriggerType, FDOUBLE in_dTriggerLevel, U8BIT in_u8EdgeCoupling);
    S16BIT DP_SCOPE_RunControl(U8BIT in_u8InstNo, U8BIT in_u8ConRunOrStop);
    S16BIT DP_SCOPE_RunSingle(U8BIT in_u8InstNo);

    S16BIT DP_Scope_SetTimePeriodMeasType(U8BIT in_u8InstNo, U8BIT in_u8ChannelNo, U8BIT in_u8MeasurementSel);

    /*Measurement*/
    S16BIT DP_Scope_Measure_Statistics(U8BIT in_u8InstNo, U8BIT u8MeasurementSel, PS_DP_DSO_MEASSTATISTICS out_sDSOMeasStats);
    S16BIT DP_Scope_Read_Phasediff(U8BIT in_u8InstNo, U8BIT u8MeasurementSel, U32BIT in_u32NoofSamples, U32BIT in_u32Delay, FDOUBLE *Out_dPhaseDiffVal);
    S16BIT DP_Scope_Read_PeakValue(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8MeasurementSel, FDOUBLE *Out_fdPeakValue);
    S16BIT DP_Scope_Read_RMSAmplitudeValue(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8MeasurementSel, FDOUBLE *Out_fdPeakValue);
    S16BIT DP_Scope_Read_TopLevelAmplitude(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8MeasurementSel, FDOUBLE *Out_fdPeakValue);
    S16BIT DP_Scope_Read_PulseWidth(U8BIT in_u8InstNo, U8BIT in_u8ChanNo, U8BIT u8MeasurementSel, U8BIT in_u8MeasType, FDOUBLE *Out_fdPulseWidthValue);
    S16BIT DP_Scope_Read_TimePeriod(U8BIT in_u8InstNo, U8BIT in_u8ChNo, U8BIT u8MeasurementSel, FDOUBLE *Out_fdTimePeriodValue);        
};
