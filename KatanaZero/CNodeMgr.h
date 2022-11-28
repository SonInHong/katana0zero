#pragma once
#include "CNode.h"
#include "CFloor.h"

class CFloor;

class CNodeMgr
{
public:
	CNodeMgr();
	~CNodeMgr();

	void Update(); // �÷��̾�� ������ ��ȸ�ϸ鼭 ���� ��ġ Ȯ��
	void Render(HDC _dc);
	static std::vector<CFloor::ConnectedNode> FindPath(CFloor* A, CFloor* B);
	

private:
	std::vector<CFloor*> m_Nodes;
};

