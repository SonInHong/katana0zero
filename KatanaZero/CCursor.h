#pragma once
#include "CObject.h"
#include "CTexture.h"
class CCursor :
    public CObject
{
public:
    CCursor();
    ~CCursor();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);
    

private:
    CTexture* Texture;
};

