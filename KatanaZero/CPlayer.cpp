#include "pch.h"
#include "CPlayer.h"
#include "CResourceMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CSword.h"
#include "CCameraMgr.h"
#include "CCore.h"
#include "CEventMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CGroundBreaker.h"
#include "CFloor.h"
#include "CTopFloor.h"
#include "CStair.h"

#define Right 1
#define Left -1
#define JumpVelocity -550
#define Animator0 dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
#define Animator1 dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][1])
#define AttackSpeed 0.1

CPlayer::CPlayer()
	:Texture(nullptr)
	,LookDirection(1)
	,PrevPos{}
	,PrevOnFloor(false)
	,AttackOnAir(false)
	,AttackTimer(AttackSpeed+1)
	, AttackTimerSwitch(false)
{
}

CPlayer::~CPlayer()
{

}

void CPlayer::GetInput()
{
	doublepoint& velocity = dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity();
	doublepoint& force = dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetForce();

	bool& OnGround = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnGround();
	bool& Run = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRun();
	bool& Walk = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWalk();
	int& OnStair = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnStair();
	int& OnWall = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnWall();
	int& WallGrab = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWallGrab();
	int& Roll = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRoll();
	int& Flip = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetFlip();
	bool& AttackOnOff = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetAttack();
	double& AttackAngle = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetAttackAngle();

	Run = false;
	Walk = false;


	if (OnGround)
		OnStair = 0;

	if (OnGround || OnStair)
		AttackOnAir = false;

	if (AttackOnOff == true)
	{
		WallGrab = false;
		OnGround = false;
		OnStair = 0;
		Roll = 0;
		Flip = 0;
		return;
	}

	if (AttackTimerSwitch == true)
	{
		AttackTimer += TimeMgr::Create()->dt();
		
		if (AttackTimer > AttackSpeed)
			AttackTimerSwitch = false;
	}
		

	if (KeyMgr::Create()->key(Key::A).key_state == KeyState::RELEASE)
	{
		if (Animator0->GetCurAnimation()->GetName() == L"RunLeft"
			|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunLeft")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"RunToIdleLeft");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}



	}

	if (KeyMgr::Create()->key(Key::D).key_state == KeyState::RELEASE)
	{
		if (Animator0->GetCurAnimation()->GetName() == L"RunRight"
			|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunRight")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"RunToIdleRight");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}

	}

	if (KeyMgr::Create()->key(Key::S).key_state == KeyState::RELEASE)
	{
		if (Animator0->GetCurAnimation()->GetName() == L"PreCrouchRight"
			|| Animator0->GetCurAnimation()->GetName() == L"CrouchRight")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"PostCrouchRight");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}

		if (Animator0->GetCurAnimation()->GetName() == L"PreCrouchLeft"
			|| Animator0->GetCurAnimation()->GetName() == L"CrouchLeft")
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"PostCrouchLeft");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
		}

	}


	if (KeyMgr::Create()->key(Key::A).pressed == true)
	{
		if (Animator0->GetCurAnimation()->GetName() != L"RollLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"RollRight" //롤링 중인경우 입력 받지 않는다
			&& Animator0->GetCurAnimation()->GetName() != L"FlipLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"FlipRight" //플립 중일경우 입력 받지 않는다
			&& Animator0->GetCurAnimation()->GetName() != L"WallGrabLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"WallGrabRight")
		{
			LookDirection = Left;

			if (Animator0->GetCurAnimation()->GetName() == L"JumpRight")
			{
				int i = Animator0->GetCurAnimation()->GetCurIndex();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"JumpLeft");
				Animator0->GetCurAnimation()->GetCurIndex() = i;
			}

			if (Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
				&& Animator0->GetCurAnimation()->GetName() != L"AttackRight")
			{
				force.x = -100000;
				Run = true;

				//m_Velocity.y = -m_OnStair * m_Velocity.x
			}
				

			if (OnGround == true || OnStair)
			{
				if (OnWall == 0)
				{
					if (Animator0->GetCurAnimation()->GetName() != L"IdleToRunLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"RunLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"AttackRight")
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"IdleToRunLeft");


					if (Animator0->GetCurAnimation()->GetName() == L"RunLeft")
					{
						force.x -= 250000;
						Run = true;
					}
						
				}


			}

			if (OnWall == 1)
			{
				if (OnGround == true || OnStair)
				{
					if (Animator0->GetCurAnimation()->GetName() == L"RunLeft"
						|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunLeft")
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"RunToIdleLeft");
				}

				else
				{
					if (Animator0->GetCurAnimation()->GetName() != L"WallGrabLeft")
					{
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"WallGrabLeft");
						WallGrab = 1;
					}

				}
			}
		}





	}


	if (KeyMgr::Create()->key(Key::D).pressed == true)
	{
		if (Animator0->GetCurAnimation()->GetName() != L"RollLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"RollRight"// 롤링 중인 경우 입력받지 않는다.
			&& Animator0->GetCurAnimation()->GetName() != L"FlipLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"FlipRight"//플립 중일 경우 입력받지 않는다.
			&& Animator0->GetCurAnimation()->GetName() != L"WallGrabLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"WallGrabRight")
		{
			LookDirection = Right;

			if (Animator0->GetCurAnimation()->GetName() == L"JumpLeft")
			{
				int i = Animator0->GetCurAnimation()->GetCurIndex();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"JumpRight");
				Animator0->GetCurAnimation()->GetCurIndex() = i;
			}
				

			if (Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
				&& Animator0->GetCurAnimation()->GetName() != L"AttackRight")
			{
				force.x = 100000;
				Run = true;
			}
				

			if (OnStair || OnGround == true)
			{
				if (OnWall == 0)
				{
					if (Animator0->GetCurAnimation()->GetName() != L"IdleToRunRight"
						&& Animator0->GetCurAnimation()->GetName() != L"RunRight"
						&& Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
						&& Animator0->GetCurAnimation()->GetName() != L"AttackRight")
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"IdleToRunRight");


					if (Animator0->GetCurAnimation()->GetName() == L"RunRight")
					{
						force.x += 250000;
						Run = true;
					}
						
				}


			}

			if (OnWall == -1)
			{
				if (OnGround == true || OnStair)
				{
					if (Animator0->GetCurAnimation()->GetName() == L"RunRight"
						|| Animator0->GetCurAnimation()->GetName() == L"IdleToRunRight")
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"RunToIdleRight");
				}

				else
				{
					if (Animator0->GetCurAnimation()->GetName() != L"WallGrabLeft")
					{
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"WallGrabRight");
						WallGrab = -1;
					}

				}
			}
		}




	}

	if (KeyMgr::Create()->key(Key::S).pressed == true)
	{
		if (OnGround == false && OnStair == 0)
		{
			if (Animator0->GetCurAnimation()->GetName() == L"FlipLeft"
				|| Animator0->GetCurAnimation()->GetName() == L"FlipRight")
			{
				if (LookDirection == Right)
				{
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FallRight");
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				}

				if (LookDirection == Left)
				{
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FallLeft");
					dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				}
			}

			force.y += 100000;
		}



		if (KeyMgr::Create()->key(Key::A).pressed == false  // S만 눌렸을 경우 
			&& KeyMgr::Create()->key(Key::D).pressed == false
			&& KeyMgr::Create()->key(Key::W).pressed == false)
		{
			if (OnGround == true || OnStair)
			{
				if (Animator0->GetCurAnimation()->GetName() != L"PreCrouchLeft"
					&& Animator0->GetCurAnimation()->GetName() != L"CrouchLeft"
					&& Animator0->GetCurAnimation()->GetName() != L"CrouchRight"
					&& Animator0->GetCurAnimation()->GetName() != L"PreCrouchRight"
					&& Animator0->GetCurAnimation()->GetName() != L"RollRight"
					&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
				{
					if (LookDirection == Right)
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PreCrouchRight");

					else if (LookDirection == Left)
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"PreCrouchLeft");

					BreakGround();
				}

			}
		}


		else
		{


			if ((KeyMgr::Create()->key(Key::S).key_state == KeyState::TAP
				&& KeyMgr::Create()->key(Key::D).pressed)
				||
				(KeyMgr::Create()->key(Key::D).key_state == KeyState::TAP
					&& KeyMgr::Create()->key(Key::S).pressed))

			{
				if (OnGround == true || OnStair)
				{
					if (Animator0->GetCurAnimation()->GetName() != L"RollRight"
						&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
					{
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"RollRight");
						Roll = 1;
					}
				}

			}

			if ((KeyMgr::Create()->key(Key::S).key_state == KeyState::TAP
				&& KeyMgr::Create()->key(Key::A).pressed)
				||
				(KeyMgr::Create()->key(Key::A).key_state == KeyState::TAP
					&& KeyMgr::Create()->key(Key::S).pressed))

			{
				if (OnGround == true || OnStair)
				{
					if (Animator0->GetCurAnimation()->GetName() != L"RollRight"
						&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
					{
						dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"RollLeft");
						Roll = -1;
					}
				}

			}
		}

	}

	if (KeyMgr::Create()->key(Key::W).key_state == KeyState::TAP)
	{
		if (OnGround == true || OnStair)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

			if (LookDirection == Right)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"JumpRight");

			if (LookDirection == Left)
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"JumpLeft");

			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

			//velocity.y += JumpVelocity;

			dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetGivenV(doublepoint{ 0,JumpVelocity });

			State = Object_State::JUMP;
			dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetJump(true);

			OnGround = false;
			OnStair = 0;

		}



	}


	if (KeyMgr::Create()->key(Key::LEFTMOUSE).key_state == KeyState::TAP)// && AttackTimer > AttackSpeed)
	{


		//마우스 커서 포지션 받아와서
		doublepoint mousepos = CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::CURSOR)[0]->GetPos();

		//실제좌표 계산
		mousepos = CCameraMgr::Create()->RealCoordinate(mousepos);

		//캐릭터 포지션이랑 비교해서 어택앵글 계산
		AttackAngle = atan2(mousepos.y - Pos.y, mousepos.x - Pos.x);

		//
		AttackOnOff = true;
		WallGrab = false;
		OnGround = false;
		OnStair = 0;
		Roll = 0;
		Flip = 0;


		//
		if (mousepos.x >= Pos.x)
			LookDirection = Right;

		else
			LookDirection = Left;

		
		
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

		if (LookDirection == Right)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"AttackRight");

		if (LookDirection == Left)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"AttackLeft");

		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

		velocity.x = 600 * 0.8* cos(AttackAngle);
		velocity.y = 600 * sin(AttackAngle);

		if (AttackOnAir && velocity.y < 0)
			velocity.y = 0;

		AttackOnAir = true;
		AttackTimer = 0;
		AttackTimerSwitch = true;

		SlashSword(AttackAngle);

		//
		return;
		


	}

	if (OnGround == false && OnStair == 0 && WallGrab == 0)
	{
		if (Animator0->GetCurAnimation()->GetName() != L"JumpRight"
			&& Animator0->GetCurAnimation()->GetName() != L"JumpLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"AttackLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"AttackRight"
			&& Animator0->GetCurAnimation()->GetName() != L"FlipRight"
			&& Animator0->GetCurAnimation()->GetName() != L"FlipLeft"
			&& Animator0->GetCurAnimation()->GetName() != L"RollRight"
			&& Animator0->GetCurAnimation()->GetName() != L"RollLeft"
			)
		{
			if (LookDirection == Right)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FallRight");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			}

			if (LookDirection == Left)
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FallLeft");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			}
		}

	}

	if ((OnGround == true || OnStair)) //PrevOnFloor == false && 
	{

		dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->SetJump(false);

		if (LookDirection == Right)
		{
			if (Animator0->GetCurAnimation()->GetName() == L"FallRight"
				|| Animator0->GetCurAnimation()->GetName() == L"JumpRight"
				|| Animator0->GetCurAnimation()->GetName() == L"WallGrabRight")
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandRight");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

				WallGrab = 0;
			}

		}

		if (LookDirection == Left)
		{
			if (Animator0->GetCurAnimation()->GetName() == L"FallLeft"
				|| Animator0->GetCurAnimation()->GetName() == L"JumpLeft"
				|| Animator0->GetCurAnimation()->GetName() == L"WallGrabLeft")
			{
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandLeft");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

				WallGrab = 0;
			}
		}
	}

	if (WallGrab == Right)
	{
		if (OnGround == true || OnStair)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandLeft");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

			WallGrab = 0;
		}

		else
		{
			if (KeyMgr::Create()->key(Key::W).key_state == KeyState::TAP)
			{
				LookDirection = Right;
				Flip = Right;
				WallGrab = 0;
				OnWall = 0;

				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FlipRight");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			}

if (KeyMgr::Create()->key(Key::D).pressed)
{
	if (KeyMgr::Create()->key(Key::D).key_state == KeyState::TAP)
		TimeMgr::Create()->StartStopWatch(0.05);

	if (TimeMgr::Create()->Timeover())
	{
		WallGrab = 0;

	}
}

if (KeyMgr::Create()->key(Key::D).key_state == KeyState::RELEASE)
TimeMgr::Create()->EndStopWatch();

		}
	}

	if (WallGrab == Left)
	{
		if (OnGround == true || OnStair)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"LandRight");
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

			WallGrab = 0;
		}

		else
		{
			if (KeyMgr::Create()->key(Key::W).key_state == KeyState::TAP)
			{
				LookDirection = Left;
				Flip = Left;
				WallGrab = 0;
				OnWall = 0;

				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"FlipLeft");
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();
			}

			if (KeyMgr::Create()->key(Key::A).pressed)
			{
				if (KeyMgr::Create()->key(Key::A).key_state == KeyState::TAP)
					TimeMgr::Create()->StartStopWatch(0.05);

				if (TimeMgr::Create()->Timeover())
				{

					WallGrab = 0;

				}
			}

			if (KeyMgr::Create()->key(Key::A).key_state == KeyState::RELEASE)
				TimeMgr::Create()->EndStopWatch();
		}
	}

	if (Animator0->GetCurAnimation()->GetName() != L"RollRight"
		&& Animator0->GetCurAnimation()->GetName() != L"RollLeft")
	{
		Roll = 0;
	}


	if (Animator0->GetCurAnimation()->GetName() != L"FlipRight"
		&& Animator0->GetCurAnimation()->GetName() != L"FlipLeft")
	{
		Flip = 0;
	}


	if (Animator0->GetCurAnimation()->GetName() == L"FlipRight")
	{
		if (OnWall != 0)
		{


			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"WallGrabRight");
			WallGrab = -1;
		}
	}

	if (Animator0->GetCurAnimation()->GetName() == L"FlipLeft")
	{
		if (OnWall != 0)
		{
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"WallGrabLeft");
			WallGrab = 1;
		}
	}


	

	// 공중에 떠있을 경우 recentfloor 추정
	doublepoint playerpos = {};
	double min = 100000;

	playerpos.y = ((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->GetAbsPos().y 
		+ ((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->GetScale().y / 2 - 10; // 10만큼 보정

	playerpos.x = ((CCollider*)m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->GetAbsPos().x;

	
	if (OnGround == false && OnStair == 0)
	{
		

		for (int i = 0; i<CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR).size(); ++i)
		{
			CFloor* CF = dynamic_cast<CFloor*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR)[i]);
			CStair* CS = dynamic_cast<CStair*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::FLOOR)[i]);
			
			if (CS)
			{
				double y = CS->GetPos().y + (CS->GetScale().y * CS->GetResize().y) / 2;
				double x1 = CS->GetPos().x - (CS->GetScale().x * CS->GetResize().x) / 2;
				double x2 = CS->GetPos().x + (CS->GetScale().x * CS->GetResize().x) / 2;

				if (y - playerpos.y > 0 && y - playerpos.y < min && playerpos.x > x1 && playerpos.x < x2)
					RecentFloor = CS;

			}

			else if (CF)
			{
				double y = CF->GetPos().y - (CF->GetScale().y * CF->GetResize().y) / 2;
				double x1 = CF->GetPos().x - (CF->GetScale().x * CF->GetResize().x) / 2;
				double x2 = CF->GetPos().x + (CF->GetScale().x * CF->GetResize().x) / 2;

				if (y - playerpos.y > 0 && y - playerpos.y < min && playerpos.x > x1 && playerpos.x < x2)
					RecentFloor = CF;
			}
			
		}
	}
}

