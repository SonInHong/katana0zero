#pragma once
#include "CObject.h"
#include "CTexture.h"

class CTile : public CObject
{
public:
	CTile();
	virtual ~CTile();

	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void SetPath(std::wstring _path);
	void SetCuttingPoint(POINT point) { TextureCuttingPoint = point; }

private:
	std::wstring m_Path;
	CTexture* Texture;
	POINT TextureCuttingPoint;


};

