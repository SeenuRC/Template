#include "FGWrapper.h"
#include "src/DP_datastructure.h"

extern unsigned char g_ucFGCount;
CFGWrapper::CFGWrapper()
{
    m_viStatus = 0;
    memset(m_arrSFGInstHandle, 0 , sizeof(m_arrSFGInstHandle));
    memset(m_arrSFGInsconfg, 0 , sizeof(m_arrSFGInsconfg));
}


/**
*\brief			This function's purpose is to open function generator.
*
*\param[in] 	in_u8InstNo                    It specifies the instrument number
*\param[in] 	pLogicalName                   It specifies the IP Address of the function generator
*
*\retval		::DP_FG_OPEN_FAILURE	is returned when instrument open is failed
*\retval		::DP_FG_INT_ID_READ_FAILED	is returned when instrument ID cannot be fetched
*\retval		::DP_FG_INT_INVALID_MODEL is returned upon when invalid model is detected
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::Open(unsigned char in_u8InstNo, char *pLogicalName)
{
    ViChar value[128]  = {0};
    ViSession viSessionHandle    =  0;
    ViSession viInstSession      =  0;
    unsigned char ucLoop         =  0;
    unsigned short usInstID      =  0;
    unsigned int uiReadCnt       =  0;
    char arrucipadress[100]        = {0};
    char szInstDetails [256]     = {0};
    char arrucTemp[256]             = {0};
    strcpy(value,pLogicalName);

    m_viStatus = viOpenDefaultRM(&viSessionHandle);
    if(m_viStatus)
    {
        return DP_FG_OPEN_FAILURE;
    }

    sprintf(arrucipadress, "%s", pLogicalName);
    m_viStatus = viOpen(viSessionHandle, arrucipadress, VI_NULL, VI_NULL, &viInstSession);

    if(m_viStatus)
    {
        return DP_FG_OPEN_FAILURE;
    }

    for( ucLoop = 8; ucLoop <= 18; ucLoop++)
    {
        arrucTemp[ucLoop-8] = value[ucLoop];
    }

    //    strcpy(gGlobalStuct.sCOTSInstInfo.m_cSAInfo[in_u8InstNo-1], ucTemp);

    /*Read Instrument Type*/

    m_viStatus = viPrintf(viInstSession, (char*)"*IDN?\n");
    if(m_viStatus)
    {
        return DP_FG_INT_ID_READ_FAILED;
    }

    m_viStatus = viBufRead(viInstSession, (ViPBuf)szInstDetails,256, (ViPUInt32)&uiReadCnt);
    if(m_viStatus)
    {
        return DP_FG_INT_ID_READ_FAILED;
    }  

    /*Read the Model ID*/
    m_viStatus = GetInstID(szInstDetails, &usInstID);

    if(m_viStatus != 0)
    {
        viClose(viInstSession);

        return DP_FG_INT_INVALID_MODEL;
    }
    else
    {
        m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession = viInstSession;
        m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID = usInstID;
        m_arrSFGInstHandle[in_u8InstNo-1].m_u8Instance = in_u8InstNo;
    }

    return DP_FG_CNFG_SUCCESS;

}

