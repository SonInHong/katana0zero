#pragma once
class CCameraMgr
{
private:
	CCameraMgr();
	~CCameraMgr();

public:
	static CCameraMgr* Create() { static CCameraMgr p; return &p; }

	doublepoint RealCoordinate(doublepoint CameraPos);
	doublepoint CameraCoordinate(doublepoint AbsPos);
	doublepoint CameraScale(doublepoint AbsScale);
	doublepoint RealScale(doublepoint CameraScale);

	void SetLeftTop(doublepoint _lt) { LeftTop = _lt; }
	doublepoint GetLeftTop() { return LeftTop; }
	void SetScale(doublepoint _sc) { Scale = _sc; }
	doublepoint GetScale() { return Scale; }
	void SetCenter(doublepoint _c) { LeftTop.x = _c.x - Scale.x / 2; LeftTop.y = _c.y - Scale.y / 2; }
	void Reset();
	void Update();
	void MoveCamera();

private:
	doublepoint LeftTop;
	doublepoint Scale;


};

