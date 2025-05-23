#include "IVIRFSignalGenEnumeration.h"
#include "../src/dp_esm_1119_datastructure.h"
#include "../Instrument/visa.h"
#include "../Instrument/visatype.h"
#include "stdio.h"
#include <unistd.h>
#include "qthread.h"
#define CHARACTER_SIZE 50

ViSession g_viRFSIGGENSession[4];
ViSession defaultSGRM = 0, g_viRFSG[3] = {0}, viSG = 0;
ViStatus status;
extern SGlobalStucture gGlobalStuct;
/**************************************************************************

					RFSIGNAL - Enumeration


***************************************************************************************************/



/**************************************************************************
// Name						:	 Open()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	To Open RF signal gen. 
**************************************************************************/

short CIVI_RFSIGGEN::Open(unsigned char in_ucSGNo, char *pLogicalName)
{
    ViChar value[128]  = {0};
    char ucTemp[20]    = {0};
    char ucLoop        =  0;
    strcpy(value,pLogicalName);
	if(in_ucSGNo == 0)
	{
		return -1;
    }
	try
	{
		if(g_viRFSIGGENSession[in_ucSGNo - 1])
		{
			viClose(g_viRFSIGGENSession[in_ucSGNo - 1]);
			g_viRFSIGGENSession[in_ucSGNo - 1] = 0;
		}

            for( ucLoop = 8; ucLoop <= 18; ucLoop++)
			{
				ucTemp[ucLoop-8] = value[ucLoop];
            }

            strcpy(gGlobalStuct.sCOTSInstInfo.m_cSGInfo,ucTemp);
            viOpenDefaultRM(&defaultSGRM);

            status = viOpen(defaultSGRM, value, VI_NULL,VI_NULL, &g_viRFSIGGENSession[in_ucSGNo - 1] );

            if(!g_viRFSIGGENSession[in_ucSGNo - 1])
            {
                viOpenDefaultRM(&defaultSGRM);
                status = viOpen(defaultSGRM, value, VI_NULL,VI_NULL, &g_viRFSIGGENSession[in_ucSGNo - 1] );
                if(status)
                {
                    return -1;
                }
                gGlobalStuct.sHandles.m_viRFSG[in_ucSGNo - 1] = g_viRFSIGGENSession[in_ucSGNo - 1];
            }
            else
            {
                gGlobalStuct.sHandles.m_viRFSG[in_ucSGNo - 1] = g_viRFSIGGENSession[in_ucSGNo - 1];
            }

		}

	catch(...) 
    {
    }

    return status;

}

/**************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigRF()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	 This function configures the frequency and the power level of the RF output signal.
******************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigRF(session ulVi ,double dFrequency, double dPowerLevel)
{
    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:FIXed %lf\n", dFrequency);
	if (status)
	{
        return status;
	}
    status = viPrintf(ulVi, (char*)":SOURce:POWer:LEVel:IMMediate:AMPLitude %lf\n", dPowerLevel);
	if (status)
	{
		return status; 
	}
	
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigALCEnabled ()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function enables the Automatic Level Control.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigALCEnabled (session ulVi, bool bAlcEnabled)
{
	if (bAlcEnabled)
	{
        status = viPrintf(ulVi, (char*)":SOURce:POWer:ALC:STATe ON\n");
		if (status)
		{
			return status; 
		}
	}
	else{
        status = viPrintf(ulVi, (char*)":SOURce:POWer:ALC:STATe OFF\n");
		if (status)
		{
			return status; 
		}
	}
	
	return status;

}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_Preset()
// Author					:	 Moorthy BS 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function preset the signal generator.
******************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_Preset(session ulVi)
{
    status = viPrintf(ulVi, (char*)"*RST\n");
	if (status)
	{
		return status; 
	}

	return (short)status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_Close()
// Author					:	 Moorthy BS 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function preset the signal generator.
******************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_Close(session ulVi)
{
	if(ulVi)
	{
		viClose(ulVi);
		ulVi = 0;
		return 0;
	}
	else
	{
		return -1;
	}
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigOutputEnabled()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function enables the RF output signal.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigOutputEnabled(session ulVi, bool bOutputEnabled)
{
	if (bOutputEnabled)
	{
        status = viPrintf(ulVi, (char*)":OUTPut:STATe ON\n");
		if (status)
		{
			return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":OUTPut:STATe OFF\n");
		if (status)
		{
			return status; 
		}
	}
	
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigAMEnabled()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the signal generator to apply amplitude modulation to the RF output signal.
******************************************************************************************************/


