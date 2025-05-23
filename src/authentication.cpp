/*******************************************************************************
// File Name    : authentication.cpp
// Author       : Kishore kumar A
// Created Date : Mar 11,2019
// Description  : To validate the user name and password of the User to provide/ restrict access to application.
*******************************************************************************/

/*******************************************************************************
// Company Name : DATA PATTERNS INDIA PRIVATE LIMITED
// Address      : No. H9,4th MainRoad,SIPCOT IT Park,Siruseri,Chennai-603103
// Email        : support@datapatterns.co.in
// Phone        : +91 44 47414000
// Fax          : +91 44 47414444
*******************************************************************************/

/******************************************************************************
// Copyright (c) 2023 DATA PATTERNS
//
// All rights reserved. These materials are confidential and proprietary to
// DATA PATTERNS and no part of these materials should be reproduced or published
// in any form by any means, electronic or mechanical, including photocopy, on any
// information storage or retrieval system, nor should the materials be disclosed
// to third parties without the explicit written authorization of DATA PATTERNS.
//
*******************************************************************************/

#include "ui_authentication.h"
#include "authentication.h"
#include "mainwindow.h"


QString gUsername;              //global variable for storing the username.
char g_arrAuthfileName[20];
extern short gsReturn;
QString strUserAuthName;
QString struserPassword;
extern CReportConfigWrapper *m_pobjReportWrapper;
extern SGlobalStucture gGlobalStuct;
int g_iReject;
bool g_bCloseEvent = 0;
extern MainWindow *p_objMainApp;

Authentication::Authentication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authentication)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QPixmap objPixLogin(":/Application/images/login_icon.png");
    ui->lbl_UsrLogin->setPixmap(objPixLogin);
    ui->LE_Pwd->setToolTip("Enter the Password");
    ui->le_Username->setToolTip("Enter the Username");
    ui->le_Username->setText("user");
    ui->le_Username->setFocus();
    ui->le_Username->setCursorPosition(0);
    ui->LE_Pwd->setText("user");

}

Authentication::~Authentication()
{
    delete ui;
}


/*******************************************************************************
 * Name                      : on_PB_Login_clicked
 * Author                    : Kishore kumar A
 * Global Variables affected : gUsername,g_arrAuthfileName
 * Created Date              : Mar 11,2019
 * Revision Date             : NIL
 * Reason for Revising       : NLL
 ***************************************************************************//**
 * To verify the username and password and display error message incase of error.
 *  * \param NIL
 * * \return No return values.
*******************************************************************************/
void Authentication::on_PB_Login_clicked()
{
    short sRetVal           = 0;
    char cErrMsg[256]       = {0};
    QString strMsg          = "";

    strUserAuthName = ui->le_Username->text();
    struserPassword = ui->LE_Pwd->text();

    if(ui->le_Username->text().isEmpty())
    {
        ui->le_Username->setFocus();
        QMessageBox::information(this,"Information","Please enter the user name");
        return;
    }

    if(ui->LE_Pwd->text().isEmpty())
    {
        ui->LE_Pwd->setFocus();
        QMessageBox::information(this,"Information","Please enter the Password");
        return;
    }
    //Verify User Deatail from Database
    sRetVal = m_pobjReportWrapper->VerifyUserDetails(strUserAuthName, struserPassword);
    if(sRetVal != DP_SUCCESS)
    {
        gsReturn = 0;
        p_objMainApp->DP_GetErrorMessage(sRetVal, cErrMsg, sizeof(cErrMsg));
        strMsg.sprintf("%s", cErrMsg);
        QMessageBox :: information(this, "Information", strMsg);
        return;
    }
    else
    {
        emit Sig_UpdateMainWindow(gGlobalStuct.sGlobalUserInfo.m_uiPrivilege);
        gsReturn = 1;
        g_bCloseEvent= 1;
        this->close();

    }
}
/*******************************************************************************
 * Name                      : on_PB_Exit_clicked
 * Author                    : Kishore kumar A
 * Global Variables affected : NIL
 * Created Date              : Mar 11,2019
 * Revision Date             : NIL
 * Reason for Revising       : NLL
 ***************************************************************************//**
 * To close the login window.
 * \param NIL
 * * \return No return values.
*******************************************************************************/
void Authentication::on_PB_Exit_clicked()
{
    if(QMessageBox::question(this,"Exit","Do you wish to exit the application", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {

        exit(0);
    }
}
/*******************************************************************************
 * Name                      : closeEvent
 * Author                    : Kishore kumar A
 * Global Variables affected : NIL
 * Created Date              : Mar 11,2019
 * Revision Date             : NIL
 * Reason for Revising       : NLL
 ***************************************************************************/
void Authentication::closeEvent(QCloseEvent *event)
{

    if(g_bCloseEvent == 0)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit",
                                                                       tr("Do you want to exit?"),
                                                                       QMessageBox::QMessageBox::No | QMessageBox::Yes,
                                                                      QMessageBox::Yes);
       if(resBtn == QMessageBox::Yes)
       {
            event->accept();
       }
       else
       {
           event->ignore();
       }
    }
}
/*******************************************************************************
 * Name                      : on_LE_Pwd_textEdited
 * Author                    : Manoj
 * Global Variables affected : NIL
 * Created Date              : Mar 09,2020
 * Revision Date             : NIL
 * Reason for Revising       : NLL
 ***************************************************************************//**
  * This function(slot for pwd line editor) will be used to enable the
  * ok button on non empty input in user and password.
  * \return No Return values.
*******************************************************************************/
void Authentication::on_LE_Pwd_textEdited(const QString &rstrPwd)
{
    if(ui->le_Username->text().isEmpty() || rstrPwd.isEmpty())
    {
        ui->PB_Login->setEnabled(false);
    }
    else
    {
        ui->PB_Login->setEnabled(true);
    }
}

/************************************ END OF FILE *****************************/
