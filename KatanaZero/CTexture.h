#pragma once
#include "CResource.h"
#include <functional>

class CTexture :
    public CResource
{
public:
    CTexture();
    ~CTexture();

private:
    HBITMAP m_hBit;
    HDC m_dc;
    int width;
    int height;

public:
    virtual bool Load(std::wstring _strFilePath);
    HDC& GetDC() { return m_dc; }
    int& GetWidth() { return width; }
    int& GetHeight() { return height; }

    void MakeBlankTexture(int _width, int _height);

};

