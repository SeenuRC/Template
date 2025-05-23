#include "crfinstdetection.h"
#include "ui_crfinstdetection.h"
#include "mainwindow.h"

extern MainWindow *p_objMainApp;
extern SGlobalStucture gGlobalStuct;
bool g_bCloseFlagEvent;
extern unsigned char g_ucSACount;
extern unsigned char g_ucSGCount;
extern unsigned char g_ucNACount;
extern unsigned char g_ucPMCount;
extern unsigned char g_ucFGCount;
CRFInstDetection::CRFInstDetection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRFInstDetection)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->twSigGen->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twSpectrumAnalyzer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twNetworkAnalyzer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twOscillocope->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFont font;
    font.setPointSize(10);
    font.setFamily("Arial");
    /*Signal Generator*/

    ui->tb_wid_IP_Addr->setHorizontalHeaderLabels(QStringList()<< tr("Instruements") << tr("IP Address"));
    ui->tb_wid_IP_Addr->horizontalHeader()->setFixedHeight(25);
    ui->tb_wid_IP_Addr->horizontalHeader()->setSectionsClickable(false);
    ui->tb_wid_IP_Addr->horizontalHeader()->setVisible(true);
    ui->tb_wid_IP_Addr->setFont(font);
    ui->tb_wid_IP_Addr->setColumnWidth(0, 80);
    ui->tb_wid_IP_Addr->setColumnWidth(1, 170);
    ui->tb_wid_IP_Addr->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tb_wid_IP_Addr->verticalHeader()->setDefaultSectionSize(20);
    ui->tb_wid_IP_Addr->horizontalHeader()->setVisible(true);
    ui->tb_wid_IP_Addr->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    ui->twSigGen->setHorizontalHeaderLabels(QStringList()<< tr("Model Number") << tr("Manufacturar"));
    ui->twSigGen->horizontalHeader()->setFixedHeight(25);
    ui->twSigGen->horizontalHeader()->setSectionsClickable(false);
    ui->twSigGen->horizontalHeader()->setFixedHeight(25);
    ui->twSigGen->horizontalHeader()->setVisible(true);
    ui->twSigGen->setFont(font);
    ui->twSigGen->setColumnWidth(0, 80);
    ui->twSigGen->setColumnWidth(1, 170);
    ui->twSigGen->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twSigGen->verticalHeader()->setDefaultSectionSize(20);
    ui->twSigGen->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    /*Signal Analyzer*/
    ui->twSpectrumAnalyzer->setHorizontalHeaderLabels(QStringList()<< tr("Model Number") << tr("Manufacturar"));
    ui->twSpectrumAnalyzer->horizontalHeader()->setSectionsClickable(false);
    ui->twSpectrumAnalyzer->horizontalHeader()->setVisible(true);
    ui->twSpectrumAnalyzer->horizontalHeader()->setFixedHeight(25);
    ui->twSpectrumAnalyzer->setFont(font);
    ui->twSpectrumAnalyzer->setColumnWidth(0, 80);
    ui->twSpectrumAnalyzer->setColumnWidth(1, 170);
    ui->twSpectrumAnalyzer->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twSpectrumAnalyzer->verticalHeader()->setDefaultSectionSize(20);
    ui->twSpectrumAnalyzer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /*Network Analyzer*/
    ui->twNetworkAnalyzer->setHorizontalHeaderLabels(QStringList()<< tr("Model Number") << tr("Manufacturar"));
    ui->twNetworkAnalyzer->horizontalHeader()->setSectionsClickable(false);
    ui->twNetworkAnalyzer->horizontalHeader()->setVisible(true);
    ui->twNetworkAnalyzer->horizontalHeader()->setFixedHeight(25);
    ui->twNetworkAnalyzer->setFont(font);
    ui->twNetworkAnalyzer->setColumnWidth(0, 80);
    ui->twNetworkAnalyzer->setColumnWidth(1, 200);
    ui->twNetworkAnalyzer->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twNetworkAnalyzer->verticalHeader()->setDefaultSectionSize(20);
    ui->twNetworkAnalyzer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /*Power Meter*/
    ui->twPowerMeter->setHorizontalHeaderLabels(QStringList()<< tr("Model Number") << tr("Manufacturar"));
    ui->twPowerMeter->horizontalHeader()->setSectionsClickable(false);
    ui->twPowerMeter->horizontalHeader()->setVisible(true);
    ui->twPowerMeter->horizontalHeader()->setFixedHeight(25);
    ui->twPowerMeter->setFont(font);
    ui->twPowerMeter->setColumnWidth(0, 80);
    ui->twPowerMeter->setColumnWidth(1, 170);
    ui->twPowerMeter->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twPowerMeter->verticalHeader()->setDefaultSectionSize(20);
    ui->twPowerMeter->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /*DSO*/
    ui->twOscillocope->setHorizontalHeaderLabels(QStringList()<< tr("Model Number") << tr("Manufacturer"));
    ui->twOscillocope->horizontalHeader()->setSectionsClickable(false);
    ui->twOscillocope->horizontalHeader()->setVisible(true);
    ui->twOscillocope->horizontalHeader()->setFixedHeight(25);
    ui->twOscillocope->setFont(font);
    ui->twOscillocope->setColumnWidth(0, 80);
    ui->twOscillocope->setColumnWidth(1, 170);
    ui->twOscillocope->verticalHeader()->setSectionResizeMode(QHeaderView::Custom);
    ui->twOscillocope->verticalHeader()->setDefaultSectionSize(20);
    ui->twOscillocope->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    InitShow();
    ui->pbConfigure->setFocus();
}

