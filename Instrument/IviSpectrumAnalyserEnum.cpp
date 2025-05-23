/**************************************************************************
// File Name: IVISpectrumAnalyser.cpp
// Author:	  Venkatesh.P
// Created Date: 06-July-09
// Description: Spectrum Analyser Instrument IVI Enumeration ALL Function Definition are Defined here
**************************************************************************/

/**************************************************************************
// Company Name: Data Patterns (India) Pvt. Ltd.
// Address: No.6, Arya Gowder Road, West Mambalam, Chennai,India - 600033
// Email: datapatterns@vsnl.com
// Phone: 91-44-4837460 / 4848702
// FAX: 91-44-4848703
**************************************************************************/

#include "IviSpectrumAnalyserEnum.h"
#include "../src/dp_esm_1119_datastructure.h"
#include "visa.h"
#include "visatype.h"
#include "stdio.h"
#include "string.h"
#include "qstring.h"
#include "qthread.h"

ViSession defaultRM = 0, viESA = 0;
#define NOISE_FIGURE_MODE		219
#define SPECTRUM_ANA_MODE		1
#define PHASE_NOISE_MODE		14
#define CONTINOUS_MODE			1
#define SINGLE_MODE				0

extern SGlobalStucture gGlobalStuct;

/*************************************************************************************************

                    Spectrum Analyser - Enumeration


***************************************************************************************************/

//Static member variable initialization

/**************************************************************************
// Name						:	 CIVI_SPECTRUM()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:   06-July-09
// Revision Date			:
// Reason for Revising		:
// Description				:	To Register all the functionalities of Spectrum Analyzer module to Database
**************************************************************************/
CIVI_SPECTRUM::CIVI_SPECTRUM()
{

}

