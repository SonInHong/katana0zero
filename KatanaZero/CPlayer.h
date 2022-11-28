#pragma once
#include "CObject.h"
#include "CTexture.h"

class CFloor;

class CPlayer :
    public CObject
{
public:
    CPlayer();
    ~CPlayer();

    void GetInput();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);

    void SlashSword(double _Angle);
    void BreakGround();
    

private:
    CTexture* Texture;
    int LookDirection;
    doublepoint PrevPos;
    bool PrevOnFloor;
    bool AttackOnAir;
    long double AttackTimer;
    bool AttackTimerSwitch;
    
    


    
};