/**
*\brief			This function's purpose is to compare instrument model with compatible instrument model.
*
*\param[in] 	in_pcInstDetails                    It specifies the instrument IP address
*\param[out] 	out_pusModel                        It specifies the intrument model ID
*
*\retval		::DP_FG_IP_FILE_NOT_EXISTS	is returned when FG input file doesn't exists
*\retval		::DP_FG_COUNT_NOT_EXISTS	is returned when FG count doesn't exists in input file
*\retval		::DP_FG_INVALID_FG_CNT_FORMAT is returned upon when FG count value is invalid format
*\retval		::DP_FG_INT_INVALID_MODEL is returned upon when invalid model is detected
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::GetInstID(char *in_pcInstDetails, unsigned short *out_pusModel)
{
    bool bIsValid       = false;
    unsigned char ucIdx = 0;
    bool bCmpRtn = 0;
    char *pszTok = NULL;
    char *pszModel = NULL;

    pszTok = strtok(in_pcInstDetails, ",");
    pszModel = strtok(NULL, " ,");

    for(ucIdx = 0; ucIdx < g_ucFGCount;  ucIdx++)
    {
        bCmpRtn = strcasecmp(pszModel, m_arrSFGInsconfg[ucIdx].m_szmodel);

        if (bCmpRtn == 0)
        {
            *out_pusModel = m_arrSFGInsconfg[ucIdx].m_usInstID;
            return DP_FG_CNFG_SUCCESS;
        }
    }

    return DP_FG_INT_INVALID_MODEL;
}


/**
*\brief			This function's purpose is to preset the function generator.
*
*\param[in] 	in_u8InstNo                    It specifies the instrument number
*
*\retval		::DP_FG_RESET_FAILURE is returned when preset failed in function generator
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_Reset(U8BIT in_u8InstNo)
{
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession,(char*)"*RST\n");

    if(m_viStatus)
    {
        return DP_FG_RESET_FAILURE;
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure waveform shape for specified channel
*\param[in] 	in_u8InstNo                      It specifies the instrument number
*\param[in] 	in_u8ChnSel                      It specifies the function generator channel selection
*\param[in] 	in_u8ShapeSel                    It specifies the waveform shape selection
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_INVALID_WAVEFORM_SHAPE_CFG is returned upon when invalid waveform shape configuration
*\retval		::DP_FG_WAVEFORM_SHAPE_CFG_FAILURE is returned upon when waveform shape configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_ConfigureWaveformShape(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8ShapeSel)
{
    QString qstrCommand;
    QStringList qstrListWaveformShape;

    qstrCommand.clear();
    qstrListWaveformShape.clear();



    /*Validations*/
    if(in_u8ShapeSel > DP_FG_WAVEFORM_EFILE)
    {
        return DP_FG_INVALID_WAVEFORM_SHAPE_CFG;
    }
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        qstrListWaveformShape <<"SINusoid"<<"SQUare"<<"PULSe"<<"RAMP"<<"PRNoise"<<"DC"<<"SINC"<<"GAUSsian"<<"LORentz"<<"ERISe"<<"EDECay"<<"HAVersine"<<"EMEMory[1]"<<"EMEMory2"<<"EFILe";
        /*For CH-1 or CH-2 Only*/
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:FUNCtion:SHAPe %s\n",in_u8ChnSel, qstrListWaveformShape.at(in_u8ShapeSel).toStdString().c_str());

            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_WAVEFORM_SHAPE_CFG_FAILURE;
            }
        }
        else/*For All channels*/
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:FUNCtion:SHAPe %s\n", (in_u8ChnSel & 0x1), qstrListWaveformShape.at(in_u8ShapeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_WAVEFORM_SHAPE_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:FUNCtion:SHAPe %s\n",(in_u8ChnSel & 0x2), qstrListWaveformShape.at(in_u8ShapeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_WAVEFORM_SHAPE_CFG_FAILURE;
            }
        }
    }
    else if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == SDG2042X) /*Added by Balamurali*/
    {
        qstrListWaveformShape <<"PULSE";
        qstrCommand = QString::asprintf("C%d:BSWV WVTP,%s\n", in_u8ChnSel, qstrListWaveformShape.at(in_u8ShapeSel).toStdString().c_str());
        m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
        if(m_viStatus)
        {
            return DP_FG_VOLTAGE_CFG_FAILURE;
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to enable/disable output for specified channel
*\param[in] 	in_u8InstNo                        It specifies the instrument number
*\param[in] 	in_u8ChnSel                        It specifies the function generator channel selection
*\param[in] 	in_bEnDisOutput                    It specifies output enable/disable selection
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_OUTPUT_EN_DIS_FAILURE is returned while enabling/disabling FG output failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_EnableOutput(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bEnDisOutput)
{
    QString qstrCommand;
    QStringList qstrListOPState;

    qstrListOPState.clear();
    qstrCommand.clear();

    qstrListOPState<<"OFF"<<"ON";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        /*For CH-1 or CH-2 Only*/
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("OUTPut%d:STATe %s\n",in_u8ChnSel, qstrListOPState.at(in_bEnDisOutput).toStdString().c_str());

            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_OUTPUT_EN_DIS_FAILURE;
            }
        }
        else/*For All channels*/
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("OUTPut%d:STATe %s\n", (in_u8ChnSel & 0x1), qstrListOPState.at(in_bEnDisOutput).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_OUTPUT_EN_DIS_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("OUTPut%d:STATe %s\n", (in_u8ChnSel & 0x2), qstrListOPState.at(in_bEnDisOutput).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_OUTPUT_EN_DIS_FAILURE;
            }
        }
    }
    else if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == SDG2042X)
    {
        qstrCommand = QString::asprintf("C%d:OUTP %s\n",in_u8ChnSel, qstrListOPState.at(in_bEnDisOutput).toStdString().c_str());

        m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
        if(m_viStatus)
        {
            return DP_FG_OUTPUT_EN_DIS_FAILURE;
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure polarity for specified channel
*\param[in] 	in_u8InstNo                        It specifies the instrument number
*\param[in] 	in_u8ChnSel                        It specifies the function generator channel selection
*\param[in] 	in_bPolaritySel                    It specifies the polarity selection(Normal/Inverted)
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_POLARITY_CFG_FAILURE is returned upon when polarity configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_SetPolarity(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bPolaritySel)
{
    QString qstrCommand;
    QStringList qstrListPolarityState;

    qstrListPolarityState.clear();
    qstrCommand.clear();

    qstrListPolarityState <<"NORMal"<<"INVerted";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("OUTPut%d:POLarity %s\n", in_u8ChnSel, qstrListPolarityState.at(in_bPolaritySel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_POLARITY_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("OUTPut%d:POLarity %s\n", (in_u8ChnSel & 0x1), qstrListPolarityState.at(in_bPolaritySel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_POLARITY_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("OUTPut%d:POLarity %s\n", (in_u8ChnSel & 0x2), qstrListPolarityState.at(in_bPolaritySel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_POLARITY_CFG_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure impedance for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_u8ImpedanceSel                    It specifies the impedance selection(Minimum/Maximum/Infinity)
*\param[in] 	in_dImpedance                        It specifies the impedance value to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_INVALID_IMPEDANCE_SEL	is returned upon invalid impedance selection
*\retval		::DP_FG_IMPEDANCE_CFG_FAILURE is returned upon when impedance configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_SetImpedance(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8ImpedanceSel, FDOUBLE in_dImpedance)
{
    /*NOTE SCPI for High Z Mode to be checked manually*/

    QString qstrCommand;
    QStringList qstrListImpedanceSel;
    QStringList qstrListImpedanceSel_SIGLENT;

    qstrCommand.clear();
    qstrListImpedanceSel.clear();

    qstrListImpedanceSel <<"MINimum"<<"MAXimum"<<"INFinity";
    qstrListImpedanceSel_SIGLENT <<"HZ";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8ImpedanceSel > DP_FG_IMPEDANCE_INFINITY)
    {
        return DP_FG_INVALID_IMPEDANCE_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        /*Allows the user to configure required impedance value*/
        if(in_dImpedance != DP_FG_IMPEDANCE_AUTO)
        {
            if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
            {
                qstrCommand = QString::asprintf("OUTPut%d:IMPedance %lf\n", in_u8ChnSel, in_dImpedance);
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_IMPEDANCE_CFG_FAILURE;
                }
            }
            else
            {
                /*For CH-1*/
                qstrCommand = QString::asprintf("OUTPut%d:IMPedance %lf\n", (in_u8ChnSel & 0x1), in_dImpedance);
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_IMPEDANCE_CFG_FAILURE;
                }

                /*For CH-2*/
                qstrCommand = QString::asprintf("OUTPut%d:IMPedance %lf\n", (in_u8ChnSel & 0x2), in_dImpedance);
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_IMPEDANCE_CFG_FAILURE;
                }
            }
        }
        else/*Allows the user to configure default impedance value available in the function generator(Min/Max/Infinity)*/
        {
            if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
            {
                qstrCommand = QString::asprintf("OUTPut%d:IMPedance %s\n", in_u8ChnSel, qstrListImpedanceSel.at(in_u8ImpedanceSel).toStdString().c_str());
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_IMPEDANCE_CFG_FAILURE;
                }
            }
            else
            {
                /*For CH-1*/
                qstrCommand = QString::asprintf("OUTPut%d:IMPedance %s\n", (in_u8ChnSel & 0x1), qstrListImpedanceSel.at(in_u8ImpedanceSel).toStdString().c_str());
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_IMPEDANCE_CFG_FAILURE;
                }

                /*For CH-2*/
                qstrCommand = QString::asprintf("OUTPut%d:IMPedance %s\n", (in_u8ChnSel & 0x2), qstrListImpedanceSel.at(in_u8ImpedanceSel).toStdString().c_str());
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_IMPEDANCE_CFG_FAILURE;
                }
            }
        }
    }
    else if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == SDG2042X)
    {
        qstrCommand = QString::asprintf("C%d:OUTP LOAD, %s\n", in_u8ChnSel, qstrListImpedanceSel_SIGLENT.at(in_dImpedance).toStdString().c_str());
        m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
        if(m_viStatus)
        {
            return DP_FG_IMPEDANCE_CFG_FAILURE;
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure duty cycle for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_dDutyCycle                        It specifies the duty cycle to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_INVALID_DCYCLE	is returned upon invalid duty cycle value configuration
*\retval		::DP_FG_DUTYCYCLE_CFG_FAILURE is returned upon when duty cycle configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgPulseDutyCycle(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dDutyCycle)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }

    if(in_dDutyCycle < 0.00 || in_dDutyCycle > 100.00)
    {
        return DP_FG_INVALID_DCYCLE;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:PULSe:DCYCle %lf\n", in_u8ChnSel, in_dDutyCycle);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_DUTYCYCLE_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:DCYCle %lf\n", (in_u8ChnSel & 0x1), in_dDutyCycle);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_DUTYCYCLE_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:DCYCle %lf\n", (in_u8ChnSel & 0x2), in_dDutyCycle);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_DUTYCYCLE_CFG_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure pulse delay for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_dDelay                            It specifies the pulse delay to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_PULSE_DELAY_CFG_FAILURE	is returned upon when pulse delay configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgPulseDelay(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dDelay)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:PULSe:DELay %lf\n", in_u8ChnSel, in_dDelay);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_DELAY_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:DELay %lf\n", (in_u8ChnSel & 0x1), in_dDelay);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_DELAY_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:DELay %lf\n", (in_u8ChnSel & 0x2), in_dDelay);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_DELAY_CFG_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}


