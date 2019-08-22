#include "ClientHeader.h"
#include "OtharPlayer_Com.h"


OtharPlayer_Com::OtharPlayer_Com()
{
}


OtharPlayer_Com::~OtharPlayer_Com()
{
}

bool OtharPlayer_Com::Init()
{
	return true;
}

int OtharPlayer_Com::Input(float DeltaTime)
{
	return 0;
}

int OtharPlayer_Com::Update(float DeltaTime)
{
	return 0;
}

int OtharPlayer_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void OtharPlayer_Com::Collision(float DeltaTime)
{
}

void OtharPlayer_Com::Render(float DeltaTime)
{
}

OtharPlayer_Com * OtharPlayer_Com::Clone()
{
	return nullptr;
}
