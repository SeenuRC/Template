#ifndef CTESTINFORMATION_H
#define CTESTINFORMATION_H
#include "../Database/dp_report_wrapper.h"
#include <QDialog>
#include "mainwindow.h"
#include "qvector.h"


namespace Ui {
class ctestinformation;
}

class ctestinformation : public QDialog
{
    Q_OBJECT

public:
    explicit ctestinformation(QWidget *parent = 0);
    ~ctestinformation();

private slots:

    void on_pbOk_clicked();
    void on_pbCancel_clicked();
    void closeEvent (QCloseEvent *event);
signals:
    void sig_MsgToPrintlog(int, QString);
protected:

  //  void keyPressEvent(QKeyEvent *e) override;

private:
    Ui::ctestinformation *ui;
};

#endif // CTESTINFORMATION_H
