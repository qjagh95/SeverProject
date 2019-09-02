#pragma once
JEONG_BEGIN

class Player_Com;
class JEONG_DLL DataManager
{
public:
	void PushClient(SocketInfo* Socket);
	void DeleteSocket(SocketInfo* Socket);
	SocketInfo* FindClientIndex(size_t ClientID);
	SocketInfo* FindClientMap(size_t Key);
	vector<SocketInfo*>* GetClientList() {	return &m_vecClient; }
	size_t GetClientCount() const { return m_vecClient.size(); }

	void PushMainPlayerInfo(Player_Com* Player, size_t ClientID);
	void PushOtherPlayerInfo(Vector4* Color, Vector3* Pos, float* Scale);

	vector<PlayerInfo*>* GetMainPlayerVec() { return &m_vecMainPlayerInfo; }
	size_t GetMainPlayerCount() const { return m_vecMainPlayerInfo.size(); }

public:
	static size_t m_ClientCount;
	static size_t m_OtherPlayerCount;

private:
	vector<SocketInfo*> m_vecClient;
	unordered_map<size_t, SocketInfo*> m_ClientMap;

	vector<PlayerInfo*> m_vecMainPlayerInfo;

	unordered_map<size_t, PlayerInfo*> m_OtherPlayerMap;

public:
	CLASS_IN_SINGLE(DataManager)
};

JEONG_END