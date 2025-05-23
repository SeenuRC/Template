#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "../include/dp_types.h"
#include "visa.h"
#include "visatype.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#define DP_SPECAN_ENABLE 1
#define DP_SPECAN_DISABLE 0
#define DP_SPEC_AN_CNFG_SUCCESS 0
#define NOISE_FIGURE_MODE		219
#define PHASE_NOISE_MODE					14
#define DP_HARMONICS_COUNT 3
#define SPECTRUM_MODE   					1

#define SKEY8257D 2
#define SAE8267D 1
#define SN9030B  2


typedef  unsigned char session;
typedef unsigned long attr;


///*********************DP SPECTRUM ANALYZER WRAPPER ERROR CODES**********************/
enum{
    DP_VI_OPEN_FAILURE = -999,
    DP_SPECAN_CLS_FAILURE,
    DP_SPECAN_CNFG_FREQSTART_FAILURE,
    DP_SPECAN_CNFG_FREQSTOP_FAILURE,
    DP_SPECAN_CNFG_FREQCENTER_FAILURE,
    DP_SPECAN_CNFG_FREQSPAN_FAILURE,
    DP_SPECAN_CNFG_FREQOFFSET_FAILURE,
    DP_SPECAN_CNFG_BWAUTO_FAILURE,
    DP_SPECAN_CNFG_RESBW_FAILURE,
    DP_SPECAN_CNFG_VIDBWAUTO_FAILURE,
    DP_SPECAN_CNFG_VIDBW_FAILURE,
    DP_SPECAN_CNFG_SWTIMEAUTO_FAILURE,
    DP_SPECAN_CNFG_SWTIME_FAILURE,
    DP_SPECAN_READ_SWTIME_FAILURE,
    DP_SPECAN_CNFG_REFLVL_FAILURE,
    DP_SPECAN_DET_HAR_FAILURE,
    DP_SPECAN_CNFG_TRIGSOUR_FAILURE,
    DP_SPECAN_CNFG_TRIGLVL_FAILURE,
    DP_SPECAN_CNFG_TRIGPOL_FAILURE,
    DP_SPECAN_CNFG_TRIGSLOPE_FAILURE,
    DP_SPECAN_CNFG_TRIGDELAY_FAILURE,
    DP_SPECAN_CNFG_MIXHARM_FAILURE,
    DP_SPECAN_CNFG_MIXBIAS_FAILURE,
    DP_SPECAN_READ_ACTIVEMARKER_FAILURE,
    DP_SPECAN_READ_PHASENOISE_FAILURE,
    DP_SPECAN_CNFG_MODE_FAILURE,
    DP_SA_LOAD_FILE_FAILURE,
    DP_SPECAN_CNFG_FREQ_FAILURE,
    DP_SPECAN_CNFG_PEAK_SRCH_FAILURE,
    DP_SPECAN_NS_CALIB_FAILURE,
    DP_SPECAN_NS_SETTLING_TIME_FAILURE,
    DP_SPECAN_CNFG_PEAK_EXC_FAILURE,
    DP_SPECAN_CNFG_TRACE_FAILURE,
    DP_SPECAN_READ_NF_FAILURE,
    DP_SPECAN_HARDIS_MEAS_FAILURE,
    DP_SPECAN_HARCNT_FAILURE,
    DP_SPECAN_DRBW_FAILURE,
    DP_SPECAN_FUNFREQ_FAILURE,
    DP_SPECAN_READ_HARVAL_FAILURE,
    DP_SPECAN_READ_HARFREQ_FAILURE,
    DP_SPECAN_CNFG_SPUR_FAILURE,
    DP_SPECAN_RESET_FAILURE,
    DP_SPECAN_INIT_FAILURE,
    DP_SPECAN_ABORT_FAILURE,
    DP_SPECAN_NS_FAILURE,
    DP_SPECAN_PHASE_NS_FAILURE,
    DP_SPECAN_PHASE_NSAT_FAILURE,
    DP_SPECAN_PHASENOISE_MM_FAILURE,
    DP_SPECAN_CNFG_AUTO_SWEEP_FAILURE,
    DP_SPECAN_READ_MARKER_FAILURE,
    DP_SPECAN_PEAK_ON_FAILURE,
    DP_SPECAN_PEAK_OFF_FAILURE,
    DP_SPECAN_CNFG_MARAMP_FAILURE,
    DP_SPECAN_PRESET_FAILURE,
    DP_SPECAN_SETSCALE_FAILURE,
    DP_SPECAN_CNFG_AUTO_ON_FAILURE,
    DP_SPECAN_CNFG_AUTO_OFF_FAILURE,
    DP_SPECAN_CNFG_DUT_SETUP_FAILURE,
    DP_SPECAN_AMP_DUT_SETUP_FAILURE,
    DP_SPECAN_CONT_ON_FAILURE,
    DP_SPECAN_CONT_OFF_FAILURE,
    DP_SPECAN_NS_DET_FAILURE,
    DP_SPECAN_PREAMP_ENADIS_FAILURE,
    DP_SPECAN_RBW_VBW_FAILURE,
    DP_SPECAN_UPCON_DUT_SETUP_FAILURE,
    DP_SPECAN_DUT_UPCON_SETUP_FAILURE,
    DP_SPECAN_DUT_DOWNCON_SETUP_FAILURE,
    DP_SPECAN_CNFG_RBWVBW_FAILURE,
    DP_SPECAN_READ_ID_ERROR,
    DP_SPECAN_RD_ACTMAR_FAILURE,
    DP_SPECAN_VID_TRIG_FAILURE,
    DP_SPECAN_FREE_RUN_FAILURE,
    DP_SPECAN_CNFG_VIDTRIG_FAILURE,
    DP_SPECAN_DUT_DOWNCON_FAILURE,
    DP_SPECAN_DOWNCON_DUT_FAILURE,
    DP_ERR_INVALID_PARAM,
    DP_SPECAN_INT_INVALID_MODEL,
    DP_SPECAN_INT_ID_READ_FAILED
};
typedef struct _SAInsconfg
{
    char m_szManufacturer[64];
    char m_szmodel[32];
    unsigned short m_usInstID;
}SAInsconfg;

