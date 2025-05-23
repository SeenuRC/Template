#include "calibration.h"
#include "ui_calibration.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>
//#include "macros.h"
//#include "Database/CReportWrapper.h"
#include "DP_CalibThread.h"
extern MainWindow *p_objMainApp;

#define PEAK_VALUE_ENABLED

extern U8BIT g_ucCalibthread;
extern U8BIT g_ucTotaltestSel;
extern U32BIT  g_uiTotRowCount;
extern U16BIT g_ucEquipment;
extern U8BIT g_ucarrTestrecord[11];
extern QString g_strlist[48];
extern bool bTestExecSts;
extern U8BIT g_ucTestStopped;

extern CReportConfigWrapper *m_pobjReportWrapper;

S8BIT arrCalibTestName[48] = {0};
U8BIT g_uc_CmbCalib_Index = 0;
U8BIT g_ucTotalspot = 0;
U8BIT g_ucSetMaxloopCount = 0;

#define SET_STYLESHEET(QObj,QSS_File) {\
    QFile file;\
    file.setFileName(QSS_File);\
    file.open(QIODevice::ReadOnly | QIODevice::Text);\
    QTextStream TxtStream(&file);\
    QObj.setStyleSheet(TxtStream.readAll());\
    file.close();\
    }

CCalibration::CCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCalibration)
{
    ui->setupUi(this);

    connect(ui->tw_CalibrationTest,SIGNAL(cellChanged(int,int)),this,SLOT(slot_tablewidgetclicked(int,int)));
    //connect(pobjCalibThread, SIGNAL(Sig_Fail_State(bool*, QString)), this, SLOT(Slot_Fail_State(bool*, QString)),Qt::BlockingQueuedConnection);
    //connect(this,&DP_AutoModeTest_Config::sig_MenuEnaDisSts, ((MainWindow *)this->parent()), &MainWindow::MenuBarEnDis);
    connect(this,&CCalibration::sig_MsgToPrintlog, ((MainWindow *)this->parent()), &MainWindow::Printlog);

    initTable();

    cThreadStoped = 0;
}

CCalibration::~CCalibration()
{
    delete ui;
}

void CCalibration::initTable()
{
    U16BIT usLoop                       = 0;

    QStringList list                    = {};
    QString strFilePath                 ;
    QFile qfConfigFile                  ;
    QTextStream objFileStream           ;

    ui->tw_CalibrationTest->setRowCount(0);
    ui->tw_CalibrationTest->setColumnCount(0);

    ui->tw_CalibrationTest->horizontalHeader()->setStyleSheet("QHeaderView::section {font: bold 14px;}");
    ui->tw_CalibrationTest->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tw_CalibrationTest->setColumnCount(1);
    ui->tw_CalibrationTest->setHorizontalHeaderLabels(QStringList()<< tr("Test Name"));
    ui->tw_CalibrationTest->horizontalHeader()->setSectionsClickable(false);
    ui->tw_CalibrationTest->horizontalHeader()->setVisible(true);
    ui->tw_CalibrationTest->setFixedWidth(502);
    ui->tw_CalibrationTest->setColumnWidth(0, 502);

    strFilePath = qApp->applicationDirPath();

    strFilePath = strFilePath + "/INPUT_FILES/Calibration.csv";

    qfConfigFile.setFileName(strFilePath);

    if(!qfConfigFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    objFileStream.setDevice(&qfConfigFile);
    objFileStream.readLine();

    while(!objFileStream.atEnd())
    {
        sprintf(arrCalibTestName,objFileStream.readLine().split(",").at(0).toStdString().c_str(),(usLoop + 1));
        ui->tw_CalibrationTest->insertRow(usLoop);
        ui->tw_CalibrationTest->setItem(usLoop,0, new QTableWidgetItem(QString::asprintf("   %s",arrCalibTestName)));
        ui->tw_CalibrationTest->item(usLoop,0)->setCheckState(Qt::Unchecked);
        ui->tw_CalibrationTest->item(usLoop,0)->setBackground(QBrush(QColor(0, 200,188))); //188
        usLoop++;
    }
    qfConfigFile.close();
}

void CCalibration::on_cb_CalibSelAll_clicked(bool checked)
{
    S32BIT iLoop                       = 0;
    S32BIT iuRowCount                  = 0;

    iuRowCount = ui->tw_CalibrationTest->rowCount();

    for(iLoop = 0; iLoop < iuRowCount; iLoop++)
    {
        if(checked)
        {
            ui->tw_CalibrationTest->item(iLoop,0)->setCheckState(Qt::Checked);
        }
        else
        {
            ui->tw_CalibrationTest->item(iLoop,0)->setCheckState(Qt::Unchecked);
        }
    }
}

void  CCalibration::slot_tablewidgetclicked(int iuRow, int iCol)
{
    S32BIT iLoop                           = 0;
    S32BIT iuRowCount                      = 0;
    U8BIT ucSelCount                       = 0;

    iuRowCount= ui->tw_CalibrationTest->rowCount();

    for(iLoop = 0; iLoop < iuRowCount; iLoop++)
    {
        if(ui->tw_CalibrationTest->item(iLoop, 0)->checkState() == Qt::Checked)
        {
            ucSelCount += 1;
        }
    }
    if(iuRowCount == ucSelCount)
    {
        ui->cb_CalibSelAll->setChecked(true);
    }
    else
    {
        ui->cb_CalibSelAll->setChecked(false);
    }
}

void CCalibration::on_pb_Calibstart_clicked()
{
    g_ucTotaltestSel                    = 0;
    g_ucTestStopped                     = 1;

    U8BIT ucCount                       = 0;

    QString  strTemp                    = "";

    connect(pobjCalibThread , SIGNAL(sig_ThreadStop()), this,SLOT(slot_ThreadStop()),Qt::UniqueConnection);

    ui->pIndCalibprogBar->reset();
    ui->pOverCalibprogBar->reset();

    memset(g_ucarrTestrecord,0,sizeof(g_ucarrTestrecord));
    g_uiTotRowCount  =   ui->tw_CalibrationTest->rowCount();

    ctestinformation objTestinfo;
    SET_STYLESHEET(objTestinfo, "./DP_APP_QSS.qss");
    objTestinfo.setWindowTitle("Test Information");
    objTestinfo.exec();

    for (ucCount = 0; ucCount < g_uiTotRowCount ; ucCount++)
    {
        if(ui->tw_CalibrationTest->item(ucCount,0)->checkState() == Qt::Checked)
        {
            g_ucTotaltestSel +=1;
            g_ucarrTestrecord[ucCount] = ucCount + 1;
            g_strlist[ucCount] = ui->tw_CalibrationTest->item(ucCount,0)->text();
        }
    }    

    if(!g_ucTotaltestSel)
    {
        strTemp = "Please Select Any One Test case...";
        QMessageBox::information(this, "failure", strTemp);
        return;
    }
    else
    {
        g_ucCalibthread = 0;
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO,"Cable loss calibration started");

        emit Sig_ConfigMenuBar(false);

        ui->pOverCalibprogBar->setMaximum(g_ucTotaltestSel);
        ui->pIndCalibprogBar->setMaximum(5);

        ui->pb_Calibstop->setEnabled(true);
        ui->pb_Calibstart->setEnabled(false);
        ui->tw_CalibrationTest->setEnabled(false);
        ui->cb_CalibSelAll->setEnabled(false);

        pobjCalibThread->Start();

        ui->pIndCalibprogBar->reset();
        ui->pOverCalibprogBar->reset();
    }
}

