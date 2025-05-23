#include "DP_communication.h"
#include "ui_DP_communication.h"
#include "DP_UART_Wrapper.h"
#include "DP_SPICommunication.h"
#include "dp_QSocketComm.h"
#include <qdebug.h>
#include "DP_I2CCommunication.h"
#include <QSerialPort>
unsigned char g_ucPortOpenStatus;
QString g_qComportName;
extern MainWindow *p_objMainApp;
char *g_ucPorts[256];
char *g_ucPorts_PS[256];
unsigned char g_ucPorts_SPI[256];
SDP_APP_UART_CONFIG SUARTConfigData;
U16BIT g_u16InstanceID;
U16BIT g_u16PSMInstanceID;
SPI_ChannelConfig g_SSPIConfig;
extern DP_DRV_HANDLE g_vpSPIHandle;
extern int g_iOpenFlag;
FT_DEVICE_LIST_INFO_NODE S_I2C_Channelinfo[MAX_CHANNEL_CNT];
SDP_I2C_PORTDETAILS g_SI2CPortDetails;
FT_HANDLE g_I2C_Handle;

CQSocketComm *m_pobjQSockComm;

DP_APP_communication::DP_APP_communication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DP_APP_communication)
{
    ui->setupUi(this);

    if(g_ucPortOpenStatus != 1)
    {
        ui->pb_UARTOpen->setEnabled(true);
        ui->pb_UARTClose->setEnabled(false);

        ui->pb_UARTOpen_PS->setEnabled(true);
        ui->pb_UARTClose_PS->setEnabled(false);
    }
    else
    {
        ui->pb_UARTOpen->setEnabled(false);
        ui->pb_UARTClose->setEnabled(true);

        ui->pb_UARTOpen_PS->setEnabled(false);
        ui->pb_UARTClose_PS->setEnabled(true);
    }

    if(g_ucPortOpenStatus != 1)
    {
        ui->pb_I2COpen->setEnabled(true);
        ui->pb_I2CClose->setEnabled(false);
    }
    else
    {
        ui->pb_I2COpen->setEnabled(false);
        ui->pb_I2CClose->setEnabled(true);
    }

    m_u8Port = 0;

    QRegularExpression NumValidator("^([0-9]{1,8}|00)$");
    QValidator *Validate = new QRegularExpressionValidator(NumValidator , this);
    ui->leSPIClkdata->setValidator(Validate);

    ui->rb_tcp->setChecked(true);
    ui->le_TargetPortNumber->setValidator( new QIntValidator(0, 65535, this));

    QString IpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression IpRegex ("^" + IpRange
                                + "(\\." + IpRange + ")"
                                + "(\\." + IpRange + ")"
                                + "(\\." + IpRange + ")$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(IpRegex, this);

    QRegExp portRange("^()([1-9]|[1-5]?[0-9]{2,4}|6[1-4][0-9]{3}|65[1-4][0-9]{2}|655[1-2][0-9]|6553[1-5])$");
    QRegExpValidator *portValidator = new QRegExpValidator(portRange, this);
     ui->le_TargetPortNumber->setValidator(portValidator);
    ui->le_IPAddress->clear();
    ui->le_IPAddress->setValidator(ipValidator);

    connect(this,&DP_APP_communication::sig_MsgToPrintlog, ((MainWindow *)this->parent()), &MainWindow::Printlog);

    m_obj_CPSUWrapper = new CDP_PSU_Wrapper;
    show();
}

DP_APP_communication::~DP_APP_communication()
{
    delete ui;
}


void DP_APP_communication::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
    {
        e->ignore();
    }
}


