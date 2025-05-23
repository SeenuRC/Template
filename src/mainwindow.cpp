#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DP_CalibThread.h"

extern SGlobalStucture gGlobalStuct;
unsigned char g_u8ConfigFlash;
unsigned char g_u8WriteRead= 0;
FILE *m_fpPrintLog;
unsigned char g_ucSACount;
unsigned char g_ucSGCount;
unsigned char g_ucNACount;
unsigned char g_ucPMCount;
unsigned char g_ucFGCount;
bool b3039BrdSts[2];
DP_DRV_HANDLE g_vpSPIHandle;
int g_iOpenFlag;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    char cArrFilename[100]  = {0};
    QString strFilepath     = "";
    QString strTempPath     = "";

    m_pobjActLog = new QDockWidget;
    m_pobjActLog->setObjectName("Action Log Widget");
    m_pobjActLog->setWindowTitle("Action Log");
    m_pobjActLog->setWidget(&objTextedit);
    m_pobjActLog->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea, m_pobjActLog);
    objTextedit.setReadOnly(true);
    objTextedit.moveCursor(QTextCursor::End);

    QTime tim = QTime::currentTime();
    QDate Dat = QDate::currentDate();

    sprintf(cArrFilename,"DP_APP_ACTION_LOG_%d-%d-%d_%d_%d_%d.txt", Dat.day(),Dat.month(),Dat.year(), tim.hour(), tim.minute(), tim.second());
    strFilepath = qApp->applicationDirPath() + "/ActionLog/" ;
    QDir objDir;
    objDir.mkdir(strFilepath);
    strTempPath = strFilepath + cArrFilename;
    memset(gGlobalStuct.m_carrOutputPath,0,sizeof(gGlobalStuct.m_carrOutputPath));
    strcpy(gGlobalStuct.m_carrOutputPath,strTempPath.toStdString().c_str());
    m_fpPrintLog = fopen(gGlobalStuct.m_carrOutputPath,"a+");
    if(m_fpPrintLog == NULL)
    {
        QMessageBox :: information(this, "Error", "Action log file logging failed",QMessageBox::Ok);
        fclose(m_fpPrintLog);
    }

    m_bIsTestActive = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Printlog(unsigned char u8InfoType, QString sMsg)
{
    char carrPrintStr[512]  = {0};
    QTime tim               = QTime::currentTime();
    QString time            = tim.toString("[HH:mm:ss]");
    QString fullmsg         = "";
    QString qsType          = "";
    QString qsType2         = "";
    QString fullmsg2        = "";
    if(u8InfoType == 1)//Success
    {
        qsType = "<b><span style =font-size:10pt;><font color=Green>[SUCCESS]</font></span></b>";
        qsType2  ="[SUCCESS]";

    }
    else if(u8InfoType == 2)//Failure
    {
        qsType = "<b><span style =font-size:10pt;><font color=Red>[FAILURE]</font></span></b>";
        qsType2  ="[FAILURE]";
    }
    else//Info
    {
        qsType = "<b><span style =font-size:10pt;><font color=Blue>[INFO]</font></span></b>";
        qsType2  ="[INFO]";
    }
    fullmsg.clear();

    fullmsg = time+ "\t"+ qsType+ " "+sMsg;
    fullmsg2 = time+ "\t"+ qsType2+ " "+sMsg;
    if((m_pobjActLog != NULL)&&(m_pobjActLog->widget()!= NULL))
    {
        ((QTextEdit *)m_pobjActLog->widget())->append(fullmsg);
    }

    if(fullmsg2.length() < 512)
    {
        strcpy(carrPrintStr,fullmsg2.toStdString().c_str());
    }
    else
    {
        qDebug("array size is very small");
    }

    if(m_fpPrintLog != NULL)
    {
        fprintf(m_fpPrintLog, "%s\n", carrPrintStr);
        fflush(m_fpPrintLog);
    }
}
void MainWindow::initWindow()
{
    QString strTempPath;
    int iRetVal = 0;
    S8BIT cErrMsg[250]                              = {0};

    m_pobjInstrumentDetection = new cinstrumentdetection(this);
    ui->swMainWindow->insertWidget(0, m_pobjInstrumentDetection);

    connect(m_pobjInstrumentDetection, SIGNAL(sig_ConnectComm()), this, SLOT(on_action_Communication_triggered()));

    m_pobjCommunication = new DP_APP_communication(this);
    ui->swMainWindow->insertWidget(1, m_pobjCommunication);

    m_pobjAutoTestCases = new DP_AutoModeTest_Config(this);
    ui->swMainWindow->insertWidget(2, m_pobjAutoTestCases);

    m_objCableLoss = new CCalibration(this);
    ui->swMainWindow->insertWidget(3, m_objCableLoss);

    m_objUserManagement = new usermanagement(this);
    ui->swMainWindow->insertWidget(4 , m_objUserManagement);

    m_pobABout = new CAboutHome(this);
    ui->swMainWindow->setCurrentIndex(0);

    m_pobjTestCaseWrapper = new CTestCaseWrapper;
    m_objCableLoss->pobjCalibThread = new CalibTestThread(this);

    m_pobjProcess = new QProcess(this);
//    connect(m_pobjProcess, SIGNAL(started()), this, SLOT(on_start()));
//    connect(m_pobjProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(on_finished(int, QProcess::ExitStatus)));
//    connect(m_pobjProcess, SIGNAL(errorOccurred(int, QProcess::ExitStatus)), this, SLOT(on_errorOccurred(int, QProcess::ExitStatus)));

}

