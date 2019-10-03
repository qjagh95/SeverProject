#include "pch.h"
#include "DBConnector.h"

SINGLETON_VAR_INIT(DBConnector)

DBConnector::DBConnector()
{
}

DBConnector::~DBConnector()
{
}

void DBConnector::AllocateHandle()
{
	// 환경 핸들러 할당
	m_ErrorMessage = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_Henv);

	if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
	{
		// ODBC 드라이버 버전 명시
		m_ErrorMessage = SQLSetEnvAttr(m_Henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

		if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
		{
			// 연결 핸들러 할당
			m_ErrorMessage = SQLAllocHandle(SQL_HANDLE_DBC, m_Henv, &m_Hdbc);

			if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
				printf("Allocate Success\n");

			else 
			{
				SQLGetDiagRec(SQL_HANDLE_DBC, m_Hdbc, ++m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
				printf("%s : %ld : %ld : %s\n", m_State, m_Rec, m_Native, m_Message);
			}
		}
		else 
		{
			SQLGetDiagRec(SQL_HANDLE_ENV, m_Henv, ++m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
			printf("%s : %ld : %ld : %s\n", m_State, m_Rec, m_Native, m_Message);
		}
	}
	else
	{
		SQLGetDiagRec(SQL_HANDLE_ENV, m_Henv, m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
		printf("%s : %ld : %ld : %s\n", m_Henv, m_Rec, m_Native, m_Message);
	}
}

void DBConnector::ConnectDataSource()
{
	m_ErrorMessage = SQLConnect(m_Hdbc, (SQLWCHAR*)L"DSN", SQL_NTS, (SQLWCHAR*)L"사용자_이름", SQL_NTS, (SQLWCHAR*)L"비밀번호", SQL_NTS);
}

void DBConnector::ExecuteStatementDriect(SQLWCHAR * SQL)
{
	if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
	{
		m_ErrorMessage = SQLAllocHandle(SQL_HANDLE_STMT, m_Hdbc, &m_Stmt);
		printf("Connect Success\n");
	}
	else 
	{
		SQLGetDiagRec(SQL_HANDLE_DBC, m_Hdbc, ++m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
		printf("%s : %ld : %ld : %s\n", m_State, m_Rec, m_Native, m_Message);
	}

	m_ErrorMessage = SQLExecDirect(m_Stmt, SQL, SQL_NTS);

	if (m_Rec == SQL_SUCCESS)
		printf("Query Seuccess\n");

	else
	{
		SQLGetDiagRec(SQL_HANDLE_STMT, m_Stmt, ++m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
		printf("%s : %ld : %ld : %s\n", m_State, m_Rec, m_Native, m_Message);
	}

}

void DBConnector::PrepareStatement(SQLWCHAR * SQL)
{
	if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
	{
		m_ErrorMessage = SQLAllocHandle(SQL_HANDLE_STMT, m_Hdbc, &m_Stmt);
		printf("Connect Success\n");
	}
	else
	{
		SQLGetDiagRec(SQL_HANDLE_DBC, m_Hdbc, ++m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
		printf("%s : %ld : %ld : %s\n", m_State, m_Rec, m_Native, m_Message);
	}

	m_ErrorMessage = SQLPrepare(m_Stmt, SQL, SQL_NTS);

	if (m_ErrorMessage == SQL_SUCCESS)
		printf("\nQuery Prepare Success\n");

	else 
	{
		SQLGetDiagRec(SQL_HANDLE_STMT, m_Stmt, ++m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
		printf("\n%s : %ld : %ld : %s\n", m_State, m_Rec, m_Native, m_Message);
	}

}

void DBConnector::ExcuteStatement()
{
	m_ErrorMessage = SQLExecute(m_Stmt);

	if (m_ErrorMessage == SQL_SUCCESS) 
		printf("Query Execute Success\n");

	else
	{
		SQLGetDiagRec(SQL_HANDLE_STMT, m_Stmt, ++m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
		printf("\n%s : %ld : %ld : %s\n", m_State, m_Rec, m_Native, m_Message);

	}
}

void DBConnector::RetrieveResult()
{
	short choco_id;
	char choco_name[16];
	float choco_cal;
	SQLLEN cid, cna, cal;

	SQLBindCol(m_Stmt, 1, SQL_C_SHORT, &choco_id, sizeof(choco_id), &cid);
	SQLBindCol(m_Stmt, 2, SQL_C_CHAR, &choco_name, sizeof(choco_name), &cna);
	SQLBindCol(m_Stmt, 3, SQL_C_FLOAT, &choco_cal, sizeof(choco_cal), &cal);

	printf("id\tname\t\tcal\t\tstars");

	do 
	{
		m_ErrorMessage = SQLFetch(m_Stmt);
		printf("\n%d\t%s\t%f", choco_id, choco_name, choco_cal);

	} while (m_ErrorMessage != SQL_NO_DATA);

	SQLFreeStmt(m_Stmt, SQL_UNBIND);
}

void DBConnector::DisconnectDataSource()
{
	if (m_Stmt != NULLPTR)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_Stmt);
		m_Stmt = NULLPTR;
	}

	SQLDisconnect(m_Hdbc);

	if (m_Hdbc != NULLPTR)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_Hdbc);
		m_Hdbc = NULLPTR;
	}

	if (m_Henv != NULLPTR)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_Henv);
		m_Henv = NULLPTR;
	}

}
