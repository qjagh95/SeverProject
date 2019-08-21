#include "stdafx.h"
#include "Texture.h"

#include "../PathManager.h"
#include "../Device.h"

JEONG_USING

Texture::Texture()
	:m_ShaderResourceView(NULLPTR)
{
	memset(m_FullPath, 0, sizeof(TCHAR) * MAX_PATH);
}

Texture::~Texture()
{
	SAFE_RELEASE(m_ShaderResourceView);
	Safe_Delete_VecList(m_vecImage);
}

bool Texture::LoadTexture(const string & TextureName, const TCHAR * FileName, const string & PathKey)
{
	const TCHAR* Path = PathManager::Get()->FindPath(PathKey);
	TCHAR FullPath[MAX_PATH] = {};

	if (Path != NULLPTR)
		lstrcpy(FullPath, Path);

	lstrcat(FullPath, FileName);

	return LoadTextureFromFullPath(TextureName, FullPath);
}

bool Texture::LoadTexture(const string & TextureName, const vector<const TCHAR*>& FileNames, const string & PathKey)
{
	SetTag(TextureName);

	for (size_t i = 0; i < FileNames.size(); ++i)
	{
		const TCHAR* pPath = PathManager::Get()->FindPath(PathKey);
		TCHAR strFullPath[MAX_PATH] = {};

		if (pPath)
			lstrcpy(strFullPath, pPath);

		lstrcat(strFullPath, FileNames[i]);

		memcpy(strFullPath, strFullPath, sizeof(TCHAR) * MAX_PATH);

		TCHAR strExt[_MAX_EXT] = {};
		char ext[_MAX_EXT] = {};

#ifdef UNICODE
		_wsplitpath_s(FileNames[i], NULLPTR, 0, NULLPTR, 0, NULLPTR, 0, strExt,	_MAX_EXT);
		WideCharToMultiByte(CP_UTF8, 0, strExt, -1, ext, lstrlen(strExt), 0, 0);
#else
		_splitpath_s(FileNames[i], NULLPTR, 0, NULLPTR, 0, NULLPTR, 0, ext,	_MAX_EXT);
#endif // UNICODE
		_strupr_s(ext);

		ScratchImage* pImage = new ScratchImage();

		if (strcmp(ext, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(m_FullPath, DDS_FLAGS_NONE, NULLPTR, *pImage)))
				return false;
		}
		else if (strcmp(ext, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(m_FullPath, NULLPTR, *pImage)))
				return false;
		}
		else
		{
			if (FAILED(LoadFromWICFile(m_FullPath, WIC_FLAGS_NONE, NULLPTR, *pImage)))
				return false;
		}

		m_vecImage.push_back(pImage);
	}

	return CreateShaderResourceArray();
}

bool Texture::LoadTextureFromFullPath(const string & TextureName, const TCHAR * FullPath)
{
	SetTag(TextureName);
	lstrcpy(m_FullPath, FullPath);

	TCHAR Sep[_MAX_EXT] = {};
	char mSep[_MAX_EXT] = {};

#ifdef UNICODE
	//������ .Ȯ���ڸ� �����´�. ex(.jpeg, .png, .bmp)
	_wsplitpath_s(FullPath, NULLPTR, 0, NULLPTR, 0, NULLPTR, 0, Sep, _MAX_EXT);
	WideCharToMultiByte(CP_UTF8, 0, Sep, -1, mSep, lstrlen(Sep), 0, 0);
#else
	_splitpath_s(FullPath, NULLPTR, 0, NULLPTR, 0, NULLPTR, 0, mSep, _MAX_EXT);
#endif
	//���� �빮�ڷ� �ٲ��ִ� �Լ�.
	_strupr_s(mSep);

	ScratchImage* newImage = new ScratchImage();

	//Ȯ���ڿ� ���� �Լ��� ���ݾ� �ٸ���.
	if (strcmp(mSep, ".DDS") == 0)
	{
		//Tex�����Լ� �̹����� �ε��Ͽ� newImage�� �ȼ������� �����Ѵ�.
		if (FAILED(LoadFromDDSFile(FullPath, DDS_FLAGS_NONE, NULLPTR, *newImage)))
			return false;
	}
	else if (strcmp(mSep, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(FullPath, NULLPTR, *newImage)))
			return false;
	}
	else
	{
		//WIC�� �� �� ������ϵ� (jpg...png...bmp...)
		if (FAILED(LoadFromWICFile(FullPath, WIC_FLAGS_NONE, NULLPTR, *newImage)))
			return false;
	}

	m_vecImage.push_back(newImage);

	return CreateShaderResource();
}

bool Texture::LoadTextureFromFullPath(const string & TextureName, const vector<const TCHAR*>& FullPaths)
{
	SetTag(TextureName);

	for (size_t i = 0; i < FullPaths.size(); ++i)
	{
		memcpy(m_FullPath, FullPaths[i], sizeof(TCHAR) * MAX_PATH);

		TCHAR strExt[_MAX_EXT] = {};
		char ext[_MAX_EXT] = {};

#ifdef UNICODE
		_wsplitpath_s(FullPaths[i], NULLPTR, 0, NULLPTR, 0, NULLPTR, 0, strExt, _MAX_EXT);
		WideCharToMultiByte(CP_UTF8, 0, strExt, -1, ext, lstrlen(strExt), 0, 0);
#else
		_splitpath_s(vecFullPath[i], NULLPTR, 0, NULLPTR, 0, NULLPTR, 0, ext,_MAX_EXT);
#endif // UNICODE
		_strupr_s(ext);

		ScratchImage* pImage = new ScratchImage();

		if (strcmp(ext, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(m_FullPath, DDS_FLAGS_NONE, NULLPTR, *pImage)))
				return false;
		}

		else if (strcmp(ext, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(m_FullPath, NULLPTR, *pImage)))
				return false;
		}
		else
		{
			if (FAILED(LoadFromWICFile(m_FullPath, WIC_FLAGS_NONE, NULLPTR, *pImage)))
				return false;
		}

		m_vecImage.push_back(pImage);
	}

	return CreateShaderResourceArray();
}

bool Texture::CreateShaderResource()
{
	//�⺻�����Լ��� ����Ͽ� �̹����� �ȼ������� ShaderResourceView������ �ѱ��.
	if (FAILED(CreateShaderResourceView(Device::Get()->GetDevice(), m_vecImage[0]->GetImages(), m_vecImage[0]->GetImageCount(), m_vecImage[0]->GetMetadata(), &m_ShaderResourceView)))
		return false;

	return true;
}

bool Texture::CreateShaderResourceArray()
{
	return true;
}

void Texture::SetShaderResource(int RegisterNumber)
{
	//�ȼ����̴��� ������ �ѱ��.
	Device::Get()->GetContext()->PSSetShaderResources(RegisterNumber, 1, &m_ShaderResourceView);
}