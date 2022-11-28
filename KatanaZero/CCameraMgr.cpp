#include "pch.h"
#include "CCameraMgr.h"
#include "CCore.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

CCameraMgr::CCameraMgr()
    :LeftTop{}
    , Scale{}
{
}

CCameraMgr::~CCameraMgr()
{
}

doublepoint CCameraMgr::RealCoordinate(doublepoint CameraPos)
{
    int height = CCore::Create()->GetWindowData().height;
    int width = CCore::Create()->GetWindowData().width;

    doublepoint y;

    y.x = CameraPos.x * (Scale.x / width) + LeftTop.x;
    y.y = CameraPos.y * (Scale.y / height) + LeftTop.y;

    return y;
}

doublepoint CCameraMgr::CameraCoordinate(doublepoint AbsPos)
{
    int height = CCore::Create()->GetWindowData().height;
    int width = CCore::Create()->GetWindowData().width;

    doublepoint y;

    y.x = (AbsPos.x - LeftTop.x) * (width / Scale.x);
    y.y = (AbsPos.y - LeftTop.y) * (height / Scale.y);

    return y;
}

doublepoint CCameraMgr::CameraScale(doublepoint AbsScale)
{
    int height = CCore::Create()->GetWindowData().height;
    int width = CCore::Create()->GetWindowData().width;

    doublepoint y;

    y.x = AbsScale.x * (width / Scale.x);
    y.y = AbsScale.y * (height / Scale.y);

    return y;
}

doublepoint CCameraMgr::RealScale(doublepoint CameraScale)
{
    int height = CCore::Create()->GetWindowData().height;
    int width = CCore::Create()->GetWindowData().width;

    doublepoint y;

    y.x = CameraScale.x * (Scale.x / width);
    y.y = CameraScale.y * (Scale.y / height);

    return y;
}

#define alpha 0.7

void CCameraMgr::Reset()
{
    LeftTop.x = 0;
    LeftTop.y = 0;

    Scale.y = alpha * CCore::Create()->GetWindowData().height;
    Scale.x = alpha * CCore::Create()->GetWindowData().width;
}

void CCameraMgr::Update()
{
    MoveCamera();
    
}

#define MOVESPEED 200

void CCameraMgr::MoveCamera()
{
    if (KeyMgr::Create()->key(Key::UP).pressed == true)
    {
        LeftTop.y -= MOVESPEED * TimeMgr::Create()->dt();
    }

    if (KeyMgr::Create()->key(Key::DOWN).pressed == true)
    {
        LeftTop.y += MOVESPEED * TimeMgr::Create()->dt();
    }

    if (KeyMgr::Create()->key(Key::LEFT).pressed == true)
    {
        LeftTop.x -= MOVESPEED * TimeMgr::Create()->dt();
    }

    if (KeyMgr::Create()->key(Key::RIGHT).pressed == true)
    {
        LeftTop.x += MOVESPEED * TimeMgr::Create()->dt();
    }
}
