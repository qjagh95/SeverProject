#include "stdafx.h"
#include "PathManager.h"

JEONG_USING
SINGLETON_VAR_INIT(PathManager)

PathManager::PathManager()
{
	TCHAR strPath[MAX_PATH];
	//�������� �ִ°��� ��θ� �̾Ƴ���.
	GetModuleFileName(NULLPTR, strPath, MAX_PATH);

	//�ڿ������� ���Ƽ� //�� ������ ������ 0���� �ʱ�ȭ
	for (int i = lstrlen(strPath); i >= 0; i--)
	{
		if (strPath[i] == '\\' || strPath[i] == '//')
		{
			memset(strPath + (i + 1), 0, sizeof(TCHAR) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	PathMap.insert(make_pair(ROOT_PATH, strPath));

	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(SHADER_PATH, TEXT("Shader\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(DATA_PATH, TEXT("Data\\"));
}

PathManager::~PathManager()
{
}

bool PathManager::Init()
{
	return true;
}

bool PathManager::AddPath(const string & KeyName, const wstring & PathName, const string & BaseKeyName)
{
	if (FindPath(KeyName) != NULLPTR)
		return false;

	const TCHAR * pPath = FindPath(BaseKeyName);
	wstring FullPath;

	if (pPath != NULLPTR)
		FullPath = pPath;

	FullPath += PathName;

	PathMap.insert(make_pair(KeyName, FullPath));
	return true;
}

const TCHAR * PathManager::FindPath(const string & KeyName)
{
	unordered_map<string, wstring>::iterator FindIter = PathMap.find(KeyName);

	if (FindIter == PathMap.end())
		return NULLPTR;

	return FindIter->second.c_str();
}

const char * PathManager::FindPathMultiByte(const string & KeyName)
{
	const TCHAR* mPath = FindPath(KeyName);

	if (mPath == NULLPTR)
		return NULLPTR;

	memset(m_MultiBytePath, 0, sizeof(char) * MAX_PATH);

	// �����ڵ� ���ڿ��� ��Ƽ����Ʈ�� ��ȯ�Ѵ�.
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, mPath, -1, m_MultiBytePath, lstrlen(mPath) * 2, 0, 0);
#else
	memcpy(m_strPath, pPath, sizeof(TCHAR) * lstrlen(pPath));
#endif	// UNICODE

	return m_MultiBytePath;
}
