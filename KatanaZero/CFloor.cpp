#include "pch.h"
#include "CPlayer.h"
#include "CFloor.h"
#include "CComponent.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "KeyMgr.h"

#define alpha 0

CFloor::CFloor()
	:m_CollideDir{}
	,ConnectedNodes{}
{
}

CFloor::~CFloor()
{
}

void CFloor::Initialize()
{
	CreateCollider();
}

bool CFloor::Collide(CObject* other)
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

	if (dx > dy  || dy < 1)
	{
		if (Pos.y > OtherPosition.y)
			iter->second = POINT{ 0,1 };

		else if(Pos.y < OtherPosition.y)
			iter->second = POINT{ 0,-1 };
	}

	if (dx < dy || dx < 0.3)
	{
		//if (Pos.x > OtherPosition.x)
			//iter->second.x = -1;

		//else if (Pos.x < OtherPosition.x)
			//iter->second.x = 1;
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
			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnGround(true);
			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnStair(0);

			other->GetComponent(COMPONENT_TYPE::COLLIDER)[0]->Update();

			if (iter->second.y == 1)
				other->SetRecentFloor(this);
		}
	}

	if (iter->second.x == 1 || iter->second.x == -1)
	{
		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
			return true;

		CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
		if (p)
		{
			other->GetPos().x += iter->second.x * dx;
			p->GetVelocity().x = 0;
		}
	}
	
	return false;
}

bool CFloor::Colliding(CObject* other)
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
		

	if (iter->second.y == -1 || iter->second.y == 1)
	{
		doublepoint OtherPosition = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetAbsPos();
		doublepoint OtherScale = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetScale();

		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
			return true;

		CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
		if (p)
		{

			double dy = (OtherScale.y + Scale.y * Resize.y) / 2 - abs(Pos.y - OtherPosition.y);

			other->GetPos().y -= iter->second.y * dy;
			other->SetState(Object_State::ON_FLOOR);

			p->GetVelocity().y = 0;
			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnGround(true);
			dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnStair(0);

			other->GetComponent(COMPONENT_TYPE::COLLIDER)[0]->Update();

			if (iter->second.y == 1)
				other->SetRecentFloor(this);
		}

	}

	if (iter->second.x == 1 || iter->second.x == -1)
	{
		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
			return true;

		CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
		if (p)
		{
			other->GetPos().x += iter->second.x * dx;
			p->GetVelocity().x = 0;
		}
	}
	

	return true;
	
}

bool CFloor::CollideEscape(CObject* other)
{
	std::map<DWORD_PTR, POINT>::iterator iter = m_CollideDir.find((DWORD_PTR)other);
	if (iter == m_CollideDir.end())
	{
		m_CollideDir.insert(std::make_pair((DWORD_PTR)other, POINT{ 0,0 }));
		iter = m_CollideDir.find((DWORD_PTR)other);
	}


	((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetCollision(false);

	if (iter->second.y == -1 || iter->second.y == 1)
	{
		iter->second.y = 0;

		if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
			return false;
		dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnGround(false);

		
	}

	if (iter->second.x == 1 || iter->second.x == -1)
	{

		

		iter->second.x = 0;

	}
	

	return false;
}

bool CFloor::DontCollide(CObject* other)
{

	return false;
}
