#include "pch.h"
#include "CCollider.h"
#include "CCameraMgr.h"
#include "PenMgr.h"



CCollider::CCollider()
	:OffSet{}
	, AbsolutePos{}
	, Scale{}
	,Collision(false)
	,CollideDir{}
{

}

CCollider::~CCollider()
{
}

void CCollider::Initialize(CObject* p)
{


	CComponent::Initialize(p);
	OffSet.x = 0;
	OffSet.y = 0;
	AbsolutePos.x = Owner->GetPos().x + OffSet.x;
	AbsolutePos.y = Owner->GetPos().y + OffSet.y;

	Scale.x = Owner->GetResize().x * Owner->GetScale().x;
	Scale.y = Owner->GetResize().y * Owner->GetScale().y;

	
}

void CCollider::Update()
{
	AbsolutePos.x = Owner->GetPos().x + OffSet.x;
	AbsolutePos.y = Owner->GetPos().y + OffSet.y;

	Scale.x = Owner->GetResize().x * Owner->GetScale().x;
	Scale.y = Owner->GetResize().y * Owner->GetScale().y;
}

void CCollider::Render(HDC _dc)
{
	if (ColliderRender)
	{
		doublepoint CameraPos = CCameraMgr::Create()->CameraCoordinate(AbsolutePos);
		doublepoint CameraScale = CCameraMgr::Create()->CameraScale(Scale);

		HPEN oldPen;

		if(Collision)
			oldPen = (HPEN)SelectObject(_dc, PenMgr::Create()->GetRedPen());
		else
			oldPen = (HPEN)SelectObject(_dc, PenMgr::Create()->GetGreenPen());
		
		HBRUSH oldBrush = (HBRUSH)SelectObject(_dc, GetStockObject(NULL_BRUSH));

		Rectangle(_dc, CameraPos.x - CameraScale.x / 2,	CameraPos.y - CameraScale.y / 2,CameraPos.x + CameraScale.x / 2,CameraPos.y + CameraScale.y / 2);



		SelectObject(_dc, oldBrush);
		SelectObject(_dc, oldPen);
	}
	
}