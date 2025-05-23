#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QSplashScreen>
#include <QPainter>

SGlobalStucture gGlobalStuct;
short gsTestExeReturn;
short gsReturn;
#define DATA_BASE 1
MainWindow *p_objMainApp;
CReportConfigWrapper *m_pobjReportWrapper;
CConfigParamReadWrapper *m_pobjReadConfigData;
#define SET_STYLESHEET(QObj,QSS_File) {\
    QFile file;\
    file.setFileName(QSS_File);\
    file.open(QIODevice::ReadOnly | QIODevice::Text);\
    QTextStream TxtStream(&file);\
    QObj.setStyleSheet(TxtStream.readAll());\
    file.close();\
    }


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopWidget qdesk;
    QCoreApplication::setApplicationVersion("1V00");

    SET_STYLESHEET(a, "./DP_APP_QSS.qss");

    unsigned char u8RetryCnt = 0;
    unsigned int  s32RetVal         = 0;
    char cErrMsg[256]               = {0};
    QString strMsg                  = "";
    S_DATABASE_CONNECTIVITY sDatabaseConnect;

    memset(&sDatabaseConnect, 0, sizeof(S_DATABASE_CONNECTIVITY));
    memset(&gGlobalStuct, 0, sizeof(SGlobalStucture));
    m_pobjReportWrapper = new CReportConfigWrapper;
    m_pobjReadConfigData = new CConfigParamReadWrapper;
    MainWindow w;
    w.setWindowIcon(QIcon(":/Application/images/loading2.gif"));

    p_objMainApp = &w;
#if DATA_BASE
    //Read the database details from INI File
    s32RetVal = p_objMainApp->DatabaseConnectivity(&sDatabaseConnect);
    if(s32RetVal)
    {
        p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
        strMsg= QString::asprintf("%s", cErrMsg);
        QMessageBox :: information(&w, "Error", strMsg);
        return DP_FAILURE;
    }

    //Connect with  Database
    s32RetVal = m_pobjReportWrapper->ConnectDatabase(sDatabaseConnect.m_szServerIP,sDatabaseConnect.m_szUID,sDatabaseConnect.m_szPassword,sDatabaseConnect.m_szDatabase,sDatabaseConnect.m_szPortNo,sDatabaseConnect.m_szODBCDriverVer);
    if(s32RetVal)
    {
        p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
        strMsg= QString::asprintf("%s", cErrMsg);
        QMessageBox :: information(&w, "Error", strMsg);
        return DP_FAILURE;
    }
    else
    {
        strMsg = "Database Connect Success";
        p_objMainApp->Printlog(PRINTLOG_TYPE_SUCCESS, strMsg);
    }
    s32RetVal = p_objMainApp->ReadSGSANAInstrumentDetails();
    if(s32RetVal)
    {
        p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
        strMsg = QString::asprintf("%s", cErrMsg);
        return DP_FAILURE;
    }
    s32RetVal = m_pobjReportWrapper->ReadTestCaseDetails(w.m_qVecTestCaseName, sDatabaseConnect.m_szSystemName);
    if(s32RetVal)
    {
        p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
        strMsg = QString::asprintf("%s", cErrMsg);
        QMessageBox :: information(&w, "Error", strMsg);
        return DP_FAILURE;
    }
    else
    {
        strMsg = "Test case deatils read success";
        p_objMainApp->Printlog(PRINTLOG_TYPE_SUCCESS, strMsg);
    }

    Authentication loginDlg;
    SET_STYLESHEET(loginDlg, "./DP_APP_QSS.qss");
    loginDlg.setWindowTitle("Login");
    loginDlg.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,loginDlg.size(),qdesk.screenGeometry() ));
//    loginDlg.exec();

#else
    gsReturn = 1;
    gsTestExeReturn = 1;
#endif
    if(gsReturn = 1)
    {
        //Read the Instrument details from Ini file
        s32RetVal = p_objMainApp->ReadDefaultSettings();
        if(s32RetVal)
        {
            p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            return DP_FAILURE;
        }

        //Initializing boards
        s32RetVal = p_objMainApp->InitializationBoards();
        if(s32RetVal)
        {
            p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            return DP_FAILURE;
        }

        CRFInstDetection objInstCfg;
        objInstCfg.setWindowTitle("Instrument Configuration");
        SET_STYLESHEET(objInstCfg, "./DP_APP_QSS.qss");
        objInstCfg.exec();

        strMsg = "Instrument configuration Successfully";
        p_objMainApp->Printlog(PRINTLOG_TYPE_SUCCESS, strMsg);
        gsReturn = 0;

        //Read the Instrument details from Ini file
        s32RetVal = p_objMainApp->ReadDefaultSettings();
        if(s32RetVal)
        {
            p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            return DP_FAILURE;
        }

        w.setWindowIcon(QIcon(":/Images/images/AppIcon.ico"));
        SET_STYLESHEET(w, "./DP_APP_QSS.qss");
        a.processEvents();
        QPixmap pixmap(":/Application/images/splash.jpg");
        CustomSplashScreen splash(pixmap);
        splash.setPermanentMessage("Application Version : 0V01");
        splash.show();
        splash.showMessage(QObject::tr("Template"),Qt::AlignLeft | Qt::AlignBottom,Qt::white);
        splash.clearMessage();
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        a.processEvents();
#if 0
        splash.showMessage(QObject::tr("Signal Generator is initializing..."),Qt::AlignLeft | Qt::AlignBottom,Qt::white);

        u8RetryCnt = 0 ;
        while(u8RetryCnt < 50)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        w.Init_SG();
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        a.processEvents();

        splash.showMessage(QObject::tr("Spectrum Analyzer is initializing..."),Qt::AlignLeft | Qt::AlignBottom,Qt::white);
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        w.Init_SA();
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }
        sleep(1);
        a.processEvents();

        a.processEvents();

        splash.showMessage(QObject::tr("Power Meter is initializing..."),Qt::AlignLeft | Qt::AlignBottom,Qt::white);

        u8RetryCnt = 0 ;
        while(u8RetryCnt < 50)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        w.Init_PM();
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        a.processEvents();

        splash.showMessage(QObject::tr("Network Analyzer is initializing..."),Qt::AlignLeft | Qt::AlignBottom,Qt::white);
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        w.Init_NA();
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }
        sleep(1);
        a.processEvents();

        splash.showMessage(QObject::tr("Function Generator is initializing..."),Qt::AlignLeft | Qt::AlignBottom,Qt::white);
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        w.Init_FG();
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }
        sleep(1);
        a.processEvents();

        splash.showMessage(QObject::tr("DSO is initializing..."),Qt::AlignLeft | Qt::AlignBottom,Qt::white);
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }

        w.Init_DSO();
        u8RetryCnt = 0 ;
        while(u8RetryCnt < 100)
        {
            a.processEvents();
            QThread::msleep(10);
            u8RetryCnt++;
        }
        sleep(1);
        a.processEvents();
#endif
        splash.close();
        w.initWindow();
        w.showMaximized();
        return a.exec();
    }
    else
    {
        return 0;
    }

}