short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigAMEnabled(session ulVi, bool bAmEnabled)
{
	if (bAmEnabled)
	{
        status = viPrintf(ulVi, (char*)":SOURce:AM:STATe ON\n");
		if (status)
		{
			return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":SOURce:AM:STATe OFF\n");
		if (status)
		{
			return status; 
		}
	}

	return status;
}


/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigAMExtCoupling()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the coupling of an external source for amplitude modulation.

******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigAMExtCoupling(session ulVi, unsigned int in_uiAmExternalCoupling)
{

	if (in_uiAmExternalCoupling)
	{
        status = viPrintf(ulVi, (char*)":SOURce:AM:EXTernal:COUPling AC\n");
		if (status)
		{
			return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":SOURce:AM:EXTernal:COUPling DC\n");
		if (status)
		{
			return status; 
		}
	}

	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigAM()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the modulation source, scaling, 
								and depth for the signal generator's amplitude modulation.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigAM(ViSession ulVi, double in_dAMRate, double dDepth)
{	
    status = viPrintf(ulVi, (char*)":SOURCE:AM1:TYPE LINEAR\n");
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(ulVi, (char*)":SOURCE:AM:INTERNAL:FREQUENCY %lf\n", in_dAMRate);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(ulVi, (char*)":SOURCE:AM:DEPTH:LINEAR %lf\n", dDepth);
	if (status)
	{
		return status; 
	}
	
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigFMEnabled()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the signal generator to apply frequency modulation to the RF output signal.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigFMEnabled(session ulVi, unsigned int uiFmEnabled)
{
	if (uiFmEnabled)
	{
        status = viPrintf(ulVi, (char*)":SOURce:FM:STATe ON\n");
		if (status)
		{
			return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":SOURce:FM:STATe OFF\n");
		if (status)
		{
			return status; 
		}
	}

	return status;
}


/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigFMExtCoupling()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the coupling of an external source for frequency modulation.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigFMExtCoupling(session ulVi,unsigned int in_uiFmExternalCoupling)
{
	if(in_uiFmExternalCoupling)
	{
        status = viPrintf(ulVi, (char*)":SOURce:FM:EXTernal:COUPling AC\n");
		if (status)
		{
		return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":SOURce:FM:EXTernal:COUPling DC\n");
		if (status)
		{
		return status; 
		}
	}

	return status;
}


/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigFM  ()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the modulation deviation and modulation source for the signal 
								generator's frequency modulation.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigFM  (session ulVi,double in_dFMRate, double in_dDeviation)
{
    status = viPrintf(ulVi, (char*)":SOURce:FM:DEViation %lf\n", in_dDeviation);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(ulVi, (char*)":SOURce:FM:INTernal:FREQuency %lf\n", in_dFMRate);
	if (status)
	{
		return status; 
	}
		
	return status;
}
/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPMEnabled ()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the signal generator to apply phase modulation to the RF output signal.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPMEnabled  (session ulVi,bool bPmEnabled)
{
	if (bPmEnabled)
	{
        status = viPrintf(ulVi, (char*)":SOURce:PM:STATe ON\n");
		if (status)
		{
			return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":SOURce:PM:STATe OFF\n");
		if (status)
		{
			return status; 
		}
	}

	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPMExtCoupling ()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the coupling of an external source for phase modulation.
******************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPMExtCoupling  (session ulVi, unsigned int in_uiPmExternalCoupling)
{
	if(in_uiPmExternalCoupling)
	{
        status = viPrintf(ulVi, (char*)":SOURce:PM:EXTernal:COUPling AC\n");
		if (status)
		{
		return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":SOURce:PM:EXTernal:COUPling DC\n");
		if (status)
		{
		return status; 
		}
	}

	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPM  ()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	Configures the attribute that control the signal generator's phase modulation. 
								The attributes are the modulation deviation and the modulating source(s). 
								This function configures the modulation deviation and modulation source for the 
								signal generator's phase modulation.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPM(session ulVi,unsigned int in_uiIndex, double dDeviation)
{
    status = viPrintf(ulVi, (char*)":SOURce:PM:DEViation %lf\n",dDeviation);
	if (status)
	{
		return status; 
	}
	
	// Index..To be Add
	
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPulMEnabled  ()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the signal generator to apply pulse modulation to the RF output signal.

******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPulMEnabled  (session ulVi,bool bPulseModulationEnabled)
{
	if (bPulseModulationEnabled)
	{
        status = viPrintf(ulVi, (char*)":SOURce:PULM:STATe ON\n");
		if (status)
		{
			return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":SOURce:PULM:STATe OFF\n");
		if (status)
		{
			return status; 
		}
	}
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPulMSource  ()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function sets the source for pulse modulation of the RF output signal.
******************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPulMSource (session ulVi,unsigned int in_uiSource)
{
 
	char szPULMode[3][15] = {"INTernal", "EXTernal", "SCALar"};
    status = viPrintf(ulVi, (char*)":SOURce:PULM:SOURce %s\n",szPULMode[in_uiSource]);
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":SOURce:PULM:STATe ON\n");
	if (status)
	{
		return status; 
	}
	return status;
}
/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPulMExtPolarity()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function specifies the polarity of the external source signal.

********************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPulMExtPolarity(session ulVi,unsigned int in_uiExternalPolarity)
{
	char szPULExtMode[2][15] = {"NORMal", "INVerted"};
    status = viPrintf(ulVi, (char*)":SOURce:PULM:EXTernal:POLarity %s\n",szPULExtMode[in_uiExternalPolarity]);
	if (status)
	{
		return status; 
	}

	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_ConfigLFGen()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the LF generators output frequency and waveform.
********************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_ConfigLFGen(session ulVi,unsigned int in_uiSrcIndex,unsigned int in_uiSrcActive,double dFrequency,unsigned int in_uiWaveform)
{
	char szLFGenMode[9][15] = {"SINE", "DUALsine", "SWEPtsine", "TRIangle", "SQUare", "RAMP", "PULSe", "NOISe", "DC" };
    status = viPrintf(ulVi, (char*)":SOURce:LFOutput:FUNCtion:FREQuency %lf\n",dFrequency);
	if (status)
	{
		return status; 
	}

    status = viPrintf(ulVi, (char*)":SOURce:LFOutput:FUNCtion:SHAPe %d\n",szLFGenMode[in_uiWaveform]);
	if (status)
	{
		return status; 
	}

	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigLFGenOutput()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function sets the output voltage and enables the LF generator within the RF signal generator.
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigLFGenOutput(session ulVi,double dAmplitude, bool bEnabled)
{
    status = viPrintf(ulVi, (char*)":SOURce:LFOutput:AMPLitude %lf\n",dAmplitude);
	if (status)
	{
		return status; 
	}
	
	if(bEnabled)
	{
        status = viPrintf(ulVi, (char*)":SOURce:LFOutput:STATe %ON\n");
		if (status)
		{
			return status; 
		}
	}
	else
	{
        status = viPrintf(ulVi, (char*)":SOURce:LFOutput:STATe %OFF\n");
		if (status)
		{
			return status; 
		}
	}
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPulIntTrg()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function specifies the time period (repetition rate) of the pulse generator 
								when using the internal trigger (free run) mode.
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPulIntTrg(session ulVi, double dPeriod)
{
    status = viPrintf(ulVi, (char*)":SOURce:PULM:INTernal:PERiod:STEP:INCRement %lf\n",dPeriod);
	if (status)
	{
		return status; 
	}
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPul()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the trigger source, pulse width, and gating enabled for the 
								pulse generator within the RF signal generator.
********************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPul(session ulVi, unsigned int in_uiTriggerSource, double dPulseWidth, bool bGatingEnabled)
{

	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigSweep()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the signal generator sweep mode and trigger source.
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigSweep(session ulVi, unsigned int in_uiMode, unsigned int in_uiTriggerSource)
{
	char szFreqMode[4][15] = {"FIXed", "CW", "SWEep", "LIST"};
    status = viPrintf(ulVi,(char*)":SOURce:FREQuency:MODE %s\n", szFreqMode[in_uiMode]);
	if (status)
	{
		return -1; 
	}
	char szSwepTrigMode[4][15] = {"BUS", "IMMediate", "EXTernal", "KEY"};
    status = viPrintf(ulVi, (char*)":TRIGger:SEQuence:SOURce %s\n", szSwepTrigMode[in_uiTriggerSource]);
	if (status)
	{
		return -1; 
	}
	
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_FreqSweepStrtStp()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the start and stop frequencies for a frequency sweep. 
								If the stop frequency is less than the start frequency, the frequency decreases during the sweep.


********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_FreqSweepStrtStp(session ulVi, double dStart, double dStop)
{
    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:STARt %lf\n", dStart);
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:STOP %lf\n", dStart);
	if (status)
	{
		return status; 
	}

	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_FreqSweepCenterSpan()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the center frequency and span for a frequency sweep. 
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_FreqSweepCenterSpan(session ulVi, double dCenter, double dSpan)
{

    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:CENTer %lf\n", dCenter);
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:SPAN %lf\n", dSpan);
	if (status)
	{
		return status; 
	}
	
	return status;
}


/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_FreqSweepTime()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the duration of one frequency sweep.
********************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_FreqSweepTime(session ulVi, double dTime)
{
    status = viPrintf(ulVi, (char*)":SOURce:SWEep:TIME %lf\n", dTime);
	if (status)
	{
		return status; 
	}
	
	return status;

}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_PowerSweepStrtStp()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the start and stop power for a power sweep. 
********************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_PowerSweepStrtStp(session ulVi, double dStart, double dStop)
{
    status = viPrintf(ulVi, (char*)":SOURce:POWER:MODE SWEep\n");
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":SOURce:POWER:STARt %lf\n", dStart);
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":SOURce:POWER:STOP %lf\n", dStart);
	if (status)
	{
		return status; 
	}
	
	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_PowerSweepTime()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the duration of one power sweep.
********************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_PowerSweepTime(session ulVi, double dTime)
{
    status = viPrintf(ulVi, (char*)":SOURce:SWEep:TIME %lf\n", dTime);
	if (status)
	{
		return status; 
	}

	return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_FreqStepStrtStp()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the settings that control the step frequencies of the generator's RF output signal.
								These settings are start and stop frequency, step size, and lin/log scaling.
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_FreqStepStrtStp(session ulVi, double dstart, double dstop, double dDwell, unsigned int in_uiNoPoints )
{
	
    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:MODE SWEep\n");
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":TRIGger:SEQuence:SOURce IMMediate\n");
	if (status)
	{
		return status; 
	}
		
    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:STARt %lf\n", dstart);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:STOP %lf\n", dstop);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(ulVi, (char*)":SOURce:SWEep:DWELl %lf\n", dDwell);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(ulVi, (char*)":SOURce:SWEep:POINts %d\n", in_uiNoPoints);
	if (status)
	{
		return status; 
	}

	return status;
	
}
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_FreqStepStrtStp1(session ulVi, double dstart, double dstop, double dDwell, unsigned int in_uiNoPoints , double dPowerLevel)
{
#if 0
    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:MODE SWEep\n");
    if (status)
    {
        return status;
    }
    status = viPrintf(ulVi, (char*)":TRIGger:SEQuence:SOURce IMMediate\n");
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:STARt %lf\n", dstart);
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)":SOURce:FREQuency:STOP %lf\n", dstop);
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)":SOURce:SWEep:DWELl %lf\n", dDwell);
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)":SOURce:SWEep:POINts %d\n", in_uiNoPoints);
    if (status)
    {
        return status;
    }
    status = viPrintf(ulVi, (char*)":SOURce:POWer:ATT:AUTO 1\n");
    if (status)
    {
        return status;
    }
    status = viPrintf(ulVi, (char*)":SOURce:POWer:AMPLitude %lf\n", dPowerLevel);
    if (status)
    {
        return status;
    }
    return status;
#endif
    char Data[500];
    unsigned long ulRetCnt = 0.0f;
    long lStatus = 0;
    unsigned char ucData[2048] = {0};

    memset(Data,0,sizeof(Data));
    memset(ucData,0,sizeof(ucData));

    status = viPrintf(ulVi, (char*)"*CLS\n");
    if (status)
    {
        return status;
    }


    lStatus = DP_Enumeration_RFSigGen_ConfigALCEnabled(ulVi,0);

    sprintf(Data,"FREQ:MODE LIST\n");

    memcpy(ucData, Data, strlen(Data));

    lStatus = viWrite(ulVi, ucData, strlen(Data), &ulRetCnt);
    if(lStatus)
    {
        return lStatus;
    }

    status = viPrintf(ulVi, (char*)"LIST:TYPE STEP\n");
    if (status)
    {
        return status;
    }


    status = viPrintf(ulVi, (char*)"FREQ:STAR %lf\n", dstart);
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)"FREQ:STOP %lf\n", dstop);
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)"SWE:POIN %d\n", in_uiNoPoints);
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)"SWE:DWEL %lf\n", dDwell);
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)"POW:ATT:AUTO 1\n");
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)"POW:AMPL %lf\n",dPowerLevel);
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)"OUTP:STAT ON\n");
    if (status)
    {
        return status;
    }

    status = viPrintf(ulVi, (char*)"INIT:CONT ON\n");
    if (status)
    {
        return status;
    }


    return status;
}

