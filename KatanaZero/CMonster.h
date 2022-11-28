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

    //������
    Main_Order MainOrder;
    Move_Order MoveOrder;
    Action_Order ActionOrder;

    //�ι� ���� ������
    doublepoint RoamingPoint;
    double RoamingDistance;
    CFloor* RoamingFloor;

    //�þ� ���� ������
    double DetectRange;
    double DetectAngle;
    doublepoint EyeOffset; //������

    //�ǰ� ���� ������
    double HurtAngle;

    void MoveLeft();
    void MoveRight();
    void ClimbDownRight();
    void ClimbDownLeft();
    void Stay();

    std::vector<CTopFloor*> IgnoreTopFloorList;
    std::vector<CTopFloor*> RecogniseTopFloorList;
};

