#ifndef _DP_CONFIG_STRUCT_H_
#define _DP_CONFIG_STRUCT_H_

#include <QString>
#include "../include/dp_types.h"
#include <QVector>

#define DB_SQL_CONN         "DB_Conn"

typedef struct
{
    U32BIT			m_ulSessionID;
    U8BIT			m_ucTestType;
    QString			m_strSystemSNo;
    QString			m_strATESNo;
    QString			m_strTestEnv;
    QString			m_strTester;
    QString			m_strStage;
    QString			m_strTemperature;
    QString			m_strLoginUserName;
    QString			m_strSysName;
    QString			m_strTestLocation;
}S_TEST_REGISTER;


/* User details Structure*/
typedef struct
{
    S32BIT	lSessionID;
    S8BIT   qsUserID[50];
    S8BIT	qsUserName[50];
    S8BIT	qsPassword[50];
    S32BIT 	sPrivilege;

}S_USER_DETAILS;

typedef struct _S_SBANDGANPAMODULE_INPUTRETURNLOSSMEASUREMENT
{
        FDOUBLE m_dStartFrequency;
        FDOUBLE m_dStopFrequency;
        FDOUBLE m_dInputPower;
        U16BIT m_u16NoofPoints;
        FDOUBLE m_dExpectedMaxReturnLoss;
        FDOUBLE m_dStepFrequency;
} S_SBANDGANPAMODULE_INPUTRETURNLOSSMEASUREMENT, *PS_SBANDGANPAMODULE_INPUTRETURNLOSSMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_OUTPUTRETURNLOSSMEASUREMENT
{
        FDOUBLE m_dStartFrequency;
        FDOUBLE m_dStopFrequency;
        FDOUBLE m_dInputPower;
        U16BIT m_u16NoofPoints;
        FDOUBLE m_dStepFrequency;
        FDOUBLE m_dExpectedMaxReturnLoss;
} S_SBANDGANPAMODULE_OUTPUTRETURNLOSSMEASUREMENT, *PS_SBANDGANPAMODULE_OUTPUTRETURNLOSSMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_RXISOLATIONMEASUREMENT
{
        FDOUBLE m_dStartFrequency;
        FDOUBLE m_dStopFrequency;
        FDOUBLE m_dInputPower;
        U16BIT m_u16NoofPoints;
        FDOUBLE m_dStepFrequency;
        FDOUBLE m_dExpMaxIsolation;
} S_SBANDGANPAMODULE_RXISOLATIONMEASUREMENT, *PS_SBANDGANPAMODULE_RXISOLATIONMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_RXGAINMEASUREMENT
{
    FDOUBLE m_dStartFrequency;
    FDOUBLE m_dStopFrequency;
    FDOUBLE m_dInputPower;
    U16BIT m_u16NoofPoints;
    FDOUBLE m_dStepFrequency;
    FDOUBLE m_dExpectedMinGain;
    FDOUBLE m_dExpectedMaxGain;
} S_SBANDGANPAMODULE_RXGAINMEASUREMENT, *PS_SBANDGANPAMODULE_RXGAINMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_NOISEFIGUREMEASUREMENT
{
    FDOUBLE m_dStartFrequency;
    FDOUBLE m_dStopFrequency;
    U16BIT m_u16NoofPoints;
    FDOUBLE m_dInputPower;
    FDOUBLE m_ExpectedNoiseFigure;
} S_SBANDGANPAMODULE_NOISEFIGUREMEASUREMENT, *PS_SBANDGANPAMODULE_NOISEFIGUREMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_TXOUTPUTPOWERMEASUREMENT
{
    FDOUBLE m_dStartFrequency;
    FDOUBLE m_dStopFrequency;
    FDOUBLE m_dStepFrequency;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dXStart;
    FDOUBLE m_dXScaleStep;
    FDOUBLE m_dYMax;
    FDOUBLE m_dYScale;
    FDOUBLE m_dMarker1;
    FDOUBLE m_dMarker2;
    U16BIT m_u16RFInputPathID;
    FDOUBLE m_dExpMinOutPower;
    FDOUBLE m_dExpMaxOutPower;
    FDOUBLE m_dPulsePeriod;
    FDOUBLE m_dPulseWidth;
    FDOUBLE m_dHighLevel;
    FDOUBLE m_dLowLevel;
}S_SBANDGANPAMODULE_TXOUTPUTPOWERMEASUREMENT, *PS_SBANDGANPAMODULE_TXOUTPUTPOWERMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_TXHARMONICSMEASUREMENT
{
    U16BIT m_u16RFInputPathID;
    FDOUBLE m_dStartFrequency;
    FDOUBLE m_dStopFrequency;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dRBW;
    FDOUBLE m_dVBW;
    FDOUBLE m_dSpan;
    FDOUBLE m_dReferenceLevel;
    FDOUBLE m_dStepFrequency;
    U16BIT m_u16RFOutputPathID;
    FDOUBLE m_dSweepTime;
    FDOUBLE m_dExpectedHarmonicsRejection;
    FDOUBLE m_dVideoTrigLevel;
    FDOUBLE m_dMarkerPos;
    FDOUBLE m_dVideoTrigLevel2ndOrder;
    FDOUBLE m_dVideoTrigLevel3rdOrder;
    FDOUBLE m_dReferenceLevel2ndOrder;
    S8BIT m_s8ReferenceLevel3rdOrder;
    FDOUBLE m_dPulseWidth;
    FDOUBLE m_dPulsePeriod;
    FDOUBLE m_dHighLevel;
    FDOUBLE m_dLowLevel;
} S_SBANDGANPAMODULE_TXHARMONICSMEASUREMENT, *PS_SBANDGANPAMODULE_TXHARMONICSMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_POWERDROOPMEASUREMENT
{
    FDOUBLE m_dStartFrequency;
    FDOUBLE m_dStopFrequency;
    FDOUBLE m_dStepFrequency;
    U16BIT m_u16RFInputPathLoss;
    FDOUBLE m_dReferenceLevel;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dExpectedPowerDroop;
    FDOUBLE m_dXScaleStart;
    FDOUBLE m_dXScaleStep;
    FDOUBLE m_dYScaleMax;
    FDOUBLE m_dMarker1;
    FDOUBLE m_dMarker2;
    FDOUBLE m_dTrigger;
    FDOUBLE m_dYScale;    
    FDOUBLE m_dPulseWidth;
    FDOUBLE m_dPulsePeriod;
    FDOUBLE m_dHighLevel;
    FDOUBLE m_dLowLevel;
} S_SBANDGANPAMODULE_POWERDROOPMEASUREMENT, *PS_SBANDGANPAMODULE_POWERDROOPMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_RFPULSERISEORFALLTIMETEST
{
    FDOUBLE m_dStartFrequency;
    FDOUBLE m_dStopFrequency;
    FDOUBLE m_dStepFrequency;
    FDOUBLE m_dInputPower;
    U16BIT m_u16RFInputPathID;
    FDOUBLE m_dXStartRT;
    FDOUBLE m_dXScaleRT;
    FDOUBLE m_dYMaxRT;
    FDOUBLE m_dYScaleRT;
    FDOUBLE m_dXStartFT;
    FDOUBLE m_dXScaleFT;
    FDOUBLE m_dYMaxFT;
    FDOUBLE m_dYScaleFT;
    FDOUBLE m_dExpectedRiseFallTime;
    FDOUBLE m_dPulseWidth;
    FDOUBLE m_dPulsePeriod;
    FDOUBLE m_dHighLevel;
    FDOUBLE m_dLowLevel;
} S_SBANDGANPAMODULE_RFPULSERISEORFALLTIMETEST, *PS_SBANDGANPAMODULE_RFPULSERISEORFALLTIMETEST;