/***************************************************************************************************
// Name						:	 DP_Enumeration_ConfigFreqStepDwell()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures how the frequency sweep advances.
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_ConfigFreqStepDwell(session ulVi,bool bEnabled, double dDwell)
{
    status = viPrintf(ulVi, (char*)":SOURce:SWEep:DWELl %lf\n",dDwell);
	if (status)
	{
		return status; 
	}
	
	return status;
}
/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_PwrStepStrtStp()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the settings that control the power steps of the generator's RF output signal. 
								These settings are start and stop power and step size. If the stop power is less than the start power, 
								the power decreases in value during the sweep.
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_PwrStepStrtStp(session ulVi, double dStart, double dStop, unsigned int in_uiNoPoints)
{
    status = viPrintf(ulVi, (char*)":SOURce:POWER:MODE SWEep\n");
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":SOURce:POWER:STARt %lf\n", dStart);
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":SOURce:POWER:STOP %lf\n", dStop);
	if (status)
	{
		return status; 
	}
    status = viPrintf(ulVi, (char*)":SOURce:SWEep:POINts %d\n", in_uiNoPoints);
	if (status)
	{
		return status; 
	}
	
	return status;

}

/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPowerStepDwell()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures how the power sweep advances.
********************************************************************************************************/
short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigPowerStepDwell(session ulVi,bool bEnabled, double dDwell)
{
    status = viPrintf(ulVi, (char*)":SOURce:SWEep:DWELl %lf\n", dDwell);
	if (status)
	{
		return status; 
	}
	return status;
}
/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigRefOsc()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the signal generator's reference oscillator.
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigRefOsc(session ulVi,unsigned int in_uiSource, double dFrequency)
{
    status = viPrintf(ulVi, (char*)":SOURce:ROSCillator:SOURce:AUTO");
	if (status)
	{
		return status; 
	}

	return status;
}


