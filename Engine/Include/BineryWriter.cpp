#include "stdafx.h"
#include "BineryWriter.h"

JEONG_USING

BineryWrite::BineryWrite(const string& FileName)
{
	wstring Temp;
	Temp = PathManager::Get()->FindPath(DATA_PATH);
	
	if (Temp.empty() == true)
		return;

	Temp += CA2W(FileName.c_str());
	string Temp2 = CW2A(Temp.c_str());

	m_WriteFile = ofstream(Temp2.c_str(), ios_base::binary);
}

BineryWrite::BineryWrite(const wstring& FileName)
{
	wstring Temp;
	Temp = PathManager::Get()->FindPath(DATA_PATH);

	if (Temp.empty() == true)
		return;

	Temp += FileName;

	m_WriteFile.open(Temp.c_str(), ios::binary);
}

BineryWrite::~BineryWrite()
{
	m_WriteFile.close();
}

void BineryWrite::WriteData(const bool& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(bool));
}

void BineryWrite::WriteData(const int& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(int));
}

void BineryWrite::WriteData(const float& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(float));
}

void BineryWrite::WriteData(const double& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(double));
}

void BineryWrite::WriteData(const Vector2 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector2));
}

void BineryWrite::WriteData(const Vector3 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector3));
}

void BineryWrite::WriteData(const Vector4 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector4));
}

void BineryWrite::WriteData(const string & Data)
{
	size_t Datalen = Data.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Data.c_str(), Datalen);
}

void BineryWrite::WriteData(const wstring & Data)
{
	string Temp = CW2A(Data.c_str());
	size_t Datalen = Data.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Temp.c_str(), Datalen);
}

void BineryWrite::WriteData(const char * Data)
{
	string Temp = Data;
	size_t Datalen = Temp.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Data, Datalen);
}

void BineryWrite::WriteData(const wchar_t * Data)
{
	wstring Temp = Data;
	string Temp2 = CW2A(Temp.c_str());

	size_t Datalen = Temp2.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Temp2.c_str(), Datalen);
}
