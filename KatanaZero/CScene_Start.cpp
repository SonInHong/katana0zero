#include "pch.h"
#include "CScene_Start.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CMonster.h"
#include "CGrunt.h"
#include "CColliderMgr.h"
#include "CCameraMgr.h"
#include "CCore.h"
#include "CResourceMgr.h"
#include "CPanelUI.h"
#include "CButtonUI.h"
#include "CFloor.h"
#include "CTopFloor.h"
#include "CStair.h"
#include "CWall.h"
#include "CFloorCeiling.h"
#include "CCursor.h"
#include "CNodeMgr.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	//CCore::Create()->SetMenuBar(true);
	

	//ShowWindow(CCore::Create()->GetToolWindowData().hwnd, SW_NORMAL);
	//UpdateWindow(CCore::Create()->GetToolWindowData().hwnd);


	//노드 테스트
	

	//카메라리셋
	CCameraMgr::Create()->Reset();
		
	//Map 추가
	CMap* _pMap = new CMap;
	_pMap->Load(L"Map.tile");
	AddObject(_pMap, GROUP_TYPE::MAP);

	//커서 추가
	CCursor* _pCursor = new CCursor;
	AddObject(_pCursor, GROUP_TYPE::CURSOR);
	ShowCursor(false);

	//Player 추가
	CPlayer* _pObj = new CPlayer;

	_pObj->SetPos(doublepoint{ 100,0 });
	_pObj->SetScale(doublepoint{ 15,30 });
	_pObj->SetResize(doublepoint{ 2,2 });

	AddObject(_pObj, GROUP_TYPE::PLAYER);

	//Monster 추가
	CGrunt* _pMon = new CGrunt;
	_pMon->SetPos(doublepoint{ 600,200 });
	_pMon->SetScale(doublepoint{15,30});
	_pMon->SetResize(doublepoint{ 2,2 });
	_pMon->SetRoamingPoint(doublepoint{ 600,200 });
	AddObject(_pMon, GROUP_TYPE::MONSTER);

	_pMon = new CGrunt;
	_pMon->SetPos(doublepoint{ 600,500 });
	_pMon->SetScale(doublepoint{ 15,30 });
	_pMon->SetResize(doublepoint{ 2,2 });
	_pMon->SetRoamingPoint(doublepoint{ 600,500 });
	AddObject(_pMon, GROUP_TYPE::MONSTER);

	//Floor 추가
	
	CStair* _pStair1 = new CStair;                                 //오른쪽 큰 계단
	_pStair1->SetScale(doublepoint{ 300, 300 });
	_pStair1->SetPos(doublepoint{ 905, 438.5 });
	_pStair1->SetDir(Right);
	AddObject(_pStair1, GROUP_TYPE::FLOOR);

	CStair* _pStair2 = new CStair;                                         //왼쪽 작은 계단
	_pStair2->SetScale(doublepoint{ 60, 60 });
	_pStair2->SetPos(doublepoint{ 510, 318.5 });
	_pStair2->SetDir(Left);
	AddObject(_pStair2, GROUP_TYPE::FLOOR);

	CFloor* _pFloor1 = new CFloor;									//메인 바닥
	_pFloor1->SetScale(doublepoint{ 800, 35 });
	_pFloor1->SetPos(doublepoint{ 370, 595 });
	AddObject(_pFloor1, GROUP_TYPE::FLOOR);

	CFloor* _pFloor2 = new CFloor;											//2층계단 오른쪽 바닥
	_pFloor2->SetScale(doublepoint{ 200, 60 });
	_pFloor2->SetPos(doublepoint{ 1156, 318.5 });
	AddObject(_pFloor2, GROUP_TYPE::FLOOR);
		
	CFloor* _pFloor = new CFloor;
	_pFloor->SetScale(doublepoint{ 480, 60 });
	_pFloor->SetPos(doublepoint{ 240, 318.5 });
	//AddObject(_pFloor, GROUP_TYPE::FLOOR);

	

	CTopFloor* _pTopFloor1 = new CTopFloor;							//계단 왼쪽 바닥
	_pTopFloor1->SetScale(doublepoint{ 575, 60 });
	_pTopFloor1->SetPos(doublepoint{ 767.5, 318.5 });
	AddObject(_pTopFloor1, GROUP_TYPE::FLOOR);

	CTopFloor* _pTopFloor2 = new CTopFloor;										// 굴뚝 위 바닥
	 _pTopFloor2->SetScale(doublepoint{ 200, 35 });
	 _pTopFloor2->SetPos(doublepoint{ 75, 117 });
	AddObject(_pTopFloor2, GROUP_TYPE::FLOOR);

	CTopFloor* _pTopFloor3 = new CTopFloor;											//2층 문 뒤 바닥
	_pTopFloor3->SetScale(doublepoint{ 480, 60 });
	_pTopFloor3->SetPos(doublepoint{ 240, 318.5 });
	AddObject(_pTopFloor3, GROUP_TYPE::FLOOR);

	CFloorCeiling* _pCeiling = new CFloorCeiling;
	_pCeiling->SetScale(doublepoint{ 60, 10 });
	_pCeiling->SetPos(doublepoint{ 480, 220 });
	//AddObject(_pCeiling, GROUP_TYPE::FLOOR);


	//바닥 노드 연결 (몬스터 기준 이동방법)
	_pStair1->Connect(_pFloor1, Move_Order::MoveLeft);
	_pFloor1->Connect(_pStair1, Move_Order::MoveRight);

	_pStair1->Connect(_pFloor2, Move_Order::MoveRight);
	_pFloor2->Connect(_pStair1, Move_Order::ClimbDownLeft);

	_pFloor2->Connect(_pTopFloor1, Move_Order::MoveToTopFloorLeft);
	_pTopFloor1->Connect(_pFloor2, Move_Order::MoveRight);

	_pTopFloor1->Connect(_pTopFloor3, Move_Order::MoveToTopFloorLeft);
	_pTopFloor3->Connect(_pTopFloor1, Move_Order::MoveToTopFloorRight);

	_pTopFloor3->Connect(_pStair2, Move_Order::ClimbDownRight);
	_pStair2->Connect(_pTopFloor3, Move_Order::MoveToTopFloorLeft);

	//_pTopFloor3->Connect(_pTopFloor2, Move_Order::Stay);

	// 벽
	CWall* _pWall = new CWall;
	_pWall->SetScale(doublepoint{ 35,700 });
	_pWall->SetPos(doublepoint{ 17,300 });
	AddObject(_pWall, GROUP_TYPE::FLOOR);

	_pWall = new CWall;
	_pWall->SetScale(doublepoint{ 35,400 });
	_pWall->SetPos(doublepoint{ 157,300 });
	AddObject(_pWall, GROUP_TYPE::FLOOR);

	_pWall = new CWall;
	_pWall->SetScale(doublepoint{ 35,700 });
	_pWall->SetPos(doublepoint{ 1260,400 });
	AddObject(_pWall, GROUP_TYPE::FLOOR);

	_pWall = new CWall;
	_pWall->SetScale(doublepoint{ 60,225 });
	_pWall->SetPos(doublepoint{ 480,112.5 });
	AddObject(_pWall, GROUP_TYPE::FLOOR);

	
	

	

	//UI 추가
	/*CPanelUI* _pUI = new CPanelUI;
	CButtonUI* _cUI = new CButtonUI;
	_pUI->setOffset(doublepoint{ 700, 0 });
	_pUI->SetScale(doublepoint{ 600, 300 });
	_pUI->setWindowData(CCore::Create()->GetWindowData());
	_cUI->setOffset(doublepoint{ 0, 0 });
	_cUI->SetScale(doublepoint{ 100, 50 });

	_pUI->setChild(_cUI);

	AddObject(_pUI, GROUP_TYPE::UI);*/

	

	

	//Obj 초기화
	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->Initialize();
		}
	}

	//충돌 그룹 지정
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::MONSTER);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::FLOOR);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::FLOOR);
	CColliderMgr::Create()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::FLOOR);
}

void CScene_Start::Exit()
{
	//충돌 그룹 해제
	CColliderMgr::Create()->Reset();

	//커서 다시 보이게
	ShowCursor(true);
}

void CScene_Start::Update()
{
	CScene::Update();
}

void CScene_Start::Render(HDC _dc)
{
	CScene::Render(_dc);

	
}