/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigALC()
// Author					:	 kannan.S 	
// Global Variables affected:	 NA				 
// Created Date				:    26-03-09
// Revision Date			:	 
// Reason for Revising		:
// Description				:	This function configures the ALC (Automatic Level Control) of the signal generator's RF output.
********************************************************************************************************/

short CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_ConfigALC(session ulVi,unsigned int in_uiSource, double dBandwidth)
{
    status = viPrintf(ulVi, (char*)":SOURce:POWer:ALC:STATe ON\n");
	if (status)
	{
		return status; 
	}
	
	char szALCMode[4][15] = {"INTernal", "DIODe", "MMHead"};
    status = viPrintf(ulVi, (char*)":SOURce:POWer:ALC:SOURce %d\n", szALCMode[in_uiSource]);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(ulVi, (char*)":SOURce:POWer:ALC:BANDwidth %lf\n", dBandwidth);
	if (status)
	{
		return status; 
	}
	
	return status; 
}

long CIVI_RFSIGGEN::DP_Enumeration_RFSigGen_SelfTest(session ulVi, unsigned int uiTimedelay, short *out_sStatus)
{
	ViChar selfTestMessage[100] = {0};	
	short lSelfTestResult = 0,sts = 0;
    ViChar viSts[100] = {0};
	ViInt32 viTimeOut = 0;
	ViInt32 viTimeDelay = 0;
    ViInt16	*Sts = NULL;
	
	viTimeDelay = uiTimedelay; 

	viSetAttribute(ulVi,VI_ATTR_TMO_VALUE,viTimeDelay);
    status = viQueryf(ulVi, (char*)"*TST?\n", "%d", out_sStatus);
	if(status)
	{
		return status;
	}

	return status;
}

