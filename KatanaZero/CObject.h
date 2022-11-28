#pragma once
class CTexture;
class CComponent;
class CCollider;
class CRigidBody;
class CFloor;

class CObject
{
protected:
	doublepoint Pos;
	doublepoint Scale;
	doublepoint Resize;
	bool Alive;
	bool CameraONOFF;
	Object_State State;


	std::vector<CComponent*> m_Component[(UINT)COMPONENT_TYPE::END];
	CFloor* RecentFloor;



public:
	CObject();
	~CObject();

	doublepoint& GetPos() { return Pos; }
	doublepoint GetScale() { return Scale; }
	doublepoint GetResize() { return Resize; }
	void SetPos(doublepoint p) { Pos = p; }
	void SetScale(doublepoint p) { Scale = p; }
	void SetResize(doublepoint p) { Resize = p; }
	Object_State GetState() { return State; }
	void SetState(Object_State s) { State = s; }
	

	void kill() { Alive = false; }
	void revive() { Alive = true; }
	bool IsDead() { return !(Alive); }
	bool GetCameraONOFF() { return CameraONOFF; }
	void SetCameraONONFF(bool b) { CameraONOFF = b; }

	virtual void Initialize();
	virtual void Update();
	virtual void ComponentUpdate() final;
	virtual void Render(HDC _dc);
	virtual void Destroy();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual bool Collide(CObject* other); // true면 충돌한것이 맞다, false면 충돌하기 전 상태로 되돌린것
	virtual bool Colliding(CObject* other);
	virtual bool CollideEscape(CObject* other);
	virtual bool DontCollide(CObject* other);

	void CreateCollider();
	void CreateCollider(doublepoint p);
	void CreateLineCollider(int dir);
	void CreateAnimator();
	void CreateRigidBody();

	const std::vector<CComponent*>& GetComponent(COMPONENT_TYPE _type) { return m_Component[(UINT)_type]; }

	CFloor* GetRecentFloor() { return RecentFloor; }
	void SetRecentFloor(CFloor* p) { RecentFloor = p; }

};

