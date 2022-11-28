#pragma once
#include "CFloor.h"
class CStair :
    public CFloor
{
public:
    CStair();
    virtual ~CStair();

    virtual void Initialize();

    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

    void SetDir(int d) { dir = d; }

private:
    int dir;

};

