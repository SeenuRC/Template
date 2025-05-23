#ifndef CTESTCASEWRAPPER_H
#define CTESTCASEWRAPPER_H

#include "mainwindow.h"

class CTestCaseWrapper
{
public:
    CTestCaseWrapper();

    short ApplyCalibData(unsigned int in_uiPathID, double in_dFrequency, double in_dUncalibPower, double *out_dCalibPower, double *out_dCableloss);

    void DP_SG_SA_GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg);
    S32BIT DP_SG_SA_Initialization(S_SG_SA_INOUT_PARAMETERS in_sInOutParam);
    S32BIT DP_SA_Initialization(S_SG_SA_INOUT_PARAMETERS in_sInOutParam);

    S32BIT DP_SG_SA_Config_for_ChannelMeasurement(PS_SG_SA_INOUT_PARAMETERS inout_psInOutParam);
    S32BIT DP_SA_Config_for_ChannelMeasurement(PS_SG_SA_INOUT_PARAMETERS inout_psInOutParam);

    S32BIT DP_Exit(U8BIT in_u8SigGenSel);
};

#endif // CTESTCASEWRAPPER_H
