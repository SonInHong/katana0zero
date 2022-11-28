#include "pch.h"
#include "CEventMgr.h"

CEventMgr::CEventMgr()
	:EventArr{}
	, AboutToDie{}
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::Event_CreateObj(CObject* _obj, GROUP_TYPE _type)
{
	CEvent p;
	p.Event = EVENT_TYPE::CREATEOBJECT;
	p.lParam = (DWORD_PTR)_obj;
	p.rParam = (DWORD)_type;

	EventArr.push_back(p);
}

void CEventMgr::Event_DestroyObj(CObject* _obj)
{
	CEvent p;
	p.Event = EVENT_TYPE::DESTROYOBJECT;
	p.lParam = (DWORD_PTR)_obj;

	EventArr.push_back(p);
}

void CEventMgr::Update()
{
	for (int i = 0; i < AboutToDie.size(); ++i)
	{
		AboutToDie[i]->Destroy();
		delete AboutToDie[i];
	}

	AboutToDie.clear();

	for (int i = 0; i < EventArr.size(); ++i)
	{
		switch (EventArr[i].Event)
		{
		case EVENT_TYPE::CREATEOBJECT:  //lparam은 Obj 주소, rParam은 Obj 타입
		{
			CSceneMgr::Create()->GetCurScene()->AddObject((CObject*)EventArr[i].lParam, (GROUP_TYPE)EventArr[i].rParam);
		}
		break;

		case EVENT_TYPE::DESTROYOBJECT:
		{
			((CObject*)EventArr[i].lParam)->kill();

			AboutToDie.push_back((CObject*)EventArr[i].lParam);
		}
		break;

		case EVENT_TYPE::CHANGESCENE:
			break;
		}
	}

	EventArr.clear();
}
