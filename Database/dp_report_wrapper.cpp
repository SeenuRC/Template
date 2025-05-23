#include "dp_report_wrapper.h"

#include <QtSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlRecord>
#include <QDebug>
#include <QVector>
#include <QTime>
#include <QDate>
#include "../src/DP_APP_macros.h"
int g_u32SystemId;
CReportConfigWrapper::CReportConfigWrapper()
{

}

CReportConfigWrapper::~CReportConfigWrapper()
{
	if(m_objDataBase.isOpen())
    {
		m_objDataBase.close();
	}
}
/**************************************************************************
// Name						:	 ConnectDatabase()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to connect the database
**************************************************************************/
S32BIT CReportConfigWrapper::ConnectDatabase(QString in_strServerIP, QString in_strUID, QString in_strPassword, QString in_strDatabase, QString in_strPortNo, QString in_strODBCDriVer)
{
    QString strConnectionString = "";
    bool bRetVal = 0;
    unsigned short usTimeout = 3000;
    if(m_objDataBase.isOpen())
    {
        m_objDataBase.close();
    }
#ifndef _QMYSQL_
    m_objDataBase = QSqlDatabase::addDatabase("QODBC");
    strConnectionString = QString::asprintf("Driver={MariaDB ODBC %0.1lf Driver};Server=%s;UID=%s;PWD=%s;DB=%s;Port=%s",in_strODBCDriVer.toFloat(), in_strServerIP.toStdString().c_str(),  in_strUID.toStdString().c_str(), in_strPassword.toStdString().c_str(), in_strDatabase.toStdString().c_str(), in_strPortNo.toStdString().c_str());
    //qDebug() << strConnectionString;
    /*Opening the data base mariadb*/
    m_objDataBase.setDatabaseName(strConnectionString);
#else

    m_objDataBase = QSqlDatabase::addDatabase("QMYSQL3");
    /*Opening the data base mariadb*/

    m_objDataBase.setDatabaseName(in_strDatabase);
    m_objDataBase.setUserName(in_strUID);
    m_objDataBase.setPassword(in_strPassword);
    m_objDataBase.setHostName(in_strServerIP);
    m_objDataBase.setPort(in_strPortNo.toInt());
    qDebug() << m_objDataBase.drivers();
#endif


    bRetVal = m_objDataBase.open();//failure = 0; success = 1;
    if(!bRetVal)
    {
        qWarning("%s", m_objDataBase.lastError().text().toLocal8Bit().data());
        return DP_RPT_WRAP_ERR_CONN_DATABASE;
    }

    QSqlQuery databaseQuery(m_objDataBase);
    databaseQuery.prepare("SET GLOBAL innodb_flush_log_at_trx_commit = 2;");
    bRetVal = databaseQuery.exec();


    return DP_DATABASE_SUCCESS;

}
/**************************************************************************
// Name						:	 TestRegistration()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to insert the test registration detail into database
**************************************************************************/
S32BIT CReportConfigWrapper::TestRegistration(S_TEST_REGISTER in_sobjTestReg)
{
    QString strQuery	= "";
    U32BIT ulSessionID	= 0;
    int iUnitNo = 0;
    QString time_format = "yyyy-MM-dd HH:mm:ss";
    QDateTime m_DateTime = QDateTime::currentDateTime();
    QString strSysTime = m_DateTime.toString(time_format);
    QString m_szAppChkSum = "";
    QString m_szAppVersion = "";
    bool bRetVal = 0;

    if(GetLastLoginSession(&ulSessionID))
    {
        return DP_DATABASE_FAILURE;
    }

    iUnitNo = (in_sobjTestReg.m_strATESNo.toInt());

    QSqlQuery databaseQuery(m_objDataBase);

    /** inserts the test information into table **/
    strQuery = QString::asprintf("INSERT INTO tr(SID,ASNO,SNo,UN,ST,Tester,Tester_t,TE,TE_t,AppVer,AppChkSm,sysType,sysName)VALUES(%d,'%s',%d,'%s','%s','%s','%s','%s','%s','%s','%s',%d,'%s')",\
                     ulSessionID + 1, in_sobjTestReg.m_strATESNo.toStdString().c_str(),iUnitNo, in_sobjTestReg.m_strLoginUserName.toStdString().c_str(), strSysTime.toStdString().c_str(),\
                     in_sobjTestReg.m_strTester.toStdString().c_str(),in_sobjTestReg.m_strTester.toStdString().c_str(),in_sobjTestReg.m_strTestLocation.toStdString().c_str(),\
                     in_sobjTestReg.m_strTestEnv.toStdString().c_str(), m_szAppVersion.toStdString().c_str(), m_szAppChkSum.toStdString().c_str(), in_sobjTestReg.m_ucTestType, in_sobjTestReg.m_strSysName.toStdString().c_str());

    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_RPT_WRAP_ERR_TR;
    }
    return DP_DATABASE_SUCCESS;
}
/**************************************************************************
// Name						:	 GetLastExecutionID()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to get the last execution ID from database
**************************************************************************/
S32BIT CReportConfigWrapper::GetLastExecutionID(U32BIT* out_pulLastExecID)
{
    U32BIT ulSessionID = 0;
    bool bRetVal = 0;

    QSqlQuery databaseQuery(m_objDataBase);

    databaseQuery.prepare("select * from crpt ORDER BY EID DESC LIMIT 1");
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_DATABASE_FAILURE;
    }
    else
    {
        while(databaseQuery.next())
        {
            QSqlRecord m_Record = databaseQuery.record();
            ulSessionID = (U32BIT)m_Record.value(2).toInt();

        }
    }

    *out_pulLastExecID = ulSessionID;

    return DP_DATABASE_SUCCESS;
}
/**************************************************************************
// Name						:	 GetLastLoginSession()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to get the last loggin session ID from database
**************************************************************************/
S32BIT CReportConfigWrapper::GetLastLoginSession(U32BIT* out_pulSessionID)
{
    U32BIT ulSessionID = 0;
    bool bRetVal = 0;
    QSqlQuery databaseQuery(m_objDataBase);
    databaseQuery.prepare("select * from tr ORDER BY SID DESC LIMIT 1");
    bRetVal = databaseQuery.exec("select * from tr ORDER BY SID DESC LIMIT 1");
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_DATABASE_FAILURE;
    }
    else
    {
        while(databaseQuery.next())
        {
            QSqlRecord m_Record = databaseQuery.record();
            ulSessionID = (U32BIT)m_Record.value(1).toInt();
        }
    }
    *out_pulSessionID = ulSessionID;
    return DP_DATABASE_SUCCESS;
}
/**************************************************************************
// Name						:	 InsertConsolidatedRecord()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to insert the consolidate record into database
**************************************************************************/
S32BIT CReportConfigWrapper::InsertConsolidatedRecord(QString in_strTestID, U16BIT in_TCID)
{
    QString strQuery = "";
    U32BIT ulExecutionID = 0, ulExecSessionID = 0;
    QString strTestID = "";
    QString time_format = "yyyy-MM-dd HH:mm:ss";
    QDateTime m_DateTime = QDateTime::currentDateTime();
    QString strSysTime = m_DateTime.toString(time_format);
    U16BIT usCycleno;
    bool bRetVal = 0;
    QSqlQuery databaseQuery(m_objDataBase);

    if(GetLastExecutionSessionID(&ulExecSessionID))
    {
        return DP_DATABASE_FAILURE;
    }

    if(GetLastExecutionID(&ulExecutionID))
    {
        return DP_DATABASE_FAILURE;
    }
    strTestID = "crpt";
    usCycleno = 1;
    strQuery = QString::asprintf("Insert into %s(ESID,EID,TC,TCID,MiniorCycleNo,MajorCycleNo,ST,StampTime)\
                    VALUES(%d, %d,'%s',%d, %d, %d,'%s','%s')",strTestID.toStdString().c_str(),\
                    ulExecSessionID, ulExecutionID + 1,in_strTestID.toStdString().c_str(),in_TCID, usCycleno , usCycleno, strSysTime.toStdString().c_str(),\
                    strSysTime.toStdString().c_str());
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_DATABASE_FAILURE;
    }

    return DP_DATABASE_SUCCESS;
}
/**************************************************************************
// Name						:	 GetLastExecutionSessionID()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to get last execution session ID from database
**************************************************************************/
S32BIT CReportConfigWrapper::GetLastExecutionSessionID(U32BIT* out_ulExecSessionID)
{
    QSqlQuery databaseQuery(m_objDataBase);
    U32BIT ulExecSessionID = 0;
    bool bRetVal = 0;

    databaseQuery.prepare("select * from me ORDER BY ESID DESC LIMIT 1");
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_DATABASE_FAILURE;
    }
    else
    {
        while(databaseQuery.next())
        {
            QSqlRecord m_Record = databaseQuery.record();
            ulExecSessionID = (U32BIT)m_Record.value(2).toInt();
        }
    }

    *out_ulExecSessionID = ulExecSessionID;

    return DP_DATABASE_SUCCESS;
}
/**************************************************************************
// Name						:	 InsertMainExec()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to insert the me detable in database
**************************************************************************/
S32BIT CReportConfigWrapper::InsertMainExec(U16BIT in_ucModeSubSys, QString in_strSystemName)
{
    U32BIT ulSessionID = 0;
    U32BIT ulExeSession = 0;
    QString strSystem	= "";
    QString strQuery	= "";
    bool bRetVal = 0;
    in_ucModeSubSys = 1;
    QString time_format = "yyyy-MM-dd HH:mm:ss";
    QDateTime m_DateTime = QDateTime::currentDateTime();
    QString strSysTime = m_DateTime.toString(time_format);

    QSqlQuery databaseQuery(m_objDataBase);

    /* Get the Session ID*/
    if(GetLastLoginSession(&ulSessionID))
    {
        return DP_DATABASE_FAILURE;
    }

    if(GetLastExecutionSessionID(&ulExeSession))	/* Get the last Execution Session ID*/
    {
        return DP_DATABASE_FAILURE;
    }

    strQuery = QString::asprintf("Insert into me(SID,ESID,ExecutionMode,System,ST)VALUES(%d,%d,%d,'%s','%s')", ulSessionID, ulExeSession + 1, in_ucModeSubSys, in_strSystemName.toStdString().c_str(), strSysTime.toStdString().c_str());
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return (!bRetVal);
    }

    return DP_DATABASE_SUCCESS;
}
/**************************************************************************
// Name						:	 ReadUserDetails()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to read the user details from database
**************************************************************************/
S32BIT CReportConfigWrapper::ReadUserDetails(QVector<S_USER_DETAILS> &Records, QString in_strUserID)
{
    QString strQuery = "";
    S_USER_DETAILS SUserDetails;
    QSqlQuery databaseQuery(m_objDataBase);
    bool bRetVal = 0;

    // Build the SQL statement
    strQuery = QString::asprintf("SELECT * FROM user_details WHERE UserID = '%s'",in_strUserID.toStdString().c_str());
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_RPT_WRAP_ERR_READ_USER_DETAILS;
    }
    else
    {
        while(databaseQuery.next())
        {
            QSqlRecord m_Record = databaseQuery.record();
    
            strcpy(SUserDetails.qsUserName,m_Record.value(1).toByteArray());
            strcpy(SUserDetails.qsUserID,m_Record.value(2).toByteArray());
            strcpy(SUserDetails.qsPassword,m_Record.value(2).toByteArray());

            SUserDetails.sPrivilege = (int)m_Record.value(4).toInt();//privilege

            Records.append(SUserDetails);
        }
    }
    return DP_DATABASE_SUCCESS;
}
/**************************************************************************
// Name						:	 ReadAllUserDetails()
// Author					:	 PARTHIPAN A
// Global Variables affected:	 NA
// Created Date				:    29-10-2022
// Revision Date			:
// Reason for Revising		:
// Description				:	This function is used to read the all user details from database
**************************************************************************/
S32BIT CReportConfigWrapper::ReadAllUserDetails(QVector<S_USER_DETAILS> &Records)
{
    QString strQuery = "";
    S_USER_DETAILS SUserDetails;
    QSqlQuery databaseQuery(m_objDataBase);
    bool bRetVal = 0;

    // Build the SQL statement
    strQuery.asprintf("SELECT * FROM user_details");
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_DATABASE_FAILURE;
    }
    else
    {
        while(databaseQuery.next())
        {
            QSqlRecord m_Record = databaseQuery.record();

            strcpy(SUserDetails.qsUserName,m_Record.value(1).toByteArray());
            strcpy(SUserDetails.qsUserID,m_Record.value(2).toByteArray());
            strcpy(SUserDetails.qsPassword,m_Record.value(2).toByteArray());
            SUserDetails.sPrivilege = (int)m_Record.value(4).toInt();//privilege

            Records.append(SUserDetails);
        }
    }

    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::InsertUserDetails(QVector<S_USER_DETAILS> &Records)
{
    QString strQuery = "";
    char carrPassword[32] = {0};
    S_USER_DETAILS sUserDetails;
    sUserDetails =Records.at(0);
    QSqlQuery databaseQuery(m_objDataBase);
    bool bRetVal = 0;

    strcpy(carrPassword,sUserDetails.qsPassword);

    /** Open the User details table and Read all User details of the given User Name **/
    strQuery = QString::asprintf("SELECT * from user_details where UserID = '%s'",sUserDetails.qsUserID);
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_RPT_WRAP_INSERT_DETAIL_ERR;
    }

    while(databaseQuery.next())
    {
        QSqlRecord m_Record = databaseQuery.record();
        if(m_Record.count() > 0)
        {
            qDebug("%s","Login Id already exists");
            return DP_RPT_WRAP_INSERT_DETAIL_ERR;
        }
        break;
    }

    // Build the SQL statement
    strQuery = QString::asprintf("INSERT INTO user_details(UserID,UserName,Password,Privilige)VALUES('%s','%s',AES_ENCRYPT('%s','%s'),%d)",\
                     sUserDetails.qsUserID,sUserDetails.qsUserName,carrPassword,sUserDetails.qsUserID,sUserDetails.sPrivilege);
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_RPT_WRAP_INSERT_DETAIL_ERR;
    }

    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::UpdateUserDetails(S_USER_DETAILS in_sUserDetails, U8BIT in_ucPrivilege)
{
    QString strQuery = "";
    char carrPassword[32] = {0};
    QSqlQuery databaseQuery(m_objDataBase);
    bool bRetVal;

    strcpy(carrPassword,in_sUserDetails.qsPassword);

    /** Open the User details table and update user details of the given User Name **/
    if(in_ucPrivilege == 0)//0- normal user
    {
        strQuery = QString::asprintf("UPDATE user_details set UserName = '%s' where UserID = '%s'",in_sUserDetails.qsUserName,in_sUserDetails.qsUserID);
    }
    else if(in_ucPrivilege == 1)//developer
    {
        strQuery = QString::asprintf("UPDATE user_details set Password = AES_ENCRYPT('%s','%s') where UserID = '%s'",carrPassword,in_sUserDetails.qsUserID,in_sUserDetails.qsUserID);
    }
    else
    {
        strQuery.asprintf("UPDATE user_details set UserName = '%s' where UserID = '%s'",in_sUserDetails.qsUserName,in_sUserDetails.qsUserID);
    }
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_RPT_WRAP_ERR_UPDATE_USER_DETAILS;
    }

  return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::DeleteUserDetails(QString in_strUserID)
{
    QString strQuery = "";
    QSqlQuery databaseQuery(m_objDataBase);
    bool bRetVal = 0;

    /** Open the User details table and update user details of the given User Name **/
    strQuery = QString::asprintf("DELETE FROM user_details where UserID = '%s'",in_strUserID.toStdString().c_str());
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_DATABASE_FAILURE;
    }

    return DP_DATABASE_SUCCESS;
}


