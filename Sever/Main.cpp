#include "pch.h"
#include "IOCP.h"

int main()
{
	IOCP* newIocp = new IOCP();
	MessageManager::Get();

	newIocp->Init();
	newIocp->Run();

	delete newIocp;
	MessageManager::Delete();
}