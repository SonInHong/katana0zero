#pragma once
#include "CNode.h"
#include "CFloor.h"

class CFloor;

class CNodeMgr
{
public:
	CNodeMgr();
	~CNodeMgr();

	void Update(); // 플레이어와 적들을 순회하면서 현재 위치 확인
	void Render(HDC _dc);
	static std::vector<CFloor::ConnectedNode> FindPath(CFloor* A, CFloor* B);
	

private:
	std::vector<CFloor*> m_Nodes;
};