/**
*\brief			This function's purpose is to configure pulse delay for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_dPulsePeriod                      It specifies the pulse period to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_PULSE_PERIOD_CFG_FAILURE	is returned upon when pulse period configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgPulsePeriod(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dPulsePeriod)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:PULSe:PERiod %lf ms\n", in_u8ChnSel, in_dPulsePeriod);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_PERIOD_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:PERiod %lf ms\n", (in_u8ChnSel & 0x1), in_dPulsePeriod);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_PERIOD_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:PERiod %lf ms\n", (in_u8ChnSel & 0x2), in_dPulsePeriod);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_PERIOD_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure pulse width for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_dPulseWidth                       It specifies the pulse width to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_PULSE_WIDTH_CFG_FAILURE	is returned upon when pulse width configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgPulseWidth(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dPulseWidth)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:PULSe:WIDTh %lf ms\n", in_u8ChnSel, in_dPulseWidth);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_WIDTH_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:WIDTh %lf ms\n", (in_u8ChnSel & 0x1), in_dPulseWidth);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_WIDTH_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:WIDTh %lf ms\n", (in_u8ChnSel & 0x2), in_dPulseWidth);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_WIDTH_CFG_FAILURE;
            }
        }
    }
    else if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == SDG2042X)
    {
        qstrCommand = QString::asprintf("C%d:BSWV WIDTH,%lf\n", in_u8ChnSel, in_dPulseWidth*1.0e-3);
        m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
        if(m_viStatus)
        {
            return DP_FG_VOLTAGE_CFG_FAILURE;
        }
    }

    return DP_FG_CNFG_SUCCESS;

}

/**
*\brief			This function's purpose is to configure trigger mode
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_bTriggerMode                      It specifies trigger mode selection in function generator(Trigger/Sync)
*
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_TRIGGER_MODE_CFG_FAILURE	is returned upon when low level voltage configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgTriggerMode(U8BIT in_u8InstNo, bool in_bTriggerMode)
{
    QString qstrCommand;
    QStringList qstrListTriggerMode;
    qstrCommand.clear();
    qstrListTriggerMode.clear();

    qstrListTriggerMode <<"TRIGger"<<"SYNC";
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        qstrCommand = QString::asprintf("OUTPut:TRIGger:MODE %s\n", qstrListTriggerMode.at(in_bTriggerMode).toStdString().c_str());
        m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
        if(m_viStatus)
        {
            return DP_FG_TRIGGER_MODE_CFG_FAILURE;
        }

    }

    return DP_FG_CNFG_SUCCESS;
}


/**
*\brief			This function's purpose is to configure high level for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_dHighLevelVal                     It specifies the high level voltage to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_HIGH_LVL_CFG_FAILURE	is returned upon when high level voltage configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgHighLevel(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dHighLevelVal)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", in_u8ChnSel, in_dHighLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LVL_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", (in_u8ChnSel & 0x1), in_dHighLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LVL_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", (in_u8ChnSel & 0x2), in_dHighLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LVL_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;

}

/**
*\brief			This function's purpose is to configure low level voltage for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_dLowLevelVal                     It specifies the low level voltage to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_LOW_LVL_CFG_FAILURE	is returned upon when low level voltage configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgLowLevel(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dLowLevelVal)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:LOW %lf V\n", in_u8ChnSel, in_dLowLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_LOW_LVL_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:LOW %lf V\n", (in_u8ChnSel & 0x1), in_dLowLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_LOW_LVL_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:LOW %lf V\n", (in_u8ChnSel & 0x2), in_dLowLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_LOW_LVL_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}


/**
*\brief			This function's purpose is to configure high level and low level voltage for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_dHighLevelVal                     It specifies the high level voltage to be configured in the function generator
*\param[in] 	in_dLowLevelVal                     It specifies the low level voltage to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_HIGH_LOW_LVL_CFG_FAILURE	is returned upon when high level and low level configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgHighLowLvl(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dHighLevelVal, FDOUBLE in_dLowLevelVal)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            /*High Lvl*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", in_u8ChnSel, in_dHighLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LOW_LVL_CFG_FAILURE;
            }

            /*Low Lvl*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:LOW %lf V\n", in_u8ChnSel, in_dLowLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LOW_LVL_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1 High Lvl*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", (in_u8ChnSel & 0x1), in_dHighLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LOW_LVL_CFG_FAILURE;
            }

            /*For CH-2 High Lvl*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", (in_u8ChnSel & 0x2), in_dHighLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LOW_LVL_CFG_FAILURE;
            }

            /*For CH-1 Low Lvl*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:LOW %lf V\n", (in_u8ChnSel & 0x1), in_dLowLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LOW_LVL_CFG_FAILURE;
            }

            /*For CH-2 Low Lvl*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:LOW %lf V\n", (in_u8ChnSel & 0x2), in_dLowLevelVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HIGH_LOW_LVL_CFG_FAILURE;
            }
        }
    }
    else if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == SDG2042X)
    {
        qstrCommand = QString::asprintf("C%d:BSWV HLEV,%lfV,LLEV,%lfV\n", in_u8ChnSel, in_dHighLevelVal,in_dLowLevelVal);
        m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
        if(m_viStatus)
        {
            return DP_FG_VOLTAGE_CFG_FAILURE;
        }

    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure offset voltage for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_dOffsetVal                        It specifies the voltage offset to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_OFFSET_CFG_FAILURE	is returned upon when voltage offset configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgOffsetVoltage(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dOffsetVal)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:OFFSet %lf mV\n", in_u8ChnSel, in_dOffsetVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_OFFSET_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:OFFSet %lf mV\n", (in_u8ChnSel & 0x1), in_dOffsetVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_OFFSET_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:OFFSet %lf mV\n", (in_u8ChnSel & 0x2), in_dOffsetVal);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_OFFSET_CFG_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to close function generator
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_CLOSE_FAILURE	is returned upon when instrument close is failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_Close(U8BIT in_u8InstNo)
{
    /*Validations*/
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    m_viStatus = viClose(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession);
    if(m_viStatus)
    {
        return DP_FG_CLOSE_FAILURE;
    }

    return DP_FG_CNFG_SUCCESS;
}


