#include "pch.h"
#include "CNodeMgr.h"
#include "CFloor.h"
#include <queue>
#include "CFloor.h"

CNodeMgr::CNodeMgr()
{
}

CNodeMgr::~CNodeMgr()
{
}

void CNodeMgr::Update()
{
}

void CNodeMgr::Render(HDC _dc)
{
}

std::vector<CFloor::ConnectedNode> CNodeMgr::FindPath(CFloor* A, CFloor* B)
{
	if(A==nullptr || B==nullptr)
		return std::vector<CFloor::ConnectedNode> {};

	struct NodeAndPath
	{
		CFloor* node;
		std::vector<CFloor::ConnectedNode> path;
	};
	
	std::vector<CFloor::ConnectedNode> Path = {};
	Path.push_back(CFloor::ConnectedNode{ A,Move_Order::Start });

	//bfs search
	std::queue<NodeAndPath> Queue = {};
	Queue.push(NodeAndPath{ A, Path });

	if (A == B)
		return Path;
	

	while (!Queue.empty())
	{
		NodeAndPath CurNodeAndPath = Queue.front(); 
		Queue.pop();

		CFloor::ConnectedNode connected;

		for (int i = 0; i < CurNodeAndPath.node->ConnectedNodes.size(); ++i)
		{			
			connected = CurNodeAndPath.node->ConnectedNodes[i];

			if (connected.node == B)
			{
				//찾았다
				CurNodeAndPath.path.push_back(connected);
				return CurNodeAndPath.path;
			}
			
			bool Find = false;
			for (int j = 0; j < CurNodeAndPath.path.size(); ++j) // 지나쳤던 곳인지 확인
			{
				if (CurNodeAndPath.path[j].node == connected.node)
					Find = true;
								
			}

			if (Find == false)   // 새로운 길이면
			{
				std::vector<CFloor::ConnectedNode> newpath = CurNodeAndPath.path;
				newpath.push_back(connected);
				Queue.push(NodeAndPath{ connected.node, newpath });
			}
			
		}
	}

	return std::vector<CFloor::ConnectedNode> {};
}
