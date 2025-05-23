/**************************************************************************
// File Name:  IVISpectrumAnalyserEnum.h
// Author:	    Venkatesh.P  	
// Created Date: 04-July-09
// Description: SpectrumAnalyserEnum Instrument IVI Enumeration Header File
**************************************************************************/

/**************************************************************************
// Company Name: Data Patterns (India) Pvt. Ltd. 
// Address: No.6, Arya Gowder Road, West Mambalam, Chennai,India - 600033
// Email: datapatterns@vsnl.com
// Phone: 91-44-4837460 / 4848702
// FAX: 91-44-4848703
**************************************************************************/


#if !defined(__ENUMERATION__OF_SPECTRUM_IVI_MODULES__)

#define __ENUMERATION__OF_SPECTRUM_IVI_MODULES__

class CIVI_SPECTRUM;

typedef  unsigned long session;
typedef unsigned long attr;
//typedef unsigned char unsigned char;

/************************************************************************************************

								Spectrum Analyser

					IVI Enumeration For Spectrum Analyser

*************************************************************************************************/
class CIVI_SPECTRUM
{

private :

	bool m_bOpenStatus;
	unsigned short m_usTotalInstances;
	char arrcErrStr[256];
	int iInstNum;	

public :
    unsigned short status;
	CIVI_SPECTRUM();	
	//parametric constructor 
	CIVI_SPECTRUM(unsigned short);		
	// IVI SpectrumAnalyser Instrument Open Function  Prototype
    short Open(char *pLogicalName, unsigned char ucInstNo);

	/************* SpectrumAnalyser Configuration ************/
	/* This function configures the frequency range defining its start frequency and its stop frequency */
	short DP_Enum_SpecAn_ConfigFreqStartStop (session in_uiVi, double in_dStartFreq, double in_dStopFreq);
	/* This function configures the frequency range of the spectrum analyzer */
    short DP_Enum_SpecAn_ConfigFreqCenterSpan (session in_uiVi, double in_dCenterFreq, double in_dSpan);
	/* This function configures the frequency offset of the spectrum analyzer */
	short DP_Enum_SpecAn_ConfigFreqOffset (session in_uiVi, double in_dFreqOffset);
	/* This function configures the coupling and sweeping attributes of the spectrum analyzer */
	short DP_Enum_SpecAn_ConfigSweepCoupling (session in_uiVi, bool in_bResolutionBWAuto, double in_dResolutionBW, bool in_bVideoBWAuto, double in_dVideoBW, bool in_bSweepTimeAuto, double in_dSweepTime, unsigned int in_uiNoOfSweepPts = 1001);
	/* This function configures the acquisition attributes of the spectrum analyzer */
	short DP_Enum_SpecAn_ConfigAcquisition (session in_uiVi, bool in_bSweepModeContinuous, signed long in_slNoOfSweeps, bool in_bDetecTypeAuto, signed long in_slDetecType, signed long in_slVerticalScale);
	/* This function configures the vertical settings of the spectrum analyzer */
	short DP_Enum_SpecAn_ConfigLevel (session in_uiVi, double in_dRefLevel);

	/****** SpectrumAnalyser Trigger Configuration ******************/
	
	/* The Trigger source that causes the spectrum analyzer to leave the Wait-for-Trigger state */
	short DP_Enum_SpecAn_ConfigTriggerSource (session in_uiVi, signed long in_slTriggerSource);
	/* This function specifies the external level and polarity for triggering */
	short DP_Enum_SpecAn_ConfigExtnlTrigger (session in_uiVi, double in_dExtnlTrigrLevel, signed long in_slExtnlTrigrPolarity);
	/* This function configures the internal trigger level of the power meter */
	short DP_Enum_SpecAn_ConfigVideoTrigger (session in_uiVi, double in_dVideoTrigrLevel, signed long in_slVideoTrigrSlope, float in_fTrigDelay);

	/*********** SpectrumAnalyser External Mixer Extension Configuration **************/

	/* This function enables external mixing */
	short DP_Enum_SpecAn_ConfigExtnlMixerEnabled (session in_uiVi, bool in_ExtnlMixingEnabled);
	/* This function specifies the number of external mixer ports */
	short DP_Enum_SpecAn_ConfigExtnlMixerNoOfPorts (session in_uiVi, signed long in_slNoofPorts);
	/* This function specifies the mixer harmonic and average conversion loss */
	short DP_Enum_SpecAn_ConfigExtnlMixer (session in_uiVi, signed long in_slHarmonic, double in_dAvgConverLoss);
	/* This function enables the external mixing bias */
	short DP_Enum_SpecAn_ConfigExtnlMixerBiasEnabled (session in_uiVi, bool in_bBiasEnabled);
	/* This function configures the external mixer bias and the external mixer bias limit */
	short DP_Enum_SpecAn_ConfigExternalMixerBias (session in_uiVi, double in_dBias, double in_dBiasLimit);
	/* This function enables the conversion loss table */
	short DP_Enum_SpecAn_ConfigConveLossTblEnabled (session in_uiVi, bool in_bConveLossTblEnb);
	/* This function configures the conversion loss table by specifying a series of frequency and a power loss pairs */
	short DP_Enum_SpecAn_ConfigConveLossTbl (session in_uiVi, signed long in_slCount, double*, double*);

