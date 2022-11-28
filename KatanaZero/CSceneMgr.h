#pragma once


class CScene;

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

	CScene* m_arrScene[(UINT)SCENE_TYPE::END]; // ��� �� ���
	CScene* m_pCurScene; //���� ��



public:
	static CSceneMgr* Create() { static CSceneMgr p; return &p; }

	void Initialize();
	void Update();
	void ComponentUpdate();
	void Clean();
	void Render(HDC _dc);

	CScene* GetCurScene() { return m_pCurScene; }


};