typedef struct _S_SBANDGANPAMODULE_CABLELOSSCALIBRATION
{
        FDOUBLE m_dStartFrequency;
        FDOUBLE m_dStopFrequency;
        FDOUBLE m_dStepFrequency;
        FDOUBLE m_dInputPower;
        S8BIT m_s8PathID;
        FDOUBLE m_dSpan;
        FDOUBLE m_dRBW;
        FDOUBLE m_dVBW;
        S8BIT m_s8SweepTime;
        FDOUBLE m_dReferenceLevel;
        FDOUBLE m_dExpMinLoss;
        FDOUBLE m_dExpMaxLoss;
} S_SBANDGANPAMODULE_CABLELOSSCALIBRATION, *PS_SBANDGANPAMODULE_CABLELOSSCALIBRATION;

typedef struct _S_SBANDGANPAMODULE_POWERTUNINGMEASUREMENT
{
        FDOUBLE m_dStartFrequency;
        FDOUBLE m_dStopFrequency;
        FDOUBLE m_dStepFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dXStart;
        FDOUBLE m_dXScaleStep;
        FDOUBLE m_dYMax;
        FDOUBLE m_dYScale;
        FDOUBLE m_dMarker1;
        FDOUBLE m_dMarker2;
        FDOUBLE m_dRFInputPathID;
        FDOUBLE m_dExpMinOutPower;
        FDOUBLE m_dExpMaxOutPower;
        FDOUBLE m_dPulseWidth;
        FDOUBLE m_dPulsePeriod;
        FDOUBLE m_dLowLevel;
        FDOUBLE m_dHighLevel;
        FDOUBLE m_dReadPeakDelay;
} S_SBANDGANPAMODULE_POWERTUNINGMEASUREMENT, *PS_SBANDGANPAMODULE_POWERTUNINGMEASUREMENT;

