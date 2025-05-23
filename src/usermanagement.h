#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include "../Database/dp_report_wrapper.h"
#include <QDialog>

namespace Ui {
class usermanagement;
}

class usermanagement : public QDialog
{
    Q_OBJECT

public:
    explicit usermanagement(QWidget *parent = 0);
    ~usermanagement();
    QString strUserID;
    QString strCurrPassword;
    QString strUserName;
    QString strNewPassword;
    QString strConfPassword;
    unsigned char u8UserSelection;
    bool ValidateField();
    void ResetContents(void);
    void LoadUserIDToCbo(unsigned short usOption);

private slots:
    void on_cmbUserSel_currentIndexChanged(int index);

    void on_pbAddDelChangeUpdate_clicked();
signals:
    void sig_MsgToPrintlog(int, QString);
protected:

    void keyPressEvent(QKeyEvent *e) override;
private:
    Ui::usermanagement *ui;
};

#endif // USERMANAGEMENT_H
