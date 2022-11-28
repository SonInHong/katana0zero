#include "pch.h"
#include "CGrunt.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CTopFloor.h"
#include "CStair.h"
#include "CFloor.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

CGrunt::CGrunt()
{
	RoamingDistance = 300;
	DetectAngle = 10;
	DetectRange = 300;
	EyeOffset = doublepoint{ 8,-20 };
}

CGrunt::~CGrunt()
{
}

void CGrunt::Initialize()
{
	CMonster::Initialize();
	CreateAnimator();
	CreateCollider(doublepoint{ 0,10 });

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_idle\\right", L"GruntIdleRight", doublepoint{ 0,0 }, doublepoint{ 30,36 }, 8, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_idle\\left", L"GruntIdleLeft", doublepoint{ 0,0 }, doublepoint{ 30,36 }, 8, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_walk\\right", L"GruntWalkRight", doublepoint{ 0,0 }, doublepoint{ 32,40 }, 10, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_walk\\left", L"GruntWalkLeft", doublepoint{ 0,0 }, doublepoint{ 32,40 }, 10, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_run\\right", L"GruntRunRight", doublepoint{ 0,0 }, doublepoint{ 36,39 }, 10, 0.07, true);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_run\\left", L"GruntRunLeft", doublepoint{ 0,0 }, doublepoint{ 36,39 }, 10, 0.07, true);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_turn\\right", L"GruntTurnRight", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 8, 0.1, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_turn\\left", L"GruntTurnLeft", doublepoint{ 0,0 }, doublepoint{ 36,35 }, 8, 0.1, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_attack\\right", L"GruntAttackRight", doublepoint{ 0,0 }, doublepoint{ 44,42 }, 8, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_attack\\left", L"GruntAttackLeft", doublepoint{ 0,0 }, doublepoint{ 44,42 }, 8, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_hurtfly\\right", L"GruntHurtflyRight", doublepoint{ 0,0 }, doublepoint{ 43,37 }, 2, 0.07, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_hurtfly\\left", L"GruntHurtflyLeft", doublepoint{ 0,0 }, doublepoint{ 43,37 }, 2, 0.07, false);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_hurtground\\right", L"GruntHurtgroundRight", doublepoint{ 0,0 }, doublepoint{ 56,41 }, 16, 0.1, false);
	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->CreateSpriteAndAnimation(L"Enemy\\Grunt\\spr_grunt_hurtground\\left", L"GruntHurtgroundLeft", doublepoint{ 0,0 }, doublepoint{ 56,41 }, 16, 0.1, false);

	//==============================================================================================================================================================
	//이벤트 지정
	//==============================================================================================================================================================

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnRight")->m_StartEvent = std::bind(&CMonster::SetLookDirection, this, 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnLeft")->m_StartEvent = std::bind(&CMonster::SetLookDirection, this, 0);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnRight")->m_CompleteEvent = std::bind(&CMonster::SetLookDirection, this, Right);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnLeft")->m_CompleteEvent = std::bind(&CMonster::SetLookDirection, this, Left);

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GruntIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntTurnLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GruntIdleLeft");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntAttackRight")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GruntIdleRight");

	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])
		->FindAnimation(L"GruntAttackLeft")->m_CompleteEvent = std::bind(&CAnimator::StartPlaying, dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0]), L"GruntIdleLeft");


	dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleRight");
	LookDirection = Right;

	
}