typedef struct _SInstHandle
{
    ViSession m_viSASession;
    unsigned short m_usInstID;
}SInstHandle;


class CSAWrapper
{
private:

    bool m_bOpenStatus;
    unsigned short m_usTotalInstances;
    char arrcErrStr[256];
    int iInstNum;

public:

    ViStatus m_viStatus;
    char m_cSAInfo[50];
    ViSession m_viRFSA[16];
    CSAWrapper();
    SAInsconfg m_SAInsconfg[3];
    SInstHandle m_arrSInstHandle[16];
    // IVI SpectrumAnalyser Instrument Open Function  Prototype
       short Open(unsigned char in_u8InstNo, char *pLogicalName);

    /************* SpectrumAnalyser Configuration ************/
    /* This function configures the frequency range defining its start frequency and its stop frequency */
       short DP_Enum_SpecAn_ConfigFreqStartStop (session in_uiVi, double in_dStartFreq, double in_dStopFreq);
    /* This function configures the frequency range of the spectrum analyzer */
       short DP_Enum_SpecAn_ConfigFreqCenterSpan (session in_uiVi, double in_dCenterFreq, double in_dSpan);
    /* This function configures the frequency offset of the spectrum analyzer */
       short DP_Enum_SpecAn_ConfigFreqOffset (session in_uiVi, double in_dFreqOffset);
    /* This function configures the coupling and sweeping attributes of the spectrum analyzer */
       short DP_Enum_SpecAn_ConfigSweepCoupling (session in_uiVi, bool in_bResolutionBWAuto, double in_dResolutionBW, bool in_bVideoBWAuto, double in_dVideoBW, bool in_bSweepTimeAuto, double in_dSweepTime);
    /* This function configures the acquisition attributes of the spectrum analyzer */
//       short DP_Enum_SpecAn_ConfigAcquisition (session in_uiVi, bool in_bSweepModeContinuous, signed long in_slNoOfSweeps, bool in_bDetecTypeAuto, signed long in_slDetecType, signed long in_slVerticalScale);
    /* This function configures the vertical settings of the spectrum analyzer */
       short DP_Enum_SpecAn_ConfigLevel (session in_uiVi, double in_dRefLevel);

    /****** SpectrumAnalyser Trigger Configuration ******************/