void CPlayer::Initialize()
{
	//충돌체
	CreateCollider(doublepoint{ 0,7 });

	//dynamic_cast<CCollider*>(m_Component[(UINT)COMPONENT_TYPE::COLLIDER][0])->SetOffSet(doublepoint{ 0,7 });

	//애니메이터
	CreateAnimator();
	CreateAnimator();
	CreateRigidBody();

	//CResourceMgr::Create()->MakeSpriteSheet(L"Player\\spr_idle", L"PlayerIdle");
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle\\right", L"IdleRight", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 10, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle\\left", L"IdleLeft", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 10, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle_to_run\\right", L"IdleToRunRight", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 4, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_idle_to_run\\left", L"IdleToRunLeft", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 4, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_run\\right", L"RunRight", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 10, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_run\\left", L"RunLeft", doublepoint{ 0,0 }, doublepoint{ 44,32 }, 10, 0.07, true);


	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_run_to_idle\\right", L"RunToIdleRight", doublepoint{ 0,0 }, doublepoint{ 52,36 }, 5, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_run_to_idle\\left", L"RunToIdleLeft", doublepoint{ 0,0 }, doublepoint{ 52,36 }, 5, 0.07, false);
	
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_attack\\right", L"AttackRight", doublepoint{ 0,0 }, doublepoint{ 62,42 }, 7, 0.03, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_attack\\left", L"AttackLeft", doublepoint{ 0,0 }, doublepoint{ 62,42 }, 7, 0.03, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_jump\\right", L"JumpRight", doublepoint{ 0,0 }, doublepoint{ 32,42 }, 4, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_jump\\left", L"JumpLeft", doublepoint{ 0,0 }, doublepoint{ 32,42 }, 4, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_fall\\right", L"FallRight", doublepoint{ 0,0 }, doublepoint{ 42,48 }, 4, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_fall\\left", L"FallLeft", doublepoint{ 0,0 }, doublepoint{ 42,48 }, 4, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_land\\right", L"LandRight", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 5, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_land\\left", L"LandLeft", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 5, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_precrouch\\right", L"PreCrouchRight", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 2, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_precrouch\\left", L"PreCrouchLeft", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 2, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_postcrouch\\right", L"PostCrouchRight", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 2, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_postcrouch\\left", L"PostCrouchLeft", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 2, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_crouch\\right", L"CrouchRight", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 1, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_crouch\\left", L"CrouchLeft", doublepoint{ 0,0 }, doublepoint{ 36,40 }, 1, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_wallslide\\right", L"WallGrabRight", doublepoint{ 0,0 }, doublepoint{ 46,42 }, 1, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_wallslide\\left", L"WallGrabLeft", doublepoint{ 0,0 }, doublepoint{ 46,42 }, 1, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_roll\\right", L"RollRight", doublepoint{ 0,0 }, doublepoint{ 48,33 }, 7, 0.04, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_roll\\left", L"RollLeft", doublepoint{ 0,0 }, doublepoint{ 48,33 }, 7, 0.04, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_player_flip\\right", L"FlipRight", doublepoint{ 0,0 }, doublepoint{ 50,45 }, 11, 0.02, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Player\\spr_player_flip\\left", L"FlipLeft", doublepoint{ 0,0 }, doublepoint{ 50,45 }, 11, 0.02, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"IdleToRunRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"IdleToRunLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PreCrouchRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"CrouchRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PreCrouchLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"CrouchLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RunToIdleRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RunToIdleLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleLeft");

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		//->FindAnimation(L"AttackRight")->m_StartEvent = std::bind(&CPlayer::SlashSword, this, Right);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunToIdleRight");

	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		//->FindAnimation(L"AttackLeft")->m_StartEvent = std::bind(&CPlayer::SlashSword, this, Left);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunToIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackRight")->m_CompleteEvent = std::bind(&CRigidBody::SetAttack, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]),false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackLeft")->m_CompleteEvent = std::bind(&CRigidBody::SetAttack, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]),false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"JumpRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"FallRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"JumpLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"FallLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"LandRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"LandLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PostCrouchRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"PostCrouchLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"IdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunToIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollRight")->m_CompleteEvent = std::bind(&CRigidBody::SetRoll, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"RunToIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollLeft")->m_CompleteEvent = std::bind(&CRigidBody::SetRoll, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"FallLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipLeft")->m_CompleteEvent = std::bind(&CRigidBody::SetFlip, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"FallRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipRight")->m_CompleteEvent = std::bind(&CRigidBody::SetFlip, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]), 0);


	//벽에 붙으면 y축 속도를 죽인다
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"WallGrabLeft")->m_StartEvent = std::bind(&CRigidBody::StartWallGrab, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"WallGrabRight")->m_StartEvent = std::bind(&CRigidBody::StartWallGrab, dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0]));


	//잔상효과
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackRight")->m_StartEvent = std::bind(&CAnimator::AfterImageON, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"AttackLeft")->m_StartEvent = std::bind(&CAnimator::AfterImageON, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipRight")->m_StartEvent = std::bind(&CAnimator::AfterImageON, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"FlipLeft")->m_StartEvent = std::bind(&CAnimator::AfterImageON, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollRight")->m_StartEvent = std::bind(&CAnimator::AfterImageON, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]));

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"RollLeft")->m_StartEvent = std::bind(&CAnimator::AfterImageON, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]));
		
	//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateAnimation(CResourceMgr::Create()->Find<CTexture>(L"PlayerIdle"), L"Idle", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 10, 0.1, true);


	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Play(L"IdleRight");
	LookDirection = Right;
	
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->Reset();

	

	
}

void CPlayer::Update()
{	
	
	GetInput();

	//CCameraMgr::Create()->SetCenter(Pos);

	PrevPos = Pos;


	//PrevOnFloor = OnGround || OnStair;
	
}

void CPlayer::Render(HDC _dc)
{
	doublepoint CameraPos = CCameraMgr::Create()->CameraCoordinate(Pos);
	doublepoint CameraScale = CCameraMgr::Create()->CameraScale(Scale);

	CObject::Render(_dc);

	
}

void CPlayer::SlashSword(double _Angle)
{
	CSword* m = new CSword;
	m->Owner = this;
	m->SetAngle(_Angle);
	m->SetResize(doublepoint{ 1.5,1.5 });
	m->Initialize();
	

	CEventMgr::Create()->Event_CreateObj(m, GROUP_TYPE::PLAYER_PROJECTILE);
	
}

void CPlayer::BreakGround()
{
	CGroundBreaker* m = new CGroundBreaker;
	m->Owner = this;
	m->SetResize(doublepoint{ 1.5,1.5 });
	m->Initialize();

	CEventMgr::Create()->Event_CreateObj(m, GROUP_TYPE::PLAYER_PROJECTILE);
}
