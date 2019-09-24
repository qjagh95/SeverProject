#include "pch.h"
#include "IOCP.h"

#include <DataManager.h>
#include <Core.h>

int main()
{
	IOCP* newIocp = new IOCP();

	newIocp->Init();
	newIocp->Run();

	SAFE_DELETE(newIocp);
	Core::Delete();
	DataManager::Delete();
}