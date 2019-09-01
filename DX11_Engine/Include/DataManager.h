#pragma once
JEONG_BEGIN

class Player_Com;
class JEONG_DLL DataManager
{
public:
	void PushClient(SocketInfo* Socket);
	void DeleteSocket(SocketInfo* Socket);
	SocketInfo* FindSocket(size_t ClientID);
	vector<SocketInfo*>* GetClientList() {	return &m_vecClient; }
	void SetPlayerObject(GameObject* Player); 
	Player_Com* GetPlayer() const { return m_Player; }
	size_t GetClientCount() const { return m_vecClient.size(); }

	void PushInfo(Player_Com* Player);

public:
	static size_t m_ClientCount;

private:
	vector<SocketInfo*> m_vecClient;
	vector<PlayerInfo*> m_vecPlayerInfo;

	GameObject* m_PlayerObject;
	Player_Com* m_Player;

public:
	CLASS_IN_SINGLE(DataManager)
};

JEONG_END