typedef struct _S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION_CONFIGURATIONTABLE
{
        FDOUBLE m_dFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dExpectedRejection;
} S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION_CONFIGURATIONTABLE, *PS_SBANDGANPAMODULE_RXOUTOFBANDREJECTION_CONFIGURATIONTABLE;

typedef struct _S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION
{
        FDOUBLE m_dInputPower;
        QVector<S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION_CONFIGURATIONTABLE> m_vectCONFIGURATIONTABLE;
        U16BIT m_u16NumberOfPoints;
        FDOUBLE m_dCalibStartFrequency;
        FDOUBLE m_dCalibStopFrequency;
} S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION, *PS_SBANDGANPAMODULE_RXOUTOFBANDREJECTION;

typedef struct _S_MODULENAME_PHASENOISEMEASUREMENT_FREQUENCYTABLE
{
    FDOUBLE m_dInputFrequency;
} S_MODULENAME_PHASENOISEMEASUREMENT_FREQUENCYTABLE, *PS_MODULENAME_PHASENOISEMEASUREMENT_FREQUENCYTABLE;

typedef struct _S_MODULENAME_PHASENOISEMEASUREMENT
{
    FDOUBLE m_dInputPower;
    FDOUBLE m_dSpan;
    FDOUBLE m_dRBW;
    FDOUBLE m_dVBW;
    FDOUBLE m_dSweepTime;
    FDOUBLE m_dExpectedMinPhaseNoise;
    FDOUBLE m_dPNStartFrequency;
    FDOUBLE m_dPNStopFrequency;
    FDOUBLE m_dReferenceLevel;
    QVector<S_MODULENAME_PHASENOISEMEASUREMENT_FREQUENCYTABLE> m_vectFREQUENCYTABLE;
    U16BIT m_u16Delay;
    S8BIT m_s8StimulasPathID;
    S8BIT m_s8MeasuredPathID;
} S_MODULENAME_PHASENOISEMEASUREMENT, *PS_MODULENAME_PHASENOISEMEASUREMENT;