void CGrunt::Update()
{
	doublepoint& velocity = dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetVelocity();
	doublepoint& force = dynamic_cast<CRigidBody*>(m_Component[(UINT)COMPONENT_TYPE::RIGIDBODY][0])->GetForce();

	bool& OnGround = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnGround();
	int& OnStair = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnStair();
	int& OnWall = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetOnWall();
	int& WallGrab = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWallGrab();
	int& Roll = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRoll();
	int& Flip = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetFlip();
	bool& Walk = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetWalk();
	bool& Run = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetRun();
	bool& AttackOnOff = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetAttack();
	double& AttackAngle = dynamic_cast<CRigidBody*>(GetComponent(COMPONENT_TYPE::RIGIDBODY)[0])->GetAttackAngle();

	CPlayer* player = dynamic_cast<CPlayer*>(CSceneMgr::Create()->GetCurScene()->GetGroupObject(GROUP_TYPE::PLAYER)[0]);
	Run = false;
	Walk = false; // !!!필수!!!

	CMonster::Update();

	if (KeyMgr::Create()->key(Key::M).pressed)
	{
		int a = 0;
	}
	
	//=======================================================================================================
	// 무브오더
	//=======================================================================================================

	switch (MoveOrder)
	{
		case Move_Order::End:
		{
			
		}
		break;

		case Move_Order::CannotDetectPlayer:
		{
			//플레이어를 찾다가 길이 없는 경우 로밍으로 돌아감
			if(MainOrder == Main_Order::PlayerDetected)
				MainOrder = Main_Order::GetToRoamingPoint;

			//지금은 바로 들어가지만, 두리번거리면서 물음표를 띄우는 액션상태 추가 후 그 액션이 끝나면 로밍으로 돌아가는 것을 추가 할 예정 
		}
		break;

		case Move_Order::MoveLeft:
		{
			if (LookDirection == Right)
				ActionOrder = Action_Order::TurnLeft;

			else
				ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunLeft : Action_Order::WalkLeft;
			
			
			
		}
		break;

		case Move_Order::MoveRight:
		{
			if (LookDirection == Left)
				ActionOrder = Action_Order::TurnRight;

			else
				ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunRight : Action_Order::WalkRight;
			
			
		}
		break;
	
		case Move_Order::ClimbDownLeft:
		{

			if (dynamic_cast<CStair*>(RecentFloor) == nullptr)
			{
				std::vector<CFloor::ConnectedNode> CN = RecentFloor->GetConnectedNodes();

				for (int i = 0; i < CN.size(); ++i)
				{
					CTopFloor* ct = dynamic_cast<CTopFloor*>(CN[i].node);

					if (ct)
					{
						ct->IgnoreMe(this);
						IgnoreTopFloorList.push_back(ct);

					}
				}

				if (Pos.x + (Scale.x*Resize.x)/2 < RecentFloor->GetPos().x - (RecentFloor->GetScale().x * RecentFloor->GetResize().x) / 2)
					OnStair = Right;
			}

			if (LookDirection == Right)
				ActionOrder = Action_Order::TurnLeft;

			else
				ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunLeft : Action_Order::WalkLeft;
			
			


		}
		break;

		case Move_Order::ClimbDownRight:
		{

			if (dynamic_cast<CStair*>(RecentFloor) == nullptr)
			{
				std::vector<CFloor::ConnectedNode> CN = RecentFloor->GetConnectedNodes();

				for (int i = 0; i < CN.size(); ++i)
				{
					CTopFloor* ct = dynamic_cast<CTopFloor*>(CN[i].node);

					if (ct)
					{
						ct->IgnoreMe(this);
						IgnoreTopFloorList.push_back(ct);

					}
				}

				if (Pos.x - (Scale.x * Resize.x) / 2 > RecentFloor->GetPos().x + (RecentFloor->GetScale().x * RecentFloor->GetResize().x) / 2)
					OnStair = Left;
			}

			if (LookDirection == Left)
				ActionOrder = Action_Order::TurnRight;

			else
				ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunRight : Action_Order::WalkRight;

			

			
		}
		break;

		case Move_Order::Stay:
		{
			
				
			// 예외처리에서 수행
			
		}
		break;

		case Move_Order::MoveToTopFloorRight:
		{
			std::vector<CFloor::ConnectedNode> CN = RecentFloor->GetConnectedNodes();
			for (int i = 0; i < CN.size(); ++i)
			{
				CTopFloor* ct = dynamic_cast<CTopFloor*>(CN[i].node);
				if (ct)
				{
					ct->RecogniseMe(this);
					RecogniseTopFloorList.push_back(ct);
				}

			}

			if (LookDirection == Left)
				ActionOrder = Action_Order::TurnRight;

			else
				ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunRight : Action_Order::WalkRight;

			
		}
		break;

		case Move_Order::MoveToTopFloorLeft:
		{
			std::vector<CFloor::ConnectedNode> CN = RecentFloor->GetConnectedNodes();
			for (int i = 0; i < CN.size(); ++i)
			{
				CTopFloor* ct = dynamic_cast<CTopFloor*>(CN[i].node);
				if (ct)
				{
					ct->RecogniseMe(this);
					RecogniseTopFloorList.push_back(ct);
				}
	
			}

			if (LookDirection == Right)
				ActionOrder = Action_Order::TurnLeft;

			else
				ActionOrder = (MainOrder == Main_Order::PlayerDetected) ? Action_Order::RunLeft : Action_Order::WalkLeft;
			
		}
		break;

	}
	//============================================================================
	//예외처리
	//============================================================================

	if (MainOrder == Main_Order::PlayerDetected && MoveOrder == Move_Order::Stay) // 스테이 플레이어 찾아가기
	{
		if (player->GetPos().x > Pos.x)
		{
			if (LookDirection == Left)
				ActionOrder = Action_Order::TurnRight;

			else
				ActionOrder = Action_Order::RunRight;

		}


		if (player->GetPos().x < Pos.x)
		{
			if (LookDirection == Right)
				ActionOrder = Action_Order::TurnLeft;

			else
				ActionOrder = Action_Order::RunLeft;

		}
	}

	if (MainOrder == Main_Order::GetToRoamingPoint && MoveOrder == Move_Order::Stay) // 스테이 로밍포인트 찾아가기
	{
		if (RoamingPoint.x > Pos.x + 50)
		{
			if (LookDirection == Left)
				ActionOrder = Action_Order::TurnRight;

			else
				ActionOrder = Action_Order::WalkRight;

		}


		if (RoamingPoint.x < Pos.x - 50)
		{
			if (LookDirection == Right)
				ActionOrder = Action_Order::TurnLeft;

			else
				ActionOrder = Action_Order::WalkLeft;

		}

		if (abs(RoamingPoint.x - Pos.x) < RoamingDistance - 1)
			MainOrder = Main_Order::RoamAround;
	}

	doublepoint diff = (player->GetPos() - Pos);              // 공격수행 조건
	if (MainOrder == Main_Order::PlayerDetected && diff.Norm() < 100 && diff.x * LookDirection >= 0)
	{
		if (LookDirection == Right)
			ActionOrder = Action_Order::AttackRight;

		else
			ActionOrder = Action_Order::AttackLeft;
	}

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntAttackRight"
		&&MainOrder != Main_Order::GetHurt)   // 피격시가 아니라면 공격은 무조건 마무리
		ActionOrder = Action_Order::AttackRight;

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntAttackLeft"
		&& MainOrder != Main_Order::GetHurt)
		ActionOrder = Action_Order::AttackLeft;

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntTurnRight"
		&& MainOrder != Main_Order::GetHurt)   // 피격시가 아니라면 턴동작은 무조건 마무리
		ActionOrder = Action_Order::TurnRight;

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntTurnLeft"
		&& MainOrder != Main_Order::GetHurt)
		ActionOrder = Action_Order::TurnLeft;
		
	if (ActionOrder == Action_Order::RunLeft && OnWall) // 벽에 붙었으면 서있기
	{
		ActionOrder = Action_Order::IdleLeft;
		force.x -= 250000;
	}

	if (ActionOrder == Action_Order::RunRight && OnWall)
	{
		ActionOrder = Action_Order::IdleRight;
		force.x += 250000;
	}
	
	if (MainOrder == Main_Order::RoamAround && ActionOrder == Action_Order::WalkRight && OnWall) // 로밍 중 벽 만나면 뒤돌기
	{
		ActionOrder = Action_Order::TurnLeft;
	}

	if (MainOrder == Main_Order::RoamAround && ActionOrder == Action_Order::WalkLeft && OnWall)
	{
		ActionOrder = Action_Order::TurnRight;
	}

	//피격 애니메이션
	
	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntHurtflyRight")
	{
		if(OnGround == true && abs(velocity.x)< 50)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtgroundRight");

		//else if(OnWall)
			//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyLeft");
	}

	if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntHurtflyLeft")
	{
		if (OnGround == true && abs(velocity.x) < 50)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtgroundLeft");

		//else if (OnWall == true)
			//dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyRIght");
	}
	
	//===========================================================================================
	// 액션 오더 수행
	//===========================================================================================

	switch (ActionOrder)
	{
	case Action_Order::End:
	{

	}
	break;

	case Action_Order::RunLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntRunLeft");
		force.x -= 250000;

		Run = true;
	}
	break;

	case Action_Order::RunRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntRunRight");
		force.x += 250000;

		Run = true;

	}
	break;

	case Action_Order::TurnLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntTurnLeft");
		
	}
	break;

	case Action_Order::TurnRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntTurnRight");
	}
	break;

	case Action_Order::IdleRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleRight");
	}
	break;

	case Action_Order::IdleLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntIdleLeft");
	}
	break;

	case Action_Order::AttackLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntAttackLeft");
	}
	break;

	case Action_Order::AttackRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntAttackRight");
	}
	break;

	case Action_Order::WalkLeft:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntWalkLeft");
		force.x -= 250000;

		Walk = true;
	}
	break;

	case Action_Order::WalkRight:
	{
		dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntWalkRight");
		force.x += 250000;

		Walk = true;
	}
	break;

	case Action_Order::HurtGround:
	{
		velocity.x = 1000 * cos(HurtAngle);
		
		OnGround = false;
		OnStair = 0;
		MainOrder = Main_Order::End;
		LookDirection = 0;

	}
	break;

	case Action_Order::HurtFly:
	{
		velocity.x = 1000 * cos(HurtAngle);
		velocity.y = 1000 * sin(HurtAngle);
		OnGround = false;
		OnStair = 0;
		MainOrder = Main_Order::End;

		if(LookDirection == Right)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyRight");

		if (LookDirection == Left)
			dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyLeft");

		else
		{
			if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntTurnLeft")
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyLeft");

			if (dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->GetCurAnimation()->GetName() == L"GruntTurnRight")
				dynamic_cast<CAnimator*>(m_Component[(UINT)COMPONENT_TYPE::ANIMATOR][0])->StartPlaying(L"GruntHurtflyRight");
		}

		LookDirection = 0;
		

	}
	break;

	}

	wchar_t _Buffer[250];
	swprintf_s(_Buffer, L"Main: %d, Move: %d, Action: %d. force.x: %f, Xspeed: %f, Yspeed: %f",
		MainOrder,MoveOrder,ActionOrder, force.x, velocity.x, velocity.y);

	std::wstring str = {};
	str += _Buffer;
		
	//SetWindowText(CCore::Create()->GetWindowData().hwnd, str.c_str());
}
