#include "SAWrapper.h"
#include <windows.h>
#include <QString>
#include "src/DP_datastructure.h"

extern SGlobalStucture gGlobalStuct;
extern unsigned char g_ucSACount;
CSAWrapper::CSAWrapper()
{
    memset(m_arrSInstHandle, 0, sizeof(m_arrSInstHandle) );
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

short CSAWrapper::DP_SPEC_AN_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize)
{
    if(out_ps8ErrStringBuf == NULL)
        return  DP_ERR_INVALID_PARAM;

    switch(in_s32ErrCode)
    {
    case DP_VI_OPEN_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize, "ViOpen Failed");
        break;

    case DP_SPECAN_CLS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"ViClose Failed!!");
        break;

    case DP_SPECAN_CNFG_FREQSTART_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Frequency Start Failed !!");
        break;

    case DP_SPECAN_CNFG_FREQSTOP_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Frequency Stop Failed !!");
        break;

    case DP_SPECAN_CNFG_FREQCENTER_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Frequency Center Failed");
        break;

    case DP_SPECAN_CNFG_FREQSPAN_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Frequency Span Failed");
        break;

    case DP_SPECAN_CNFG_FREQOFFSET_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Frequency Center Failed");
        break;

    case DP_SPECAN_CNFG_PEAK_SRCH_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Automatic Peak Search Failed");
        break;

    case DP_SPECAN_CNFG_PEAK_EXC_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Peak Excursion Failed");
        break;

    case DP_SPECAN_CNFG_TRACE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Set Marker Trace Failed");
        break;

    case DP_SPECAN_CNFG_BWAUTO_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring sweep coupling Resolution BandWidth Auto Failed");
        break;

    case DP_SPECAN_CNFG_RESBW_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring sweep coupling Resolution BandWidth Failed");
        break;

    case DP_SPECAN_CNFG_VIDBWAUTO_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring sweep coupling Video BandWidth Auto Failed");
        break;

    case DP_SPECAN_CNFG_VIDBW_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring sweep coupling video BandWidth Failed");
        break;

    case DP_SPECAN_CNFG_SWTIMEAUTO_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring sweep coupling time Auto Failed");
        break;

    case DP_SPECAN_CNFG_SWTIME_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring sweep coupling sweeptime Failed");
        break;

    case DP_SPECAN_READ_SWTIME_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read sweeptime Failed");
        break;

    case DP_SPECAN_CNFG_REFLVL_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Reference Level Failed");
        break;

    case DP_SPECAN_DET_HAR_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Harmonic Detection of Reference level,Frequency,RBW Failed");
        break;

    case DP_SPECAN_CNFG_TRIGSOUR_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Trigger Source Failed");
        break;

    case DP_SPECAN_CNFG_TRIGLVL_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring External Trigger Level Failed");
        break;

    case DP_SPECAN_CNFG_TRIGPOL_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring External Trigger Polarity Failed");
        break;

    case DP_SPECAN_CNFG_TRIGSLOPE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Video Trigger Slope Failed");
        break;

    case DP_SPECAN_CNFG_VIDTRIG_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Video Trigger Failed");
        break;

    case DP_SPECAN_CNFG_TRIGDELAY_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Video Trigger Delay Failed");
        break;

    case DP_SPECAN_CNFG_MIXHARM_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring External Mixer Harmonic Failed");
        break;

    case DP_SPECAN_CNFG_MIXBIAS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring External MixerbBias Enabled Failed");
        break;

    case DP_SPECAN_READ_ACTIVEMARKER_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Active Marker Frequency Failed");
        break;

    case DP_SPECAN_READ_PHASENOISE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Phase Noise Failed");
        break;

    case DP_SPECAN_CNFG_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Mode Configuration Failed");
        break;

    case DP_SPECAN_CNFG_FREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Frequency Failed");
        break;

    case DP_SPECAN_READ_NF_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read NoiseFigure Failed");
        break;

    case DP_SPECAN_HARDIS_MEAS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Harmonic Distortion Measurement Failed");
        break;

    case DP_SPECAN_HARCNT_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Harmonic Count Failed");
        break;

    case DP_SPECAN_DRBW_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring drbw Failed");
        break;

    case DP_SPECAN_FUNFREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Fundamental Frequency Failed");
        break;

    case DP_SPECAN_READ_HARVAL_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Harmonic String Value Failed");
        break;

    case DP_SPECAN_READ_HARFREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Harmonic string Frequency Failed");
        break;

    case DP_SPECAN_CNFG_SPUR_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Spurious Failed");
        break;

    case DP_SPECAN_RESET_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Reset Failed");
        break;

    case DP_SPECAN_INIT_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Initiate Failed");
        break;

    case DP_SPECAN_ABORT_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Abort Failed");
        break;

    case DP_SPECAN_NS_CALIB_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Noise Source Calibration Failure");
        break;

    case DP_SPECAN_NS_SETTLING_TIME_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Noise Source Settling time Failure");
        break;

    case DP_SPECAN_NS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Calibrated NoiseSource Failed");
        break;

    case DP_SPECAN_PHASE_NS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read PhaseNoise Failed");
        break;

    case DP_SPECAN_PHASE_NSAT_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read PhaseNoise autotune Failed");
        break;

    case DP_SPECAN_PHASENOISE_MM_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read PhaseNoise markermode Failed");
        break;

    case DP_SPECAN_CNFG_AUTO_SWEEP_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring AutoSweep Failed");
        break;;
    case DP_SPECAN_CNFG_MARAMP_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Marker frequency Failed");
        break;

    case DP_SPECAN_READ_MARKER_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Read Marker Value Failed");
        break;

    case DP_SPECAN_PEAK_ON_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Peak Threshold On Failed");
        break;

    case DP_SPECAN_PEAK_OFF_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Peak Threshold Off Failed");
        break;

    case DP_SPECAN_PRESET_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Mode Preset Failed");
        break;

    case DP_SPECAN_SETSCALE_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Set Scale Failed");
        break;

    case DP_SPECAN_CNFG_AUTO_ON_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring AutoOn Failed");
        break;

    case DP_SPECAN_CNFG_AUTO_OFF_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring AutoOff Failed");
        break;

    case DP_SPECAN_AMP_DUT_SETUP_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"RSSA Amplifier DUTSetup Configuring Failed");
        break;

    case DP_SPECAN_CONT_ON_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Continuous On Failed");
        break;

    case DP_SPECAN_CONT_OFF_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Continuous Off Failed");
        break;

    case DP_SPECAN_NS_DET_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"RSSA NoiseSource Detection Failed");
        break;

    case DP_SPECAN_PREAMP_ENADIS_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"RSSA PreAmplifier Enable Disable Failed");
        break;

    case DP_SPECAN_RBW_VBW_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Resolution bandwidth Video bandwidth Failed");
        break;

    case DP_SPECAN_UPCON_DUT_SETUP_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"RSSA Configuring UpConvertion DUTSetup Failed");
        break;

    case DP_SPECAN_DUT_UPCON_SETUP_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"RSSA UpConvertion DUTSetup Configuring Failed");
        break;

    case DP_SPECAN_DUT_DOWNCON_SETUP_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring NoiseFigure DownConvertion Frequency ModeFailed");
        break;

    case DP_SPECAN_VID_TRIG_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring Video Trigger Failed");
        break;

    case DP_SPECAN_FREE_RUN_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring FreeRun Failed");
        break;

    case DP_SPECAN_DOWNCON_DUT_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring DownConvertion DUTSetup Failed");
        break;
    case DP_SPECAN_CNFG_RBWVBW_FAILURE:
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Configuring DownConvertion DUTSetup Failed");
        break;

    case DP_SPECAN_INT_INVALID_MODEL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Signal GeneratorInvalid Model!");
        break;

    case DP_SPECAN_INT_ID_READ_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Signal Generator Model Number Read Failure!");
        break;

    default:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid Error Code !");
    }

}

short CSAWrapper::Open(unsigned char in_u8InstNo, char *pLogicalName)
{
    ViChar value[128]  = {0};
    ViSession rm    =  0;
    ViSession viESA ;
    unsigned char ucLoop = 0;
    unsigned short usInstID = 0;
    unsigned int uiReadCnt = 0;
    char arripadress[100]={0};
    char szInstDetails [256] = {0};
    char ucTemp[256] ={0};
    strcpy(value,pLogicalName);

    m_viStatus = viOpenDefaultRM(&rm);
    if(m_viStatus)
    {
        return DP_VI_OPEN_FAILURE;
    }

    sprintf(arripadress, "%s", pLogicalName);
    m_viStatus = viOpen(rm, arripadress, VI_NULL, VI_NULL, &viESA);
    
    if(m_viStatus)
    {
        return DP_VI_OPEN_FAILURE;
    }

    for( ucLoop = 8; ucLoop <= 18; ucLoop++)
    {
        ucTemp[ucLoop-8] = value[ucLoop];
    }

    strcpy(gGlobalStuct.sCOTSInstInfo.m_cSAInfo[in_u8InstNo-1],ucTemp);

    /*Read Instrument Type*/

    m_viStatus = viPrintf(viESA, (char*)"*IDN?\n");
    if(m_viStatus)
    {
        return DP_SPECAN_INT_ID_READ_FAILED;
    }

    m_viStatus = viBufRead(viESA, (ViPBuf)szInstDetails,256, (ViPUInt32)&uiReadCnt);
    if(m_viStatus)
    {
        return DP_SPECAN_INT_ID_READ_FAILED;
    }

    /*Read the Model ID*/
   m_viStatus = GetInstID(szInstDetails, &usInstID);

   if(m_viStatus != 0)
   {
       viClose(viESA);

       return DP_SPECAN_INT_INVALID_MODEL;
   }
   else
   {
      m_arrSInstHandle[in_u8InstNo-1].m_viSASession = viESA;
      m_arrSInstHandle[in_u8InstNo-1].m_usInstID = usInstID;
   }

    return DP_SPEC_AN_CNFG_SUCCESS;

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


short CSAWrapper::DP_Enum_SpecAn_ConfigFreqStartStop(U8BIT in_u8InstNo, double in_dStartFreq, double in_dStopFreq)
{
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
       m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"FREQ:STAR %lf\n", in_dStartFreq);
       if (m_viStatus)
       {
           return DP_SPECAN_CNFG_FREQSTART_FAILURE;
       }
       m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"FREQ:STOP:STOP %lf\n", in_dStopFreq);
       if (m_viStatus)
       {
           return DP_SPECAN_CNFG_FREQSTOP_FAILURE;
       }
    }
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"FREQ:STAR %lf\n", in_dStartFreq);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQSTART_FAILURE;
        }
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"FREQ:STOP %lf\n", in_dStopFreq);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQSTOP_FAILURE;
        }
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
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


