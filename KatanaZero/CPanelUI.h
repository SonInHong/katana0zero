#pragma once
#include "CUI.h"
class CPanelUI :
    public CUI
{
public:
    CPanelUI();
    virtual ~CPanelUI();

    virtual void Tap();
    virtual void Press();
    virtual void Release();
    virtual void Click();

protected:
    POINT dragStart;
    bool Tapped;
};

