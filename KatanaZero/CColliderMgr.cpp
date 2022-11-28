#include "pch.h"
#include "CColliderMgr.h"
#include "CCollider.h"
#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include <math.h>
#include "CLineCollider.h"


CColliderMgr::CColliderMgr()
	:m_Matrix{}
	, CollisionData{}
{
}

CColliderMgr::~CColliderMgr()
{
}

void CColliderMgr::Initialize()
{
}

void CColliderMgr::Update()
{
	//현재씬을 불러와서 오브젝트들 충돌체끼리 검사.
	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (int j = 0; j <= i; ++j)
		{
			if (m_Matrix[i][j] == true)
			{
				GroupCollision((GROUP_TYPE)i, (GROUP_TYPE)j);
			}
		}
	}
}

void CColliderMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	if (_eLeft >= _eRight)
		m_Matrix[(UINT)_eLeft][(UINT)_eRight] = true;

	else if (_eLeft <= _eRight)
		m_Matrix[(UINT)_eRight][(UINT)_eLeft] = true;
}

void CColliderMgr::GroupCollision(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 왼쪽인자와 오른쪽인자에 속하는 오브젝트 벡터를 가져온다
	const std::vector<CObject*>& LeftObj = CSceneMgr::Create()->GetCurScene()->GetGroupObject(_eLeft);
	const std::vector<CObject*>& RightObj = CSceneMgr::Create()->GetCurScene()->GetGroupObject(_eRight);

	for (int i = 0; i < LeftObj.size(); ++i)
	{
		if (LeftObj[i]->GetComponent(COMPONENT_TYPE::COLLIDER).empty() == true) //비어있으면
			continue;

		for (int j = 0; j < RightObj.size(); ++j)
		{
			if (RightObj[j]->GetComponent(COMPONENT_TYPE::COLLIDER).empty() == true || LeftObj[i] == RightObj[j]) // 오른쪽 비어있거나, 왼쪽과 오른쪽이 같은 오브젝트일경우
				continue;

			const std::vector<CComponent*>& LeftColliders = LeftObj[i]->GetComponent(COMPONENT_TYPE::COLLIDER);   // 두 오브젝트의 충돌체들 불러와서 충돌검사 진행 시작
			const std::vector<CComponent*>& RightColliders = RightObj[j]->GetComponent(COMPONENT_TYPE::COLLIDER);

			for (int m = 0; m < LeftColliders.size(); ++m)
			{
				for (int n = 0; n < RightColliders.size(); ++n)
				{
					CollisionID temp;
					temp.INPUT.LEFT = LeftColliders[m]->GetID();
					temp.INPUT.RIGHT = RightColliders[n]->GetID();

					std::map<ULONGLONG, bool>::iterator iter = CollisionData.find(temp.ID);


					if (iter == CollisionData.end())
					{
						CollisionData.insert(std::make_pair(temp.ID, false));
						iter = CollisionData.find(temp.ID);
					}

					
					if (IsCollision(dynamic_cast<CCollider*>(LeftColliders[m]), dynamic_cast<CCollider*>(RightColliders[n])))
					{

						if (iter->second == false) //충돌 진입
						{
							
							LeftObj[i]->Collide(RightObj[j]);
							RightObj[j]->Collide(LeftObj[i]);
							
							iter->second = true;
							

							
						}

						else // 충돌중
						{
							LeftObj[i]->Colliding(RightObj[j]);
							RightObj[j]->Colliding(LeftObj[i]);
							
							iter->second = true;

							if (LeftObj[i]->IsDead())
							{
								RightObj[j]->CollideEscape(LeftObj[i]);

								iter->second = false;
							}

							if (RightObj[j]->IsDead())
							{
								LeftObj[i]->CollideEscape(RightObj[j]);

								iter->second = false;
							}
							
						}
					}


					else
					{

						if (iter->second == false) // 충돌x
						{
							LeftObj[i]->DontCollide(RightObj[j]);
							RightObj[j]->DontCollide(LeftObj[i]);

							iter->second = false;
						}

						else // 충돌 탈출
						{

							LeftObj[i]->CollideEscape(RightObj[j]);
							RightObj[j]->CollideEscape(LeftObj[i]);
							
							iter->second = false;
							
						}

					}


				}
			}
		}
	}
}

bool CColliderMgr::IsCollision(CCollider* _eLeft, CCollider* _eRight)
{
	if (abs(_eLeft->GetAbsPos().y - _eRight->GetAbsPos().y) < (_eLeft->GetScale().y + _eRight->GetScale().y) / 2
		&& abs(_eLeft->GetAbsPos().x - _eRight->GetAbsPos().x) < (_eLeft->GetScale().x + _eRight->GetScale().x) / 2)
	{
		CLineCollider* left = dynamic_cast<CLineCollider*>(_eLeft);
		CLineCollider* right = dynamic_cast<CLineCollider*>(_eRight);

		if (left == nullptr && right == nullptr)
			return true;

		else
		{
			std::function<bool(doublepoint)> func;

			doublepoint X, Y, Z, W;
			int  dir = 0;
			if (left)
			{
				func = std::bind(&CLineCollider::LineFunction, left,std::placeholders::_1);

				X.x = _eRight->GetAbsPos().x + _eRight->GetScale().x / 2;
				X.y = _eRight->GetAbsPos().y + _eRight->GetScale().y / 2;
				
				Y.x = _eRight->GetAbsPos().x - _eRight->GetScale().x / 2;
				Y.y = _eRight->GetAbsPos().y + _eRight->GetScale().y / 2;

				Z.x = _eRight->GetAbsPos().x + _eRight->GetScale().x / 2;
				Z.y = _eRight->GetAbsPos().y - _eRight->GetScale().y / 2;

				W.x = _eRight->GetAbsPos().x + _eRight->GetScale().x / 2;
				W.y = _eRight->GetAbsPos().y - _eRight->GetScale().y / 2;

				dir = left->GetDir();

			}

			if (right)
			{
				func = std::bind(&CLineCollider::LineFunction, right, std::placeholders::_1);

				X.x = _eLeft->GetAbsPos().x + _eLeft->GetScale().x / 2;
				X.y = _eLeft->GetAbsPos().y + _eLeft->GetScale().y / 2;

				Y.x = _eLeft->GetAbsPos().x - _eLeft->GetScale().x / 2;
				Y.y = _eLeft->GetAbsPos().y + _eLeft->GetScale().y / 2;

				Z.x = _eLeft->GetAbsPos().x + _eLeft->GetScale().x / 2;
				Z.y = _eLeft->GetAbsPos().y - _eLeft->GetScale().y / 2;

				W.x = _eLeft->GetAbsPos().x + _eLeft->GetScale().x / 2;
				W.y = _eLeft->GetAbsPos().y - _eLeft->GetScale().y / 2;

				dir = right->GetDir();
			}

			if (dir == 1)
			{
				if (func(X) == false)
					return false;

				else
					return true;

			}

			if (dir == -1)
			{
				if (func(Y) == false)
					return false;

				else
					return true;

			}

			bool x = func(X);
			bool y = func(Y);
			bool z = func(Z);
			bool w = func(W);
			if (x == y && y == z && z == w)
			{
				return false;
			}

			else
				return true;
		}
	}
		
	
	
	else
		return false;
}

void CColliderMgr::Reset()
{
	for (int i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (int j = 0; j < (UINT)GROUP_TYPE::END; ++j)
		{
			m_Matrix[i][j] = false;
		}
	}
}
