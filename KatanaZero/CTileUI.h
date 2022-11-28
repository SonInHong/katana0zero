#pragma once
#include "CPanelUI.h"
#include "CTile.h"
#include "CTexture.h"

class CTileUI :
    public CPanelUI
{
public:
    CTileUI();
    CTileUI(POINT);
    CTileUI(std::wstring _Path);
    virtual ~CTileUI();

    virtual void Render(HDC _dc);

    virtual void Tap();
    virtual void Press();
    virtual void Release();
    virtual void Click();

    void SetClicked(bool b) { clicked = b; }

private:
    std::wstring relativePath;
    CTexture* Texture;
    bool clicked;

    static int posx;
    static int posy;

    static HPEN GreenPen;
};



