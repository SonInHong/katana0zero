#pragma once
class PathMgr
{
private:
	PathMgr();
	~PathMgr();

	wchar_t m_szContentPath[255];
public:
	static PathMgr* Create()
	{
		static PathMgr p;
		return &p;
	}

public:
	void Initialize();
	const wchar_t* GetContentPath() { return m_szContentPath; }
};

