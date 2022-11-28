#pragma once


class CObject;

class CScene
{
protected:
	std::vector<CObject*> m_arrObj[(UINT)GROUP_TYPE::END];
	std::wstring m_strName; // æ¿ ¿Ã∏ß


public:
	CScene();
	virtual ~CScene();

	void SetName(std::wstring _strName) { m_strName = _strName; }
	const std::wstring& GetName() { return m_strName; }

public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Update();
	virtual void ComponentUpdate();
	virtual void Render(HDC _dc);
	virtual void Load(const std::wstring& Path);

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}

	void ClearObject(GROUP_TYPE _eType);
	
	const std::vector<CObject*>& GetGroupObject(GROUP_TYPE _type)
	{
		return m_arrObj[(UINT)_type];
	}

	std::vector<CObject*>& GetUI()
	{
		return m_arrObj[(UINT)GROUP_TYPE::UI];
	}

	void Clean();

};