typedef struct _S_MODULENAME_GAINMEASUREMENTSA_GAIN_CFG_DETAILS
{
    U8BIT m_u8GainEqPathSel;
    FDOUBLE m_dRFDCA_2;
    FDOUBLE m_dIFFrequency;
    FDOUBLE m_dInputFreq;
    U8BIT m_u8BandSel;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dExpMinGain;
    FDOUBLE m_dExpMaxGain;
    FDOUBLE m_dNomGain;
    FDOUBLE m_dIFDCA_1;
    FDOUBLE m_dIFDCA_2;
} S_MODULENAME_GAINMEASUREMENTSA_GAIN_CFG_DETAILS, *PS_MODULENAME_GAINMEASUREMENTSA_GAIN_CFG_DETAILS;

typedef struct _S_MODULENAME_IMAGEREJECTION_CFGDETAILS
{
    FDOUBLE m_dFrequency;
    FDOUBLE m_dIFFrequency;
    FDOUBLE m_dImageFrequency;
    FDOUBLE m_dInputPower;
    U8BIT m_u8BandSel;
    FDOUBLE m_dExpMinRej;
    U8BIT m_u8GainEqPathSel;
    FDOUBLE m_dIFDCA_1;
    FDOUBLE m_dIFDCA_2;
    FDOUBLE m_dRFDCA_2;
} S_MODULENAME_IMAGEREJECTION_CFGDETAILS, *PS_MODULENAME_IMAGEREJECTION_CFGDETAILS;

typedef struct _S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT_CFGDETAILS
{
    U8BIT m_u8GainEqPathSel;
    FDOUBLE m_dRFDCA_2;
    FDOUBLE m_dCenterFreq;
    FDOUBLE m_dIFFrequency;
    FDOUBLE m_dIFDCA_1;
    U8BIT m_u8BandSel;
    FDOUBLE m_dFrequency;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dExpMinGain;
    FDOUBLE m_dExpMaxGain;
    U8BIT m_u8IsRefGain;
    FDOUBLE m_dIFDCA_2;
} S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT_CFGDETAILS, *PS_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT_CFGDETAILS;

typedef struct _S_MODULENAME_RXSPURIOUSMEASUREMENT_CFGDETAILS
{
    U8BIT m_u8BandSel;
    U8BIT m_u8InOutSel;
    FDOUBLE m_dFrequency;
    FDOUBLE m_dInputPower;
    U8BIT m_u8AttBandSel;
    FDOUBLE m_dExpSpurRej;
    FDOUBLE m_dBandCenFre;
    FDOUBLE m_dIFFreq;
} S_MODULENAME_RXSPURIOUSMEASUREMENT_CFGDETAILS, *PS_MODULENAME_RXSPURIOUSMEASUREMENT_CFGDETAILS;

typedef struct _S_MODULENAME_VSWRMEASUREMENT_CFGDETAIL
{
    U8BIT m_u8BandSel;
    FDOUBLE m_dBandFreq;
    U8BIT m_u8AttBandSel;
    FDOUBLE m_dExpMaxVswr;
    FDOUBLE m_dNoOfPoints;
    FDOUBLE m_dStartFreq;
    FDOUBLE m_dStopFreq;
} S_MODULENAME_VSWRMEASUREMENT_CFGDETAIL, *PS_MODULENAME_VSWRMEASUREMENT_CFGDETAIL;

typedef struct _S_MODULENAME_ATTENUATORVALIDATION_CFGDETAILS
{
    U8BIT m_u8GainEqPathSel;
    FDOUBLE m_dIFDCA_2;
    FDOUBLE m_dRFDCA_2;
    FDOUBLE m_dUserDCA;
    FDOUBLE m_dIFFrequency;
    FDOUBLE m_dBandCenterFrequency;
    FDOUBLE m_dInputPower;
    U8BIT m_u8BandSel;
    U8BIT m_u8AttenSel;
    U8BIT m_u8RefSel;
    FDOUBLE m_dInputFrequency;
    FDOUBLE m_dIFDCA_1;
    FDOUBLE m_dExpMinDCA;
    FDOUBLE m_dExpMaxDCA;
} S_MODULENAME_ATTENUATORVALIDATION_CFGDETAILS, *PS_MODULENAME_ATTENUATORVALIDATION_CFGDETAILS;

