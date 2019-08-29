#include "stdafx.h"
#include "OtharPlayer_Com.h"

JEONG_USING

OtharPlayer_Com::OtharPlayer_Com()
{
}


OtharPlayer_Com::~OtharPlayer_Com()
{
}

bool OtharPlayer_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("OtherRenderer");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

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