	/*********** SpectrumAnalyser Measurement Configuration **************/
	//This function configures the type of trace to acquire
	short DP_Enum_SpecAn_ConfigureTraceType(session in_uiVi, unsigned short in_TraceIndex, signed long in_slTraceType);
	/* This function initiates a signal acquisition based on the present instrument configuration */
	short DP_Enum_SpecAn_ReadYTrace (session in_uiVi, unsigned short in_TraceIndex, signed long in_slMaxTime, signed long in_slArrayLength, double*);
	/* This function returns the trace the spectrum analyzer acquires */
	//short DP_Enum_SpecAn_FetchYTrace (session in_uiVi, unsigned short in_TraceIndex, signed long in_slArrayLength, ContainerArray* pContainerArray);
	short DP_Enum_SpecAn_FetchYTrace (session in_uiVi, unsigned short in_TraceIndex, signed long in_slArrayLength, double* dValue);
	
	/*********** SpectrumAnalyser Low Level Measurement Configuration **************/

	/* This function initiates an acquisition */
	short DP_Enum_SpecAn_Initiate (session in_uiVi);
	/* This function aborts a previously initiated measurement */
	short DP_Enum_SpecAn_Abort (session in_uiVi);
	/* This function determines if an acquisition is in progress or complete */
	short DP_Enum_SpecAn_AcquisitionSts (session in_uiVi, signed long *out_slAcquisSts);
	/* This function sends a command to trigger the spectrum analyzer */
 	short DP_Enum_SpecAn_SendSoftTrigger (session in_uiVi);
	/* added by priyanka */
        short DP_Enum_SpecAn_ReadActiveMarker(session in_uiVi, unsigned char in_ucMode, unsigned short in_TraceIndex, double in_dMarkerPos, double *out_dMarkerAmp);
	short DP_Enum_SpecAn_ModeConfig(session in_uiVi, unsigned short in_usModeNo);
	short DP_Enum_SpecAn_ReadNF(session in_uiVi,double *out_dNoiseVal,  double *out_dGainValue);
    short DP_Enum_SpecAn_ConfigureHarmonicDistortionMeasurement(session in_uiVi,unsigned char in_UCHARCnt, double dRefLevel, double dFundFreq, double in_dRBW, unsigned char in_ucAtten);
	short DP_Enum_SpecAn_ReadHarmonic(session in_uiVi,unsigned char in_ucmaximumTime_ms,	signed long harmonicDistArraySize,double *in_dharmonicDistTotalQ,double *out_Ampl1,double *out_Ampl2,signed long *in_plharmonicDistValuesRead);
	short DP_Enum_SpecAn_WaitForOperation_Complete(session in_uiVi);
	short DP_Enum_SpecAn_Config_Meas_Mode(session in_uiVi, unsigned short in_usSelection_Num);
    short DP_Enum_SpecAn_ConfigureSpurious(session in_uiVi, unsigned char in_ucRegNo);
	short DP_Enum_SpecAn_ReadSpuriousMeas(session in_uiVi,unsigned short in_usTotSamp, double*, double*, unsigned short* usCount);
	short DP_Enum_SpecAn_Save_Image(session in_uiVi,unsigned short usSpotNo);
	short DP_Enum_Read_Status(session in_uiVi, long *out_usstatus);
	short DP_Enum_SpecAn_Reset(session in_uiVi);