short CSAWrapper::DP_Enum_SpecAn_ConfigFreqCenterSpan(U8BIT in_u8InstNo, double in_dCenterFreq, double in_dSpan)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":FREQ:CENT %lf\n", in_dCenterFreq);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQCENTER_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":FREQ:SPAN  %lf\n", in_dSpan);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQSPAN_FAILURE;
        }
    }
    else
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":SENS:FREQ:CENT %lf\n", in_dCenterFreq);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQCENTER_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":SENS:FREQ:SPAN  %lf\n", in_dSpan);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQSPAN_FAILURE;
        }
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
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


short CSAWrapper::DP_Enum_SpecAn_ConfigFreqOffset(U8BIT in_u8InstNo, double in_dFreqOffset)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"FREQ:OFFS %lf\n", in_dFreqOffset);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQOFFSET_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper::DP_Enum_SpecAn_ConfigSweepCoupling(U8BIT in_u8InstNo, bool in_bResolutionBWAuto, double in_dResolutionBW, bool in_bVideoBWAuto, double in_dVideoBW, bool in_bSweepTimeAuto, double in_dSweepTime)
{
   if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
   {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"BWID:AUTO %d\n",in_bResolutionBWAuto);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_BWAUTO_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"BAND %lf\n",in_dResolutionBW);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_RESBW_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"BWID:VID:AUTO %d\n",in_bVideoBWAuto);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_VIDBWAUTO_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"BAND:VID %lf\n",in_dVideoBW);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_VIDBW_FAILURE;
        }
        if(in_bSweepTimeAuto)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"SWE:TIME:AUTO %d\n", in_bSweepTimeAuto);
            if (m_viStatus)
            {
                return DP_SPECAN_CNFG_SWTIMEAUTO_FAILURE;
            }
        }
        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"SWE:TIME %lf\n", in_dSweepTime);
            if (m_viStatus)
            {
                return DP_SPECAN_CNFG_SWTIME_FAILURE;
            }
        }


    }
   else
   {
       if(in_bResolutionBWAuto)
       {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"BWID:AUTO %d\n",in_bResolutionBWAuto);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_BWAUTO_FAILURE;
        }
       }
       else
       {

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"SENS:BAND:RES %lf\n",in_dResolutionBW);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_RESBW_FAILURE;
        }
       }

       if(in_bVideoBWAuto)
       {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"BWID:VID:AUTO %d\n",in_bVideoBWAuto);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_VIDBWAUTO_FAILURE;
        }
       }
       else
       {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"SENS:BAND:RES:VID %lf\n",in_dVideoBW);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_VIDBW_FAILURE;
        }
       }
        if(in_bSweepTimeAuto)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"SWE:TIME:AUTO %d\n", in_bSweepTimeAuto);
            if (m_viStatus)
            {
                return DP_SPECAN_CNFG_SWTIMEAUTO_FAILURE;
            }
        }
        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"SENS:SWE:TIME %lf\n", in_dSweepTime);
            if (m_viStatus)
            {
                return DP_SPECAN_CNFG_SWTIME_FAILURE;
            }
        }


    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ReadConfigSweep()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    16-10-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	 Read Configure Sweep Time of the Spectrum Analyser
******************************************************************************************************/
short CSAWrapper::DP_Enum_SpecAn_ReadConfigSweep(U8BIT in_u8InstNo, double *out_dSweepTime)
{
    short m_viStatus	   = 0;
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":SENS:SWE:TIME?\n", "%lf", out_dSweepTime);
        if(m_viStatus)
        {
            return DP_SPECAN_READ_SWTIME_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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

short CSAWrapper::DP_Enum_SpecAn_ConfigLevel (U8BIT in_u8InstNo,double in_dRefLevel)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":DISP:WIND:TRAC:Y:RLEV %f\n", in_dRefLevel);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_REFLVL_FAILURE;
        }

        m_viStatus = DP_Enum_SpecAn_WaitForOperation_Complete(in_u8InstNo, 100);
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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


short CSAWrapper::DP_Enum_SpecAn_ConfigTriggerSource(U8BIT in_u8InstNo, signed long in_slTriggerSource)
{
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        char szTrigMode[7][15] = {"EXTernal1", "IMMediate", "IFPower", "VIDeo","RFPower", "BBPower", "PSEN"};
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:SOUR %s\n",szTrigMode[in_slTriggerSource]);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_TRIGSOUR_FAILURE;
        }
    }
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
    {
        char szTrigMode[4][15] = {"EXTernal1", "IMMediate", "LINE", "VIDeo"};
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:SOUR %s\n",szTrigMode[in_slTriggerSource]);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_TRIGSOUR_FAILURE;
        }
    }
    
    return DP_SPEC_AN_CNFG_SUCCESS;
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


short CSAWrapper::DP_Enum_SpecAn_ConfigExtnlTrigger(U8BIT in_u8InstNo)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"TRIG:SEQ:SOUR EXT\r\n");
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_TRIGLVL_FAILURE;
        }
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"TRIG:SEQ:HOLD:TIME -0.00003\r\n");
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_TRIGLVL_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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


short CSAWrapper::DP_Enum_SpecAn_ConfigVideoTrigger (U8BIT in_u8InstNo, double in_dVideoTrigrLevel, signed long in_slVideoTrigrSlope, float in_fTrigDelay)
{

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SN9030B) //keysight
    {

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:SEQ:SOUR VID\n");
        if (m_viStatus)
        {
           return DP_SPECAN_CNFG_TRIGSLOPE_FAILURE;
        }
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:SEQ:VID:LEV %lf\n",in_dVideoTrigrLevel);
        if (m_viStatus)
        {
           return DP_SPECAN_CNFG_TRIGSLOPE_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"TRIG:SEQ:VID:DEL -0.00001\r\n");
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_TRIGLVL_FAILURE;
        }

    }
    else if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:SEQ:SOUR VID\n");
        if (m_viStatus)
        {
           return DP_SPECAN_CNFG_TRIGSLOPE_FAILURE;
        }
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"SENS:DEM:SQU:LEV %lf\n",in_dVideoTrigrLevel);
        if (m_viStatus)
        {
           return DP_SPECAN_CNFG_TRIGSLOPE_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"TRIG:SEQ:HOLD:TIME -0.00001\r\n");
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_TRIGLVL_FAILURE;
        }

    }

 return DP_SPEC_AN_CNFG_SUCCESS;

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

short CSAWrapper::DP_Enum_SpecAn_ConfigExtnlMixer (U8BIT in_u8InstNo, signed long in_slHarmonic, double in_dAvgConverLoss)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"MIX:HARM %lf\n", in_slHarmonic);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_MIXHARM_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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


short CSAWrapper::DP_Enum_SpecAn_ConfigExtnlMixerBiasEnabled(U8BIT in_u8InstNo, bool in_bBiasEnabled)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"MIX:BIAS:STAT %d\n", in_bBiasEnabled);
        if (m_viStatus)
        {
            return DP_SPECAN_CNFG_MIXBIAS_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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

short CSAWrapper::DP_Enum_SpecAn_ReadActiveMarker(U8BIT in_u8InstNo, double in_dMarkerPos, double *out_dMarkerAmp)
{
    char cpReadValue[64] = {0};
    double dMarAmp = 0.0f;
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
         if(!in_dMarkerPos)
         {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:STAT ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_READ_ACTIVEMARKER_FAILURE;
            }
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:MAX\n");
            if(m_viStatus)
            {
                return DP_SPECAN_READ_ACTIVEMARKER_FAILURE;
            }
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:Y? \n","%s",cpReadValue);
            dMarAmp= strtod(cpReadValue,NULL);
         }
         else
         {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK:STAT ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_READ_ACTIVEMARKER_FAILURE;
            }
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:X %lf\n", in_dMarkerPos);
            if(m_viStatus)
            {
                return DP_SPECAN_RD_ACTMAR_FAILURE;
            }
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK1:Y? \n","%s",cpReadValue);
            dMarAmp= strtod(cpReadValue,NULL);
         }
    }
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
    {
        if(!in_dMarkerPos)
        {
           m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:STAT ON\n");
           if(m_viStatus)
           {
               return DP_SPECAN_READ_ACTIVEMARKER_FAILURE;
           }
           m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:MAX\n");
           if(m_viStatus)
           {
               return DP_SPECAN_READ_ACTIVEMARKER_FAILURE;
           }
           viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:Y? \n","%s",cpReadValue);
           dMarAmp= strtod(cpReadValue,NULL);
        }
        else
        { 
           m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:MODE POS\n");
           if(m_viStatus)
           {
               return DP_SPECAN_READ_ACTIVEMARKER_FAILURE;
           }
           m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:X %lf\n", in_dMarkerPos);
           if(m_viStatus)
           {
               return DP_SPECAN_RD_ACTMAR_FAILURE;
           }
           viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK1:Y? \n","%s",cpReadValue);
           dMarAmp= strtod(cpReadValue,NULL);
        }
    }
    *out_dMarkerAmp = dMarAmp;
    return DP_SPEC_AN_CNFG_SUCCESS;
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

short CSAWrapper:: DP_Enum_SpecAn_WaitForOperation_Complete(U8BIT in_u8InstNo, U32BIT in_u32TimeOutinMS)
{
    int iOpc = 0;
    unsigned int uiReadTimeout = 0;

    U32BIT u32TOCnt = 0;

    // while(u32TOCnt < in_u32TimeOutinMS)
    {
        viGetAttribute(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, VI_ATTR_TMO_VALUE, (void *)&uiReadTimeout);

        viSetAttribute(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, VI_ATTR_TMO_VALUE, in_u32TimeOutinMS);

        m_viStatus = viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOpc);
        if(m_viStatus)
        {
            viSetAttribute(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, VI_ATTR_TMO_VALUE, uiReadTimeout);
            return m_viStatus;
        }

    }

    viSetAttribute(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, VI_ATTR_TMO_VALUE, uiReadTimeout);

    return DP_SPEC_AN_CNFG_SUCCESS;
}