typedef struct _S_MODULENAME_SPURIOUSMEASUREMENT_CFGDETAILS
{
    U8BIT m_u8GainEqPathSel;
    FDOUBLE m_dRFDCA_2;
    U8BIT m_u8BandSel;
    FDOUBLE m_dFrequency;
    FDOUBLE m_dExpSpurRej;
    FDOUBLE m_dIFDCA_1;
    FDOUBLE m_dIFDCA_2;
    FDOUBLE m_dCenterFreq;
    FDOUBLE m_dIFFrequency;
} S_MODULENAME_SPURIOUSMEASUREMENT_CFGDETAILS, *PS_MODULENAME_SPURIOUSMEASUREMENT_CFGDETAILS;

typedef struct _S_MODULENAME_GAINMEASUREMENTSA
{
    U16BIT m_u16ChannelNumber;
    U16BIT m_u16StimulusPathId;
    U16BIT m_u16MeasurementPathID;
    U8BIT m_u8Precision;
    FDOUBLE m_dSpan;
    FDOUBLE m_dRbw;
    FDOUBLE m_dVbw;
    FDOUBLE m_dSweepTime;
    FDOUBLE m_dRefLevel;
    U8BIT m_u8MarkerPos;
    U32BIT m_u32delay;
    QVector<S_MODULENAME_GAINMEASUREMENTSA_GAIN_CFG_DETAILS> m_vectGAIN_CFG_DETAILS;
    U8BIT m_u8SigGenSel;
    U8BIT m_u8SpecSel;
} S_MODULENAME_GAINMEASUREMENTSA, *PS_MODULENAME_GAINMEASUREMENTSA;

typedef struct _S_MODULENAME_IMAGEREJECTION
{
    U8BIT m_u8ChannelNumber;
    U16BIT m_u16StimulusPathId;
    U16BIT m_u16MeasurementPathId;
    U8BIT m_u8Precision;
    FDOUBLE m_dSpan;
    FDOUBLE m_dRBW;
    FDOUBLE m_dVBW;
    FDOUBLE m_dSweepTime;
    FDOUBLE m_dRefLevel;
    U8BIT m_u8MarkerPos;
    U64BIT m_u64Delay;
    FDOUBLE m_dExpImageRej;
    QVector<S_MODULENAME_IMAGEREJECTION_CFGDETAILS> m_vectCFGDETAILS;
} S_MODULENAME_IMAGEREJECTION, *PS_MODULENAME_IMAGEREJECTION;

typedef struct _S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT
{
    U8BIT m_u8ChannelNumber;
    U16BIT m_u16StimulusPathId;
    U16BIT m_u16MeasurementPathId;
    U8BIT m_u8Precision;
    FDOUBLE m_dSpan;
    FDOUBLE m_dRBW;
    FDOUBLE m_dVBW;
    FDOUBLE m_dSweepTime;
    FDOUBLE m_dRefLevel;
    U8BIT m_u8MarkerPos;
    U64BIT m_u64Delay;
    FDOUBLE m_dExp1dBPoint;
    QVector<S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT_CFGDETAILS> m_vectCFGDETAILS;
} S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT, *PS_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT;

typedef struct _S_MODULENAME_VSWRMEASUREMENT
{
    U8BIT m_u8ChannelNumber;
    FDOUBLE m_dStartFrequency;
    FDOUBLE m_dStopFrequency;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dDataPoints;
    U16BIT m_u16Trace;
    FDOUBLE m_dRefLevel;
    FDOUBLE m_dExpMaxRetLoss;
    QVector<S_MODULENAME_VSWRMEASUREMENT_CFGDETAIL> m_vectCFGDETAIL;
} S_MODULENAME_VSWRMEASUREMENT, *PS_MODULENAME_VSWRMEASUREMENT;

