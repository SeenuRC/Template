#include "DP_RFSS_7121_communication.h"
#include "ui_DP_RFSS_7121_communication.h"
#include "DP_RFSS_7121_UART.h"
#include "DP_RFSS_7121_Wrapper.h"
#include "QDebug.h"
#include <QSerialPort>
unsigned char g_ucPortOpenStatus;
QString g_qComportName;

char *g_ucPorts[256];

SDP_RFSS_7121_UART_CONFIG SUARTConfigData;
U16BIT g_u16InstanceID;

DP_RFSS_7121_communication::DP_RFSS_7121_communication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DP_RFSS_7121_communication)
{
    ui->setupUi(this);

    if(g_ucPortOpenStatus != 1)
    {
        ui->pb_UARTOpen->setEnabled(true);
        ui->pb_UARTClose->setEnabled(false);
    }
    else
    {
        ui->pb_UARTOpen->setEnabled(false);
        ui->pb_UARTClose->setEnabled(true);
    }
    show();

}

DP_RFSS_7121_communication::~DP_RFSS_7121_communication()
{
    delete ui;
}


void DP_RFSS_7121_communication::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
    {
        e->ignore();
    }
}


void DP_RFSS_7121_communication::show()
{

    char cErrMsg[250] = {0};
    U16BIT usPortNo                      = 0;
    S32BIT s32RetVal                     = 0;
    U8BIT u8AvailPortCnt                 = 0;

    QStringList list                    = {};

    ui->cmbUARTPortSel->clear();

    s32RetVal = DPRFSS7121_FindPorts(&u8AvailPortCnt, (PU8BIT*) g_ucPorts);
    if(s32RetVal != 0)
    {
        DPRFSSU_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
        QMessageBox::warning(this, "Board Operations Panel", cErrMsg);
        return;
    }

    for(usPortNo = 0; usPortNo < u8AvailPortCnt; usPortNo++)
    {
        list.append(QString(g_ucPorts[usPortNo]));
    }

    if(g_ucPortOpenStatus == 1)
    {
        list += (QStringList() << g_qComportName);
    }

    ui->cmbUARTPortSel->addItems(list);
    ui->cmbUARTPortSel->setCurrentIndex(0);
}

void DP_RFSS_7121_communication::on_pb_UARTOpen_clicked()
{
    bool ok = false;
    unsigned char u8PortNo = 0;
    int iRetVal = 0;
    QString strTemp = "";

    QString in_qsComNo ="";
    char cErrMsg[250] = {0};
    in_qsComNo = ui->cmbUARTPortSel->currentText();
    in_qsComNo.remove("COM", Qt::CaseSensitive);
    u8PortNo = in_qsComNo.toInt(&ok, 10);
    SUARTConfigData.m_u32BaudRate = ui->cmbUARTBaudRateSel->currentText().toInt();

    //iRetVal = ReadUARTConfiguration(&SUARTConfigData);
    if(iRetVal != DPRFSS_7121_SUCCESS)
    {
        QMessageBox::warning(this, "Board Operations Panel", "Unable to Open UART Configuration File");
        return;
    }
    
    SUARTConfigData.m_u32BaudRate   = QSerialPort::Baud115200;
    SUARTConfigData.m_s8DataSize    = QSerialPort::Data8;
    SUARTConfigData.m_s8Parity      = QSerialPort::NoParity;
    SUARTConfigData.m_s8FlowControl = QSerialPort::NoFlowControl;
    SUARTConfigData.m_s8StopBit     = 0;
    SUARTConfigData.m_u8PortNo      = ui->cmbUARTPortSel->currentIndex();

    iRetVal = DPRFSS7121_UART_Open(SUARTConfigData, &g_u16InstanceID);
    if(iRetVal != DPRFSS_7121_SUCCESS)
    {
        DPRFSSU_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
        QMessageBox::warning(this, "Board Operations Panel", cErrMsg);
        return;
    }
    else
    {
        strTemp.sprintf("COM%d open success",u8PortNo);
        QMessageBox::information(this, tr("Success"),strTemp);

        g_ucPortOpenStatus = 1;
        g_qComportName = ui->cmbUARTPortSel->currentText();

        iRetVal = BoardID_Validation(g_u16InstanceID);
        if(iRetVal != DPRFSS_7121_SUCCESS)
        {
            DPRFSSU_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
            QMessageBox::warning(this, "Board Operations Panel", cErrMsg);
            g_ucPortOpenStatus = 0;
            iRetVal = DPRFSS7121_UART_Close(g_u16InstanceID);
            if(iRetVal != DPRFSS_7121_SUCCESS)
            {
                DPRFSSU_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
                QMessageBox::warning(this, "Board Operations Panel", cErrMsg);
                return;
            }
            ui->pb_UARTClose->setEnabled(false);
            ui->pb_UARTOpen->setEnabled(true);
            ui->cmbUARTPortSel->setEnabled(true);
            return;
        }
        else
        {
            strTemp = "Test Board Details Read Successfully";
            QMessageBox::information(this,"Information",strTemp);
            ui->pb_UARTClose->setEnabled(true);
            ui->pb_UARTOpen->setEnabled(false);
            ui->cmbUARTPortSel->setEnabled(false);
        }
    }
}