/*******************************************PhaseNoise Measurement********************************************************/
short CSAWrapper:: DP_Enum_SpecAn_ConfigPNMeasurement(U8BIT in_u8InstNo,double in_dCenterFrequency, double in_dStartOffset, double in_dStopOffset,  double in_dRefLvl, double in_dSpan)
{
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D)
    {
        /*To Set Center Frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"FREQUENCY:CENTER %lf\n", in_dCenterFrequency);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQSPAN_FAILURE;
        }

        /*To set Refrence Level*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":DISP:LPL:VIEW:WIND:TRAC:Y:SCAL:RLEV %lf\n", in_dRefLvl);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_REFLVL_FAILURE;
        }

        /*To set Start offet frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:LPL:FREQ:OFFS:STAR %lf\n", in_dStartOffset);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_REFLVL_FAILURE;
        }

        /*To set Stop offet frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:LPL:FREQ:OFFS:STOP %lf\n", in_dStopOffset);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_REFLVL_FAILURE;
        }
    }
    else if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D)
    {
        /*To Set Center Frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"FREQUENCY:CENTER %lf\n", in_dCenterFrequency);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQSPAN_FAILURE;
        }

        /*To Set Span*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":FREQ:SPAN  %lf\n", in_dSpan);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQSPAN_FAILURE;
        }

        /*To set Refrence frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:FIX:RPO:X %lf\n", in_dStartOffset);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQ_FAILURE;
        }

        /*To set Refrence Level*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:FIX:RPO:Y %lf\n", in_dRefLvl);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_REFLVL_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:PEXC 6DB\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_PEAK_EXC_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:STAT ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MARAMP_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:TRAC 1\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_TRACE_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:MAX\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MARAMP_FAILURE;
        }

        /*switch on Phase noise Measurement*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:PNO ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MODE_FAILURE;
        }

        /*Automatic Peak Search ON*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:PNO:AUTO ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_PEAK_SRCH_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}
#if 0
short CSAWrapper:: DP_Enum_SpecAn_ConfigPNMeasurement(U8BIT in_u8InstNo,double in_dCenterFrequency, double in_dRefFreq,  double in_dRefLvl, double in_dSpan)
{
   if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
   {
       /*To Set Center Frequency*/
       m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"FREQUENCY:CENTER %lf\n", in_dCenterFrequency);
       if(m_viStatus)
       {
           return DP_SPECAN_CNFG_FREQSPAN_FAILURE;
       }

        /*To Set Span*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"FREQUENCY:SPAN %lf\n", in_dSpan);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQSPAN_FAILURE;
        }

        /*To set Refrence frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:FIX:RPO:X %lf\n", in_dRefFreq);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_FREQ_FAILURE;
        }

        /*To set Refrence Level*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:FIX:RPO:Y %lf\n", in_dRefLvl);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_REFLVL_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:PEXC 6DB\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_PEAK_EXC_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:STAT ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MARAMP_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:TRAC 1\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_TRACE_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:MAX\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MARAMP_FAILURE;
        }

        /*switch on Phase noise Measurement*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:PNO ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MODE_FAILURE;
        }

        /*Automatic Peak Search ON*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:PNO:AUTO ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_PEAK_SRCH_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}
/***************************************************************************************************/
#endif
/*******************************************Read Phase Noise********************************************************/

short CSAWrapper:: DP_Enum_SpecAn_ReadPN(U8BIT in_u8InstNo, double in_dMarFreq, double *out_dPower)
{
    double dPower= 0.0;

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:STAT ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MARAMP_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:TRAC 1\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_TRACE_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:MAX\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MARAMP_FAILURE;
        }

        /*switch on Phase noise Measurement*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:PNO ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MODE_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:X %lf\n", in_dMarFreq);
        if(m_viStatus)
        {
            return DP_SPECAN_READ_ACTIVEMARKER_FAILURE;
        }

        m_viStatus = viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:DELT:FUNC:PNO:RES?\n", "%lf", &dPower);
        if(m_viStatus)
        {
            return DP_SPECAN_READ_PHASENOISE_FAILURE;
        }
    }
    *out_dPower = dPower;

    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper:: DP_Enum_SpecAn_ModeConfig(U8BIT in_u8InstNo, unsigned short in_usModeNo)
{
    char StrFormat[64]= "";
    iInstNum = 219;
    int iOper  = 0;
    unsigned char ucOPCCnt = 0;

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SN9030B)
    {
        if(in_usModeNo == 219)
        {
            sprintf(StrFormat,(char*)":INST:CONF:NFIG:NFIG\n");
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,StrFormat);
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_MODE_FAILURE;
            }
            if(in_u8InstNo != 0)
            {
                while(!iOper)
                {
                    viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"*OPC?\n", "%d", &iOper);
                    if(ucOPCCnt > 100)
                        break;
                    ucOPCCnt++;
                }
            }
            if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SN9030B) //kEYsIGHT
            {
                if(in_usModeNo == NOISE_FIGURE_MODE)
                {

                    /*To display the Noise source Layout in meter format*/
                    viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":DISP:NFIG:FORM TABLe\n");

                    while(!iOper)
                    {
                        viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

                        if(ucOPCCnt > 100)
                            break;
                        ucOPCCnt++;
                    }
                }
            }
        }
       else if(in_usModeNo == 1)
       {
            sprintf(StrFormat,(char*)":INST:CONF:SA:SAN\n");
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,StrFormat);
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_MODE_FAILURE;
            }
            if(in_u8InstNo != 0)
            {
                while(!iOper)
                {
                    viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"*OPC?\n", "%d", &iOper);
                    if(ucOPCCnt > 100)
                        break;
                    ucOPCCnt++;
                }
            }
       }
    }
    else if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        if(in_usModeNo == 1)
        {
            sprintf(StrFormat,(char*)":INST:CRE:NEW SANALYZER, 'Spectrum'\n");
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,StrFormat);
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_MODE_FAILURE;
            }
        }

        sprintf(StrFormat,(char*)":INST:CRE:NEW NOIS, 'NOISE'\n");
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,StrFormat);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MODE_FAILURE;
        }
        Sleep(1000);
        if(iInstNum == PHASE_NOISE_MODE)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:LPLot\n");

            if(m_viStatus)
                return DP_SPECAN_CNFG_MODE_FAILURE;

            if(in_u8InstNo != 0)
            {
                while(!iOper)
                {
                    viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"*OPC?\n", "%d", &iOper);
                }
            }
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":TRAC:LPLot:TYPE RAW\n");
            if(m_viStatus)
                return DP_SPECAN_CNFG_MODE_FAILURE;
        }
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
        {
               if(iInstNum == NOISE_FIGURE_MODE)
               {
                  /*To display the Noise source Layout in meter format*/
                  viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"DISP:TABL ON\n");

                  while(!iOper)
                  {
                      viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);
                      if(ucOPCCnt > 100)
                          break;
                      ucOPCCnt++;
                  }

                  viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":DISP:WIND4:SEL\n");

                  while(!iOper)
                  {
                      viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);
                      if(ucOPCCnt > 100)
                          break;
                      ucOPCCnt++;
                  }

                  viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":DISP:WIND4:SIZE LARG\n");

                  while(!iOper)
                  {
                      viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);
                      if(ucOPCCnt > 100)
                          break;
                      ucOPCCnt++;
                  }
                }
         }
         Sleep(1000);
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
        {
                if(in_usModeNo == NOISE_FIGURE_MODE)
                {

                    /*To display the Noise source Layout in meter format*/
                    viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":DISP:NFIG:FORM TABLe\n");

                    while(!iOper)
                    {
                       viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

                       if(ucOPCCnt > 100)
                           break;
                       ucOPCCnt++;
                    }
                }
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper:: DP_Enum_SpecAn_ReadNF(U8BIT in_u8InstNo, double *out_dNoiseVal,  double *out_dGainValue, double *out_dNoiseTemp)
{
    QString csFormat = "";
    QString strInformation = "",strData[5];
    char cpReadValue[10000] = {0};
    static ViChar *cTok;
    unsigned short usIndex   = 0;
    int iOper  = 0;
    unsigned char ucOPCCnt = 0;

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        /*To read the Gain value*/

        DP_Enum_SpecAn_SingleRun(in_u8InstNo, 1);

        DP_Enum_SpecAn_WaitForOperation_Complete(in_u8InstNo, 20000);

       /*To read the Gain value*/
       viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "TRAC? TRACE1,Gain\n", "%s", cpReadValue);

       cTok = strtok(cpReadValue,",");
       while(cTok != NULL)
       {
           out_dGainValue[usIndex] = atof(cTok);

           if(out_dGainValue[usIndex] > 100.00 || out_dGainValue[usIndex] < -100.0)
           {
               out_dGainValue[usIndex] = 0.0;
           }

           cTok = strtok(NULL,",");
           usIndex++;
       }

       /*To read the Noise figure value*/
       viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRAC? TRACE1,Noise\n", "%s", cpReadValue);

       usIndex = 0;
       cTok = strtok(cpReadValue,",");
       while(cTok != NULL)
       {
          out_dNoiseVal[usIndex] = atof(cTok);
          if(out_dNoiseVal[usIndex] > 100.00 || out_dNoiseVal[usIndex] < -100.0)
          {
              out_dNoiseVal[usIndex] = 0.0;
          }
          cTok = strtok(NULL,",");
          usIndex++;
        }

   }

   if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
   {
        out_dNoiseTemp = NULL;
        /*To apply the calibrated value*/
        viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENSE:NFIG:CAL:STATE ON\n");

        /*To set single measurement*/
        viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":INIT:CONT 1\n");

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 300)
                break;

            ucOPCCnt++;
        }

        Sleep(2000);

        /*To set single measurement*/
        viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":INIT:CONT OFF\n");

        Sleep(500);
        /*To read the Gain value*/
        viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, ":FETCH:NFIG:CORR:NFIG?\n", "%s", cpReadValue);

        cTok = strtok(cpReadValue,",");
        while(cTok != NULL)
        {
            out_dNoiseVal[usIndex] = atof(cTok);
            cTok = strtok(NULL,",");
            usIndex++;
        }
        Sleep(500);
        usIndex = 0;
        viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, ":FETCH:NFIG:CORR:GAIN?\n", "%s", cpReadValue);

        cTok = strtok(cpReadValue,",");
        while(cTok != NULL)
        {
            out_dGainValue[usIndex] = atof(cTok);
            cTok = strtok(NULL,",");
            usIndex++;
        }
   }

    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper ::  DP_Enum_SpecAn_ConfigureHarmonicDistortionMeasurement(U8BIT in_u8InstNo, unsigned char in_ucHarCnt, double dRefLevel, double dFundFreq, double in_dRBW, unsigned char in_ucAtten)
{
    int iOpc = 0;
    QString strTemp = "";

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
       m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:HARM:NUMB %d\n", in_ucHarCnt);
        if(m_viStatus)
        {
            return DP_SPECAN_HARDIS_MEAS_FAILURE;
        }

       m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:HARM\n");
        if(m_viStatus)
        {
            return DP_SPECAN_HARDIS_MEAS_FAILURE;
        }

       if(in_u8InstNo != 0)
        {
            while(!iOpc)
            {
                m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOpc);
                if(m_viStatus)
                {
                    break;
                }
            }
        }

       if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
       {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:FUNC:HARM:NHARM %d\n", in_ucHarCnt);
            if(m_viStatus)
            {
                return DP_SPECAN_HARCNT_FAILURE;
            }

            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:FUNC:HARM ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_HARDIS_MEAS_FAILURE;
            }
       }

       if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
       {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:HARM:AVER:COUN %d\n", in_ucHarCnt);
            if(m_viStatus)
            {
                return DP_SPECAN_HARCNT_FAILURE;
            }
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:HARM:SWE:STAT ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_HARDIS_MEAS_FAILURE;
            }
       }

       m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"DISP:HARM:VIEW:WIND:TRAC:Y:RLEV %f\n", dRefLevel);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_REFLVL_FAILURE;
        }

       m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"HARM:BWID %f\n", in_dRBW);
        if(m_viStatus)
        {
            return DP_SPECAN_DRBW_FAILURE;
        }

       m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"HARM:FREQ:FUND %f\n", dFundFreq);
        if(m_viStatus)
        {
            return DP_SPECAN_FUNFREQ_FAILURE;
        }
     }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper :: DP_Enum_SpecAn_HarmonicMeasurement(U8BIT in_u8InstNo)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:FUNC:HARM ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_HARDIS_MEAS_FAILURE;
        }
    }
}