/**************************************************************************
// Name						:	 Slot_MessageBox()
// Author					:	 Venkatesh S
// Global Variables affected:	 NA
// Created Date				:    13-07-2023
// Revision Date			:
// Reason for Revising		:
// Description				:	To call message box from thread
**************************************************************************/
void CCalibration::Slot_MessageBox(S16BIT sWarningType,QString strMsgHeader,QString strMessage)
{
    switch (sWarningType)
    {
    case 1:
        QMessageBox::information(this,strMsgHeader,strMessage);
        break;
    case 2:
        QMessageBox::warning(this,strMsgHeader,strMessage);
        break;
    }
}

void CCalibration::on_pb_Calibstop_clicked()
{
    g_ucTestStopped = 0;
}

void CCalibration::slot_ThreadStop()
{
    S16BIT sRetVal    = 0;
    // Disable RF Out in SG
    sRetVal =  p_objMainApp->m_objRFSGWrapper.DP_Enumeration_RFSigGen_ConfigOutputEnabled(m_SSGInst[0].m_uiInstance, false);
    if(sRetVal)
    {
        QMessageBox::warning(this,"Calibration failed","Signal generator power off failed");
    }

    ui->pb_Calibstart->setEnabled(true);
    ui->pb_Calibstop->setEnabled(false);
    ui->tw_CalibrationTest->setEnabled(true);
    ui->cb_CalibSelAll->setEnabled(true);
    emit Sig_ConfigMenuBar(true);

    if(g_ucTestStopped == 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO,"Cable loss calibration stopped");
    }
    else
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO,"Cable loss calibration finished");
    }
    g_ucTestStopped = 0;
}

/**************************************************************************
// Name						:	 Slot_TotTest_ProgBar()
// Author					:	 Venkatesh S
// Global Variables affected:	 NA
// Created Date				:    21-08-2023
// Revision Date			:
// Reason for Revising		:
// Description				:	To update the total progressbar value in GUI
**************************************************************************/
void CCalibration::Slot_TotTest_ProgBar(char in_cValue)
{
    S32BIT s32Value             = 0;

    s32Value = in_cValue;

    ui->pOverCalibprogBar->setValue(s32Value);
}

/**************************************************************************
// Name						:	 Slot_TotTest_ProgBar()
// Author					:	 Venkatesh S
// Global Variables affected:	 NA
// Created Date				:    31-08-2023
// Revision Date			:
// Reason for Revising		:
// Description				:	To update the Individual progressbar value in GUI
**************************************************************************/
void CCalibration::Slot_IndTest_ProgBar(S32BIT iValue)
{
    ui->pIndCalibprogBar->setValue(iValue);
}

/**************************************************************************
// Name						:	 Slot_SetMaxTotTest_ProgBar()
// Author					:	 Venkatesh S
// Global Variables affected:	 NA
// Created Date				:    21-08-2023
// Revision Date			:
// Reason for Revising		:
// Description				:	To update the Individual progressbar value in GUI
**************************************************************************/
void CCalibration::Slot_SetIndTotTest_ProgBar(S32BIT iStart,S32BIT iStop,S32BIT iStep)
{
    S32BIT iValue       = 0;

    iValue = (iStop - iStart)/iStep;

    ui->pOverCalibprogBar->setMaximum(iValue);
}
