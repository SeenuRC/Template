#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include "qthread.h"
#include "mainwindow.h"

namespace Ui {
class CCalibration;
}

class CCalibration : public QWidget
{
    Q_OBJECT

public:
    explicit CCalibration(QWidget *parent = 0);
    ~CCalibration();

    S8BIT cThreadStoped;

    CSGWrapper *m_objSGWrapper;

    SDEVICEDETAILS m_SSGInst[1];

    class CalibTestThread *pobjCalibThread;

    void initTable();

private slots:
    void on_cb_CalibSelAll_clicked(bool checked);

    void slot_tablewidgetclicked(int irow, int iCol);

    void on_pb_Calibstart_clicked();

    void Slot_MessageBox(S16BIT sWarningType,QString strMsgHeader,QString strMessage);

    void on_pb_Calibstop_clicked();

    void Slot_TotTest_ProgBar(char);

    void Slot_IndTest_ProgBar(S32BIT iValue);

    void Slot_SetIndTotTest_ProgBar(S32BIT iStart,S32BIT iStop,S32BIT iStep);

    //void Slot_Fail_State( bool *out_bTstSts, QString);

public slots:
    void slot_ThreadStop();

signals:
    void sig_MsgToPrintlog(int, QString);

    void Sig_ConfigMenuBar(bool);

private:
    Ui::CCalibration *ui;
};

#endif // CALIBRATION_H
