#pragma once

JEONG_USING
class Player_Com;
class Stage_Com : public UserComponent_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Stage_Com* Clone() override;

	void SetPlayer(Player_Com* Player) { m_MainPlayer = Player; }
	void CreateEatting(const Vector3& Pos, const Vector4& RGB, size_t ID);
	size_t GetUpdateEattingSize() const { return m_UpdateEatting.size(); }
	void DeleteEatObject(int ID);
	Player_Com* GetMainPlayer() { return m_MainPlayer; }

private:
	vector<GameObject*> m_UpdateEatting;
	Vector2 m_WinSize;
	Player_Com* m_MainPlayer;
	float m_TimeVar;
	float m_Sec;
	bool m_isTimeCheck;
	bool m_isCheck;

public:
	Stage_Com();
	Stage_Com(const Stage_Com& CopyData);
	~Stage_Com();
};
