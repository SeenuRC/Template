#include "cinstrumentdetection.h"
#include "ui_cinstrumentdetection.h"
#include "mainwindow.h"


extern SGlobalStucture gGlobalStuct;

cinstrumentdetection::cinstrumentdetection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cinstrumentdetection)
{
    ui->setupUi(this);

    ui->tw_InstDetection->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Instrument
    ui->tw_InstDetection->setHorizontalHeaderLabels(QStringList()<< tr("Instrument Name") << tr("IP Address")<<tr("Open status"));
    ui->tw_InstDetection->horizontalHeader()->setSectionsClickable(false);
    ui->tw_InstDetection->horizontalHeader()->setVisible(true);

    ui->tw_InstDetection->setFixedWidth(800);
    ui->tw_InstDetection->setColumnWidth(0, 200);
    ui->tw_InstDetection->setColumnWidth(1, 300);
    ui->tw_InstDetection->setColumnWidth(2, 100);
    ui->tw_InstDetection->setColumnWidth(3, 50);
    //Instrument Detection

    ui->tw_InstDetection->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tw_InstDetection->verticalHeader()->setDefaultSectionSize(40);
    ui->tw_InstDetection->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(this ,&cinstrumentdetection::sig_MsgToPrintlog, ((MainWindow *)this->parent()),&MainWindow::Printlog);
    OnAddCots();
}

cinstrumentdetection::~cinstrumentdetection()
{
    delete ui;
}
/**************************************************************************
// Name						:	 on_pbProceed_clicked()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To Proceed the next GUI panel
**************************************************************************/
void cinstrumentdetection::on_pbProceed_clicked()
{
    emit sig_ConnectComm();
}

