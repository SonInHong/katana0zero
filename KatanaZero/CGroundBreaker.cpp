#include "pch.h"
#include "CGroundBreaker.h"
#include "CEventMgr.h"
#include "TimeMgr.h"
#include "CTopFloor.h"
#include "CCollider.h"
#include "KeyMgr.h"

CGroundBreaker::CGroundBreaker()
	:Owner(nullptr)
	,Speed(600)
	,OffSet{}
{
}

CGroundBreaker::~CGroundBreaker()
{
}

bool CGroundBreaker::CollideEscape(CObject* other)
{
	CTopFloor* ct = dynamic_cast<CTopFloor*>(other);

	if (ct)
	{
		std::map<DWORD_PTR, POINT>& CollideDir = ct->GetCollideDir();
		std::map<DWORD_PTR, POINT>::iterator iter = CollideDir.find((DWORD_PTR)Owner);

		if (iter != CollideDir.end())
			iter->second.y = 0;

		//CEventMgr::Create()->Event_DestroyObj(this);
	}

	return false;
}

void CGroundBreaker::Initialize()
{
	CreateCollider();

	Pos.x = Owner->GetPos().x + OffSet.x;
	Pos.y = Owner->GetPos().y + OffSet.y;
	Scale.x = 30;
	Scale.y = 30;

}

void CGroundBreaker::Update()
{
	OffSet.y += Speed * TimeMgr::Create()->dt();

	Pos.x = Owner->GetPos().x;
	Pos.y = Owner->GetPos().y + OffSet.y;

	if(KeyMgr::Create()->key(Key::S).key_state == KeyState::RELEASE)
		CEventMgr::Create()->Event_DestroyObj(this);
}
