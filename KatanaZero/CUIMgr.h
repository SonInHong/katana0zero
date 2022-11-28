#pragma once

class CUI;

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();

public:
	static CUIMgr* Create() { static CUIMgr p; return &p; }

	void Update();

	CUI* WhoAmIPointing(CUI*);

private:
	bool pressed;
	CUI* pressedWhere;
	CUI* FocusUI;

};

