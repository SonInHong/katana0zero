#include "pch.h"
#include "CMap.h"
#include "CCameraMgr.h"
#include "CResourceMgr.h"
#include "CCore.h"
#include "PathMgr.h"

CMap::CMap()
{
}

CMap::~CMap()
{
}

void CMap::Initialize()
{
	
}

void CMap::Update()
{
}

void CMap::Render(HDC _dc)
{

	

	doublepoint RealPos = CCameraMgr::Create()->RealCoordinate(doublepoint{0,0});
	doublepoint RealScale = CCameraMgr::Create()->RealScale(doublepoint{(double)CCore::Create()->GetWindowData().width,(double)CCore::Create()->GetWindowData().height});

	doublepoint lefttop = RealPos;
	doublepoint rightbottom = RealPos + RealScale;

	if (lefttop.x < 0)
		lefttop.x = 0;
	if (lefttop.y < 0)
		lefttop.y = 0;
	if (rightbottom.x > Scale.x)
		rightbottom.x = Scale.x;
	if (rightbottom.y > Scale.y)
		rightbottom.y = Scale.y;

	RealScale = rightbottom - lefttop;

	doublepoint CameraPos = CCameraMgr::Create()->CameraCoordinate(lefttop);
	doublepoint CameraScale = CCameraMgr::Create()->CameraScale(RealScale);

	

	//BitBlt(_dc, 0, 0, CCore::Create()->GetWindowData().width, CCore::Create()->GetWindowData().height, Texture->GetDC(), 0, 0, SRCCOPY);
	TransparentBlt(_dc, CameraPos.x, CameraPos.y, CameraScale.x, CameraScale.y, Texture->GetDC(), lefttop.x, lefttop.y, RealScale.x, RealScale.y, RGB(1, 1, 1));
		//Texture->GetWidth(), Texture->GetHeight(), RGB(1, 1, 1));

	CObject::Render(_dc);
}

void CMap::Load(const std::wstring& _Path)
{
	std::wstring AbsPath = PathMgr::Create()->GetContentPath();
	AbsPath += L"\\content\\MapData\\";
	AbsPath += _Path;

	FILE* file = nullptr;
	_wfopen_s(&file, AbsPath.c_str(), L"rb");

	POINT TileCount;
	POINT TileSize;

	fread(&TileCount, sizeof(POINT), 1, file);
	fread(&TileSize, sizeof(POINT), 1, file);

	if(Texture==nullptr)
		Texture = new CTexture;

	Texture->MakeBlankTexture(TileCount.x * TileSize.x, TileCount.y * TileSize.y);
	
 

	for (int i = 0; i < TileCount.x * TileCount.y; ++i)
	{
		size_t n = 0;
		wchar_t str[255] = L"";
		POINT TextureCuttingPoint;

		fread(&Pos, sizeof(doublepoint), 1, file);
		fread(&Scale, sizeof(doublepoint), 1, file);
		fread(&n, sizeof(size_t), 1, file);
		fread(str, n * sizeof(wchar_t), 1, file);
		fread(&TextureCuttingPoint, sizeof(POINT), 1, file);
		
		CTexture* tex = CResourceMgr::Create()->Load<CTexture>(str);

		BitBlt(Texture->GetDC(), Pos.x, Pos.y, Scale.x, Scale.y, tex->GetDC(), TextureCuttingPoint.x, TextureCuttingPoint.y, SRCCOPY);
		//m_Path.clear();
		//m_Path += str;
	}

	Pos.x = 0; Pos.y = 0;

	Scale.x = TileCount.x * TileSize.x;
	Scale.y = TileCount.y * TileSize.y;

}
