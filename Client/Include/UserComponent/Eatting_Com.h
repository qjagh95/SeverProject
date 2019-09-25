#pragma once
#include "BasicInfo.h"

JEONG_USING
class Stage_Com;
class Eatting_Com : public BasicInfo
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Eatting_Com* Clone() override;

	void PlayerColl(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

	bool GetisUpdate() const { return m_isUpdate; }
	void SetIsUpdate(bool isUpdate) { m_isUpdate = isUpdate; }
	void SetStage(Stage_Com* Stage) { m_CurStage = Stage; }
	void SetID(int Index) { m_ID = Index; }
	int GetID() const { return m_ID; }

private:
	bool m_isUpdate;
	int m_ID;
	Stage_Com* m_CurStage;

public:
	Eatting_Com();
	~Eatting_Com();
};