/**********************************************HARMONIC MEASUREMENT*****************************************************/

short CSAWrapper ::  DP_Enum_SpecAn_ReadHarmonicMeasurement(U8BIT in_u8InstNo, unsigned char in_ucHarCnt, double *dFundFreq,  double *dPower)
{
    unsigned short usIndex = 0;
    char carrValue[1024] = {0};
    unsigned char ucarrReadData[1024] = {0};
    unsigned char ucarrWriteData[256] = {0};
    ViUInt32 u32ReadCnt = 0;
    ViUInt32 u32WriteCnt = 0;
    static ViChar *cTok;
    double dHarmonicValues[3] = {0.0, 0.0, 0.0};

    char szHarList[] = {"CALC:MARK:FUNC:HARM:LIST?\n"};
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"INIT:CONT OFF\n");
        if(m_viStatus)
        {
            return DP_SPECAN_HARDIS_MEAS_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:FUNC:HARM:NHARM %d\n", in_ucHarCnt);
        if(m_viStatus)
        {
            return DP_SPECAN_HARCNT_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CALC:MARK:FUNC:HARM ON\n");
        if(m_viStatus)
        {
            return DP_SPECAN_HARDIS_MEAS_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"INIT;*WAI\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_REFLVL_FAILURE;
        }

        memcpy(ucarrWriteData, szHarList, sizeof(szHarList) );

        m_viStatus = viWrite(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, ucarrWriteData, sizeof(szHarList)-1, &u32WriteCnt);
        if(m_viStatus)
        {
            return DP_SPECAN_DET_HAR_FAILURE;
        }

        m_viStatus = viRead(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, ucarrReadData, sizeof(ucarrReadData), &u32ReadCnt);
        if(m_viStatus)
        {
            return DP_SPECAN_DET_HAR_FAILURE;
        }

        memcpy(carrValue, ucarrReadData,  u32ReadCnt);

        printf("%s", carrValue);

        cTok = strtok(carrValue, ",");
        while(cTok != NULL)
        {
            dHarmonicValues[usIndex] = atof(cTok);
            cTok = strtok(NULL,",");
            usIndex++;
            if(usIndex == in_ucHarCnt)
            {
                break;
            }
        }

        *dPower = dHarmonicValues[0];
        *dFundFreq = dHarmonicValues[1];
     }
    return DP_SPEC_AN_CNFG_SUCCESS;
}
/***************************************************************************************************/


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
short CSAWrapper ::  DP_Enum_SpecAn_ReadHarmonic(U8BIT in_u8InstNo,
                                                    unsigned char in_ucmaximumTime_ms,
                                                    signed long harmonicDistArraySize,
                                                    double *in_dharmonicDistTotalQ,
                                                    double *out_Ampl1,
                                                    double *out_Ampl2,
                                                    signed long *in_plharmonicDistValuesRead)
{


    double harmonicDistFrequency[10] = {0.0};
    char StrValue[1024] = {0};
    static ViChar *cTok;
    unsigned short usIndex = 0;


    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        viSetAttribute(in_u8InstNo,VI_ATTR_TMO_VALUE,6000);

        viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":READ:HARM:AMPL:ALL?\n","%s",StrValue);

        viSetAttribute(in_u8InstNo,VI_ATTR_TMO_VALUE,3000);

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
    }
    return 0;

}

short CSAWrapper ::  DP_Enum_SpecAn_ReadHarmonicValue(U8BIT in_u8InstNo, double* pdHarFreq, double* pdHarAmp)
{

    char StrValue[1024] = "",strFreq[1024] = "";
    static ViChar *cTok,*cAmpTok;
    unsigned short usIndex = 0;

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        viSetAttribute(in_u8InstNo,VI_ATTR_TMO_VALUE,6000);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":READ:HARM:AMPL:ALL?\n","%s",StrValue);
        if(m_viStatus)
        {
            return DP_SPECAN_READ_HARVAL_FAILURE;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":READ:HARM:FREQ:ALL?\n","%s",strFreq);
        if(m_viStatus)
        {
            return DP_SPECAN_READ_HARFREQ_FAILURE;
        }

        viSetAttribute(in_u8InstNo,VI_ATTR_TMO_VALUE,3000);

        //	Sleep(100);

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
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper ::  DP_Enum_SpecAn_ConfigureSpurious(U8BIT in_u8InstNo, unsigned char in_ucRegNo)
{
    //  unsigned char in_ucRegNo;
    int iOpc= 0;    
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"*RCL %d\n",in_ucRegNo);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_SPUR_FAILURE;
        }

        if(in_u8InstNo != 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:SPUR\n");
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_SPUR_FAILURE;
            }
            while(!iOpc)
            {
                m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"*OPC?\n", "%d", &iOpc);
                if(m_viStatus)
                {
                    break;
                }
            }
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper ::    DP_Enum_SpecAn_ReadSpuriousMeas(U8BIT in_u8InstNo,unsigned short in_usTotSamp,
                                                        double* pFreq,double* pAmp, unsigned short* usCount)
{

    double SuprDistAmplitude[64] ={0.0};
    double SuprDistFrequency[64] = {0.0},dSwapped = 0;
    double dReadvalue[1000]= {0.0};
    char StrValue[2000] = {0};
    static ViChar *cTok = NULL;
    unsigned short usIndex = 0;
    int iTotCnt = 0;
    int iTempLoop = 0;
    unsigned char ucSamCnt = 0;

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        viSetAttribute(in_u8InstNo,VI_ATTR_TMO_VALUE,6000);
        viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":READ:SPUR?\n","%s",StrValue);
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
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper::DP_Enum_SpecAn_Save_Image(U8BIT in_u8InstNo, unsigned short in_SpotNo)
{
    char StrFormat[64]= {0};

    sprintf(StrFormat,":MMEM:STORE:SCR ’C:PS_Spot%d.GIF'",in_SpotNo+1);
    viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,StrFormat);
    return DP_SPEC_AN_CNFG_SUCCESS;
}


short CSAWrapper::DP_Enum_SpecAn_Reset(U8BIT in_u8InstNo)
{
    m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"*RST\n");

    if(m_viStatus)
    {
        return DP_SPECAN_RESET_FAILURE;
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
}


/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_Initiate()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:  Configure Initiate of the CSAWrapper Analyser
******************************************************************************************************/


