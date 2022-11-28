#pragma once
class CNodeMgr;

class CNode
{
public:
	CNode();
	~CNode();

	struct ConnectedNode
	{
		CNode* node;
		Move_Order HowToMove;
	};

	void Render(HDC _dc);
	void Connect(CNode* other, Move_Order how) { ConnectedNodes.push_back(ConnectedNode{ other,how }); }


private:
	doublepoint LeftTop;
	doublepoint RightBottom;

	std::vector<ConnectedNode> ConnectedNodes;

	friend CNodeMgr;
};

