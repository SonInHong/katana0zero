#include "pch.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CObject.h"
#include "CTexture.h"
#include "CResourceMgr.h"

CAnimator::CAnimator()
	:m_mapAnim{}
	, m_pCurAnim(nullptr)
{
}

CAnimator::~CAnimator()
{
}

void CAnimator::CreateAnimation(const std::wstring _TextureName, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop)
{

	CAnimation* p = new CAnimation;

	if (m_mapAnim.find(_name) != m_mapAnim.end()) // 이미 있으면 함수종료
		return;

	p->m_Texture = CResourceMgr::Create()->Load<CTexture>(_TextureName); // 원래 텍스쳐 로딩 안되는 경우 예외처리 필요하나 생략
	p->m_Complete = false;
	p->m_CurIndex = 0;
	p->m_FrameCount = _FrameCount;
	p->m_FrameTime = _FrameTime;
	p->m_LeftTop = _LeftTop;
	p->m_Name = _name;
	p->m_SpriteSize = _SpriteSize;
	p->m_Loop = _Loop;
	p->m_Owner = this;

	m_mapAnim.insert(std::make_pair(p->m_Name, p));

}

void CAnimator::CreateAnimation(CTexture* _Texture, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop)
{
	CAnimation* p = new CAnimation;

	if (m_mapAnim.find(_name) != m_mapAnim.end()) // 이미 있으면 함수종료
		return;

	p->m_Texture = _Texture;
	p->m_Complete = false;
	p->m_CurIndex = 0;
	p->m_FrameCount = _FrameCount;
	p->m_FrameTime = _FrameTime;
	p->m_LeftTop = _LeftTop;
	p->m_Name = _name;
	p->m_SpriteSize = _SpriteSize;
	p->m_Loop = _Loop;
	p->m_Owner = this;
	
	m_mapAnim.insert(std::make_pair(p->m_Name, p));
}

void CAnimator::CreateSpriteAndAnimation(const std::wstring _Path, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop)
{
	CTexture* sprite = CResourceMgr::Create()->MakeSpriteSheet(_Path, _name);
	CreateAnimation(sprite, _name, _LeftTop, _SpriteSize, _FrameCount, _FrameTime, _Loop);

}

void CAnimator::CreateAnimation(const std::wstring _TextureName, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop, doublepoint _offset)
{
	CAnimation* p = new CAnimation;

	if (m_mapAnim.find(_name) != m_mapAnim.end()) // 이미 있으면 함수종료
		return;

	p->m_Texture = CResourceMgr::Create()->Load<CTexture>(_TextureName); // 원래 텍스쳐 로딩 안되는 경우 예외처리 필요하나 생략
	p->m_Complete = false;
	p->m_CurIndex = 0;
	p->m_FrameCount = _FrameCount;
	p->m_FrameTime = _FrameTime;
	p->m_LeftTop = _LeftTop;
	p->m_Name = _name;
	p->m_SpriteSize = _SpriteSize;
	p->m_Loop = _Loop;
	p->m_Owner = this;
	p->OffSet = _offset;

	m_mapAnim.insert(std::make_pair(p->m_Name, p));
}

void CAnimator::CreateAnimation(CTexture* _Texture, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop, doublepoint _offset)
{
	CAnimation* p = new CAnimation;

	if (m_mapAnim.find(_name) != m_mapAnim.end()) // 이미 있으면 함수종료
		return;

	p->m_Texture = _Texture;
	p->m_Complete = false;
	p->m_CurIndex = 0;
	p->m_FrameCount = _FrameCount;
	p->m_FrameTime = _FrameTime;
	p->m_LeftTop = _LeftTop;
	p->m_Name = _name;
	p->m_SpriteSize = _SpriteSize;
	p->m_Loop = _Loop;
	p->m_Owner = this;
	p->OffSet = _offset;

	m_mapAnim.insert(std::make_pair(p->m_Name, p));
}

void CAnimator::CreateSpriteAndAnimation(const std::wstring _Path, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop, doublepoint _offset)
{
	CTexture* sprite = CResourceMgr::Create()->MakeSpriteSheet(_Path, _name);
	CreateAnimation(sprite, _name, _LeftTop, _SpriteSize, _FrameCount, _FrameTime, _Loop, _offset);
}

CAnimation* CAnimator::FindAnimation(const std::wstring& _name)
{
	std::map<std::wstring, CAnimation*>::iterator iter = m_mapAnim.find(_name);
	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator::Play(const std::wstring& _name)
{
	

	CAnimation* p = FindAnimation(_name);

	if (p == nullptr)
		return;

	m_pCurAnim = p;


}

void CAnimator::StartPlaying(const std::wstring& _name)
{
	
	CAnimation* p = FindAnimation(_name);

	if (p == nullptr)
		return;

	if (p != m_pCurAnim)
	{
		if (m_pCurAnim != nullptr)
			m_pCurAnim->Reset();

		p->Reset();
	}
		

	m_pCurAnim = p;
}

void CAnimator::Reset()
{
	if(m_pCurAnim)
		m_pCurAnim->Reset();
}

void CAnimator::AfterImageON()
{
	 m_pCurAnim->AfterImageOn();
}

void CAnimator::AfterImageOff()
{
	m_pCurAnim->AfterImageOff();
}

void CAnimator::Update()
{

	if (m_pCurAnim != nullptr)
	{
		m_pCurAnim->Update();

	}


}

void CAnimator::Render(HDC _dc)
{
	if (m_pCurAnim != nullptr)
	{
		m_pCurAnim->Render(_dc);

	}
}