typedef struct _S_MODULENAME_ATTENUATORVALIDATION
{
    U16BIT m_u16ChannelNumber;
    U16BIT m_u16StimulusPathId;
    U16BIT m_u16MeasurementPathId;
    U8BIT m_u8Precision;
    FDOUBLE m_dSpan;
    FDOUBLE m_dRBW;
    FDOUBLE m_dVBW;
    FDOUBLE m_dSweepTime;
    FDOUBLE m_dRefLevel;
    U8BIT m_u8MarkerPos;
    U32BIT m_u32Delay;
    QVector<S_MODULENAME_ATTENUATORVALIDATION_CFGDETAILS> m_vectCFGDETAILS;
} S_MODULENAME_ATTENUATORVALIDATION, *PS_MODULENAME_ATTENUATORVALIDATION;

typedef struct _S_MODULENAME_SPURIOUSMEASUREMENT_FREQUENCYTABLE
{
    FDOUBLE m_dInputFrequency;
} S_MODULENAME_SPURIOUSMEASUREMENT_FREQUENCYTABLE, *PS_MODULENAME_SPURIOUSMEASUREMENT_FREQUENCYTABLE;

typedef struct _S_MODULENAME_SPURIOUSMEASUREMENT
{
    FDOUBLE m_dInputPower;
    FDOUBLE m_dSpan;
    FDOUBLE m_dRBW;
    FDOUBLE m_dVBW;
    FDOUBLE m_dSweepTime;
    FDOUBLE m_dExpectedMinSpurious;
    FDOUBLE m_dReferenceLevel;
    QVector<S_MODULENAME_SPURIOUSMEASUREMENT_FREQUENCYTABLE> m_vectFREQUENCYTABLE;
    U16BIT m_u16Delay;
    S8BIT m_s8StimulasPathID;
    S8BIT m_s8MeasuredPathID;
    S8BIT m_s8InputChannel;
    S8BIT m_s8OutputChannel;
    FDOUBLE m_dCenterFrequency;
    FDOUBLE m_dFrequencyTolerance;
} S_MODULENAME_SPURIOUSMEASUREMENT, *PS_MODULENAME_SPURIOUSMEASUREMENT;

typedef struct _S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT_RFBITEISOLATIONDATA
{
    FDOUBLE m_dInputFreq;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dExpIso;
} S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT_RFBITEISOLATIONDATA, *PS_MODULENAME_RFTOBITEISOLATIONMEASUREMENT_RFBITEISOLATIONDATA;

typedef struct _S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT_RFBLANKISOLATIONDATA
{
    FDOUBLE m_dInputFreq;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dExpIso;
} S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT_RFBLANKISOLATIONDATA, *PS_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT_RFBLANKISOLATIONDATA;

typedef struct _S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT
{
    FDOUBLE m_dRBW;
    FDOUBLE m_dVBW;
    FDOUBLE m_dReferenceLevel;
    FDOUBLE m_dSweepTime;
    U16BIT m_u16Delay;
    QVector<S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT_RFBITEISOLATIONDATA> m_vectRFBITEISOLATIONDATA;
    FDOUBLE m_dSpan;
    U8BIT m_u8StimuluspathId;
    U8BIT m_u8MeasurepathID;
} S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT, *PS_MODULENAME_RFTOBITEISOLATIONMEASUREMENT;

typedef struct _S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT
{
    FDOUBLE m_dRBW;
    FDOUBLE m_dVBW;
    FDOUBLE m_dReferenceLevel;
    FDOUBLE m_dSweepTime;
    U16BIT m_u16Delay;
    QVector<S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT_RFBLANKISOLATIONDATA> m_vectRFBLANKISOLATIONDATA;
    FDOUBLE m_dSpan;
    U8BIT m_u8StimuluspathID;
    U8BIT m_u8MeasurepathID;
} S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT, *PS_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT;