    /* The Trigger source that causes the spectrum analyzer to leave the Wait-for-Trigger state */
       short DP_Enum_SpecAn_ConfigTriggerSource (session in_uiVi, signed long in_slTriggerSource);
    /* This function specifies the external level and polarity for triggering */
       //short DP_Enum_SpecAn_ConfigExtnlTrigger (session in_uiVi, double in_dExtnlTrigrLevel, signed long in_slExtnlTrigrPolarity);
       short DP_Enum_SpecAn_ConfigExtnlTrigger (session in_uiVi);
    /* This function configures the internal trigger level of the power meter */
       short DP_Enum_SpecAn_ConfigVideoTrigger (session in_uiVi, double in_dVideoTrigrLevel, signed long in_slVideoTrigrSlope, float in_fTrigDelay);

    /*********** SpectrumAnalyser External Mixer Extension Configuration **************/

    /* This function enables external mixing */
//       short DP_Enum_SpecAn_ConfigExtnlMixerEnabled (session in_uiVi, bool in_ExtnlMixingEnabled);
    /* This function specifies the number of external mixer ports */
//       short DP_Enum_SpecAn_ConfigExtnlMixerNoOfPorts (session in_uiVi, signed long in_slNoofPorts);
    /* This function specifies the mixer harmonic and average conversion loss */
       short DP_Enum_SpecAn_ConfigExtnlMixer (session in_uiVi, signed long in_slHarmonic, double in_dAvgConverLoss);
    /* This function enables the external mixing bias */
       short DP_Enum_SpecAn_ConfigExtnlMixerBiasEnabled (session in_uiVi, bool in_bBiasEnabled);
    /* This function configures the external mixer bias and the external mixer bias limit */
//       short DP_Enum_SpecAn_ConfigExternalMixerBias (session in_uiVi, double in_dBias, double in_dBiasLimit);
    /* This function enables the conversion loss table */
//       short DP_Enum_SpecAn_ConfigConveLossTblEnabled (session in_uiVi, bool in_bConveLossTblEnb);
    /* This function configures the conversion loss table by specifying a series of frequency and a power loss pairs */
//       short DP_Enum_SpecAn_ConfigConveLossTbl (session in_uiVi, signed long in_slCount, double*, double*);

    /*********** SpectrumAnalyser Measurement Configuration **************/
    //This function configures the type of trace to acquire
//       short DP_Enum_SpecAn_ConfigureTraceType(session in_uiVi, unsigned short in_TraceIndex, signed long in_slTraceType);
    /* This function initiates a signal acquisition based on the present instrument configuration */
//       short DP_Enum_SpecAn_ReadYTrace (session in_uiVi, unsigned short in_TraceIndex, signed long in_slMaxTime, signed long in_slArrayLength, double*);
    /* This function returns the trace the spectrum analyzer acquires */
    //short DP_Enum_SpecAn_FetchYTrace (session in_uiVi, unsigned short in_TraceIndex, signed long in_slArrayLength, ContainerArray* pContainerArray);
       short DP_Enum_SpecAn_FetchYTrace (session in_uiVi, unsigned short in_TraceIndex, signed long in_slArrayLength, double* dValue);

    /*********** SpectrumAnalyser Low Level Measurement Configuration **************/

    /* This function initiates an acquisition */
       short DP_Enum_SpecAn_Initiate (session in_uiVi);
    /* This function aborts a previously initiated measurement */
       short DP_Enum_SpecAn_Abort (session in_uiVi);
    /* This function determines if an acquisition is in progress or complete */
//       short DP_Enum_SpecAn_AcquisitionSts (session in_uiVi, signed long *out_slAcquisSts);
    /* This function sends a command to trigger the spectrum analyzer */
//       short DP_Enum_SpecAn_SendSoftTrigger (session in_uiVi);
    /* added by priyanka */
    //   short DP_Enum_SpecAn_ReadActiveMarker(session in_uiVi, unsigned char in_ucMode, unsigned short in_TraceIndex, double in_dMarkerPos, double *out_dMarkerAmp);
       short DP_Enum_SpecAn_ReadActiveMarker(session in_uiVi, double in_dMarkerPos, double *out_dMarkerAmp);
       short DP_Enum_SpecAn_ModeConfig(session in_uiVi, unsigned short in_usModeNo);
       short DP_Enum_SpecAn_ReadNF(session in_uiVi,double *out_dNoiseVal,  double *out_dGainValue, double *out_dNoiseTemp);
       short DP_Enum_SpecAn_ConfigureHarmonicDistortionMeasurement(session in_uiVi,unsigned char in_UCHARCnt, double dRefLevel, double dFundFreq, double in_dRBW, unsigned char in_ucAtten);
       short DP_Enum_SpecAn_ReadHarmonic(session in_uiVi,unsigned char in_ucmaximumTime_ms,	signed long harmonicDistArraySize,double *in_dharmonicDistTotalQ,double *out_Ampl1,double *out_Ampl2,signed long *in_plharmonicDistValuesRead);
       short DP_Enum_SpecAn_WaitForOperation_Complete(session in_uiVi, U32BIT in_u32TimeOutinMS);
   //    short DP_Enum_SpecAn_Config_Meas_Mode(session in_uiVi, unsigned short in_usSelection_Num);
       short DP_Enum_SpecAn_ConfigureSpurious(session in_uiVi, unsigned char in_ucRegNo);
       short DP_Enum_SpecAn_ReadSpuriousMeas(session in_uiVi,unsigned short in_usTotSamp, double*, double*, unsigned short* usCount);
       short DP_Enum_SpecAn_Save_Image(session in_uiVi,unsigned short usSpotNo);
//       short DP_Enum_Read_Status(session in_uiVi, long *out_usstatus);
       short DP_Enum_SpecAn_Reset(session in_uiVi);