CRFInstDetection::~CRFInstDetection()
{
    delete ui;
}

void CRFInstDetection::on_pbConfigure_clicked()
{
    QString strIPAddress = "";
    QString strIPConfig = "";
    QString strFilePath  = "";
    QString strTempPath  = "";
    unsigned char ucRow = 0;
    QSettings *ptrInstDetailsFile = NULL;
    PS8BIT pszSecName = NULL;
    S8BIT szSecKey[128]		= {0};
    QString strTmp = ";";
    strTempPath = qApp->applicationDirPath();

    strFilePath = strTempPath + "/INPUT_FILES/" + "BrdDetails.ini";

    QFileInfo check_DB_file(strFilePath);
    if (check_DB_file.exists() && check_DB_file.isFile())
    {
        ptrInstDetailsFile = new(QSettings)(strFilePath, QSettings::IniFormat);
    }
    else
    {
        return;
    }

    pszSecName = (PS8BIT)"INST_CONFIG";

    for(unsigned char ucSiggenIdx = 0; ucSiggenIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount; ucSiggenIdx++)
    {
         strIPAddress = ((QLineEdit *)ui->tb_wid_IP_Addr->cellWidget(ucSiggenIdx, 1))->text();
         strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szSGLName[ucSiggenIdx],strIPAddress.toStdString().c_str());
         strIPConfig  = QString::asprintf("TCPIP0::%s::inst0::INSTR",  strIPAddress.toStdString().c_str());
         sprintf(szSecKey , "%s/SG_%d_LNAME", pszSecName,(ucSiggenIdx+1));
         ptrInstDetailsFile->setValue(szSecKey, strIPConfig.toStdString().c_str());
    }

    for(unsigned char ucSpecIdx = 0; ucSpecIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount; ucSpecIdx++)
    {
         ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + ucSpecIdx;
         strIPAddress = ((QLineEdit *)ui->tb_wid_IP_Addr->cellWidget(ucRow, 1))->text();
         strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szSALName[ucSpecIdx],strIPAddress.toStdString().c_str());
         strIPConfig  = QString::asprintf("TCPIP0::%s::inst0::INSTR",  strIPAddress.toStdString().c_str());
         sprintf(szSecKey , "%s/SA_%d_LNAME", pszSecName,(ucSpecIdx+1));
         ptrInstDetailsFile->setValue(szSecKey, strIPConfig.toStdString().c_str());
    }

    for(unsigned char ucNAIdx = 0; ucNAIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount; ucNAIdx++)
    {
         ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + ucNAIdx;
         strIPAddress = ((QLineEdit *)ui->tb_wid_IP_Addr->cellWidget(ucRow, 1))->text();
         strcpy(gGlobalStuct.m_sBrdDetectionConfig.m_szNALName[ucNAIdx],strIPAddress.toStdString().c_str());
         strIPConfig  = QString::asprintf("TCPIP0::%s::inst0::INSTR",  strIPAddress.toStdString().c_str());
         sprintf(szSecKey , "%s/NA_%d_LNAME", pszSecName,(ucNAIdx+1));
         ptrInstDetailsFile->setValue(szSecKey, strIPConfig.toStdString().c_str());
    }

    g_bCloseFlagEvent = 1;

    delete ptrInstDetailsFile;
    this->close();

}

