#pragma once
#include "CFloor.h"
#include <map>
class CMonster;

class CTopFloor :
    public CFloor
{
public:
    CTopFloor();
    virtual ~CTopFloor();

    virtual void Render(HDC _dc);

    virtual bool Collide(CObject* other);
    virtual bool Colliding(CObject* other);

    void ResetIgnoreList() { IgnoreList.clear(); }
    void IgnoreMe(CMonster* p) { IgnoreList.push_back(p); }
    bool IsItHere(CMonster* p)
    {
        for (int i = 0; i < IgnoreList.size(); ++i)
        {
            if (IgnoreList[i] == p)
                return true;
        }

        return false;
    }

    void ResetRecogniseList() { RecogniseList.clear(); }
    void RecogniseMe(CMonster* p) { RecogniseList.push_back(p); }
    bool AmIrecognised(CMonster* p)
    {
        for (int i = 0; i < RecogniseList.size(); ++i)
        {
            if (RecogniseList[i] == p)
                return true;
        }

        return false;
    }


private:
    std::vector<CMonster*> IgnoreList;
    std::vector<CMonster*> RecogniseList;
};

