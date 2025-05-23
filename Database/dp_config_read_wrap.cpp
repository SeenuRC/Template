#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QSqlField>

#include "../Database/dp_report_wrapper.h"
#include "../Database/dp_config_read_wrap.h"
#include "../include/dp_types.h"

extern class CReportConfigWrapper *m_pobjReportWrapper;

CConfigParamReadWrapper::CConfigParamReadWrapper()
{

}

CConfigParamReadWrapper::~CConfigParamReadWrapper()
{

}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_RXISOLATIONMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(2).toDouble();
        out_pSConfigData->m_u16NoofPoints = (U16BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dStepFrequency = strListData.value(4).toDouble();
        out_pSConfigData->m_dExpMaxIsolation = strListData.value(5).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_OUTPUTRETURNLOSSMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(2).toDouble();
        out_pSConfigData->m_u16NoofPoints = (U16BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dStepFrequency = strListData.value(4).toDouble();
        out_pSConfigData->m_dExpectedMaxReturnLoss = strListData.value(5).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_INPUTRETURNLOSSMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(2).toDouble();
        out_pSConfigData->m_u16NoofPoints = (U16BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dExpectedMaxReturnLoss = strListData.value(4).toDouble();
        out_pSConfigData->m_dStepFrequency = strListData.value(5).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}


S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_RXGAINMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(2).toDouble();
        out_pSConfigData->m_u16NoofPoints = (U16BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dStepFrequency = strListData.value(4).toDouble();
        out_pSConfigData->m_dExpectedMinGain = strListData.value(5).toDouble();
        out_pSConfigData->m_dExpectedMaxGain = strListData.value(6).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}
S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_NOISEFIGUREMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_u16NoofPoints = (U16BIT)strListData.value(2).toUInt();
        out_pSConfigData->m_dInputPower = strListData.value(3).toDouble();
        out_pSConfigData->m_ExpectedNoiseFigure = strListData.value(4).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_TXOUTPUTPOWERMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dStepFrequency = strListData.value(2).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(3).toDouble();
        out_pSConfigData->m_dXStart = strListData.value(4).toDouble();
        out_pSConfigData->m_dXScaleStep = strListData.value(5).toDouble();
        out_pSConfigData->m_dYMax = strListData.value(6).toDouble();
        out_pSConfigData->m_dYScale = strListData.value(7).toDouble();
        out_pSConfigData->m_dMarker1 = strListData.value(8).toDouble();
        out_pSConfigData->m_dMarker2 = strListData.value(9).toDouble();
        out_pSConfigData->m_u16RFInputPathID = (U16BIT)strListData.value(10).toUInt();
        out_pSConfigData->m_dExpMinOutPower = strListData.value(11).toDouble();
        out_pSConfigData->m_dExpMaxOutPower = strListData.value(12).toDouble();
        out_pSConfigData->m_dPulsePeriod = strListData.value(13).toDouble();
        out_pSConfigData->m_dPulseWidth = strListData.value(14).toDouble();
        out_pSConfigData->m_dHighLevel = strListData.value(15).toDouble();
        out_pSConfigData->m_dLowLevel = strListData.value(16).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_TXHARMONICSMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_u16RFInputPathID = (U16BIT)strListData.value(0).toUInt();
        out_pSConfigData->m_dStartFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(2).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(3).toDouble();
        out_pSConfigData->m_dRBW = strListData.value(4).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(5).toDouble();
        out_pSConfigData->m_dSpan = strListData.value(6).toDouble();
        out_pSConfigData->m_dReferenceLevel = strListData.value(7).toDouble();
        out_pSConfigData->m_dStepFrequency = strListData.value(8).toDouble();
        out_pSConfigData->m_u16RFOutputPathID = (U16BIT)strListData.value(9).toUInt();
        out_pSConfigData->m_dSweepTime = strListData.value(10).toDouble();
        out_pSConfigData->m_dExpectedHarmonicsRejection = strListData.value(11).toDouble();
        out_pSConfigData->m_dVideoTrigLevel = strListData.value(12).toDouble();
        out_pSConfigData->m_dMarkerPos = strListData.value(13).toDouble();
        out_pSConfigData->m_dVideoTrigLevel2ndOrder = strListData.value(14).toDouble();
        out_pSConfigData->m_dVideoTrigLevel3rdOrder = strListData.value(15).toDouble();
        out_pSConfigData->m_dReferenceLevel2ndOrder = strListData.value(16).toDouble();
        out_pSConfigData->m_s8ReferenceLevel3rdOrder = (S8BIT)strListData.value(17).toInt();
        out_pSConfigData->m_dPulseWidth = strListData.value(18).toDouble();
        out_pSConfigData->m_dPulsePeriod = strListData.value(19).toDouble();
        out_pSConfigData->m_dHighLevel = strListData.value(20).toDouble();
        out_pSConfigData->m_dLowLevel = strListData.value(21).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_POWERDROOPMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dStepFrequency = strListData.value(2).toDouble();
        out_pSConfigData->m_u16RFInputPathLoss = (U16BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dReferenceLevel = strListData.value(4).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(5).toDouble();
        out_pSConfigData->m_dExpectedPowerDroop = strListData.value(6).toDouble();
        out_pSConfigData->m_dXScaleStart = strListData.value(7).toDouble();
        out_pSConfigData->m_dXScaleStep = strListData.value(8).toDouble();
        out_pSConfigData->m_dYScaleMax = strListData.value(9).toDouble();
        out_pSConfigData->m_dMarker1 = strListData.value(10).toDouble();
        out_pSConfigData->m_dMarker2 = strListData.value(11).toDouble();
        out_pSConfigData->m_dTrigger = strListData.value(12).toDouble();
        out_pSConfigData->m_dYScale = strListData.value(13).toDouble();
        out_pSConfigData->m_dPulseWidth = strListData.value(14).toDouble();
        out_pSConfigData->m_dPulsePeriod = strListData.value(15).toDouble();
        out_pSConfigData->m_dHighLevel = strListData.value(16).toDouble();
        out_pSConfigData->m_dLowLevel = strListData.value(17).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_RFPULSERISEORFALLTIMETEST *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dStepFrequency = strListData.value(2).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(3).toDouble();
        out_pSConfigData->m_u16RFInputPathID = (U16BIT)strListData.value(4).toUInt();
        out_pSConfigData->m_dXStartRT = strListData.value(5).toDouble();
        out_pSConfigData->m_dXScaleRT = strListData.value(6).toDouble();
        out_pSConfigData->m_dYMaxRT = strListData.value(7).toDouble();
        out_pSConfigData->m_dYScaleRT = strListData.value(8).toDouble();
        out_pSConfigData->m_dXStartFT = strListData.value(9).toDouble();
        out_pSConfigData->m_dXScaleFT = strListData.value(10).toDouble();
        out_pSConfigData->m_dYMaxFT = strListData.value(11).toDouble();
        out_pSConfigData->m_dYScaleFT = strListData.value(12).toDouble();
        out_pSConfigData->m_dExpectedRiseFallTime = strListData.value(13).toDouble();
        out_pSConfigData->m_dPulseWidth = strListData.value(14).toDouble();
        out_pSConfigData->m_dPulsePeriod = strListData.value(15).toDouble();
        out_pSConfigData->m_dHighLevel = strListData.value(16).toDouble();
        out_pSConfigData->m_dLowLevel = strListData.value(17).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_CABLELOSSCALIBRATION *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dStepFrequency = strListData.value(2).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(3).toDouble();
        out_pSConfigData->m_s8PathID = (S8BIT)strListData.value(4).toInt();
        out_pSConfigData->m_dSpan = strListData.value(5).toDouble();
        out_pSConfigData->m_dRBW = strListData.value(6).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(7).toDouble();
        out_pSConfigData->m_s8SweepTime = (S8BIT)strListData.value(8).toInt();
        out_pSConfigData->m_dReferenceLevel = strListData.value(9).toDouble();
        out_pSConfigData->m_dExpMinLoss = strListData.value(10).toDouble();
        out_pSConfigData->m_dExpMaxLoss = strListData.value(11).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_POWERTUNINGMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dStepFrequency = strListData.value(2).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(3).toDouble();
        out_pSConfigData->m_dXStart = strListData.value(4).toDouble();
        out_pSConfigData->m_dXScaleStep = strListData.value(5).toDouble();
        out_pSConfigData->m_dYMax = strListData.value(6).toDouble();
        out_pSConfigData->m_dYScale = strListData.value(7).toDouble();
        out_pSConfigData->m_dMarker1 = strListData.value(8).toDouble();
        out_pSConfigData->m_dMarker2 = strListData.value(9).toDouble();
        out_pSConfigData->m_dRFInputPathID = strListData.value(10).toDouble();
        out_pSConfigData->m_dExpMinOutPower = strListData.value(11).toDouble();
        out_pSConfigData->m_dExpMaxOutPower = strListData.value(12).toDouble();
        out_pSConfigData->m_dPulseWidth = strListData.value(13).toDouble();
        out_pSConfigData->m_dPulsePeriod = strListData.value(14).toDouble();
        out_pSConfigData->m_dLowLevel = strListData.value(15).toDouble();
        out_pSConfigData->m_dHighLevel = strListData.value(16).toDouble();
        out_pSConfigData->m_dReadPeakDelay = strListData.value(17).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dInputPower = strListData.value(0).toDouble();
        out_pSConfigData->m_u16NumberOfPoints = (U16BIT)strListData.value(1).toUInt();
        out_pSConfigData->m_dCalibStartFrequency = strListData.value(2).toUInt();
        out_pSConfigData->m_dCalibStopFrequency = strListData.value(3).toUInt();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 151202", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_SBANDGANPAMODULE_RXOUTOFBANDREJECTION_CONFIGURATIONTABLE sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_dFrequency = strTempList.at(0).toDouble();
        sTabStruct.m_dInputPower = strTempList.at(1).toDouble();
        sTabStruct.m_dExpectedRejection = strTempList.at(2).toDouble();
        out_pSConfigData->m_vectCONFIGURATIONTABLE.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_PHASENOISEMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dInputPower = strListData.value(0).toDouble();
        out_pSConfigData->m_dSpan = strListData.value(1).toDouble();
        out_pSConfigData->m_dRBW = strListData.value(2).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(3).toDouble();
        out_pSConfigData->m_dSweepTime = strListData.value(4).toDouble();
        out_pSConfigData->m_dExpectedMinPhaseNoise = strListData.value(5).toDouble();
        out_pSConfigData->m_dPNStartFrequency = strListData.value(6).toDouble();
        out_pSConfigData->m_dPNStopFrequency = strListData.value(7).toDouble();
        out_pSConfigData->m_dReferenceLevel = strListData.value(8).toDouble();
        out_pSConfigData->m_u16Delay = (U16BIT)strListData.value(9).toUInt();
        out_pSConfigData->m_s8StimulasPathID = (S8BIT)strListData.value(10).toInt();
        out_pSConfigData->m_s8MeasuredPathID = (S8BIT)strListData.value(11).toInt();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200622", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_PHASENOISEMEASUREMENT_FREQUENCYTABLE sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_dInputFrequency = strTempList.at(0).toDouble();
        out_pSConfigData->m_vectFREQUENCYTABLE.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_ATTENUATORVALIDATION *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_u16ChannelNumber = (U16BIT)strListData.value(0).toUInt();
        out_pSConfigData->m_u16StimulusPathId = (U16BIT)strListData.value(1).toUInt();
        out_pSConfigData->m_u16MeasurementPathId = (U16BIT)strListData.value(2).toUInt();
        out_pSConfigData->m_u8Precision = (U8BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dSpan = strListData.value(4).toDouble();
        out_pSConfigData->m_dRBW = strListData.value(5).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(6).toDouble();
        out_pSConfigData->m_dSweepTime = strListData.value(7).toDouble();
        out_pSConfigData->m_dRefLevel = strListData.value(8).toDouble();
        out_pSConfigData->m_u8MarkerPos = (U8BIT)strListData.value(9).toUInt();
        out_pSConfigData->m_u32Delay = strListData.value(10).toUInt();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200698", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_ATTENUATORVALIDATION_CFGDETAILS sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_u8GainEqPathSel = (U8BIT)strTempList.at(0).toUInt();
        sTabStruct.m_dIFDCA_2 = strTempList.at(1).toDouble();
        sTabStruct.m_dRFDCA_2 = strTempList.at(2).toDouble();
        sTabStruct.m_dUserDCA = strTempList.at(3).toDouble();
        sTabStruct.m_dIFFrequency = strTempList.at(4).toDouble();
        sTabStruct.m_dBandCenterFrequency = strTempList.at(5).toDouble();
        sTabStruct.m_dInputPower = strTempList.at(6).toDouble();
        sTabStruct.m_u8BandSel = (U8BIT)strTempList.at(7).toUInt();
        sTabStruct.m_u8AttenSel = (U8BIT)strTempList.at(8).toUInt();
        sTabStruct.m_u8RefSel = (U8BIT)strTempList.at(9).toUInt();
        sTabStruct.m_dInputFrequency = strTempList.at(10).toDouble();
        sTabStruct.m_dIFDCA_1 = strTempList.at(11).toDouble();
        sTabStruct.m_dExpMinDCA = strTempList.at(12).toDouble();
        sTabStruct.m_dExpMaxDCA = strTempList.at(13).toDouble();
        out_pSConfigData->m_vectCFGDETAILS.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_VSWRMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_u8ChannelNumber = (U8BIT)strListData.value(0).toUInt();
        out_pSConfigData->m_dStartFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(2).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(3).toDouble();
        out_pSConfigData->m_dDataPoints = strListData.value(4).toDouble();
        out_pSConfigData->m_u16Trace = (U16BIT)strListData.value(5).toUInt();
        out_pSConfigData->m_dRefLevel = strListData.value(6).toDouble();
        out_pSConfigData->m_dExpMaxRetLoss = strListData.value(7).toDouble();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200751", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_VSWRMEASUREMENT_CFGDETAIL sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_u8BandSel = (U8BIT)strTempList.at(0).toUInt();
        sTabStruct.m_dBandFreq = strTempList.at(1).toDouble();
        sTabStruct.m_u8AttBandSel = (U8BIT)strTempList.at(2).toUInt();
        sTabStruct.m_dExpMaxVswr = strTempList.at(3).toDouble();
        sTabStruct.m_dNoOfPoints = strTempList.at(4).toDouble();
        sTabStruct.m_dStartFreq = strTempList.at(5).toDouble();
        sTabStruct.m_dStopFreq = strTempList.at(6).toDouble();
        out_pSConfigData->m_vectCFGDETAIL.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_u8ChannelNumber = (U8BIT)strListData.value(0).toUInt();
        out_pSConfigData->m_u16StimulusPathId = (U16BIT)strListData.value(1).toUInt();
        out_pSConfigData->m_u16MeasurementPathId = (U16BIT)strListData.value(2).toUInt();
        out_pSConfigData->m_u8Precision = (U8BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dSpan = strListData.value(4).toDouble();
        out_pSConfigData->m_dRBW = strListData.value(5).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(6).toDouble();
        out_pSConfigData->m_dSweepTime = strListData.value(7).toDouble();
        out_pSConfigData->m_dRefLevel = strListData.value(8).toDouble();
        out_pSConfigData->m_u8MarkerPos = (U8BIT)strListData.value(9).toUInt();
        out_pSConfigData->m_u64Delay = strListData.value(10).toULongLong();
        out_pSConfigData->m_dExp1dBPoint = strListData.value(11).toDouble();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200729", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_1DBCOMPRESSIONPOINTMEASUREMENT_CFGDETAILS sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_u8GainEqPathSel = (U8BIT)strTempList.at(0).toUInt();
        sTabStruct.m_dRFDCA_2 = strTempList.at(1).toDouble();
        sTabStruct.m_dCenterFreq = strTempList.at(2).toDouble();
        sTabStruct.m_dIFFrequency = strTempList.at(3).toDouble();
        sTabStruct.m_dIFDCA_1 = strTempList.at(4).toDouble();
        sTabStruct.m_u8BandSel = (U8BIT)strTempList.at(5).toUInt();
        sTabStruct.m_dFrequency = strTempList.at(6).toDouble();
        sTabStruct.m_dInputPower = strTempList.at(7).toDouble();
        sTabStruct.m_dExpMinGain = strTempList.at(8).toDouble();
        sTabStruct.m_dExpMaxGain = strTempList.at(9).toDouble();
        sTabStruct.m_u8IsRefGain = (U8BIT)strTempList.at(10).toUInt();
        sTabStruct.m_dIFDCA_2 = strTempList.at(11).toDouble();
        out_pSConfigData->m_vectCFGDETAILS.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_IMAGEREJECTION *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_u8ChannelNumber = (U8BIT)strListData.value(0).toUInt();
        out_pSConfigData->m_u16StimulusPathId = (U16BIT)strListData.value(1).toUInt();
        out_pSConfigData->m_u16MeasurementPathId = (U16BIT)strListData.value(2).toUInt();
        out_pSConfigData->m_u8Precision = (U8BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dSpan = strListData.value(4).toDouble();
        out_pSConfigData->m_dRBW = strListData.value(5).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(6).toDouble();
        out_pSConfigData->m_dSweepTime = strListData.value(7).toDouble();
        out_pSConfigData->m_dRefLevel = strListData.value(8).toDouble();
        out_pSConfigData->m_u8MarkerPos = (U8BIT)strListData.value(9).toUInt();
        out_pSConfigData->m_u64Delay = strListData.value(10).toULongLong();
        out_pSConfigData->m_dExpImageRej = strListData.value(11).toDouble();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200742", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_IMAGEREJECTION_CFGDETAILS sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_dFrequency = strTempList.at(0).toDouble();
        sTabStruct.m_dIFFrequency = strTempList.at(1).toDouble();
        sTabStruct.m_dImageFrequency = strTempList.at(2).toDouble();
        sTabStruct.m_dInputPower = strTempList.at(3).toDouble();
        sTabStruct.m_u8BandSel = (U8BIT)strTempList.at(4).toUInt();
        sTabStruct.m_dExpMinRej = strTempList.at(5).toDouble();
        sTabStruct.m_u8GainEqPathSel = (U8BIT)strTempList.at(6).toUInt();
        sTabStruct.m_dIFDCA_1 = strTempList.at(7).toDouble();
        sTabStruct.m_dIFDCA_2 = strTempList.at(8).toDouble();
        sTabStruct.m_dRFDCA_2 = strTempList.at(9).toDouble();
        out_pSConfigData->m_vectCFGDETAILS.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_GAINMEASUREMENTSA *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_u16ChannelNumber = (U16BIT)strListData.value(0).toUInt();
        out_pSConfigData->m_u16StimulusPathId = (U16BIT)strListData.value(1).toUInt();
        out_pSConfigData->m_u16MeasurementPathID = (U16BIT)strListData.value(2).toUInt();
        out_pSConfigData->m_u8Precision = (U8BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_dSpan = strListData.value(4).toDouble();
        out_pSConfigData->m_dRbw = strListData.value(5).toDouble();
        out_pSConfigData->m_dVbw = strListData.value(6).toDouble();
        out_pSConfigData->m_dSweepTime = strListData.value(7).toDouble();
        out_pSConfigData->m_dRefLevel = strListData.value(8).toDouble();
        out_pSConfigData->m_u8MarkerPos = (U8BIT)strListData.value(9).toUInt();
        out_pSConfigData->m_u32delay = strListData.value(10).toUInt();
        out_pSConfigData->m_u8SigGenSel = (U8BIT)strListData.value(11).toUInt();
        out_pSConfigData->m_u8SpecSel = (U8BIT)strListData.value(12).toUInt();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200686", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_GAINMEASUREMENTSA_GAIN_CFG_DETAILS sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_u8GainEqPathSel = (U8BIT)strTempList.at(0).toUInt();
        sTabStruct.m_dRFDCA_2 = strTempList.at(1).toDouble();
        sTabStruct.m_dIFFrequency = strTempList.at(2).toDouble();
        sTabStruct.m_dInputFreq = strTempList.at(3).toDouble();
        sTabStruct.m_u8BandSel = (U8BIT)strTempList.at(4).toUInt();
        sTabStruct.m_dInputPower = strTempList.at(5).toDouble();
        sTabStruct.m_dExpMinGain = strTempList.at(6).toDouble();
        sTabStruct.m_dExpMaxGain = strTempList.at(7).toDouble();
        sTabStruct.m_dNomGain = strTempList.at(8).toDouble();
        sTabStruct.m_dIFDCA_1 = strTempList.at(9).toDouble();
        sTabStruct.m_dIFDCA_2 = strTempList.at(10).toDouble();
        out_pSConfigData->m_vectGAIN_CFG_DETAILS.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_SPURIOUSMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dInputPower = strListData.value(0).toDouble();
        out_pSConfigData->m_dSpan = strListData.value(1).toDouble();
        out_pSConfigData->m_dRBW = strListData.value(2).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(3).toDouble();
        out_pSConfigData->m_dSweepTime = strListData.value(4).toDouble();
        out_pSConfigData->m_dExpectedMinSpurious = strListData.value(5).toDouble();
        out_pSConfigData->m_dReferenceLevel = strListData.value(6).toDouble();
        out_pSConfigData->m_u16Delay = (U16BIT)strListData.value(7).toUInt();
        out_pSConfigData->m_s8StimulasPathID = (S8BIT)strListData.value(8).toInt();
        out_pSConfigData->m_s8MeasuredPathID = (S8BIT)strListData.value(9).toInt();
        out_pSConfigData->m_s8InputChannel = (S8BIT)strListData.value(10).toInt();
        out_pSConfigData->m_s8OutputChannel = (S8BIT)strListData.value(11).toInt();
        out_pSConfigData->m_dCenterFrequency = strListData.value(12).toDouble();
        out_pSConfigData->m_dFrequencyTolerance = strListData.value(13).toDouble();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200774", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_SPURIOUSMEASUREMENT_FREQUENCYTABLE sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_dInputFrequency = strTempList.at(0).toDouble();
        out_pSConfigData->m_vectFREQUENCYTABLE.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dRBW = strListData.value(0).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(1).toDouble();
        out_pSConfigData->m_dReferenceLevel = strListData.value(2).toDouble();
        out_pSConfigData->m_dSweepTime = strListData.value(3).toDouble();
        out_pSConfigData->m_u16Delay = (U16BIT)strListData.value(4).toUInt();
        out_pSConfigData->m_dSpan = strListData.value(5).toDouble();
        out_pSConfigData->m_u8StimuluspathId = (U8BIT)strListData.value(6).toUInt();
        out_pSConfigData->m_u8MeasurepathID = (U8BIT)strListData.value(7).toUInt();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200896", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_RFTOBITEISOLATIONMEASUREMENT_RFBITEISOLATIONDATA sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_dInputFreq = strTempList.at(0).toDouble();
        sTabStruct.m_dInputPower = strTempList.at(1).toDouble();
        sTabStruct.m_dExpIso = strTempList.at(2).toDouble();
        out_pSConfigData->m_vectRFBITEISOLATIONDATA.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dRBW = strListData.value(0).toDouble();
        out_pSConfigData->m_dVBW = strListData.value(1).toDouble();
        out_pSConfigData->m_dReferenceLevel = strListData.value(2).toDouble();
        out_pSConfigData->m_dSweepTime = strListData.value(3).toDouble();
        out_pSConfigData->m_u16Delay = (U16BIT)strListData.value(4).toUInt();
        out_pSConfigData->m_dSpan = strListData.value(5).toDouble();
        out_pSConfigData->m_u8StimuluspathID = (U8BIT)strListData.value(6).toUInt();
        out_pSConfigData->m_u8MeasurepathID = (U8BIT)strListData.value(7).toUInt();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 200905", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_RFTOBLANKINGISOLATIONMEASUREMENT_RFBLANKISOLATIONDATA sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_dInputFreq = strTempList.at(0).toDouble();
        sTabStruct.m_dInputPower = strTempList.at(1).toDouble();
        sTabStruct.m_dExpIso = strTempList.at(2).toDouble();
        out_pSConfigData->m_vectRFBLANKISOLATIONDATA.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_FILTERREJECTIONMEASUREMENT *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dInputPower = strListData.value(0).toDouble();
        out_pSConfigData->m_u16DataPoints = (U16BIT)strListData.value(1).toUInt();
        out_pSConfigData->m_dReferenceLevel = strListData.value(2).toDouble();
        out_pSConfigData->m_dExpectdMinmumFilterRejection = strListData.value(3).toDouble();
        out_pSConfigData->m_dHPFOBStartFrequency = strListData.value(4).toDouble();
        out_pSConfigData->m_dHPFOBStopFrequency = strListData.value(5).toDouble();
        out_pSConfigData->m_dHPFRejectionFrequency = strListData.value(6).toDouble();
        out_pSConfigData->m_dLPFOBStartFrequency = strListData.value(7).toDouble();
        out_pSConfigData->m_dLPFOBStopFrequency = strListData.value(8).toDouble();
        out_pSConfigData->m_dLPFRejectionFrequency = strListData.value(9).toDouble();
        out_pSConfigData->m_dNotchRejectionStartFrequency = strListData.value(10).toDouble();
        out_pSConfigData->m_dNotchRejectionStopFrequency = strListData.value(11).toDouble();
        out_pSConfigData->m_dPBStartFrequency = strListData.value(12).toDouble();
        out_pSConfigData->m_dPBStopFrequency = strListData.value(13).toDouble();
        out_pSConfigData->m_dNotchPBStartFrequency = strListData.value(14).toDouble();
        out_pSConfigData->m_dNotchPBStopFrequency = strListData.value(15).toDouble();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_TXPHASESHIFTERCALIBRATION *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toFloat();
        out_pSConfigData->m_dInputPower = strListData.value(2).toDouble();
        out_pSConfigData->m_u16NoOfPoints = (U16BIT)strListData.value(3).toUInt();
        out_pSConfigData->m_u8TraceSelection = (U8BIT)strListData.value(4).toUInt();
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_TXATTENUATIONVALIDATION *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(2).toFloat();
        out_pSConfigData->m_u16NoOfPoints = (U16BIT)strListData.value(3).toUInt();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 152304", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_TXATTENUATIONVALIDATION_ATTENUATIONDATA sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_dDCAAttenuation = strTempList.at(0).toDouble();
        sTabStruct.m_dExpectedMinimumAttenuation = strTempList.at(1).toDouble();
        sTabStruct.m_dExpectedMaximumAttenuation = strTempList.at(2).toDouble();
        out_pSConfigData->m_vectATTENUATIONDATA.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}

S32BIT CConfigParamReadWrapper::ReadConfigData(U32BIT in_u32TestID, U32BIT in_u32CfgNo, S_MODULENAME_TXPHASESHIFTERVALIDATION *out_pSConfigData)
{
    QSqlQuery objQuery(m_pobjReportWrapper->m_objDataBase);
    QStringList strTempList;
    QStringList strListData;
    QString strTempQuery = "";
    QString strTemp = "";
    bool bQryExecSts = false;
    strListData.clear();

    /*Check valid Test ID*/
    strTempQuery = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_ID = %d", in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_TEST_ID_1;
    }

    /*Check valid Configuration Number*/
    strTempQuery = QString::asprintf("SELECT config_data.Configuration_No FROM config_data WHERE config_data.Configuration_No = %d AND config_data.Test_ID = %d", in_u32CfgNo, in_u32TestID);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_INVALID_CFN_NUM_1;
    }

    strTempQuery = QString::asprintf("SELECT GROUP_CONCAT(cd.Value SEPARATOR '$') FROM config_data cd INNER JOIN parameter_definition_table pd ON pd.Parameter_Id = cd.Parameter_Id WHERE pd.Test_ID = %d AND cd.Configuration_No IN (0,%d) AND pd.Control_Type <> 'Table' ORDER BY pd.Parameter_Id ASC", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    if(objQuery.next() == true)
    {
        strTemp = objQuery.value(0).toString();
        strListData = strTemp.split("$");
        if(strListData.isEmpty())
        {
            return DP_ERR_QUERY_EXE_1;
        }
        out_pSConfigData->m_dStartFrequency = strListData.value(0).toDouble();
        out_pSConfigData->m_dStopFrequency = strListData.value(1).toDouble();
        out_pSConfigData->m_dInputPower = strListData.value(2).toDouble();
        out_pSConfigData->m_u16NoOfPoints = (U16BIT)strListData.value(3).toUInt();
    }

    strTempQuery = QString::asprintf("SELECT value FROM config_data WHERE Test_ID = %d AND Configuration_No = %d AND Parameter_ID = 152104", in_u32TestID, in_u32CfgNo);

    bQryExecSts = objQuery.exec(strTempQuery);
    if(bQryExecSts == false)
    {
        return DP_ERR_QUERY_EXE_1;
    }

    while(objQuery.next())
    {
        S_MODULENAME_TXPHASESHIFTERVALIDATION_PHASEDATA sTabStruct;
        strTemp = objQuery.value(0).toString();
        strTempList = strTemp.split(",");
        sTabStruct.m_dPhaseShift = strTempList.at(0).toDouble();
        sTabStruct.m_dExpectedMinimumPhaseShift = strTempList.at(1).toDouble();
        sTabStruct.m_dExpectedMaximumPhaseShift = strTempList.at(2).toDouble();
        out_pSConfigData->m_vectPHASEDATA.push_back(sTabStruct);
    }

    return DP_RETURN_DB_SUCCESS;
}
