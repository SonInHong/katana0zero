#include "pch.h"
#include "CWall.h"
#include "CComponent.h"
#include "CRigidBody.h"
#include "CPlayer.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "KeyMgr.h"

CWall::CWall()
{
}

CWall::~CWall()
{
}

void CWall::Initialize()
{
	CreateCollider();
}

bool CWall::Collide(CObject* other)
{
	std::map<DWORD_PTR, POINT>::iterator iter = m_CollideDir.find((DWORD_PTR)other);
	if (iter == m_CollideDir.end())
	{
		m_CollideDir.insert(std::make_pair((DWORD_PTR)other, POINT{ 0,0 }));
		iter = m_CollideDir.find((DWORD_PTR)other);
	}

	((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetCollision(true);

	doublepoint OtherPosition = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetAbsPos();
	doublepoint OtherScale = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetScale();

	double dx = (OtherScale.x + Scale.x * Resize.x) / 2 - abs(Pos.x - OtherPosition.x);
	double dy = (OtherScale.y + Scale.y * Resize.y) / 2 - abs(Pos.y - OtherPosition.y);

	if (dx < dy || dx <1)
	{
		if (Pos.x > OtherPosition.x)
			iter->second.x = -1;

		else if (Pos.x < OtherPosition.x)
			iter->second.x = 1;
	}

	if (dx > dy || dy < 1)
	{
		if (Pos.y > OtherPosition.y)
			iter->second.y =1;

		else if (Pos.y < OtherPosition.y)
			iter->second.y = -1;
	}
	
	
	if (iter->second.x == 1 || iter->second.x == -1)
	{
		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
			return true;

		CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
		if (p)
		{
			//double dx = (other->GetScale().x * other->GetResize().x + Scale.x * Resize.x) / 2 - abs(Pos.x - other->GetPos().x);

			other->GetPos().x += iter->second.x * dx;
			p->GetVelocity().x = 0;

			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnWall(iter->second.x);

			
		}
						
		
	}

	if (iter->second.y == -1 || iter->second.y == 1)
	{
		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
			return true;

		CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
		if (p)
		{
			
			other->GetPos().y -= iter->second.y * dy;
			other->SetState(Object_State::ON_FLOOR);
			
			p->GetVelocity().y = 0;

			if (iter->second.y == 1)
			{
				dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnGround(true);
				dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnStair(0);
			}
		}
	}
	
	other->GetComponent(COMPONENT_TYPE::COLLIDER)[0]->Update();

	return true;
}

bool CWall::Colliding(CObject* other)
{
	std::map<DWORD_PTR, POINT>::iterator iter = m_CollideDir.find((DWORD_PTR)other);
	if (iter == m_CollideDir.end())
	{
		m_CollideDir.insert(std::make_pair((DWORD_PTR)other, POINT{ 0,0 }));
		iter = m_CollideDir.find((DWORD_PTR)other);
	}

	doublepoint OtherPosition = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetAbsPos();
	doublepoint OtherScale = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetScale();

	double dx = (OtherScale.x + Scale.x * Resize.x) / 2 - abs(Pos.x - OtherPosition.x);
	double dy = (OtherScale.y + Scale.y * Resize.y) / 2 - abs(Pos.y - OtherPosition.y);

	if (iter->second.x == 1 || iter->second.x == -1)
	{
		

		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
			return true;

		CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
		if (p)
		{
			double dx = (OtherScale.x + Scale.x * Resize.x) / 2 - abs(Pos.x - OtherPosition.x);

			other->GetPos().x += iter->second.x * dx;
			p->GetVelocity().x = 0;
			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnWall(iter->second.x);

			
		
		}
	}

	if (iter->second.y == -1 || iter->second.y == 1)
	{
		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
			return true;

		CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
		if (p)
		{			
			other->GetPos().y -= iter->second.y * dy;
			other->SetState(Object_State::ON_FLOOR);
			
			if (iter->second.y == 1)
			{
				dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnGround(true);
				dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnStair(0);
			}
			
		}
	}

	other->GetComponent(COMPONENT_TYPE::COLLIDER)[0]->Update();

	return true;
}

bool CWall::CollideEscape(CObject* other)
{
	std::map<DWORD_PTR, POINT>::iterator iter = m_CollideDir.find((DWORD_PTR)other);
	if (iter == m_CollideDir.end())
	{
		m_CollideDir.insert(std::make_pair((DWORD_PTR)other, POINT{ 0,0 }));
		iter = m_CollideDir.find((DWORD_PTR)other);
	}

	((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetCollision(false);

	if (iter->second.x == 1 || iter->second.x == -1)
	{

		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty() == false)
		{
			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnWall(0);
			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetWallGrab(0);

		}
		
		iter->second.x = 0;

	}

	if (iter->second.y == 1 || iter->second.y == -1)
	{
		if (iter->second.y == 1 && other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty() == false)
		{
			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnGround(false);
			
		}

		iter->second.y = 0;

	}

	return false;
}

bool CWall::DontCollide(CObject* other)
{
	return false;
}