/**
*\brief			This function's purpose is to hold either pulse width or pulse duty for specified channel
*\param[in] 	in_u8InstNo                          It specifies the instrument number
*\param[in] 	in_u8ChnSel                          It specifies the function generator channel selection
*\param[in] 	in_bHoldSel                          It specifies the pulse hold selection(Width/Duty)
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_PULSE_HOLD_FAILURE	is returned upon when pulse hold configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgPulseHold(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bHoldSel)
{
    QString qstrCommand;
    QStringList qstrListHoldMode;
    qstrCommand.clear();
    qstrListHoldMode.clear();

    qstrListHoldMode <<"WIDTh"<<"DUTY";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:PULSe:HOLD %s\n", in_u8ChnSel, qstrListHoldMode.at(in_bHoldSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_HOLD_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:HOLD %s\n", (in_u8ChnSel & 0x1), qstrListHoldMode.at(in_bHoldSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_HOLD_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:HOLD %s\n", (in_u8ChnSel & 0x2), qstrListHoldMode.at(in_bHoldSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_HOLD_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure burst mode for specified channel
*\param[in] 	in_u8InstNo                               It specifies the instrument number
*\param[in] 	in_u8ChnSel                               It specifies the function generator channel selection
*\param[in] 	in_bBurstModeSel                          It specifies the burst mode selection(Triggered/Gated)
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_BURST_MODE_CFG_FAILURE	is returned upon when burst mode configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgBurstMode(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bBurstModeSel)
{
    QString qstrCommand;
    QStringList qstrListBurstMode;
    qstrCommand.clear();
    qstrListBurstMode.clear();

    qstrListBurstMode <<"TRIGgered"<<"GATed";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:BURSt:MODE %s\n", in_u8ChnSel, qstrListBurstMode.at(in_bBurstModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_MODE_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:BURSt:MODE %s\n", (in_u8ChnSel & 0x1), qstrListBurstMode.at(in_bBurstModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_MODE_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:BURSt:MODE %s\n", (in_u8ChnSel & 0x2), qstrListBurstMode.at(in_bBurstModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_MODE_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure burst cycles for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_u8BurstCycleModeSel                          It specifies the burst cycle mode selection(Minimum/Maximum/Infinity)
*\param[in] 	in_u32NoOfCycles                                It specifies the number of cycles to be configured in function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_INVALID_BURST_MODE_SEL	is returned upon invalid burst mode selection
*\retval		::DP_FG_BURST_CYCLE_CFG_FAILURE	is returned upon when burst cycle configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgBurstCycles(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8BurstCycleModeSel, U32BIT in_u32NoOfCycles)
{
    QString qstrCommand;
    QStringList qstrListBurstCycle;

    qstrCommand.clear();
    qstrListBurstCycle.clear();

    qstrListBurstCycle <<"MINimum"<<"MAXimum"<<"INFinity";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }
    if(in_u8BurstCycleModeSel > DP_FG_BURST_CYCLE_INFINITY)
    {
        return DP_FG_INVALID_BURST_MODE_SEL;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u32NoOfCycles != DP_FG_BURST_CYCLE_AUTO)
        {
            if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
            {
                qstrCommand = QString::asprintf("SOURce%d:BURSt:NCYCles %u\n", in_u8ChnSel, in_u32NoOfCycles);
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_BURST_CYCLE_CFG_FAILURE;
                }
            }
            else
            {
                /*For CH-1*/
                qstrCommand = QString::asprintf("SOURce%d:BURSt:NCYCles %u\n", (in_u8ChnSel & 0x1), in_u32NoOfCycles);
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_BURST_CYCLE_CFG_FAILURE;
                }

                /*For CH-2*/
                qstrCommand = QString::asprintf("SOURce%d:BURSt:NCYCles %u\n", (in_u8ChnSel & 0x2), in_u32NoOfCycles);
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_BURST_CYCLE_CFG_FAILURE;
                }
            }
        }
        else
        {
            if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
            {
                qstrCommand = QString::asprintf("SOURce%d:BURSt:NCYCles %s\n", in_u8ChnSel, qstrListBurstCycle.at(in_u8BurstCycleModeSel).toStdString().c_str());
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_BURST_CYCLE_CFG_FAILURE;
                }
            }
            else
            {
                /*For CH-1*/
                qstrCommand = QString::asprintf("SOURce%d:BURSt:NCYCles %s\n", (in_u8ChnSel & 0x1), qstrListBurstCycle.at(in_u8BurstCycleModeSel).toStdString().c_str());
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_BURST_CYCLE_CFG_FAILURE;
                }

                /*For CH-2*/
                qstrCommand = QString::asprintf("SOURce%d:BURSt:NCYCles %s\n", (in_u8ChnSel & 0x2), qstrListBurstCycle.at(in_u8BurstCycleModeSel).toStdString().c_str());
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_BURST_CYCLE_CFG_FAILURE;
                }
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}


