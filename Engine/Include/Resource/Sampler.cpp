#include "stdafx.h"
#include "Sampler.h"

JEONG_USING

Sampler::Sampler()
	:m_SamplerState(NULLPTR)
{
}

Sampler::~Sampler()
{
	SAFE_RELEASE(m_SamplerState);
}

bool Sampler::CreateSampler(const string & TagName, D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE eU, D3D11_TEXTURE_ADDRESS_MODE eV, D3D11_TEXTURE_ADDRESS_MODE eW)
{
	SetTag(TagName);

	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.Filter = eFilter;
	SamplerDesc.AddressU = eU;
	SamplerDesc.AddressV = eV;
	SamplerDesc.AddressW = eW;
	SamplerDesc.MinLOD = -FLT_MAX;
	SamplerDesc.MaxLOD = FLT_MAX;

	//�׼����̼� (���ؽ��� �ɰ��ų� �Ҷ� ���)
	//�Ÿ��� �ֶ��� ������ ���̰�, �Ÿ��� ����ﶧ�� ����� �繰�� ������ �ø���.
	//LOD (Level of DeTail�� ����) ũ�� Ȯ����ҽ� ���ι���Ǵ� ������ ���Ѵ�. ����Ʈ 0
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	float f = 1.0f;
	memcpy(SamplerDesc.BorderColor, &f, sizeof(float) * 4);

	if (FAILED(Device::Get()->GetDevice()->CreateSamplerState(&SamplerDesc, &m_SamplerState)))
		return false;

	return true;
}

void Sampler::SetSamplerState(int RegisterNumber)
{
	Device::Get()->GetContext()->PSSetSamplers(RegisterNumber, 1, &m_SamplerState);
}
