#pragma once
#include "CObject.h"
class CUI :
    public CObject
{
public:
    CUI();
    virtual ~CUI();

    virtual void Initialize();
    virtual void Update();
    virtual void UpdatePosition();
    virtual void Render(HDC _dc);

    virtual void Tap();
    virtual void Press();
    virtual void Release();
    virtual void Click();

    CUI* GetParent() { return m_Parent; }
    void setOffset(doublepoint _offset) { m_Offset = _offset; }
    void setParent(CUI* _parent) { m_Parent = _parent; }
    void setChild(CUI* _child) { _child->setParent(this); _child->setWindowData(m_WindowData); m_Childs.push_back(_child); }
    void setWindowData(WindowData _w) { m_WindowData = _w; }
    void setMemDc(HDC _dc) { m_dc = _dc; }

    const std::vector<CUI*>& GetChilds() { return m_Childs; }

    bool IsMouseOn();

protected:
    CUI* m_Parent;
    std::vector<CUI*> m_Childs;
    doublepoint m_Offset;
    WindowData m_WindowData;
    HDC m_dc;

};