/**
*\brief			This function's purpose is to enable/disable burst in function generator
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_bBurstEnDis                                  It specifies the burst enable/disable selection
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_BURST_ENDIS_FAILURE	is returned upon when burst mode enable/disable failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success


*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_EnableBurstMode(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bBurstEnDis)
{
    QString qstrCommand;

    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:BURSt:STATe %d\n", in_u8ChnSel, in_bBurstEnDis);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_EN_DIS_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:BURSt:STATe %d\n", (in_u8ChnSel & 0x1), in_bBurstEnDis);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_EN_DIS_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:BURSt:STATe %d\n", (in_u8ChnSel & 0x2), in_bBurstEnDis);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_EN_DIS_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure burst idle mode for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_u8BurstModeSel                               It specifies the burst idle mode selection(Start/DC/End)
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_BURST_IDLE_CFG_FAILURE	is returned upon when burst idle mode configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgBurstIdle(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8BurstModeSel)
{
    QString qstrCommand;
    QStringList qstrListBurstMode;

    qstrListBurstMode.clear();
    qstrCommand.clear();

    qstrListBurstMode<<"START"<<"DC"<<"END";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }
    if(in_u8BurstModeSel > DP_FG_BURST_IDLE_STOP)
    {
        return DP_FG_INVALID_BURST_IDLE_MODE_SEL;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:BURSt:IDLE %s\n", in_u8ChnSel, qstrListBurstMode.at(in_u8BurstModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_IDLE_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:BURSt:IDLE %s\n", (in_u8ChnSel & 0x1), qstrListBurstMode.at(in_u8BurstModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_IDLE_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:BURSt:IDLE %s\n", (in_u8ChnSel & 0x2), qstrListBurstMode.at(in_u8BurstModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_BURST_IDLE_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure center frequency for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_dCenterFrequency                             It specifies the center frequency to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_CENTER_FREQ_CFG_FAILURE	is returned upon when center frequency configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          14 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgCenterFrequency(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dCenterFrequency)
{
    QString qstrCommand;

    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:CENTer %lf\n", in_u8ChnSel, in_dCenterFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_CENTER_FREQ_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:CENTer %lf\n", (in_u8ChnSel & 0x1), in_dCenterFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_CENTER_FREQ_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:CENTer %lf\n", (in_u8ChnSel & 0x2), in_dCenterFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_CENTER_FREQ_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure frequency for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_bFreqModeSel                                 It specifies the burst idle mode selection(CW/Fixed)
*\param[in] 	in_dFrequency                                   It specifies the frequency to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_FREQ_CFG_FAILURE	is returned upon when frequency configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgFrequency(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bFreqModeSel, FDOUBLE in_dFrequency)
{
    QString qstrCommand;
    QStringList qstrListFreqMode;

    qstrListFreqMode.clear();
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    qstrListFreqMode<<"CW"<<"FIXed";

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:%s %lf MHz\n", in_u8ChnSel, qstrListFreqMode.at(in_bFreqModeSel).toStdString().c_str(), in_dFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_FREQ_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:%s %lf MHz\n", (in_u8ChnSel & 0x1), qstrListFreqMode.at(in_bFreqModeSel).toStdString().c_str(), in_dFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_FREQ_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:%s %lf MHz\n", (in_u8ChnSel & 0x2), qstrListFreqMode.at(in_bFreqModeSel).toStdString().c_str(), in_dFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_FREQ_CFG_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure frequency mode for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_u8FreqModeSel                                It specifies the frequency mode selection(CW/Fixed/Sweep)
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_INVALID_FREQ_MODE_SEL	is returned upon when invalid frequency mode is configured
*\retval		::DP_FG_FREQ_MODE_CFG_FAILURE	is returned upon when frequency mode configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgFrequencyMode(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8FreqModeSel)
{
    QString qstrCommand;
    QStringList qstrListFreqMode;

    qstrListFreqMode.clear();
    qstrCommand.clear();

    qstrListFreqMode<<"CW"<<"FIXed"<<"SWEep";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }
    if(in_u8FreqModeSel > DP_FG_FREQ_SWEEP)
    {
        return DP_FG_INVALID_FREQ_MODE_SEL;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:MODE %s\n", in_u8ChnSel, qstrListFreqMode.at(in_u8FreqModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_FREQ_MODE_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:MODE %s\n", (in_u8ChnSel & 0x1), qstrListFreqMode.at(in_u8FreqModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_FREQ_MODE_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:MODE %s\n", (in_u8ChnSel & 0x2), qstrListFreqMode.at(in_u8FreqModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_FREQ_MODE_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure span for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_dCenterFrequency                             It specifies the center frequency to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_SPAN_CFG_FAILURE	is returned upon when span configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_ConfigureSpan(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dCenterFrequency)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:SPAN %lf MHz\n", in_u8ChnSel, in_dCenterFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SPAN_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:SPAN %lf MHz\n", (in_u8ChnSel & 0x1), in_dCenterFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SPAN_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:SPAN %lf MHz\n", (in_u8ChnSel & 0x2), in_dCenterFrequency);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SPAN_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure start & stop frequency for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_dStartFreq                                   It specifies the start frequency to be configured in the function generator
*\param[in] 	in_dStopFreq                                    It specifies the stop frequency to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_START_STOP_CFG_FAILURE	is returned upon when start and stop frequency configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgStartStopFreq(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dStartFreq, FDOUBLE in_dStopFreq)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:STARt %lf MHz\n", in_u8ChnSel, in_dStartFreq);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_START_STOP_CFG_FAILURE;
            }

            qstrCommand = QString::asprintf("SOURce%d:FREQuency:STOP %lf MHz\n", in_u8ChnSel, in_dStopFreq);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_START_STOP_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1 Start Freq*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:STARt %lf MHz\n", (in_u8ChnSel & 0x1), in_dStartFreq);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_START_STOP_CFG_FAILURE;
            }

            /*For CH-1 Stop Freq*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:STOP %lf MHz\n", (in_u8ChnSel & 0x1), in_dStopFreq);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_START_STOP_CFG_FAILURE;
            }

            /*For CH-2 Start Freq*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:STARt %lf MHz\n", (in_u8ChnSel & 0x2), in_dStartFreq);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_START_STOP_CFG_FAILURE;
            }

            /*For CH-2 Stop Freq*/
            qstrCommand = QString::asprintf("SOURce%d:FREQuency:STOP %lf MHz\n", (in_u8ChnSel & 0x2), in_dStopFreq);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_START_STOP_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure sweep hold time for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_dHoldTime                                    It specifies the sweep hold time to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_HOLD_TIME_CFG_FAILURE	is returned upon when sweep hold time configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgSweepHoldTime(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dHoldTime)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:SWEep:HTIMe %lf ms\n", in_u8ChnSel, in_dHoldTime);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HOLD_TIME_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:SWEep:HTIMe %lf ms\n", (in_u8ChnSel & 0x1), in_dHoldTime);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HOLD_TIME_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:SWEep:HTIMe %lf ms\n", (in_u8ChnSel & 0x2), in_dHoldTime);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_HOLD_TIME_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure sweep mode for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_bSweepModeSel                                It specifies the sweep mode selection to be configured in the function generator(Auto/Manual)
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_SWEEP_MODE_CFG_FAILURE	is returned upon when sweep mode configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgSweepMode(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, bool in_bSweepModeSel)
{
    QString qstrCommand;
    QStringList qstrListSweepMode;
    qstrCommand.clear();
    qstrListSweepMode.clear();

    qstrListSweepMode <<"AUTO"<<"MANual";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:SWEep:MODE %s\n", in_u8ChnSel, qstrListSweepMode.at(in_bSweepModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SWEEP_MODE_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:SWEep:MODE %s\n", (in_u8ChnSel & 0x1), qstrListSweepMode.at(in_bSweepModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SWEEP_MODE_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:SWEep:MODE %s\n", (in_u8ChnSel & 0x2), qstrListSweepMode.at(in_bSweepModeSel).toStdString().c_str());
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SWEEP_MODE_CFG_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}

/**
*\brief			This function's purpose is to configure sweep time for specified channel
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_dSweepTime                                   It specifies the sweep time to be configured in the function generator(Auto/Manual)
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_SWEEP_TIME_CFG_FAILURE	is returned upon when sweep time configuration failed
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success
*
*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgSweepTime(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dSweepTime)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:SWEep:TIME %lf ms\n", in_u8ChnSel, in_dSweepTime);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SWEEP_TIME_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:SWEep:TIME %lf ms\n", (in_u8ChnSel & 0x1), in_dSweepTime);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SWEEP_TIME_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:SWEep:TIME %lf ms\n", (in_u8ChnSel & 0x2), in_dSweepTime);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_SWEEP_TIME_CFG_FAILURE;
            }
        }
    }
    return DP_FG_CNFG_SUCCESS;
}


/**
*\brief			This function's purpose is to configure duty cycle and pulse period in function generator.
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_dSweepTime                                   It specifies the sweep time to be configured in the function generator
*\param[in] 	in_dPulsePeriod                                 It specifies the pulse period to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	is returned upon invalid instrument number
*\retval		::DP_FG_INVALID_DCYCLE	is returned upon invalid duty cycle value configuration
*\retval		::DP_FG_DUTYCYCLE_CFG_FAILURE	is returned upon when duty cycle configuration fails
*\retval		::DP_FG_PULSE_PERIOD_CFG_FAILURE is returned upon when pulse period configuration fails
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success

*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_CfgPulse(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dDutyCycle, FDOUBLE in_dPulsePeriod)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }
    if(in_dDutyCycle < 0.00 || in_dDutyCycle > 100.00)
    {
        return DP_FG_INVALID_DCYCLE;
    }

    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            if(in_dDutyCycle != 0)
            {
                qstrCommand = QString::asprintf("SOURce%d:PULSe:DCYCle %lf\n", in_u8ChnSel, in_dDutyCycle);
                m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
                if(m_viStatus)
                {
                    return DP_FG_DUTYCYCLE_CFG_FAILURE;
                }
            }

            qstrCommand = QString::asprintf("SOURce%d:PULSe:PERiod %0.6lf ms\n", in_u8ChnSel, in_dPulsePeriod);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_PERIOD_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1 DCycle*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:DCYCle %lf\n", (in_u8ChnSel & 0x1), in_dDutyCycle);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_DUTYCYCLE_CFG_FAILURE;
            }

            /*For CH-2 DCycle*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:DCYCle %lf\n", (in_u8ChnSel & 0x2), in_dDutyCycle);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_DUTYCYCLE_CFG_FAILURE;
            }

            /*For CH-1 Pulse Period*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:PERiod %lf ms\n", (in_u8ChnSel & 0x1), in_dPulsePeriod);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_PERIOD_CFG_FAILURE;
            }

            /*For CH-2 Pulse Period*/
            qstrCommand = QString::asprintf("SOURce%d:PULSe:PERiod %lf ms\n", (in_u8ChnSel & 0x2), in_dPulsePeriod);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_PULSE_PERIOD_CFG_FAILURE;
            }
        }
    }
    else if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == SDG2042X)
    {
        qstrCommand = QString::asprintf("C%d:BSWV PERI,%lf\n", in_u8ChnSel, in_dPulsePeriod*1.0e-3);
        m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
        if(m_viStatus)
        {
            return DP_FG_VOLTAGE_CFG_FAILURE;
        }
    }
    return DP_FG_CNFG_SUCCESS;
}


