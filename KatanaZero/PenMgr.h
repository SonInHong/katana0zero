#pragma once
class PenMgr
{
private:
	PenMgr();
	~PenMgr();

public:
	static PenMgr* Create() { static PenMgr p; return &p; }

	void Initialize();
	HPEN GetGreenPen() { return GreenPen; }
	HPEN GetRedPen() { return RedPen; }


private:
	HPEN GreenPen = 0;
	HPEN RedPen = 0;
};

