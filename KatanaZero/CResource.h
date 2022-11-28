#pragma once
#include <functional>

class CResource
{
public:
	CResource();
	virtual ~CResource();


private:
	std::wstring m_strKey;
	std::wstring m_strRelativePath;

public:
	virtual bool Load(std::wstring _strFilePath) =0 ;
	void SetKey(std::wstring _strKey) { m_strKey = _strKey; }
	void SetRelativePath(std::wstring _strRelativePath) { m_strRelativePath = _strRelativePath; }

	std::wstring& GetKey() { return m_strKey; }
	std::wstring& GetRelativePath() { return m_strRelativePath; }
	

};

