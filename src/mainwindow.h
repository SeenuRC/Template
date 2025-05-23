#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../include/dp_types.h"
#include "../Instrument/visa.h"
#include "../Instrument/visatype.h"
#include "DP_datastructure.h"
#include "../Database/dp_report_wrapper.h"
#include "Instrument/SAWrapper.h"
#include "Instrument/SGWrapper.h"
#include "Instrument/ZnlNetworkAnalyzer.h"
#include <QSplashScreen>
#include "QMessageBox"
#include <QFile>
#include <QDir>
#include <QCloseEvent>
#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <QStyle>
#include <QThread>
#include <QTextStream>
#include <QTextEdit>
#include <QDockWidget>
#include <QStackedWidget>
#include <QTime>
#include  <QDate>
#include <QSettings>
#include <QDebug>
#include <QSpinBox>
#include "DP_APP_macros.h"
#include "authentication.h"
#include "ctestinformation.h"
#include "usermanagement.h"
#include "cabouthome.h"
#include "cinstrumentdetection.h"
#include "DP_communication.h"
#include "DP_Automode_Config.h"
#include "DP_Automode_testthread.h"
#include "crfinstdetection.h"
#include <QPixmap>
#include "ctestcasewrapper.h"
#include "../Database/dp_config_struct.h"
#include "../Database/dp_config_read_wrap.h"
#include "../Instrument/PowerMeter.h"
#include "calibration.h"

#include "DP_APP_macros.h"
#include "../Instrument/FGWrapper.h"
#include "../Instrument/DSOwrapper.h"

QT_BEGIN_NAMESPACE
class CustomSplashScreen : public QSplashScreen {
public:
    CustomSplashScreen(const QPixmap &pixmap) : QSplashScreen(pixmap) {}

    void setPermanentMessage(const QString &message) {
        permanentMessage = message;
        update();
    }

    void showMessage(const QString &message, int alignment = Qt::AlignTop | Qt::AlignLeft, const QColor &color = Qt::white) {
        // Add the message to the list of messages
        messages.clear();
        messages.append({ message, alignment, color });
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QSplashScreen::paintEvent(event);
        QPainter painter(this);
        QFont font;
        font.setPointSize(10);
        painter.setFont(font);
        // Draw permanent message
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignTop| Qt::AlignLeft, permanentMessage);
        // Draw additional messages
        int yOffset = 20; // Offset to avoid overlapping
        for (const auto& msg : messages) {
            painter.setPen(msg.color);
            painter.drawText(rect(), msg.alignment, msg.text);
            yOffset += 20; // Increase the offset for each message
        }
    }

private:
    struct Message {
        QString text;
        int alignment;
        QColor color;
    };

    QString permanentMessage;
    QList<Message> messages;
};
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QDockWidget *m_pobjActLog;
    QTextEdit objTextedit;
    bool m_bIsTestActive;
    bool m_btrFlag;
    U8BIT m_arr8ChSel[8];



    void closeEvent(QCloseEvent *) override;
    short DatabaseConnectivity(S_DATABASE_CONNECTIVITY *out_psDatabaseConnect);
    short ReadDefaultSettings();
    short Init_SA();
    short Init_NA();
    short Init_SG();
    short Init_PM();
    short Init_FG();
    short Init_DSO();
    void initWindow();
    short ReadSGSANAInstrumentDetails();
    S16BIT InitializationBoards();


    class CSGWrapper		    m_objRFSGWrapper;
    class CSAWrapper            m_objSAWrapper;
    class CZnlNetworkAnalyzer   m_objNAWrap;
    class CPMWrapper         m_objPMWrap;
    class CFGWrapper            m_objFGWrapper;    
    class CDSO_Wrapper                      m_objDSOWrapper;

    class usermanagement                    *m_objUserManagement;
    class CAboutHome                        *m_pobABout;
    class cinstrumentdetection              *m_pobjInstrumentDetection;
    class DP_APP_communication        *m_pobjCommunication;
    class DP_AutoModeTest_Config     *m_pobjAutoTestCases;
    class CTestCaseWrapper *m_pobjTestCaseWrapper;
    class CCalibration                      *m_objCableLoss;
    QProcess                                *m_pobjProcess;
    QStringList qsliAutoTestCaseList;
    QVector<S_TESTCASE_NAME> m_qVecTestCaseName;

public slots :
     void Printlog(unsigned char u8InfoType,QString sMsg);
     short DP_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize);
     void MenuBarEnDis(unsigned char ucEnDisSts);


protected:
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void on_actionHome_triggered();

    void on_action_Communication_triggered();

    void on_actionUserManagment_triggered();

    void on_actionA_bout_triggered();

    void on_action_ATE_Test_Cases_triggered();

    void on_actionCableloss_Calibration_triggered();

    void on_actionReport_Generator_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