short MainWindow::DatabaseConnectivity(S_DATABASE_CONNECTIVITY* out_psDatabaseConnect)
{
    PS8BIT pszSecName = NULL;
    S8BIT szSecKey[128]		= {0};
    QString strTempPath = "";
    QString strTemp = "";
    QString strFilePath = "";
    QSettings *ptDBDetailsFile = NULL;

    QString qstrDBDetFilePath = qApp->applicationDirPath() + "/INPUT_FILES/DatabaseConnectivity.ini";

    if(out_psDatabaseConnect == NULL)
    {
        return DP_APP_ERR_INVALID_POINTER;
    }

    QFileInfo check_DB_file(qstrDBDetFilePath);
    if (check_DB_file.exists() && check_DB_file.isFile())
    {
        ptDBDetailsFile = new(QSettings)(qstrDBDetFilePath, QSettings::IniFormat);
    }
    else
    {
        return DP_APP_ERR_DB_FILE_PATH;
    }

    pszSecName = (PS8BIT)"ODBC_DATA_SOURCE";
    sprintf(szSecKey , "%s/UID", pszSecName);
    if((strTemp = ptDBDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_UID;
    }

    strcpy(out_psDatabaseConnect->m_szUID, strTemp.toStdString().c_str());

    sprintf(szSecKey , "%s/PASSWORD", pszSecName);
    if((strTemp = ptDBDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_PASSWORD;
    }

    strcpy(out_psDatabaseConnect->m_szPassword, strTemp.toStdString().c_str());

    sprintf(szSecKey , "%s/DATABASE", pszSecName);
    if((strTemp = ptDBDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_NAME;
    }

    strcpy(out_psDatabaseConnect->m_szDatabase, strTemp.toStdString().c_str());

    sprintf(szSecKey , "%s/PORT_NO", pszSecName);
    if((strTemp = ptDBDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_PORT_NO;
    }

    strcpy(out_psDatabaseConnect->m_szPortNo, strTemp.toStdString().c_str());

    sprintf(szSecKey , "%s/SYSTEM_NAME", pszSecName);
    if((strTemp = ptDBDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_PORT_NO;
    }

    strcpy(out_psDatabaseConnect->m_szSystemName, strTemp.toStdString().c_str());

    pszSecName = (PS8BIT)"ODBC_DATA_SOURCE";
    sprintf(szSecKey , "%s/SERVER_IP", pszSecName);
    if((strTemp = ptDBDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_SERVER_IP;
    }

    strcpy(out_psDatabaseConnect->m_szServerIP, strTemp.toStdString().c_str());

    pszSecName = (PS8BIT)"ODBC_DATA_SOURCE";
    sprintf(szSecKey , "%s/VERSION", pszSecName);
    if((strTemp = ptDBDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_SERVER_IP;
    }

    strcpy(out_psDatabaseConnect->m_szODBCDriverVer, strTemp.toStdString().c_str());

    return DP_SUCCESS;
}

short MainWindow::ReadDefaultSettings()
{
    PS8BIT pszSecName = NULL;
    S8BIT szSecKey[128]		= {0};
    QString strTemp = "";
    QString strTempPath             = "";
    QString strFilePath             = "";
    QSettings *ptrInstDetailsFile = NULL;

    strTempPath = qApp->applicationDirPath();

    strFilePath = strTempPath + "/INPUT_FILES/" + "BrdDetails.ini";
    //qDebug() << strFilePath;

    QFileInfo check_DB_file(strFilePath);
    if (check_DB_file.exists() && check_DB_file.isFile())
    {
        ptrInstDetailsFile = new(QSettings)(strFilePath, QSettings::IniFormat);
    }
    else
    {
        return DP_APP_ERR_DB_FILE_PATH;
    }

    pszSecName = (PS8BIT)"INST_CONFIG";
    sprintf(szSecKey , "%s/TOT_SG_COUNT", pszSecName);
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_SERVER_IP;
    }
    gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount = strTemp.toUInt(0,10);

    for(unsigned char ucIdx = 0; ucIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount; ucIdx++)
    {
        sprintf(szSecKey , "%s/SG_%d_LNAME", pszSecName, (ucIdx+1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_APP_ERR_DB_SERVER_IP;
        }
        strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szSGLName[ucIdx], strTemp.toStdString().c_str());
    }

    sprintf(szSecKey , "%s/TOT_SA_COUNT", pszSecName);
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_SERVER_IP;
    }
    gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount = strTemp.toUInt(0,10);

    for(unsigned char ucIdx = 0; ucIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount; ucIdx++)
    {
        sprintf(szSecKey , "%s/SA_%d_LNAME", pszSecName,(ucIdx+1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_APP_ERR_DB_SERVER_IP;
        }
        strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szSALName[ucIdx], strTemp.toStdString().c_str());
    }

    /*Network Analyzer*/
    sprintf(szSecKey , "%s/TOT_NA_COUNT", pszSecName);
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_SERVER_IP;
    }
    gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount = strTemp.toUInt(0,10);

    for(unsigned char ucIdx = 0; ucIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount; ucIdx++)
    {
        sprintf(szSecKey , "%s/NA_%d_LNAME", pszSecName,(ucIdx+1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_APP_ERR_DB_SERVER_IP;
        }
        strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szNALName[ucIdx], strTemp.toStdString().c_str());
    }

    /*Power Meter*/
    sprintf(szSecKey , "%s/TOT_PM_COUNT", pszSecName);
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_SERVER_IP;
    }
    gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount = strTemp.toUInt(0,10);

    for(unsigned char ucIdx = 0; ucIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount; ucIdx++)
    {
        sprintf(szSecKey , "%s/PM_%d_LNAME", pszSecName,(ucIdx+1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_APP_ERR_DB_SERVER_IP;
        }
        strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szPMLName[ucIdx], strTemp.toStdString().c_str());
    }

    /*Function Generator*/
    sprintf(szSecKey , "%s/TOT_FG_COUNT", pszSecName);
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_SERVER_IP;
    }
    gGlobalStuct.m_sBrdDetectionConfig.m_ucFGCount = strTemp.toUInt(0,10);

    for(unsigned char ucIdx = 0; ucIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucFGCount; ucIdx++)
    {
        sprintf(szSecKey , "%s/FG_%d_LNAME", pszSecName,(ucIdx+1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_APP_ERR_DB_SERVER_IP;
        }
        strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szFGLName[ucIdx], strTemp.toStdString().c_str());
    }

    /*Fetching oscilloscope instrument details from ini file*/
    sprintf(szSecKey , "%s/TOT_DSO_COUNT", pszSecName);
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return DP_APP_ERR_DB_SERVER_IP;
    }
    gGlobalStuct.m_sBrdDetectionConfig.m_ucDSOCount = strTemp.toUInt(0,10);

    for(unsigned char ucIdx = 0; ucIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucDSOCount; ucIdx++)
    {
        sprintf(szSecKey , "%s/DSO_%d_LNAME", pszSecName,(ucIdx+1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return DP_APP_ERR_DB_SERVER_IP;
        }
        strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szDSOLName[ucIdx], strTemp.toStdString().c_str());
    }

    return DP_SUCCESS;
}

/**************************************************************************
// Name						:	 Init_SG()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To open and detect the Signal Generator
**************************************************************************/
short MainWindow::Init_SG()
{
    unsigned char ucInstNo = 0;
    QString strTemp             = "";
    QPixmap pixmap(":/Application/images/splash_2.png");
    CustomSplashScreen splash(pixmap);

    /*For Signal Geneartor Open and Detection*/
    for(ucInstNo = 1;ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount; ucInstNo++)
    {
        if(m_objRFSGWrapper.Open(ucInstNo, gGlobalStuct.m_sBrdDetectionConfig.m_szSGLName[ucInstNo- 1]))
        {
            splash.showMessage(QObject::tr("Signal Generator is not initiliazed"),Qt::AlignLeft | Qt::AlignBottom,Qt::red);
            gGlobalStuct.sCOTSInstInfo.m_bSigGen[ucInstNo -1] = false;
            strcpy(gGlobalStuct.sCOTSInstInfo.m_cSGInfo[ucInstNo -1], "Not Available");
            strTemp.sprintf("Signal Generator -%d is not initiliazed",ucInstNo);
            Printlog(PRINTLOG_TYPE_FAILURE, strTemp);
        }
        else
        {
            splash.showMessage(QObject::tr("Signal Generator is initiliazed"),Qt::AlignLeft | Qt::AlignBottom,Qt::green);
            gGlobalStuct.sCOTSInstInfo.m_bSigGen[ucInstNo -1] = true;
            strTemp.sprintf("Signal Generator -%d is initiliazed",ucInstNo);
            Printlog(PRINTLOG_TYPE_SUCCESS, strTemp);
        }
    }/*End of SG open Loop*/
    this->repaint();

    return DP_SUCCESS;
}/*End of SG open And Detection*/

/**************************************************************************
// Name						:	 Init_SA()m_pobABout
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To to open and detect Spectrum Analyzer
**************************************************************************/
short MainWindow::Init_SA()
{
    unsigned char ucInstNo = 0;
    QString strTemp = "";
    QPixmap pixmap(":/Application/images/splash_2.png");
    CustomSplashScreen splash(pixmap);
    for( ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount; ucInstNo++)
    {
        /*For Spectrum Analyzer Open and Detection*/
        if(m_objSAWrapper.Open(ucInstNo , gGlobalStuct.m_sBrdDetectionConfig.m_szSALName[ucInstNo-1]))
        {

            splash.showMessage(QObject::tr("Spectrum Analyzer  is not initiliazed"),Qt::AlignLeft | Qt::AlignBottom,Qt::red);
            gGlobalStuct.sCOTSInstInfo.m_bSA[ucInstNo-1] = false;
            strcpy(gGlobalStuct.sCOTSInstInfo.m_cSAInfo[ucInstNo -1], "Not Available");
            strTemp.sprintf("Spectrum Analyzer - %d is not initiliazed", ucInstNo);
            Printlog(PRINTLOG_TYPE_FAILURE, strTemp);
        }
        else
        {
            splash.showMessage(QObject::tr("Spectrum Analyzer is initiliazed"),Qt::AlignLeft | Qt::AlignBottom,Qt::green);
            gGlobalStuct.sCOTSInstInfo.m_bSA[ucInstNo-1] = true;
            strTemp.sprintf("Spectrum Analyzer - %d is initiliazed", ucInstNo);
            Printlog(PRINTLOG_TYPE_SUCCESS, strTemp);
        }
    }
    this->repaint();

    return DP_SUCCESS;

}/*End of SA open And Detection*/

/**************************************************************************
// Name						:	 Init_SA()m_pobABout
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To to open and detect Spectrum Analyzer
**************************************************************************/
short MainWindow::Init_NA()
{
    unsigned char ucInstNo = 0;
    QString strTemp = "";
    QPixmap pixmap(":/Application/images/splash_2.png");
    CustomSplashScreen splash(pixmap);
    for( ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount; ucInstNo++)
    {
        /*For Spectrum Analyzer Open and Detection*/
        if(m_objNAWrap.Open(ucInstNo , gGlobalStuct.m_sBrdDetectionConfig.m_szNALName[ucInstNo-1]))
        {
            splash.showMessage(QObject::tr("Network Analyzer is not initiliazed"),Qt::AlignLeft | Qt::AlignBottom,Qt::red);
            gGlobalStuct.sCOTSInstInfo.m_bNA = false;
            strcpy(gGlobalStuct.sCOTSInstInfo.m_cNAInfo[ucInstNo -1], "Not Available");
            strTemp.sprintf("Network Analyzer is - %d not initiliazed",ucInstNo);
            Printlog(PRINTLOG_TYPE_FAILURE, strTemp);
        }
        else
        {
            splash.showMessage(QObject::tr("Network Analyzer is initiliazed"),Qt::AlignLeft | Qt::AlignBottom,Qt::green);
            gGlobalStuct.sCOTSInstInfo.m_bNA = true;
            strTemp.sprintf("Network Analyzer is - %d initiliazed",ucInstNo);
            Printlog(PRINTLOG_TYPE_SUCCESS, strTemp);
        }
    }
    this->repaint();

    return DP_SUCCESS;

}/*End of SA open And Detection*/

/**************************************************************************
// Name						:	 Init_PM()m_pobABout
// Author					:	 
// Global Variables affected:	 NA
// Created Date				:    05-03-2024
// Revision Date			:
// Reason for Revising		:
// Description				:	To to open and detect Power Meter
**************************************************************************/
short MainWindow::Init_PM()
{
    unsigned char ucInstNo = 0;
    QString strTemp = "";
    QPixmap pixmap(":/Application/images/splash_2.png");
    CustomSplashScreen splash(pixmap);
    for( ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount; ucInstNo++)
    {
        /*For Power Meter Open and Detection*/
        if(m_objPMWrap.Open(ucInstNo , gGlobalStuct.m_sBrdDetectionConfig.m_szPMLName[ucInstNo-1]))
        {

            splash.showMessage(QObject::tr("Power Meter  is not initialized"),Qt::AlignLeft | Qt::AlignBottom,Qt::red);
            gGlobalStuct.sCOTSInstInfo.m_bPM[ucInstNo-1] = false;
            strcpy(gGlobalStuct.sCOTSInstInfo.m_cSAInfo[ucInstNo -1], "Not Available");
            strTemp.sprintf("Power Meter - %d is not initiliazed", ucInstNo);
            Printlog(PRINTLOG_TYPE_FAILURE, strTemp);
        }
        else
        {
            splash.showMessage(QObject::tr("Power Meter is initialized"),Qt::AlignLeft | Qt::AlignBottom,Qt::green);
            gGlobalStuct.sCOTSInstInfo.m_bPM[ucInstNo-1] = true;
            strTemp.sprintf("Power Meter - %d is initiliazed", ucInstNo);
            Printlog(PRINTLOG_TYPE_SUCCESS, strTemp);
        }
    }
    this->repaint();

    return DP_SUCCESS;

}/*End of SA open And Detection*/


/**************************************************************************
// Name						:	 Init_FG()m_pobABout
// Author					:
// Global Variables affected:	 NA
// Created Date				:    05-03-2024
// Revision Date			:
// Reason for Revising		:
// Description				:	To to open and detect Power Meter
**************************************************************************/
short MainWindow::Init_FG()
{
    unsigned char ucInstNo = 0;
    QString strTemp = "";
    QPixmap pixmap(":/Application/images/splash_2.png");
    CustomSplashScreen splash(pixmap);
    for( ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucFGCount; ucInstNo++)
    {
        /*For Power Meter Open and Detection*/
        if(m_objFGWrapper.Open(ucInstNo , gGlobalStuct.m_sBrdDetectionConfig.m_szFGLName[ucInstNo-1]))
        {

            splash.showMessage(QObject::tr("Function Generator is not initialized"),Qt::AlignLeft | Qt::AlignBottom,Qt::red);
            gGlobalStuct.sCOTSInstInfo.m_bFG[ucInstNo-1] = false;
            strcpy(gGlobalStuct.sCOTSInstInfo.m_cSAInfo[ucInstNo -1], "Not Available");
            strTemp.sprintf("Function Generator %d is not initiliazed", ucInstNo);
            Printlog(PRINTLOG_TYPE_FAILURE, strTemp);
        }
        else
        {
            splash.showMessage(QObject::tr("Power Meter is initialized"),Qt::AlignLeft | Qt::AlignBottom,Qt::green);
            gGlobalStuct.sCOTSInstInfo.m_bFG[ucInstNo-1] = true;
            strTemp.sprintf("Function Generator - %d is initiliazed", ucInstNo);
            Printlog(PRINTLOG_TYPE_SUCCESS, strTemp);
        }
    }
    this->repaint();

    return DP_SUCCESS;

}

short MainWindow::Init_DSO()
{
    unsigned char ucInstNo = 0;
    QString strTemp = "";
    QPixmap pixmap(":/Application/images/splash_2.png");
    CustomSplashScreen splash(pixmap);
    char ucTemp[256] ={0};

    for( ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucDSOCount; ucInstNo++)
    {
        /*For DSO Open and Detection*/
        if(m_objDSOWrapper.DP_Scope_Open(ucInstNo , gGlobalStuct.m_sBrdDetectionConfig.m_szDSOLName[ucInstNo-1]))
        {
            splash.show();
            splash.showMessage(QObject::tr("DSO is not initiliazed"),Qt::AlignLeft | Qt::AlignBottom,Qt::red);
            gGlobalStuct.sCOTSInstInfo.m_bDSO[ucInstNo-1] = false;
            strcpy(gGlobalStuct.sCOTSInstInfo.m_cDSOInfo[ucInstNo -1], "Not Available");
            strTemp = QString::asprintf("DSO - %d is not initiliazed",ucInstNo);
            Printlog(PRINTLOG_TYPE_FAILURE, strTemp);
        }
        else
        {
            splash.show();
            splash.showMessage(QObject::tr("DSO is initiliazed"),Qt::AlignLeft | Qt::AlignBottom,Qt::green);
            gGlobalStuct.sCOTSInstInfo.m_bDSO[ucInstNo-1] = true;
            for(U8BIT ucLoop = 8; ucLoop <= 18; ucLoop++)
            {
                ucTemp[ucLoop-8] = gGlobalStuct.m_sBrdDetectionConfig.m_szDSOLName[ucInstNo-1][ucLoop];
            }
            strcpy(gGlobalStuct.sCOTSInstInfo.m_cDSOInfo[ucInstNo -1], ucTemp);
            strTemp = QString::asprintf("DSO - %d is initiliazed",ucInstNo);
            Printlog(PRINTLOG_TYPE_SUCCESS, strTemp);
        }
    }
    this->repaint();

    return DP_SUCCESS;
}
/*******************************************************************************
 * Name                         : InitializationBoards
 * Author                       : Poovarasan S
 * Global Variables affected	: NIL
 * Created Date                 : July 16, 2023
 ***************************************************************************//**
 * @brief	This function is used to Open the IO Boards
 *
 * @param   NA
 *
 * @return	NA
 ******************************************************************************/
S16BIT MainWindow::InitializationBoards()
{
    S16BIT s16RetVal                = 0;
    S8BIT out_ps8ErrMsg[512]        = {0};
    QString qstrTempMsg;
    U8BIT  u8BoardNo        = 0;
    qstrTempMsg.clear();

    u8BoardNo = 2;

#ifdef DP3039_BRD_ENABLE   /*DP-IP-3039-300*/

    s16RetVal = m_objIP3039Wrapper.Open(u8BoardNo, m_sin3039devicelocation, m_sout3039devicelocation);
    if(s16RetVal)
    {
        m_objIP3039Wrapper.GetErrorMessage(s16RetVal, out_ps8ErrMsg);
        Printlog(PRINTLOG_TYPE_FAILURE, out_ps8ErrMsg);
        b3039BrdSts[0]  = false;
        b3039BrdSts[1]  = false;
        //sleep(100);
    }
    else
    {
        qstrTempMsg = QString::asprintf("DP-IP-3039 Module Open Successfully!");
        Printlog(PRINTLOG_TYPE_SUCCESS, qstrTempMsg);

        b3039BrdSts[0]  = m_sout3039devicelocation[0].m_bOpenIP3039;
        b3039BrdSts[1]  = m_sout3039devicelocation[1].m_bOpenIP3039;

        for(u8BoardNo = 1 ; u8BoardNo <= 2 ;u8BoardNo++)
        {
            s16RetVal = m_objIP3039Wrapper.Reset(u8BoardNo);
            if(s16RetVal)
            {
                m_objIP3039Wrapper.GetErrorMessage(s16RetVal, out_ps8ErrMsg);
                Printlog(PRINTLOG_TYPE_FAILURE, out_ps8ErrMsg);
            }
            //sleep(100);
        }
    }
#endif

    return DP_SUCCESS;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString strAppName = "";
    QMessageBox msgbox;
    strAppName =  "Report Generator";
    if(m_pobjProcess->state() == QProcess::Running)
    {
        QString StrAppName = QString::asprintf("%s Application is running in other instance, Please close that application to close this Application", (strAppName.toStdString().c_str()));

        msgbox.setText(StrAppName);
        msgbox.setWindowTitle("Report Generator");
        msgbox.addButton(QMessageBox::Ok);
        msgbox.setDefaultButton(QMessageBox::Ok);
        msgbox.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

        if (msgbox.exec() == QMessageBox::Ok)
        {
            event->ignore();
        }
    }
    if(m_bIsTestActive)
    {
        QMessageBox::information(this,"Warning","Current Test Case in process");
        event->ignore();
    }
    else
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit",
                                                                    tr("Do you want to exit?"),
                                                                    QMessageBox::QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes )
        {
            if(m_bIsTestActive)
            {
                event->ignore();
            }
            else
            {
                event->accept();
            }
        }
        else
        {
            event->ignore();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
    {
        e->ignore();
    }
    else
    {
        ;
    }
}

void MainWindow::on_actionHome_triggered()
{
    ui->swMainWindow->setCurrentIndex(0);
}

void MainWindow::on_actionUserManagment_triggered()
{
    ui->swMainWindow->setCurrentIndex(4);
}

void MainWindow::on_action_Communication_triggered()
{
    ui->swMainWindow->setCurrentIndex(1);
}

void MainWindow::on_action_ATE_Test_Cases_triggered()
{
    ui->swMainWindow->setCurrentIndex(2);
}

void MainWindow::on_actionA_bout_triggered()
{
    m_pobABout->setWindowTitle("About Panel");
    m_pobABout->show();
    m_pobABout->setModal(true);
    m_pobABout->exec();
}

void MainWindow::MenuBarEnDis(unsigned char ucEnDisSts)
{
    ui->menubar->setEnabled(ucEnDisSts);
}


short MainWindow::DP_GetErrorMessage(S32BIT in_s32ErrCode, PS8BIT out_ps8ErrStringBuf, U16BIT in_u16BufSize)
{
    if(out_ps8ErrStringBuf == nullptr)
        return  DP_APP_INVALID_POINTER;

    switch(in_s32ErrCode)
    {
    case DP_APP_INVALID_HANDLE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid Handle");
        break;

    case DP_APP_WRITE_FAIL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "FPGA Write Failed");
        break;

    case DP_APP_READ_FAIL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "FPGA Read Failed");
        break;

    case DP_APP_ERR_INCORRECT_SIZE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Incorrect Bulk data size");
        break;

    case DP_APP_DPSPL6847_INCORRECT_BRDID:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Incorrect TestBoard ID Read");
        break;

    case DP_APP_DPSPL6847_INCORRECT_TYPEID:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Incorrect TestBoard TypeID Read");
        break;

    case DP_APP_INVALID_RF_BITE_PATHSEL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid RF/BITE Path Selection");
        break;

    case DP_APP_INVALID_FILTER_BYPASS_PATHSEL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid Filter/Bypass Path Selection");
        break;

    case DP_APP_ERR_BAND_CONFIG_FAIL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Frequency Band Configuration Failed");
        break;

    case DP_APP_ERR_INVALID_DCA_SEL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid DCA Selection");
        break;

    case DP_APP_ERR_INVALID_ATTEN_DATA_LIMIT:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid DCA Attenuation Data");
        break;

    case DP_APP_ERR_SET_ATTEN_FAIL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DCA Attenuation Configuration Failed");
        break;
    case DP_APP_ERR_COM_PORT_NOT_OPENED:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DP_APP_ERR:: COM port not opened !");
        break;
    case DP_APP_RS232_PORT_BUSY:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DP_APP_ERR:: Unable to Open the COM Port..COM Port Busy !");
        break;
   case DP_APP_ERR_INVALID_PARAM:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "DP_APP_ERR:: Invalid Param!");
        break;

    case DP_APP_SOCKET_CREATE_FAIL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Socket Creation failed!");
        break;
    case DP_APP_ERR_INVALID_SOCKET:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: Error Invalid Socket.");
        break;
    case DP_APP_CRC_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: CRC Error.");
        break;
    case DP_APP_DATA_NA_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: Data Not Available Error.");
        break;
    case DP_APP_MSGKIND_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: Message Kind Error.");
        break;
    case DP_APP_CMD_NOTOK_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: Comnmand NotOk Error.");
        break;
    case DP_APP_READ_ACK_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: Acknowledgement Read Error.");
        break;
    case DP_APP_OP_LOAD_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: Output Load Error.");
        break;
    case DP_APP_CMD_ID_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: Command Identifier Error.");
        break;
    case DP_APP_MSG_COUNT_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: PSM Message Counter Error.");
        break;
    case DP_APP_RESP_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: PSM Response Error.");
        break;
    case DP_APP_RWR_ERR_BRD_DET_PATH:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "DP_APP_ERR:: Unable to read the COTS & Instruments default configuration");
        break;
    case DP_APP_ERR_DB_FILE_PATH:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to open the Database connect configuration file");
        break;
    case DP_APP_ERR_DB_SERVER_IP:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to read the Server IP from configuration file");
        break;
    case DP_APP_ERR_DB_UID:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to read the UID from configuration file");
        break;
    case DP_APP_ERR_DB_PASSWORD:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to read the Password from configuration file");
        break;
    case DP_APP_ERR_DB_NAME:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to read the Database name from configuration file");
        break;
    case DP_APP_ERR_PORT_NO:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to read the Port Number from configuration file");
        break;
    case DP_APP_ERR_SYSTEM_NAME:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to read the system name from configuration file");
        break;
    case DP_APP_ERR_OPEN_DB:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to connect to Report Database");
        break;
    case DP_APP_INSERT_DETAIL_ERR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to insert the user detail into Database");
        break;
    case DP_APP_ERR_VERIFY_DETAILS:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Please enter the correct Username and Password");
        break;
    case DP_APP_ERR_READ_USER_DETAILS:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to read user detail from Database");
        break;
    case DR_RDR_7343_ERR_TR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to insert test registeration details into the Database");
        break;
    case DP_APP_ERR_UPDATE_USER_DETAILS:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to update user details into the Database");
        break;
    case DP_APP_ERR_CONN_DATABASE:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Unable to connect Database");
        break;
    case DP_APP_MARIADB_SERVICE_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "MariadB Service Not Enabled");
        break;
    case DP_APP_ERR_INVALID_CHANNEL:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid Channel Selection");
        break;
    case DP_APP_ERR_INVALID_TIMEOUT:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid Timeout");
        break;
    case DP_APP_ERR_ETH_SEND_ERR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Socket send error");
        break;
    case DP_APP_ERR_ETH_RECV_ERR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Socket receive error");
        break;
    case DP_APP_SOCKET_BIND_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Socket bind error");
        break;
    case DP_APP_ERR_INVALID_POINTER:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Invalid pointer variable");
        break;
    case DP_APP_SOCKET_CONN_ERROR:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Socket Connect Error");
        break;
    case DP_APP_ERR_HEADER_MISMATCH:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize,  "Header Mismatch Error");
        break;

    default:
        snprintf(out_ps8ErrStringBuf, in_u16BufSize, "Invalid Error Code !");
    }

    return DP_SUCCESS;
}

