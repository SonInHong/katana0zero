#include "pch.h"
#include "PenMgr.h"

PenMgr::PenMgr()
{
}

PenMgr::~PenMgr()
{
}

void PenMgr::Initialize()
{
	GreenPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	RedPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
}
