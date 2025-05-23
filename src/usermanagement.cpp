#include "usermanagement.h"
#include "ui_usermanagement.h"
#include "mainwindow.h"
#include <QMessageBox>

extern CReportConfigWrapper *m_pobjReportWrapper;
extern SGlobalStucture gGlobalStuct;
extern QString strUserAuthName ;
extern MainWindow *p_objMainApp;
usermanagement::usermanagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::usermanagement)
{
    ui->setupUi(this);
    on_cmbUserSel_currentIndexChanged(0);
}

usermanagement::~usermanagement()
{
    delete ui;
}
/**************************************************************************
// Name						:	 on_cmbUserSel_currentIndexChanged()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To change the user required selection from GUI
**************************************************************************/
void usermanagement::on_cmbUserSel_currentIndexChanged(int index)
{
    QString strCaption = "";
    u8UserSelection =ui->cmbUserSel->currentIndex();
    switch(u8UserSelection)
    {
    case 0://Add User
    {
        ui->lblUserID->setVisible(true);
        ui->leUserID->setVisible(true);
        ui->lblUserName->setVisible(true);
        ui->leUserName->setVisible(true);
        ui->lblCurrentPassword->setVisible(true);
        ui->leCurrentPassword->setVisible(true);
        ui->lblNewPassword->setVisible(true);
        ui->leNewPassword->setVisible(true);
        ui->lblconfirmPassword->setVisible(true);
        ui->leConfirmPassword->setVisible(true);
        ui->lblcmbUserId->setVisible(false);
        ui->cmbUseriId->setVisible(false);
        strCaption.sprintf("%s", "&Add User");
        ui->pbAddDelChangeUpdate->setText(strCaption);
    }
        break;
    case 1://Change Password
    {
        if(gGlobalStuct.sGlobalUserInfo.m_uiPrivilege == DP_APP_ATE_NORMAL_USER)
        {
            ui->cmbUseriId->clear();
            ui->cmbUseriId->addItem(strUserAuthName);
        }
        ui->lblUserID->setVisible(false);
        ui->leUserID->setVisible(false);
        ui->lblcmbUserId->setVisible(true);
        ui->cmbUseriId->setVisible(true);
        ui->lblUserName->setVisible(false);
        ui->leUserName->setVisible(false);
        ui->lblCurrentPassword->setVisible(true);
        ui->leCurrentPassword->setVisible(true);
        ui->lblNewPassword->setVisible(true);
        ui->leNewPassword->setVisible(true);
        ui->lblconfirmPassword->setVisible(true);
        ui->leConfirmPassword->setVisible(true);
        strCaption.sprintf("%s", "C&hange Password");
        ui->pbAddDelChangeUpdate->setText(strCaption);
        LoadUserIDToCbo(DP_APP_UPDATEUSER);
    }
        break;
    case 2://Upadte User
    {
        if(gGlobalStuct.sGlobalUserInfo.m_uiPrivilege == DP_APP_ATE_NORMAL_USER)
        {
            ui->cmbUseriId->clear();
            ui->cmbUseriId->addItem(strUserAuthName);
        }
        ui->lblUserID->setVisible(false);
        ui->leUserID->setVisible(false);
        ui->lblcmbUserId->setVisible(true);
        ui->cmbUseriId->setVisible(true);
        ui->lblUserName->setVisible(true);
        ui->leUserName->setVisible(true);
        ui->lblCurrentPassword->setVisible(true);
        ui->leCurrentPassword->setVisible(true);
        ui->lblNewPassword->setVisible(false);
        ui->leNewPassword->setVisible(false);
        ui->lblconfirmPassword->setVisible(false);
        ui->leConfirmPassword->setVisible(false);
        strCaption.sprintf("%s", "U&pdate User");
        ui->pbAddDelChangeUpdate->setText(strCaption);
        LoadUserIDToCbo(DP_APP_UPDATEUSER);
    }
        break;

    case 3://Delete User
    {
        ui->lblUserID->setVisible(false);
        ui->leUserID->setVisible(false);
        ui->lblcmbUserId->setVisible(true);
        ui->cmbUseriId->setVisible(true);
        ui->lblUserName->setVisible(false);
        ui->leUserName->setVisible(false);
        ui->lblCurrentPassword->setVisible(true);
        ui->leCurrentPassword->setVisible(true);
        ui->lblNewPassword->setVisible(false);
        ui->leNewPassword->setVisible(false);
        ui->lblconfirmPassword->setVisible(false);
        ui->leConfirmPassword->setVisible(false);
        strCaption.sprintf("%s", "&Delete User");
        ui->pbAddDelChangeUpdate->setText(strCaption);
        LoadUserIDToCbo(DP_APP_DELETEUSER);
    }
        break;
    }

}
/**************************************************************************
// Name						:	 on_pbAddDelChangeUpdate_clicked()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To perform the add,delete,update user details into the database
**************************************************************************/
void usermanagement::on_pbAddDelChangeUpdate_clicked()
{
    short sRetVal = 0;
    char cErrMsg[256]        = {0};
    QString strMsg           = "";
    QString strSelLoginId    = "";
    U32BIT u32Priviledge = 0;

    strUserID            = ui->leUserID->text().toStdString().c_str();
    strUserName          = ui->leUserName->text().toStdString().c_str();
    strCurrPassword      = ui->leCurrentPassword->text().toStdString().c_str();
    strNewPassword       = ui->leNewPassword->text().toStdString().c_str();
    strConfPassword      = ui->leConfirmPassword->text().toStdString().c_str();
    S_USER_DETAILS ReadRec;

    if(u8UserSelection == 0)//add user
    {
        if(ui->leUserID->text().isEmpty())
        {
            ui->leUserID->setFocus();
            QMessageBox::information(this,"Information","Please enter the user ID");
            return;
        }
        if(ui->leUserName->text().isEmpty())
        {
            ui->leUserName->setFocus();
            QMessageBox::information(this,"Information","Please enter the user Name");
            return;
        }
        if(ui->leCurrentPassword->text().isEmpty())
        {
            ui->leCurrentPassword->setFocus();
            QMessageBox::information(this,"Information","Please enter the Current login user password");
            return;
        }
        if(ui->leNewPassword->text().isEmpty())
        {
            ui->leNewPassword->setFocus();
            QMessageBox::information(this,"Information","Please enter the New password");
            return;
        }
        if(ui->leConfirmPassword->text().isEmpty())
        {
            ui->leConfirmPassword->setFocus();
            QMessageBox::information(this,"Information","Please enter the confirm password");
            return;
        }
        if(!ValidateField())
        {
            return;
        }
        sRetVal = m_pobjReportWrapper->VerifyUserDetails(gGlobalStuct.sGlobalUserInfo.m_strCurrentUser, gGlobalStuct.sGlobalUserInfo.m_carrCurrentPwd);
        if(sRetVal != DP_SUCCESS)
        {
            p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            QMessageBox :: information(this, "Information", strMsg);
            ui->leCurrentPassword->setFocus();
            return;
        }

        strcpy(ReadRec.qsUserName,strUserName.toStdString().c_str());
        strcpy(ReadRec.qsUserID,  strUserID.toStdString().c_str());
        strcpy(ReadRec.qsPassword,strNewPassword.toStdString().c_str());

        if(gGlobalStuct.sGlobalUserInfo.m_uiPrivilege == DP_APP_DEVELOPER)
        {
            ReadRec.sPrivilege		= DP_APP_DEVELOPER;
        }
        else if(gGlobalStuct.sGlobalUserInfo.m_uiPrivilege == DP_APP_DP_USER)
        {
            ReadRec.sPrivilege		= DP_APP_DEVELOPER;
        }
        else
        {
            ReadRec.sPrivilege		= DP_APP_ATE_NORMAL_USER;
        }

        QVector <S_USER_DETAILS> QvectRecords;

        QvectRecords.push_back(ReadRec);

        sRetVal = m_pobjReportWrapper->InsertUserDetails(QvectRecords);
        if(sRetVal != DP_SUCCESS)
        {
            p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            QMessageBox :: information(this, "Information", strMsg);
            ResetContents();
            return;
        }
        strMsg = "New User have been Added Successfully";
        QMessageBox :: information(this, "Information", strMsg);
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strMsg);
    }
    else if(u8UserSelection == 1)//Password Change
    {

        if(ui->leCurrentPassword->text().isEmpty())
        {
            ui->leCurrentPassword->setFocus();
            QMessageBox::information(this,"Information","Please enter the Current login user password");
            return;
        }

        if(ui->leNewPassword->text().isEmpty())
        {
            ui->leNewPassword->setFocus();
            QMessageBox::information(this,"Information","Please enter the New password");
            return;
        }
        if(ui->leConfirmPassword->text().isEmpty())
        {
            ui->leConfirmPassword->setFocus();
            QMessageBox::information(this,"Information","Please enter the confirm password");
            return;
        }
        if(!ValidateField())
        {
            return;
        }

        sRetVal = m_pobjReportWrapper->VerifyUserDetails(gGlobalStuct.sGlobalUserInfo.m_strCurrentUser, gGlobalStuct.sGlobalUserInfo.m_carrCurrentPwd);

        if(sRetVal != DP_SUCCESS)
        {
            p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            QMessageBox :: information(this, "Information", strMsg);
            ui->leCurrentPassword->setFocus();
            return;
        }
        strSelLoginId = ui->cmbUseriId->currentText().toStdString().c_str();
        strcpy(ReadRec.qsUserName,strUserName.toStdString().c_str());
        strcpy(ReadRec.qsUserID,strSelLoginId.toStdString().c_str());
        strcpy(ReadRec.qsPassword,strNewPassword.toStdString().c_str());
        gGlobalStuct.sGlobalUserInfo.m_uiPrivilege = 1;
        QVector <S_USER_DETAILS> QvectRecords;
        QvectRecords.push_back(ReadRec);
        sRetVal = m_pobjReportWrapper->UpdateUserDetails(ReadRec,gGlobalStuct.sGlobalUserInfo.m_uiPrivilege);
        if(sRetVal != DP_SUCCESS)
        {
            p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            QMessageBox :: information(this, "Information", strMsg);
            ResetContents();
            return;
        }
        strMsg = "User details Updated Successfully";
        QMessageBox :: information(this, "Information", strMsg);
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strMsg);
        ResetContents();
        LoadUserIDToCbo(DP_APP_UPDATEUSER);
    }
    else if(u8UserSelection == 2)//Update user name change
    {
        if(ui->leUserName->text().isEmpty())
        {
            ui->leUserName->setFocus();
            QMessageBox::information(this,"Information","Please enter the user Name");
            return;
        }
        if(ui->leCurrentPassword->text().isEmpty())
        {
            ui->leCurrentPassword->setFocus();
            QMessageBox::information(this,"Information","Please enter the Current login user password");
            return;
        }
        sRetVal = m_pobjReportWrapper->VerifyUserDetails(gGlobalStuct.sGlobalUserInfo.m_strCurrentUser, gGlobalStuct.sGlobalUserInfo.m_carrCurrentPwd);

        if(sRetVal != DP_SUCCESS)
        {
            p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            QMessageBox :: information(this, "Information", strMsg);
            ui->leCurrentPassword->setFocus();
            return;
        }
        strSelLoginId = ui->cmbUseriId->currentText().toStdString().c_str();
        strcpy(ReadRec.qsUserName,strUserName.toStdString().c_str());
        strcpy(ReadRec.qsPassword,strCurrPassword.toStdString().c_str());
        strcpy(ReadRec.qsUserID,strSelLoginId.toStdString().c_str());
        gGlobalStuct.sGlobalUserInfo.m_uiPrivilege = 2;
        QVector <S_USER_DETAILS> QvectRecords;
        QvectRecords.push_back(ReadRec);
        sRetVal = m_pobjReportWrapper->UpdateUserDetails(ReadRec,gGlobalStuct.sGlobalUserInfo.m_uiPrivilege);
        if(sRetVal != DP_SUCCESS)
        {
            p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            QMessageBox :: information(this, "Information", strMsg);
            ResetContents();
            return;
        }
        strMsg = "User name updated Successfully";
        QMessageBox :: information(this, "Information", strMsg);
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strMsg);
        ResetContents();
        LoadUserIDToCbo(DP_APP_UPDATEUSER);
    }
    else//Delete User Details
    {

        if(!ui->cmbUseriId->count())
        {
            ui->cmbUseriId->setFocus();
            QMessageBox::information(this,"Information","User Id is Empty");
            return;
        }

        if(ui->leCurrentPassword->text().isEmpty())
        {
            ui->leCurrentPassword->setFocus();
            QMessageBox::information(this,"Information","Please enter the Current login user password");
            return;
        }
        sRetVal = m_pobjReportWrapper->VerifyUserDetails(gGlobalStuct.sGlobalUserInfo.m_strCurrentUser, gGlobalStuct.sGlobalUserInfo.m_carrCurrentPwd);

        if(sRetVal != DP_SUCCESS)
        {
            p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            QMessageBox :: information(this, "Information", strMsg);
            ui->leCurrentPassword->setFocus();
            return;
        }

        strSelLoginId = ui->cmbUseriId->currentText().toStdString().c_str();
        sRetVal = m_pobjReportWrapper->DeleteUserDetails(strSelLoginId);
        if(sRetVal != DP_SUCCESS)
        {
            p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
            strMsg = QString::asprintf("%s", cErrMsg);
            QMessageBox :: information(this, "Information", strMsg);
            ui->leCurrentPassword->setFocus();
            ResetContents();
            return;
        }
        LoadUserIDToCbo(DP_APP_DELETEUSER);
        strMsg = "User details deleted successfully";
        QMessageBox :: information(this, "Information", strMsg);
        emit sig_MsgToPrintlog(PRINTLOG_TYPE_SUCCESS, strMsg);
        ResetContents();
    }

}
bool usermanagement::ValidateField()
{
    if( strConfPassword	!=	strNewPassword)
    {
        QMessageBox::information(this,"Information","Please enter the correct password");
        strCurrPassword		=	"";
        strNewPassword      =   "";
        ui->leCurrentPassword->setFocus();
        return 0;
    }
    return 1;
}
/**************************************************************************
// Name						:	 ResetContents()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To reset the condents
**************************************************************************/
void usermanagement::ResetContents(void)
{
    ui->leUserID->clear();
    ui->leUserName->clear();
    ui->leCurrentPassword->clear();
    ui->leNewPassword->clear();
    ui->leConfirmPassword->clear();
}
/**************************************************************************
// Name						:	 LoadUserIDToCbo()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	To load the user details in GUI based on the updation
**************************************************************************/
void usermanagement::LoadUserIDToCbo(unsigned short usOption)
{
    int iRecCount	= 0;
    int iIndex		= 0;
    QString strMsg = "";
    QVector <S_USER_DETAILS> QvectRecords;
    S_USER_DETAILS* ReadRec;
    if(gGlobalStuct.sGlobalUserInfo.m_uiPrivilege != DP_APP_ATE_NORMAL_USER)
    {
        // Read details from the database.
        m_pobjReportWrapper->ReadAllUserDetails(QvectRecords);
        ReadRec  = QvectRecords.data();
        iRecCount= QvectRecords.size();
        if(iRecCount == 0)
        {
            strMsg = "No record found from database";
            QMessageBox :: information(this, "Information", strMsg);
        }
        ui->cmbUseriId->clear();

        if( usOption == DP_APP_DELETEUSER && gGlobalStuct.sGlobalUserInfo.m_uiPrivilege == DP_APP_ATE_ADMIN )
        {
            for(iIndex = 0; iIndex < iRecCount; iIndex++)
            {
                if(ReadRec[iIndex].sPrivilege == DP_APP_ATE_NORMAL_USER )
                {
                    ui->cmbUseriId->addItem(ReadRec[iIndex].qsUserID);
                }
            }
        }

        if(usOption == DP_APP_DELETEUSER && gGlobalStuct.sGlobalUserInfo.m_uiPrivilege == DP_APP_DEVELOPER)
        {
            for(iIndex = 0; iIndex < iRecCount; iIndex++)
            {
                if(ReadRec[iIndex].qsUserID != gGlobalStuct.sGlobalUserInfo.m_strCurrentUser )
                {
                     ui->cmbUseriId->addItem(ReadRec[iIndex].qsUserID);
                }
            }
        }


        if(usOption == DP_APP_UPDATEUSER && gGlobalStuct.sGlobalUserInfo.m_uiPrivilege == DP_APP_ATE_ADMIN)
        {
            for(iIndex = 0; iIndex < iRecCount; iIndex++)
            {
                if(ReadRec[iIndex].sPrivilege == DP_APP_ATE_NORMAL_USER || ReadRec[iIndex].qsUserID == gGlobalStuct.sGlobalUserInfo.m_strCurrentUser )
                {
                    ui->cmbUseriId->addItem(ReadRec[iIndex].qsUserID);
                }
            }
        }

        if(usOption == DP_APP_UPDATEUSER && gGlobalStuct.sGlobalUserInfo.m_uiPrivilege == DP_APP_DEVELOPER)
        {
            for(iIndex = 0; iIndex < iRecCount; iIndex++)
            {
                ui->cmbUseriId->addItem(ReadRec[iIndex].qsUserID);
            }
        }
    }

    ui->cmbUseriId->setCurrentIndex(0);
}
void usermanagement::keyPressEvent(QKeyEvent *e)
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