void cinstrumentdetection::EnaDisdisplay_control(bool in_bEnDisable)
{
    if(in_bEnDisable == false)
    {
        ui->pbProceed->setEnabled(false);
    }
    else
    {
        ui->pbProceed->setEnabled(true);
    }

    ui->tw_InstDetection->currentColumn();

}
/**************************************************************************
// Name						:	 OnAddCots()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To initialize the COTS instrument
**************************************************************************/
void cinstrumentdetection::OnAddCots()
{
    int s32RetVal = 0;
    unsigned char		ucInstNo	  = 0;
    QString     strInstname	  = "";
    QString		strInstrInfo  = "";
    unsigned char ucBoardCnt = 0;
    QString strCotsName ="";
    QString strTemp ="";
    unsigned char ucRow = 0;
    QPixmap pixmap(":/Application/images/reconnect.png");
    QFont font;


    font.setPointSize(8);
    font.setFamily("Arial");

    ui->tw_InstDetection->setFont(font);

    /*	Instrument initialization*/
    //Signal generator
    for(ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount; ucInstNo++)
    {
        strInstname.sprintf("Signal Generator %d",ucInstNo);
        ui->tw_InstDetection->insertRow(ucRow);
        strInstrInfo.sprintf("IP Address : %s",gGlobalStuct.sCOTSInstInfo.m_cSGInfo[ucInstNo-1]);
        if(gGlobalStuct.sCOTSInstInfo.m_bSigGen[ucBoardCnt] == false)
        {
            QString strBoardOpen ="Not Opened";
            ui->tw_InstDetection->setItem(ucRow,2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(255, 126, 126)));
        }
        else
        {
            QString strBoardOpen ="Opened";
            ui->tw_InstDetection->setItem(ucRow,2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(126, 255, 126)));
        }

        ui->tw_InstDetection->setItem(ucRow,0,new QTableWidgetItem(strInstname));
        ui->tw_InstDetection->setItem(ucRow,1,new QTableWidgetItem(strInstrInfo));
        ucRow++;
    }
    // Spectrum Analyzer
    ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount;
    for(ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount; ucInstNo++)
    {
        ui->tw_InstDetection->insertRow(ucRow);
        strInstname.sprintf("Signal Analyzer %d", ucInstNo);
        strInstrInfo = "";
        strInstrInfo.sprintf("IP Address : %s",gGlobalStuct.sCOTSInstInfo.m_cSAInfo[ucInstNo-1]);
        if(gGlobalStuct.sCOTSInstInfo.m_bSA[ucBoardCnt] == false)
        {
            QString strBoardOpen ="Not Opened";
            ui->tw_InstDetection->setItem(ucRow,2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(255, 126, 126)));
        }
        else
        {
            QString strBoardOpen ="Opened";
            ui->tw_InstDetection->setItem(ucRow,2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(126, 255, 126)));
        }

        ui->tw_InstDetection->setItem(ucRow,0,new QTableWidgetItem(strInstname));
        ui->tw_InstDetection->setItem(ucRow,1,new QTableWidgetItem(strInstrInfo));
        ucRow++;
    }
    ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount;
    // Nerwork Analyzer
    for(ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount; ucInstNo++)
    {
        ui->tw_InstDetection->insertRow(ucRow);
        strInstname.sprintf("Network Analyzer %d",ucInstNo);
        strInstrInfo = "";
        strInstrInfo = QString::asprintf("IP Address : %s", gGlobalStuct.sCOTSInstInfo.m_cNAInfo[ucInstNo-1]);
        if(gGlobalStuct.sCOTSInstInfo.m_bNA == false)
        {
            QString strBoardOpen ="Not Opened";
            ui->tw_InstDetection->setItem(ucRow, 2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(255, 126, 126)));
        }
        else
        {
            QString strBoardOpen ="Opened";
            ui->tw_InstDetection->setItem(ucRow, 2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(126, 255, 126)));
        }

        ui->tw_InstDetection->setItem(ucRow,0,new QTableWidgetItem(strInstname));
        ui->tw_InstDetection->setItem(ucRow,1,new QTableWidgetItem(strInstrInfo));
        ucRow++;

    }

    ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount;
    // Power Meter
    for(ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount; ucInstNo++)
    {
        ui->tw_InstDetection->insertRow(ucRow);
        strInstname.sprintf("Power Meter %d",ucInstNo);
        strInstrInfo = "";
        strInstrInfo = QString::asprintf("IP Address : %s", gGlobalStuct.sCOTSInstInfo.m_cPMInfo[ucInstNo-1]);
        if(gGlobalStuct.sCOTSInstInfo.m_bPM[ucBoardCnt] == false)
        {
            QString strBoardOpen ="Not Opened";
            ui->tw_InstDetection->setItem(ucRow, 2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(255, 126, 126)));
        }
        else
        {
            QString strBoardOpen ="Opened";
            ui->tw_InstDetection->setItem(ucRow, 2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(126, 255, 126)));
        }

        ui->tw_InstDetection->setItem(ucRow,0,new QTableWidgetItem(strInstname));
        ui->tw_InstDetection->setItem(ucRow,1,new QTableWidgetItem(strInstrInfo));
        ucRow++;

    }

    ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount;
    for(ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucFGCount; ucInstNo++)
    {
        ui->tw_InstDetection->insertRow(ucRow);
        strInstname.sprintf("Function Generator %d",ucInstNo);
        strInstrInfo = "";
        strInstrInfo = QString::asprintf("IP Address : %s", gGlobalStuct.sCOTSInstInfo.m_cFGInfo[ucInstNo-1]);
        if(gGlobalStuct.sCOTSInstInfo.m_bFG[ucBoardCnt] == false)
        {
            QString strBoardOpen ="Not Opened";
            ui->tw_InstDetection->setItem(ucRow, 2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(255, 126, 126)));
        }
        else
        {
            QString strBoardOpen ="Opened";
            ui->tw_InstDetection->setItem(ucRow, 2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(126, 255, 126)));
        }

        ui->tw_InstDetection->setItem(ucRow,0,new QTableWidgetItem(strInstname));
        ui->tw_InstDetection->setItem(ucRow,1,new QTableWidgetItem(strInstrInfo));
        ucRow++;

    }

    ucRow = gGlobalStuct.m_sBrdDetectionConfig.m_ucSGCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucSACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucNACount + gGlobalStuct.m_sBrdDetectionConfig.m_ucPMCount + gGlobalStuct.m_sBrdDetectionConfig.m_ucDSOCount;
    for(ucInstNo = 1; ucInstNo <= gGlobalStuct.m_sBrdDetectionConfig.m_ucDSOCount; ucInstNo++)
    {
        ui->tw_InstDetection->insertRow(ucRow);
        strInstname.sprintf("DSO %d",ucInstNo);
        strInstrInfo = "";
        strInstrInfo = QString::asprintf("IP Address : %s", gGlobalStuct.sCOTSInstInfo.m_cDSOInfo[ucInstNo-1]);
        if(gGlobalStuct.sCOTSInstInfo.m_bDSO[ucBoardCnt] == false)
        {
            QString strBoardOpen ="Not Opened";
            ui->tw_InstDetection->setItem(ucRow, 2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(255, 126, 126)));
        }
        else
        {
            QString strBoardOpen ="Opened";
            ui->tw_InstDetection->setItem(ucRow, 2,new QTableWidgetItem(strBoardOpen));
            ui->tw_InstDetection->item(ucRow, 2)->setBackground(QBrush(QColor(126, 255, 126)));
        }

        ui->tw_InstDetection->setItem(ucRow,0,new QTableWidgetItem(strInstname));
        ui->tw_InstDetection->setItem(ucRow,1,new QTableWidgetItem(strInstrInfo));
        ucRow++;
    }

    for(int iLoop = 0; iLoop < 4; iLoop++)
    {
        QWidget *tempWdg = new QWidget();
        QLabel *Label = new QLabel();
        QVBoxLayout *Layout = new QVBoxLayout(tempWdg);
        Label->setScaledContents(true);
        Label->setFixedSize(30,25);
        Layout->addWidget(Label);
        Label->setPixmap(pixmap);
        Layout->setAlignment(Qt::AlignCenter);
        ui->tw_InstDetection->setCellWidget(iLoop,3,tempWdg);
    }
}
void cinstrumentdetection::keyPressEvent(QKeyEvent *e)
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
