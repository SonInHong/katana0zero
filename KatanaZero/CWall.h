#pragma once
#include "CObject.h"
#include <map>

class CWall :
    public CObject
{
public:
    CWall();
    virtual ~CWall();

    virtual void Initialize();
    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);
    virtual bool CollideEscape(CObject* other);
    virtual bool DontCollide(CObject* other);

private:
    std::map<DWORD_PTR, POINT> m_CollideDir;
        
};

