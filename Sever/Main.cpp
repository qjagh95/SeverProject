#include "pch.h"
#include "IOCP.h"

int main()
{
	IOCP* newIocp = new IOCP();
	newIocp->Init();

	newIocp->Run();

	delete newIocp;
}