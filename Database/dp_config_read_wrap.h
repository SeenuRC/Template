#ifndef DP_CONFIG_READ_WRAP_H
#define DP_CONFIG_READ_WRAP_H

#include "../include/dp_types.h"
#include "../Database/dp_config_struct.h"

class CConfigParamReadWrapper
{
private:

public:

    CConfigParamReadWrapper();
    ~CConfigParamReadWrapper();

    void GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg);

    /********** Test Case Wrapper Functionality ***********/
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_INPUTRETURNLOSSMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_OUTPUTRETURNLOSSMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_RXGAINMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_NOISEFIGUREMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_TXOUTPUTPOWERMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_TXHARMONICSMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_POWERDROOPMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_RFPULSERISEORFALLTIMETEST *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_CABLELOSSCALIBRATION *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_POWERTUNINGMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_RXISOLATIONMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION *out_pSConfigData);    
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_PHASENOISEMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_GAINMEASUREMENTSA *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_IMAGEREJECTION *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_VSWRMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_ATTENUATORVALIDATION *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_SPURIOUSMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_FILTERREJECTIONMEASUREMENT *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_TXPHASESHIFTERVALIDATION *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_TXPHASESHIFTERCALIBRATION *out_pSConfigData);
    S32BIT ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_TXATTENUATIONVALIDATION *out_pSConfigData);
};

#endif // DP_CONFIG_READ_WRAP_H
