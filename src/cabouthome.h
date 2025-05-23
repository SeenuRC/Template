#ifndef CABOUTHOME_H
#define CABOUTHOME_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class CAboutHome;
}

class CAboutHome : public QDialog
{
    Q_OBJECT

public:
    explicit CAboutHome(QWidget *parent = 0);
    ~CAboutHome();
    void show();
    unsigned int crc32Cksum(const void *buf, unsigned int bufLen, unsigned int crc32);
signals:
    void sig_MsgToPrintlog(U8BIT, QString);

private slots:
    void on_pB_Ok_clicked();

protected:

    void keyPressEvent(QKeyEvent *e) override;

private:
    Ui::CAboutHome *ui;
};

#endif // CABOUTHOME_H