void DP_APP_communication::show()
{
    char cErrMsg[250] = {0};
    U16BIT usPortNo                      = 0;
    S32BIT s32RetVal                     = 0;
    U8BIT u8AvailPortCnt                 = 0;

    QStringList list                    = {};
    QStringList list_PS                    = {};
    QString strTempPath                 = "";
    QString strFilePath                 = "";
    QString strTemp                     = "";
    QSettings *ptrInstDetailsFile       = NULL;
    PS8BIT pszSecName                   = NULL;
    S8BIT szSecKey[128]                 = {0};

    /******************************************UART Communication*********************************************/
    ui->cmbUARTPortSel->clear();

    s32RetVal = UART_FindPorts(&u8AvailPortCnt, (PU8BIT*) g_ucPorts);
    if(s32RetVal != 0)
    {
        p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
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

    /****************************************** UART Power Supply Communication*********************************************/
    ui->cmbUARTPortSel_PS->clear();
    u8AvailPortCnt                 = 0;
    s32RetVal = UART_FindPorts(&u8AvailPortCnt, (PU8BIT*) g_ucPorts_PS);
    if(s32RetVal != 0)
    {
        p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
        QMessageBox::warning(this, "Board Operations Panel", cErrMsg);
        return;
    }

    for(usPortNo = 0; usPortNo < u8AvailPortCnt; usPortNo++)
    {
        list_PS.append(QString(g_ucPorts_PS[usPortNo]));
    }

    if(g_ucPortOpenStatus == 1)
    {
        list_PS += (QStringList() << g_qComportName);
    }

    ui->cmbUARTPortSel_PS->addItems(list_PS);
    ui->cmbUARTPortSel_PS->setCurrentIndex(0);

    strTempPath = qApp->applicationDirPath();

    strFilePath = strTempPath + "/INPUT_FILES/" + "BrdDetails.ini";

    QFileInfo check_DB_file(strFilePath);
    if (check_DB_file.exists() && check_DB_file.isFile())
    {
        ptrInstDetailsFile = new(QSettings)(strFilePath, QSettings::IniFormat);
    }
    else
    {
        p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
        QMessageBox::warning(this, "File Open failed!", cErrMsg);
        return;
    }

    if((strTemp = ptrInstDetailsFile->value("POWER_SUPPLY/PS_VOLTAGE").toString()) == NULL)
    {
        return;
    }
    ui->LE_Voltage->setText(strTemp);
    m_fVoltage = strTemp.toFloat();

    if((strTemp = ptrInstDetailsFile->value("POWER_SUPPLY/PS_CURRENT").toString()) == NULL)
    {
        return;
    }
    ui->LE_Current->setText(strTemp);
    m_fCurrent = strTemp.toFloat();

    if((strTemp = ptrInstDetailsFile->value("POWER_SUPPLY/PS_UNDER_VOLTAGE").toString()) == NULL)
    {
        return;
    }
    m_fUnderVoltage = strTemp.toFloat();

    if((strTemp = ptrInstDetailsFile->value("POWER_SUPPLY/PS_OVER_VOLTAGE").toString()) == NULL)
    {
        return;
    }
    m_fOverVoltage = strTemp.toFloat();

    if((strTemp = ptrInstDetailsFile->value("POWER_SUPPLY/MODEL_NAME").toString()) == NULL)
    {
        return;
    }
    strcpy(m_szModel, strTemp.toStdString().c_str());

    /******************************************SPI Communication*********************************************/
    usPortNo             = 0;
    QString strData             = "";

    FT_DEVICE_LIST_INFO_NODE S_SPI_Channelinfo;

    s32RetVal = DP_SPI_FindPorts(g_ucPorts_SPI);
    if(s32RetVal != DP_SUCCESS)
    {
        return;
    }

    /* Get All SPI Port'S */
    for(usPortNo = 0; usPortNo < 255; usPortNo++)
    {
        if(g_ucPorts_SPI[usPortNo] == 1)
        {
#ifndef __SIMULATION__
            s32RetVal = DP_SPI_GetPortInfo(usPortNo, &S_SPI_Channelinfo);
            if(s32RetVal == DP_SUCCESS)
            {
                strData = QString::asprintf("%s", S_SPI_Channelinfo.SerialNumber);
                list += (QStringList()<<strData);
            }
            else
            {
                break;
            }
#endif
        }
    }
    ui->cmbPortSel->clear();
    ui->cmbPortSel->addItems(list);

    if(g_iOpenFlag == 1)
    {
        ui->pbClosePort->setDisabled(false);
        ui->pbOpen->setDisabled(true);
    }
    else
    {
        ui->pbClosePort->setDisabled(true);
        ui->pbOpen->setDisabled(false);
    }
    ui->leSPIClkdata->setText("1000000");

    /******************************************I2C Communication*********************************************/
     U32BIT u32Loop              = 0;
    if(g_SI2CPortDetails.m_u8OpenFlag == 0)
    {
        memset(&g_SI2CPortDetails, 0, sizeof(g_SI2CPortDetails));
    }

    /**
    * Scan for the connected FTDI Cable and Get the Total number of cables connected
    **/
    s32RetVal = DP_I2C_FindTotalDevices(&g_SI2CPortDetails.m_u32TotalDevices);
    if(s32RetVal != 0)
    {
        p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE , cErrMsg);
        QMessageBox::warning(this, " Application", cErrMsg);
        return;
    }

    if(g_SI2CPortDetails.m_u32TotalDevices == 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE , "USB Serial Converter Driver module is not loaded / No Devices is in the system");
//        QMessageBox::information( this, "Application", "USB Serial Converter Driver module is not loaded / No Devices is in the system");
        return;
    }
    else
    {;}

    if(g_SI2CPortDetails.m_u8OpenFlag == 1)
    {
        ui->pb_I2CClose->setDisabled(false);
        ui->pb_I2COpen->setDisabled(true);
    }
    else
    {
        ui->pb_I2CClose->setDisabled(true);
        ui->pb_I2COpen->setDisabled(false);
    }

    for(u32Loop = 0; u32Loop < g_SI2CPortDetails.m_u32TotalDevices; u32Loop++)
    {
        s32RetVal = DP_I2C_GetChannelInfo(u32Loop, &S_I2C_Channelinfo[u32Loop]);
        if(s32RetVal != 0)
        {
            p_objMainApp->DP_GetErrorMessage(s32RetVal, cErrMsg, sizeof(cErrMsg));
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE , cErrMsg);
            QMessageBox::warning(this, " Application", cErrMsg);
            return;
        }
        strData = QString("%1").arg(S_I2C_Channelinfo[u32Loop].SerialNumber);
        list += (QStringList()<<strData);
    }

    ui->cmbI2CPortSel->clear();

    ui->cmbI2CClkSel->setCurrentIndex(g_SI2CPortDetails.m_u8ClockSelIndex);

    ui->cmbI2CPortSel->addItems(list);
    ui->cmbI2CPortSel->setCurrentIndex(0);
}

