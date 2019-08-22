#pragma once
#include "BasicInfo.h"

class OtharPlayer_Com : public BasicInfo
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	OtharPlayer_Com* Clone() override;

public:
	OtharPlayer_Com();
	~OtharPlayer_Com();
};