short CSAWrapper::DP_Enum_SpecAn_Initiate (U8BIT in_u8InstNo)
{

    m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"INITiate:IMMediate\n");
    if(m_viStatus)
    {
        return DP_SPECAN_INIT_FAILURE;
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_Abort()
// Author					:	 Venkatesh.p
// Global Variables affected:	 NA
// Created Date				:    06-07-09
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configure  Abort of the CSAWrapper Analyser
******************************************************************************************************/

short CSAWrapper::DP_Enum_SpecAn_Abort (U8BIT in_u8InstNo)
{
    unsigned char in_ucRegNo = 0;
    m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"ABOR\n",in_ucRegNo);
    if(m_viStatus)
    {
        return DP_SPECAN_ABORT_FAILURE;
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper::Calibration_With_NoiseSoure (U8BIT in_u8InstNo, U32BIT in_u32TimeOutinMS)
{
    unsigned short usOPCCnt = 0;

    int iOper = 0;

    /*Calibration*/
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CORR:STAT ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_NS_CALIB_FAILURE;
            }

            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:CORR\n");
            if(m_viStatus)
            {
                return DP_SPECAN_NS_CALIB_FAILURE;
            }

            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"INIT:IMM;*WAI\n");
            if(m_viStatus)
            {
                return DP_SPECAN_NS_CALIB_FAILURE;
            }

            DP_Enum_SpecAn_SingleRun(in_u8InstNo, 1);

            Sleep(in_u32TimeOutinMS);
        }
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:CAL:INIT\n");
            if(m_viStatus)
            {
                return DP_SPECAN_NS_CALIB_FAILURE;
            }
        }

        if(in_u8InstNo != 0)
        {
            while(!iOper)
            {
                viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

                if(usOPCCnt > 300)
                    break;

                usOPCCnt++;
                //Sleep(10);
            }
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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

short CSAWrapper:: Read_PhaseNoise(U8BIT in_u8InstNo, unsigned short in_TraceIndex, double in_dMarkerPos,  double *out_dMarkerAmp)
{
    char csFormat[50]={0};
    int iOpc =0;
    int iTempRetCount = 0;
    float fOutPhaseNoise = 0.0f;

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"DISP:WIND:TRAC:Y:RLEV -20\n");
        if(m_viStatus)
        {
            return DP_SPECAN_PHASE_NS_FAILURE;
        }

        /*To perform auto tune*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":FREQ:CARRier:SEARch\n");
        if(m_viStatus)
        {
            return DP_SPECAN_PHASE_NSAT_FAILURE;
        }

        iTempRetCount = 0;

        while(!iOpc)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOpc);
            if(m_viStatus)
            {
                return DP_SPECAN_PHASE_NS_FAILURE;
            }

            if(iTempRetCount >= 100)
                break;
            iTempRetCount++;
        }

        iTempRetCount = 0;

        /* Sets the marker mode as normal*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:LPLot:MARKer1:MODE POS\n");
        if(m_viStatus)
        {
            return DP_SPECAN_PHASENOISE_MM_FAILURE;
        }

        /*to set the marker frequency*/
        sprintf(csFormat, ":CALC:LPLot:MARKer1:x %f\n",in_dMarkerPos);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,csFormat);
        if(m_viStatus)
        {
            return DP_SPECAN_PHASE_NS_FAILURE;
        }

        /*To read the phase noise value*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":CALC:LPLOT:MARKER1:Y?\n", "%f", &fOutPhaseNoise);
        if(m_viStatus)
        {
            return DP_SPECAN_PHASE_NS_FAILURE;
        }
    }
    *out_dMarkerAmp = fOutPhaseNoise;
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper::ConfigAutoSweep (U8BIT in_u8InstNo)
{
    unsigned char ucOPCCnt = 0;

    int iOper = 0;

    /*Sweep Mode*/
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"INIT:CONT OFF\n");
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_AUTO_SWEEP_FAILURE;
            }

            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"CONF:LIST:SING\n");
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_AUTO_SWEEP_FAILURE;
            }
        }
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SWE:TIME:AUTO ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_AUTO_SWEEP_FAILURE;
            }
        }

        if(in_u8InstNo != 0)
        {
            while(!iOper)
            {
                viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }
        }
   }
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper::ConfigMaxMarker (U8BIT in_u8InstNo, double* out_dAmplitude)
{
    unsigned char ucOPCCnt = 0;
    int iOper = 0;
    double dAmplitude = 0.0f;

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK1:MAX\n");
        if(m_viStatus)
        {
            return DP_SPECAN_READ_MARKER_FAILURE;
        }

        if(in_u8InstNo != 0)
        {
            while(!iOper)
            {
                viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK1:Y?\n","%f",&dAmplitude);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_MARAMP_FAILURE;
        }

        if(in_u8InstNo != 0)
        {
            while(!iOper)
            {
                viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }
        }
    }
    *out_dAmplitude = dAmplitude;

    return DP_SPEC_AN_CNFG_SUCCESS;
}
short CSAWrapper::DP_Enum_SpecAn_Close(U8BIT in_u8InstNo)
{
    ViSession rm = 0;
    m_viStatus = viClose(in_u8InstNo);
    //m_viStatus = viClose(in_ucSpecNo);
    if(m_viStatus)
    {
        return DP_SPECAN_CLS_FAILURE;
    }

    viClose(rm);
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper::AgSA_ConfigDUTSetup(U8BIT in_u8InstNo, float in_fLOFreq, float in_fIFFreq, unsigned char ucDUTModeSel)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[50] ={0};

    /* to set LO Fixed frequency*/

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
        {
            if(ucDUTModeSel == 2)
            {
                sprintf(szCmdStr,"SENS:CONF:MODE:DOWN:SYST:LOSC:FREQ %f\n", in_fLOFreq);
            }
            else
            {
                sprintf(szCmdStr,"SENS:CONF:MODE:UPC:SYST:LOSC:FREQ %f\n",in_fLOFreq);
            }
        }
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
        {
            if(ucDUTModeSel == 2)
            {
                sprintf(szCmdStr,":NFIG:MODE:DOWN:LOSC:FREQ %f\n", in_fLOFreq);
            }
            else
            {
                sprintf(szCmdStr,":NFIG:MODE:Upconv:LOSC:FREQ %f\n",in_fLOFreq);
            }
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }
    //	Sleep(100);

    return DP_SPEC_AN_CNFG_SUCCESS;
}


/***************************************************************************************************
// Name						:	 AgSA_DUTSetupConfig
// Author					:
// Global Variables affected:	 NA
// Created Date				:    10-june-2013
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures Noise Figure Mode DUT Setup
******************************************************************************************************/
short CSAWrapper::AgSA_DUTSetupConfig(U8BIT in_u8InstNo, float in_fLOFreq, float in_fIFFreq,unsigned char ucDUTModeSel)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[50] = {0};

    /* to set LO Fixed frequency*/
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
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

        m_viStatus   = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
     }
    //	Sleep(100);

    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper::AgSA_ConfigDUTSetup(U8BIT in_u8InstNo, float in_fLOFreq, float in_fIFFreq)
{
    short m_viStatus	  = 0;
    unsigned char ucOPCCnt	  = 0;
    int iOper		  = 0;
    char szCmdStr[50] = {0};

    /* to set LO Fixed frequency*/
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        sprintf(szCmdStr,":NFIG:MODE:DOWN:LOSC:FREQ %f\n", (in_fLOFreq - in_fIFFreq));
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);

        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

    #if 0
        /* to set IF Fixed frequency*/
        sprintf(szCmdStr,":NFIG:MODE:DOWN:IF:FREQ %f\n", in_fIFFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    #endif
   }
    //	Sleep(100);

   return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper:: AgSA_ConfigMaxHold(U8BIT in_u8InstNo)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    /* to set the Trace  in		*/
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRAC1:TYPE MAXH\n");

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper:: AgSA_ReadPeakValue(U8BIT in_u8InstNo, float* out_pfPeakValue)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    long double ldFreq = 0;

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":INIT:CONT OFF\n");

        //	Sleep(1000);


        /* to set the Marker in Peak*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:MAX\n");

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;
            ucOPCCnt++;
        }

        iOper		= 0;
        ucOPCCnt	= 0;
        *out_pfPeakValue = 0.0;
        /*Read the marker value*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:Y?\n", "%f", out_pfPeakValue);

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;
            ucOPCCnt++;
        }


        iOper		= 0;
        ucOPCCnt	= 0;


        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:X?\n", "%ld", &ldFreq);

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;
            ucOPCCnt++;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)":INIT:CONT ON\n");
    }
  return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper:: DP_Enum_SpecAn_ReadPeakValue(U8BIT in_u8InstNo, double *out_pdFreq, double* out_pfPeakValue)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    double dPeakFreq = 0.0;
    double dPeakPwr = 0.0;


    /* to set the Marker in Peak*/
    m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:MAX:PEAK\n");

    while(!iOper)
    {
        viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }

    iOper		= 0;
    ucOPCCnt	= 0;
    *out_pfPeakValue = 0.0;
    /*Read the marker value*/
    m_viStatus = viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK:Y?\n", "%lf", &dPeakPwr);


    *out_pfPeakValue = dPeakPwr;
    while(!iOper)
    {
        viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;
        ucOPCCnt++;
    }


    iOper		= 0;
    ucOPCCnt	= 0;


    m_viStatus = viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK:X?\n", "%lf", &dPeakFreq);

//    while(!iOper)
//    {
//        viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

//        if(ucOPCCnt > 100)
//            break;
//        ucOPCCnt++;
//    }

    *out_pdFreq = dPeakFreq;

    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper:: AgSA_ReadNextPeakValue(U8BIT in_u8InstNo, float* out_pfPeakValue, double* out_pdFreq)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    /* to set the Marker in Peak*/
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:MAX:NEXT\n");
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;
            ucOPCCnt++;
        }

        iOper		= 0;
        ucOPCCnt	= 0;
        *out_pfPeakValue = 0.0;

        /*Read the marker value*/
        m_viStatus = viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:Y?\n", "%f", out_pfPeakValue);

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;
            ucOPCCnt++;
        }

        iOper		= 0;
        ucOPCCnt	= 0;
        *out_pdFreq = 0.0;

        m_viStatus = viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"CALC:MARK1:X?\n", "%lf", out_pdFreq);
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;
            ucOPCCnt++;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper:: AgSA_SetRFBTrig(U8BIT in_u8InstNo, unsigned char ucTrigType)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    /* to set the Marker in Peak*/
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        if(ucTrigType == 1) // RF Burst
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:SOUR RFB\n");
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:RFB:LEV:ABS -80\n");
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:RFB:SLOP POS\n");
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:RFB:DEL 0.0\n");
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:RFB:LEV:TYPE REL \n");
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:RFB:LEV:REL 0.00\n");
        }
        else if(ucTrigType == 2) //Video Trigger
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:SOUR VID\n");
        }

        else if(ucTrigType == 3) //FRee Run
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"TRIG:SOUR IMM\n");
        }

        //	viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":SENS:FREQ:TUNE:IMM\n");//added by Senthil vason for Auto tune 16/4/2013
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"INIT:CONT ON\n");
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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

short CSAWrapper::DP_Enum_SpecAn_PeakThresholdOnOff(U8BIT in_u8InstNo, unsigned char in_ucOnOff)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        if(in_ucOnOff)
        {
            /* Sets the marker mode as normal*/
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK:PEAK:THR:STAT ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_PEAK_ON_FAILURE;
            }
        }
        else
        {
            /* Sets the marker mode as normal*/
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK:PEAK:THR:STAT OFF\n");
            if(m_viStatus)
            {
                return DP_SPECAN_PEAK_OFF_FAILURE;
            }
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;

}