void DP_APP_communication::on_pb_UARTOpen_clicked()
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

    if(ui->cmbUARTPortSel->currentText() == "")
    {
        QMessageBox::information(this, tr("Success"),"Port not detected");
        return;
    }
    SUARTConfigData.m_u32BaudRate = ui->cmbUARTBaudRateSel->currentText().toInt();
    SUARTConfigData.m_u32BaudRate   = 921600/*QSerialPort::Baud115200*/;
    SUARTConfigData.m_s8DataSize    = 8;
    SUARTConfigData.m_s8Parity      = 1;
    SUARTConfigData.m_s8FlowControl = 0;
    SUARTConfigData.m_s8StopBit     = 0;;
    SUARTConfigData.m_u8PortNo      = ui->cmbUARTPortSel->currentIndex();

    iRetVal = UART_Open(SUARTConfigData, &g_u16InstanceID);
    if(iRetVal != 0)
    {
        p_objMainApp->DP_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
        QMessageBox::warning(this, "Board Operations Panel", cErrMsg);
        return;
    }
    else
    {
        strTemp.sprintf("COM%d open success",u8PortNo);
        QMessageBox::information(this, tr("Success"),strTemp);
        g_ucPortOpenStatus = 1;
        g_qComportName = ui->cmbUARTPortSel->currentText();
        ui->pb_UARTClose->setEnabled(true);
        ui->pb_UARTOpen->setEnabled(false);
    }
}

