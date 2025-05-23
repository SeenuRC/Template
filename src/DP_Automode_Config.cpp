#include "DP_Automode_Config.h"
#include "ui_DP_Automode_Config.h"
#include <QFileDialog>

bool g_bAutoTestStartStop;
unsigned char g_ucPressYES_NO;
extern SGlobalStucture gGlobalStuct;
extern MainWindow *p_objMainApp;
extern unsigned char ucFailCnt[30];
extern unsigned char ucSuccessCnt[30];

#define SET_STYLESHEET(QObj,QSS_File) {\
    QFile file;\
    file.setFileName(QSS_File);\
    file.open(QIODevice::ReadOnly | QIODevice::Text);\
    QTextStream TxtStream(&file);\
    QObj.setStyleSheet(TxtStream.readAll());\
    file.close();\
    }

DP_AutoModeTest_Config::DP_AutoModeTest_Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DP_AutoModeTest_Config)
{
    ui->setupUi(this);

    pAutoTestThread = new CAutoTestThread(this);

    ui->sbMajorCycleCount->setRange(1, 10);
    ui->sbDelayBwTestCase->setRange(1, 60);
    ui->sbDelayBwMajorCycle->setRange(1, 60);
    ui->sbDelayBwMinorCycle->setRange(1, 60);

    ui->sbMajorCycleCount->setValue(1);
    ui->sbDelayBwMajorCycle->setValue(1);
    ui->sbDelayBwTestCase->setValue(1);
    ui->sbDelayBwMinorCycle->setValue(1);

    initTestSelTable();
    connect(this,&DP_AutoModeTest_Config::sig_MenuEnaDisSts, ((MainWindow *)this->parent()), &MainWindow::MenuBarEnDis);
    connect(this,&DP_AutoModeTest_Config::sig_MsgToPrintlog, ((MainWindow *)this->parent()), &MainWindow::Printlog);
    connect(ui->twAutoModeTestCase,SIGNAL(cellChanged(int,int)),this,SLOT(slot_tablewidgetclicked(int,int)));
    connect(ui->twAutoModeTestCase,SIGNAL(cellChanged(int,int)),this,SLOT(slot_tablewidgetclicked(int,int)));
    connect(pAutoTestThread, SIGNAL(sig_SetMajorCycleProgressbarRange(int, int)), this, SLOT(Slot_SetMajorCyclePrgrBarRange(int, int)));
    connect(pAutoTestThread, SIGNAL(sig_SetMinorCycleProgressbarRange(int, int)), this, SLOT(Slot_SetMinorCyclePrgrBarRange(int, int)));
    connect(pAutoTestThread, SIGNAL(sig_SetOverAllProgressbarRange(int, int)), this, SLOT(Slot_SetOverAllPrgrBarRange(int, int)));
    connect(pAutoTestThread, SIGNAL(sig_QusToUser(char*)),this,SLOT(slot_QusToUser(char*)),Qt::BlockingQueuedConnection);
    connect(pAutoTestThread, SIGNAL(sig_QusToUserContinueSkip(char*)),this,SLOT(slot_QusToUserContinueSkip(char*)),Qt::BlockingQueuedConnection);
    connect(pAutoTestThread, SIGNAL(sig_UpdateMajorProgressBar(int)), this, SLOT(Slot_UpdateMajorProgressBar(int)),Qt::BlockingQueuedConnection);
    connect(pAutoTestThread , &CAutoTestThread::sig_MsgToPrintlog, ((MainWindow *)this->parent()), &MainWindow::Printlog);
    connect(pAutoTestThread, SIGNAL(sig_ChangeStartStopEnDis(bool)), this, SLOT(slot_ChangeStartStopEnDis(bool)),Qt::BlockingQueuedConnection);
    connect(pAutoTestThread, SIGNAL(sig_UpdateUI(unsigned char, unsigned long, unsigned char, unsigned long, unsigned char, unsigned long, int)), this, SLOT(UpdateUI(unsigned char, unsigned long, unsigned char, unsigned long, unsigned char, unsigned long, int)),Qt::BlockingQueuedConnection);
    connect(pAutoTestThread, SIGNAL(Sig_Fail_State(bool*, QString)), this, SLOT(Slot_Fail_State(bool*, QString)),Qt::BlockingQueuedConnection);

}


