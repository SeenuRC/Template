

#include <qstring.h>
#include "visa.h"
#include "visatype.h"
#include "stdio.h"
/************************************************************************************************

								RFSIGNAL GEN

					IVI Enumeration For RFSIGNAL GEN

*************************************************************************************************/

class CIVI_RFSIGGEN
{

private :

	typedef  unsigned long  session;


    QString  m_IVIRFFGENOpenStatus;

	unsigned short m_usTotalInstances;
	bool m_bOpenStatus;
	char arrcErrStr[256];
   //ViStatus status;
	
public :

    //IVI DSO Instrument Open Function  Prototype
    short Open(unsigned char in_ucSGNo, char *pLogicalName);
    //void GetOpenStatus(QString &csStatus, bool &m_bStatus);
    //void GetErrorMessage(session ulVi, CString &csErrString);

	short	DP_Enumeration_RFSigGen_Preset(session ulVi);
	short	DP_Enumeration_RFSigGen_ConfigRF(session ulVi ,double dFrequency, double dPowerLevel);
	short	DP_Enumeration_RFSigGen_ConfigALCEnabled (session ulVi,bool bAlcEnabled);
	short	DP_Enumeration_RFSigGen_ConfigOutputEnabled(session ulVi,bool bOutputEnabled);
	short	DP_Enumeration_RFSigGen_ConfigAMEnabled(session ulVi,bool bAmEnabled);
	short	DP_Enumeration_RFSigGen_ConfigAMExtCoupling(session ulVi,unsigned int in_uiAmExternalCoupling);
    short	DP_Enumeration_RFSigGen_ConfigAM( session ulVi, double in_dAMRate, double dDepth);
	short	DP_Enumeration_RFSigGen_ConfigFMEnabled(session ulVi,unsigned int uiFmEnabled);
	short	DP_Enumeration_RFSigGen_ConfigFMExtCoupling(session ulVi,unsigned int in_uiFmExternalCoupling);
	short	DP_Enumeration_RFSigGen_ConfigFM  (session ulVi,double in_dFMRate, double in_dDeviation);
	short	DP_Enumeration_RFSigGen_ConfigPMEnabled (session ulVi,bool bPmEnabled);
	short	DP_Enumeration_RFSigGen_ConfigPMExtCoupling (session ulVi, unsigned int in_uiPmExternalCoupling);
	short	DP_Enumeration_RFSigGen_ConfigPM (session ulVi,unsigned int in_uiIndex, double dDeviation);
	short	DP_Enumeration_RFSigGen_ConfigPulMEnabled (session ulVi,bool bPulseModulationEnabled);
	short	DP_Enumeration_RFSigGen_ConfigPulMSource (session ulVi,unsigned int in_uiSource);
	short	DP_Enumeration_RFSigGen_ConfigPulMExtPolarity(session ulVi,unsigned int in_uiExternalPolarity);
	short	DP_Enumeration_ConfigLFGen(session ulVi,unsigned int in_uiSrcIndex,unsigned int in_uiSrcActive,double dFrequency,unsigned int in_uiWaveform);
	short	DP_Enumeration_RFSigGen_ConfigLFGenOutput(session ulVi,double dAmplitude,bool bEnabled);
	short	DP_Enumeration_RFSigGen_ConfigPulExtTrg (session ulVi, unsigned int in_uiExternalSlope, double dDelay);
	short	DP_Enumeration_RFSigGen_ConfigPulIntTrg(session ulVi, double dPeriod);
	short	DP_Enumeration_RFSigGen_ConfigPul(session ulVi, unsigned int in_uiTriggerSource, double dPulseWidth, bool bGatingEnabled);
	short	DP_Enumeration_RFSigGen_ConfigPulDble(session ulVi, bool bEnabled, double dDelay);
	short	DP_Enumeration_RFSigGen_ConfigPulOutput(session ulVi, unsigned int in_uiPolarity, bool bEnabled);
	short	DP_Enumeration_RFSigGen_ConfigSweep(session ulVi, unsigned int in_uiMode, unsigned int in_uiTriggerSource);
	short	DP_Enumeration_RFSigGen_FreqSweepStrtStp(session ulVi, double dStart, double dStop);
	short	DP_Enumeration_RFSigGen_FreqSweepCenterSpan(session ulVi, double dCenter, double dSpan);
	short	DP_Enumeration_RFSigGen_FreqSweepTime(session ulVi, double dTime);
	short	DP_Enumeration_RFSigGen_PowerSweepStrtStp(session ulVi, double dStart, double dStop);
	short	DP_Enumeration_RFSigGen_PowerSweepTime(session ulVi, double dTime);
    short DP_Enumeration_RFSigGen_FreqStepStrtStp(session ulVi, double dstart, double dstop, double dDwell, unsigned int in_uiNoPoints );
    short	DP_Enumeration_RFSigGen_FreqStepStrtStp1(session ulVi, double dstart, double dstop, double dDwell, unsigned int in_uiNoPoints ,  double dPowerLevel);
	short	DP_Enumeration_ConfigFreqStepDwell(session ulVi,bool bEnabled, double dDwell);
	short	DP_Enumeration_RFSigGen_ResetFreqStep(session ulVi);
	short	DP_Enumeration_RFSigGen_ResetPowerStep(session ulVi);
	short	DP_Enumeration_RFSigGen_PwrStepStrtStp(session ulVi, double dStart, double dStop, unsigned int in_uiNoPoints);
	short	DP_Enumeration_RFSigGen_ConfigPowerStepDwell(session ulVi,bool bEnabled, double dDwell);
	short	DP_Enumeration_RFSigGen_ConfigRefOsc(session ulVi,unsigned int in_uiSource, double dFrequency);
	short	DP_Enumeration_RFSigGen_ConfigALC(session ulVi,unsigned int in_uiSource, double dBandwidth);
    short   DP_Enumeration_RFSigGen_Close(session ulVi);
    long    ConfigFreqStepSweep(session in_ulVi, double in_dStartFreq, double in_dStopFreq,  unsigned int in_uiNoPoints, double in_dDwellTime );
    short   EnableDisableFreqSweep(session in_ulVi, bool in_bEnaDisable);
    short   DisableAllModulation(session in_ulVi);
    long    Config_List_Dwell(session in_ulVi, char *strListDwell);
//	long Config_ListFreqPwr(session in_ulVi,ViConstString Name, ViInt32 Length,ViReal64 dFreq[6],ViReal64 dPwr[6]);
    short   Config_ListSweep(session in_ulVi);
    short   Run_Single_Sweep(session in_ulVi);
    short   Run_Continuous_Sweep(session in_ulVi);
    short   Stop_Continuous_Sweep(session in_ulVi);
    long    DP_Enumeration_RFSigGen_SelfTest(session ulVi, unsigned int uiTimedelay, short *out_sStatus);
    short   Config_ListConfiguration(session in_ulVi, double *dFreq, double *dPwr, float *fDwellTime, unsigned char in_ucListCount);
    ViStatus _VI_FUNC AgRfSigGen_GetAttributeViString ( ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViInt32 AttributeValueBufferSize, ViChar AttributeValue[] );
};