S32BIT CReportConfigWrapper::VerifyUserDetails(QString in_strUserID, QString in_strPassword)
{
    QString strQuery = "";
    bool bIsUserMatched = false;
    QSqlQuery databaseQuery(m_objDataBase);
    bool bRetVal = 0;
    // Build the SQL statement
    strQuery = QString::asprintf("SELECT * FROM user_details WHERE (UserID = '%s' AND Password = AES_ENCRYPT('%s','%s') )",in_strUserID.toStdString().c_str(), in_strPassword.toStdString().c_str(), in_strUserID.toStdString().c_str());
    databaseQuery.prepare(strQuery);
    bRetVal = databaseQuery.exec();
    if(!bRetVal)
    {
        qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
        return DP_RPT_WRAP_ERR_VERIFY_DETAILS;
    }
    else
    {
        while(databaseQuery.next())
        {
            QSqlRecord m_Record = databaseQuery.record();
            for(int i=0; i< m_Record.count(); i++)
            {
                m_Record.value(2).toString();//UserID
                m_Record.value(3).toString();//password
                m_Record.value(4).toInt();//privilege
                bIsUserMatched = true;
            }
        }
    }
    if(bIsUserMatched == false)
    {
        qDebug("USER does not Exists");
        return DP_RPT_WRAP_ERR_VERIFY_DETAILS;
    }
    return DP_DATABASE_SUCCESS;
}

