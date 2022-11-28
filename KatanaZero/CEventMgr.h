#pragma once
#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include <map>

class CObject;

class CEventMgr
{
public:
	struct CEvent
	{
		EVENT_TYPE Event;
		DWORD_PTR lParam;
		DWORD rParam;
	};

private:
	CEventMgr();
	~CEventMgr();

public:
	static CEventMgr* Create() { static CEventMgr p; return &p; }
	void Event_CreateObj(CObject* _obj, GROUP_TYPE _type);
	void Event_DestroyObj(CObject* _obj);
	void Update();

private:
	std::vector<CEvent> EventArr;
	std::vector<CObject*> AboutToDie;

};

