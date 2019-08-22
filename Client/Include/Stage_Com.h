#pragma once
class Eatting;
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

	void CreateEatting(const Vector3& Pos);

private:
	list<Eatting*> m_vecEatting;

public:
	Stage_Com();
	Stage_Com(const Stage_Com& CopyData);
	~Stage_Com();
};