long CIVI_RFSIGGEN::ConfigFreqStepSweep(session in_ulVi, double in_dStartFreq, double in_dStopFreq,  unsigned int in_uiNoPoints, double in_dDwellTime )
{
    status = viPrintf(in_ulVi, (char*)":SOURce:FREQuency:MODE SWEep\n");
	if (status)
	{
		return status; 
	}
    status = viPrintf(in_ulVi, (char*)":TRIGger:SEQuence:SOURce IMMediate\n");
	if (status)
	{
		return status; 
	}
		
    status = viPrintf(in_ulVi, (char*)":SOURce:FREQuency:STARt %lf\n", in_dStartFreq);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(in_ulVi, (char*)":SOURce:FREQuency:STOP %lf\n", in_dStopFreq);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(in_ulVi, (char*)":SOURce:SWEep:DWELl %lf\n", in_dDwellTime);
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(in_ulVi, (char*)":SOURce:SWEep:POINts %d\n", in_uiNoPoints);
	if (status)
	{
		return status; 
	}
	
	return 0;
}

short CIVI_RFSIGGEN::EnableDisableFreqSweep(session in_ulVi, bool in_bEnaDisable)
{
	if(in_bEnaDisable)
	{
        status = viPrintf(in_ulVi, (char*)":SWEep:CONTrol:STATe ON\n");
		if(status)
		{
			return status;
		}
		
	}
	else
	{
        status = viPrintf(in_ulVi, (char*)":SWEep:CONTrol:STATe OFF\n");
		if(status)
		{
			return status;
		}	
	}
	return status;
}
//To Disable all modulation
short CIVI_RFSIGGEN::DisableAllModulation(session in_ulVi)
{
    status = viPrintf(in_ulVi, (char*)":OUTPut:MODulation:STATe OFF\n");
	if(status)
	{
		return status;
	}
	
	return status;
}
//Configuration of List with Frequency and Power
short CIVI_RFSIGGEN::Config_ListConfiguration(session ulVi, double *in_dFreq, double *in_dPwr, float *fDwellTime, unsigned char in_ucListCount)
{
	short status = 0;
    char szListDwell[255] = {0};
    char szListFreq[255] = {0};
    char szListPwr[255] = {0};
    char szFreq[255] = {0};
    char szPwr[255] = {0};
    char szDwell[255] = {0};
	double dFreq[20] = {0.0};
    double dPwr[20] = {0.0}; 
	
    status = viPrintf(ulVi, (char*)":SOURce:LIST:MODE AUTO");
	if(status)
	{
		return status;
	}
	
    for(unsigned char ucCount = 0; ucCount < in_ucListCount; ucCount++)
	{	
		dFreq[ucCount] = in_dFreq[ucCount];
		dPwr[ucCount] = in_dPwr[ucCount];
		if(ucCount == 0)
		{
			sprintf(szListFreq, "%0.3lf", dFreq[ucCount] );
			sprintf(szListPwr,  "%0.2lf", dPwr[ucCount] );
			sprintf(szListDwell,"%0.4f",  fDwellTime );			
		}
		else
		{
			
			sprintf(szFreq,  ",%0.3lf", dFreq[ucCount] );
			sprintf(szPwr,   ",%0.2lf", dPwr[ucCount] );
			sprintf(szDwell, ",%0.4f",  fDwellTime[ucCount] );

			sprintf(szListFreq, "%s%s", szListFreq, szFreq);
			sprintf(szListPwr,  "%s%s", szListPwr, szPwr);
			sprintf(szListDwell,"%s%s", szListDwell, szDwell);
			
		}
		
	}
	
    status = viPrintf(ulVi, (char*)":SOURce:LIST:FREQuency %s\n", szListFreq);
	if(status)
	{
		return status;
	}
	
    status = viPrintf(ulVi, (char*)":SOURce:LIST:POWer %s\n", szListPwr);
	if(status)
	{
		return status;
	}
	
    status = viPrintf(ulVi, (char*)":SOURce:LIST:DWELL %s\n", szListDwell);
	if(status)
	{
		return status;
	}
	
	return 0;
	
}


