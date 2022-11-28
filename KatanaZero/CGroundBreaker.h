#pragma once
#include "CObject.h"
#include "CPlayer.h"

class CGroundBreaker :
    public CObject
{
public:
    CGroundBreaker();
    virtual ~CGroundBreaker();

    virtual bool CollideEscape(CObject* other);
    virtual void Initialize();
    virtual void Update();

private:
    CObject* Owner;
    float Speed;
    doublepoint OffSet;

    friend CPlayer;
};

