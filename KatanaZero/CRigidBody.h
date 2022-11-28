#pragma once
#include "CComponent.h"
class CRigidBody :
    public CComponent
{
public:
    CRigidBody();
    virtual ~CRigidBody();

    virtual void Initialize(CObject* p);
    virtual void Update();

    doublepoint GetPrevPos() { return m_PrevPos; }

    doublepoint& GetForce() { return m_Force; }
    doublepoint& GetVelocity() { return m_Velocity; }
    
    void SetGivenV(doublepoint d) { m_GivenVelocity = d; }
    void SetGivenA(doublepoint a) { m_GivenAccel = a; }
    //void SetOnGround(bool b) { m_OnGround = b ; }
    //void SetOnStair(int b) { m_OnStair = b ; }

    bool& GetOnGround() { return m_OnGround; }
    int& GetOnStair() { return m_OnStair; }
    int& GetOnWall() { return m_OnWall; }
    int& GetWallGrab() { return m_WallGrab; }
    int& GetRoll() { return m_Roll; }
    int& GetFlip() { return m_Flip; }
    bool& GetAttack() { return m_Attack; }
    bool& GetWalk() { return m_Walk; }
    bool& GetRun() { return m_Run; }

    double& GetAttackAngle() { return m_AttackAngle; }
    
    void SetJump(bool b) { m_Jump = b; }
    void SetOnGround(bool b) { m_OnGround = b || m_OnGround; }
    void SetOnStair(int b) { m_OnStair += b; }
    void SetOnWall(int b) { m_OnWall = b; }
    void SetWallGrab(int b) { m_WallGrab = b; }
    void SetRoll(int b) { m_Roll = b; }
    void SetFlip(int b) { m_Flip = b; }
    void SetAttack(bool b) { m_Attack = b; }
     
    void CalculateFriction();
    void StartWallGrab() { if (m_Velocity.y > 0) m_Velocity.y = 0; }
private:
    double m_Mass;
    double m_MaxSpeed;
    double m_MaxXspeed;
    double m_MaxXWalkspeed;
    double m_Friction;
    
    doublepoint m_PrevPos;
    doublepoint m_Force;
    doublepoint m_Velocity;
    doublepoint m_GivenVelocity;
    doublepoint m_Accelaration;
    doublepoint m_GivenAccel;
    
    bool m_OnGround;
    int m_OnStair;
    bool m_Jump;
    int m_OnWall;
    int m_WallGrab;
    int m_Roll;
    int m_Flip;
    bool m_Attack;
    bool m_Walk;
    bool m_Run;

    double m_AttackAngle;
    double m_StopFricCff;
    double m_MoveFricCff;
    double m_DragCoff;
    double m_Area;
    double m_Volume;
    




};