/**
*\brief			This function's purpose is to configure voltage in function generator.
*\param[in] 	in_u8InstNo                                     It specifies the instrument number
*\param[in] 	in_u8ChnSel                                     It specifies the function generator channel selection
*\param[in] 	in_dVoltage                                     It specifies the voltage to be configured in the function generator
*
*\retval		::DP_FG_INVALID_CH_SEL	is returned upon invalid channel selection
*\retval		::DP_FG_INVALID_INST_NUM	 is returned upon invalid instrument number
*\retval		::DP_FG_VOLATAGE_CFG_FAILURE is returned upon when voltage configuration fails
*\retval		::DP_FG_CNFG_SUCCESS	is returned upon success

*\author        Yogeswaran
*\date          15 May 2024
*/
S16BIT CFGWrapper::DP_FG_SetDCVoltage(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, FDOUBLE in_dVoltage)
{
    QString qstrCommand;
    qstrCommand.clear();

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }
    if(m_arrSFGInstHandle[in_u8InstNo-1].m_usInstID == TKX_AFG31102)
    {
        if(in_u8ChnSel == DP_FG_CH1 || in_u8ChnSel == DP_FG_CH2)
        {
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", in_u8ChnSel, in_dVoltage);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_VOLTAGE_CFG_FAILURE;
            }
        }
        else
        {
            /*For CH-1*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", (in_u8ChnSel & 0x1), in_dVoltage);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_VOLTAGE_CFG_FAILURE;
            }

            /*For CH-2*/
            qstrCommand = QString::asprintf("SOURce%d:VOLTage:LEVel:IMMediate:HIGH %lf V\n", (in_u8ChnSel & 0x2), in_dVoltage);
            m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
            if(m_viStatus)
            {
                return DP_FG_VOLTAGE_CFG_FAILURE;
            }
        }
    }

    return DP_FG_CNFG_SUCCESS;
}

