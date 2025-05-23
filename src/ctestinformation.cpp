#include "ctestinformation.h"
#include "ui_ctestinformation.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <array>

extern CReportConfigWrapper *m_pobjReportWrapper;
extern short gsTestExeReturn;
extern SGlobalStucture gGlobalStuct;
QString strUserId        = "";
extern QString strUserAuthName;
extern QString struserPassword;
extern MainWindow *p_objMainApp;
int g_iCloseFlag;
ctestinformation::ctestinformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ctestinformation)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    ui->lblUserID->setVisible(false);
    ui->lblPassword->setVisible(false);
    ui->leUserId->setVisible(false);
    ui->lePassword->setVisible(false);
    ui->lblTestType->setVisible(false);
    ui->cmbTestType->setVisible(false);
    ui->leUserId->setText("admin");
    ui->lePassword->setText("admin");
    ui->leEmceSno->setText("1");
    ui->leTestLocation->setText("GBW");
    ui->leTesterName->setText("DATA PATTERNS");
    ui->leTestEvni->setText("SRC");
    ui->leRemarks->setText("App Simulation");
}

ctestinformation::~ctestinformation()
{
    delete ui;
}
/**************************************************************************
// Name						:	 on_pbOk_clicked()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To insert the test information details into the database
**************************************************************************/
void ctestinformation::on_pbOk_clicked()
{
    //    QString strUserId        ="";
    QString strPassword      ="";
    unsigned int u32Previledge = 0;
    QString strEmcrSno       ="";
    QString strTesterName    ="";
    QString strTestLocation  ="";
    QString strTestEnvi      ="";
    QString strRemarks      ="";
    char cErrMsg[256]        = {0};
    QString strMsg           = "";
    short sRetVal            = 0;
    S_TEST_REGISTER sTestReg;
    sTestReg.m_ulSessionID	 = 1;
    strUserId       =ui->leUserId->text().toStdString().c_str();
    strPassword     =ui->lePassword->text().toStdString().c_str();
    strEmcrSno      =ui->leEmceSno->text().toStdString().c_str();
    strTesterName   =ui->leTesterName->text().toStdString().c_str();
    strTestLocation =ui->leTestLocation->text().toStdString().c_str();
    strTestEnvi      =ui->leTestEvni->text().toStdString().c_str();
    strRemarks       = ui->leRemarks->text().toStdString().c_str();
    sTestReg.m_ucTestType = 3;

    if(ui->leUserId->text().isEmpty())
    {
        ui->leUserId->setFocus();
        QMessageBox::information(this,"Information","Please enter the user ID");
        return;
    }

    if(ui->lePassword->text().isEmpty())
    {
        ui->leUserId->setFocus();
        QMessageBox::information(this,"Information","Please enter the Password");
        return;
    }

    if(ui->leEmceSno->text().isEmpty())
    {
        ui->leUserId->setFocus();
        QMessageBox::information(this,"Information","Please enter the ATE Serial Number");
        return;
    }

    if(ui->leTesterName->text().isEmpty())
    {
        ui->leUserId->setFocus();
        QMessageBox::information(this,"Information","Please enter the Tester Name");
        return;
    }

    if(ui->leTestLocation->text().isEmpty())
    {
        ui->leUserId->setFocus();
        QMessageBox::information(this,"Information","Please enter the Tested Location");
        return;
    }

    if(ui->leTestEvni->text().isEmpty())
    {
        ui->leUserId->setFocus();
        QMessageBox::information(this,"Information","Please enter the Test Environment");
        return;
    }

    if(ui->leRemarks->text().isEmpty())
    {
        ui->leUserId->setFocus();
        QMessageBox::information(this,"Information","Please enter the Remarks");
        return;
    }

    QVector <S_USER_DETAILS> QvectRecords;
    S_USER_DETAILS *ReadRec;
    //Verify User Deatail from Database
#if 0
    sRetVal = m_pobjReportWrapper->VerifyUserDetails(strUserAuthName, struserPassword, &u32Previledge);
    if(sRetVal != DP_SUCCESS)
    {
        p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
        strMsg = QString::asprintf("%s", cErrMsg);
        QMessageBox :: information(this, "Information", strMsg);
        return;
    }
    // Read details from the database.
    sRetVal = m_pobjReportWrapper->ReadUserDetails(QvectRecords, strUserAuthName);
    if(sRetVal != DP_SUCCESS)
    {
        p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
        strMsg = QString::asprintf("%s", cErrMsg);
        QMessageBox :: information(this, "Information", strMsg);
        return;
    }

    if(QvectRecords.size() == 0)
    {
        p_objMainApp->DP_GetErrorMessage(DP_APP_ERR_USER_ID, cErrMsg, sizeof(cErrMsg));
        strMsg = QString::asprintf("%s", cErrMsg);
        QMessageBox :: information(this, "Information", strMsg);
        return;
    }
#endif
    ReadRec = QvectRecords.data();
    gGlobalStuct.sGlobalUserInfo.m_uiPrivilege = u32Previledge;
    strcpy(gGlobalStuct.sGlobalUserInfo.m_strUserID,ReadRec->qsUserID);
    strcpy(gGlobalStuct.sGlobalUserInfo.m_strCurrentUser,strUserAuthName.toStdString().c_str());
    strcpy(gGlobalStuct.sGlobalUserInfo.m_carrCurrentPwd,struserPassword.toStdString().c_str());

    sTestReg.m_strATESNo        = strEmcrSno;
    sTestReg.m_strLoginUserName = strUserAuthName;
    sTestReg.m_strTester        = strTesterName;
    sTestReg.m_strTestLocation  = strTestLocation;
    sTestReg.m_strTestEnv       = strTestEnvi;
    sTestReg.m_strSysName       = "TMLSRV06";


    sRetVal = m_pobjReportWrapper->TestRegistration(sTestReg);
    if(sRetVal != DP_SUCCESS)
    {
        p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
        strMsg = QString::asprintf("%s", cErrMsg);
        QMessageBox :: information(this, "Information", strMsg);
        p_objMainApp->m_btrFlag = false;
        return;
    }
    else
    {
        strMsg = "Test registeration Succes";
        p_objMainApp->m_btrFlag = true;
        g_iCloseFlag =1;
        this->close();
    }

}
/**************************************************************************
// Name						:	 on_pbCancel_clicked()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To cancel and close the application
**************************************************************************/
void ctestinformation::on_pbCancel_clicked()
{
    p_objMainApp->m_btrFlag = false;
    this->close();
    return;
}
/**************************************************************************
// Name						:	 closeEvent()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To close the application
**************************************************************************/
void ctestinformation::closeEvent(QCloseEvent *event)
{
    if(g_iCloseFlag == 0)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit",
                                                                    tr("Do you wish to close the test?"),
                                                                    QMessageBox::QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes)
        {
            p_objMainApp->m_btrFlag = false;
            this->close();
        }
        else
        {
            p_objMainApp->m_btrFlag = true;
            event->ignore();
        }
    }
}

