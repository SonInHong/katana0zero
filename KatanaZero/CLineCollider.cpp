#include "pch.h"
#include "CLineCollider.h"
#include "CCameraMgr.h"
#include "PenMgr.h"


CLineCollider::CLineCollider()
	:A{}
	,B{}
	,dir(1)
{
}

CLineCollider::~CLineCollider()
{
}

void CLineCollider::Initialize(CObject* p)
{
	CCollider::Initialize(p);

	A.x = AbsolutePos.x + Scale.x/2;
	A.y = AbsolutePos.y - dir * Scale.y/2;

	B.x = AbsolutePos.x - Scale.x/2;
	B.y = AbsolutePos.y + dir * Scale.y/2;
}

void CLineCollider::Update()
{
	CCollider::Update();
	A.x = AbsolutePos.x + Scale.x / 2;
	A.y = AbsolutePos.y - dir * Scale.y / 2;

	B.x = AbsolutePos.x - Scale.x / 2;
	B.y = AbsolutePos.y + dir * Scale.y / 2;
}

void CLineCollider::Render(HDC _dc)
{
	if (ColliderRender)
	{
		doublepoint CA = CCameraMgr::Create()->CameraCoordinate(A);
		doublepoint CB = CCameraMgr::Create()->CameraCoordinate(B);

		HPEN oldPen;

		if (Collision)
			oldPen = (HPEN)SelectObject(_dc, PenMgr::Create()->GetRedPen());
		else
			oldPen = (HPEN)SelectObject(_dc, PenMgr::Create()->GetGreenPen());

		HBRUSH oldBrush = (HBRUSH)SelectObject(_dc, GetStockObject(NULL_BRUSH));

		MoveToEx(_dc, CA.x, CA.y, NULL);
		LineTo(_dc, CB.x, CB.y);

		SelectObject(_dc, oldBrush);
		SelectObject(_dc, oldPen);
	}
	
}
