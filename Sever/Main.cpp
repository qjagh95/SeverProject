﻿#include "pch.h"
#include "IOCP.h"
#include "DBConnector.h"

#include <DataManager.h>
#include <Core.h>

int main()
{
	DBConnector::Get()->AllocateHandle();
	DBConnector::Get()->ConnectDataSource();
	DBConnector::Get()->ExecuteStatementDriect((SQLWCHAR*)L"DELETE FROM Players");

	IOCP* newIocp = new IOCP();

	newIocp->Init();
	newIocp->Run();

	SAFE_DELETE(newIocp);
	Core::Delete();
	DBConnector::Delete();
}