DP_AutoModeTest_Config::~DP_AutoModeTest_Config()
{
    delete ui;
}

void DP_AutoModeTest_Config::initTestSelTable()
{
    U32BIT u32row               = 0;
    U32BIT u32TestcaseListCnt   = 0;
    U32BIT u32TestVectorSize    = 0;
    U32BIT uiColumnCount        = 0;

    U8BIT u8TestIdx = 0;
    QStringList qsliCalibrationTestList ;

    p_objMainApp->qsliAutoTestCaseList.clear();
    ui->twAutoModeTestCase->setHorizontalHeaderLabels(QStringList()<< tr("Test Case") << tr("Minor Cycle\nCount (1 - 10)")<<tr("Success Count")<<tr("Fail Count")<<tr("Result"));
    ui->twAutoModeTestCase->horizontalHeader()->setSectionsClickable(false);
    ui->twAutoModeTestCase->horizontalHeader()->setFixedHeight(50);
    ui->twAutoModeTestCase->horizontalHeader()->setVisible(true);

    ui->twAutoModeTestCase->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    u32TestVectorSize = p_objMainApp->m_qVecTestCaseName.size();
    for(U8BIT u8Idx = 0; u8Idx < u32TestVectorSize; u8Idx++)
    {
        p_objMainApp->qsliAutoTestCaseList.append((QString)p_objMainApp->m_qVecTestCaseName.at(u8Idx).m_szTestCaseName);
        pAutoTestThread->m_STestConfig.m_strTestName[u8Idx] = p_objMainApp->qsliAutoTestCaseList.at(u8TestIdx).toStdString().c_str();
    }

    uiColumnCount = ui->twAutoModeTestCase->columnCount();
    for(U32BIT column = 1; column < uiColumnCount; column++)
    {
        if(column != 1)
        {
            ui->twAutoModeTestCase->setItemDelegateForColumn(column, new NotEditableDelegate);
        }
    }

    u32TestcaseListCnt = p_objMainApp->qsliAutoTestCaseList.size();
    for(u32row = 0; u32row < (u32TestcaseListCnt - 1) ; u32row++)
    {
        ui->twAutoModeTestCase->insertRow(u32row);
        ui->twAutoModeTestCase->setItem(u32row, 0, new QTableWidgetItem(p_objMainApp->qsliAutoTestCaseList[u32row]));
        ui->twAutoModeTestCase->item(u32row, 0)->setCheckState(Qt::Unchecked);

        QSpinBox *SpinBox = new QSpinBox;
        SpinBox->setRange(1, 10);
        SpinBox->setValue(1);
        SpinBox->setAlignment(Qt::AlignCenter);
        SpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        SpinBox->setAccessibleName("TABLE");

        ui->twAutoModeTestCase->setCellWidget(u32row, 1, SpinBox);
        ui->twAutoModeTestCase->setItem(u32row, 2, new QTableWidgetItem("0"));
        ui->twAutoModeTestCase->setItem(u32row, 3, new QTableWidgetItem("0"));
        ui->twAutoModeTestCase->item(u32row, 2)->setTextAlignment(Qt::AlignCenter);
        ui->twAutoModeTestCase->item(u32row, 3)->setTextAlignment(Qt::AlignCenter);
        ui->twAutoModeTestCase->item(u32row, 0)->setFlags(ui->twAutoModeTestCase->item(0,0)->flags() & ~Qt::ItemIsEditable);
        ui->twAutoModeTestCase->item(u32row, 2)->setFlags(ui->twAutoModeTestCase->item(0,2)->flags() & ~Qt::ItemIsEditable);
        ui->twAutoModeTestCase->item(u32row, 3)->setFlags(ui->twAutoModeTestCase->item(0,3)->flags() & ~Qt::ItemIsEditable);

        m_SpinBox[u32row] = SpinBox;
    }
}

