#pragma once
#include "CObject.h"
#include "CTexture.h"

class CMap :
    public CObject
{
public:
    CMap();
    ~CMap();

    virtual void Initialize();
    virtual void Update();
    virtual void Render(HDC _dc);
    void Load(const std::wstring& _Path);
private:
    CTexture* Texture;
};

