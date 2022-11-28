#pragma once
#include "CObject.h"
#include "CTexture.h"

class CTopFloor;
class CFloor;
class CSword;

class CMonster :
    public CObject
{
public:
    CMonster();
    ~CMonster();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);

    void SetLookDirection(int b) { LookDirection = b; }
    void AdjustRoamingFloor(doublepoint p);
    void SetRoamingPoint(doublepoint p) { RoamingPoint = p; }
    void SetHurtAngle(double a) { HurtAngle = a; }

    friend CSword;

protected:
    CTexture* Texture;
    int LookDirection;
    doublepoint PrevPos;
    bool PrevOnFloor;
    bool AttackOnAir;
    long double AttackTimer;
    bool AttackTimerSwitch;

    //오더들
    Main_Order MainOrder;
    Move_Order MoveOrder;
    Action_Order ActionOrder;

    //로밍 관련 변수들
    doublepoint RoamingPoint;
    double RoamingDistance;
    CFloor* RoamingFloor;

    //시야 관련 변수들
    double DetectRange;
    double DetectAngle;
    doublepoint EyeOffset; //눈높이

    //피격 관련 변수들
    double HurtAngle;

    void MoveLeft();
    void MoveRight();
    void ClimbDownRight();
    void ClimbDownLeft();
    void Stay();

    std::vector<CTopFloor*> IgnoreTopFloorList;
    std::vector<CTopFloor*> RecogniseTopFloorList;
};

