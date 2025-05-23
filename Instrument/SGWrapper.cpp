#include "SGWrapper.h"
#include <stdio.h>
#include <qdebug.h>
#include "dprfsigen_err.h"
#include "src/DP_datastructure.h"

extern SGlobalStucture gGlobalStuct;
extern unsigned char g_ucSGCount;
ViSession g_viSGSession;
CSGWrapper::CSGWrapper()
{

    memset(m_arrSInstHandle, 0, sizeof(m_arrSInstHandle) );

}
/**************************************************************************
// Name						:	 Open()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    26-03-09
// Revision Date			:
// Reason for Revising		:
// Description				:	To Open RF signal gen.
**************************************************************************/
short CSGWrapper::Open(unsigned char in_ucSGNo, char *pLogicalName)
{
    ViChar value[128]  = {0};
    ViSession defaultSGRM = 0;
    ViSession viSGSession = 0;
    unsigned short usInstID = 0;
    unsigned int uiReadCnt = 0;
    unsigned char ucLoop = 0;
    char arripadress[100]={0};
    char szInstDetails [256] = {0};
    char ucTemp[256] ={0};
    strcpy(value,pLogicalName);

    try
    {
        vistatus =  viOpenDefaultRM(&defaultSGRM);
        if(vistatus != 0)
        {
            return DP_SIGGEN_OPEN_FAILURE;
        }

        vistatus = viOpen(defaultSGRM, pLogicalName, VI_NULL, VI_NULL, &viSGSession);
        if(vistatus != 0)
        {
            vistatus = viOpenDefaultRM(&defaultSGRM);
            if(vistatus != 0)
            {
                return DP_SIGGEN_OPEN_FAILURE;
            }
            sprintf(arripadress, "%s", pLogicalName);
            vistatus = viOpen(defaultSGRM, arripadress, VI_NULL, VI_NULL, &viSGSession );
            if(vistatus)
            {
                return DP_SIGGEN_OPEN_FAILURE;
            }
        }

        for( ucLoop = 8; ucLoop <= 18; ucLoop++)
        {
            ucTemp[ucLoop-8] = value[ucLoop];
        }

        strcpy(gGlobalStuct.sCOTSInstInfo.m_cSGInfo[in_ucSGNo-1],ucTemp);

        /*Read Instrument Type*/

        vistatus = viPrintf(viSGSession, (char*)"*IDN?\n");
        if(vistatus)
        {
            return DP_SIGGEN_INT_ID_READ_FAILED;
        }

        vistatus = viBufRead(viSGSession, (ViPBuf)szInstDetails, 256,(ViPUInt32)&uiReadCnt);
        if(vistatus)
        {
            return DP_SIGGEN_INT_ID_READ_FAILED;
        }

        /*Read the Model ID*/
       vistatus = GetInstID(szInstDetails, &usInstID);

       if(vistatus != 0)
       {
           viClose(viSGSession);

           return DP_SIGGEN_INT_INVALID_MODEL;
       }
       else
       {
           m_arrSInstHandle[in_ucSGNo-1].m_viSGSession = viSGSession;
           g_viSGSession = viSGSession;
           m_arrSInstHandle[in_ucSGNo-1].m_usInstID = usInstID;
       }
    }

    catch(...)
    {
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_Preset(unsigned char in_ucSGNo)
{
    qDebug()<<g_viSGSession;
    vistatus = viPrintf(g_viSGSession, "*RST\n");
    if (vistatus)
    {
        return DP_SIGGEN_PRST_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_Close(unsigned char in_ucSGNo)
{
    vistatus = viClose(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession);
    if(vistatus != 0)
    {
        return DP_SIGGEN_DISCONN_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;

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

short CSGWrapper::DP_Enumeration_RFSigGen_ConfigRF(unsigned char in_ucSGNo, double dFrequency, double dPowerLevel)
{
    if((m_arrSInstHandle[in_ucSGNo-1].m_usInstID == KS_E8257D) || (m_arrSInstHandle[in_ucSGNo-1].m_usInstID == RS_SMB100A) || (m_arrSInstHandle[in_ucSGNo-1].m_usInstID == KS_N5183B))
    {

//        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:FIXed %lf\n", dFrequency);
//        if (vistatus)
//        {
//            return DP_SIGGEN_FREQ_CNFG_FAILURE;
//        }

        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency %lf\n", dFrequency);
        if (vistatus)
        {
            return DP_SIGGEN_FREQ_CNFG_FAILURE;
        }

        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWer:LEVel:IMMediate:AMPLitude %lf\n", dPowerLevel);
        if (vistatus)
        {
            return DP_SIGGEN_PWRLVL_CNFG_FAILURE;
        }
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigOutputEnabled(unsigned char in_ucSGNo, bool bOutputEnabled)
{

    if((m_arrSInstHandle[in_ucSGNo-1].m_usInstID == KS_E8257D) || (m_arrSInstHandle[in_ucSGNo-1].m_usInstID == RS_SMB100A) || (m_arrSInstHandle[in_ucSGNo-1].m_usInstID == KS_N5183B))
    {
        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":OUTPut:STATe %d\n", bOutputEnabled);
        if (vistatus)
        {
            return DP_SIGGEN_ALC_FAILURE;
        }
    }

    return DP_SIGGEN_SUCCESS;

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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigALCEnabled (unsigned char in_ucSGNo, bool bAlcEnabled)
{
    char szCmd[64];

    if((m_arrSInstHandle[in_ucSGNo-1].m_usInstID == KS_E8257D) || (m_arrSInstHandle[in_ucSGNo-1].m_usInstID == RS_SMB100A))
    {

        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWer:ALC:STATe %d\n", bAlcEnabled);
        if (vistatus)
        {
            return DP_SIGGEN_ALC_FAILURE;
        }
    }

    return DP_SIGGEN_SUCCESS;

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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigAMEnabled(unsigned char in_ucSGNo, unsigned char in_ucSource, bool bAmEnabled)
{
    char szPMSource[3][30] = {"INTernal","EXTernal","INT,EXT"};

    vistatus = ((bAmEnabled) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:AM:STATe ON\n") : viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:AM:STATe OFF\n");
    if (vistatus)
    {
        return DP_SIGGEN_RFAM_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"AM:SOUR %s\n", szPMSource[in_ucSource]);
    if (vistatus)
    {
        return DP_SIGGEN_MOD_SRC_FAILED;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigAMExtCoupling(unsigned char in_ucSGNo, unsigned int in_uiAmExternalCoupling)
{
    vistatus = ((in_uiAmExternalCoupling) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:AM:EXTernal:COUPling AC\n") : viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:AM:EXTernal:COUPling DC\n");
    if (vistatus)
    {
        return DP_SIGGEN_RFAM_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigAM(unsigned char in_ucSGNo, unsigned char in_ucAMType, double in_dAMRate, double dDepth)
{
    char szAMType[2][15] = {"LINear", "EXPonential"};

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:AM1:TYPE %s\n",szAMType[in_ucAMType]);
    if (vistatus)
    {
        return DP_SIGGEN_AM_SRC_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"LFO2:FREQ %lf\n", in_dAMRate);
    if (vistatus)
    {
        return DP_SIGGEN_AM_SCALING_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:AM:DEPT:LINEAR %lf\n", dDepth);
    if (vistatus)
    {
        return DP_SIGGEN_AM_DEPTH_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigFMEnabled(unsigned char in_ucSGNo, unsigned char in_ucSource, unsigned int uiFmEnabled)
{
    char szPMSource[3][30] = {"INTernal","EXTernal","INT,EXT"};

    vistatus = ((uiFmEnabled) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:FM:STATe ON\n") : viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:FM:STATe OFF\n");
    if (vistatus)
    {
        return DP_SIGGEN_RFFM_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"AM:SOUR %s\n", szPMSource[in_ucSource]);
    if (vistatus)
    {
        return DP_SIGGEN_MOD_SRC_FAILED;
    }


    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigFMExtCoupling(unsigned char in_ucSGNo,unsigned int in_uiFmExternalCoupling)
{
    vistatus = ((in_uiFmExternalCoupling) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:FM:EXTernal:COUPling AC\n") : viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FM:EXTernal:COUPling DC\n");
    if (vistatus)
    {
        return DP_SIGGEN_RFFM_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigFM(unsigned char in_ucSGNo, unsigned char in_ucMode, double in_dFMRate, double in_dDeviation)
{
    char szFMMode[3][15] = {"NORMal","LNOise","HDEViation"};

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"FM:MODE %s\n", szFMMode[in_ucMode]);
    if (vistatus)
    {
        return DP_SIGGEN_MOD_CONFIG_FAILED;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FM:DEViation %lf\n", in_dDeviation);
    if (vistatus)
    {
        return DP_SIGGEN_MOD_DEV_FAILED;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"LFO2:FREQ %lf\n", in_dFMRate);
    if (vistatus)
    {
        return DP_SIGGEN_MOD_SRC_FAILED;
    }

    return DP_SIGGEN_SUCCESS;
}
/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPMEnabled ()
// Author					:	 kannan.S
// Global Variables affected:	 NA
// Created Date				:    26-03-09
// Revision Date			:
// Reason for Revising		:
// Description				:	This function configures the signal generator to apply pulse modulation to the RF output signal.
******************************************************************************************************/
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPulmMEnabled  (unsigned char in_ucSGNo, bool bPmEnabled)
{
    vistatus = ((bPmEnabled) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)"SOURce:PULM:STATe ON\n") : viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PULM:STATe OFF\n");
    if (vistatus)
    {
        return DP_SIGGEN_RF_PHSMOD_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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

short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPMEnabled(unsigned char in_ucSGNo, bool bPmEnabled, unsigned char in_ucSource)
{
    char szPMSource[3][15] = {"INTernal","EXTernal","INT,EXT"};

    vistatus = ((bPmEnabled) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)"SOURce:PM:STATe ON\n") : viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PM:STATe OFF\n");
    if (vistatus)
    {
        return DP_SIGGEN_RF_PHSMOD_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"PM:SOUR %lf\n", szPMSource[in_ucSource]);
    if (vistatus)
    {
        return DP_SIGGEN_MOD_SRC_FAILED;
    }

    return DP_SIGGEN_SUCCESS;
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

short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPMExtCoupling  (unsigned char in_ucSGNo, unsigned int in_uiPmExternalCoupling)
{
    vistatus = ((in_uiPmExternalCoupling) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:PM:EXTernal:COUPling AC\n") : viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:PM:EXTernal:COUPling DC\n");
    if (vistatus)
    {
        return DP_SIGGEN_PHSMOD_EXTSRC_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPM(unsigned char in_ucSGNo, unsigned char in_ucMode, double dFreq, double dDeviation)
{
    char szPhMMode[3][15] = {"NORMal","LNOise","HDEViation"};

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"FM:MODE %s\n", szPhMMode[in_ucMode]);
    if (vistatus)
    {
        return DP_SIGGEN_MOD_CONFIG_FAILED;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"LFO2:FREQ %lf\n",dFreq);
    if (vistatus)
    {
        return DP_SIGGEN_PHSMOD_DEV_FAILED;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:PM:DEViation %lf\n",dDeviation);
    if (vistatus)
    {
        return DP_SIGGEN_PHSMOD_DEV_FAILED;
    }

    // Index..To be Add

    return DP_SIGGEN_SUCCESS;
}
/***************************************************************************************************
// Name						:	 DP_Enumeration_RFSigGen_ConfigPM  ()
// Author					:	 kannan.S
// Global Variables affected:	 NA
// Created Date				:    26-03-09
// Revision Date			:
// Reason for Revising		:
// Description				:	Configures pulse generator settings for pulse Modulation
******************************************************************************************************/
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPulGen(unsigned char in_ucSGNo)
{
    /*Set Pulse Period*/
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PULM:PERiod 10 us\n");
    if (vistatus)
    {
        return DP_SIGGEN_PUL_PERIOD_FAILED;
    }

    /*Set pulse width*/
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PULM:WIDth 8 us\n");
    if (vistatus)
    {
        return DP_SIGGEN_PUL_WIDTH_FAILED;
    }

    /*Set double pulse width*/
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PULM:DOUBle:WIDTh 0.0000012\n");
    if (vistatus)
    {
        return DP_SIGGEN_DPUL_WIDTH_FAILED;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PULM:DOUBle:DELay 0.0000045\n");
    if (vistatus)
    {
        return DP_SIGGEN_PUL_DELAY_FAILED;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPulOutput(unsigned char in_ucSGNo,unsigned int in_uiPolarity, bool bPulseModulationEnabled)
{
    char szPulmPol[2][15] = {"NORmal","INVerted"};

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"PULM:POL %s\n",szPulmPol[in_uiPolarity]);
    if (vistatus)
    {
        return DP_SIGGEN_PUL_POL_FAILED;
    }

    vistatus = ((bPulseModulationEnabled) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:PULM:STATe ON\n"): viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:PULM:STATe OFF\n");
    if (vistatus)
    {
        return DP_SIGGEN_PULMOD_OPSIG_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPulMSource (unsigned char in_ucSGNo,unsigned int in_uiSource)
{
    char szPULMode[3][15] = {"INTernal", "EXTernal"};
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:PULM:SOURce %s\n",szPULMode[in_uiSource]);
    if (vistatus)
    {
        return DP_SIGGEN_RFOP_PULMOD_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:PULM:STATe ON\n");
    if (vistatus)
    {
        return DP_SIGGEN_RFOP_PULMOD_FAILURE;
    }
    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPulMExtPolarity(unsigned char in_ucSGNo,unsigned int in_uiExternalPolarity)
{
    char szPULExtMode[2][15] = {"NORMal", "INVerted"};

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PULM:POLarity %s\n",szPULExtMode[in_uiExternalPolarity]);
    if (vistatus)
    {
        return DP_SIGGEN_EXT_POL_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_ConfigLFGen(unsigned char in_ucSGNo,unsigned int in_uiSrcIndex,unsigned int in_uiSrcActive,double dFrequency,unsigned int in_uiWaveform)
{
    char szLFGenMode[9][15] = {"SINE", "SQUare", "TRIangle", "SAWTooth", "ISAWtooth" };
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LFOutput:FREQuency %lf\n",dFrequency);
    if (vistatus)
    {
        return DP_SIGGEN_LFOP_FREQ_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LFOutput:SHAPe %d\n",szLFGenMode[in_uiWaveform]);
    if (vistatus)
    {
        return DP_SIGGEN_LFOP_WFM_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigLFGenOutput(unsigned char in_ucSGNo,double dAmplitude, bool bEnabled)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LFOutput:VOLT %lf\n",dAmplitude);
    if (vistatus)
    {
        return DP_SIGGEN_LFOP_VOLT_FAILURE;
    }

    vistatus = ((bEnabled) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:LFOutput:STATe ON\n"): viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LFOutput:STATe OFF\n");
    if (vistatus)
    {
        return DP_SIGGEN_LFGEN_ENA_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPulIntTrg(unsigned char in_ucSGNo, double dPeriod)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:PULM:INTernal:PERiod:STEP:INCRement %lf\n",dPeriod);
    if (vistatus)
    {
        return DP_SIGGEN_INT_TRIG_FAILURE;
    }
    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPulTrig(unsigned char in_ucSGNo, unsigned int in_uiTriggerSource, double dPulseWidth, bool bGatingEnabled)
{
    char szSwepTrigMode[6][15] = {"AUTO", "IMMediate", "EGATe", "SINGLe"};

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PULM:TRIGger:MODE %s\n", szSwepTrigMode[in_uiTriggerSource]);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_TRIG_SRC_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}

short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPulDble(unsigned char in_ucSGNo, bool bEnabled, double dDelay)
{
    char szSwepTrigMode[6][15] = {"SING", "DOUB", "PTRain"};

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOURce:PULM:MODE %d\n",szSwepTrigMode[bEnabled]);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_TRIG_SRC_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigSweep(unsigned char in_ucSGNo, unsigned int in_uiMode, unsigned int in_uiTriggerSource)
{

    if((m_arrSInstHandle[in_ucSGNo-1].m_usInstID == KS_E8257D) || (m_arrSInstHandle[in_ucSGNo-1].m_usInstID == RS_SMB100A))
    {
        char szFreqMode[4][15] = { "CW", "FIXed", "SWEep", "LIST"};
        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:FREQuency:MODE %s\n", szFreqMode[in_uiMode]);
        if (vistatus)
        {
            return DP_SIGGEN_CFG_SWMODE_FAILURE;
        }
        char szSwepTrigMode[6][15] = {"AUTO", "IMMediate", "SINGLe", "BUS", "EXTernal", "EAUTo"};
        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":TRIGger:FSWeep:SOURce %s\n", szSwepTrigMode[in_uiTriggerSource]);
        if (vistatus)
        {
            return DP_SIGGEN_CFG_TRIG_SRC_FAILURE;
        }
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_FreqSweepStrtStp(unsigned char in_ucSGNo, double dStart, double dStop)
{
    if((m_arrSInstHandle[in_ucSGNo-1].m_usInstID == KS_E8257D) || (m_arrSInstHandle[in_ucSGNo-1].m_usInstID == RS_SMB100A))
    {
        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SOURce:FREQuency:STARt %lf\n", dStart);
        if (vistatus)
        {
            return DP_SIGGEN_CFG_FREQ_SWEEP_FAILURE;
        }

        vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:STOP %lf\n", dStop);
        if (vistatus)
        {
            return DP_SIGGEN_CFG_FREQ_SWEEP_FAILURE;
        }
    }

    return DP_SIGGEN_SUCCESS;
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

short CSGWrapper::DP_Enumeration_RFSigGen_FreqSweepCenterSpan(unsigned char in_ucSGNo, double dCenter, double dSpan)
{

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:CENTer %lf\n", dCenter);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_CNTR_FREQ_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:SPAN %lf\n", dSpan);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_SPAN_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;

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
short CSGWrapper::DP_Enumeration_RFSigGen_FreqSweepTime(unsigned char in_ucSGNo, double dTime)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:DWELl %lf\n", dTime);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_DURA_FS_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;

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
short CSGWrapper::DP_Enumeration_RFSigGen_PowerSweepStrtStp(unsigned char in_ucSGNo, double dStart, double dStop)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWER:MODE SWEep\n");
    if (vistatus)
    {
        return DP_SIGGEN_CFG_PWR_SWEEP_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWER:STARt %lf\n", dStart);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_START_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWER:STOP %lf\n", dStop);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_STOP_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_PowerSweepTime(unsigned char in_ucSGNo, double dTime)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:DWEL %lf\n", dTime);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_DURA_PS_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
*******************************************************************************************************/
short CSGWrapper::DP_Enumeration_RFSigGen_FreqStepStrtStp(unsigned char in_ucSGNo, double dstart, double dstop, double dDwell, unsigned int in_uiNoPoints)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:MODE SWEep\n");
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_MODE_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":TRIGger:FSWeep:SOURce IMMediate\n");
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FS_SRC_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:STARt %lf\n", dstart);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_START_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:STOP %lf\n", dstop);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_STOP_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:POW:DWELl %lf\n", dDwell);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_PWR_DWELL_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:POWer:POINts %d\n", in_uiNoPoints);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_PWR_PNTS_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;

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
short CSGWrapper::DP_Enumeration_ConfigFreqStepDwell(unsigned char in_ucSGNo,bool bEnabled, double dDwell)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:POW:DWELl %lf\n",dDwell);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_PWR_DWELL_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_PwrStepStrtStp(unsigned char in_ucSGNo, double dStart, double dStop, unsigned int in_uiNoPoints)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWER:MODE SWEep\n");
    if (vistatus)
    {
        return DP_SIGGEN_CFG_PWR_SWEEP_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWER:STARt %lf\n", dStart);
    if (vistatus)
    {
        return DP_SIGGEN_PWR_START_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWER:STOP %lf\n", dStop);
    if (vistatus)
    {
        return DP_SIGGEN_PWR_STOP_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:POW:POINts %d\n", in_uiNoPoints);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_PWR_PNTS_FAILURE;
    }
    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigPowerStepDwell(unsigned char in_ucSGNo,bool bEnabled, double dDwell)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:POW:DWELl %lf\n", dDwell);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_PWR_DWELL_FAILURE;
    }
    return DP_SIGGEN_SUCCESS;
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

short CSGWrapper::DP_Enumeration_RFSigGen_ConfigRefOsc(unsigned char in_ucSGNo,unsigned int in_uiSource, double dFrequency)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:ROSCillator:SOURce:INTernal");
    if (vistatus)
    {
        return DP_SIGGEN_CFG_REF_OSC_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
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
short CSGWrapper::DP_Enumeration_RFSigGen_ConfigALC(unsigned char in_ucSGNo,unsigned int in_uiSource, double dBandwidth)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWer:ALC ON\n");
    if (vistatus)
    {
        return DP_SIGGEN_CFG_ALC_FAILURE;
    }

    char szALCMode[4][15] = {"INTernal", "DIODe", "MMHead"};
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWer:ALC:SOURce %d\n", szALCMode[in_uiSource]);
    if (vistatus)
    {
        return DP_SIGGEN_ALC_SRC_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:POWer:ALC:BANDwidth %lf\n", dBandwidth);
    if (vistatus)
    {
        return DP_SIGGEN_ALC_BW_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}

long CSGWrapper::DP_Enumeration_RFSigGen_SelfTest(unsigned char in_ucSGNo, unsigned int uiTimedelay, short *out_sStatus)
{
    ViInt32 viTimeDelay = 0;
    viTimeDelay = uiTimedelay;

    viSetAttribute(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,VI_ATTR_TMO_VALUE,viTimeDelay);
    vistatus = viQueryf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"*TST?\n", "%d", out_sStatus);
    if(vistatus)
    {
        return DP_SIGGEN_SLF_TST_QUERY_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}

long CSGWrapper::ConfigFreqStepSweep(unsigned char in_ucSGNo, double in_dStartFreq, double in_dStopFreq,  unsigned int in_uiNoPoints, double in_dDwellTime )
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:MODE SWEep\n");
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_MODE_FAILURE;
    }
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":TRIGger:FSWeep:SOURce IMMediate\n");
    if (vistatus)
    {
        return DP_SIGGEN_TRIG_SRC_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:STARt %lf\n", in_dStartFreq);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_START_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:FREQuency:STOP %lf\n", in_dStopFreq);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_STOP_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:FREQuency:DWELl %lf\n", in_dDwellTime);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_DWELL_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:SWEep:FREQuency:POINts %d\n", in_uiNoPoints);
    if (vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_PNTS_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}

short CSGWrapper::EnableDisableFreqSweep(unsigned char in_ucSGNo, bool in_bEnaDisable)
{
    vistatus = ((in_bEnaDisable) > 0) ? viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession,(char*)":SWEep:CONTrol:STATe ON\n"): viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SWEep:CONTrol:STATe OFF\n");
    if (vistatus)
    {
        return DP_SIGGEN_FREQ_SWEEP_ENADIS_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}
//To Disable all modulation
short CSGWrapper::DisableAllModulation(unsigned char in_ucSGNo)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":MODulation:STATe OFF\n");
    if(vistatus)
    {
        return DP_SIGGEN_DIS_MOD_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}
//Configuration of List with Frequency and Power
short CSGWrapper::Config_ListConfiguration(unsigned char in_ucSGNo, double *in_dFreq, double *in_dPwr, float *fDwellTime, unsigned char in_ucListCount)
{
    char szListDwell[255] = {0};
    char szListFreq[255] = {0};
    char szListPwr[255] = {0};
    char szFreq[255] = {0};
    char szPwr[255] = {0};
    char szDwell[255] = {0};
    double dFreq[20] = {0.0};
    double dPwr[20] = {0.0};
    unsigned char ucCount = 0;
#if 0
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LIST:MODE AUTO");
    if(vistatus)
    {
        return DP_SIGGEN_LST_MODE_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOUR1:LIST:SEL New_list\n");
    if(vistatus)
    {
        return DP_SIGGEN_LST_SEL_FAILURE;
    }

    for(ucCount = 0; ucCount < in_ucListCount; ucCount++)
    {
        dFreq[ucCount] = in_dFreq[ucCount];
        dPwr[ucCount] = in_dPwr[ucCount];
        if(ucCount == 0)
        {
            sprintf(szListFreq, "%0.3lf", dFreq[ucCount]);
            sprintf(szListPwr,  "%0.2lf", dPwr[ucCount]);
            sprintf(szListDwell,"%0.4f",  fDwellTime[ucCount]);
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
#endif
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LIST:FREQuency %s\n", szListFreq);
    if(vistatus)
    {
        return DP_SIGGEN_LIST_FREQ_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LIST:POWer %s\n", szListPwr);
    if(vistatus)
    {
        return DP_SIGGEN_LIST_PWR_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LIST:DWEL %s\n", szListDwell);
    if(vistatus)
    {
        return DP_SIGGEN_LIST_DWELL_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOUR:LIST:TRIG:SOUR SIN\n");
    if(vistatus)
    {
        return DP_SIGGEN_LIST_TRIG_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOUR1:FREQ:MODE LIST\n");
    if(vistatus)
    {
        return DP_SIGGEN_LST_MODE_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOUR1:LIST:TRIG:EXEC\n");
    if(vistatus)
    {
        return DP_SIGGEN_LIST_TRIG_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)"SOUR1:FREQ:MODE CW\n");
    if(vistatus)
    {
        return DP_SIGGEN_LIST_DWELL_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}

long CSGWrapper::Config_List_Dwell(unsigned char in_ucSGNo, char *strListDwell)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURce:LIST:DWEL %s\n", strListDwell);
    if(vistatus)
    {
        return DP_SIGGEN_LIST_DWELL_FAILURE;
    }
    return DP_SIGGEN_SUCCESS;
}

short CSGWrapper::Config_ListSweep(unsigned char in_ucSGNo)
{
    //To Configure in list
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:LIST:TYPE LIST\n");
    if(vistatus)
    {
        return DP_SIGGEN_CFG_LIST_TYPE_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":INIT:CONT:ALL 0\n");
    if(vistatus)
    {
        return DP_SIGGEN_CONT_INIT_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:LIST:DIR UP\n");
    if(vistatus)
    {
        return DP_SIGGEN_CFG_LIST_DIR_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:LIST:SWEEP:TRIGGER:TYPE FRUN\n");
    if(vistatus)
    {
        return DP_SIGGEN_LIST_TRIG_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:LIST:DWELL:TYPE LIST\n"); //To Configure Dwell Type
    if(vistatus)
    {
        return DP_SIGGEN_CFG_LIST_DWELL_TYPE_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:FREQ:MODE SWEep\n"); ////To On the Frequency Sweep
    if(vistatus)
    {
        return DP_SIGGEN_CFG_FREQ_MODE_FAILURE;
    }

    QThread::msleep(500);

    return DP_SIGGEN_SUCCESS;
}

short CSGWrapper::Run_Single_Sweep(unsigned char in_ucSGNo)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:POWER:MODE LIST\n");
    if (vistatus)
    {
        return DP_SIGGEN_PWR_MODE_LIST_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:LIST:SWEEP:TRIGGER:TYPE FRUN\n"); //To Make Sigle Sweep
    if (vistatus)
    {
        return DP_SIGGEN_LIST_TRIG_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":INIT:CONT:ALL OFF\n");
    if(vistatus)
    {
        return DP_SIGGEN_CONT_INIT_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":INIT:IMM:ALL\n");
    if (vistatus)
    {
        return DP_SIGGEN_INIT_IMM_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:TSWEEP\n");
    if (vistatus)
    {
        return DP_SIGGEN_CFG_SWMODE_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}

short CSGWrapper::Run_Continuous_Sweep(unsigned char in_ucSGNo)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:POWER:MODE LIST\n");
    if(vistatus)
    {
        return DP_SIGGEN_PWR_MODE_LIST_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:LIST:SWEEP:TRIGGER:TYPE FRUN\n");
    if(vistatus)
    {
        return DP_SIGGEN_LIST_TRIG_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":INIT:IMM:ALL\n");
    if(vistatus)
    {
        return DP_SIGGEN_INIT_IMM_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":SOURCE:TSWEEP\n");
    if(vistatus)
    {
        return DP_SIGGEN_CFG_SWMODE_FAILURE;
    }

    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":INIT:CONT:ALL ON\n");
    if(vistatus)
    {
        return DP_SIGGEN_CONT_INIT_FAILURE;
    }

    return DP_SIGGEN_SUCCESS;
}

short CSGWrapper::Stop_Continuous_Sweep(unsigned char in_ucSGNo)
{
    vistatus = viPrintf(m_arrSInstHandle[in_ucSGNo-1].m_viSGSession, (char*)":INITiate:CONTinuous:ALL OFF\n");
    if(vistatus)
    {
        return DP_SIGGEN_CONT_INIT_FAILURE;
    }
    return DP_SIGGEN_SUCCESS;
}

S32BIT CSGWrapper:: DP_SIGGEN_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize)
{
    if(out_ps8ErrStringBuf == NULL)
        return DP_SIGGEN_ERR_INVALID_PARAM;
#if 0
    switch(in_s32ErrCode)
    {
    case DP_SIGGEN_SUCCESS:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Signal Generator Open Success!");
        break;

    case DP_SIGGEN_OPEN_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Signal Generator Open Failure!");
        break;

    case DP_SIGGEN_INT_ID_READ_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Signal Generator Model Number Read Failure!");
        break;

    case DP_SIGGEN_INT_INVALID_MODEL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Signal GeneratorIbvalid Model!");
        break;

    case DP_SIGGEN_CFG_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Configuration Failed!");
        break;

    case DP_SIGGEN_FREQ_CNFG_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Frequency Configuration Failed!");
        break;

    case DP_SIGGEN_PWRLVL_CNFG_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Power Level Configuration Failed!");
        break;

    case DP_SIGGEN_ALC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Automatic Level Control Enable Failed!");
        break;

    case DP_SIGGEN_PRST_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Signal Generator Preset Failed!");
        break;

    case DP_SIGGEN_DISCONN_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Signal Generator Disconnect Failed!");
        break;

    case DP_SIGGEN_RFOP_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "RF Output Signal Enable or Disable Failed!");
        break;

    case DP_SIGGEN_RFAM_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Apply Amplitude Modulation to RF Output Signal Failed!");
        break;

    case DP_SIGGEN_AM_SRC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Amplitude Modulation Source Failed!");
        break;

    case DP_SIGGEN_AM_SCALING_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Amplitude Modulation Scaling Failed!");
        break;

    case DP_SIGGEN_AM_DEPTH_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Amplitude Modulation Depth Failed!");
        break;

    case DP_SIGGEN_RFFM_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Apply Frequency Modulation to RF Output Signal Failed!");
        break;

    case DP_SIGGEN_MOD_DEV_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Modulation Deviation to FM Failed!");
        break;

    case DP_SIGGEN_MOD_SRC_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Modulation Source to FM Failed!");
        break;

    case DP_SIGGEN_RF_PHSMOD_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Apply Phase Modulation to RF Output Signal Failed!");
        break;

    case DP_SIGGEN_PHSMOD_EXTSRC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "External Source for Phase Modulation Failed!");
        break;

    case DP_SIGGEN_PUL_PERIOD_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Pulse Modulation setting Pulse period Failed!");
        break;

    case DP_SIGGEN_PUL_WIDTH_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Pulse Modulation Deviation to RF Output Signal Failed!");
        break;

    case DP_SIGGEN_DPUL_WIDTH_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Pulse Modulation Deviation to RF Output Signal Failed!");
        break;

    case DP_SIGGEN_PUL_DELAY_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Pul Modulation Deviation to RF Output Signal Failed!");
        break;

    case DP_SIGGEN_PHSMOD_DEV_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Phase Modulation Deviation to RF Output Signal Failed!");
        break;

    case DP_SIGGEN_PULMOD_OPSIG_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Apply Pulse Modulation to RF Output Signal Failed!");
        break;

    case DP_SIGGEN_PUL_POL_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Pulse Polarity Failed!");
        break;

    case DP_SIGGEN_RFOP_PULMOD_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Pulse Modulation Source for RF Output Signal Failed!");
        break;

    case DP_SIGGEN_EXT_POL_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "External Source for Pulse Modulation Polarity Failed!");
        break;

    case DP_SIGGEN_LFOP_FREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the LFGenerator Output Frequency Failed!");
        break;

    case DP_SIGGEN_LFOP_WFM_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the LFGenerator Output Waveform Failed!");
        break;

    case DP_SIGGEN_LFOP_VOLT_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring LFOutput Voltage Failed!");
        break;

    case DP_SIGGEN_LFGEN_ENA_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "LF Generator Output Enable Failed!");
        break;

    case DP_SIGGEN_INT_TRIG_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Repetition rate of the pulse generator using Interanal Trigger Failed!");
        break;

    case DP_SIGGEN_CFG_SWMODE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Sweep mode for Signal Generator Failed!");
        break;

    case DP_SIGGEN_CFG_TRIG_SRC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Triger Source for Signal Generator Failed!");
        break;

    case DP_SIGGEN_CFG_FREQ_SWEEP_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Start and Stop Frequency for Frequency Sweep Failed!");
        break;

    case DP_SIGGEN_CFG_CNTR_FREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Center Frequency For Frequency Sweep Failed!");
        break;

    case DP_SIGGEN_CFG_SPAN_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Span For Frequency Sweep Failed!");
        break;

    case DP_SIGGEN_CFG_DURA_FS_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Duration of One Frequency Sweep Failed!");
        break;

    case DP_SIGGEN_CFG_PWR_SWEEP_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Power Sweep Failed!");
        break;

    case DP_SIGGEN_CFG_START_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the start power for a power sweep Failed!");
        break;

    case DP_SIGGEN_CFG_STOP_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the stop power for a power sweep Failed!");
        break;

    case DP_SIGGEN_CFG_DURA_PS_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Duration of One Power Sweep Failed!");
        break;

    case DP_SIGGEN_CFG_FREQ_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Frequency Sweep Mode Failed!");
        break;

    case DP_SIGGEN_CFG_FS_SRC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Duration of One Frequency Sweep Failed!");
        break;

    case DP_SIGGEN_CFG_FREQ_START_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Frequency Start Failed!");
        break;

    case DP_SIGGEN_CFG_FREQ_STOP_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Frequency Stop Failed!");
        break;

    case DP_SIGGEN_CFG_PWR_DWELL_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Power Dwell for Step Frequency Failed!");
        break;

    case DP_SIGGEN_CFG_PWR_PNTS_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Power Point for Step Frequency Failed!");
        break;

    case DP_SIGGEN_PWR_START_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Power Mode Start Failed!");
        break;

    case DP_SIGGEN_PWR_STOP_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Power Mode Stop Failed!");
        break;

    case DP_SIGGEN_CFG_REF_OSC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Refrence Oscillator for Signal Generator Failed!");
        break;

    case DP_SIGGEN_CFG_ALC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Automatic Level Control Failed!");
        break;

    case DP_SIGGEN_ALC_SRC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Source for Automatic Level Control Failed!");
        break;

    case DP_SIGGEN_ALC_BW_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Bandwidth for Automatic Level Control Failed!");
        break;

    case DP_SIGGEN_SLF_TST_QUERY_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Self Test Query Failed!");
        break;

    case DP_SIGGEN_TRIG_SRC_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Frequency Sweep Triggrer Source Failed!");
        break;

    case DP_SIGGEN_CFG_FREQ_DWELL_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Frequency Dwell Failed!");
        break;

    case DP_SIGGEN_CFG_FREQ_PNTS_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Frequency Points Failed!");
        break;

    case DP_SIGGEN_FREQ_SWEEP_ENADIS_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring Frequency Sweep Enable Disable Failed!");
        break;

    case DP_SIGGEN_DIS_MOD_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Disable All Modulation Failed!");
        break;

    case DP_SIGGEN_LST_MODE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring List Mode Failed!");
        break;

    case DP_SIGGEN_LST_SEL_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Creating a New list Failed!");
        break;

    case DP_SIGGEN_LIST_FREQ_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the List with Frequency Failed!");
        break;

    case DP_SIGGEN_LIST_PWR_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the List with Power Failed!");
        break;

    case DP_SIGGEN_LIST_DWELL_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the List with Dwell Failed!");
        break;

    case DP_SIGGEN_LIST_TRIG_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Triggering the List Mode Failed!");
        break;

    case DP_SIGGEN_CFG_LIST_TYPE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the List Type Failed!");
        break;

    case DP_SIGGEN_CONT_INIT_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Intiate All Continuous power Measurement Failed!");
        break;

    case DP_SIGGEN_CFG_LIST_DIR_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the List Type Directory Failed!");
        break;

    case DP_SIGGEN_CFG_LIST_DWELL_TYPE_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the List Dwell Type Failed!");
        break;

    case DP_SIGGEN_PWR_MODE_LIST_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Configuring the Power Mode List Type Failed!");
        break;

    case DP_SIGGEN_INIT_IMM_FAILURE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Intiate All Immediate power Measurement Failed!");
        break;

    case DP_SIGGEN_MOD_CONFIG_FAILED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Mode Configuration for Frequency Modulation Failed!");
        break;

    case DP_SIGGEN_INVALID_SGNO:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid signal generator number");
        break;

    default:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid Error Code !");
    }
#endif
    return DP_SIGGEN_SUCCESS;
}

short CSGWrapper::GetInstID(char *in_pcInstDetails, unsigned short *out_pusModel)
{

    unsigned char ucIdx = 0;
    bool bCmpRtn = 0;
    char *pszTok = NULL;
    char *pszModel = NULL;

    pszTok = strtok(in_pcInstDetails, ",");
    pszModel = strtok(NULL, " ,");

    for(ucIdx = 0; ucIdx < g_ucSGCount;  ucIdx++)
    {
        bCmpRtn = strcasecmp(pszModel, m_InstCfg[ucIdx].szModel);

        if (bCmpRtn == 0)
        {
            *out_pusModel = m_InstCfg[ucIdx].usInstID;
            return DP_SIGGEN_SUCCESS;
        }
    }

    return DP_SIGGEN_INT_INVALID_MODEL;
}