    //Added by shamnugam
//    short AgSA_ConfigAmpDUTSetup(session in_uiVi, float in_fStartFreq, float in_fStopFreq, int in_iNoOfPoints);
       short AgSA_ConfigAmpDUTSetup(session in_uiVi, double in_fStartFreq, double in_fStopFreq, int in_iNoOfPoints);
       short DP_Enum_SpecAn_ReadHarmonicValue(session in_uiVi,double*,double*);
       short Calibration_With_NoiseSoure(session in_uiVi, U32BIT in_u32TimeOutinMS);
       short Read_PhaseNoise(session in_uiVi, unsigned short in_TraceIndex, double in_dMarkerPos, double *out_dMarkerAmp);
       short ConfigAutoSweep(session in_uiVi);
       short ConfigMaxMarker(session in_uiVi,double* out_dAmplitude);
//       short AgSA_SelfTest(session in_uiVi,short *out_sStatus);
       short DP_Enum_SpecAn_Close(session in_uiVi);
       short AgSA_ConfigDUTSetup(session in_uiVi, float in_fLOFreq, float in_fIFFreq);
       short AgSA_ConfigDUTSetup(session in_uiVi, float in_fLOFreq, float in_fIFFreq,unsigned char ucDUTModeSel);
       short AgSA_SetRFBTrig(session in_uiVi, unsigned char ucTrigType);
       short AgSA_ReadPeakValue(session in_uiVi, float* out_pfPeakValue);
       short AgSA_ConfigMaxHold(session in_uiVi);
       short AgSA_ReadNextPeakValue(session in_uiVi, float* out_pfPeakValue, double* out_pdFreq);
       short DP_Enum_SpecAn_PeakThresholdOnOff(session in_uiVi, unsigned char in_ucOnOff);
       short DP_Enum_Mode_Preset(session in_uiVi);
       short DP_Enum_Set_Scale(session in_uiVi,double in_dScale);
//       short DP_Enum_Delta_Marker_EnableDisbale(session in_uiVi,bool in_bEnableDisable);
       short DP_Enum_SpecAn_Config_AutoAlign(session in_uiVi, unsigned char in_ucOnOff);
       short DP_Enum_SpecAn_ContRun(session in_uiVi, unsigned char in_ucOnOff);
       short DP_Enum_SpecAn_SingleRun(session in_uiVi, unsigned char in_ucOnOff);
//       short TOI_ConfigFreqCenterSpan(session in_uiVi, double in_dCenterFreq, double in_dSpan);
//       short TOI_ConfigSweepCoupling(session in_uiVi, bool in_bResolutionBWAuto, double in_dResolutionBW, bool in_bVideoBWAuto, double in_dVideoBW, bool in_bSweepTimeAuto, double in_dSweepTime);
//       short TOI_ConfigLevel (session in_uiVi,double in_dRefLevel);
//       short TOI_ReadValues(session in_uiVi,unsigned int in_uiTOIDelay, double* out_pdUpFreq, double* out_pdUpTOI, double* out_pdLowFreq, double* out_pdLowTOI);
//       short TOI_SetScale(session in_uiVi,double in_dScale);
//       short TOI_MechAttenuation(session in_uiVi,double in_dMechAtt);
       short AgSA_NoiseSourceDetection(session in_uiVi,short *out_sStatus);
       short AgSA_PreAmpEnableDisable(session in_uiVi,unsigned char in_ucSts);
       short DP_Enum_SpecAn_ConfigRBW_VBW(session in_uiVi, double in_dResolutionBW,  double in_dVideoBW);
       short DP_Enum_SpecAn_ReadPeakValue(session in_uiVi, double *out_pdFreq, double* out_pfPeakValue);
 //      short AgSA_ConfigDownConvDUTSetup(session in_uiVi, float in_fStartFreq, float in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints);
        short DP_Enum_SpecAn_NFConfigBW(U8BIT in_u8InstNo, double in_dBandWidth);
       short AgSA_ConfigNFDownConvFreqMode(session in_uiVi);
//       short AgSA_ConfigUpConvDUTSetup(session in_uiVi, float in_fStartFreq, float in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints);
       short DP_Enum_SpecAn_ReadConfigSweep(session in_uiVi, double *out_dSweepTime);

//	short AgSA_ConfigDownConvDUTSetup(session in_uiVi, float in_fStartFreq, float in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);
//       short AgSA_ConfigDownConvDUTSetup(session in_uiVi, double in_fStartFreq, double in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);

//    short AgSA_ConfigUpConvDUTSetup(session in_uiVi, float in_fStartFreq, float in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);
       short AgSA_ConfigUpConvDUTSetup(session in_uiVi, double in_fStartFreq, double in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);

