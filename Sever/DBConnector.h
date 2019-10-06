#pragma once

JEONG_USING

class DBConnector
{
public:
	//ODCB �ڵ麯�� �ʱ�ȭ
	void InitHandle();
	//������ DBMS�����Լ�
	void ConnectDataSource();
	//�������� ���ڷι޾� ������ �غ���� ���� ����
	void SQLProcess(SQLWCHAR* SQL);
	//�������� �����Ű�� ���� �غ������ ��ġ�� �Լ� ���� �������� ������ ������ �� �Լ��� ����ϰŶ�
	void PrepareStatement(SQLWCHAR* SQL);
	//�����غ�� �������� �����϶�
	void ExcuteStatement();
	//�������� ����� �д´�.
	void RetrieveResult();
	//�Ҵ��ߴ� �ڵ��� ��� �����϶�
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

