#pragma once
#include "CCollider.h"
class CLineCollider :
    public CCollider
{
public:
    CLineCollider();
    ~CLineCollider();

    virtual void Initialize(CObject* p);
    virtual void Update();
    virtual void Render(HDC _dc);

    doublepoint GetA() { return A; }
    doublepoint GetB() { return B; }
    int GetDir() { return dir; }

    bool LineFunction(doublepoint x) { return x.y > ((B.y - A.y) / (B.x - A.x)) * (x.x - A.x) + A.y; }
    double LineFunctionValue(doublepoint x) {
        return x.y - (((B.y - A.y) / (B.x - A.x)) * (x.x - A.x) + A.y);
        int a = 0;
    }
    double GetDeltaY(double x) { return ((B.y - A.y) / (B.x - A.x)) * x;  }
    void SetDir(int d) { dir = d; }


private:
    doublepoint A;
    doublepoint B;
    int dir;
};