void CReportConfigWrapper::GetErrorMessage(S32BIT in_s32ErrVal, PS8BIT out_pcErrMsg)
{
    switch(in_s32ErrVal)
    {
    case DP_RPT_WRAP_ERR_CONN_DATABASE:
        sprintf(out_pcErrMsg, "DB : Error in Databse Connection");
        break;
	case DP_DATABASE_FAILURE:
	    sprintf(out_pcErrMsg, "DB : Error while executing query");
        break;
	case DP_RPT_WRAP_ERR_TR:
		sprintf(out_pcErrMsg, "DB : Error while inserting data into test registration");
        break;
	case DP_RPT_WRAP_ERR_READ_USER_DETAILS:
		sprintf(out_pcErrMsg, "DB : Error while reading user details");
        break;
	case DP_RPT_WRAP_INSERT_DETAIL_ERR:
		sprintf(out_pcErrMsg, "DB : Error while inserting user details");
        break;
    case DP_ERR_FILE_PATH:	
        sprintf(out_pcErrMsg, "DB : Error in File Opening");
        break;
	case DP_RPT_WRAP_ERR_UPDATE_USER_DETAILS:
		sprintf(out_pcErrMsg, "DB : Error updating user details");
        break;
	case DP_RPT_WRAP_ERR_VERIFY_DETAILS:
		sprintf(out_pcErrMsg, "DB : Error while executing query for user details verification");
        break;
    case DP_ERR_DB_NOT_OPENED_1:
        sprintf(out_pcErrMsg, "DB : Error in Databse Connection");
        break;
    case DP_ERR_INVALID_TEST_ID_1:
        sprintf(out_pcErrMsg, "DB : Invalid Test ID");
        break;
    case DP_ERR_INVALID_CFN_NUM_1:
		sprintf(out_pcErrMsg, "DB : Invalid Configuration Number");
		break;
    case DP_ERR_QUERY_EXE_1:
		sprintf(out_pcErrMsg, "DB : Error in Query execution");
		break;
    case DP_ERR_FILE_OPEN_1:
		sprintf(out_pcErrMsg, "DB : FIle Open faild");
		break;
    default:
        sprintf(out_pcErrMsg, "DB : Unknown Error");
        break;
	}
}