short MainWindow::ReadSGSANAInstrumentDetails()
{
    S8BIT szSecKey[128]		= {0};
    QString strTemp = "";
    QString strIPConfig = "";
    QString strTempPath             = "";
    QString strFilePath             = "";
    QSettings *ptrInstDetailsFile = NULL;

    strTempPath = qApp->applicationDirPath();

    strFilePath = strTempPath + "/INPUT_FILES/" + "RFInstrumentDetails.ini";
    QFileInfo objfileInfo(strFilePath);
    if (objfileInfo.exists() && objfileInfo.isFile())
    {
        ptrInstDetailsFile = new(QSettings)(strFilePath, QSettings::IniFormat);
    }
    else
    {
        return PRINTLOG_TYPE_FAILURE;
    }

    /********Specrtrum Analyzer***********/
    sprintf(szSecKey , "INST_CONFIG/TOT_SA_COUNT");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {

        return PRINTLOG_TYPE_FAILURE;
    }

    g_ucSACount = strTemp.toUInt(0,10);

    for(U8BIT u8Idx = 0; u8Idx < g_ucSACount; u8Idx++)
    {
        sprintf(szSecKey, "INST_CONFIG/SA_%d_MANUFACTURER", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objSAWrapper.m_SAInsconfg[u8Idx].m_szManufacturer, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/SA_%d_MODEL", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objSAWrapper.m_SAInsconfg[u8Idx].m_szmodel, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/SA_%d_ID", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        m_objSAWrapper.m_SAInsconfg[u8Idx].m_usInstID = strTemp.toUInt(0,10);
    }

    /********Signal Generator***********/
    sprintf(szSecKey , "INST_CONFIG/TOT_SG_COUNT");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return PRINTLOG_TYPE_FAILURE;
    }

    g_ucSGCount = strTemp.toUInt(0,10);

    for(U8BIT u8Idx = 0; u8Idx < g_ucSGCount; u8Idx++)
    {
        sprintf(szSecKey, "INST_CONFIG/SG_%d_MANUFACTURER", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
           return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objRFSGWrapper.m_InstCfg[u8Idx].szManufacturer, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/SG_%d_MODEL", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objRFSGWrapper.m_InstCfg[u8Idx].szModel, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/SG_%d_ID", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        m_objRFSGWrapper.m_InstCfg[u8Idx].usInstID = strTemp.toUInt(0,10);
    }

    /********Network Analyzer***********/
    sprintf(szSecKey , "INST_CONFIG/TOT_NA_COUNT");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
       return PRINTLOG_TYPE_FAILURE;
    }

    g_ucNACount = strTemp.toUInt(0,10);

    for(U8BIT u8Idx = 0; u8Idx < g_ucNACount; u8Idx++)
    {
        sprintf(szSecKey, "INST_CONFIG/NA_%d_MANUFACTURER", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objNAWrap.m_InstCfg[u8Idx].m_szManufacturer, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/NA_%d_MODEL", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objNAWrap.m_InstCfg[u8Idx].m_szModel, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/NA_%d_ID", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
           return PRINTLOG_TYPE_FAILURE;
        }

        m_objNAWrap.m_InstCfg[u8Idx].m_usInstruementID = strTemp.toUInt(0,10);
    }

    /********Power Meter***********/
    sprintf(szSecKey , "INST_CONFIG/TOT_PM_COUNT");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
       return PRINTLOG_TYPE_FAILURE;
    }

    g_ucPMCount = strTemp.toUInt(0,10);

    for(U8BIT u8Idx = 0; u8Idx < g_ucPMCount; u8Idx++)
    {
        sprintf(szSecKey, "INST_CONFIG/PM_%d_MANUFACTURER", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objPMWrap.m_InstCfg[u8Idx].m_PM_Manufacturer, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/PM_%d_MODEL", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objPMWrap.m_InstCfg[u8Idx].m_PM_model, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/PM_%d_ID", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
           return PRINTLOG_TYPE_FAILURE;
        }

        m_objPMWrap.m_InstCfg[u8Idx].usInstID = strTemp.toUInt(0,10);
    }

    /********Function Generator***********/
    sprintf(szSecKey , "INST_CONFIG/TOT_FG_COUNT");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
       return PRINTLOG_TYPE_FAILURE;
    }

    g_ucFGCount = strTemp.toUInt(0,10);

    for(U8BIT u8Idx = 0; u8Idx < g_ucFGCount; u8Idx++)
    {
        sprintf(szSecKey, "INST_CONFIG/FG_%d_MANUFACTURER", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objFGWrapper.m_arrSFGInsconfg[u8Idx].m_szManufacturer, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/FG_%d_MODEL", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objFGWrapper.m_arrSFGInsconfg[u8Idx].m_szmodel, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/FG_%d_ID", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
           return PRINTLOG_TYPE_FAILURE;
        }

        m_objFGWrapper.m_arrSFGInsconfg[u8Idx].m_usInstID = strTemp.toUInt(0,10);
    }

    /********Digital Signal Oscilloscope***********/
    sprintf(szSecKey , "INST_CONFIG/TOT_DSO_COUNT");
    if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
    {
        return PRINTLOG_TYPE_FAILURE;
    }

    m_objDSOWrapper.m_u8AvailDSOCount = strTemp.toUInt(0,10);

    for(U8BIT u8Idx = 0; u8Idx < m_objDSOWrapper.m_u8AvailDSOCount; u8Idx++)
    {
        sprintf(szSecKey, "INST_CONFIG/DSO_%d_MANUFACTURER", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objDSOWrapper.m_InstDSOCfg[u8Idx].szManufacturer, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/DSO_%d_MODEL", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        strcpy(m_objDSOWrapper.m_InstDSOCfg[u8Idx].szModel, strTemp.toStdString().c_str());

        sprintf(szSecKey , "INST_CONFIG/DSO_%d_ID", (u8Idx + 1));
        if((strTemp = ptrInstDetailsFile->value(szSecKey).toString()) == NULL)
        {
            return PRINTLOG_TYPE_FAILURE;
        }

        m_objDSOWrapper.m_InstDSOCfg[u8Idx].usInstID = strTemp.toUInt(0,10);
    }

    return 0;
}

void MainWindow::on_actionCableloss_Calibration_triggered()
{
    ui->swMainWindow->setCurrentIndex(3);
}

void MainWindow::on_actionReport_Generator_triggered()
{
    QString strReportGenExePath  = "";
    QString strExePath = "";

    strReportGenExePath = qApp->applicationDirPath() + "/ReportGenerator/ReportGenerator.exe";

    QFileInfo check_file(strReportGenExePath);
    if(!(check_file.exists() && check_file.isFile()))
    {
        QMessageBox::information(this, "Information", "Input File not Existed");
        return;
    }

    strExePath = "\""+strReportGenExePath+"\"";

    m_pobjProcess->start(strExePath);

    bool bRetVal =  m_pobjProcess->waitForStarted();
    if(bRetVal == false)
    {
        Printlog(0, "Timeout Occured");
    }
}
