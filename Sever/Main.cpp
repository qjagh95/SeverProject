#include "pch.h"
#include "IOCP.h"

#include <DataManager.h>
#include <Core.h>


int main()
{
	IOCP* newIocp = new IOCP();
	MessageManager::Get();

	newIocp->Init();
	newIocp->Run();

	SAFE_DELETE(newIocp);
	MessageManager::Delete();
	Core::Delete();
	DataManager::Delete();
}