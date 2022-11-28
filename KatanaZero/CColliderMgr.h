#pragma once
#include <map>


class CCollider;

class CColliderMgr
{
private:
	CColliderMgr();
	~CColliderMgr();

public:
	static CColliderMgr* Create()
	{
		static CColliderMgr p;
		return &p;
	}

public:
	void Initialize();
	void Update();
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void GroupCollision(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _eLeft, CCollider* _eRight);
	void Reset();

private:
	bool m_Matrix[(UINT)GROUP_TYPE::END][(UINT)GROUP_TYPE::END];
	std::map<ULONGLONG, bool> CollisionData;
};