void DP_AutoModeTest_Config::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
    {
        e->ignore();
    }
}

void DP_AutoModeTest_Config::paintEvent(QPaintEvent *)
{
    ui->twAutoModeTestCase->setColumnWidth(0, ui->twAutoModeTestCase->width()* 0.47);
    ui->twAutoModeTestCase->setColumnWidth(1, ui->twAutoModeTestCase->width()* 0.17);
    ui->twAutoModeTestCase->setColumnWidth(2, ui->twAutoModeTestCase->width()* 0.20);
    ui->twAutoModeTestCase->setColumnWidth(3, ui->twAutoModeTestCase->width()* 0.17);
}


void DP_AutoModeTest_Config::on_pbTestStart_clicked()
{
    unsigned char  ucTestCnt        = 0;
    unsigned char  ucTestCaseCnt    = 0;
    unsigned char  ucCycleCnt		= 0;
    unsigned short usTotCycleCnt    = 0;
    unsigned short usRange          = 0;
    unsigned int   uiTotRec         = 0;
    QTableWidgetItem* pItem         = NULL;
    QTableWidgetItem* pItem1        = NULL;
    QString strCycleCnt             = "";
    QString strTemp1                = "";
    uiTotRec =   ui->twAutoModeTestCase->rowCount();

    //check for invalid cycle number
    for(ucTestCaseCnt = 0; ucTestCaseCnt < uiTotRec; ucTestCaseCnt++)
    {
        pItem = ui->twAutoModeTestCase->item(ucTestCaseCnt, 2);
        pItem->setText("0/0");
        pItem1 = ui->twAutoModeTestCase->item(ucTestCaseCnt, 3);
        pItem1->setText("0/0");
    }
    ctestinformation objTestinfo;
    SET_STYLESHEET(objTestinfo, "./DP_APP_QSS.qss");
    objTestinfo.setWindowTitle("Test Information");
    objTestinfo.exec();

    if(p_objMainApp->m_btrFlag == false)
    {
        return;
    }
    else
    {
        strTemp1 = "Test Registration detail insert into the database Success";
        p_objMainApp->Printlog(PRINTLOG_TYPE_SUCCESS, strTemp1);
    }

    m_ucTestCnt = 0;

    //update the selected testcases in the table
    for(ucTestCaseCnt = 0; ucTestCaseCnt < uiTotRec; ucTestCaseCnt++)
    {
        if(ui->twAutoModeTestCase->item(ucTestCaseCnt,0)->checkState() == Qt::Checked)
        {
            //store the selected testcase ID in array
            m_ucarrTestID[m_ucTestCnt] = ucTestCaseCnt + 1;
            ucCycleCnt = m_SpinBox[ucTestCaseCnt]->value();
            usTotCycleCnt += ucCycleCnt;
            m_ucarrCycleCnt[m_ucTestCnt] = ucCycleCnt;
            ucTestCnt += ucCycleCnt;
            m_ucTestCnt++;
        }
    }
    if(!m_ucTestCnt)
    {
        strTemp1 = QString::asprintf("%s", "Please select atleast any one test case");
        QMessageBox :: information(this, "Warning", strTemp1);
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_INFO, strTemp1);
        ui->pbTestStart->setEnabled(true);
        ui->pbTestStop->setEnabled(false);
        return;
    }

    /*Total Testcases = Total No.of cycles * Major Cycle Count*/
    m_uiMajCyclDelay = ui->sbDelayBwMajorCycle->value();
    m_uiDelay        = ui->sbDelayBwTestCase->value();
    m_usTotMajorCnt   = ui->sbMajorCycleCount->value();

    usRange = usTotCycleCnt * m_usTotMajorCnt;

    //Progress Bar Configuration
    ui->pbar_OverallAMT->setRange(0, usRange);
    ui->pbar_OverallAMT->setValue(0);
    ui->pbar_MajorAMT->setValue(0);
    ui->pbar_MinorAMT->setValue(0);
    bTestExecSts = true;//update global flag
    slot_ChangeStartStopEnDis(1);

    m_uiMinorCyclDelay = ui->sbDelayBwMinorCycle->value();

    memset(pAutoTestThread->m_uiAutoTestSuccessCnt, 0, sizeof(pAutoTestThread->m_uiAutoTestSuccessCnt));
    memset(pAutoTestThread->m_uiAutoTestFailCnt, 0, sizeof(pAutoTestThread->m_uiAutoTestFailCnt));
    memset(ucFailCnt, 0, sizeof(ucFailCnt));
    memset(ucSuccessCnt, 0, sizeof(ucSuccessCnt));
    memset(p_objMainApp->m_arr8ChSel,0,sizeof(p_objMainApp->m_arr8ChSel));
    g_bAutoTestStartStop = true;
    pAutoTestThread->start();//thread begin

}