long CIVI_RFSIGGEN::Config_List_Dwell(session ulVi, char *strListDwell)
{
    status = viPrintf(ulVi, (char*)":SOURce:LIST:DWELL %s\n", strListDwell);
	if(status)
	{
		return status;
	}
	return 0;

} 

short CIVI_RFSIGGEN::Config_ListSweep(session ulVi)
{
	//To Configure in list
    status = viPrintf(ulVi, (char*)":SOURCE:LIST:TYPE LIST\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(ulVi, (char*)":INIT:CONT:ALL 0\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(ulVi, (char*)":SOURCE:LIST:DIR UP\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(ulVi, (char*)":SOURCE:LIST:SWEEP:TRIGGER:TYPE FRUN\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(ulVi, (char*)":SOURCE:LIST:DWELL:TYPE LIST\n"); //To Configure Dwell Type
	if(status)
	{
		return status;
	}
	
    status = viPrintf(ulVi, (char*)":SOURCE:FREQ:MODE LIST\n"); ////To On the Frequency Sweep
	if(status)
	{
		return status;
	}

     QThread::msleep(500);
	
	return 0;

}


short CIVI_RFSIGGEN::Run_Single_Sweep(session in_ulVi)
{	
    status = viPrintf(in_ulVi, (char*)":SOURCE:POWER:MODE LIST\n");
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(in_ulVi, (char*)":SOURCE:LIST:SWEEP:TRIGGER:TYPE FRUN\n"); //To Make Sigle Sweep
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(in_ulVi, (char*)":INIT:CONT:ALL OFF\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(in_ulVi, (char*)":INIT:IMM:ALL\n");
	if (status)
	{
		return status; 
	}
	
    status = viPrintf(in_ulVi, (char*)":SOURCE:TSWEEP\n");
	if (status)
	{
		return status; 
	}

	return 0;

}


short CIVI_RFSIGGEN::Run_Continuous_Sweep(session in_ulVi)
{
    status = viPrintf(in_ulVi, (char*)":SOURCE:POWER:MODE LIST\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(in_ulVi, (char*)":SOURCE:LIST:SWEEP:TRIGGER:TYPE FRUN\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(in_ulVi, (char*)":INIT:IMM:ALL\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(in_ulVi, (char*)":SOURCE:TSWEEP\n");
	if(status)
	{
		return status;
	}
	
    status = viPrintf(in_ulVi, (char*)":INIT:CONT:ALL ON\n");
	if(status)
	{
		return status;
	}

	return status;

}

short CIVI_RFSIGGEN::Stop_Continuous_Sweep(session in_ulVi)
{	
    status = viPrintf(in_ulVi, (char*)":INITiate:CONTinuous:ALL OFF\n");
	if(status)
	{
		return status;
	}
	
	return status;
}
