#pragma once

JEONG_BEGIN

class OtharPlayer_Com;
class JEONG_DLL OTManager
{
public:
	void InsertOT(size_t Key, OtharPlayer_Com* OT);
	OtharPlayer_Com* FindOT(size_t Key);
	void DeleteOT(size_t key);
	void DeleteAll();

private:
	unordered_map<size_t, OtharPlayer_Com*> m_OTMap;

private:
	CLASS_IN_SINGLE(OTManager)
};

JEONG_END