void DP_RFSS_7121_communication::on_pb_UARTClose_clicked()
{
    QString strTemp ="";
    int iRetVal = 0;
    char cErrMsg[250] = {0};
    QString qCurrSelName = ui->cmbUARTPortSel->currentText();

    if(g_qComportName == qCurrSelName)
    {
        iRetVal = DPRFSS7121_UART_Close(g_u16InstanceID);
        if(iRetVal != DPRFSS_7121_SUCCESS)
        {
            DPRFSSU_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
            QMessageBox::warning(this, "Board Operations Panel", cErrMsg);
            return;
        }
        else
        {
            strTemp = "COM Port closed Successfully";
            QMessageBox::information(this,"Information",strTemp);
            ui->pb_UARTClose->setEnabled(false);
            ui->pb_UARTOpen->setEnabled(true);
            ui->cmbUARTPortSel->setEnabled(true);
            g_ucPortOpenStatus=0;
        }
    }
    else
    {
        strTemp.sprintf("Opened COM port is %s",g_qComportName.toStdString().c_str());
        QMessageBox::warning(this,"Warning",strTemp);
        return;
    }
}
/******************************************************************************
* Name                      : ReadUARTConfiguration
* Author                    : Tumma Kruthika
* Global Variables affected : NIL
* Created Date              : 17 Jan 2022
* Revision Date             : NIL
* Reason for Revising       : NIL
***************************************************************************//**
* To disconnect from the target.
* * \param NIL
* * \return No return values.
*******************************************************************************/
short DP_RFSS_7121_communication::ReadUARTConfiguration(PSDP_RFSS_7121_UART_CONFIG out_pSUARTConfigData)
{
    char arrcTemp[256] = {'\0'};
    unsigned int uiDataBit = 0;
    unsigned int uiBaudRate = 0;
    unsigned int uiParityBit = 0;
    unsigned int uiStopBit = 0;
    unsigned int uiFlowCntrl = 0;
    QString strData = "";
    QString strTemp = "";
    QString qstrFilePath = "";
    U8BIT ucRetval = 0;
    FILE *fpConfig = NULL;

    qstrFilePath = qApp->applicationDirPath();
    strData = "UART_Config.csv";
    qstrFilePath = qstrFilePath + "/INPUT_FILES/" + strData;

    /*UART Config file open*/
    fpConfig = fopen(qstrFilePath.toLatin1(), "r");
    if(fpConfig == NULL)
    {
        strTemp.sprintf("UART Config File Open Error");
        QMessageBox::information(this,"Failure",strTemp);
        return 0;
    }
    else
    {
        fscanf(fpConfig, "%s", arrcTemp);
    }

    while(!feof(fpConfig))
    {
        fscanf(fpConfig, "%d,%d,%d,%d,%d", &uiBaudRate, &uiDataBit, &uiParityBit , &uiStopBit, &uiFlowCntrl);
    }

    if(fpConfig != NULL)
    {
        fclose(fpConfig);
        fpConfig = NULL;
    }

    out_pSUARTConfigData->m_u32BaudRate = uiBaudRate;
    out_pSUARTConfigData->m_s8DataSize = uiDataBit;
    out_pSUARTConfigData->m_s8Parity = uiParityBit;
    out_pSUARTConfigData->m_s8StopBit = uiStopBit;
    out_pSUARTConfigData->m_s8FlowControl = uiFlowCntrl;

    return DPRFSS_7121_SUCCESS;
}

/******************************************************************************
* Name                      : BoardID_Validation
* Author                    : Tumma Kruthika
* Global Variables affected : NIL
* Created Date              : 17 Jan 2022
* Revision Date             : NIL
* Reason for Revising       : NIL
***************************************************************************//**
* To disconnect from the target.
* * \param NIL
* * \return No return values.
*******************************************************************************/
short DP_RFSS_7121_communication::BoardID_Validation(U16BIT in_u16InstanceID)
{

    S32BIT s32RetVal = 0;
    U16BIT u16RdBrdID = 0;
    U16BIT u16FPGATypeID  = 0;

    s32RetVal = DP_RFSS_7121_DPSPL6847_FPGA_Read(in_u16InstanceID, DPRFSS_7121_DPSPL6847_BRDID_REG, &u16RdBrdID);
    if(s32RetVal != DPRFSS_7121_SUCCESS)
    {
        return s32RetVal;
    }

    s32RetVal = DP_RFSS_7121_DPSPL6847_FPGA_Read(in_u16InstanceID, DPRFSS_7121_DPSPL6847_TYPEID_REG, &u16FPGATypeID);
    if(s32RetVal != DPRFSS_7121_SUCCESS)
    {
        return s32RetVal;
    }

    if((u16RdBrdID != DPRFSS_7121_DPSPL6847_BRD_ID))
    {
        return DPRFSS7121_BRD_RD_FAILED;
    }
    if(u16FPGATypeID  != DPRFSS_7121_DPSPL6847_TYPE_ID)
    {
        return DPRFSS7121_BRD_TYPE_ID_FAILED;
    }

    return DPRFSS_7121_SUCCESS;
}


