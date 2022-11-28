#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "TimeMgr.h"
#include "CCameraMgr.h"
#include <functional>
#pragma comment(lib, "Msimg32.lib")

CAnimation::CAnimation()
	:m_Owner(nullptr)
	, m_Complete(false)
	, m_CurIndex(0)
	, m_FrameCount(0)
	, m_FrameTime(0)
	, m_LeftTop{}
	, m_Loop(false)
	, m_Name{}
	, m_SpriteSize{}
	, m_Texture(nullptr)
	, m_Start(true)
	, OffSet{}
	, AfterImageOnOff(false)
	,AfterImage{}
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::Update()
{
	Timer += TimeMgr::Create()->dt();

	if (m_Start == true)
	{
		m_StartEvent();
		m_Start = false;
	}


	if (Timer >= m_FrameTime)
	{
		if (m_CurIndex >= m_FrameCount - 1)
		{
			if (m_Loop == true)
			{
				m_EndEvent();
				m_CurIndex = 0;

			}


			else
			{
				m_CompleteEvent();
				m_Complete = true;

			}
		}

		else
			++m_CurIndex;

		//인덱스 체크해서 루프 애니메이션이면 인덱스 0으로 초기화 아니면 컴플리트

		Timer = 0;

		//타이머 초기화
	}
}

void CAnimation::Render(HDC _dc)
{
	doublepoint Pos = m_Owner->GetOwner()->GetPos() + OffSet;
	doublepoint Scale = doublepoint{ (double)m_SpriteSize.x, (double)m_SpriteSize.y };
	doublepoint Resize = m_Owner->GetOwner()->GetResize();

	if (m_Owner->GetOwner()->GetCameraONOFF() == true)
	{
		Pos = CCameraMgr::Create()->CameraCoordinate(Pos);
		Scale = CCameraMgr::Create()->CameraScale(Scale);
	}

	BLENDFUNCTION func = {};
	func.AlphaFormat = AC_SRC_ALPHA;
	func.BlendFlags = 0;
	func.BlendOp = AC_SRC_OVER;
	func.AlphaFormat = AC_SRC_ALPHA;
	func.SourceConstantAlpha = 30;

	BLENDFUNCTION func2 = {};
	func2.AlphaFormat = AC_SRC_ALPHA;
	func2.BlendFlags = 0;
	func2.BlendOp = AC_SRC_OVER;
	func2.AlphaFormat = 0;
	func2.SourceConstantAlpha = 2;

	//AlphaBlend(_dc, Pos.x - m_SpriteSize.x / 2, Pos.y - m_SpriteSize.y / 2, m_SpriteSize.x, m_SpriteSize.y 
		//,m_Texture->GetDC(), m_LeftTop.x + m_SpriteSize.x * m_CurIndex, m_LeftTop.y, m_SpriteSize.x, m_SpriteSize.y, func);


	TransparentBlt(_dc, Pos.x - Resize.x * Scale.x / 2 , Pos.y - Resize.y*Scale.y / 2, Resize.x*Scale.x, Resize.y*Scale.y 
		, m_Texture->GetDC(), m_LeftTop.x + m_SpriteSize.x * m_CurIndex, m_LeftTop.y, m_SpriteSize.x, m_SpriteSize.y, RGB(255, 255, 255));

	if (AfterImageOnOff == true)
	{
		for (int i = 0; i < AfterImage.size(); ++i)
		{
			doublepoint pos = AfterImage[i].Pos;
			doublepoint size = AfterImage[i].Size;
			int index = AfterImage[i].Index;


			AlphaBlend(_dc, pos.x - size.x / 2, pos.y - size.y / 2, size.x, size.y
				, m_Texture->GetDC(), m_LeftTop.x + m_SpriteSize.x * index, m_LeftTop.y, m_SpriteSize.x, m_SpriteSize.y, func);

			
		}

		ImageSave IS = { m_CurIndex, doublepoint{Pos.x,Pos.y},doublepoint{Resize.x * Scale.x, Resize.y * Scale.y} };
		AfterImage.push_back(IS);

	}
}

void CAnimation::Reset()
{
	m_Start = true;
	m_Complete = false;
	m_CurIndex = 0;
	
	AfterImage.clear();
	AfterImageOnOff = false;

}