void DP_AutoModeTest_Config::on_pbTestStop_clicked()
{
    g_bAutoTestStartStop = 0;
    EnableDisableGUI();
}

void DP_AutoModeTest_Config::on_cbTestSelectAll_clicked(bool checked)
{
    for(U8BIT ucRowCnt = 0; ucRowCnt < ui->twAutoModeTestCase->rowCount(); ucRowCnt++)
    {
        if(checked)
        {
            ui->twAutoModeTestCase->item(ucRowCnt, 0)->setCheckState(Qt::Checked);
        }
        else
        {
            ui->twAutoModeTestCase->item(ucRowCnt, 0)->setCheckState(Qt::Unchecked);
        }
    }
}

void  DP_AutoModeTest_Config::slot_tablewidgetclicked(int irow, int iCol)
{
    U8BIT ucLoop = 0;
    int iRowCount = 0;
    unsigned char ucSelCount = 0;

    iRowCount= ui->twAutoModeTestCase->rowCount();
    for(ucLoop = 0; ucLoop < iRowCount; ucLoop++)
    {
        if(ui->twAutoModeTestCase->item(ucLoop, 0)->checkState() == Qt::Checked)
        {
            ucSelCount += 1;
        }
    }

    if(ucSelCount == p_objMainApp->m_qVecTestCaseName.size()-1)
    {
        ui->cbTestSelectAll->setChecked(true);
    }
    else
    {
        ui->cbTestSelectAll->setChecked(false);
    }
}

