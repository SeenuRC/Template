#ifndef DP_APP_TESTTHREAD_H
#define DP_APP_TESTTHREAD_H

#include "mainwindow.h"
#include "DP_Automode_Config.h"

class CAutoTestThread : public QThread
{
    Q_OBJECT

private:

    QSettings *m_INIParser;
    
public:

    CAutoTestThread(QObject *parent = 0)
    {

    }

    SAUTOTESTCONFIG m_STestConfig;

    QString strTemp;
    unsigned int m_uiAutoTestFailCnt[30];
    unsigned int m_uiAutoTestSuccessCnt[30];

    void Start(SAUTOTESTCONFIG in_STestConfig);

    short PerformTest(unsigned char in_ucTotMinorCyleCnt, unsigned short in_usMajorCnt, QString in_strTestName,  unsigned char in_ucTestID, unsigned char in_ucTestIdx, short (CAutoTestThread::*fnPtr)(bool *, char *), unsigned char in_ucChNo);

    double dproundoff(double in_dData, unsigned char in_ucPrecision);

    double dpdbmtoWatt(double in_ddbm);

    short  Input_Return_Loss_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short  Output_Return_Loss_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short  Rx_Isolation_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short  Rx_Gain_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short  Noise_Figure_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short Power_Tuning_Measurement(bool *out_bTstSts, char *in_szTestname);

    short  Tx_Output_Power_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short  Tx_Harmonics_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short  Power_Droop_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short  RF_Pulse_Rise_Fall_Time_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short  Rx_OutOfBand_Rejection_Measuremnt(bool *out_bTstSts, char *in_szTestname);

    short PhaseNoiseMeasurement(bool *out_bTstSts, char* in_szTestname);

    short RxGainMeasuremnt_SA(bool *out_bTstSts, char *in_szTestname);

    short Receiver1dBMeasurement(bool *out_bTstSts, char *in_szTestname);

    short VSWRMeasurement(bool *out_bTstSts, char *in_szTestname);

    short RxSpuriousMeasuremnt(bool *out_bTstSts, char *in_szTestname);

    short ImageRejection(bool *out_bTstSts, char *in_szTestname);

    short RFBITEIsolation( bool *out_bTestSts, char* in_szTestname);

    short RFBlankIsolation( bool *out_bTestSts, char* in_szTestname);

    short CHN_CHN_Isolation(bool *out_bTestSts, char* in_szTestname);

    short FilterRejection(bool *out_bTestSts, char* in_szTestname);

    short Tx_Phaseshifter_Validation(bool *out_bTestSts, char* in_szTestname);

    short Tx_Phaseshifter_Calibration(bool *out_bTstSts, char *in_szTestname);

    double Calc_CenterFreq(double in_dFreq);

    double Atten_Roundoff(double in_dData, unsigned char in_ucPrecision);

    short ApplyCalibData(unsigned int in_uiPathID, double in_dFrequency, double in_dUncalibPower, double *out_dCalibPower, double *out_dCableloss);
    
    void GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg);

    void TestPerformance(unsigned char in_usTestCase, unsigned char in_ucMajorCycle,  unsigned char in_ucMinorCycle, bool in_TestSts);

    S32BIT PowerON();

    S32BIT PowerOff();

    void Channel_Config(unsigned short *in_pus_Channel_sel);

    S32BIT PerformNACalib(FDOUBLE in_dStartFreq, FDOUBLE in_dStopFreq, FDOUBLE in_dInputPower, U32BIT in_u32DataPoints, QString in_strCalibStateFile, U8BIT in_u8TraceSel);

    short OnSafeState();

    QVector <S_RX_GAIN_MEASUREMENT> QVectRx_Gain_Measurement;

    QVector <S_INPUT_RETURN_LOSS> QVect_InputReturnLoss_Measurement;

    QVector <S_HARMONIC_MEASUREMENT> QVect_Harmonics_Measurement;

    QVector <S_NOISEFIGURE_MEASUREMENT> QVect_NoiseFig_Measurement;

    QVector <S_POWERDROOP_MEASUREMENT> QVect_PowerDroop_Measurement;

    QVector <S_RF_PULSE_RISE_FALL_TIME_MEAS> QVect_RF_PulseRise_Measurement;

    QVector <S_OUTPUT_RETURN_LOSS> QVect_OutputReturnLoss_Measurement;

    QVector <S_RX_ISOLATION_MEASUREMENT> QVectRxIsolation_Measurement;

    QVector <S_TX_OUTPUT_POWER_MEASUREMENT> QVectTx_OutputPower_Measurement;

    QVector <S_POWER_TUNING_MEASUREMENT> QVect_PowerTuning_Measurement;

    QVector <S_RX_OUTOFBAND_REJECTION_MEASUREMENT> QVect_RxOutofBand_Measurement;

    QVector <S_PHASE_NOISE_MEASUREMENT> QVectRx_PhaseNoise_Measurement;

    QVector <S_RX_GAIN_MEASUREMENT_SA> QVectRx_Gain_Measurement_SA;

    QVector <S_VSWR_MEASUREMENT> QVectVSWRMeasurement;

    QVector <S_RX_ATTENUATOR_VALIDATION> QVectRx_Attenuator_Validation;

    QVector <S_ONEDBCOMPRESSIONPOINT> QVectReceiver1dBCompressionPoint;

    QVector <S_IMAGE_REJECTION> QVectImageRejection;

    QVector <S_SPURIOUS_MEASUREMENT> QVectRx_Spurious_Measurement;

    QVector <S_MODULEID_BITEISOLATION_MEAS> qvBiteIsolationData;

    QVector <S_MODULEID_BLANKING_ISOLATION_MEAS> qvBlankingIsolationData;

   QVector <S_MODULEID_FILTER_REJECTION> qvFilterRejectionMeasData;

   QVector <S_MODULEID_TX_PHASESHIFTER_VALIDATION> QVectTxPhaseShiftMeasurement;

   QVector <S_MODULEID_TX_PHASESHIFTER_CALIBRATION> QVectTxPhaseShiftCALMeasurement;

signals:

    void sig_UpdateUI(unsigned char, unsigned long in_ulTotSuccessCnt, unsigned char,  unsigned long in_ulTotFailCnt, unsigned char , unsigned long , int );

    void sig_ChangeStartStopEnDis(bool bStatus);

    void sig_UpdateActionLog(unsigned char, QString);

    void sig_MsgToPrintlog(int, QString);

    void Sig_Fail_State( bool *out_bTstSts, QString q);

    void sig_QusToUser(char*);

    void sig_MenuEnaDisSts(unsigned char);

    void sig_AutoModeGUIEnDis();

    void sig_SetMajorCycleProgressbarRange(int, int);

    void sig_SetMinorCycleProgressbarRange(int, int);

    void sig_SetOverAllProgressbarRange(int, int);

    void sig_AutoModeGUIUpdate(unsigned char, bool);

    void sig_UpdateMajorProgressBar(int);

    void sig_UpdateMinorProgressBar(int);

    void sig_UpdateOverallProgressBar(int);

    void sig_FileOpen();

    short Sig_Config_BITE_Param(double dFreq, unsigned char ucSecSel, unsigned char ucBITEInp);

    short Sig_Config_Gain_Param(double dFreq, unsigned char ucSecSel, double fAtten, unsigned char in_ucGainEql, unsigned char in_ucAttenSel );

    void  sig_QusToUserContinueSkip(char*);

protected:

    void run();
};

#endif // DP_APP_TESTTHREAD_H

