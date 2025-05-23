#ifndef DP_CALIBTHREAD_H
#define DP_CALIBTHREAD_H

#include "src/mainwindow.h"
#include "Database/dp_config_read_wrap.h"
#include "Database/dp_report_wrapper.h"

class CalibTestThread : public QThread
{
    Q_OBJECT

public:
    QSqlDatabase DBExe;

private:

    SAUTOTESTCONFIG m_STestConfig;    

public:
    CalibTestThread(QObject *parent = 0);

    CConfigParamReadWrapper objReadParam;

    void OnSafeState();

//    S16BIT InsertRecord(QVector<S_SBANDPLLBASEDSYNTHESIZERANDLOMODULE_CABLELOSSCALIBRATION> &in_sRecordData);

    QVector <S_CALIB_CABLOSS_RPT> QVectCalibration;

    void Start();

    void stop();

signals:
    void sig_QusToUser(S8BIT *pszUsrmessage);

    void Sig_IndTest_ProgBar(S32BIT);

    void Sig_TotTest_ProgBar(char);

    void Sig_SetMaxIndTest_ProgBar(S32BIT iStart,S32BIT iStop,S32BIT iStep);

    void Sig_ShowMessageBox(S16BIT sWarningType,QString strMsgHeader,QString strMessage);

    void sig_MsgToPrintlog(int, QString);

    void sig_ThreadStop();

    void Sig_Fail_State( bool *out_bTstSts, QString q);

protected:

    void run();
};

#endif // DPCRF6642_CALIBTHREAD_H
