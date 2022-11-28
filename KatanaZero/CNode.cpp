#include "pch.h"
#include "CNode.h"

CNode::CNode()
	:LeftTop{}
	,RightBottom{}
	,ConnectedNodes{}
{
}

CNode::~CNode()
{
}

void CNode::Render(HDC _dc)
{
	Rectangle(_dc, LeftTop.x, LeftTop.y, RightBottom.x, RightBottom.y);
}
