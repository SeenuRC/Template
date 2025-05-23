#ifndef DPRFSS7139_AUTOMODETESTCASE_H
#define DPRFSS7139_AUTOMODETESTCASE_H

#include "mainwindow.h"
#include <QDialog>
#include "dp_dlegate_items.h"
#include "DP_Automode_testthread.h"
namespace Ui {
class DP_AutoModeTest_Config;
}


class DP_AutoModeTest_Config : public QDialog
{
    Q_OBJECT

public:
    explicit DP_AutoModeTest_Config(QWidget *parent = 0);
    ~DP_AutoModeTest_Config();

    class CAutoTestThread *pAutoTestThread;

    unsigned char m_ucarrCycleCnt[100];
    unsigned char m_ucarrTestSel[256];
    unsigned char m_ucTestCnt;
    unsigned char m_ucarrTestID[50];
    unsigned int m_uiMajCyclDelay;
    unsigned int m_uiMinorCyclDelay;
    unsigned int m_usTotMajorCnt;
    unsigned int m_uiDelay;
    unsigned long m_ulOverallProg;
    short m_sMajorCycleNo;
    int m_iCurrCycleNo;
    volatile bool bTestExecSts;

signals:

    void sig_UpdateActionLog(unsigned char, QString);
    void sig_MsgToPrintlog(int, QString);
    void sig_MenuEnaDisSts(unsigned char ucEnDisSts);

private:
    Ui::DP_AutoModeTest_Config *ui;

    QSpinBox *m_SpinBox[20];

    void initTestSelTable();

protected:
    void paintEvent(QPaintEvent *);

private slots:

    void on_pbTestStart_clicked();

    void on_pbTestStop_clicked();

    void  slot_tablewidgetclicked(int irow, int iCol);

    void on_cbTestSelectAll_clicked(bool checked);

    void EnableDisableGUI();

    void Slot_UpdateMajorProgressBar(int iValue);

    void Slot_UpdateMinorProgressBar(int iValue);

    void Slot_UpdateOverallProgressBar(int iValue);

    void Slot_SetMajorCyclePrgrBarRange(int iMin, int iMax);

    void Slot_SetMinorCyclePrgrBarRange(int iMin, int iMax);

    void Slot_SetOverAllPrgrBarRange(int iMin, int iMax);

    void AutoTestGUIUpdate(unsigned char in_ucTestCase, bool in_bAutoTestSts);

    void keyPressEvent(QKeyEvent *e);


public slots:

    void Slot_Fail_State( bool *out_bTstSts, QString);

    void slot_QusToUser(char *pszUsrmessage);

    void Slot_FileOpen();

    void slot_QusToUserContinueSkip(char *pszUsrmessage);

    void slot_ChangeStartStopEnDis(bool bStatus);

    void UpdateUI(unsigned char in_ucSuccessCnt, unsigned long in_ulTotSuccessCnt, unsigned char in_ucFailCnt, unsigned long in_ulTotFailCnt, unsigned char in_ucTestCnt, unsigned long ulOverallProg, int iCurrCycleNo);
};

#endif // DPRFSS7139_AUTOMODETESTCASE_H