void DP_AutoModeTest_Config::EnableDisableGUI()
{
    U8BIT ucRowIdx = 0;
    if(g_bAutoTestStartStop == 1)//Thread start
    {
        p_objMainApp->m_bIsTestActive = true;
        emit sig_MenuEnaDisSts(false);
        for(ucRowIdx = 0; ucRowIdx < ui->twAutoModeTestCase->rowCount(); ucRowIdx++)
        {
            m_SpinBox[ucRowIdx]->setEnabled(false);
            ui->twAutoModeTestCase->item(ucRowIdx, 0)->setFlags(ui->twAutoModeTestCase->item(ucRowIdx, 0)->flags() & ~Qt::ItemIsUserCheckable);
        }
        ui->cbTestSelectAll->setEnabled(false);
        ui->sbMajorCycleCount->setEnabled(false);
        ui->sbDelayBwTestCase->setEnabled(false);
        ui->sbDelayBwMajorCycle->setEnabled(false);
        ui->sbDelayBwMinorCycle->setEnabled(false);
        ui->pbTestStart->setEnabled(false);
        ui->pbTestStop->setEnabled(true);
    }
    else//thread stop
    {
        p_objMainApp->m_bIsTestActive = false;
        emit sig_MenuEnaDisSts(true);
        for(ucRowIdx = 0; ucRowIdx < ui->twAutoModeTestCase->rowCount(); ucRowIdx++)
        {
            m_SpinBox[ucRowIdx]->setEnabled(true);
            ui->twAutoModeTestCase->item(ucRowIdx, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        }
        ui->cbTestSelectAll->setEnabled(true);
        ui->sbMajorCycleCount->setEnabled(true);
        ui->sbDelayBwTestCase->setEnabled(true);
        ui->sbDelayBwMajorCycle->setEnabled(true);
        ui->sbDelayBwMinorCycle->setEnabled(true);
        ui->pbTestStart->setEnabled(true);
        ui->pbTestStop->setEnabled(false);
    }

    return;
}

/**************************************************************************
// Name						:	 Slot_UpdateMajorProgressBar()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to update the major progress bar value
**************************************************************************/
void DP_AutoModeTest_Config::Slot_UpdateMajorProgressBar(int iValue)
{
    ui->pbar_MajorAMT->setValue(iValue);
}
/**************************************************************************
// Name						:	 Slot_UpdateMinorProgressBar()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to update minor the progress bar value
**************************************************************************/
void DP_AutoModeTest_Config::Slot_UpdateMinorProgressBar(int iValue)
{
    ui->pbar_MinorAMT->setValue(iValue);
}
/**************************************************************************
// Name						:	 Slot_UpdateOverallProgressBar()
// Author					:
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to update the overall progress bar value
**************************************************************************/
void DP_AutoModeTest_Config::Slot_UpdateOverallProgressBar(int iValue)
{
    ui->pbar_OverallAMT->setValue(iValue);
}

void DP_AutoModeTest_Config::Slot_SetMajorCyclePrgrBarRange(int iMin, int iMax)
{
    ui->pbar_MajorAMT->setRange(iMin, iMax);
}

void DP_AutoModeTest_Config::Slot_SetMinorCyclePrgrBarRange(int iMin, int iMax)
{
    ui->pbar_MinorAMT->setRange(iMin, iMax);
}

void DP_AutoModeTest_Config::Slot_SetOverAllPrgrBarRange(int iMin, int iMax)
{
    ui->pbar_OverallAMT->setRange(iMin, iMax);
}


void DP_AutoModeTest_Config::AutoTestGUIUpdate(unsigned char in_ucTestCase, bool in_bAutoTestSts)
{
    QString strData = "";
    if(in_bAutoTestSts == 0)
    {
        pAutoTestThread->m_uiAutoTestSuccessCnt[in_ucTestCase]++;
        strData = QString::asprintf("%u", pAutoTestThread->m_uiAutoTestSuccessCnt[in_ucTestCase]);
        ui->twAutoModeTestCase->setItem((in_ucTestCase), 2, new QTableWidgetItem(strData));
        ui->twAutoModeTestCase->item(in_ucTestCase, 2)->setTextAlignment(Qt::AlignCenter);
        ui->twAutoModeTestCase->item((in_ucTestCase), 2)->setBackground(QBrush(QColor(128, 255, 126)));
    }
    else
    {
        pAutoTestThread->m_uiAutoTestFailCnt[in_ucTestCase]++;
        strData = QString::asprintf("%u", pAutoTestThread->m_uiAutoTestFailCnt[in_ucTestCase]);
        ui->twAutoModeTestCase->setItem((in_ucTestCase), 3, new QTableWidgetItem(strData));
        ui->twAutoModeTestCase->item(in_ucTestCase, 3)->setTextAlignment(Qt::AlignCenter);
        ui->twAutoModeTestCase->item((in_ucTestCase), 3)->setBackground(QBrush(QColor(255, 124, 124)));
    }
}

void DP_AutoModeTest_Config::Slot_Fail_State( bool *out_bTstSts, QString qstr_Msg)
{
    QMessageBox::information(this,"Warning", qstr_Msg);
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE , qstr_Msg);
    *out_bTstSts = 1;
}

