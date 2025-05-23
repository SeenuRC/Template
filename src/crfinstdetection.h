#ifndef CRFINSTDETECTION_H
#define CRFINSTDETECTION_H

#include <QDialog>
#include <QtWidgets>
namespace Ui {
class CRFInstDetection;
}

class CRFInstDetection : public QDialog
{
    Q_OBJECT

public:
    int iTotNoofSiggen            ;
    int iTotNoofSpectrumAnalyzer  ;
    int iTotNoofNetworkAnalyzer   ;
    explicit CRFInstDetection(QWidget *parent = nullptr);
    ~CRFInstDetection();
    void InitShow();

private slots:
    void on_pbConfigure_clicked();
    void closeEvent (QCloseEvent *event);
    void keyPressEvent(QKeyEvent *e);

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::CRFInstDetection *ui;
};

#endif // CRFINSTDETECTION_H
