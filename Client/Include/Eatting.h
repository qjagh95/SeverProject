#pragma once
#include "BasicInfo.h"
JEONG_USING

class Stage_Com;
class Eatting : public BasicInfo
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Eatting* Clone() override;

	void PlayerColl(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

	bool GetisUpdate() const { return m_isUpdate; }
	void SetIsUpdate(bool isUpdate) { m_isUpdate = isUpdate; }
	void SetStage(Stage_Com* Stage) { m_IncludeStage = Stage; }
	void SetIndex(int Index) { m_Index = Index; }
	__int64 GetIndex() const { return m_Index; }

private:
	bool m_isUpdate;
	Stage_Com* m_IncludeStage;
	__int64 m_Index;

public:
	Eatting();
	~Eatting();
};

