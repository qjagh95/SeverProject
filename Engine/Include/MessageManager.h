#pragma once

JEONG_BEGIN

class JEONG_DLL MessageManager
{
public:
	void SendNewPlayerMsg(SocketInfo* Socket);
	//IOData�� ���� ä���� �����Ѵ�.
	void RecvNewPlayerMsg(SocketInfo* Socket, IO_Data* Data);

private:
	void Send(SocketInfo* Socket, IO_Data* Data);


private:
	CLASS_IN_SINGLE(MessageManager)
};

JEONG_END