/*SIGLENT Configure Basic Mode*/
S16BIT CFGWrapper::DP_FG_Config(U8BIT in_u8InstNo, U8BIT in_u8ChnSel, U8BIT in_u8ShapeSel,FDOUBLE in_dPeriod, FDOUBLE in_dWidth, FDOUBLE in_dHighLevel,FDOUBLE in_dLowLevel)
{
    QString qstrCommand;
    qstrCommand.clear();
    QStringList qstrListWaveformShape;

//    qstrListWaveformShape <<"SINusoid"<<"SQUare"<<"PULSE"<<"RAMP"<<"PRNoise"<<"DC"<<"SINC"<<"GAUSsian"<<"LORentz"<<"ERISe"<<"EDECay"<<"HAVersine"<<"EMEMory[1]"<<"EMEMory2"<<"EFILe";
    qstrListWaveformShape <<"PULSE";

    /*Validations*/
    if(in_u8ChnSel > DP_FG_ALLCHN || in_u8ChnSel == DP_FG_ZERO)
    {
        return DP_FG_INVALID_CH_SEL;
    }
    if(in_u8InstNo > DP_FG_MAX_INST_NO || in_u8InstNo == DP_FG_ZERO)
    {
        return DP_FG_INVALID_INST_NUM;
    }
    /*Validations*/
    if(in_u8ShapeSel > DP_FG_WAVEFORM_EFILE)
    {
        return DP_FG_INVALID_WAVEFORM_SHAPE_CFG;
    }

    qstrCommand = QString::asprintf("C%d:BSWV WVTP,%s,PERI,%lf,HLEV,%lfV,LLEV,%lfV,WIDTH,%lf\n", in_u8ChnSel, qstrListWaveformShape.at(in_u8ShapeSel).toStdString().c_str(),in_dPeriod*1.0e-3,in_dHighLevel,in_dLowLevel,in_dWidth*1.0e-3);
    m_viStatus = viPrintf(m_arrSFGInstHandle[in_u8InstNo-1].m_viFGSession, (char*)qstrCommand.toStdString().c_str());
    if(m_viStatus)
    {
        return DP_FG_VOLTAGE_CFG_FAILURE;
    }

    return DP_FG_CNFG_SUCCESS;
}


