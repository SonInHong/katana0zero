#pragma once
#include "CObject.h"
#include <map>

class CNodeMgr;

class CFloor :
    public CObject
{
public:
    CFloor();
    virtual ~CFloor();

    virtual void Initialize();
    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

    std::map<DWORD_PTR, POINT>& GetCollideDir() { return m_CollideDir; }

	struct ConnectedNode
	{
		CFloor* node;
		Move_Order HowToMove;
	};

	void Connect(CFloor* other, Move_Order how) { ConnectedNodes.push_back(ConnectedNode{ other,how }); }
   std::vector<ConnectedNode> GetConnectedNodes() { return ConnectedNodes; }

protected:
    std::map<DWORD_PTR, POINT> m_CollideDir;
	std::vector<ConnectedNode> ConnectedNodes;
    
	friend CNodeMgr;
};



