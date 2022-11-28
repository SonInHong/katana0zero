#include "pch.h"
#include "CRigidBody.h"
#include "TimeMgr.h"
#include "CPlayer.h"
#include <math.h>
#include "KeyMgr.h"
#include "CCore.h"
#include "CFloor.h"

CRigidBody::CRigidBody()
	:m_Mass(50)
	,m_MaxSpeed(10000)
	,m_MaxXspeed(350)
	,m_MaxXWalkspeed(150)
	,m_Friction(0)
	,m_PrevPos{}
	,m_Force{}
	,m_Velocity{}
	,m_GivenVelocity{}
	,m_Accelaration{}	
	,m_OnGround(false)
	,m_OnStair(0)
	,m_GivenAccel{}
	,m_DragCoff(0.1)
	,m_StopFricCff(10)
	,m_MoveFricCff(1500)
	,m_Area(1)
	,m_WallGrab(0)
	,m_Roll(0)
	,m_Flip(0)
	,m_Walk(false)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Initialize(CObject* p)
{
	Owner = p;
	m_PrevPos = p->GetPos();
}

#define Gravity_Accelaration 1500;

void CRigidBody::Update()
{
	
	if (KeyMgr::Create()->key(Key::W).key_state == KeyState::TAP)
	{
		int a = 0;
	}

	if (KeyMgr::Create()->key(Key::M).pressed)
	{
		int a = 0; //디버깅용
	}

	

	m_PrevPos = Owner->GetPos();
		
	//마찰 계산
	CalculateFriction();

	if (m_Velocity.x > 0)
		m_Friction *= -1;

	if (m_Velocity.x < 0)
		m_Friction *= 1;
		
	m_Force.x += m_Friction;

	// 벽을 잡고있을경우 y방향으로 마찰력
	if (m_WallGrab != 0)
	{
		if (m_Velocity.y < 0)
			m_Force.y -= 40000;
		else
			m_Force.y -= 65000;
	}

		

	// F = ma
	
	m_Accelaration.x = m_GivenAccel.x + m_Force.x / m_Mass;
	m_Accelaration.y = m_GivenAccel.y + m_Force.y / m_Mass + Gravity_Accelaration;

	if (m_Attack)
	{
		m_Accelaration.x = -0.9*0.8*cos(m_AttackAngle) * Gravity_Accelaration;
		m_Accelaration.y = -0.9*sin(m_AttackAngle) * Gravity_Accelaration;

		if (m_Accelaration.x * m_Velocity.x >= 0)
			m_Accelaration.x = 0;

		if (m_Accelaration.y * m_Velocity.y >= 0)
			m_Accelaration.y = 0;

	}
	
	

	// x방향 속도 계산
	m_Velocity.x = m_Velocity.x + m_Accelaration.x * TimeMgr::Create()->dt();
	m_Velocity.x += m_GivenVelocity.x;

	if (m_Attack == false)
	{
		if (m_Run && abs(m_Velocity.x) > m_MaxXspeed)
			m_Velocity.x = m_Velocity.x / abs(m_Velocity.x) * m_MaxXspeed;

		if(m_Walk && abs(m_Velocity.x) > m_MaxXWalkspeed)
			m_Velocity.x = m_Velocity.x / abs(m_Velocity.x) * m_MaxXWalkspeed;
	}
	
	if (abs(m_Velocity.x) < 3)
		m_Velocity.x = 0;

	

	if (m_Roll == 1) // 구르기
		m_Velocity.x = 500;

	if(m_Roll == -1)
		m_Velocity.x = -500;

	// y방향 속도 계산
	//if ( m_OnGround||m_OnStair)
		//m_Velocity.y = 0; //땅에 닿고 있으면 0으로

		
	m_Velocity.y = m_Velocity.y + m_Accelaration.y * TimeMgr::Create()->dt(); // 중력 적용
	m_Velocity.y += m_GivenVelocity.y;
	
	if (m_Velocity.Norm() > m_MaxSpeed)
		m_Velocity = m_Velocity / m_Velocity.Norm() * m_MaxSpeed;
		
	//Filp일 경우
	
		if (m_Flip == 1)
		{
			m_Velocity.x = 500;
			m_Velocity.y = -250;
		}

		if (m_Flip == -1)
		{
			m_Velocity.x = -500;
			m_Velocity.y = -250;
		}

	doublepoint m_NextPos;

	if(m_OnStair)// && (m_Walk ||m_Run) )
	{
		m_NextPos.x = m_PrevPos.x + ( (1/sqrt(2)) * m_Velocity.x + m_OnStair * (1 / sqrt(2)) * m_Velocity.y) * TimeMgr::Create()->dt();
		m_NextPos.y = m_PrevPos.y + (- m_OnStair*(1 / sqrt(2)) * m_Velocity.x + (1 / sqrt(2)) * m_Velocity.y) * TimeMgr::Create()->dt();
	}

	else
		m_NextPos = m_PrevPos + m_Velocity * TimeMgr::Create()->dt();

	//벽을 잡고있을 경우 몸을 계속 벽쪽으로 끌어당겨준다. 숫자: 탈출속도
	#define EscapeVelocity 10
	if (m_WallGrab == 1)
		m_NextPos.x -= EscapeVelocity * TimeMgr::Create()->dt();

	if (m_WallGrab == -1)
		m_NextPos.x += EscapeVelocity * TimeMgr::Create()->dt();


	

	Owner->SetPos(m_NextPos);

	doublepoint RFpos = { 0,0 };
	
	if (Owner->GetRecentFloor())
	{
		RFpos = Owner->GetRecentFloor()->GetPos();
	}
	
	//디버깅용
	wchar_t _Buffer[250];
	swprintf_s(_Buffer,  L"RecentFloor: (%f, %f), DeltaTime: %f, Velocity.x: %f, Velocity.y: %f, OnGround: %d, OnStair: %d, OnJump: %d, OnWall: %d, WallGrab: %d, Roll: %d, Flip:%d. Attack:%d",
		RFpos.x, RFpos.y, TimeMgr::Create()->dt(), m_Velocity.x, m_Velocity.y, m_OnGround, m_OnStair, m_Jump, m_OnWall, m_WallGrab, m_Roll, m_Flip, m_Attack);

	std::wstring str = {};
	str += _Buffer;

	if(dynamic_cast<CPlayer*>(Owner) != nullptr)
		SetWindowText(CCore::Create()->GetWindowData().hwnd, str.c_str());

	//힘 초기화
	m_Force = doublepoint{ 0,0 };

	//
	m_GivenVelocity.x = 0; m_GivenVelocity.y = 0;

	//
	m_GivenAccel.x = 0; m_GivenAccel.y = 0;
	
	m_Accelaration.x = 0; m_Accelaration.y = 0;
	//
	m_OnGround = false;
	m_OnStair = 0;
	
	

}

void CRigidBody::CalculateFriction()
{
	m_Friction = 0;

	//정지마찰
	
	//운동마찰
	if (m_OnGround == true || m_OnStair)
	{
		if (abs(m_Velocity.x) > 3)
		{
			double MoveFr = m_MoveFricCff * m_Mass;
			m_Friction += MoveFr;
		}
		
	}
	
	//공기마찰
	double AirFr = 0.5 * m_DragCoff * pow(m_Velocity.x,2) * m_Area;
	m_Friction += AirFr;
	//벽마찰

}


