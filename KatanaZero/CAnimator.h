#pragma once
#include <map>
#include "CComponent.h"

class CTexture;
class CObject;
class CAnimation;

class CAnimator :
    public CComponent
{
public:
    

    CAnimator();
    ~CAnimator();

    void CreateAnimation(const std::wstring _TextureName, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop);
    void CreateAnimation(CTexture* _Texture, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop);
    void CreateSpriteAndAnimation(const std::wstring _Path, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop);

    void CreateAnimation(const std::wstring _TextureName, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop, doublepoint _offset);
    void CreateAnimation(CTexture* _Texture, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop, doublepoint _offset);
    void CreateSpriteAndAnimation(const std::wstring _Path, const std::wstring _name, doublepoint _LeftTop, doublepoint _SpriteSize, int _FrameCount, double _FrameTime, bool _Loop, doublepoint _offset);

    CAnimation* FindAnimation(const std::wstring& _name);
    CAnimation* GetCurAnimation() { return m_pCurAnim; }
    void Play(const std::wstring& _name);
    void StartPlaying(const std::wstring& _name);
    void Reset();
    void Dont_Play_Anything() { Reset();  m_pCurAnim = nullptr; }
    void AfterImageON(); 
    void AfterImageOff(); 

    virtual void Update();
    virtual void Render(HDC _dc);








private:
    std::map<std::wstring, CAnimation*> m_mapAnim;
    CAnimation* m_pCurAnim;




};