       short AgSA_ConfigDownConvDUTSetup(session in_uiVi, unsigned char in_ucFreqMode, double in_fStartFreq, double in_fStopFreq, unsigned char in_ucLOFreqMode, double in_dLOFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag, unsigned char in_ucAvgSamples);

//       short DP_Enum_SpecAn_ConfigAutoSweepCoupling(session in_uiVi, double in_dResolutionBW);
       short AgSA_UpConvDUTSetupConfig(session in_uiVi,unsigned char in_ucFreqMode,double in_fStartFreq, double in_fStopFreq,unsigned char in_ucLOFreqMode, float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);

    //   short AgSA_DownConvDUTSetupConfig(session in_uiVi, double in_fStartFreq, double in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag);
       short AgSA_DUTSetupConfig(session in_uiVi, float in_fLOFreq, float in_fIFFreq,unsigned char ucDUTModeSel);
       short AgSA_AmpDUTSetupConfig(session in_uiVi, double in_fStartFreq, double in_fStopFreq, double in_dStepFreq, int in_iNoOfPoints, unsigned char in_ucAvgSamples);
//       short DP_Enum_SpecAn_NoiFigModeConfig(session in_uiVi, unsigned short in_usModeNo);
       short DP_Enum_ConfigureVideoTrigger(session in_uiVi, double in_dVideoTriggerLevel,int in_iVideoTriggerSlope);
       short DP_Enum_ConfigureFreeRun(session in_uiVi);

       short DP_SPEC_AN_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT U16BufSize);

       short DP_Enum_SpecAn_ReadHarmonicMeasurement(session in_uiVi, unsigned char in_ucHarCnt, double *dFundFreq, double *dPower);

       short DP_Enum_SpecAn_HarmonicMeasurement(session in_uiVi);

//       short DP_Enum_SpecAn_ConfigPNMeasurement(session in_uiVi,double in_dCenterFrequency, double in_dRefFreq,  double in_dRefLvl, double in_dSpan);
       short DP_Enum_SpecAn_ConfigPNMeasurement(U8BIT in_u8InstNo,double in_dCenterFrequency, double in_dStartOffset, double in_dStopOffset,  double in_dRefLvl, double in_dSpan);
       short DP_Enum_SpecAn_ReadPN(session in_uiVi, double in_dMarFreq, double *out_dPower);


    short GetInstID(char *in_pcInstDetails, unsigned short  *Out_usModel);

    short DP_SA_StoreStateIntoFile(U8BIT in_u8InstNo, char* in_csFileName);

    short DP_SA_LoadStateFromFile(U8BIT in_u8InstNo, char* strFileName);

    short AgSA_SetThersHold(U8BIT in_u8InstNo, bool in_bThreshotState, double in_dThreshold);

};

#endif // SPECTRUM_H