void DP_APP_communication::on_pb_UARTClose_clicked()
{
    QString strTemp ="";
    int iRetVal = 0;
    char cErrMsg[250] = {0};
    QString qCurrSelName = ui->cmbUARTPortSel->currentText();

    if(g_qComportName == qCurrSelName)
    {
        iRetVal = UART_Close(g_u16InstanceID);
        if(iRetVal != 0)
        {
            p_objMainApp->DP_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
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
short DP_APP_communication::ReadUARTConfiguration(PSDP_APP_UART_CONFIG out_pSUARTConfigData)
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

    return 0;
}

void DP_APP_communication::on_pb_UARTOpen_PS_clicked()
{
    bool ok = false;
    int iRetVal = 0;
    QString strTemp = "";
    QString strComm = "";
    QString qsPSUError = "";

    S8BIT s8PSUModel[20]        = {0};
    U8BIT u8StopBit = 0;
    U8BIT u8DataBit = 0;
    U8BIT u8Parity  = 0;
    U8BIT u8PortNum = 0;
    U8BIT u8Address  = 1;

    S16BIT s16RetVal = 0;
    U64BIT u64BaudRate  = 0;

    char cErrMsg[250] = {0};
    QString in_qsComNo ="";
    SDP_APP_UART_CONFIG sUARTConfigData;
    memset(&sUARTConfigData, 0, sizeof(SDP_APP_UART_CONFIG));

    in_qsComNo = ui->cmbUARTPortSel_PS->currentText();
    in_qsComNo.remove("COM", Qt::CaseSensitive);
    u8PortNum = ui->cmbUARTPortSel_PS->currentIndex()/*in_qsComNo.toInt(&ok, 10)*/;

    u64BaudRate = ui->cmbUARTBaudRateSel->currentText().toLong();
    u64BaudRate = QSerialPort::Baud9600;

//    s16RetVal = ReadUARTConfiguration(&sUARTConfigData);
//    if(s16RetVal != DP_SUCCESS)
//    {
//        strTemp= "UART Configuration Read from file failed";
//        QMessageBox::warning(this, "Warning", strTemp);
//        return;
//    }
    u8StopBit =  0/*sUARTConfigData.m_s8StopBit*/;
    u8DataBit =  QSerialPort::Data8/*sUARTConfigData.m_s8DataSize*/;
    u8Parity =  QSerialPort::NoParity/*sUARTConfigData.m_s8Parity*/;

    iRetVal = m_obj_CPSUWrapper->DP_PSU_Open(u8PortNum, u64BaudRate, u8StopBit, u8DataBit, u8Parity, &g_u16PSMInstanceID);
    if(iRetVal != 0)
    {
        p_objMainApp->DP_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
        QMessageBox::warning(this, "PSU Open Failed", cErrMsg);
        return;
    }
    else
    {
        s16RetVal = m_obj_CPSUWrapper->DP_PSU_PowerRating(g_u16PSMInstanceID, u8Address, m_szModel , &m_fVoltrating, &m_fCurrentRating);
        if(s16RetVal != 0)
        {

            p_objMainApp->DP_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
            QMessageBox::warning(this, "Read Power Rating Failed", cErrMsg);

            on_pb_UARTClose_PS_clicked();
            return;
        }
        strTemp = QString::asprintf("COM%d open Success",u8PortNum);
        QMessageBox::information(this, tr("DP_SUCCESS"),strTemp);

        ui->pb_UARTClose_PS->setEnabled(true);
        ui->pb_UARTOpen_PS->setEnabled(false);
        ui->cmbUARTPortSel_PS->setEnabled(false);
        g_qComportName = ui->cmbUARTPortSel_PS->currentText();
        g_ucPortOpenStatus=1;
    }
}

void DP_APP_communication::on_pb_UARTClose_PS_clicked()
{
    QString strTemp ="";
    int iRetVal = 0;
    char cErrMsg[250] = {0};
    QString qCurrSelName = ui->cmbUARTPortSel_PS->currentText();

    if(g_qComportName == qCurrSelName)
    {
        iRetVal = UART_Close(g_u16PSMInstanceID);
        if(iRetVal != 0)
        {
            p_objMainApp->DP_GetErrorMessage(iRetVal, cErrMsg, sizeof(cErrMsg));
            QMessageBox::warning(this, "Board Operations Panel", cErrMsg);
            return;
        }
        else
        {
            strTemp = "COM Port closed Successfully";
            QMessageBox::information(this,"Information",strTemp);
            ui->pb_UARTClose_PS->setEnabled(false);
            ui->pb_UARTOpen_PS->setEnabled(true);
            ui->cmbUARTPortSel_PS->setEnabled(true);
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

void DP_APP_communication::on_PB_PowerOn_clicked()
{
    U8BIT u8Address             = 1;
    U8BIT u8SeriesType          = 0;

    S16BIT s16RetVal            = 0;
    FSINGLE fVoltage            = 0.0;
    FSINGLE fCurrent            = 0.0;
    FSINGLE fVoltageRD          = 0.0;
    FSINGLE fCurrentRD          = 0.0;
    FSINGLE m_fOverVoltageRD    = 0.0;
    FSINGLE m_fUnderVoltageRD   = 0.0;

    QString qsPSUError          = "";
    QString qstrModel           = "";

    fVoltage = ui->LE_Voltage->text().toFloat();
    fCurrent = ui->LE_Current->text().toFloat();

    qstrModel = QString::asprintf("%s",m_szModel);
    if(qstrModel.contains("GEN"))
    {
        u8SeriesType = DP_PSU_GEN_SERIES;
    }
    else
    {
        u8SeriesType = DP_PSU_Z_SERIES;
    }

    s16RetVal = m_obj_CPSUWrapper->DP_PSU_OV_UV_Config(g_u16PSMInstanceID,u8Address, u8SeriesType, m_fVoltrating, 0.0);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Unable to configure Over and Under Voltage");
        return;
    }

    s16RetVal = m_obj_CPSUWrapper->DP_PSU_VoltCurrConfig(g_u16PSMInstanceID,u8Address,DP_PSU_LOCAL_MODE, fVoltage, fCurrent);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Unable to configure Voltage and Current");
        return;
    }

    s16RetVal = m_obj_CPSUWrapper->DP_PSU_OV_UV_Config(g_u16PSMInstanceID,u8Address,u8SeriesType, m_fOverVoltage, m_fUnderVoltage);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Unable to configure Over and Under Voltage");
        return;
    }

    s16RetVal = m_obj_CPSUWrapper->DP_PSU_Readback_Config(g_u16PSMInstanceID,u8Address,&fVoltageRD,&fCurrentRD,&m_fOverVoltageRD,&m_fUnderVoltageRD);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Unable to Reaback Configuration");
        return;
    }

    if(fVoltage != fVoltageRD || fCurrent != fCurrentRD)
    {
        qsPSUError = "Voltage and Current not configured properly";
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,qsPSUError);
        return;
    }

    if(m_fOverVoltage != m_fOverVoltageRD || m_fUnderVoltage != m_fUnderVoltageRD)
    {
        qsPSUError = "Over Voltage and Over Current not configured properly";
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,qsPSUError);
        return;
    }

    s16RetVal = m_obj_CPSUWrapper->SetFOLDEnable(g_u16PSMInstanceID, true);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Enable Fold failed");
        return;
    }

    s16RetVal = m_obj_CPSUWrapper->DP_PSU_Output(g_u16PSMInstanceID,u8Address,DP_PSU_LOCAL_MODE,DP_PSU_OUTPUT_ON);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Enable PSU Out Failed");
        return;
    }

    ui->PB_PowerOn->setEnabled(false);
    ui->PB_PowerOff->setEnabled(true);
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,"PSU Power ON Success");
}

