#pragma once
#include "CMonster.h"
class CGrunt :
    public CMonster
{
public:
    CGrunt();
    ~CGrunt();

    virtual void Initialize();
    virtual void Update();


};