short CSAWrapper::DP_Enum_Mode_Preset(U8BIT in_u8InstNo)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SYST:PRES\n");
        if(m_viStatus)
        {
            return DP_SPECAN_PRESET_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper::DP_Enum_Set_Scale(U8BIT in_u8InstNo,double in_dScale)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"DISP:WIND:TRAC:Y:PDIV %f\n", in_dScale);
        if(m_viStatus)
        {
            return DP_SPECAN_SETSCALE_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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

short CSAWrapper::DP_Enum_SpecAn_Config_AutoAlign(U8BIT in_u8InstNo, unsigned char in_ucOnOff)
{
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        if(in_ucOnOff)
        {
            /* Sets the marker mode as normal*/
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CAL:AUTO ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_AUTO_ON_FAILURE;
            }
        }
        else
        {
            /* Sets the marker mode as normal*/
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CAL:AUTO OFF\n");
            if(m_viStatus)
            {
                return DP_SPECAN_CNFG_AUTO_OFF_FAILURE;
            }
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper::AgSA_ConfigAmpDUTSetup(U8BIT in_u8InstNo, double in_fStartFreq, double in_fStopFreq, int in_iNoOfPoints)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    char szCmdStr[100] ={0};

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:MODE:DUT AMPL\n");
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the frequency as Sweep frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:FREQ:MODE SWEP\n");
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, "FREQ:STAR %f\n", in_fStartFreq);
        /* to set the start frequency */
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the stop frequency */

        sprintf(szCmdStr, "FREQ:STOP %f\n", in_fStopFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, "SWE:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }
  if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
  {

      m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":INST:CONF:NFIG:NFIG\n");
      if(m_viStatus)
      {
          return DP_SPECAN_CNFG_DUT_SETUP_FAILURE;
      }

      while(!iOper)
      {
         viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

         if(ucOPCCnt > 100)
             break;
         ucOPCCnt++;
      }

      /*To display the Noise source Layout in meter format*/
      viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":DISP:NFIG:FORM TABLe\n");

      while(!iOper)
      {
         viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

         if(ucOPCCnt > 100)
             break;
         ucOPCCnt++;
      }

      m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT AMPL\n");
      if(m_viStatus)
      {
          return DP_SPECAN_CNFG_DUT_SETUP_FAILURE;
      }

      ucOPCCnt = 0;
      iOper	 = 0;

      while(!iOper)
      {
          viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

          if(ucOPCCnt > 100)
              break;

          ucOPCCnt++;
      }

      /* to set the frequency as Sweep frequency*/
      m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:FREQ:MODE SWEP\n");
      ucOPCCnt = 0;
      iOper	 = 0;
      while(!iOper)
      {
          viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

          if(ucOPCCnt > 100)
              break;

          ucOPCCnt++;
      }

      sprintf(szCmdStr, ":NFIG:FREQ:STAR %f\n", in_fStartFreq);
      /* to set the start frequency */
      m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
      ucOPCCnt = 0;
      iOper	 = 0;
      while(!iOper)
      {
          viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

          if(ucOPCCnt > 100)
              break;

          ucOPCCnt++;
      }

      /* to set the stop frequency */

      sprintf(szCmdStr, ":NFIG:FREQ:STOP %f\n", in_fStopFreq);

      m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
      ucOPCCnt = 0;
      iOper	 = 0;
      while(!iOper)
      {
          viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

          if(ucOPCCnt > 100)
              break;

          ucOPCCnt++;
      }

      sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

      /* to set the Sweep Points*/
      m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
      ucOPCCnt = 0;
      iOper	 = 0;
      while(!iOper)
      {
          viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

          if(ucOPCCnt > 100)
              break;

          ucOPCCnt++;
      }
  }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

