#pragma once
#include "CScene.h"
class CScene_Tool :
    public CScene
{
public:
    CScene_Tool();
    ~CScene_Tool();

    virtual void Enter();
    virtual void Exit();
    virtual void Update();
    virtual void Render(HDC _dc);

    void CreateTiles(POINT count, POINT size);
    void Save();
    void Load();
    void Save(const std::wstring& _Path);
    virtual void Load(const std::wstring& _Path);

    void LoadTile();

    void SetBrushTile(std::wstring _other) { BrushTile = _other; }

private:
    POINT TileCount;
    POINT TileSize;
    HDC mem_dc;
    HBITMAP mem_bitmap;
    std::wstring BrushTile;

    
};

