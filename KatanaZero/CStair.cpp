#include "pch.h"
#include "CStair.h"
#include "CRigidBody.h"
#include "CPlayer.h"
#include "CObject.h"
#include "CLineCollider.h"
#include "CCollider.h"

#define alpha 0

CStair::CStair()
	:dir(1)
{
}

CStair::~CStair()
{
}

void CStair::Initialize()
{
	CreateLineCollider(dir);
	
}

bool CStair::Collide(CObject* other)
{
	((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetCollision(true);

	if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
		return true;
	
	doublepoint OtherPosition = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetAbsPos();
	doublepoint OtherScale = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetScale();



	CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
	if (p)
	{
				
		doublepoint X, Y;
		X.x = OtherPosition.x + OtherScale.x / 2;
		X.y = OtherPosition.y + OtherScale.y / 2;

		Y.x = OtherPosition.x - OtherScale.x / 2;
		Y.y = OtherPosition.y + OtherScale.y / 2;

		if (X.y > Pos.y + Scale.y*Resize.y / 2)
		{
			//벽취급

			double dx = (OtherScale.x + Scale.x * Resize.x) / 2 - abs(Pos.x - OtherPosition.x);
			double dy = (OtherScale.y + Scale.y * Resize.y) / 2 - abs(Pos.y - OtherPosition.y);
			
			if (dx < dy || dx< 0.5)
			{
				other->GetPos().x -= dir * dx;
				p->GetVelocity().x = 0;
			}
				
			if (dy < dx || dy < 0.5)
			{
				other->GetPos().y += dy;
				p->GetVelocity().y = 0;
			}

			return false;
		}

		

		double a = ((CLineCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->LineFunctionValue(X);
		double b = ((CLineCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->LineFunctionValue(Y);
		double c = (OtherScale.y + Scale.y * Resize.y) / 2 - abs(Pos.y - OtherPosition.y);
		
		double dy = min(max(a, b), c);

		if (c == dy)
		{
			other->GetPos().y -= dy;
			p->SetOnGround(true);
			p->GetVelocity().y = 0;
		}

		else
		{
			
				other->GetPos().y -= 0.5 * dy;
				other->GetPos().x -= dir * 0.5 * dy;
				p->SetOnStair(dir);
				p->GetVelocity().y = 0;

			
			other->SetRecentFloor(this);
		}
		
		if (p->GetAttack())
		{
			//other->GetPos().y += dy;
			//other->GetPos().x -= dir * dy;

		}

		
		
		other->SetState(Object_State::ON_FLOOR);
						
		other->GetComponent(COMPONENT_TYPE::COLLIDER)[0]->Update();

		//dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnStair(dir);
		//dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnGround(false);
		
		return false;
		
	}
		
	
		
	
	
	return false;
}

bool CStair::Colliding(CObject* other)
{
	((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetCollision(true);

	if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
		return true;

	doublepoint OtherPosition = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetAbsPos();
	doublepoint OtherScale = ((CCollider*)other->GetComponent(COMPONENT_TYPE::COLLIDER)[0])->GetScale();


	CRigidBody* p = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0]);
	if (p)
	{
		
		doublepoint X, Y;
		X.x = OtherPosition.x + OtherScale.x / 2;
		X.y = OtherPosition.y + OtherScale.y / 2;

		Y.x = OtherPosition.x - OtherScale.x / 2;
		Y.y = OtherPosition.y + OtherScale.y / 2;

		if (X.y > Pos.y + Scale.y * Resize.y / 2)
		{
			//벽취급

			double dx = (OtherScale.x + Scale.x * Resize.x) / 2 - abs(Pos.x - OtherPosition.x);
			double dy = (OtherScale.y + Scale.y * Resize.y) / 2 - abs(Pos.y - OtherPosition.y);

			if (dx < dy || dx < 0.5)
			{
				other->GetPos().x -= dir * dx;
				p->GetVelocity().x = 0;
			}

			if (dy < dx || dy < 0.5)
			{
				other->GetPos().y += dy;
				p->GetVelocity().y = 0;
			}


			return false;
		}

		double a = ((CLineCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->LineFunctionValue(X);
		double b = ((CLineCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->LineFunctionValue(Y);
		double c = (OtherScale.y + Scale.y * Resize.y) / 2 - abs(Pos.y - OtherPosition.y);

		double dy = min(max(a, b),c);
									
		if (c == dy)
		{
			other->GetPos().y -= dy;
			p->GetVelocity().y = 0;
			p->SetOnGround(true);
		}

		else
		{ 
			
				other->GetPos().y -= 0.5 * dy;
				other->GetPos().x -= dir * 0.5 * dy;
				p->SetOnStair(dir);
				p->GetVelocity().y = 0;
			
			

			other->SetRecentFloor(this);
		}

		
		if (p->GetAttack())
		{
			//other->GetPos().y += dy;
			//other->GetPos().x -= dir * dy;

		}

		
		

		other->SetState(Object_State::ON_FLOOR);
		
		other->GetComponent(COMPONENT_TYPE::COLLIDER)[0]->Update();

		//dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnStair(dir);
		//dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->SetOnGround(false);

		return false;
	}
	return false;
}

bool CStair::CollideEscape(CObject* other)
{
	((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetCollision(false);
	
	if (other->GetComponent(COMPONENT_TYPE::RIGIDBODY).empty())
		return false;

	doublepoint& velocity = dynamic_cast<CRigidBody*>(other->GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetVelocity();
	
	
	return false;
}

bool CStair::DontCollide(CObject* other)
{
	return false;
}