/***************************************************************************************************
// Name						:	 AgSA_ConfigAmpDUTSetup
// Author					:
// Global Variables affected:	 NA
// Created Date				:    20-11-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	 Configures Noise Figure Mode DUT Setup
******************************************************************************************************/
short CSAWrapper::AgSA_AmpDUTSetupConfig(U8BIT in_u8InstNo, double in_fStartFreq, double in_fStopFreq, double in_dStepFreq, int in_iNoOfPoints, unsigned char in_ucAvgSamples)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    char szCmdStr[256] ={0};

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:DUT AMPL\n");
        if(m_viStatus)
        {
            return DP_SPECAN_AMP_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the start frequency */

        sprintf(szCmdStr, ":SENS:FREQ:STAR %lf\n", in_fStartFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the stop frequency */

        sprintf(szCmdStr, (char*)":SENS:FREQ:STOP %f\n", in_fStopFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":SENS:FREQ:STEP %lf\n", in_dStepFreq);

        /* to set the Step Frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":SENS:FREQ:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":SENS:SWE:COUN %d \n", in_ucAvgSamples);

        /* to set the average samples*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":INST:CONF:NFIG:NFIG\n");
        if(m_viStatus)
        {
            return DP_SPECAN_AMP_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":DISP:NFIG:FORM TABLe\n");
        if(m_viStatus)
        {
            return DP_SPECAN_AMP_DUT_SETUP_FAILURE;
        }

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:NFIG:MODE:PROF 1\n");
        if(m_viStatus)
        {
            return DP_SPECAN_AMP_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:DUT AMPL\n");
        if(m_viStatus)
        {
            return DP_SPECAN_AMP_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the frequency as Sweep frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:FREQ:MODE SWEP\n");
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":NFIG:FREQ:STAR %f\n", in_fStartFreq);
        /* to set the start frequency */
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the stop frequency */
        sprintf(szCmdStr, ":NFIG:FREQ:STOP %f\n", in_fStopFreq);
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper::DP_Enum_SpecAn_ContRun(U8BIT in_u8InstNo, unsigned char in_ucOnOff)
{
   if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
   {
        if(in_ucOnOff)
        {
            /* Sets the marker mode as normal*/
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":INIT:CONT ON\n");
            if(m_viStatus)
            {
                return DP_SPECAN_CONT_ON_FAILURE;
            }
        }
        else
        {
            /* Sets the marker mode as normal*/
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":INIT:CONT OFF\n");
            if(m_viStatus)
            {
                return DP_SPECAN_CONT_OFF_FAILURE;
            }
        }
   }

    return DP_SPEC_AN_CNFG_SUCCESS;
}
short CSAWrapper::DP_Enum_SpecAn_SingleRun(U8BIT in_u8InstNo, unsigned char in_ucOnOff)
{
   if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
   {
        if(in_ucOnOff)
        {
            /* Sets the marker mode as normal*/
            /*This Query only applicable for R&S Noise figure single run measurement only*/
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:CONF:LIST:SINGLE;:INIT:IMM \n");
            if(m_viStatus)
            {
                return DP_SPECAN_CONT_ON_FAILURE;
            }
        }

   }

    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper::AgSA_NoiseSourceDetection(U8BIT in_u8InstNo,short *out_sm_viStatus)
{
    char szCmdStr[1024] = {0};
    memset(szCmdStr, 0, 1024);

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        sprintf(szCmdStr, "INST:SEL NFIGURE\n");

        /* Sets the marker mode as normal*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,szCmdStr);
        if(m_viStatus)
        {
            return DP_SPECAN_NS_DET_FAILURE;
        }

        sprintf(szCmdStr, "SENS:CORR:ENR:SNS:ATT?\n");
        //	sprintf(szCmdStr, "SENS:NFIG:CORR:ENR:SPOT?\n");

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,szCmdStr,"%d",out_sm_viStatus);
        if(m_viStatus)
        {
            return DP_SPECAN_NS_DET_FAILURE;
        }
     }
    //	Sleep(1000);

    return DP_SPEC_AN_CNFG_SUCCESS;
}
/***************************************************************************************************
// Name						:	 AgSA_PreAmpEnableDisable()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    31-03-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	This function use to Pre- AMP enable/Disable
********************************************************************************************************/

short CSAWrapper::AgSA_PreAmpEnableDisable(U8BIT in_u8InstNo,unsigned char in_ucSts)
{

    char szCmdStr[1024] = {""};
    memset(szCmdStr, 0, 1024);
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D)
        {
            if(!in_ucSts)
            {
                sprintf(szCmdStr, "INP:GAIN:STAT ON\n");
            }
            else
            {
                sprintf(szCmdStr, "INP:GAIN:STAT OFF\n");
            }
        }
        if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D)
        {
            if(!in_ucSts)
            {
                sprintf(szCmdStr, ":POW:GAIN ON\n");
            }
            else
            {
                sprintf(szCmdStr, ":POW:GAIN OFF\n");
            }
        }

        /* Sets the marker mode as normal*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,szCmdStr);
        if(m_viStatus)
        {
            return DP_SPECAN_PREAMP_ENADIS_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_Enum_SpecAn_ConfigRBW_VBW()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    31-03-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	This function use to Configure VBW and RBW
********************************************************************************************************/
short CSAWrapper::DP_Enum_SpecAn_ConfigRBW_VBW(U8BIT in_u8InstNo, double in_dResolutionBW,  double in_dVideoBW)
{

    char szRWBCmdStr[1024] = {""};
    char szVBWCmdStr[1024] = {""};

    memset(szRWBCmdStr, 0, 1024);
    memset(szVBWCmdStr, 0, 1024);

    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        sprintf(szRWBCmdStr, "BAND %lf KHZ\n",in_dResolutionBW/1000);
        sprintf(szVBWCmdStr, "BAND:VID %lf kHz\n",in_dVideoBW/1000);

        /* Sets the marker mode as normal*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,szRWBCmdStr);
        if(m_viStatus)
        {
            return DP_SPECAN_RBW_VBW_FAILURE;
        }

        //	Sleep(200);

        /* Sets the marker mode as normal*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,szVBWCmdStr);
        if(m_viStatus)
        {
            return DP_SPECAN_CNFG_RBWVBW_FAILURE;
        }
    }
    //	Sleep(200);

    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper::AgSA_ConfigUpConvDUTSetup(U8BIT in_u8InstNo, double in_fStartFreq, double in_fStopFreq,float in_fIFFreq, int in_iNoOfPoints, unsigned char in_ucRFContextConfigFlag ,unsigned char in_ucSideBandConfigFlag)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[100] ={0};

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D)//R&s
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:DUT UPConv\n");
        if(m_viStatus)
        {
            return DP_SPECAN_UPCON_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the frequency as Sweep frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":CONF:MODE:FREQ SWEP\n");
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /*to Set frequency of LO as Swept*/
        //	m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":NFIG:MODE:DUT:LOSC SWEPT\n");
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:MODE:SYST:LO SWEPT\n");
        if(m_viStatus)
        {
            return DP_SPECAN_UPCON_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
        if(in_ucRFContextConfigFlag == 0)
        {
            //To set Frequency Context
            sprintf(szCmdStr, ":CONF:MODE:FREQ:CONT RF\n");

        }
        else
        {
            //To set Frequency Context
            sprintf(szCmdStr, ":CONF:MODE:FREQ:CONT IF\n");

        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

    //    if(in_ucSideBandConfigFlag == 0)
    //    {
    //        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:MODE:Upconv:LOSC:OFFS LSB\n");
    //    }
    //    else if(in_ucSideBandConfigFlag == 1)
    //    {
    //        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:MODE:Upconv:LOSC:OFFS USB\n");
    //    }
    //    else if(in_ucSideBandConfigFlag == 2)
    //    {
    //        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:MODE:Upconv:LOSC:OFFS DSB\n");
    //    }

        if(m_viStatus)
        {
            return DP_SPECAN_UPCON_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

    #if 0
        //To Set the IF Frequency
        sprintf(szCmdStr, ":CONF:MODE:SYST:IF:FREQ %f\n", in_fIFFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    #endif
        sprintf(szCmdStr, "FREQ:STAR %lf\n", in_fStartFreq);
        /* to set the start frequency */
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the stop frequency */
        sprintf(szCmdStr,"FREQ:STOP %lf\n", in_fStopFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, "SWE:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //KeySight
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT Upconv\n");
        if(m_viStatus)
        {
            return DP_SPECAN_UPCON_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the frequency as Sweep frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":NFIG:FREQ:MODE SWEP\n");
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /*to Set frequency of LO as Swept*/
        //	status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":NFIG:MODE:DUT:LOSC SWEPT\n");
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT:LOSC FIXED\n");
        if(m_viStatus)
        {
            return DP_SPECAN_UPCON_DUT_SETUP_FAILURE;
        }
        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
        if(in_ucRFContextConfigFlag == 0)
        {
            //To set Frequency Context
            sprintf(szCmdStr, ":NFIG:MODE:FREQ:CONT RF\n");

        }
        else
        {
            //To set Frequency Context
            sprintf(szCmdStr, ":NFIG:MODE:FREQ:CONT IF\n");

        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        if(in_ucSideBandConfigFlag == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:Upconv:LOSC:OFFS LSB\n");
        }
        else if(in_ucSideBandConfigFlag == 1)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:Upconv:LOSC:OFFS USB\n");
        }
        else if(in_ucSideBandConfigFlag == 2)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:Upconv:LOSC:OFFS DSB\n");
        }

        if(m_viStatus)
        {
            return DP_SPECAN_UPCON_DUT_SETUP_FAILURE;
        }
        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

    #if 0
        //To Set the IF Frequency
        sprintf(szCmdStr, ":NFIG:MODE:DOWN:IF:FREQ %f\n", in_fIFFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    #endif
        sprintf(szCmdStr, ":NFIG:FREQ:STAR %lf\n", in_fStartFreq);
        /* to set the start frequency */
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the stop frequency */
        sprintf(szCmdStr,":NFIG:FREQ:STOP %lf\n", in_fStopFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper::AgSA_UpConvDUTSetupConfig(U8BIT in_u8InstNo,unsigned char in_ucFreqMode,double in_fStartFreq, double in_fStopFreq,unsigned char in_ucLOFreqMode, float in_fIFFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[100] = {0};
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:DUT Upconv\n");
        if(m_viStatus)
        {
            return DP_SPECAN_DUT_UPCON_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        if(in_ucFreqMode == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:FREQ:MODE SWEPT\n");
        }

        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENSe:FREQuency:FIXed\n");
        }

        if(m_viStatus)
        {
            return DP_SPECAN_DUT_UPCON_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        if(in_ucLOFreqMode == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:MODE:SYST:LOSC SWEPT\n");
        }
        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CONF:MODE:SYST:LOSC FIXED\n");
        }
        if(m_viStatus)
        {
            return DP_SPECAN_DUT_UPCON_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

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

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

    //    if(in_ucSideBandConfigFlag == 0)
    //    {
    //        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS LSB\n");
    //    }
    //    else if(in_ucSideBandConfigFlag == 1)
    //    {
    //        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS USB\n");
    //    }
    //    else if(in_ucSideBandConfigFlag == 2)
    //    {
    //        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS DSB\n");
    //    }

    //    if(m_viStatus)
    //        return DP_SPECAN_DUT_UPCON_SETUP_FAILURE;

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, "FREQ:STAR %lf\n", in_fStartFreq);
        /* to set the start frequency */
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the stop frequency */
        sprintf(szCmdStr, "FREQ:STOP %lf\n", in_fStopFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, "SWE:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //Keysight
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:DUT Upconv\n");
        if(m_viStatus)
        {
            return DP_SPECAN_DUT_UPCON_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        if(in_ucFreqMode == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:FREQ:MODE SWEPT\n");
        }

        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:FREQ:MODE FIXED\n");
        }

        if(m_viStatus)
        {
            return DP_SPECAN_DUT_UPCON_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        if(in_ucLOFreqMode == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT:LOSC SWEPT\n");
        }
        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT:LOSC FIXED\n");
        }
        if(m_viStatus)
            return m_viStatus;

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

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

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        if(in_ucSideBandConfigFlag == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS LSB\n");
        }
        else if(in_ucSideBandConfigFlag == 1)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS USB\n");
        }
        else if(in_ucSideBandConfigFlag == 2)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:UPC:LOSC:OFFS DSB\n");
        }

        if(m_viStatus)
        {
            return DP_SPECAN_UPCON_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":NFIG:FREQ:STAR %lf\n", in_fStartFreq);
        /* to set the start frequency */
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the stop frequency */
        sprintf(szCmdStr, ":NFIG:FREQ:STOP %lf\n", in_fStopFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    //Sleep(100);

    return DP_SPEC_AN_CNFG_SUCCESS;
}

//To Configure Spectrum in Noise Figure Frequency Mode
short CSAWrapper::AgSA_ConfigNFDownConvFreqMode(U8BIT in_u8InstNo)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[50] ={0};


    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        /* to set the frequency as Sweep frequency*/
        /*	m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":NFIG:FREQ:MODE SWEPT\n");
            ucOPCCnt = 0;
            iOper	 = 0;
            while(!iOper)
            {
                viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }*/


        /* to set LO Fixed Mode*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:SYST:LOSC:FREQ \n");
        if(m_viStatus)
        {
            return m_viStatus;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the frequency as Sweep frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SWE:TYPE SWE\n");
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        //To set Frequency Context
        sprintf(szCmdStr, "DISP:TRAC:X IF\n");
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }
    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //Keysight
    {
        /* to set the frequency as Sweep frequency*/
        /*	status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":NFIG:FREQ:MODE SWEPT\n");
            ucOPCCnt = 0;
            iOper	 = 0;
            while(!iOper)
            {
                viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }*/


        /* to set LO Fixed Mode*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT:LOSC FIXED\n");
        if(m_viStatus)
        {
            return m_viStatus;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the frequency as Sweep frequency*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:FREQ:MODE FIXED\n");
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        //To set Frwequency Context
        sprintf(szCmdStr, ":NFIG:MODE:FREQ:CONT IF\n");
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
}


/***************************************************************************************************
// Name						:	 DP_Enum_ConfigureVideoTrigger()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    24-11-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Video Trigger of the Spectrum Analyser
******************************************************************************************************/
short CSAWrapper::DP_Enum_ConfigureVideoTrigger(U8BIT in_u8InstNo, double in_dVideoTriggerLevel,int in_iVideoTriggerSlope)
{

    short m_viStatus	= 0;
    char szCmdStr[100] = {0};


    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"TRIG:SOUR VID\n");
        if(m_viStatus)
        {
            return DP_SPECAN_VID_TRIG_FAILURE;
        }

        char szVISlopeMode[2][15] = {"NEG", "POS"};
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"TRIG:VID:SLOP %s\n", szVISlopeMode[in_iVideoTriggerSlope]);
        if(m_viStatus)
        {
            return DP_SPECAN_VID_TRIG_FAILURE;
        }

        sprintf(szCmdStr, "TRIG:VID:LEV %lf\n", in_dVideoTriggerLevel);
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,szCmdStr);
        if(m_viStatus)
        {
            return m_viStatus;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

/***************************************************************************************************
// Name						:	 DP_Enum_ConfigureFreeRun()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    24-11-2018
// Revision Date			:
// Reason for Revising		:
// Description				:	Configure Trigger as free run in Spectrum Analyser
******************************************************************************************************/
short CSAWrapper::DP_Enum_ConfigureFreeRun(U8BIT in_u8InstNo)
{
    QString csFormat = "";
    short m_viStatus	   = 0;
    if((m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) || (m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D))
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"TRIG:SOUR IMM\n");
        if(m_viStatus)
        {
            return DP_SPECAN_FREE_RUN_FAILURE;
        }
    }
    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper::   AgSA_ConfigDownConvDUTSetup(U8BIT in_u8InstNo, unsigned char in_ucFreqMode, double in_fStartFreq, double in_fStopFreq, unsigned char in_ucLOFreqMode, double in_dLOFreq, int in_iNoOfPoints,unsigned char in_ucRFContextConfigFlag,unsigned char in_ucSideBandConfigFlag, unsigned char in_ucAvgSamples)
{
    short m_viStatus	= 0;
    unsigned char ucOPCCnt	= 0;
    int iOper		= 0;
    char szCmdStr[100] ={0};

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D) //R&s
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:CONF:MODE:DUT DOWN\n");
        if(m_viStatus)
        {
            return DP_SPECAN_DOWNCON_DUT_FAILURE;
        }

        /* to set the frequency as Sweep frequency*/
        if(in_ucFreqMode == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SWE:TYPE SWE\n");
        }
        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)"SENS:CONF:MODE:SYST:LOSC:FREQ\n");
        }

        if(m_viStatus)
        {
            return DP_SPECAN_DOWNCON_DUT_FAILURE;
        }

        /*to Set frequency of LO as Swept*/
        if(in_ucLOFreqMode == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:CONF:MODE:SYST:LO SWEEP\n");
        }
        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:CONF:MODE:SYST:LO FIX\n");
        }

        if(m_viStatus)
        {
            return DP_SPECAN_DOWNCON_DUT_FAILURE;
        }

        if(in_ucLOFreqMode == 1)
        {
            sprintf(szCmdStr, ":SENS:CONF:MODE:SYST:LO:FREQ %lf \n", in_dLOFreq);
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
            if(m_viStatus)
            {
                return DP_SPECAN_DOWNCON_DUT_FAILURE;
            }
        }

        if(in_ucFreqMode == 0)
        {
            sprintf(szCmdStr, ":SENS:FREQ:STAR %lf\n", in_fStartFreq);
            /* to set the start frequency */
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
            ucOPCCnt = 0;
            iOper	 = 0;
            while(!iOper)
            {
                viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }

            /* to set the stop frequency */
            sprintf(szCmdStr, ":SENS:FREQ:STOP %lf\n", in_fStopFreq);

            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
            ucOPCCnt = 0;
            iOper	 = 0;
            while(!iOper)
            {
                viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }
        }
        else
        {
            sprintf(szCmdStr, "FREQ:STAR %lf\n", in_fStartFreq);
            /* to set the start frequency */
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
            ucOPCCnt = 0;
            iOper	 = 0;
            while(!iOper)
            {
                viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }
        }

        sprintf(szCmdStr, ":SENS:FREQ:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);
            if(ucOPCCnt > 100)
                break;
            ucOPCCnt++;
        }

        sprintf(szCmdStr, ":SENS:SWE:COUN %d \n", in_ucAvgSamples);

        /* to set the average samples*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SKEY8257D) //kEYsIGHT
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":INST:CONF:NFIG:NFIG\n");
        if(m_viStatus)
        {
            return DP_SPECAN_DOWNCON_DUT_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
        sprintf(szCmdStr, ":DISP:NFIG:FORM TABL\n");
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":SENS:NFIG:MODE:PROF 1\n"); // Profile
        if(m_viStatus)
        {
            return DP_SPECAN_AMP_DUT_SETUP_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT DOWN\n");
        if(m_viStatus)
        {
            return DP_SPECAN_DOWNCON_DUT_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /* to set the frequency as Sweep frequency*/
        if(in_ucFreqMode == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:FREQ:MODE SWEPt\n");
        }
        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:FREQ:MODE FIXED\n");
        }

        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        /*to Set frequency of LO as Swept*/
        if(in_ucLOFreqMode == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT:LOSC SWEPt\n");
        }
        else
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":NFIG:MODE:DUT:LOSC FIXED\n");
        }

        if(m_viStatus)
        {
            return DP_SPECAN_DOWNCON_DUT_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

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

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, (char*)"*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        if(in_ucSideBandConfigFlag == 0)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":NFIG:MODE:DOWN:LOSC:OFFS LSB\n");
        }
        else if(in_ucSideBandConfigFlag == 1)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":NFIG:MODE:DOWN:LOSC:OFFS USB\n");
        }
        else if(in_ucSideBandConfigFlag == 2)
        {
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,":NFIG:MODE:DOWN:LOSC:OFFS DSB\n");
        }

        if(m_viStatus)
        {
            return DP_SPECAN_DUT_DOWNCON_FAILURE;
        }

        ucOPCCnt = 0;
        iOper	 = 0;

        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }


        //To Set the IF Frequency
        sprintf(szCmdStr, ":NFIG:MODE:DOWN:LOSC:FREQ %f\n", in_dLOFreq);

        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }

        if(in_ucFreqMode == 0)
        {
            sprintf(szCmdStr, ":NFIG:FREQ:STAR %lf\n", in_fStartFreq);
            /* to set the start frequency */
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
            ucOPCCnt = 0;
            iOper	 = 0;
            while(!iOper)
            {
                viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }

            /* to set the stop frequency */
            sprintf(szCmdStr, ":NFIG:FREQ:STOP %lf\n", in_fStopFreq);

            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
            ucOPCCnt = 0;
            iOper	 = 0;
            while(!iOper)
            {
                viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }
        }
        else
        {
            sprintf(szCmdStr, ":NFIG:FREQ:FIXED %lf\n", in_fStartFreq);
            /* to set the start frequency */
            m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
            ucOPCCnt = 0;
            iOper	 = 0;
            while(!iOper)
            {
                viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

                if(ucOPCCnt > 100)
                    break;

                ucOPCCnt++;
            }
        }

        sprintf(szCmdStr, ":NFIG:SWE:POIN %d\n", in_iNoOfPoints);

        /* to set the Sweep Points*/
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdStr);
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
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
short CSAWrapper:: AgSA_ConfigMaxHold(session ucSpecNo)
{
    short m_viStatus	= 0;
    UCHAR ucOPCCnt	= 0;
    int iOper		= 0;

    char szCmdStr[50] ={0};

    to set the Trace  in
    m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "TRAC1:TYPE MAXH\n");

    while(!iOper)
    {
        viQueryf(ViRFSpecAn, "*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    return m_viStatus;
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


//short CSAWrapper::DP_Enum_SpecAn_FetchYTrace (U8BIT in_u8InstNo, const char *in_pcTraceName,  signed long in_slArrayLength, signed long* out_slActPts, ContainerArray* pContainerArray)
//short CSAWrapper::DP_Enum_SpecAn_FetchYTrace (U8BIT in_u8InstNo,unsigned short in_Traceindex,  signed long in_slArrayLength, ContainerArray* pContainerArray)
short CSAWrapper::DP_Enum_SpecAn_FetchYTrace (U8BIT in_u8InstNo,unsigned short in_Traceindex,  signed long in_slArrayLength, double* out_pdAmplitude)
{

    double in_dAmplitude [1024], dVal = 0.0;
    int iCount=0;
    signed long out_slActPts = 10;
    unsigned short usIndex =0;

/* Simulation Mode of Operation */
#ifdef TESTSEQ_WITHOUT_SPECTRUM_MODULE

    for (iCount = 0;iCount<1024;iCount++)
    {
        in_dAmplitude[iCount] = iCount;
    }

//	csFormat.Format(" SpecAn Config Fetch(in_u8InstNo = %ld, TraceName = %s, ArrayLength = %ld, ActPts = %ld, Amplitude = %f)", in_u8InstNo, in_pcTraceName,  in_slArrayLength, out_slActPts, in_dAmplitude);
//	AfxMessageBox(csFormat);

#else

    FILE *fpLog = fopen("YTradeData.txt", "w");

    if(fpLog)
    {
        for(unsigned int ulIdx = 0; ulIdx < out_slActPts;  ulIdx++)
        {
            fprintf(fpLog, "%f\n", out_pdAmplitude[ulIdx]);
        }
        fclose(fpLog);
    }

#endif


    return DP_SPEC_AN_CNFG_SUCCESS;
}


short CSAWrapper::GetInstID(char *in_pcInstDetails, unsigned short *out_pusModel)
{
    unsigned char ucIdx = 0;
    bool bCmpRtn = 0;
    char *pszTok = NULL;
    char *pszModel = NULL;
    pszTok = strtok(in_pcInstDetails, ",");
    pszModel = strtok(NULL, " ,");

    for(ucIdx = 0; ucIdx < g_ucSACount;  ucIdx++)
    {
        bCmpRtn = strcasecmp(pszModel, m_SAInsconfg[ucIdx].m_szmodel);

        if (bCmpRtn == 0)
        {
            *out_pusModel = m_SAInsconfg[ucIdx].m_usInstID;
            return 0;
        }
    }

    return DP_SPECAN_INT_INVALID_MODEL;
}


short CSAWrapper:: AgSA_SetThersHold(U8BIT in_u8InstNo, bool in_bThreshotState, double in_dThreshold)
{
    /*FOR Keysight*/
    short status = 0;
    int iOper  = 0;
    unsigned char ucOPCCnt = 0;

    if(in_bThreshotState)
    {
        status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK:PEAK:THR:STAT ON\n");
        status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK:PEAK:THR %lf dBm\n",in_dThreshold);
    }
    else
    {
        status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK:PEAK:THR:STAT OFF\n");
        status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK:PEAK:THR:AUTO:STAT OFF\n");
    }

    status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK:PEAK:EXC:STAT OFF\n");
    status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":CALC:MARK:PEAK:EXC:AUTO:STAT OFF\n");

    /*FOR R&S Yet to be added*/
    return status;
}

short CSAWrapper::DP_SA_StoreStateIntoFile(U8BIT in_u8InstNo, char* in_csFileName)
{
    short status = 0;
    char szCmdString[256] = {0};
    int iOper  = 0;
    unsigned char ucOPCCnt = 0;

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D)
    {
        sprintf(szCmdString, "MMEM:STOR:STAT 1,'%s'\n", in_csFileName);
        status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdString);
    }
    else/*Keysight --- N9030B*/
    {
        sprintf(szCmdString, "MMEM:STOR:STAT \"%s.state\"\n", in_csFileName);\
        status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdString);
    }
    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    if(status != VI_SUCCESS)
    {
        return DP_SA_LOAD_FILE_FAILURE;
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
}