/**************************************************************************
// Name						:	 DP_FG_GetErrorMessage()
// Author					:	 Yogeswaran S
// Global Variables affected:	 NA
// Created Date				:    14-05-2024
// Revision Date			:
// Reason for Revising		:
// Description				:	This function's purpose is to return error message.
**************************************************************************/
S16BIT CFGWrapper::DP_FG_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize)
{
    if(out_ps8ErrStringBuf == NULL)
    {
        return DP_FG_ERR_INVALID_PARAM;
    }

    switch(in_s32ErrCode)
    {
    case DP_FG_OPEN_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to open function generator");
        break;
    }
    case DP_FG_INVALID_INST_NUM:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid instrument number");
        break;
    }
    case DP_FG_INT_ID_READ_FAILED:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to read instrument ID");
        break;
    }
    case DP_FG_INVALID_CH_SEL:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid channel selection");
        break;
    }
    case DP_FG_INT_INVALID_MODEL:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Incompatible function generator detected");
        break;
    }
    case DP_FG_RESET_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to preset function generator");
        break;
    }
    case DP_FG_WAVEFORM_SHAPE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure waveform shape in Function Generator");
        break;
    }
    case DP_FG_OUTPUT_EN_DIS_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to enable/disable output in Function Generator");
        break;
    }
    case DP_FG_POLARITY_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure polarity in Function Generator");
        break;
    }
    case DP_FG_IMPEDANCE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure impedance in Function Generator");
        break;
    }
    case DP_FG_DUTYCYCLE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure duty cycle in Function Generator");
        break;
    }

    case DP_FG_PULSE_DELAY_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure delay in Function Generator");
        break;
    }
    case DP_FG_PULSE_PERIOD_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure pulse period in Function Generator");
        break;
    }
    case DP_FG_PULSE_WIDTH_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure pulse width in Function Generator");
        break;
    }
    case DP_FG_TRIGGER_MODE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure trigger mode in Function Generator");
        break;
    }
    case DP_FG_HIGH_LVL_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure high level in Function Generator");
        break;
    }
    case DP_FG_LOW_LVL_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure low level in Function Generator");
        break;
    }
    case DP_FG_HIGH_LOW_LVL_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure high level and low level in Function Generator");
        break;
    }
    case DP_FG_OFFSET_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure voltage offset in Function Generator");
        break;
    }
    case DP_FG_CLOSE_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to close Function Generator");
        break;
    }
    case DP_FG_PULSE_HOLD_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure pulse hold in Function Generator");
        break;
    }
    case DP_FG_AM_SOURCE_SEL_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure AM Source selection in Function Generator");
        break;
    }
    case DP_FG_AM_OUT_ENDIS_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to enable/disable AM OUT in Function Generator");
        break;
    }
    case DP_FG_BURST_MODE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure Burst mode in Function Generator");
        break;
    }
    case DP_FG_BURST_CYCLE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure burst cycle in Function Generator");
        break;
    }
    case DP_FG_BURST_EN_DIS_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to enable/disable burst OUT in Function Generator");
        break;
    }
    case DP_FG_BURST_IDLE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure burst idle mode in Function Generator");
        break;
    }
    case DP_FG_CENTER_FREQ_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure center frequency in Function Generator");
        break;
    }
    case DP_FG_FREQ_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure frequency in Function Generator");
        break;
    }
    case DP_FG_FREQ_MODE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure frequency mode in Function Generator");
        break;
    }
    case DP_FG_SPAN_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure span in Function Generator");
        break;
    }
    case DP_FG_START_STOP_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure start stop frequency in Function Generator");
        break;
    }
    case DP_FG_HOLD_TIME_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure sweep hold time in Function Generator");
        break;
    }
    case DP_FG_SWEEP_MODE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure sweep mode in Function Generator");
        break;
    }
    case DP_FG_SWEEP_TIME_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure sweep time in Function Generator");
        break;
    }
    case DP_FG_INVALID_IMPEDANCE_SEL:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid impedance selection in Function Generator");
        break;
    }
    case DP_FG_INVALID_BURST_MODE_SEL:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid burst mode selection in Function Generator");
        break;
    }
    case DP_FG_INVALID_BURST_IDLE_MODE_SEL:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid burst idle mode selection in Function Generator");
        break;
    }
    case DP_FG_INVALID_FREQ_MODE_SEL:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid frequency mode selection");
        break;
    }
    case DP_FG_COUNT_NOT_EXISTS:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"FG count doesn't exists in instrument details file");
        break;
    }
    case DP_FG_INVALID_FG_CNT_FORMAT:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid FG Count format");
        break;
    }
    case DP_FG_INVALID_DCYCLE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Duty cycle should be greater than 0 and less than or equal to 100");
        break;
    }
    case DP_FG_VOLTAGE_CFG_FAILURE:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Unable to configure voltage in function generator");
        break;
    }
    case DP_FG_IP_FILE_NOT_EXISTS:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Input file for FG configuration does not exists");
        break;
    }
    default:
    {
        snprintf(out_ps8ErrStringBuf,in_u16BufSize,"Invalid FG Error Code!");
        break;
    }
    }
    return DP_FG_CNFG_SUCCESS;
}


S16BIT CFGWrapper::GetInstrumentDetails()
{
    bool bIsValid       = false;
    unsigned char ucIdx = 0;
    QString strTempPath             = "";
    QString strFilePath             = "";
    QSettings *pObjSettings = NULL;
    QString strTemp;

    strTemp.clear();
    strTemp.clear();

    strTempPath = qApp->applicationDirPath();
    strFilePath = strTempPath + "/INPUT_FILES/" + "RFInstrumentDetails.ini";

    QFileInfo objfileInfo(strFilePath);
    if (objfileInfo.exists() && objfileInfo.isFile())
    {
        pObjSettings = new(QSettings)(strFilePath, QSettings::IniFormat);
    }
    else
    {
        return DP_FG_IP_FILE_NOT_EXISTS;
    }

    strTemp = QString::asprintf("INST_CONFIG/TOT_FG_COUNT");

    if(pObjSettings->value(strTemp).toString().isEmpty())
    {
        return DP_FG_COUNT_NOT_EXISTS;
    }

    strTemp = pObjSettings->value(strTemp).toString();

    m_ucAvailFGCount = strTemp.toUInt(&bIsValid , 10);
    if(bIsValid == false)
    {
        return DP_FG_INVALID_FG_CNT_FORMAT;
    }

    for(ucIdx = 0; ucIdx < m_ucAvailFGCount; ucIdx++)
    {
        strTemp = QString::asprintf("INST_CONFIG/FG_%d_MODEL", ucIdx + 1);

        if(pObjSettings->value(strTemp).toString().isEmpty())
        {
            return DP_FG_COUNT_NOT_EXISTS;
        }

        strcpy(m_arrSFGInsconfg[ucIdx].m_szmodel, pObjSettings->value(strTemp).toString().toStdString().c_str());

        strTemp = QString::asprintf("INST_CONFIG/FG_%d_ID",ucIdx + 1);
        if(pObjSettings->value(strTemp).toString().isEmpty())
        {
            return DP_FG_COUNT_NOT_EXISTS;
        }

        m_arrSFGInsconfg[ucIdx].m_usInstID = pObjSettings->value(strTemp).toString().toUInt();

        if(pObjSettings->value(strTemp).toString().isEmpty())
        {
            return DP_FG_COUNT_NOT_EXISTS;
        }
    }

    return DP_FG_CNFG_SUCCESS;
}


