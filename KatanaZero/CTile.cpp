#include "pch.h"
#include "CTile.h"
#include "CResourceMgr.h"
#include "CCameraMgr.h"

CTile::CTile()
	:m_Path{}
	,Texture(nullptr)
	, TextureCuttingPoint{}
{
	//m_Path += L"Map\\test\\0.bmp";
}

CTile::~CTile()
{
}

void CTile::Update()
{
}

void CTile::Render(HDC _dc)
{
	if (!m_Path.empty())
		Texture = CResourceMgr::Create()->Load<CTexture>(m_Path);

	else if (m_Path.empty())
		Texture = nullptr;

	doublepoint camerapos = CCameraMgr::Create()->CameraCoordinate(Pos);

	Rectangle(_dc, camerapos.x, camerapos.y, camerapos.x + Scale.x, camerapos.y + Scale.y);

	if(Texture != nullptr)
		BitBlt(_dc, camerapos.x, camerapos.y, Scale.x, Scale.y, Texture->GetDC(), TextureCuttingPoint.x, TextureCuttingPoint.y, SRCCOPY);
}

void CTile::Save(FILE* file)
{
	size_t n = m_Path.size();
	fwrite(&Pos, sizeof(doublepoint), 1, file);
	fwrite(&Scale, sizeof(doublepoint), 1, file);
	fwrite(&n, sizeof(size_t), 1, file);
	fwrite(m_Path.c_str(), m_Path.size() * sizeof(wchar_t), 1, file);
	fwrite(&TextureCuttingPoint, sizeof(POINT), 1, file);
}

void CTile::Load(FILE* file)
{
	size_t n = 0;
	wchar_t str[255] = L"";

	fread(&Pos, sizeof(doublepoint), 1, file);
	fread(&Scale, sizeof(doublepoint), 1, file);
	fread(&n, sizeof(size_t), 1, file);
	fread(str, n*sizeof(wchar_t), 1, file);
	fread(&TextureCuttingPoint, sizeof(POINT), 1, file);
	m_Path.clear();
	m_Path += str;
	
}

void CTile::SetPath(std::wstring _path)
{
	m_Path = _path;
}
