#pragma once
#include "CObject.h"
#include "CTexture.h"
#include "CPlayer.h"
#include "define.h"

class CSword :
    public CObject
{
public:
    CSword();
    ~CSword();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);

    virtual bool Collide(CObject* other);

    void SetAngle(double a) { Angle = a; }

private:
    CTexture* Texture;
    CObject* Owner;
    float Speed;
    doublepoint OffSet;
    int Direction;

    double Angle;

    friend CPlayer;
};