void DP_AutoModeTest_Config::slot_QusToUser(char *pszUsrmessage)
{
    QString strText = "";
    strText = QString::asprintf("%s",pszUsrmessage);
    switch(QMessageBox::question(this,"Auto Mode Test Cases",strText, QMessageBox::Yes, QMessageBox::No))
    {
    case QMessageBox::Yes:
        g_ucPressYES_NO = 1;
        break;
    case QMessageBox::No:
        g_ucPressYES_NO = 0;
        break;
    default:
        break;
    }

    return ;
}
void DP_AutoModeTest_Config::Slot_FileOpen()
{
    QString	m_strFilePathName = "";
    m_strFilePathName = QFileDialog::getOpenFileName(this, "Open","./InputFiles/","Text Files (*.txt)");
}

void DP_AutoModeTest_Config::slot_QusToUserContinueSkip(char *pszUsrmessage)
{
    QString strText = "";
    strText = QString::asprintf("%s",pszUsrmessage);


    int iRetVal = QMessageBox::information(this, "Application", strText, "Continue", "Skip");

    if(iRetVal == 0)
    {
        g_ucPressYES_NO = 1;
    }
    else
    {
        g_ucPressYES_NO = 0;
    }

    return;
}

void DP_AutoModeTest_Config::slot_ChangeStartStopEnDis(bool bStatus)
{
    U8BIT u8RowIdx = 0;
    if(bStatus)//thread start
    {
        for(u8RowIdx = 0; u8RowIdx < ui->twAutoModeTestCase->rowCount(); u8RowIdx++)
        {
            m_SpinBox[u8RowIdx]->setEnabled(false);
            ui->twAutoModeTestCase->item(u8RowIdx, 0)->setFlags(ui->twAutoModeTestCase->item(u8RowIdx, 0)->flags() & ~Qt::ItemIsUserCheckable);
        }
        ui->cbTestSelectAll->setEnabled(false);
        ui->sbMajorCycleCount->setEnabled(false);
        ui->sbDelayBwTestCase->setEnabled(false);
        ui->sbDelayBwMajorCycle->setEnabled(false);
        ui->sbDelayBwMinorCycle->setEnabled(false);
        ui->pbTestStart->setEnabled(false);
        ui->pbTestStop->setEnabled(true);
        emit sig_MenuEnaDisSts(false);
    }
    else//thread stop
    {
        for(u8RowIdx = 0; u8RowIdx < ui->twAutoModeTestCase->rowCount(); u8RowIdx++)
        {
            m_SpinBox[u8RowIdx]->setEnabled(true);
            ui->twAutoModeTestCase->item(u8RowIdx, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        }
        ui->cbTestSelectAll->setEnabled(true);
        ui->sbMajorCycleCount->setEnabled(true);
        ui->sbDelayBwTestCase->setEnabled(true);
        ui->sbDelayBwMajorCycle->setEnabled(true);
        ui->sbDelayBwMinorCycle->setEnabled(true);
        ui->pbTestStart->setEnabled(true);
        ui->pbTestStop->setEnabled(false);
        emit sig_MenuEnaDisSts(true);
    }
}

void DP_AutoModeTest_Config::UpdateUI(unsigned char in_ucSuccessCnt, unsigned long in_ulTotSuccessCnt, unsigned char in_ucFailCnt, unsigned long in_ulTotFailCnt, unsigned char in_ucTestCnt, unsigned long ulOverallProg, int iCurrCycleNo)
{
    QTableWidgetItem* pItem         = NULL;
    QTableWidgetItem* pItem1        = NULL;
    QString strTemp                 = "";

    strTemp = QString::asprintf("%d/%ld",in_ucSuccessCnt, in_ulTotSuccessCnt);
    pItem = ui->twAutoModeTestCase->item((in_ucTestCnt-1), 2);
    pItem->setText(strTemp.toStdString().c_str());

    strTemp = QString::asprintf("%d/%ld",in_ucFailCnt, in_ulTotFailCnt);
    pItem1 = ui->twAutoModeTestCase->item((in_ucTestCnt-1), 3);
    pItem1->setText(strTemp.toStdString().c_str());

    ui->pbar_MinorAMT->setValue(iCurrCycleNo);
    ui-> pbar_OverallAMT->setValue(ulOverallProg);

}

