#pragma once
#include "CFloor.h"

class CFloorCeiling :
    public CFloor
{
public:
    CFloorCeiling();
    virtual ~CFloorCeiling();

    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
};

