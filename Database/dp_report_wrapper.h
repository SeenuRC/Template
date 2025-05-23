#ifndef _DP_REPORTWRAPPER_H_
#define _DP_REPORTWRAPPER_H_

#include <qstring.h>
#include <qsqldatabase.h>
#include "../Database/dp_rpt_datastruct.h"
#include "../Database/dp_config_struct.h"

#define DP_RETURN_DB_SUCCESS 0
#define DP_RETURN_DB_FAILURE -1

enum{
    DP_DATABASE_SUCCESS = 0,
    DP_ERR_DB_NOT_OPENED_1 = -100,
    DP_ERR_INVALID_TEST_ID_1,
    DP_ERR_INVALID_CFN_NUM_1,
    DP_ERR_QUERY_EXE_1,
    DP_ERR_FILE_OPEN_1,
    DP_DATABASE_FAILURE,
    DP_RPT_WRAP_ERR_CONN_DATABASE,
    DP_RPT_WRAP_ERR_TR,
    DP_RPT_WRAP_ERR_READ_USER_DETAILS,
    DP_RPT_WRAP_INSERT_DETAIL_ERR,
    DP_RPT_WRAP_ERR_UPDATE_USER_DETAILS,
    DP_RPT_WRAP_ERR_VERIFY_DETAILS,
    DP_ERR_DB_OPEN,
    DP_ERR_FILE_PATH,
};

typedef struct
{
    S32BIT	lSessionID;
    S8BIT   qsUserID[50];
    S8BIT	qsUserName[50];
    S8BIT	qsPassword[50];
    S32BIT 	sPrivilege;

}S_RWR_ATE_MD_USER_DETAILS;
typedef struct _STESTCASE_TABLE
{
    S32BIT m_iID;
    S8BIT m_szTestCaseName[64];
    S32BIT m_iConfigNo;
}S_TESTCASE_NAME;

class CReportConfigWrapper
{
private:

    U32BIT m_u32MinorCycleNo;
       U32BIT m_u32MajorCycleNo;
public:

    CReportConfigWrapper();
    ~CReportConfigWrapper();

    QSqlDatabase m_objDataBase;

    void setMajorCycleNo(U32BIT in_u32MajorCycleNo)
        {
            m_u32MajorCycleNo = in_u32MajorCycleNo;
        }

        void setMinorCycleNo(U32BIT in_u32MinorCycleNo)
        {
            m_u32MinorCycleNo = in_u32MinorCycleNo;
        }

    void GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg);
    S32BIT ConnectDatabase(QString in_strServerIP, QString in_strUID, QString in_strPassword, QString in_strDatabase, QString in_strPortNo, QString in_strODBCDriVer);
    S32BIT TestRegistration(S_TEST_REGISTER in_sobjTestReg);
    S32BIT GetLastExecutionID(U32BIT* out_pulLastExecID);
    S32BIT GetLastLoginSession(U32BIT* out_pulSessionID);
    S32BIT InsertConsolidatedRecord(QString in_strTestID, U16BIT in_TCID);
    S32BIT GetLastExecutionSessionID(U32BIT* out_ulExecSessionID);
    S32BIT InsertMainExec(U16BIT in_ucModeSubSys, QString in_strSystemName);
    S32BIT ReadUserDetails(QVector<S_USER_DETAILS> &Records, QString in_strUserID);
    S32BIT ReadAllUserDetails(QVector<S_USER_DETAILS> &Records);
    S32BIT InsertUserDetails(QVector<S_USER_DETAILS> &Records);
    S32BIT UpdateUserDetails(S_USER_DETAILS in_sUserDetails, U8BIT in_ucPrivilege);
    S32BIT DeleteUserDetails(QString in_strUserID);
    S32BIT VerifyUserDetails(QString in_strUserID, QString in_strPassword);
    S32BIT ReadTestCaseID(QString in_strTestName, U32BIT *out_u32TestCaseID);
    S32BIT ReadTestCaseID_CableLoss(QString in_strTestName, U32BIT *out_u32TestCaseID);
    S32BIT ReadTestCaseDetails(QVector<S_TESTCASE_NAME> &out_sVecTestCaseName, char *in_szSystemName);


    /********** Data Base Wrapper Functionality ***********/
    S32BIT Insert_harmonic_measurement_Record(QVector <S_HARMONIC_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_noisefigure_measurement_Record(QVector <S_NOISEFIGURE_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_powerdroop_measurement_Record(QVector <S_POWERDROOP_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_rf_pulse_rise_fall_time_meas_Record(QVector <S_RF_PULSE_RISE_FALL_TIME_MEAS> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_rx_gain_measurement_Record(QVector <S_RX_GAIN_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_input_return_loss_Record(QVector <S_INPUT_RETURN_LOSS> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_output_return_loss_Record(QVector <S_OUTPUT_RETURN_LOSS> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_rx_isolation_measurement_Record(QVector <S_RX_ISOLATION_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_tx_output_power_measurement_Record(QVector <S_TX_OUTPUT_POWER_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_power_tuning_measurement_Record(QVector <S_POWER_TUNING_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_rx_outofband_rejection_measurement_Record(QVector <S_RX_OUTOFBAND_REJECTION_MEASUREMENT> &in_sRecordData, QString in_strTestName);  
    S32BIT Insert_phase_noise_measurement_Record(QVector <S_PHASE_NOISE_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_image_rejection_Record(QVector <S_IMAGE_REJECTION> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_onedbcompressionpoint_Record(QVector <S_ONEDBCOMPRESSIONPOINT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_rx_attenuator_validation_Record(QVector <S_RX_ATTENUATOR_VALIDATION> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_rx_gain_measurement_sa_Record(QVector <S_RX_GAIN_MEASUREMENT_SA> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_vswr_measurement_Record(QVector <S_VSWR_MEASUREMENT> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_spurious_measurement_Record(QVector <S_SPURIOUS_MEASUREMENT> &in_sRecordData, QString in_strTestName);    
    S32BIT Insert_moduleid_biteisolation_meas_Record(QVector <S_MODULEID_BITEISOLATION_MEAS> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_moduleid_blanking_isolation_meas_Record(QVector <S_MODULEID_BLANKING_ISOLATION_MEAS> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_moduleid_filter_rejection_Record(QVector <S_MODULEID_FILTER_REJECTION> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_moduleid_tx_attenuator_validation_Record(QVector <S_MODULEID_TX_ATTENUATOR_VALIDATION> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_moduleid_tx_phaseshifter_calibration_Record(QVector <S_MODULEID_TX_PHASESHIFTER_CALIBRATION> &in_sRecordData, QString in_strTestName);
    S32BIT Insert_moduleid_tx_phaseshifter_validation_Record(QVector <S_MODULEID_TX_PHASESHIFTER_VALIDATION> &in_sRecordData, QString in_strTestName);


    /********** Data Base Wrapper Functionality ***********/
    S32BIT Insert_calib_cabloss_rpt_Record(QVector <S_CALIB_CABLOSS_RPT> &in_sRecordData, QString in_strTestName);

};

#endif // _DP_REPORTWRAPPER_H_