/**************************************************************************
// Name						:	 Open()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	To Open Spectrum Analyzer.
**************************************************************************/
short CIVI_SPECTRUM::Open(char *pLogicalName, unsigned char ucInstNo)
{
    ViSession rm    =  0;
    ViUInt16 Type   =  0;
    char ucTemp[20] = {0};
    char ucLoop     =  0;
    char value[50]  = {0};

    strcpy(value,pLogicalName);

    if(viESA)
    {
        viClose(viESA);
        viESA = 0;
    }

    for( ucLoop = 8; ucLoop <= 18; ucLoop++)
    {
        ucTemp[ucLoop-8] = value[ucLoop];
    }

    strcpy(gGlobalStuct.sCOTSInstInfo.m_cSAInfo,ucTemp);

    status = viOpenDefaultRM(&rm);

    status = viOpen(rm, pLogicalName, VI_NULL, VI_NULL, &viESA);
    if(status)
    {
        return status;
    }
    else
    {
        gGlobalStuct.sHandles.m_viRFSA[0] = viESA;
    }

    ViBoolean termDefault = VI_FALSE;
    if((VI_SUCCESS == viGetAttribute(viESA, VI_ATTR_RSRC_CLASS,value)) && (0 == strcmp("SOCKET", (ViString)value)))
        termDefault = VI_TRUE;

    else if((VI_SUCCESS == viGetAttribute(viESA, VI_ATTR_INTF_TYPE, &Type)) && (Type == VI_INTF_ASRL))
        termDefault = VI_TRUE;

    m_bOpenStatus = true;

    viSetAttribute(viESA, VI_ATTR_TERMCHAR_EN, termDefault);

    viPrintf(viESA, (char*)"*RST\n");

    return status;

}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigFreqStartStop()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Freq Start Stop of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigFreqStartStop(session in_uiVi, double in_dStartFreq, double in_dStopFreq)
{
    status = viPrintf(in_uiVi, (char*)":FREQ:STAR %lf\n", in_dStartFreq);
    if (status)
    {
        return status;
    }
    status = viPrintf(in_uiVi, (char*)":FREQ:STOP %lf\n", in_dStopFreq);
    if (status)
    {
        return status;
    }

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigFreqCenterSpan()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Freq CenterSpan of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigFreqCenterSpan(session in_uiVi, double in_dCenterFreq, double in_dSpan)
{
    status = viPrintf(in_uiVi, (char*)":FREQ:CENT %lf\n", in_dCenterFreq);
    if (status)
    {
        return status;
    }

    status = viPrintf(in_uiVi, (char*)":FREQ:SPAN  %lf\n", in_dSpan);
    if (status)
    {
        return status;
    }

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigFreqOffset()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Freq Offset of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigFreqOffset(session in_uiVi, double in_dFreqOffset)
{
    status = viPrintf(in_uiVi, (char*)"FREQ:OFFS %lf\n", in_dFreqOffset);
    if (status)
    {
        return status;
    }

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigSweepCoupling()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Sweep Coupling of the Spectrum Analyser
******************************************************************************************************/
short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigSweepCoupling(session in_uiVi, bool in_bResolutionBWAuto, double in_dResolutionBW, bool in_bVideoBWAuto, double in_dVideoBW, bool in_bSweepTimeAuto, double in_dSweepTime,  unsigned int in_uiNoOfSweepPts)
{
    status = viPrintf(in_uiVi, (char*)"BWID:AUTO %d\n",in_bResolutionBWAuto);
    if (status)
    {
        return status;
    }

    status = viPrintf(in_uiVi, (char*)"BAND %lf\n",in_dResolutionBW);
    if (status)
    {
        return status;
    }

    status = viPrintf(in_uiVi, (char*)"BWID:VID:AUTO %d\n",in_bVideoBWAuto);
    if (status)
    {
        return status;
    }

    status = viPrintf(in_uiVi, (char*)"BAND:VID %lf\n",in_dVideoBW);
    if (status)
    {
        return status;
    }

    status = viPrintf(in_uiVi, (char*)"SWE:TIME:AUTO %d\n", in_bSweepTimeAuto);
    if (status)
    {
        return status;
    }

    status = viPrintf(in_uiVi, (char*)"SWE:TIME %lf\n", in_dSweepTime);
    if (status)
    {
        return status;
    }
	
	if(in_uiNoOfSweepPts != 1001)
	{
		status = viPrintf(in_uiVi, ":SWEEP:POINts %d\n", in_uiNoOfSweepPts);
		if(status)
		{
			return status;
		}
	}
	
    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ReadConfigSweep()
// Author					:	 Kaushik Piparotar
// Global Variables affected:	 NA
// Created Date				:    16-10-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	 Read Configure Sweep Time of the Spectrum Analyser
******************************************************************************************************/
short CIVI_SPECTRUM::DP_Enum_SpecAn_ReadConfigSweep(session in_uiVi, double *out_dSweepTime)
{
    short status	   = 0;
    status = viQueryf(viESA, (char*)":SENS:SWE:TIME?\n", "%lf", out_dSweepTime);
    if(status)
    {
        return status;
    }

    return status;
}


/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigLevel()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Reference Level of the Spectrum Analyser
******************************************************************************************************/

short CIVI_SPECTRUM:: DP_Enum_SpecAn_ConfigLevel (session in_uiVi,double in_dRefLevel)
{
    status = viPrintf(in_uiVi, (char*)"DISP:WIND:TRAC:Y:RLEV %f\n", in_dRefLevel);
    if (status)
    {
        return status;
    }

    status = DP_Enum_SpecAn_WaitForOperation_Complete(in_uiVi);

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigTriggerSource()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Configure Trigger Source of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigTriggerSource(session in_uiVi, signed long in_slTriggerSource)
{
    char szTrigMode[4][15] = {"EXTernal1", "IMMediate", "LINE", "VIDeo"};
    status = viPrintf(in_uiVi, (char*)"TRIG:SOUR %s\n",szTrigMode[in_slTriggerSource]);
    if (status)
    {
        return status;
    }

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigExtnlTrigger()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:  Configure External Trigger of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigExtnlTrigger(session in_uiVi, double in_dExtnlTrigrLevel, signed long in_slExtnlTrigrPolarity)
{
    status = viPrintf(in_uiVi, (char*)"TRIG:EXT1:LEV lf\n",in_dExtnlTrigrLevel);
    if (status)
    {
        return status;
    }

    char szExSlopeMode[2][15] = {"NEG", "POS"};
    status = viPrintf(in_uiVi, (char*)"TRIG:EXT1:SLOP %s\n",szExSlopeMode[in_slExtnlTrigrPolarity]);
    if (status)
    {
        return status;
    }

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigVideoTrigger()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Configure Video Trigger of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM:: DP_Enum_SpecAn_ConfigVideoTrigger (session in_uiVi, double in_dVideoTrigrLevel, signed long in_slVideoTrigrSlope, float in_fTrigDelay)
{
    // Triger level to be add
    char szSlopeMode[5][15] = {"NEG", "POS"};
    status = viPrintf(in_uiVi, (char*)"TRIG:VID:SLOP %s\n",szSlopeMode[in_slVideoTrigrSlope]);
    if (status)
    {
        return status;
    }
    status = viPrintf(in_uiVi, (char*)"TRIG:VID:DEL:STAT ON\n");
    if (status)
    {
        return status;
    }
    status = viPrintf(in_uiVi, (char*)"TRIG:VID:DEL %f\n", in_fTrigDelay);
    if (status)
    {
        return status;
    }

    return status;

}


/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigExtnlMixer()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configure Configure External Mixer of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigExtnlMixer (session ulVi, signed long in_slHarmonic, double in_dAvgConverLoss)
{
    status = viPrintf(ulVi, (char*)"MIX:HARM %lf\n", in_slHarmonic);
    if (status)
    {
        return status;
    }

    return status;

    // in_dAvgConverLoss to be add
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigExtnlMixerBiasEnabled()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configure Configure External Mixer Bias Enable of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigExtnlMixerBiasEnabled (session in_uiVi, bool in_bBiasEnabled)
{
    status = viPrintf(in_uiVi, (char*)"MIX:BIAS:STAT %d\n", in_bBiasEnabled);
    if (status)
    {
        return status;
    }

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ReadActiveMarker
// Author					:	 Priyanka T
// Global Variables affected:	 NA
// Created Date				:    09-01-10
// Revision Date			:
// Reason for Revising		: Added Single / Continuous Mode Selection (in_ucContSingleMode) on 09-05-14
// Description				:	 This function configures the type of trace to acquire
******************************************************************************************************/

short CIVI_SPECTRUM::DP_Enum_SpecAn_ReadActiveMarker(session in_uiVi, unsigned char in_ucContSingleMode, unsigned short in_TraceIndex, double in_dMarkerPos,  double *out_dMarkerAmp)
{
    char cpReadValue[64] = {0};
    int iTempRetCount = 0;
    double dMarAmp = 0.0f;
    double dMarkerPos = 0;
    if(!in_dMarkerPos)
    {
        status = viPrintf(in_uiVi, (char*)"CALC:MARK1:STAT ON\n");
        if(status)
            return status;
        status = viPrintf(in_uiVi, (char*)"CALC:MARK1:MAX\n");
        if(status)
            return status;
        viQueryf(in_uiVi,(char*)"CALC:MARK:Y? \n","%s",cpReadValue);
        dMarAmp= strtod(cpReadValue,NULL);
    }
    else
    {
        status = viPrintf(in_uiVi, (char*)"CALC:MARK1:STAT ON\n");
        if(status)
            return status;

        status = viPrintf(in_uiVi, (char*)"CALC:MARK1:MODE POS\n");
        if(status)
            return status;

        status = viPrintf(in_uiVi, (char*)"CALC:MARK1:X %lf\n", in_dMarkerPos);
        if(status)
            return status;
        viQueryf(in_uiVi,(char*)"CALC:MARK:Y? \n","%s",cpReadValue);
        dMarAmp= strtod(cpReadValue,NULL);
    }

    *out_dMarkerAmp = dMarAmp;
    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_WaitForOperation_Complete
// Author					:	 Priyanka T
// Global Variables affected:	 NA
// Created Date				:    09-01-10
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function Gives Single Sweep
******************************************************************************************************/

short CIVI_SPECTRUM:: DP_Enum_SpecAn_WaitForOperation_Complete(session in_uiVi)
{
    int iOpc = 0;
    if(viESA != 0)
    {
        while(!iOpc)
        {
            status = viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOpc);
            if(status)
            {
                break;
            }
        }
    }
    return 0;

}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ModeConfig
// Author					:	 Priyanka T
// Global Variables affected:	 NA
// Created Date				:    09-01-10
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function configures the type of trace to acquire
                                //Mode No : 1 --> SA Mode , 219 --> Noise Figure Mode
******************************************************************************************************/
short CIVI_SPECTRUM:: DP_Enum_SpecAn_ModeConfig(session in_uiVi, unsigned short in_usModeNo)
{
    char StrFormat[64]= "";
    iInstNum = -1;
    int iOper  = 0;
    unsigned char ucOPCCnt = 0;
    sprintf(StrFormat,(char*)":INSTrument:NSELect %d\n",in_usModeNo);
    status = viPrintf(viESA,StrFormat);
    if(status)
        return status;

    status = DP_Enum_SpecAn_WaitForOperation_Complete(viESA);
    if(status)
        return status;

    status = viQueryf(viESA, "*OPC?\n", "%d", &iOper);
    if(status)
        return status;

    status = viQueryf(viESA,(char*)"INST:NSEL?\n","%d",&iInstNum);
    if(status)
        return status;

    if(iInstNum == PHASE_NOISE_MODE)
    {
        status = viPrintf(viESA,(char*)":CONF:LPLot\n");

        if(status)
            return status;

        if(viESA != 0)
        {
            while(!iOper)
            {
                viQueryf(viESA,(char*)"*OPC?\n", "%d", &iOper);
            }
        }
        status = viPrintf(viESA,(char*)":TRAC:LPLot:TYPE RAW\n");
        if(status)
            return status;
    }

    if(iInstNum == NOISE_FIGURE_MODE)
    {
        //Sleep(1000);

        /*To display the Noise source Layout in meter format*/
        viPrintf(viESA,(char*)":DISP:NFIG:FORM TABLe\n");

        while(!iOper)
        {
            viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

    }

    return 0;
}

/**************************************************************************************
// Name						:	 DP_Enum_SpecAn_ReadNF
// Author					:	 Priyanka T
// Global Variables affected:	 NA
// Created Date				:    09-01-10
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function used to read the measured noise figure
****************************************************************************************/
short CIVI_SPECTRUM:: DP_Enum_SpecAn_ReadNF(session in_uiVi,double *out_dNoiseVal,  double *out_dGainValue)
{
    QString csFormat = "";

    QString strInformation = "",strData[5];
    int iSubItem= 0;
    double dMarkerReadValue = 0;
    double dGainValue= 0.0;
    double dNoiseFigure = 0.0;
    int iOpc = 0;
    char cpReadValue[10000] = {0};
    static ViChar *cTok;
    unsigned short usIndex   = 0;
    unsigned int viTimeDelay = 0,sts = 0;

    unsigned int u32RetCnt = 0;


    /*To apply the calibrated value*/
    viPrintf(viESA,(char*)"SENSE:NFIG:CAL:STATE ON\n");

    /*To set single measurement*/
    viPrintf(viESA,(char*)":INIT:CONT ON\n");

    //	Sleep(500);
    //   QThread::msleep(500);
    /*To set single measurement*/
    viPrintf(viESA,(char*)":INIT:CONT OFF\n");

    /*Sometimes values are not reading so retried*/

    /*To apply the calibrated value*/
    viPrintf(viESA,(char*)":NFIG:CAL:STATE ON\n");

    /*To set single measurement*/
    viPrintf(viESA,(char*)":INIT:CONT ON\n");

    while(!iOpc)
    {
        status = viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOpc);
        if(status)
        {
            break;
        }
    }

//       QThread::msleep(25000);
    /*To set single measurement*/
 //   viPrintf(viESA,(char*)":INIT:CONT OFF\n");

/*
    status = viPrintf(in_uiVi, ":READ:NFIG:ARR:DATA:CORR:GAIN?\n");
    if(status)
    {

        return status;
    }

    status = viRead(in_uiVi, (ViPBuf)cpReadValue, 10000, &u32RetCnt);
    if(status)
    {

        return status;
    }

    usIndex = 0;
   cTok = strtok(cpReadValue,",");
   while(cTok != NULL)
   {
       out_dGainValue[usIndex] = atof(cTok);
       cTok = strtok(NULL,",");
       usIndex++;
   }

    status = viPrintf(in_uiVi, "READ:NFIG:ARR:DATA:CORR:NFIG?\n");
    if(status)
    {

        return status;
    }


    status = viRead(in_uiVi, (ViPBuf)cpReadValue, 10000, &u32RetCnt);
    if(status)
    {

        return status;
    }

    usIndex = 0;
    cTok = strtok(cpReadValue,",");
    while(cTok != NULL)
    {
        out_dNoiseVal[usIndex] = atof(cTok);
        cTok = strtok(NULL,",");
        usIndex++;
    }
*/
    /*To read the Gain value*/
#if 0
    viQueryf(viESA, "READ:NFIG:ARR:DATA:CORR:GAIN?\n", "%s", cpReadValue);

    cTok = strtok(cpReadValue,",");
    while(cTok != NULL)
    {
        out_dGainValue[usIndex] = atof(cTok);
        cTok = strtok(NULL,",");
        usIndex++;
    }
#endif

    /*Set Measurement timeout as 25 Seconds*/
    viSetAttribute(viESA, VI_ATTR_TMO_VALUE, 25000);

    /*To read the Noise figure value*/
    viQueryf(viESA, (char*)"READ:NFIG:ARR:DATA:CORR:NFIG?\n", "%s", cpReadValue);

    /*Restore Measurement timeout as 2 Seconds*/
    viSetAttribute(viESA, VI_ATTR_TMO_VALUE, 2000);

    usIndex = 0;
    cTok = strtok(cpReadValue,",");
    while(cTok != NULL)
    {
        out_dNoiseVal[usIndex] = atof(cTok);
        cTok = strtok(NULL,",");
        usIndex++;
    }

    /*To set single measurement*/
    viPrintf(viESA,(char*)":INIT:CONT OFF\n");

    return 0;
}

short CIVI_SPECTRUM ::  DP_Enum_SpecAn_ConfigureHarmonicDistortionMeasurement(session in_uiVi, unsigned char in_ucHarCnt, double dRefLevel, double dFundFreq, double in_dRBW, unsigned char in_ucAtten)
{

    ViChar message[128] = "";
    int iOpc = 0;
    QString strTemp = "";

    status = viPrintf(viESA,(char*)":SENS:HARM:NUMB %d\n", in_ucHarCnt);
    if(status)
        return status;

    status = viPrintf(viESA,(char*)":CONF:HARM\n");
    if(status)
        return status;

    if(viESA != 0)
    {
        while(!iOpc)
        {
            status = viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOpc);
            if(status)
            {
                break;
            }
        }
    }

    status = viPrintf(viESA,(char*)":SENS:HARM:AVER:COUN %d\n", in_ucHarCnt);
    if(status)
        return status;

    status = viPrintf(viESA,(char*)":SENS:HARM:SWE:STAT ON\n");
    if(status)
        return status;

    status = viPrintf(viESA,(char*)"DISP:HARM:VIEW:WIND:TRAC:Y:RLEV %f\n", dRefLevel);
    if(status)
        return status;

    status = viPrintf(viESA,(char*)"HARM:BWID %f\n", in_dRBW);
    if(status)
        return status;

    status = viPrintf(viESA,(char*)"HARM:FREQ:FUND %f\n", dFundFreq);
    if(status)
        return status;

    status = viPrintf(viESA,(char*)"INITiate:IMMediate\n");
    if(status)
        return status;

    return status;
}


/***************************************************************************************************
// Name						:	 AgSA_ReadHarmonic
// Author					:	 Velrajan M
// Global Variables affected:	 NA
// Created Date				:    11-12-09
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function stops the present measurement and set up the instrument for
the harmonic measurement
******************************************************************************************************/
short CIVI_SPECTRUM ::  DP_Enum_SpecAn_ReadHarmonic(session in_uiVi,
                                                    unsigned char in_ucmaximumTime_ms,
                                                    signed long harmonicDistArraySize,
                                                    double *in_dharmonicDistTotalQ,
                                                    double *out_Ampl1,
                                                    double *out_Ampl2,
                                                    signed long *in_plharmonicDistValuesRead)
{

    signed long *out_slActPts = &harmonicDistArraySize;
    double harmonicDistAmplitude[10] ={0.0};
    double harmonicDistFrequency[10] = {0.0};
    char StrValue[1024] = {0};
    static ViChar *cTok;
    unsigned short usIndex = 0;
    unsigned long ulOut = 0;
    int iOpc = 0;

    viSetAttribute(viESA,VI_ATTR_TMO_VALUE,6000);

    viQueryf(viESA,":READ:HARM:AMPL:ALL?\n","%s",StrValue);

    viSetAttribute(viESA,VI_ATTR_TMO_VALUE,3000);

    //	Sleep(500);

    cTok = strtok(StrValue,",");
    while(cTok != NULL)
    {
        if(usIndex == 0)
        {
            harmonicDistFrequency[usIndex] = atof(cTok);
        }
        else
        {
            harmonicDistFrequency[usIndex] = atof(cTok);
            harmonicDistFrequency[usIndex] = harmonicDistFrequency[usIndex] - harmonicDistFrequency[0];
        }
        cTok = strtok(NULL,",");
        usIndex++;
    }
    *out_Ampl1 = harmonicDistFrequency[1];
    *out_Ampl2 = harmonicDistFrequency[2];
    return 0;

}

short CIVI_SPECTRUM ::  DP_Enum_SpecAn_ReadHarmonicValue(session in_uiVi,
                                                         double* pdHarFreq,double* pdHarAmp)
{
    double harmonicDistAmplitude[64] ={0.0};
    double harmonicDistFrequency[64] = {0.0};
    char StrValue[1024] = {0},strFreq[1024] = {0};
    static ViChar *cTok,*cAmpTok = NULL;
    unsigned short usIndex = 0;

    viSetAttribute(viESA,VI_ATTR_TMO_VALUE,6000);

    status = viQueryf(viESA,(char*)":READ:HARM:AMPL:ALL?\n","%s",StrValue);
    if(status)
        return status;

    status = viQueryf(viESA,(char*)":READ:HARM:FREQ:ALL?\n","%s",strFreq);
    if(status)
        return status;

    viSetAttribute(viESA,VI_ATTR_TMO_VALUE,3000);

  //  msleep(100);

    cTok = strtok(StrValue,",");
    while(cTok != NULL)
    {
        pdHarAmp[usIndex] = atof(cTok);
        cTok = strtok(NULL,",");
        usIndex++;
    }

    usIndex=0;
    cAmpTok = strtok(strFreq,",");
    while(cAmpTok != NULL)
    {
        pdHarFreq[usIndex] = atof(cAmpTok);
        cAmpTok = strtok(NULL,",");
        usIndex++;
    }

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigureSpurious()
// Author					:	 Velrajan M
// Global Variables affected:	 NA
// Created Date				:    11-12-09
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function sets up the harmonic distortion measurement
******************************************************************************************************/
short CIVI_SPECTRUM ::  DP_Enum_SpecAn_ConfigureSpurious(session in_uiVi, unsigned char in_ucRegNo)
{
    //  unsigned char in_ucRegNo;
    int iOpc= 0;
    status = viPrintf(viESA,(char*)"*RCL %d\n",in_ucRegNo);
    if(status)
        return status;

    if(viESA != 0)
    {
        status = viPrintf(viESA,(char*)":CONF:SPUR\n");
        if(status)
            return status;
        while(!iOpc)
        {
            status = viQueryf(viESA,(char*)"*OPC?\n", "%d", &iOpc);
            if(status)
            {
                break;
            }
        }
    }

    return 0;
}

/***************************************************************************************************
// Name						:	 ReadSpuriousMeas()
// Author					:	 Velrajan M
// Global Variables affected:	 NA
// Created Date				:    11-12-09
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function stops the present measurement and set up the instrument for
the harmonic measurement
******************************************************************************************************/
short CIVI_SPECTRUM ::  DP_Enum_SpecAn_ReadSpuriousMeas(session in_uiVi,unsigned short in_usTotSamp,
                                                        double* pFreq,double* pAmp, unsigned short* usCount)
{

    double SuprDistAmplitude[64] ={0.0};
    double SuprDistFrequency[64] = {0.0},dSwapped = 0;
    double dReadvalue[1000]= {0.0};
    char StrValue[2000] = {0};
    static ViChar *cTok = NULL;
    unsigned short usIndex = 0, usHiReadIdx = 0;
    unsigned long ulOut = 0;
    int iOpc = 0;
    int iTotCnt = 0;
    unsigned long ulCount  =0;
    int iTempLoop = 0;
    unsigned char ucSamCnt = 0;

    viSetAttribute(viESA,VI_ATTR_TMO_VALUE,6000);
    viQueryf(viESA,(char*)":READ:SPUR?\n","%s",StrValue);
    cTok = strtok(StrValue,",");
    while(cTok != NULL)
    {
        dReadvalue[usIndex] = atof(cTok);
        cTok = strtok(NULL,",");
        usIndex++;
    }
    iTotCnt = dReadvalue[0];

    int iLoop = 0;
    int iFreqVal = 3, iAmpValue = 4;

    for(iTempLoop = 1; iTempLoop < usIndex; iTempLoop++)
    {
        if(iTempLoop == iFreqVal )
        {
            SuprDistFrequency[iLoop] = dReadvalue[iTempLoop];
            iFreqVal = iFreqVal + 6;
        }
        else if(iTempLoop ==  iAmpValue)
        {
            SuprDistAmplitude[iLoop] = dReadvalue[iTempLoop];
            iAmpValue = iAmpValue + 6;
            iLoop++;
        }
    }
    if(iTotCnt > 2)
    {
        for(unsigned char ucOuterLoop = 0; ucOuterLoop < iTotCnt; ucOuterLoop ++)
        {
            for(unsigned char ucInnerLoop = ucOuterLoop + 1; ucInnerLoop < iTotCnt; ucInnerLoop ++)
            {
                if(SuprDistAmplitude[ucOuterLoop] < SuprDistAmplitude[ucInnerLoop])
                {
                    dSwapped						= SuprDistAmplitude[ucOuterLoop];
                    SuprDistAmplitude[ucOuterLoop]	= SuprDistAmplitude[ucInnerLoop];
                    SuprDistAmplitude[ucInnerLoop]	= dSwapped;

                    dSwapped						= SuprDistFrequency[ucOuterLoop];
                    SuprDistFrequency[ucOuterLoop]	= SuprDistFrequency[ucInnerLoop];
                    SuprDistFrequency[ucInnerLoop]	= dSwapped;
                }
            }
        }

        for( ucSamCnt = 1;ucSamCnt < 3;ucSamCnt++)
        {
            pFreq[ucSamCnt-1]		= SuprDistFrequency[ucSamCnt];
            pAmp[ucSamCnt-1]		= SuprDistAmplitude[0] - SuprDistAmplitude[ucSamCnt];
        }

        *usCount = ucSamCnt-1;
    }
    else
    {
        for(iTempLoop = 0; iTempLoop <in_usTotSamp; iTempLoop++)
        {
            pFreq[iTempLoop]	= 0;
            pAmp[iTempLoop]		= 0;
        }
        *usCount = 0;
    }

    return 0;
}


short CIVI_SPECTRUM::DP_Enum_SpecAn_Save_Image(session in_uiVi, unsigned short in_SpotNo)
{
    char StrFormat[64]= {0};

    sprintf(StrFormat,":MMEM:STORE:SCR ’C:PS_Spot%d.GIF'",in_SpotNo+1);
    viPrintf(viESA,StrFormat);
    return 0;
}


short CIVI_SPECTRUM::DP_Enum_SpecAn_Reset(session in_uiVi)
{
    status = viPrintf(viESA,(char*)"*RST\n");

    if(status)
        return status;

    return status;
}


/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_Initiate()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:  Configure Initiate of the Spectrum Analyser
******************************************************************************************************/


short CIVI_SPECTRUM::DP_Enum_SpecAn_Initiate (session in_uiVi)
{

    status = viPrintf(viESA,(char*)"INITiate:IMMediate\n");
    if(status)
        return status;

    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_Abort()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configure  Abort of the Spectrum Analyser
******************************************************************************************************/

short CIVI_SPECTRUM::DP_Enum_SpecAn_Abort (session in_uiVi)
{
    unsigned char in_ucRegNo = 0;
    status = viPrintf(viESA,(char*)"ABOR\n",in_ucRegNo);
    if(status)
        return status;

    return status;
}


/***************************************************************************************************
// Name						:	 Calibration_With_NoiseSoure()
// Author					:	 Shanmugam .S
// Global Variables affected:	 NA
// Created Date				:    22-jan-2012
// Revision Date			:
// Reason for Revising		:
// Description				:	 Calibrates the Noise soure for noise figure measurement
******************************************************************************************************/
short CIVI_SPECTRUM::Calibration_With_NoiseSoure (session in_uiVi)
{
    unsigned short usOPCCnt = 0;

    int iOper = 0;

    /*Calibration*/
    status = viPrintf(viESA,(char*)":NFIG:CAL:INIT\n");
    if(status)
        return status;

    QThread::msleep(200);

    if(viESA != 0)
    {
        while(!iOper)
        {
            viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

            if(usOPCCnt > 300)
                break;

            usOPCCnt++;
             QThread::msleep(100);
        }
    }

    return 0;
}

/***************************************************************************************************
// Name						:	 Read_PhaseNoise
// Author					:	 Priyanka T
// Global Variables affected:	 NA
// Created Date				:    09-01-10
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function configures the type of trace to acquire
******************************************************************************************************/

short CIVI_SPECTRUM:: Read_PhaseNoise(session in_uiVi, unsigned short in_TraceIndex, double in_dMarkerPos,  double *out_dMarkerAmp)
{
    char csFormat[50]={0};
    double dMarkerPos = 0;
    int iOpc =0;
    int iTempRetCount = 0;
    float fOutPhaseNoise = 0.0f;

    status = viPrintf(viESA,(char*)"DISP:LPL:VIEW:WIND:TRAC:Y:RLEV -20\n");
    if(status)
        return status;

    /*To perform auto tune*/
    status = viPrintf(viESA,(char*)":FREQ:CARRier:SEARch\n");
    if(status)
        return status;

    iTempRetCount = 0;

    while(!iOpc)
    {
        status = viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOpc);

        if(status)
            return status;

        if(iTempRetCount >= 100)
            break;

        iTempRetCount++;
       usleep(1000);
    }

    iTempRetCount = 0;

    usleep(4500000);

    /* Sets the marker mode as normal*/
    status = viPrintf(viESA,(char*)":CALC:LPLot:MARKer1:MODE POS\n");

    if(status)
        return status;

    /*to set the marker frequency*/
    sprintf(csFormat, ":CALC:LPLot:MARKer1:x %lf\n",in_dMarkerPos);

    status = viPrintf(viESA,csFormat);

    if(status)
        return status;

    /*To read the phase noise value*/
    status = viQueryf(viESA, (char*)":CALC:LPLOT:MARKER1:Y?\n", "%f", &fOutPhaseNoise);

    if(status)
    {
        return status;
    }

    *out_dMarkerAmp = fOutPhaseNoise;
    return 0;
}


/***************************************************************************************************
// Name						:	 ConfigAutoSweep()
// Author					:	 Shanmugam .S
// Global Variables affected:	 NA
// Created Date				:    22-jan-2012
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures sweep mode as auto
******************************************************************************************************/
short CIVI_SPECTRUM::ConfigAutoSweep (session in_uiVi)
{
    unsigned char ucOPCCnt = 0;

    int iOper = 0;

    /*Calibration*/
    status = viPrintf(viESA,(char*)":SWE:TIME:AUTO ON\n");

    if(status)
        return status;

    if(viESA != 0)
    {
        while(!iOper)
        {
            viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    return 0;
}


/***************************************************************************************************
// Name						:	 ConfigMaxMarker()
// Author					:	 Shanmugam .S
// Global Variables affected:	 NA
// Created Date				:    22-jan-2012
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures sweep mode as auto
******************************************************************************************************/
short CIVI_SPECTRUM::ConfigMaxMarker (session in_uiVi, double* out_dAmplitude)
{
    unsigned char ucOPCCnt = 0;

    int iOper = 0;

    double dAmplitude = 0.0f;

    status = viPrintf(viESA,(char*)":CALC:MARK1:MAX\n");

    if(status)
        return status;

    if(viESA != 0)
    {
        while(!iOper)
        {
            viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    status = viQueryf(viESA,(char*)":CALC:MARK1:Y?\n","%f",&dAmplitude);

    if(status)
        return status;

    if(viESA != 0)
    {
        while(!iOper)
        {
            viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    *out_dAmplitude = dAmplitude;

    return 0;
}
short CIVI_SPECTRUM::AgSA_Close(session ulVi)
{
    ViSession rm = 0;
    viClose(viESA);
    viClose(rm);
    return status;
}

/***************************************************************************************************
// Name						:	 AgSA_ConfigDUTSetup
// Author					:	 Shanmugam .S
// Global Variables affected:	 NA
// Created Date				:    10-june-2013
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures Noise Figure Mode DUT Setup
******************************************************************************************************/
short CIVI_SPECTRUM::AgSA_ConfigDUTSetup(session ulVi, float in_fLOFreq, float in_fIFFreq, unsigned char ucDUTModeSel)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[50] ={0};

    /* to set LO Fixed frequency*/
    if(ucDUTModeSel == 2)
    {
        sprintf(szCmdStr,":NFIG:MODE:DOWN:LOSC:FREQ %f\n", in_fLOFreq);
    }
    else
    {
        sprintf(szCmdStr,":NFIG:MODE:Upconv:LOSC:FREQ %f\n",in_fLOFreq);
    }

    status = viPrintf(viESA, szCmdStr);

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    //	Sleep(100);

    return 0;
}


/***************************************************************************************************
// Name						:	 AgSA_DUTSetupConfig
// Author					:	 Kaushik Piparotar
// Global Variables affected:	 NA
// Created Date				:    10-june-2013
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures Noise Figure Mode DUT Setup
******************************************************************************************************/
short CIVI_SPECTRUM::AgSA_DUTSetupConfig(session ulVi, float in_fLOFreq, float in_fIFFreq,unsigned char ucDUTModeSel)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[50] = {0};

    /* to set LO Fixed frequency*/
    if(ucDUTModeSel == 2)
    {
        //	sprintf(szCmdStr,":NFIG:MODE:DOWN:LOSC:FREQ %f\n", in_fLOFreq);
        sprintf(szCmdStr,"SENS:CONF:MODE:DOWN:LOSC:FREQ %f\n", in_fLOFreq);
    }
    else
    {
        //	sprintf(szCmdStr,":NFIG:MODE:Upconv:LOSC:FREQ %f\n",in_fLOFreq);
        sprintf(szCmdStr,"SENS:CONF:MODE:UPC:LOSC:FREQ %f\n", in_fLOFreq);
    }

    status   = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    //	Sleep(100);

    return 0;
}

short CIVI_SPECTRUM::AgSA_ConfigDUTSetup(session ulVi, float in_fLOFreq, float in_fIFFreq)
{
    short status	  = 0;
    unsigned char ucOPCCnt	  = 0;
    int iOper		  = 0;
    char szCmdStr[50] = {0};

    /* to set LO Fixed frequency*/
    sprintf(szCmdStr,":NFIG:MODE:DOWN:LOSC:FREQ %f\n", (in_fLOFreq - in_fIFFreq));
    status = viPrintf(viESA, szCmdStr);

    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

#if 0
    /* to set IF Fixed frequency*/
    sprintf(szCmdStr,":NFIG:MODE:DOWN:IF:FREQ %f\n", in_fIFFreq);

    status = viPrintf(viESA, szCmdStr);

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, "*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }
#endif

    //	Sleep(100);

    return 0;
}

/***************************************************************************************************
// Name						:	 AgSA_ConfigMaxHold
// Author					:	 Shanmugam .S
// Global Variables affected:	 NA
// Created Date				:    25-june-2013
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures Noise Figure Mode DUT Setup
******************************************************************************************************/
short CIVI_SPECTRUM:: AgSA_ConfigMaxHold(session ulVi)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[50] ={0};

    /* to set the Trace  in		*/
    status = viPrintf(viESA, (char*)"TRAC1:TYPE MAXH\n");

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    return status;
}



/***************************************************************************************************
// Name						:	 AgSA_ReadPeakValue
// Author					:	 Shanmugam .S
// Global Variables affected:	 NA
// Created Date				:    25-june-2013
// Revision Date			:
// Reason for Revising		:
// Description				:	 reads the peak value
******************************************************************************************************/
short CIVI_SPECTRUM:: AgSA_ReadPeakValue(session ulVi, float* out_pfPeakValue)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    char szCmdStr[50] ={0};
    long double ldFreq = 0;


    status = viPrintf(viESA, (char*)":INIT:CONT OFF\n");

    //	Sleep(1000);


    /* to set the Marker in Peak*/
    status = viPrintf(viESA, (char*)"CALC:MARK1:MAX\n");

    while(!iOper)
    {
        viQueryf(viESA,(char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    iOper		= 0;
    ucOPCCnt	= 0;
    *out_pfPeakValue = 0.0;
    /*Read the marker value*/
    status = viQueryf(viESA, (char*)"CALC:MARK1:Y?\n", "%f", out_pfPeakValue);

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }


    iOper		= 0;
    ucOPCCnt	= 0;


    status = viQueryf(viESA, (char*)"CALC:MARK1:X?\n", "%ld", &ldFreq);

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    status = viPrintf(viESA, (char*)":INIT:CONT ON\n");
    return status;
}

short CIVI_SPECTRUM:: AgSA_ReadPeakValue(session ulVi, double *out_pdFreq, float* out_pfPeakValue)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    char szCmdStr[50] ={0};
    long double ldFreq = 0;


    /* to set the Marker in Peak*/
    status = viPrintf(viESA, (char*)"CALC:MARK1:MAX\n");

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    iOper		= 0;
    ucOPCCnt	= 0;
    *out_pfPeakValue = 0.0;
    /*Read the marker value*/
    status = viQueryf(viESA, (char*)"CALC:MARK1:Y?\n", "%f", out_pfPeakValue);

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }


    iOper		= 0;
    ucOPCCnt	= 0;


    status = viQueryf(viESA, (char*)"CALC:MARK1:X?\n", "%lf", &ldFreq);

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    *out_pdFreq = ldFreq;

    return status;
}

short CIVI_SPECTRUM:: AgSA_ReadNextPeakValue(session ulVi, float* out_pfPeakValue, double* out_pdFreq)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    char szCmdStr[50] ={0};

    /* to set the Marker in Peak*/
    status = viPrintf(viESA, (char*)"CALC:MARK1:MAX:NEXT\n");
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    iOper		= 0;
    ucOPCCnt	= 0;
    *out_pfPeakValue = 0.0;

    /*Read the marker value*/
    status = viQueryf(viESA, (char*)"CALC:MARK1:Y?\n", "%f", out_pfPeakValue);

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    iOper		= 0;
    ucOPCCnt	= 0;
    *out_pdFreq = 0.0;

    status = viQueryf(viESA, (char*)"CALC:MARK1:X?\n", "%lf", out_pdFreq);
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }
    return status;
}

/***************************************************************************************************
// Name						:	 AgSA_SetRFBTrig
// Author					:	 Shanmugam .S
// Global Variables affected:	 NA
// Created Date				:    25-june-2013
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures the trigger type as RF burst
******************************************************************************************************/
short CIVI_SPECTRUM:: AgSA_SetRFBTrig(session ulVi, unsigned char ucTrigType)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    char szCmdStr[50] ={0};

    /* to set the Marker in Peak*/
    if(ucTrigType == 1) // RF Burst
    {
        status = viPrintf(viESA, (char*)"TRIG:SOUR RFB\n");
        status = viPrintf(viESA, (char*)"TRIG:RFB:LEV:ABS -80\n");
        status = viPrintf(viESA, (char*)"TRIG:RFB:SLOP POS\n");
        status = viPrintf(viESA, (char*)"TRIG:RFB:DEL 0.0\n");
        status = viPrintf(viESA, (char*)"TRIG:RFB:LEV:TYPE REL \n");
        status = viPrintf(viESA, (char*)"TRIG:RFB:LEV:REL 0.00\n");
    }
    else if(ucTrigType == 2) //Video Trigger
    {
        status = viPrintf(viESA, (char*)"TRIG:SOUR VID\n");
    }

    else if(ucTrigType == 3) //FRee Run
    {
        status = viPrintf(viESA, (char*)"TRIG:SOUR IMM\n");
    }

    //	viPrintf(viESA,":SENS:FREQ:TUNE:IMM\n");//added by Senthil vason for Auto tune 16/4/2013
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }
    status = viPrintf(viESA, (char*)"INIT:CONT ON\n");
    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_Config_Meas_Mode
// Author					:	 Priyanka T
// Global Variables affected:	 NA
// Created Date				:    09-01-10
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function Used to Change Measuring Mode
******************************************************************************************************/

short CIVI_SPECTRUM::DP_Enum_SpecAn_PeakThresholdOnOff(session in_uiVi, unsigned char in_ucOnOff)
{
    int iOpc = 0;
    char StrFormat[64]= "";

    if(in_ucOnOff)
    {
        /* Sets the marker mode as normal*/
        status = viPrintf(viESA,(char*)":CALC:MARK:PEAK:THR:STAT ON\n");

        if(status)
            return status;
    }
    else
    {
        /* Sets the marker mode as normal*/
        status = viPrintf(viESA,(char*)":CALC:MARK:PEAK:THR:STAT OFF\n");

        if(status)
            return status;
    }
    return 0;

}



short CIVI_SPECTRUM::DP_Enum_Mode_Preset(session in_uiVi)
{
    status = viPrintf(viESA,(char*)":SYST:PRES\n");
    if(status)
        return status;
    return 0;
}

/***************************************************************************************************
// Name						:	 DP_Enum_Set_Scale
// Author					:	 Naresh G
// Global Variables affected:	 NA
// Created Date				:    25-04-14
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function Used to configure scale/div
******************************************************************************************************/
short CIVI_SPECTRUM::DP_Enum_Set_Scale(session in_uiVi,double in_dScale)
{
    status = viPrintf(viESA,(char*)"DISP:WIND:TRAC:Y:PDIV %f\n", in_dScale);
    if(status)
        return status;
    return 0;

}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_Config_AutoAlign
// Author					:	 Naresh G
// Global Variables affected:	 NA
// Created Date				:    21-04-14
// Revision Date			:
// Reason for Revising		:
// Description				:	 This function Used to ON/OFF auto alignment
******************************************************************************************************/

short CIVI_SPECTRUM::DP_Enum_SpecAn_Config_AutoAlign(session in_uiVi, unsigned char in_ucOnOff)
{
    int iOpc = 0;
    char StrFormat[64]= "";

    if(in_ucOnOff)
    {
        /* Sets the marker mode as normal*/
        status = viPrintf(viESA,(char*)":CAL:AUTO ON\n");

        if(status)
            return status;
    }
    else
    {
        /* Sets the marker mode as normal*/
        status = viPrintf(viESA,(char*)":CAL:AUTO OFF\n");

        if(status)
            return status;
    }
    return 0;

}

/***************************************************************************************************
// Name						:	 AgSA_ConfigAmpDUTSetup
// Author					:	 Shanmugam .S
// Global Variables affected:	 NA
// Created Date				:    10-june-2013
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures Noise Figure Mode DUT Setup
******************************************************************************************************/
short CIVI_SPECTRUM::AgSA_ConfigAmpDUTSetup(session ulVi, double in_fStartFreq, double in_fStopFreq, int in_iNoOfPoints)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[100] ={0};

    status = viPrintf(viESA,(char*)":NFIG:MODE:DUT AMPL\n");

    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set tthe frequency as Sweep frequency*/
    status = viPrintf(viESA,(char*)":NFIG:FREQ:MODE SWEP\n");
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    sprintf(szCmdStr, ":NFIG:FREQ:STAR %f\n", in_fStartFreq);
    /* to set the start frequency */
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set the stop frequency */

    sprintf(szCmdStr, ":NFIG:FREQ:STOP %f\n", in_fStopFreq);

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

    /* to set the Sweep Points*/
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    //	Sleep(100);

    return 0;
}

/***************************************************************************************************
// Name						:	 AgSA_ConfigAmpDUTSetup
// Author					:	 Kaushik Piparotar
// Global Variables affected:	 NA
// Created Date				:    20-11-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures Noise Figure Mode DUT Setup
******************************************************************************************************/
short CIVI_SPECTRUM::AgSA_AmpDUTSetupConfig(session ulVi, double in_fStartFreq, double in_fStopFreq, int in_iNoOfPoints)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[100] ={0};

    //	status = viPrintf(viESA,":NFIG:MODE:DUT AMPL\n");
    status = viPrintf(viESA,(char*)"SENS:CONF:MODE:DUT AMPL\n");
    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set tthe frequency as Sweep frequency*/
    status = viPrintf(viESA,(char*)":NFIG:FREQ:MODE SWEP\n");
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    sprintf(szCmdStr, ":NFIG:FREQ:STAR %f\n", in_fStartFreq);
    /* to set the start frequency */
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set the stop frequency */

    sprintf(szCmdStr, (char*)":NFIG:FREQ:STOP %f\n", in_fStopFreq);

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

    /* to set the Sweep Points*/
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    //	Sleep(100);

    return 0;
}

short CIVI_SPECTRUM::DP_Enum_SpecAn_ContRun(session in_uiVi, unsigned char in_ucOnOff)
{
    int iOpc = 0;

    if(in_ucOnOff)
    {
        /* Sets the marker mode as normal*/
        status = viPrintf(viESA,(char*)":INIT:CONT ON\n");

        if(status)
            return status;
    }
    else
    {
        /* Sets the marker mode as normal*/
        status = viPrintf(viESA,(char*)":INIT:CONT OFF\n");

        if(status)
            return status;
    }

    return 0;
}

short CIVI_SPECTRUM::AgSA_NoiseSourceDetection(session ulVi,short *out_sStatus)
{

    char szCmdStr[1024] = {0};
    memset(szCmdStr, 0, 1024);

    sprintf(szCmdStr, "INST:SEL NFIGURE\n");

    /* Sets the marker mode as normal*/
    status = viPrintf(viESA,szCmdStr);
    if(status)
    {
        return status;
    }

    sprintf(szCmdStr, "NFIG:CORR:ENR:SNS:ATT?\n");
    //	sprintf(szCmdStr, "SENS:NFIG:CORR:ENR:SPOT?\n");

    status = viQueryf(viESA,szCmdStr,"%d",out_sStatus);
    if(status)
    {
        return status;
    }

    //	Sleep(1000);

    return 0;
}
/***************************************************************************************************
// Name						:	 AgSA_PreAmpEnableDisable()
// Author					:	 Kaushik Piparotar
// Global Variables affected:	 NA
// Created Date				:    31-03-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	This function use to Pre- AMP enable/Disable
********************************************************************************************************/

short CIVI_SPECTRUM::AgSA_PreAmpEnableDisable(session ulVi,unsigned char in_ucSts)
{

    char szCmdStr[1024] = {""};
    memset(szCmdStr, 0, 1024);

    if(!in_ucSts)
    {
        sprintf(szCmdStr, ":POW:GAIN ON\n");
    }
    else
    {
        sprintf(szCmdStr, ":POW:GAIN OFF\n");
    }

    /* Sets the marker mode as normal*/
    status = viPrintf(viESA,szCmdStr);
    if(status)
    {
        return status;
    }

    //	Sleep(1000);

    return 0;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigRBW_VBW()
// Author					:	 Kaushik Piparotar
// Global Variables affected:	 NA
// Created Date				:    31-03-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	This function use to Configure VBW and RBW
********************************************************************************************************/
short CIVI_SPECTRUM::DP_Enum_SpecAn_ConfigRBW_VBW(session in_uiVi, double in_dResolutionBW,  double in_dVideoBW)
{

    char szRWBCmdStr[1024] = {""};
    char szVBWCmdStr[1024] = {""};

    memset(szRWBCmdStr, 0, 1024);
    memset(szVBWCmdStr, 0, 1024);

    sprintf(szRWBCmdStr, "BAND %lf KHZ\n",in_dResolutionBW/1000);
    sprintf(szVBWCmdStr, "BAND:VID %lf kHz\n",in_dVideoBW/1000);

    /* Sets the marker mode as normal*/
    status = viPrintf(viESA,szRWBCmdStr);
    if(status)
    {
        return status;
    }

    //	Sleep(200);

    /* Sets the marker mode as normal*/
    status = viPrintf(viESA,szVBWCmdStr);
    if(status)
    {
        return status;
    }

    //	Sleep(200);

    return 0;
}

short CIVI_SPECTRUM::AgSA_ConfigUpConvDUTSetup(session ulVi, double in_fStartFreq, double in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints, unsigned char in_ucRFContextConfigFlag ,unsigned char in_ucSideBandConfigFlag)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[100] ={0};


    status = viPrintf(viESA,(char*)":NFIG:MODE:DUT Upconv\n");
    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set the frequency as Sweep frequency*/
    status = viPrintf(viESA,":NFIG:FREQ:MODE SWEP\n");
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /*to Set frequency of LO as Swept*/
    //	status = viPrintf(viESA,":NFIG:MODE:DUT:LOSC SWEPT\n");
    status = viPrintf(viESA,(char*)":NFIG:MODE:DUT:LOSC FIXED\n");
    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }
    if(in_ucRFContextConfigFlag == 0)
    {
        //To set Frwequency Context
        sprintf(szCmdStr, ":NFIG:MODE:FREQ:CONT RF\n");

    }
    else
    {
        //To set Frwequency Context
        sprintf(szCmdStr, ":NFIG:MODE:FREQ:CONT IF\n");

    }

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    if(in_ucSideBandConfigFlag == 0)
    {
        status = viPrintf(viESA,(char*)":NFIG:MODE:Upconv:LOSC:OFFS LSB\n");
    }
    else if(in_ucSideBandConfigFlag == 1)
    {
        status = viPrintf(viESA,(char*)":NFIG:MODE:Upconv:LOSC:OFFS USB\n");
    }
    else if(in_ucSideBandConfigFlag == 2)
    {
        status = viPrintf(viESA,(char*)":NFIG:MODE:Upconv:LOSC:OFFS DSB\n");
    }

    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

#if 0
    //To Set the IF Frequency
    sprintf(szCmdStr, ":NFIG:MODE:DOWN:IF:FREQ %f\n", in_fIFFreq);

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }
#endif
    sprintf(szCmdStr, ":NFIG:FREQ:STAR %lf\n", in_fStartFreq);
    /* to set the start frequency */
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set the stop frequency */
    sprintf(szCmdStr,":NFIG:FREQ:STOP %lf\n", in_fStopFreq);

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

    /* to set the Sweep Points*/
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    //	Sleep(100);

    return 0;
}

short CIVI_SPECTRUM::AgSA_UpConvDUTSetupConfig(session ulVi,unsigned char in_ucFreqMode,double in_fStartFreq, double in_fStopFreq,unsigned char in_ucLOFreqMode, float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[100] = {0};

    status = viPrintf(viESA,(char*)"SENS:CONF:MODE:DUT Upconv\n");
    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    if(in_ucFreqMode == 0)
    {
        status = viPrintf(viESA,(char*)":NFIG:FREQ:MODE SWEPT\n");
    }

    else
    {
        status = viPrintf(viESA,(char*)":NFIG:FREQ:MODE FIXED\n");
    }

    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    if(in_ucLOFreqMode == 0)
    {
        status = viPrintf(viESA,(char*)":NFIG:MODE:DUT:LOSC SWEPT\n");
    }
    else
    {
        status = viPrintf(viESA,(char*)":NFIG:MODE:DUT:LOSC FIXED\n");
    }
    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }
    if(in_ucRFContextConfigFlag == 0)
    {
        sprintf(szCmdStr, "SENS:CONF:MODE:UPC:FREQ:CONT RF\n");
    }
    else
    {
        sprintf(szCmdStr, "SENS:CONF:MODE:UPC:FREQ:CONT IF\n");
    }

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    if(in_ucSideBandConfigFlag == 0)
    {
        status = viPrintf(viESA,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS LSB\n");
    }
    else if(in_ucSideBandConfigFlag == 1)
    {
        status = viPrintf(viESA,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS USB\n");
    }
    else if(in_ucSideBandConfigFlag == 2)
    {
        status = viPrintf(viESA,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS DSB\n");
    }

    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    sprintf(szCmdStr, ":NFIG:FREQ:STAR %lf\n", in_fStartFreq);
    /* to set the start frequency */
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set the stop frequency */
    sprintf(szCmdStr, ":NFIG:FREQ:STOP %lf\n", in_fStopFreq);

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

    /* to set the Sweep Points*/
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    //Sleep(100);

    return 0;
}


//To Configure Spectrum in Noise Figure Frequency Mode
short CIVI_SPECTRUM::AgSA_ConfigNFDownConvFreqMode(session ulVi)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[50] ={0};

    /* to set the frequency as Sweep frequency*/
    /*	status = viPrintf(viESA,":NFIG:FREQ:MODE SWEPT\n");
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(viESA, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }*/


    /* to set LO Fixed Mode*/
    status = viPrintf(viESA,(char*)":NFIG:MODE:DUT:LOSC FIXED\n");
    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set the frequency as Sweep frequency*/
    status = viPrintf(viESA,(char*)":NFIG:FREQ:MODE FIXED\n");
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    //To set Frwequency Context
    sprintf(szCmdStr, ":NFIG:MODE:FREQ:CONT IF\n");
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    return 0;
}


/***************************************************************************************************
// Name						:	 DP_Enum_ConfigureVideoTrigger()
// Author					:	 Kaushik Piparotar
// Global Variables affected:	 NA
// Created Date				:    24-11-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Video Trigger of the Spectrum Analyser
******************************************************************************************************/
short CIVI_SPECTRUM::DP_Enum_ConfigureVideoTrigger(session in_uiVi, double in_dVideoTriggerLevel,int in_iVideoTriggerSlope)
{

    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[100] = {0};

    status = viPrintf(viESA,(char*)"TRIG:SOUR VID\n");
    if(status)
        return status;

    char szVISlopeMode[2][15] = {"NEG", "POS"};
    status = viPrintf(viESA,(char*)"TRIG:VID:SLOP %s\n", szVISlopeMode[in_iVideoTriggerSlope]);
    if(status)
        return status;

    sprintf(szCmdStr, "TRIG:VID:LEV %lf\n", in_dVideoTriggerLevel);
    status = viPrintf(viESA,szCmdStr);
    if(status)
        return status;


    return status;
    //	Sleep(1000);
}

/***************************************************************************************************
// Name						:	 DP_Enum_ConfigureFreeRun()
// Author					:	 Kaushik Piparotar
// Global Variables affected:	 NA
// Created Date				:    24-11-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Trigger as free run in Spectrum Analyser
******************************************************************************************************/
short CIVI_SPECTRUM::DP_Enum_ConfigureFreeRun(session in_uiVi)
{
    QString csFormat = "";
    short status	   = 0;
    unsigned char ucOPCCnt	   = 0;
    int iOper		   = 0;
    char szCmdStr[100] = {0};

    status = viPrintf(viESA,(char*)"TRIG:SOUR IMM\n");
    if(status)
        return status;

    return 0;
}

short CIVI_SPECTRUM::   AgSA_ConfigDownConvDUTSetup(session ulVi, unsigned char in_ucFreqMode, double in_fStartFreq, double in_fStopFreq, unsigned char in_ucLOFreqMode, float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag)
{
    short status	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[100] ={0};

    status = viPrintf(viESA,(char*)":NFIG:MODE:DUT DOWN\n");
    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /* to set the frequency as Sweep frequency*/
    if(in_ucFreqMode == 0)
    {
        status = viPrintf(viESA,(char*)":NFIG:FREQ:MODE SWEPT\n");
    }
    else
    {
        status = viPrintf(viESA,(char*)":NFIG:FREQ:MODE FIXED\n");
    }

    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    /*to Set frequency of LO as Swept*/
    if(in_ucLOFreqMode == 0)
    {
        status = viPrintf(viESA,(char*)":NFIG:MODE:DUT:LOSC SWEPT\n");
    }
    else
    {
        status = viPrintf(viESA,(char*)":NFIG:MODE:DUT:LOSC FIXED\n");
    }

    if(status)
    {
        return status;
    }

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }
    if(in_ucRFContextConfigFlag == 0)
    {
        //To set Frwequency Context
        sprintf(szCmdStr, ":NFIG:MODE:FREQ:CONT RF\n");

    }
    else
    {
        //To set Frwequency Context
        sprintf(szCmdStr, ":NFIG:MODE:FREQ:CONT IF\n");

    }

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    if(in_ucSideBandConfigFlag == 0)
    {
        status = viPrintf(viESA,":NFIG:MODE:DOWN:LOSC:OFFS LSB\n");
    }
    else if(in_ucSideBandConfigFlag == 1)
    {
        status = viPrintf(viESA,":NFIG:MODE:DOWN:LOSC:OFFS USB\n");
    }
    else if(in_ucSideBandConfigFlag == 2)
    {
        status = viPrintf(viESA,":NFIG:MODE:DOWN:LOSC:OFFS DSB\n");
    }

    if(status)
        return status;

    ucOPCCnt = 0;
    iOper	 = 0;

    while(!iOper)
    {
        viQueryf(viESA, "*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }


    //To Set the IF Frequency
    sprintf(szCmdStr, ":NFIG:MODE:DOWN:IF:FREQ %f\n", in_fIFFreq);

    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, "*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    if(in_ucFreqMode == 0)
    {
        sprintf(szCmdStr, ":NFIG:FREQ:STAR %lf\n", in_fStartFreq);
        /* to set the start frequency */
        status = viPrintf(viESA, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(viESA, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the stop frequency */
        sprintf(szCmdStr, ":NFIG:FREQ:STOP %lf\n", in_fStopFreq);

        status = viPrintf(viESA, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(viESA, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }
    else
    {
        sprintf(szCmdStr, ":NFIG:FREQ:FIXED %lf\n", in_fStartFreq);
        /* to set the start frequency */
        status = viPrintf(viESA, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(viESA, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

    /* to set the Sweep Points*/
    status = viPrintf(viESA, szCmdStr);
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(viESA, "*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

     QThread::msleep(100);

    return 0;
}
/***************************************************************************************************
// Name						:	 AgSA_ConfigMaxHold
// Author					:	 Shanmugam .S 	
// Global Variables affected:	 NA				 
// Created Date				:    25-june-2013
// Revision Date			:	 
// Reason for Revising		:
// Description				:	 Configures Noise Figure Mode DUT Setup
******************************************************************************************************/
/*
short CIVI_SPECTRUM:: AgSA_ConfigMaxHold(session ulVi)
{
	short status	= 0;
	UCHAR ucOPCCnt	= 0;
	int iOper		= 0;
	
	char szCmdStr[50] ={0};
		
    to set the Trace  in
	status = viPrintf(viESA, "TRAC1:TYPE MAXH\n");

	while(!iOper)
	{
		viQueryf(viESA, "*OPC?\n", "%d", &iOper);	
		
		if(ucOPCCnt > 100)
			break;
		
		ucOPCCnt++;
	}
	
	return status;
}

*/
/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_FetchYTrace()
// Author					:	 Venkatesh.p 	
// Global Variables affected:	 NA				 
// Created Date				:    06-07-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	 Configure Configure Read of the Spectrum Analyser
******************************************************************************************************/


//short CIVI_SPECTRUM::DP_Enum_SpecAn_FetchYTrace (session in_uiVi, const char *in_pcTraceName,  signed long in_slArrayLength, signed long* out_slActPts, ContainerArray* pContainerArray)
//short CIVI_SPECTRUM::DP_Enum_SpecAn_FetchYTrace (session in_uiVi,unsigned short in_Traceindex,  signed long in_slArrayLength, ContainerArray* pContainerArray)
short CIVI_SPECTRUM::DP_Enum_SpecAn_FetchYTrace (session in_uiVi,unsigned short in_Traceindex,  signed long in_slArrayLength, double* out_pdAmplitude)
{
	
	ViUInt32 u32RetCnt = 0;
	int i32StrLength = 0;
	unsigned short usIndex =0;
    static ViChar *pcTok = NULL;
    char *pszYTraceData = NULL;
		
	i32StrLength =  in_slArrayLength * 19;
	
    pszYTraceData = (char *)malloc(i32StrLength);
	
	if(pszYTraceData == NULL)
	{
        return -1;
	}

/* Simulation Mode of Operation */
#ifdef TESTSEQ_WITHOUT_SPECTRUM_MODULE
	

//	csFormat.Format(" SpecAn Config Fetch(in_uiVi = %ld, TraceName = %s, ArrayLength = %ld, ActPts = %ld, Amplitude = %f)", in_uiVi, in_pcTraceName,  in_slArrayLength, out_slActPts, in_dAmplitude);
//	AfxMessageBox(csFormat);
	
#else

    status = viPrintf(in_uiVi, ":TRACE? TRACE%d\n", in_Traceindex);
    if(status)
	{
		free(pszYTraceData);
		pszYTraceData = NULL;
        return status;
	}

    status = viRead(in_uiVi, (ViPBuf)pszYTraceData, i32StrLength, &u32RetCnt);
    if(status)
	{
		free(pszYTraceData);
		pszYTraceData = NULL;
        return status;
	}
    pcTok = strtok(pszYTraceData, ",");
    
	usIndex = 0;
	
	while(pcTok != NULL)
    {
        out_pdAmplitude[usIndex] = atof(pcTok);
		
        pcTok = strtok(NULL,",");
		
        usIndex++;
		if(usIndex == in_slArrayLength)
		{
			break;
		}
    }

    free(pszYTraceData);
    pszYTraceData = NULL;
	
#if 1
	FILE *fpLog = fopen("YTradeData.txt", "w");

	if(fpLog)
	{
        for(unsigned long ulIdx = 0; ulIdx < in_slArrayLength;  ulIdx++)
		{
            fprintf(fpLog, "%f\n", out_pdAmplitude[ulIdx]);
		}

	}    
    fclose(fpLog);
    fpLog = NULL;
    
#endif
#endif


	return 0;
}
short CIVI_SPECTRUM:: AgSA_SetThersHold(session in_uiVi)
{
    short status = 0;

    status = viPrintf(viESA,(char*)":CALC:MARK:PEAK:THR:STAT ON\n");

    status = viPrintf(viESA,(char*)":CALC:MARK:PEAK:THR -120 dBm\n");

    status = viPrintf(viESA,(char*)":CALC:MARK:PEAK:EXC:STAT OFF\n");

    return status;
}