	//Added by shamnugam
//	short AgSA_ConfigAmpDUTSetup(session ulVi, float in_fStartFreq, float in_fStopFreq, int in_iNoOfPoints);
	short AgSA_ConfigAmpDUTSetup(session ulVi, double in_fStartFreq, double in_fStopFreq, int in_iNoOfPoints);
	short DP_Enum_SpecAn_ReadHarmonicValue(session in_uiVi,double*,double*);
	short Calibration_With_NoiseSoure(session in_uiVi);
	short Read_PhaseNoise(session in_uiVi, unsigned short in_TraceIndex, double in_dMarkerPos, double *out_dMarkerAmp);
	short ConfigAutoSweep(session in_uiVi);
	short ConfigMaxMarker(session in_uiVi,double* out_dAmplitude);
	short AgSA_SelfTest(session ulVi,short *out_sStatus);
	short AgSA_Close(session ulVi);
	short AgSA_ConfigDUTSetup(session ulVi, float in_fLOFreq, float in_fIFFreq);
    short AgSA_ConfigDUTSetup(session ulVi, float in_fLOFreq, float in_fIFFreq,unsigned char ucDUTModeSel);
    short AgSA_SetRFBTrig(session ulVi, unsigned char ucTrigType);
	short AgSA_ReadPeakValue(session ulVi, float* out_pfPeakValue);
    short AgSA_SetThersHold(session in_uiVi);
	short AgSA_ConfigMaxHold(session ulVi);
	short AgSA_ReadNextPeakValue(session ulVi, float* out_pfPeakValue, double* out_pdFreq);
	short DP_Enum_SpecAn_PeakThresholdOnOff(session in_uiVi, unsigned char in_ucOnOff);
	short DP_Enum_Mode_Preset(session in_uiVi);
	short DP_Enum_Set_Scale(session in_uiVi,double in_dScale);
	short DP_Enum_Delta_Marker_EnableDisbale(session in_uiVi,bool in_bEnableDisable);
	short DP_Enum_SpecAn_Config_AutoAlign(session in_uiVi, unsigned char in_ucOnOff);
	short DP_Enum_SpecAn_ContRun(session in_uiVi, unsigned char in_ucOnOff);
	short TOI_ConfigFreqCenterSpan(session in_uiVi, double in_dCenterFreq, double in_dSpan);
	short TOI_ConfigSweepCoupling(session in_uiVi, bool in_bResolutionBWAuto, double in_dResolutionBW, bool in_bVideoBWAuto, double in_dVideoBW, bool in_bSweepTimeAuto, double in_dSweepTime);
	short TOI_ConfigLevel (session in_uiVi,double in_dRefLevel);
	short TOI_ReadValues(session in_uiVi,unsigned int in_uiTOIDelay, double* out_pdUpFreq, double* out_pdUpTOI, double* out_pdLowFreq, double* out_pdLowTOI);
	short TOI_SetScale(session in_uiVi,double in_dScale);
	short TOI_MechAttenuation(session in_uiVi,double in_dMechAtt);
	short AgSA_NoiseSourceDetection(session ulVi,short *out_sStatus);
    short AgSA_PreAmpEnableDisable(session ulVi,unsigned char in_ucSts);
	short DP_Enum_SpecAn_ConfigRBW_VBW(session in_uiVi, double in_dResolutionBW,  double in_dVideoBW);
	short AgSA_ReadPeakValue(session ulVi, double *out_pdFreq, float* out_pfPeakValue);
	short AgSA_ConfigDownConvDUTSetup(session ulVi, float in_fStartFreq, float in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints);
	short AgSA_ConfigNFDownConvFreqMode(session ulVi);
	short AgSA_ConfigUpConvDUTSetup(session ulVi, float in_fStartFreq, float in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints);
	short DP_Enum_SpecAn_ReadConfigSweep(session in_uiVi, double *out_dSweepTime);

///	short AgSA_ConfigDownConvDUTSetup(session ulVi, float in_fStartFreq, float in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);
    short AgSA_ConfigDownConvDUTSetup(session ulVi, double in_fStartFreq, double in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);

//	short AgSA_ConfigUpConvDUTSetup(session ulVi, float in_fStartFreq, float in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);
    short AgSA_ConfigUpConvDUTSetup(session ulVi, double in_fStartFreq, double in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);

    short AgSA_ConfigDownConvDUTSetup(session ulVi, unsigned char in_ucFreqMode, double in_fStartFreq, double in_fStopFreq, unsigned char in_ucLOFreqMode, float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);

	short DP_Enum_SpecAn_ConfigAutoSweepCoupling(session in_uiVi, double in_dResolutionBW);
    short AgSA_UpConvDUTSetupConfig(session ulVi,unsigned char in_ucFreqMode,double in_fStartFreq, double in_fStopFreq,unsigned char in_ucLOFreqMode, float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);

    short AgSA_DownConvDUTSetupConfig(session ulVi, double in_fStartFreq, double in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);
    short AgSA_DUTSetupConfig(session ulVi, float in_fLOFreq, float in_fIFFreq,unsigned char ucDUTModeSel);
	short AgSA_AmpDUTSetupConfig(session ulVi, double in_fStartFreq, double in_fStopFreq, int in_iNoOfPoints);
	short DP_Enum_SpecAn_NoiFigModeConfig(session in_uiVi, unsigned short in_usModeNo);
	short DP_Enum_ConfigureVideoTrigger(session in_uiVi, double in_dVideoTriggerLevel,int in_iVideoTriggerSlope);
	short DP_Enum_ConfigureFreeRun(session in_uiVi);

	short CHP_ConfigFreqCenterSpan(session in_uiVi, double in_dCenterFreq, double in_dSpan);
	short CHP_ConfigSweepCoupling(session in_uiVi, bool in_bResolutionBWAuto, double in_dResolutionBW, bool in_bVideoBWAuto, double in_dVideoBW, bool in_bSweepTimeAuto, double in_dSweepTime, double in_IntBandwidth);
	short CHP_ReadPwrSpecDensity(session in_uiVi, double* out_pdCHPower, double* out_pdPwrSpecDensity);
};
#endif