void DP_APP_communication::on_PB_PowerOff_clicked()
{
    U8BIT u8Address             = 1;
    S16BIT s16RetVal            = 0;

    QString qsPSUError          = "";

    s16RetVal = m_obj_CPSUWrapper->DP_PSU_Output(g_u16PSMInstanceID,u8Address,DP_PSU_LOCAL_MODE,DP_PSU_OUTPUT_OFF);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Disable PSU Out Failed");
        return;
    }

    s16RetVal = m_obj_CPSUWrapper->SetFOLDEnable(g_u16PSMInstanceID, false);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"Disable Fold failed");
        return;
    }
    ui->PB_PowerOn->setEnabled(true);
    ui->PB_PowerOff->setEnabled(false);
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,"PSU Power OFF Success");
}

void DP_APP_communication::on_PB_Read_clicked()
{
    U8BIT u8Address             = 1;
    S16BIT s16RetVal            = 0;
    FSINGLE fVoltage            = 0.0;
    FSINGLE fCurrent            = 0.0;

    QString qsPSUError          = "";

    s16RetVal = m_obj_CPSUWrapper->DP_PSU_Measure_Ouput(g_u16PSMInstanceID,u8Address,&fVoltage,&fCurrent);
    if(s16RetVal != 0)
    {
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,"PSU Measure Output Failed");
        return;
    }

    ui->LE_Measured_Voltage->setText(QString::asprintf("%.3lf",fVoltage));
    ui->LE_Measured_Current->setText(QString::asprintf("%.2lf",fCurrent));
    emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS,"Voltage and Current Read Success");
}

