#pragma once
#include "CUI.h"
class CButtonUI :
    public CUI
{
public:
    CButtonUI();
    ~CButtonUI();

    virtual void Tap();
    virtual void Press();
    virtual void Release();
    virtual void Click();
};

