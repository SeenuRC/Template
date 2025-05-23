#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include  <QDialog>
#include "QMessageBox"
#include  <QCryptographicHash>
#include "../Database/dp_report_wrapper.h"

namespace Ui {
class Authentication;
}

class Authentication : public QDialog
{
    Q_OBJECT

private:

public:
    explicit Authentication(QWidget *parent = 0);
    QString  m_strUsrname, m_strPwd,str;
    ~Authentication();

private slots:

    void on_PB_Exit_clicked();

    void on_PB_Login_clicked();

    void on_LE_Pwd_textEdited(const QString &arg1);

    void closeEvent (QCloseEvent *event);
signals :
    void Sig_UpdateMainWindow(unsigned int);


private:
    Ui::Authentication *ui;
};

#endif // AUTHENTICATION_H
