#ifndef cinstrumentdetection_H
#define cinstrumentdetection_H

#include <QDialog>

namespace Ui {
class cinstrumentdetection;
}

class cinstrumentdetection : public QDialog
{
    Q_OBJECT

public:
    explicit cinstrumentdetection(QWidget *parent = 0);

    ~cinstrumentdetection();

    void OnAddCots();

    void Slot_Communication();

    void EnaDisdisplay_control(bool in_bEnDisable);

signals:
    void sig_ConnectComm();

    void sig_MsgToPrintlog(int, QString);


private slots:

    void on_pbProceed_clicked();

protected:

    void keyPressEvent(QKeyEvent *e) override;

private:

    Ui::cinstrumentdetection *ui;
};

#endif // cinstrumentdetection_H
