#pragma once
#include "CComponent.h"
class CCollider :
    public CComponent
{
public:
    CCollider();
    virtual ~CCollider();

    virtual void Initialize(CObject* p);
    virtual void Update();
    virtual void Render(HDC _dc);

    doublepoint GetAbsPos() { return AbsolutePos; }
    doublepoint GetScale() { return Scale; }

    void SetCollision(bool b) { Collision = b; }
    void SetOffSet(doublepoint b) { OffSet = b; }
    POINT& GetCollideDir() { return CollideDir; }

protected:

    bool Collision;
    doublepoint OffSet;
    doublepoint AbsolutePos;
    doublepoint Scale;
    doublepoint Resize;

    POINT CollideDir;


};