typedef struct _S_MODULENAME_FILTERREJECTIONMEASUREMENT
{
    FDOUBLE m_dInputPower;
    U16BIT m_u16DataPoints;
    FDOUBLE m_dReferenceLevel;
    FDOUBLE m_dExpectdMinmumFilterRejection;
    FDOUBLE m_dHPFOBStartFrequency;
    FDOUBLE m_dHPFOBStopFrequency;
    FDOUBLE m_dHPFRejectionFrequency;
    FDOUBLE m_dLPFOBStartFrequency;
    FDOUBLE m_dLPFOBStopFrequency;
    FDOUBLE m_dLPFRejectionFrequency;
    FDOUBLE m_dNotchRejectionStartFrequency;
    FDOUBLE m_dNotchRejectionStopFrequency;
    FDOUBLE m_dPBStartFrequency;
    FDOUBLE m_dPBStopFrequency;
    FDOUBLE m_dNotchPBStartFrequency;
    FDOUBLE m_dNotchPBStopFrequency;
} S_MODULENAME_FILTERREJECTIONMEASUREMENT, *PS_MODULENAME_FILTERREJECTIONMEASUREMENT;

typedef struct _S_MODULENAME_TXPHASESHIFTERVALIDATION_PHASEDATA
{
        FDOUBLE m_dPhaseShift;
        FDOUBLE m_dExpectedMinimumPhaseShift;
        FDOUBLE m_dExpectedMaximumPhaseShift;
} S_MODULENAME_TXPHASESHIFTERVALIDATION_PHASEDATA, *PS_MODULENAME_TXPHASESHIFTERVALIDATION_PHASEDATA;

typedef struct _S_MODULENAME_TXATTENUATIONVALIDATION_ATTENUATIONDATA
{
        FDOUBLE m_dDCAAttenuation;
        FDOUBLE m_dExpectedMinimumAttenuation;
        FDOUBLE m_dExpectedMaximumAttenuation;
} S_MODULENAME_TXATTENUATIONVALIDATION_ATTENUATIONDATA, *PS_MODULENAME_TXATTENUATIONVALIDATION_ATTENUATIONDATA;

typedef struct _S_MODULENAME_TXPHASESHIFTERVALIDATION
{
        FDOUBLE m_dStartFrequency;
        FDOUBLE m_dStopFrequency;
        FDOUBLE m_dInputPower;
        U16BIT m_u16NoOfPoints;
        QVector<S_MODULENAME_TXPHASESHIFTERVALIDATION_PHASEDATA> m_vectPHASEDATA;
} S_MODULENAME_TXPHASESHIFTERVALIDATION, *PS_MODULENAME_TXPHASESHIFTERVALIDATION;

typedef struct _S_MODULENAME_TXPHASESHIFTERCALIBRATION
{
        FDOUBLE m_dStartFrequency;
        FDOUBLE m_dStopFrequency;
        FDOUBLE m_dStepFrequency;
        FDOUBLE m_dInputPower;
        U16BIT m_u16NoOfPoints;
        U8BIT m_u8TraceSelection;
} S_MODULENAME_TXPHASESHIFTERCALIBRATION, *PS_MODULENAME_TXPHASESHIFTERCALIBRATION;

typedef struct _S_MODULENAME_TXATTENUATIONVALIDATION
{
        FDOUBLE m_dStartFrequency;
        FDOUBLE m_dStopFrequency;
        FDOUBLE m_dInputPower;
        U16BIT m_u16NoOfPoints;
        QVector<S_MODULENAME_TXATTENUATIONVALIDATION_ATTENUATIONDATA> m_vectATTENUATIONDATA;
} S_MODULENAME_TXATTENUATIONVALIDATION, *PS_MODULENAME_TXATTENUATIONVALIDATION;
#endif /*#ifndef _DP_CONFIG_STRUCT_H_*/
