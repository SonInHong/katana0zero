#include "pch.h"
#include "CObject.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CResourceMgr.h"
#include "CCollider.h"
#include "CLineCollider.h"
#include "CComponent.h"
#include "CAnimator.h"
#include "CRigidBody.h"

CObject::CObject()
	:Pos{}
	, Scale{}
	, Resize{}
	, m_Component{}
	, Alive(true)
	, CameraONOFF(true)
	, State(Object_State::END)
	,RecentFloor(nullptr)
{
	Resize.x = 1; Resize.y = 1;
}

CObject::~CObject()
{

}

void CObject::Initialize()
{


}

void CObject::Update()
{

}

void CObject::ComponentUpdate()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		for (int j = 0; j < m_Component[i].size(); ++j)
			m_Component[i][j]->Update();
	}
}

void CObject::Render(HDC _dc)
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		for (int j = 0; j < m_Component[i].size(); ++j)
			m_Component[i][j]->Render(_dc);
	}
}

void CObject::Destroy()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		for (int j = 0; j < m_Component[i].size(); ++j)
		{
			delete m_Component[i][j];
		
		}
		m_Component[i].clear();
	}
}

void CObject::Save(FILE* file)
{
}

void CObject::Load(FILE* file)
{
}

bool CObject::Collide(CObject* other)
{
	return true;
}

bool CObject::Colliding(CObject* other)
{
	return false;
}

bool CObject::CollideEscape(CObject* other)
{
	return false;
}

bool CObject::DontCollide(CObject* other)
{
	return false;
}

void CObject::CreateCollider()
{
	CCollider* p = new CCollider;
	p->Initialize(this);
	m_Component[(UINT)COMPONENT_TYPE::COLLIDER].push_back(p);
}

void CObject::CreateCollider(doublepoint p)
{
	CCollider* t = new CCollider;
	t->Initialize(this);
	t->SetOffSet(p);
	m_Component[(UINT)COMPONENT_TYPE::COLLIDER].push_back(t);
}

void CObject::CreateLineCollider(int dir)
{
	CLineCollider* p = new CLineCollider;
	p->SetDir(dir);
	p->Initialize(this);
	m_Component[(UINT)COMPONENT_TYPE::COLLIDER].push_back(p);
}

void CObject::CreateAnimator()
{
	CAnimator* p = new CAnimator;
	p->Initialize(this);
	m_Component[(UINT)COMPONENT_TYPE::ANIMATOR].push_back(p);
}

void CObject::CreateRigidBody()
{
	CRigidBody* p = new CRigidBody;
	p->Initialize(this);
	m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY].push_back(p);
}