void DP_APP_communication::on_pbOpen_clicked()
{
    S8BIT cErrMsg[255]          = "";
    U8BIT u8ConfigMode          = 0;
    U8BIT u8ConfigCS            = 0;
    U8BIT u8ConfigActLH         = 0;
    S32BIT s32RetVal            = 0;
    U32BIT uiClockdata          = 0;

    if(ui->cmbPortSel->currentText() == "")
    {
        QMessageBox::information(this, "DP-CRF-6615 Application", "Please select any Port");
        return;
    }

    if(ui->leSPIClkdata->text().isEmpty())
    {
        QMessageBox::warning(this, "DP-CRF-6615 Application", "Please enter SPI Clock data");
        ui->leSPIClkdata->setFocus();
        return;
    }

    if(g_iOpenFlag != 1)
    {
        m_u8Port = ui->cmbPortSel->currentIndex();
        uiClockdata=ui->leSPIClkdata->text().toUInt();
        g_SSPIConfig.ClockRate = (unsigned int)uiClockdata;
        u8ConfigMode  = ui->cmbMode->currentIndex();
        u8ConfigCS    = ui->cmbChipSel->currentIndex();
        if(ui->cmbPolarity->currentIndex())
        {
            u8ConfigActLH = 0;
        }
        else
        {
            u8ConfigActLH = 1;
        }
        g_SSPIConfig.configOptions = (((u8ConfigActLH & 0x01) << 5) | ((u8ConfigCS & 0x07) << 2) | (u8ConfigMode & 0x03));
        g_SSPIConfig.LatencyTimer = 1;
        g_SSPIConfig.Pin = 0xCCCCCCCC;
#ifndef __SIMULATION__
        s32RetVal = DP_SPI_PortOpen(m_u8Port, &g_vpSPIHandle);
        if(s32RetVal != DP_SUCCESS)
        {
            sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE , "SPI Port Open Error");
            return;
        }

        s32RetVal = DP_SPI_PortConfigure(g_vpSPIHandle, &g_SSPIConfig);
        if(s32RetVal != DP_SUCCESS)
        {
             sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE , "SPI Port Configure Error");
            DP_SPI_PortClose(g_vpSPIHandle);
            return;
        }
#endif

        sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS , cErrMsg);
        QMessageBox::information(this, "DP-CRF-6615 Application", "SPI Port opened successfully");

        sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS , "SPI Port opened successfully");
        g_iOpenFlag = 1;

        ui->pbClosePort->setEnabled(true);
        ui->pbOpen->setEnabled(false);
        ui->cmbPortSel->setEnabled(false);
        ui->cmbMode->setEnabled(false);
        ui->leSPIClkdata->setEnabled(false);
        ui->cmbPolarity->setEnabled(false);
    }
    else
    {
        sig_MsgToPrintlog(PRINTLOG_TYPE_INFO , "Port already opened");
        QMessageBox::warning(this, "DP-CRF-6615 Application", "Port already opened");
        ui->pbClosePort->setEnabled(false);
        ui->pbOpen->setEnabled(true);
    }
}

void DP_APP_communication::on_pbClosePort_clicked()
{
    char cErrMsg[100]       = "";
    S32BIT s32RetVal        = 0;

    if(g_iOpenFlag)
    {
        // close the board
#ifndef __SIMULATION__
        s32RetVal = DP_SPI_PortClose(g_vpSPIHandle);
        if(s32RetVal != DP_SUCCESS)
        {
            sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE , "SPI Port Close Error");
            return;
        }
#endif
        sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS , "SPI Port closed successfully");
        QMessageBox::information(this, "DP-CRF-6615 Application", "SPI Port closed successfully");


        g_iOpenFlag = 0;
        ui->pbOpen->setEnabled(true);
        ui->pbClosePort->setEnabled(false);
        ui->cmbPortSel->setEnabled(true);
        ui->cmbMode->setEnabled(true);
        ui->leSPIClkdata->setEnabled(true);
        ui->cmbPolarity->setEnabled(true);
    }
    else
    {
        sig_MsgToPrintlog(PRINTLOG_TYPE_INFO , "Port is not opened !");
        QMessageBox::warning(this, "DP-CRF-6615 Application", "Port is not opened !");
        ui->pbOpen->setEnabled(true);
        ui->pbClosePort->setEnabled(true);
    }
}

