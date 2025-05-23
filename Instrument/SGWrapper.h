#ifndef SGWRAPPER_H
#define SGWRAPPER_H
#include "../include/dp_types.h"
#include "visa.h"
#include "visatype.h"
#include<QString>
//#include"dprfsigen_err.h"
#include<QThread>

#define KS_E8257D  1
#define KS_E8267D  2
#define RS_SMB100A  3
#define KS_N5183B  4

#pragma pack(push, 1)

typedef struct _SGInstCfg
{
    char szManufacturer[64];
    char szModel[32];
    unsigned short usInstID;
}SGInstCfg;

typedef struct _SSGInstHandle
{
    ViSession m_viSGSession;
    unsigned short m_usInstID;
}SSGInstHandle;
#pragma pack(pop)

class CSGWrapper
{
private :

    QString  m_IVIRFFGENOpenStatus;
    unsigned short m_usTotalInstances;
    bool m_bOpenStatus;
    char arrcErrStr[256];
public:
    ViSession viRFSigGenSession[10];
    ViSession defaultSGRM, g_viRFSG[3], viSG;
    ViStatus vistatus;
    SGInstCfg m_InstCfg[32];
    SSGInstHandle m_arrSInstHandle[16];
    CSGWrapper();
        short GetInstID(char *in_pcInstDetails, unsigned short  *Out_usModel);
     short Open(unsigned char in_ucSGNo, char *pLogicalName);
      short	DP_Enumeration_RFSigGen_Preset(unsigned char in_ucSGNo);
      short	DP_Enumeration_RFSigGen_ConfigRF(unsigned char in_ucSGNo, double dFrequency, double dPowerLevel);
      short	DP_Enumeration_RFSigGen_ConfigALCEnabled (unsigned char in_ucSGNo,bool bAlcEnabled);
      short	DP_Enumeration_RFSigGen_ConfigOutputEnabled(unsigned char in_ucSGNo,bool bOutputEnabled);
      short	DP_Enumeration_RFSigGen_ConfigAMEnabled(unsigned char in_ucSGNo, unsigned char in_ucSource, bool bAmEnabled);
      short	DP_Enumeration_RFSigGen_ConfigAMExtCoupling(unsigned char in_ucSGNo,unsigned int in_uiAmExternalCoupling);
      short	DP_Enumeration_RFSigGen_ConfigAM( unsigned char in_ucSGNo, unsigned char in_ucAMType, double in_dAMRate, double dDepth);
      short	DP_Enumeration_RFSigGen_ConfigFMEnabled(unsigned char in_ucSGNo, unsigned char in_ucSource, unsigned int uiFmEnabled);
      short	DP_Enumeration_RFSigGen_ConfigFMExtCoupling(unsigned char in_ucSGNo,unsigned int in_uiFmExternalCoupling);
      short	DP_Enumeration_RFSigGen_ConfigFM(unsigned char in_ucSGNo,unsigned char in_ucMode,double in_dFMRate, double in_dDeviation);
      short   DP_Enumeration_RFSigGen_ConfigPMEnabled(unsigned char in_ucSGNo, bool bEnabled,  unsigned char in_ucSource);
      short	DP_Enumeration_RFSigGen_ConfigPMExtCoupling (unsigned char in_ucSGNo, unsigned int in_uiPmExternalCoupling);
      short	DP_Enumeration_RFSigGen_ConfigPM(unsigned char in_ucSGNo,unsigned char in_ucMode, double dFreq, double dDeviation);
      short	DP_Enumeration_RFSigGen_ConfigPulOutput (unsigned char in_ucSGNo, unsigned int in_uiPolarity, bool bPulseModulationEnabled);
      short	DP_Enumeration_RFSigGen_ConfigPulMSource (unsigned char in_ucSGNo,unsigned int in_uiSource);
      short	DP_Enumeration_RFSigGen_ConfigPulmMEnabled (unsigned char in_ucSGNo,bool bPmEnabled);
      short	DP_Enumeration_RFSigGen_ConfigPulMExtPolarity(unsigned char in_ucSGNo,unsigned int in_uiExternalPolarity);
      short	DP_Enumeration_ConfigLFGen(unsigned char in_ucSGNo,unsigned int in_uiSrcIndex,unsigned int in_uiSrcActive,double dFrequency,unsigned int in_uiWaveform);
      short	DP_Enumeration_RFSigGen_ConfigLFGenOutput(unsigned char in_ucSGNo,double dAmplitude,bool bEnabled);
      short	DP_Enumeration_RFSigGen_ConfigPulIntTrg(unsigned char in_ucSGNo, double dPeriod);
      short	DP_Enumeration_RFSigGen_ConfigPulTrig(unsigned char in_ucSGNo, unsigned int in_uiTriggerSource, double dPulseWidth, bool bGatingEnabled);
      short	DP_Enumeration_RFSigGen_ConfigPulDble(unsigned char in_ucSGNo, bool bEnabled, double dDelay);
      short	DP_Enumeration_RFSigGen_ConfigSweep(unsigned char in_ucSGNo, unsigned int in_uiMode, unsigned int in_uiTriggerSource);
      short	DP_Enumeration_RFSigGen_FreqSweepStrtStp(unsigned char in_ucSGNo, double dStart, double dStop);
      short	DP_Enumeration_RFSigGen_FreqSweepCenterSpan(unsigned char in_ucSGNo, double dCenter, double dSpan);
      short	DP_Enumeration_RFSigGen_FreqSweepTime(unsigned char in_ucSGNo, double dTime);
      short	DP_Enumeration_RFSigGen_PowerSweepStrtStp(unsigned char in_ucSGNo, double dStart, double dStop);
      short	DP_Enumeration_RFSigGen_PowerSweepTime(unsigned char in_ucSGNo, double dTime);
      short	DP_Enumeration_RFSigGen_FreqStepStrtStp(unsigned char in_ucSGNo, double dstart, double dstop, double dDwell, unsigned int in_uiNoPoints);
      short	DP_Enumeration_ConfigFreqStepDwell(unsigned char in_ucSGNo,bool bEnabled, double dDwell);
      short	DP_Enumeration_RFSigGen_PwrStepStrtStp(unsigned char in_ucSGNo, double dStart, double dStop, unsigned int in_uiNoPoints);
      short	DP_Enumeration_RFSigGen_ConfigPowerStepDwell(unsigned char in_ucSGNo,bool bEnabled, double dDwell);
      short	DP_Enumeration_RFSigGen_ConfigRefOsc(unsigned char in_ucSGNo,unsigned int in_uiSource, double dFrequency);
      short	DP_Enumeration_RFSigGen_ConfigALC(unsigned char in_ucSGNo,unsigned int in_uiSource, double dBandwidth);
      short   DP_Enumeration_RFSigGen_Close(unsigned char in_ucSGNo);
      long    ConfigFreqStepSweep(unsigned char in_ucSGNo, double in_dStartFreq, double in_dStopFreq,  unsigned int in_uiNoPoints, double in_dDwellTime );
      short   EnableDisableFreqSweep(unsigned char in_ucSGNo, bool in_bEnaDisable);
      short   DisableAllModulation(unsigned char in_ucSGNo);
      long    Config_List_Dwell(unsigned char in_ucSGNo, char *strListDwell);
      short   Config_ListSweep(unsigned char in_ucSGNo);
      short   Run_Single_Sweep(unsigned char in_ucSGNo);
      short   Run_Continuous_Sweep(unsigned char in_ucSGNo);
      short   Stop_Continuous_Sweep(unsigned char in_ucSGNo);
      long    DP_Enumeration_RFSigGen_SelfTest(unsigned char in_ucSGNo, unsigned int uiTimedelay, short *out_sStatus);
      short   Config_ListConfiguration(unsigned char in_ucSGNo, double *dFreq, double *dPwr, float *fDwellTime, unsigned char in_ucListCount);
      S32BIT   DP_SIGGEN_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize);
      short   DP_Enumeration_RFSigGen_ConfigPulGen(unsigned char in_ucSGNo);

};

#endif // SGWRAPPER_H
