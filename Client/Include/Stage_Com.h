#pragma once
class Eatting;
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

	void CreateEatting(const Vector3& Pos, const Vector3& RGB, float Scale);

private:
	vector<GameObject*> m_vecAllEatting;
	unordered_map<__int64, GameObject*> m_AllEattingMap; //매모리를 희생해서 찾는속도를 빠르게하겠다.
	list<GameObject*> m_UpdateEatting;
	static __int64 m_Count;
	Vector2 m_WinSize;

public:
	Stage_Com();
	Stage_Com(const Stage_Com& CopyData);
	~Stage_Com();
};