void DP_APP_communication::on_pb_EtheConnect_clicked()
{
    QHostAddress objProcIPAddress;
    S16BIT S16RetValue = 0;
    QString qsActionLog;
    S8BIT m_ErrMsg[1024]= {0};
    m_u8ConType = QTCP_CONN;

    S16RetValue = m_pobjQSockComm->ConnectToServer(ui->le_IPAddress->text(), ui->le_TargetPortNumber->text().toLong(), m_u8ConType);
    if(S16RetValue)
    {

        m_pobjQSockComm->GetErrorMessage(S16RetValue, m_ErrMsg);
        QMessageBox::information(this, "QSocketComm", m_ErrMsg);
        return;
    }

    ui->pb_EtheConnect->setDisabled(true);
    ui->pb_EtheDisconnect->setDisabled(false);
    QMessageBox::information(this, "QSocketComm", "Connection Established");

    m_pobjQSockComm->DP_Set0214TimeandDate();
}

void DP_APP_communication::on_pb_EtheDisconnect_clicked()
{
    S32BIT s32RetVal= 0;
    S8BIT m_ErrMsg[1024]= {0};

    s32RetVal = m_pobjQSockComm->DisConnectFromServer();
    if(s32RetVal)
    {
        m_pobjQSockComm->GetErrorMessage(s32RetVal, m_ErrMsg);
        QMessageBox::information(this, "QSocketComm", m_ErrMsg);
        return;
    }
    ui->pb_EtheConnect->setDisabled(false);
    ui->pb_EtheDisconnect->setDisabled(true);
    QMessageBox::information(this, "QSocketComm", "DisConnected from Host");
}

void DP_APP_communication::on_pb_I2COpen_clicked()
{
    char cErrMsg[256];
    bool ok                     = 0;
    U8BIT u8DeviceNo            = 0;
    S32BIT s32RetVal            = 0;
    U32BIT u32ClockVal          = 0;

    I2C_ChannelConfig sI2CConfig;
    QString strText = "";
    QString strDevLoc = "";
    QString strClockSel = "";

    if(g_SI2CPortDetails.m_u32TotalDevices > 0)
    {
        if(g_SI2CPortDetails.m_u8OpenFlag != 1)
        {
            /**
            * Get the current selected Device Serial Number using the user selected Index
            **/
            strDevLoc = ui->cmbI2CPortSel->currentText();
            strDevLoc = g_SI2CPortDetails.m_arrs8SerialNum;
            strcpy(g_SI2CPortDetails.m_arrs8SerialNum, strDevLoc.toLatin1().data());
            u8DeviceNo = ui->cmbI2CPortSel->currentIndex();

            s32RetVal = DP_I2C_Open(u8DeviceNo, &g_I2C_Handle);
            if(s32RetVal != 0)
            {
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE,cErrMsg);
                QMessageBox::warning(this, "Application", cErrMsg);
                return;
            }
            else
            {
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS , "I2C Port opened successfully");
            }

            g_SI2CPortDetails.m_u8PortCurIndex = u8DeviceNo;

            strClockSel = ui->cmbI2CClkSel->currentText();

            u32ClockVal = strClockSel.toUInt(&ok,10);

            sI2CConfig.ClockRate = (I2C_CLOCKRATE)(u32ClockVal * I2C_CLOCK_FACTOR);
            sI2CConfig.LatencyTimer = 16;
            sI2CConfig.Options = I2C_ENABLE_DRIVE_ONLY_ZERO | I2C_DISABLE_3PHASE_CLOCKING;

            s32RetVal = DP_I2C_ConfigurePort(g_I2C_Handle, &sI2CConfig);
            if(s32RetVal != 0)
            {
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE , cErrMsg);
                QMessageBox::warning(this, " Application", cErrMsg);
                return;
            }
            else
            {
                emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS ,"I2C channel configuration success");
            }

            g_SI2CPortDetails.m_u8OpenFlag = 1;
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, "I2C device opened successfully");

            QMessageBox::information(this, " Application", "I2C device opened successfully");

            ui->pb_I2COpen->setEnabled(false);
            ui->cmbI2CPortSel->setEnabled(false);
            ui->cmbI2CClkSel->setEnabled(false);
            ui->pb_I2CClose->setEnabled(true);
        }
        else
        {
            strText = QString::asprintf("Device [%s] is already opened",g_SI2CPortDetails.m_arrs8SerialNum);
            emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strText);
            QMessageBox::warning(this, " Application", strText);
        }
    }
    else
    {
        strText = "Port Not found";
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_FAILURE, strText);
        QMessageBox::warning(this, " Application", strText);
    }
}
