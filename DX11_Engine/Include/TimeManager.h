#pragma once

JEONG_BEGIN

class Timer;
class JEONG_DLL TimeManager
{
public:
	bool Init();
	bool CreateTimer(const string& TagName);
	Timer* FindTimer(const string& TagName);

private:
	//��Ƽ������ȯ�濡���� Ÿ�̸Ӱ� ���ε��ƾ��Ѵ�.
	unordered_map<string, Timer*> m_TimerMap;

public:
	CLASS_IN_SINGLE(TimeManager)
};

JEONG_END