/*********************************************************************************
// Name						: DP_SA_LoadStateFromFile
// Author					: -
// Global Variables affected: NIL
// Created Date				: 10-Jan-23
// Revision Date			: NIL
// Reason for Revising		: NIL
// Description				: To load the state file with calibration in SA
*********************************************************************************/

short CSAWrapper::DP_SA_LoadStateFromFile(U8BIT in_u8InstNo, char* strFileName)
{
    short status = 0;
    int iOper  = 0;
    unsigned char ucOPCCnt = 0;
    char szCmdString[256] = {0};

    if(m_arrSInstHandle[in_u8InstNo-1].m_usInstID == SAE8267D)
    {
        sprintf(szCmdString, "MMEM:LOAD:STAT 1,'%s'\n", strFileName);
        status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdString);
        if(status != VI_SUCCESS)
        {
            return DP_SA_LOAD_FILE_FAILURE;
        }
        Sleep(2000);
    }
    else/*Keysight --- N9030B*/
    {
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":INST:CONF:NFIG:NFIG\n");
        if(m_viStatus)
        {
            return DP_SPECAN_DOWNCON_DUT_FAILURE;
        }
        ucOPCCnt = 0;
        iOper	 = 0;
        while(!iOper)
        {
            viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

            if(ucOPCCnt > 100)
                break;

            ucOPCCnt++;
        }
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession,(char*)":DISP:NFIG:FORM TABLe\n");
        if(m_viStatus)
        {
            return DP_SPECAN_AMP_DUT_SETUP_FAILURE;
        }
        Sleep(2);
        sprintf(szCmdString, "MMEM:LOAD:STAT \"%s.state\"\n", strFileName);
        m_viStatus = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdString);
        if(m_viStatus)
        {
            return DP_SPECAN_DOWNCON_DUT_FAILURE;
        }
    }

    ucOPCCnt = 0;
    iOper	 = 0;
    while(!iOper)
    {
        viQueryf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, "*OPC?\n", "%d", &iOper);

        if(ucOPCCnt > 100)
            break;

        ucOPCCnt++;
    }

    if(status != VI_SUCCESS)
    {
        return DP_SA_LOAD_FILE_FAILURE;
    }

    return DP_SPEC_AN_CNFG_SUCCESS;
}

short CSAWrapper::DP_Enum_SpecAn_NFConfigBW(U8BIT in_u8InstNo, double in_dBandWidth)
{
    short status = 0;
    int iOper  = 0;
    unsigned char ucOPCCnt = 0;
    char szCmdString[256] = {0};

    sprintf(szCmdString, ":SENS:NFIG:BWID:RES %lf \n",in_dBandWidth*1e6);
    status = viPrintf(m_arrSInstHandle[in_u8InstNo-1].m_viSASession, szCmdString);
    if(status != VI_SUCCESS)
    {
        return DP_SA_LOAD_FILE_FAILURE;
    }
         return DP_SPEC_AN_CNFG_SUCCESS;
}
