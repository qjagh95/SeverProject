#pragma once

JEONG_USING

class Player_Com;
class DataManager
{
public:
	void PushClient(SocketInfo* Socket);
	void DeleteSocket(SocketInfo* Socket);
	SocketInfo* FindClientIndex(size_t ClientID);
	SocketInfo* FindClientMap(size_t Key);
	PlayerInfo* FindPlayerInfoIndex(size_t ClientID);
	PlayerInfo* FindPlayerInfoKey(size_t Key);
	vector<SocketInfo*>* GetClientVec() { return &m_vecClient; }
	size_t GetClientCount() const { return m_vecClient.size(); }

	void PushPlayerInfo(const Vector4& Color, const Vector3& Pos, size_t ClientID, float Scale);

	vector<PlayerInfo*>* GetPlayerVec() { return &m_vecPlayerInfo; }
	size_t GetPlayerCount() const { return m_vecPlayerInfo.size(); }

	vector<EatInfo*>* GetEatVec() { return &m_vecEat; }

	void DeleteEat(int ID);

	void CloseAll();
	void Init();

public:
	static size_t m_ClientCount;
	static size_t m_PlayerCount;

private:
	vector<SocketInfo*> m_vecClient;
	unordered_map<size_t, SocketInfo*> m_ClientMap;

	vector<PlayerInfo*> m_vecPlayerInfo;
	//CliendID == Key
	unordered_map<size_t, PlayerInfo*> m_PlayerMap;

	vector<EatInfo*> m_vecEat;

public:
	CLASS_IN_SINGLE(DataManager)
};