#pragma once

JEONG_USING

class DBConnector
{
public:
	//ODCB 핸들변수 초기화
	void InitHandle();
	//연결할 DBMS접속함수
	void ConnectDataSource();
	//쿼리문을 인자로받아 쿼리문 준비과정 없이 실행
	void SQLProcess(SQLWCHAR* SQL);
	//쿼리문을 실행시키기 위해 준비과정을 거치는 함수 위와 차이점은 여러번 실행히 이 함수를 사용하거라
	void PrepareStatement(SQLWCHAR* SQL);
	//실행준비된 쿼리문을 실행하라
	void ExcuteStatement();
	//쿼리문의 결과를 읽는다.
	void RetrieveResult();
	//할당했던 핸들을 모두 해제하라
	void DisconnectDataSource();

private:
	void ErrorMessage(SQLSMALLINT HandleType, SQLHANDLE Handle);

private:
	SQLHENV m_Henv;
	SQLHDBC m_Hdbc;
	SQLHSTMT m_Stmt;
	SQLRETURN m_ErrorMessage;

	SQLSMALLINT m_Length;
	SQLINTEGER m_Rec;
	SQLINTEGER m_Native;
	SQLWCHAR m_State[7];
	SQLWCHAR m_Message[256];

public:
	CLASS_IN_SINGLE(DBConnector)

};