void CRFInstDetection::InitShow()
{
    unsigned char ucRow = 0;
    QString strInstModelName = "";
    QString strInstManufName = "";
    char ucLoop     =  0;
    char value[50]  = {0};
    QString strTemp             = "";
    QString strTempPath     = "";
    QString strFilePath     = "";
    QString strTmp          = ";";
    strTempPath = qApp->applicationDirPath();
    QString ipRange = "(([ 0]+)|([ 0]*[0-9] *)|([0-9][0-9] )|([ 0][0-9][0-9])|(1[0-9][0-9])|([2][0-4][0-9])|(25[0-5]))";

    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator ipValidator(ipRegex, this);

    ui->tb_wid_IP_Addr->setRowCount((gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucFGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucDSOCount)-1);
    ui->tb_wid_IP_Addr->setColumnCount(2);
    ui->tb_wid_IP_Addr->verticalHeader()->hide();

    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount; u8InsNumRowIdx++)
    {
        memset(value,0,sizeof(value));
        ui->tb_wid_IP_Addr->insertRow(u8InsNumRowIdx);
        QLineEdit *lineEdit = new QLineEdit(ui->tb_wid_IP_Addr);
        lineEdit->setValidator(&ipValidator);
        lineEdit->setInputMask("000.000.000.000");
        /*Signal Generator*/
        strcpy(value, gGlobalStuct.m_sBrdDetectionConfig.m_szSGLName[u8InsNumRowIdx]);
        for(ucLoop = 8; ucLoop <= 18; ucLoop++)
        {
            gGlobalStuct.sCOTSInstInfo.m_cSGInfo[u8InsNumRowIdx][ucLoop-8] = value[ucLoop];
        }
        strTemp = QString::asprintf("%s",gGlobalStuct.sCOTSInstInfo.m_cSGInfo);
        lineEdit->setText(strTemp);
        ui->tb_wid_IP_Addr->setCellWidget(u8InsNumRowIdx,1,lineEdit);
        strTemp.sprintf("Signal Generator %d",(u8InsNumRowIdx+1));
        ui->tb_wid_IP_Addr->setItem(u8InsNumRowIdx, 0, new QTableWidgetItem(strTemp));
    }

    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount; u8InsNumRowIdx++)
    {
        memset(value,0,sizeof(value));
        strTemp = "";
        QLineEdit *lineEdit = new QLineEdit(ui->tb_wid_IP_Addr);
        lineEdit->setValidator(&ipValidator);
        lineEdit->setInputMask("000.000.000.000");
        /*Spectrum Analyzer*/
        strcpy(value, gGlobalStuct.m_sBrdDetectionConfig.m_szSALName[u8InsNumRowIdx]);
        for(ucLoop = 8; ucLoop <= 18; ucLoop++)
        {
             gGlobalStuct.sCOTSInstInfo.m_cSAInfo[u8InsNumRowIdx][ucLoop-8] = value[ucLoop];
        }
        strTemp = QString::asprintf("%s",gGlobalStuct.sCOTSInstInfo.m_cSAInfo);
        lineEdit->setText(strTemp);
        ui->tb_wid_IP_Addr->setCellWidget((gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + u8InsNumRowIdx),1,lineEdit);
        strTemp.sprintf("Spectrum Analyzer %d",(u8InsNumRowIdx+1));
        ui->tb_wid_IP_Addr->setItem(gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + u8InsNumRowIdx, 0, new QTableWidgetItem(strTemp));
    }

    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount; u8InsNumRowIdx++)
    {
        ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + u8InsNumRowIdx;
        QLineEdit *lineEdit = new QLineEdit(ui->tb_wid_IP_Addr);
        lineEdit->setValidator(&ipValidator);
        lineEdit->setInputMask("000.000.000.000");
        /*Network Analyzer*/
        strcpy(value, gGlobalStuct.m_sBrdDetectionConfig.m_szNALName[u8InsNumRowIdx]);
        for(ucLoop = 8; ucLoop <= 18; ucLoop++)
        {
            gGlobalStuct.sCOTSInstInfo.m_cNAInfo[u8InsNumRowIdx][ucLoop-8] = value[ucLoop];
        }
        strTemp = QString::asprintf("%s",gGlobalStuct.sCOTSInstInfo.m_cNAInfo);
        lineEdit->setText(strTemp);
        ui->tb_wid_IP_Addr->setCellWidget(ucRow,1,lineEdit);
        strTemp.sprintf("Network Analyzer %d",(u8InsNumRowIdx+1));
        ui->tb_wid_IP_Addr->setItem(ucRow, 0, new QTableWidgetItem(strTemp));
    }

    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount; u8InsNumRowIdx++)
    {
        ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount + u8InsNumRowIdx;
        QLineEdit *lineEdit = new QLineEdit(ui->tb_wid_IP_Addr);
        lineEdit->setValidator(&ipValidator);
        lineEdit->setInputMask("000.000.000.000");
        /*Power Meter*/
        strcpy(value, gGlobalStuct.m_sBrdDetectionConfig.m_szPMLName[u8InsNumRowIdx]);
        for(ucLoop = 8; ucLoop <= 18; ucLoop++)
        {
            gGlobalStuct.sCOTSInstInfo.m_cPMInfo[u8InsNumRowIdx][ucLoop-8] = value[ucLoop];
        }
        strTemp = QString::asprintf("%s",gGlobalStuct.sCOTSInstInfo.m_cPMInfo);
        lineEdit->setText(strTemp);
        ui->tb_wid_IP_Addr->setCellWidget(ucRow,1,lineEdit);
        strTemp.sprintf("Power Meter %d",(u8InsNumRowIdx+1));
        ui->tb_wid_IP_Addr->setItem(ucRow, 0, new QTableWidgetItem(strTemp));
    }

    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucFGCount; u8InsNumRowIdx++)
    {
        ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount + u8InsNumRowIdx;
        QLineEdit *lineEdit = new QLineEdit(ui->tb_wid_IP_Addr);
        lineEdit->setValidator(&ipValidator);
        lineEdit->setInputMask("000.000.000.000");
        /*Function Generator*/
        strcpy(value, gGlobalStuct.m_sBrdDetectionConfig.m_szFGLName[u8InsNumRowIdx]);
        for(ucLoop = 8; ucLoop <= 18; ucLoop++)
        {
            gGlobalStuct.sCOTSInstInfo.m_cFGInfo[u8InsNumRowIdx][ucLoop-8] = value[ucLoop];
        }
        strTemp = QString::asprintf("%s",gGlobalStuct.sCOTSInstInfo.m_cFGInfo);
        lineEdit->setText(strTemp);
        ui->tb_wid_IP_Addr->setCellWidget(ucRow,1,lineEdit);
        strTemp.sprintf("Function Generator %d",(u8InsNumRowIdx+1));
        ui->tb_wid_IP_Addr->setItem(ucRow, 0, new QTableWidgetItem(strTemp));
    }

    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < gGlobalStuct.m_sBrdDetectionConfig.m_ucDSOCount; u8InsNumRowIdx++)
    {
        memset(value,0,sizeof(value));
        strTemp = "";
        ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucFGCount + u8InsNumRowIdx;
        QLineEdit *lineEdit = new QLineEdit(ui->tb_wid_IP_Addr);
        lineEdit->setValidator(&ipValidator);
        lineEdit->setInputMask("000.000.000.000");
        /*Spectrum Analyzer*/
        strcpy(value, gGlobalStuct.m_sBrdDetectionConfig.m_szDSOLName[u8InsNumRowIdx]);
        for(ucLoop = 8; ucLoop <= 18; ucLoop++)
        {
             gGlobalStuct.sCOTSInstInfo.m_cDSOInfo[u8InsNumRowIdx][ucLoop-8] = value[ucLoop];
        }
        strTemp = QString::asprintf("%s",gGlobalStuct.sCOTSInstInfo.m_cDSOInfo);
        lineEdit->setText(strTemp);
        ui->tb_wid_IP_Addr->setCellWidget(ucRow,1,lineEdit);
        strTemp.sprintf("Oscilloscope  %d",(u8InsNumRowIdx+1));
        ui->tb_wid_IP_Addr->setItem(ucRow, 0, new QTableWidgetItem(strTemp));
    }

    /*Signal Generator*/
    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < g_ucSGCount; u8InsNumRowIdx++)
    {
        strInstModelName = QString::asprintf("%s", p_objMainApp->m_objRFSGWrapper.m_InstCfg[u8InsNumRowIdx].szModel);
        strInstManufName = QString::asprintf("%s", p_objMainApp->m_objRFSGWrapper.m_InstCfg[u8InsNumRowIdx].szManufacturer);
        ui->twSigGen->insertRow(u8InsNumRowIdx);
        ui->twSigGen->setItem(u8InsNumRowIdx, 0, new QTableWidgetItem(strInstModelName));
        ui->twSigGen->setItem(u8InsNumRowIdx, 1, new QTableWidgetItem(strInstManufName));
    }

    /*Signal Analyzer*/
    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < g_ucSACount; u8InsNumRowIdx++)
    {
        strInstModelName = QString::asprintf("%s", p_objMainApp->m_objSAWrapper.m_SAInsconfg[u8InsNumRowIdx].m_szmodel);
        strInstManufName = QString::asprintf("%s", p_objMainApp->m_objSAWrapper.m_SAInsconfg[u8InsNumRowIdx].m_szManufacturer);
        ui->twSpectrumAnalyzer->insertRow(u8InsNumRowIdx);
        ui->twSpectrumAnalyzer->setItem(u8InsNumRowIdx, 0, new QTableWidgetItem(strInstModelName));
        ui->twSpectrumAnalyzer->setItem(u8InsNumRowIdx, 1, new QTableWidgetItem(strInstManufName));
    }

    /*Network Analyzer*/
    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < g_ucNACount; u8InsNumRowIdx++)
    {
        strInstModelName = QString::asprintf("%s", p_objMainApp->m_objNAWrap.m_InstCfg[u8InsNumRowIdx].m_szModel);
        strInstManufName = QString::asprintf("%s", p_objMainApp->m_objNAWrap.m_InstCfg[u8InsNumRowIdx].m_szManufacturer);
        ui->twNetworkAnalyzer->insertRow(u8InsNumRowIdx);
        ui->twNetworkAnalyzer->setItem(u8InsNumRowIdx, 0, new QTableWidgetItem(strInstModelName));
        ui->twNetworkAnalyzer->setItem(u8InsNumRowIdx, 1, new QTableWidgetItem(strInstManufName));
    }

    /*Power Meter*/
    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < g_ucPMCount; u8InsNumRowIdx++)
    {
        strInstModelName = QString::asprintf("%s", p_objMainApp->m_objPMWrap.m_InstCfg[u8InsNumRowIdx].m_PM_model);
        strInstManufName = QString::asprintf("%s", p_objMainApp->m_objPMWrap.m_InstCfg[u8InsNumRowIdx].m_PM_Manufacturer);
        ui->twPowerMeter->insertRow(u8InsNumRowIdx);
        ui->twPowerMeter->setItem(u8InsNumRowIdx, 0, new QTableWidgetItem(strInstModelName));
        ui->twPowerMeter->setItem(u8InsNumRowIdx, 1, new QTableWidgetItem(strInstManufName));
    }

    /*Function Generator*/
    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < g_ucFGCount; u8InsNumRowIdx++)
    {
        strInstModelName = QString::asprintf("%s", p_objMainApp->m_objFGWrapper.m_arrSFGInsconfg[u8InsNumRowIdx].m_szmodel);
        strInstManufName = QString::asprintf("%s", p_objMainApp->m_objFGWrapper.m_arrSFGInsconfg[u8InsNumRowIdx].m_szManufacturer);
        ui->twFunctionGenerator->insertRow(u8InsNumRowIdx);
        ui->twFunctionGenerator->setItem(u8InsNumRowIdx, 0, new QTableWidgetItem(strInstModelName));
        ui->twFunctionGenerator->setItem(u8InsNumRowIdx, 1, new QTableWidgetItem(strInstManufName));
    }

    /*Oscilloscope*/
    for(U8BIT u8InsNumRowIdx = 0; u8InsNumRowIdx < p_objMainApp->m_objDSOWrapper.m_u8AvailDSOCount; u8InsNumRowIdx++)
    {
        strInstModelName = QString::asprintf("%s", p_objMainApp->m_objDSOWrapper.m_InstDSOCfg[u8InsNumRowIdx].szModel);
        strInstManufName = QString::asprintf("%s", p_objMainApp->m_objDSOWrapper.m_InstDSOCfg[u8InsNumRowIdx].szManufacturer);
        ui->twOscillocope->insertRow(u8InsNumRowIdx);
        ui->twOscillocope->setItem(u8InsNumRowIdx, 0, new QTableWidgetItem(strInstModelName));
        ui->twOscillocope->setItem(u8InsNumRowIdx, 1, new QTableWidgetItem(strInstManufName));
    }

}

void CRFInstDetection::closeEvent(QCloseEvent *event)
{

    if(g_bCloseFlagEvent == 0)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit",
                                                                    tr("Do you want to exit?"),
                                                                    QMessageBox::QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if(resBtn == QMessageBox::Yes)
        {
            event->accept();
            exit(0);
        }
        else
        {
            event->ignore();
        }
    }

}

void CRFInstDetection::paintEvent(QPaintEvent *)
{
    ui->twSigGen->setColumnWidth(0, ui->twSigGen->width()* 0.17);
    ui->twSigGen->setColumnWidth(1, ui->twSigGen->width()* 0.37);
}


void CRFInstDetection::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
    {
        e->ignore();
    }
}