S32BIT CReportConfigWrapper::Insert_harmonic_measurement_Record(QVector <S_HARMONIC_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;
    
    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

	if(in_sRecordData.size() == 0)
	{
        return DP_DATABASE_FAILURE;
	}
	
    sRetVal = InsertConsolidatedRecord(in_strTestName, 400);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_harmonic_measurement (EID, ChannelNo, Inputfreq, InputPower, ExpMin2ndHarmonicRejection, ExpMin3rdHarmonicRejection, FundamentalFreq, 2ndHarmFreq, 3rdHarmFreq, FundamentalPwr, 2ndHarmPwr, 3rdHarmPwr, Measured_Power, MeasPathLoss, CalculatedPower, 2ndHarmonicRejection, 3rdHarmonicRejection, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputfreq, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dExpMin2ndHarmonicRejection, in_sRecordData.at(0).m_dExpMin3rdHarmonicRejection, in_sRecordData.at(0).m_dFundamentalFreq, in_sRecordData.at(0).m_d2ndHarmFreq, in_sRecordData.at(0).m_d3rdHarmFreq, in_sRecordData.at(0).m_dFundamentalPwr, in_sRecordData.at(0).m_d2ndHarmPwr, in_sRecordData.at(0).m_d3rdHarmPwr, in_sRecordData.at(0).m_dMeasured_Power, in_sRecordData.at(0).m_dMeasPathLoss, in_sRecordData.at(0).m_dCalculatedPower, in_sRecordData.at(0).m_d2ndHarmonicRejection, in_sRecordData.at(0).m_d3rdHarmonicRejection, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_noisefigure_measurement_Record(QVector <S_NOISEFIGURE_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;
    
    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

	if(in_sRecordData.size() == 0)
	{
        return DP_DATABASE_FAILURE;
	}
	
    sRetVal = InsertConsolidatedRecord(in_strTestName, 401);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_noisefigure_measurement (EID, ChannelNo, RFfreq, ExpNoiseFig, MeasNoiseFig, Result )VALUES( %d, %d, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dRFfreq, in_sRecordData.at(0).m_dExpNoiseFig, in_sRecordData.at(0).m_dMeasNoiseFig, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_powerdroop_measurement_Record(QVector <S_POWERDROOP_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;
    
    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

	if(in_sRecordData.size() == 0)
	{
        return DP_DATABASE_FAILURE;
	}
	
    sRetVal = InsertConsolidatedRecord(in_strTestName, 402);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_powerdroop_measurement (EID, ChannelNo, RFInputFreq, MeasCabeloss, InputPower, MeasPowerMarker1, MeasPowerMarker2, ExpMaxPowerDroop, CalPowerDroop, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dRFInputFreq, in_sRecordData.at(0).m_dMeasCabeloss, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dMeasPowerMarker1, in_sRecordData.at(0).m_dMeasPowerMarker2, in_sRecordData.at(0).m_dExpMaxPowerDroop, in_sRecordData.at(0).m_dCalPowerDroop, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_rf_pulse_rise_fall_time_meas_Record(QVector <S_RF_PULSE_RISE_FALL_TIME_MEAS> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;
    
    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

	if(in_sRecordData.size() == 0)
	{
        return DP_DATABASE_FAILURE;
	}
	
    sRetVal = InsertConsolidatedRecord(in_strTestName, 403);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_rf_pulse_rise_fall_time_meas (EID, ChannelNo, InputPower, InputFrequency, ExpRiseFallTime, MeasRiseTime, MeasFallTime, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dExpRiseFallTime, in_sRecordData.at(0).m_dMeasRiseTime, in_sRecordData.at(0).m_dMeasFallTime, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_rx_gain_measurement_Record(QVector <S_RX_GAIN_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
       S32BIT sRetVal = 0;
       U32BIT ulExeID = 0;

       bool bRetVal = false;

       QSqlQuery databaseQuery(m_objDataBase);

       if(in_sRecordData.size() == 0)
       {
           return DP_DATABASE_FAILURE;
       }

       sRetVal = InsertConsolidatedRecord(in_strTestName, 404);
       if(sRetVal != 0)
       {
           return DP_DATABASE_FAILURE;
       }

       sRetVal = GetLastExecutionID(&ulExeID);
       if(sRetVal)
       {
           return DP_DATABASE_FAILURE;
       }

       while(in_sRecordData.size())
       {
          csQuery = QString::asprintf("insert into moduleid_rx_gain_measurement (EID, ChannelNo, InputFrequency, InputPower, ExpMinimumGain, ExpMaximumGain, MeasuredFreq, MeasuredGain, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dExpMinimumGain, in_sRecordData.at(0).m_dExpMaximumGain, in_sRecordData.at(0).m_dMeasuredFreq, in_sRecordData.at(0).m_dMeasuredGain, in_sRecordData.at(0).m_u8Result);

           databaseQuery.prepare(csQuery);
           bRetVal = databaseQuery.exec();
           if(!bRetVal)
           {
               qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
               return DP_DATABASE_FAILURE;
           }
           in_sRecordData.removeAt(0);
       }
       return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_input_return_loss_Record(QVector <S_INPUT_RETURN_LOSS> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 405);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_input_return_loss (EID, ChannelNo, InputPower, InputFrequency, ExpMaxReturnloss, MeasReturnLoss, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dExpMaxReturnloss, in_sRecordData.at(0).m_dMeasReturnLoss, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_output_return_loss_Record(QVector <S_OUTPUT_RETURN_LOSS> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 406);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_output_return_loss (EID, ChannelNo, InputPower, InputFrequency, ExpMaxReturnloss, MeasReturnLoss, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dExpMaxReturnloss, in_sRecordData.at(0).m_dMeasReturnLoss, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_rx_isolation_measurement_Record(QVector <S_RX_ISOLATION_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
        S32BIT sRetVal = 0;
        U32BIT ulExeID = 0;

        bool bRetVal = false;

        QSqlQuery databaseQuery(m_objDataBase);

        if(in_sRecordData.size() == 0)
        {
            return DP_DATABASE_FAILURE;
        }

        sRetVal = InsertConsolidatedRecord(in_strTestName, 409);
        if(sRetVal != 0)
        {
            return DP_DATABASE_FAILURE;
        }

        sRetVal = GetLastExecutionID(&ulExeID);
        if(sRetVal)
        {
            return DP_DATABASE_FAILURE;
        }

        while(in_sRecordData.size())
        {
           csQuery = QString::asprintf("insert into moduleid_rx_isolation_measurement (EID, ChannelNo, InputFrequency, InputPower, ExpIsolation, CalcIsolation, MeasureGainRxMode, MeasureGainIdleMode, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dExpIsolation, in_sRecordData.at(0).m_dCalcIsolation, in_sRecordData.at(0).m_dMeasureGainRxMode, in_sRecordData.at(0).m_dMeasureGainIdleMode, in_sRecordData.at(0).m_u8Result);

            databaseQuery.prepare(csQuery);
            bRetVal = databaseQuery.exec();
            if(!bRetVal)
            {
                qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
                return DP_DATABASE_FAILURE;
            }
            in_sRecordData.removeAt(0);
        }
        return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_tx_output_power_measurement_Record(QVector <S_TX_OUTPUT_POWER_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 407);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_tx_output_power_measurement (EID, ChannelNo, RFfreq, InputPower, OutputCableLoss, ExpectedOutputPower, MeasuredOutputPower, CalculatedOutputPower, MeasPowerWatt, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dRFfreq, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dOutputCableLoss, in_sRecordData.at(0).m_dExpectedOutputPower, in_sRecordData.at(0).m_dMeasuredOutputPower, in_sRecordData.at(0).m_dCalculatedOutputPower, in_sRecordData.at(0).m_dMeasPowerWatt, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::ReadTestCaseDetails(QVector<S_TESTCASE_NAME> &out_sVecTestCaseName, char *in_szSystemName)
{
    U32BIT in_u32SysID = 0;
    QString strQry = "";
    QSqlQuery objSqlQuery(m_objDataBase);
    objSqlQuery.clear();
    out_sVecTestCaseName.clear();

    /* Read System ID from from database */
    if(in_szSystemName == NULL)
        return DP_APP_ERR_INVALID_PARAM;

    strQry = QString::asprintf("SELECT systemconfig.SysID FROM systemconfig WHERE systemconfig.sysName='%s';",in_szSystemName);

    objSqlQuery = QSqlQuery(strQry);
    if((objSqlQuery.isSelect() == true) && (objSqlQuery.isActive() == true))
    {
        while(objSqlQuery.next() == true)
        {
            in_u32SysID = objSqlQuery.value(0).toInt();
            g_u32SystemId = in_u32SysID;
        }
    }
    else
    {
        qDebug() << objSqlQuery.lastError();
        qWarning("%s", objSqlQuery.lastError().text().toLocal8Bit().data());
        return DP_APP_ERR_QUERY_EXE;
    }

    /* Read details from test_case_config_table table from database */

    strQry = QString::asprintf("SELECT DISTINCT Test_Case_Name, Test_ID, Configuration_No FROM Test_Case_Config_Table WHERE SysID = %d ORDER BY Test_ID ASC", in_u32SysID);

    objSqlQuery = QSqlQuery(strQry);
    if((objSqlQuery.isSelect() == true) && (objSqlQuery.isActive() == true))
    {
        while(objSqlQuery.next() == true)
        {
            S_TESTCASE_NAME sTestcaseName;
            sTestcaseName.m_iID = objSqlQuery.value(1).toInt();
            memcpy(sTestcaseName.m_szTestCaseName, (objSqlQuery.value(0).toString().toStdString().c_str()), sizeof(sTestcaseName.m_szTestCaseName));
            sTestcaseName.m_iConfigNo = objSqlQuery.value(2).toInt();
            out_sVecTestCaseName.append(sTestcaseName);
        }
        return DP_DATABASE_SUCCESS;
    }
    else
    {
        qDebug() << objSqlQuery.lastError();
        qWarning("%s", objSqlQuery.lastError().text().toLocal8Bit().data());
        return DP_APP_ERR_QUERY_EXE;
    }

}
S32BIT CReportConfigWrapper::ReadTestCaseID(QString in_strTestName, U32BIT *out_u32TestCaseID)
{
    QString strQry = "";
    bool bQryExecSts = false;
    QSqlQuery objSqlQuery(m_objDataBase);
    objSqlQuery.clear();

    /* Read System ID from from database */
    if(out_u32TestCaseID == NULL)
        return DP_APP_ERR_INVALID_PARAM;

    strQry = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_Case_Name = '%s 'AND test_case_config_table.SysID = %d;",in_strTestName.toStdString().c_str(), g_u32SystemId);

    bQryExecSts = objSqlQuery.exec(strQry);
    if(bQryExecSts == false)
    {
        return DP_APP_ERR_QUERY_EXE;
    }

    while(objSqlQuery.next())
    {
       *out_u32TestCaseID = objSqlQuery.value(0).toInt();
    }

     return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::ReadTestCaseID_CableLoss(QString in_strTestName, U32BIT *out_u32TestCaseID)
{
    QString strQry = "";
    bool bQryExecSts = false;
    QSqlQuery objSqlQuery(m_objDataBase);
    objSqlQuery.clear();

    /* Read System ID from from database */
    if(out_u32TestCaseID == NULL)
        return DP_APP_ERR_INVALID_PARAM;

    strQry = QString::asprintf("SELECT test_case_config_table.Test_ID FROM test_case_config_table WHERE test_case_config_table.Test_Case_Name = '%s 'AND test_case_config_table.SysID = 13;",in_strTestName.toStdString().c_str());

    bQryExecSts = objSqlQuery.exec(strQry);
    if(bQryExecSts == false)
    {
        return DP_APP_ERR_QUERY_EXE;
    }

    while(objSqlQuery.next())
    {
       *out_u32TestCaseID = objSqlQuery.value(0).toInt();
    }

     return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_calib_cabloss_rpt_Record(QVector <S_CALIB_CABLOSS_RPT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 408);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into calib_cabloss_rpt (EID, PathID, PathName, RadioFreq, Amp, PathLoss, ST )VALUES( %d, %d, '%s', %lf, %lf, %lf, '%s' )", ulExeID, in_sRecordData.at(0).m_s32PathID, in_sRecordData.at(0).m_szPathName, in_sRecordData.at(0).m_dRadioFreq, in_sRecordData.at(0).m_dAmp, in_sRecordData.at(0).m_dPathLoss, in_sRecordData.at(0).m_szST);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_power_tuning_measurement_Record(QVector <S_POWER_TUNING_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 410);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_power_tuning_measurement (EID, ChannelNo, Frequency, InputPower, MeasuredOutputPower, Current, PulseWidth, PulsePeriod, CalculatedOutputPower, MeasPowerWatt, Efficiency, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dMeasuredOutputPower, in_sRecordData.at(0).m_dCurrent, in_sRecordData.at(0).m_dPulseWidth, in_sRecordData.at(0).m_dPulsePeriod, in_sRecordData.at(0).m_dCalculatedOutputPower, in_sRecordData.at(0).m_dMeasPowerWatt, in_sRecordData.at(0).m_dEfficiency, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_rx_outofband_rejection_measurement_Record(QVector <S_RX_OUTOFBAND_REJECTION_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 411);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_rx_outofband_rejection_measurement (EID, ChannelNo, InputFrequency, InputPower, ObservedGain, ExpRejection, CalculatedRejection, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dObservedGain, in_sRecordData.at(0).m_dExpRejection, in_sRecordData.at(0).m_dCalculatedRejection, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_phase_noise_measurement_Record(QVector <S_PHASE_NOISE_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_phase_noise_measurement (EID, ConnectorID, InputFrequency, InputPower, StimulasPathLoss, ExpectedOutputFrequency, OutputFrequency, ExpectedPhaseNoiseLevel, ObservedPhaseNoiseLevel, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_s8ConnectorID, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dStimulasPathLoss, in_sRecordData.at(0).m_dExpectedOutputFrequency, in_sRecordData.at(0).m_dOutputFrequency, in_sRecordData.at(0).m_dExpectedPhaseNoiseLevel, in_sRecordData.at(0).m_dObservedPhaseNoiseLevel, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_image_rejection_Record(QVector <S_IMAGE_REJECTION> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_image_rejection (EID, RFIFPort, BandDetails, ChannelNo, ImageNo, MSTCAtten, AGCAtten, RfFrequency, InputPower, RFCableloss, ImageFreq, MeasActpwr, ImagePwr, ExpMinRej, ImageRej, Result )VALUES( %d, '%s', '%s', %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_szRFIFPort, in_sRecordData.at(0).m_szBandDetails, in_sRecordData.at(0).m_s32ChannelNo, in_sRecordData.at(0).m_s32ImageNo, in_sRecordData.at(0).m_dMSTCAtten, in_sRecordData.at(0).m_dAGCAtten, in_sRecordData.at(0).m_dRfFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dRFCableloss, in_sRecordData.at(0).m_dImageFreq, in_sRecordData.at(0).m_dMeasActpwr, in_sRecordData.at(0).m_dImagePwr, in_sRecordData.at(0).m_dExpMinRej, in_sRecordData.at(0).m_dImageRej, in_sRecordData.at(0).m_s32Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_onedbcompressionpoint_Record(QVector <S_ONEDBCOMPRESSIONPOINT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_onedbcompressionpoint (EID, ChannelNo, RFIFPort, BandDetails, DCA1Atten, DCA2Atten, RFInptFreq, RFCableloss, RFpwr, MeasCableloss, MeasActPwr, RefGain, CalcGain, 1dBComPoint, Result )VALUES( %d, %d, '%s', '%s', %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_s32ChannelNo, in_sRecordData.at(0).m_szRFIFPort, in_sRecordData.at(0).m_szBandDetails, in_sRecordData.at(0).m_dDCA1Atten, in_sRecordData.at(0).m_dDCA2Atten, in_sRecordData.at(0).m_dRFInptFreq, in_sRecordData.at(0).m_dRFCableloss, in_sRecordData.at(0).m_dRFpwr, in_sRecordData.at(0).m_dMeasCableloss, in_sRecordData.at(0).m_dMeasActPwr, in_sRecordData.at(0).m_dRefGain, in_sRecordData.at(0).m_dCalcGain, in_sRecordData.at(0).m_d1dBComPoint, in_sRecordData.at(0).m_s32Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_rx_attenuator_validation_Record(QVector <S_RX_ATTENUATOR_VALIDATION> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_rx_attenuator_validation (EID, RFIFPort, BandDetails, ChannelNo, BandSel, DCA, RFinputfreq, RFinputPower, RFCableloss, RefPower, Measpower, MeasCBloss, CalcPower, ExpMinAtten, ExpMaxAtten, CalcAttenuation, Result )VALUES( %d, '%s', '%s', %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_szRFIFPort, in_sRecordData.at(0).m_szBandDetails, in_sRecordData.at(0).m_s32ChannelNo, in_sRecordData.at(0).m_s32BandSel, in_sRecordData.at(0).m_dDCA, in_sRecordData.at(0).m_dRFinputfreq, in_sRecordData.at(0).m_dRFinputPower, in_sRecordData.at(0).m_dRFCableloss, in_sRecordData.at(0).m_dRefPower, in_sRecordData.at(0).m_dMeaspower, in_sRecordData.at(0).m_dMeasCBloss, in_sRecordData.at(0).m_dCalcPower, in_sRecordData.at(0).m_dExpMinAtten, in_sRecordData.at(0).m_dExpMaxAtten, in_sRecordData.at(0).m_dCalcAttenuation, in_sRecordData.at(0).m_s32Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_rx_gain_measurement_sa_Record(QVector <S_RX_GAIN_MEASUREMENT_SA> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_rx_gain_measurement_sa (EID, RFIFPort, BandDetails, ChannelNo, DCA1Atten, DCA2Atten, IFFreq, Rf_inputfreq, RFCableloss, Rf_inputPower, MeasCableloss, Mesured_Power, Calculated_Power, ExpMinGain, ExpMaxGain, Calculated_Gain, Result )VALUES( %d, '%s', '%s', %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_szRFIFPort, in_sRecordData.at(0).m_szBandDetails, in_sRecordData.at(0).m_s32ChannelNo, in_sRecordData.at(0).m_dDCA1Atten, in_sRecordData.at(0).m_dDCA2Atten, in_sRecordData.at(0).m_dIFFreq, in_sRecordData.at(0).m_dRf_inputfreq, in_sRecordData.at(0).m_dRFCableloss, in_sRecordData.at(0).m_dRf_inputPower, in_sRecordData.at(0).m_dMeasCableloss, in_sRecordData.at(0).m_dMesured_Power, in_sRecordData.at(0).m_dCalculated_Power, in_sRecordData.at(0).m_dExpMinGain, in_sRecordData.at(0).m_dExpMaxGain, in_sRecordData.at(0).m_dCalculated_Gain, in_sRecordData.at(0).m_s32Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_vswr_measurement_Record(QVector <S_VSWR_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_vswr_measurement (EID, RFIFPort, BandDetails, ChannelNo, StartFreq, StopFreq, InputPortMaxFreq, inputPower, OutputPortMaxFreq, ExpectedVSWR, InputPortVSWR, OutputPortVSWR, Result )VALUES( %d, '%s', '%s', %d, %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_szRFIFPort, in_sRecordData.at(0).m_szBandDetails, in_sRecordData.at(0).m_s32ChannelNo, in_sRecordData.at(0).m_s32StartFreq, in_sRecordData.at(0).m_s32StopFreq, in_sRecordData.at(0).m_dInputPortMaxFreq, in_sRecordData.at(0).m_dinputPower, in_sRecordData.at(0).m_dOutputPortMaxFreq, in_sRecordData.at(0).m_dExpectedVSWR, in_sRecordData.at(0).m_dInputPortVSWR, in_sRecordData.at(0).m_dOutputPortVSWR, in_sRecordData.at(0).m_s32Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_spurious_measurement_Record(QVector <S_SPURIOUS_MEASUREMENT> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_spurious_measurement (EID, Span, InputFrequency, InputPower, StimulasPathLoss, OutputFrequency, OutputPower, MeasuredPathLoss, ExpMinSpurious, ObservedOffsetFreq, Observedspurious, Result )VALUES( %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_dSpan, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dStimulasPathLoss, in_sRecordData.at(0).m_dOutputFrequency, in_sRecordData.at(0).m_dOutputPower, in_sRecordData.at(0).m_dMeasuredPathLoss, in_sRecordData.at(0).m_dExpMinSpurious, in_sRecordData.at(0).m_dObservedOffsetFreq, in_sRecordData.at(0).m_dObservedspurious, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_moduleid_biteisolation_meas_Record(QVector <S_MODULEID_BITEISOLATION_MEAS> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_biteisolation_meas (EID, ChannelNo, Inputfreq, InputPower, StimulusPathloss, MeasuredPathloss, SetPower, Measured_PowerANTIN, Measured_Power, ExpIsolation, MeasIsolation, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_s32ChannelNo, in_sRecordData.at(0).m_dInputfreq, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dStimulusPathloss, in_sRecordData.at(0).m_dMeasuredPathloss, in_sRecordData.at(0).m_dSetPower, in_sRecordData.at(0).m_dMeasured_PowerANTIN, in_sRecordData.at(0).m_dMeasured_Power, in_sRecordData.at(0).m_dExpIsolation, in_sRecordData.at(0).m_dMeasIsolation, in_sRecordData.at(0).m_s32Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_moduleid_blanking_isolation_meas_Record(QVector <S_MODULEID_BLANKING_ISOLATION_MEAS> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_blanking_isolation_meas (EID, ChannelNo, Inputfreq, InputPower, StimulusPathLoss, MeasuredPathLoss, SetPower, MeasBypassPower, MeasBlankingPower, ExpIsolation, MeasIsolation, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_s32ChannelNo, in_sRecordData.at(0).m_dInputfreq, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dStimulusPathLoss, in_sRecordData.at(0).m_dMeasuredPathLoss, in_sRecordData.at(0).m_dSetPower, in_sRecordData.at(0).m_dMeasBypassPower, in_sRecordData.at(0).m_dMeasBlankingPower, in_sRecordData.at(0).m_dExpIsolation, in_sRecordData.at(0).m_dMeasIsolation, in_sRecordData.at(0).m_s32Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_moduleid_filter_rejection_Record(QVector <S_MODULEID_FILTER_REJECTION> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_filter_rejection (EID, ChannelNo, InputPower, HPFLoss, NotchLoss, LPFLoss, MaxGain500_2000MHz, MaxGain10_18GHz, HPFRejection, NotchRejection, LPFRejection, ExpRejection, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_s32ChannelNo, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dHPFLoss, in_sRecordData.at(0).m_dNotchLoss, in_sRecordData.at(0).m_dLPFLoss, in_sRecordData.at(0).m_dMaxGain500_2000MHz, in_sRecordData.at(0).m_dMaxGain10_18GHz, in_sRecordData.at(0).m_dHPFRejection, in_sRecordData.at(0).m_dNotchRejection, in_sRecordData.at(0).m_dLPFRejection, in_sRecordData.at(0).m_dExpRejection, in_sRecordData.at(0).m_s32Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}

S32BIT CReportConfigWrapper::Insert_moduleid_tx_attenuator_validation_Record(QVector <S_MODULEID_TX_ATTENUATOR_VALIDATION> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_tx_attenuator_validation (EID, ChannelNo, InputFrequency, AppliedAttenuation, InputPower, MeasuredPower, ExpMinAttenuation, ExpMaxAttenuation, CalcAttenuation, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dAppliedAttenuation, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dMeasuredPower, in_sRecordData.at(0).m_dExpMinAttenuation, in_sRecordData.at(0).m_dExpMaxAttenuation, in_sRecordData.at(0).m_dCalcAttenuation, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_moduleid_tx_phaseshifter_calibration_Record(QVector <S_MODULEID_TX_PHASESHIFTER_CALIBRATION> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_tx_phaseshifter_calibration (EID, Channelno, InputFrequency, InputPower, Phase, ExpMinPhase, ExpMaxPhase, MeasuredPhase, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8Channelno, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dPhase, in_sRecordData.at(0).m_dExpMinPhase, in_sRecordData.at(0).m_dExpMaxPhase, in_sRecordData.at(0).m_dMeasuredPhase, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
S32BIT CReportConfigWrapper::Insert_moduleid_tx_phaseshifter_validation_Record(QVector <S_MODULEID_TX_PHASESHIFTER_VALIDATION> &in_sRecordData, QString in_strTestName)
{
    QString csQuery = "";
    S32BIT sRetVal = 0;
    U32BIT ulExeID = 0;

    bool bRetVal = false;

    QSqlQuery databaseQuery(m_objDataBase);

    if(in_sRecordData.size() == 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = InsertConsolidatedRecord(in_strTestName, 3);
    if(sRetVal != 0)
    {
        return DP_DATABASE_FAILURE;
    }

    sRetVal = GetLastExecutionID(&ulExeID);
    if(sRetVal)
    {
        return DP_DATABASE_FAILURE;
    }

    while(in_sRecordData.size())
    {
       csQuery = QString::asprintf("insert into moduleid_tx_phaseshifter_validation (EID, ChannelNo, InputFrequency, InputPower, Phase, ExpMinPhase, ExpMaxPhase, MeasuredPhase, Result )VALUES( %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %d )", ulExeID, in_sRecordData.at(0).m_u8ChannelNo, in_sRecordData.at(0).m_dInputFrequency, in_sRecordData.at(0).m_dInputPower, in_sRecordData.at(0).m_dPhase, in_sRecordData.at(0).m_dExpMinPhase, in_sRecordData.at(0).m_dExpMaxPhase, in_sRecordData.at(0).m_dMeasuredPhase, in_sRecordData.at(0).m_u8Result);

        databaseQuery.prepare(csQuery);
        bRetVal = databaseQuery.exec();
        if(!bRetVal)
        {
            qWarning("%s", databaseQuery.lastError().text().toLocal8Bit().data());
            return DP_DATABASE_FAILURE;
        }
        in_sRecordData.removeAt(0);
    }
    return DP_DATABASE_SUCCESS;
}
