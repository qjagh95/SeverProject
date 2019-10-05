#include "pch.h"
#include "DBConnector.h"

SINGLETON_VAR_INIT(DBConnector)

DBConnector::DBConnector()
{
}

DBConnector::~DBConnector()
{
	DisconnectDataSource();
}

void DBConnector::AllocateHandle()
{
	// 환경 핸들러 할당
	m_ErrorMessage = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_Henv);
	ErrorMessage(SQL_HANDLE_ENV, m_Henv);

	if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
	{
		// ODBC 드라이버 버전 명시
		m_ErrorMessage = SQLSetEnvAttr(m_Henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

		// 연결 핸들러 할당
		if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
			m_ErrorMessage = SQLAllocHandle(SQL_HANDLE_DBC, m_Henv, &m_Hdbc);

		ErrorMessage(SQL_HANDLE_DBC, m_Hdbc);
	}
}

void DBConnector::ConnectDataSource()
{
	m_ErrorMessage = SQLConnect(m_Hdbc, (SQLWCHAR*)L"PlayerDB", SQL_NTS, (SQLWCHAR*)L"qjagh95", SQL_NTS, (SQLWCHAR*)L"9568", SQL_NTS);
}

void DBConnector::ExecuteStatementDriect(SQLWCHAR * SQL)
{
	SQLCloseCursor(m_Stmt);

	if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
	{
		m_ErrorMessage = SQLAllocHandle(SQL_HANDLE_STMT, m_Hdbc, &m_Stmt);
		ErrorMessage(SQL_HANDLE_STMT, m_Stmt);
	}

	m_ErrorMessage = SQLExecDirect(m_Stmt, SQL, SQL_NTS);
	ErrorMessage(SQL_HANDLE_STMT, m_Stmt);

}

void DBConnector::PrepareStatement(SQLWCHAR * SQL)
{
	if (m_ErrorMessage == SQL_SUCCESS || m_ErrorMessage == SQL_SUCCESS_WITH_INFO)
	{
		m_ErrorMessage = SQLAllocHandle(SQL_HANDLE_STMT, m_Hdbc, &m_Stmt);
		ErrorMessage(SQL_HANDLE_STMT, m_Stmt);
	}

	m_ErrorMessage = SQLPrepare(m_Stmt, SQL, SQL_NTS);

	ErrorMessage(SQL_HANDLE_STMT, m_Stmt);
}

//실행준비된 쿼리문을 실행하라
void DBConnector::ExcuteStatement()
{
	m_ErrorMessage = SQLExecute(m_Stmt);
	ErrorMessage(SQL_HANDLE_STMT, m_Stmt);
}

//가변인자 템플릿으로 만들면 편할듯
void DBConnector::RetrieveResult()
{
	long PlayerID = 0;
	float PlayerScale = 0.0f;
	SQLLEN cid = 0;
	SQLLEN cal = 0;

	m_ErrorMessage = SQLBindCol(m_Stmt, 1, SQL_C_LONG, &PlayerID, sizeof(PlayerID), &cid);
	m_ErrorMessage = SQLBindCol(m_Stmt, 2, SQL_C_FLOAT, &PlayerScale, sizeof(PlayerScale), &cal);

	int Index = 1;

	while (true)
	{
		m_ErrorMessage = SQLFetch(m_Stmt);

		if (m_ErrorMessage == SQL_NO_DATA)
			break;

		printf("Ranking : %d // ID : %d Scale : %f \n", Index, PlayerID, PlayerScale);
		Index++;
	};

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

void DBConnector::ErrorMessage(SQLSMALLINT HandleType, SQLHANDLE Handle)
{
	if (m_ErrorMessage == SQL_NO_DATA)
		return;

	if (m_ErrorMessage != SQL_SUCCESS && m_ErrorMessage != SQL_SUCCESS_WITH_INFO)
	{
		wstring Temp;
		SQLGetDiagRec(HandleType, Handle, ++m_Rec, m_State, &m_Native, m_Message, sizeof(m_Message), &m_Length);
		Temp = m_Message;

		wcout << Temp.c_str() << endl;
	}
}
