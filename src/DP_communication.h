#ifndef DP_APP_COMMUNICATION_H
#define DP_APP_COMMUNICATION_H

#include <QDialog>
#include "mainwindow.h"
#include "Instrument/dp-lins_psu_wrapper.h"
#include <QHostAddress>

namespace Ui {
class DP_APP_communication;
}

class DP_APP_communication : public QDialog
{
    Q_OBJECT

    U8BIT m_u8Port;

public:
    explicit DP_APP_communication(QWidget *parent = 0);
    ~DP_APP_communication();

    CDP_PSU_Wrapper *m_obj_CPSUWrapper;
    U8BIT m_u8ConType;

    FSINGLE m_fVoltage;
    FSINGLE m_fCurrent;
    FSINGLE m_fOverVoltage;
    FSINGLE m_fUnderVoltage;
    FSINGLE m_fVoltrating;
    FSINGLE m_fCurrentRating;
    char m_szModel[30];

    void show();

    short ReadUARTConfiguration(PSDP_APP_UART_CONFIG);


private slots:

    void on_pb_UARTOpen_clicked();

    void on_pb_UARTClose_clicked();

    void keyPressEvent(QKeyEvent *e);

    void on_pb_UARTOpen_PS_clicked();

    void on_pb_UARTClose_PS_clicked();

    void on_PB_PowerOn_clicked();

    void on_PB_PowerOff_clicked();

    void on_PB_Read_clicked();

    void on_pbOpen_clicked();

    void on_pbClosePort_clicked();

    void on_pb_EtheConnect_clicked();

    void on_pb_EtheDisconnect_clicked();

    void on_pb_I2COpen_clicked();

signals:

    void sig_MsgToPrintlog(int, QString);

private:
    Ui::DP_APP_communication *ui;
};

#endif // DP_APP_COMMUNICATION_H
