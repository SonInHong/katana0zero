#include "pch.h"
#include "CScene_Tool.h"
#include "CCore.h"
#include "CCameraMgr.h"
#include "CTile.h"
#include "PathMgr.h"
#include "KeyMgr.h"
#include <commdlg.h>
#include "CObject.h"
#include "CTileUI.h"
#include "CEventMgr.h"
#include "CResourceMgr.h"

#define TILECOUNT 3
#define TILESIZE 36

CScene_Tool::CScene_Tool()
	:TileCount{}
	,TileSize{}
	,mem_dc{}
	,mem_bitmap{}
	, BrushTile{}
{
	
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::Enter()
{
	CCore::Create()->SetMenuBar(true);
	CCore::Create()->SetToolMenuBar(true);

	ShowWindow(CCore::Create()->GetToolWindowData().hwnd, SW_NORMAL);
	UpdateWindow(CCore::Create()->GetToolWindowData().hwnd);

	//카메라 리셋
	CCameraMgr::Create()->Reset();

	//타일 세팅
	CreateTiles(POINT{ TILECOUNT,TILECOUNT }, POINT{ TILESIZE,TILESIZE });


	//툴 윈도우 더블버퍼 세팅
	mem_dc = CreateCompatibleDC(CCore::Create()->GetToolWindowData().hdc);
	mem_bitmap = CreateCompatibleBitmap(CCore::Create()->GetToolWindowData().hdc, CCore::Create()->GetToolWindowData().width, CCore::Create()->GetToolWindowData().height);

	HBITMAP used = (HBITMAP)SelectObject(mem_dc, mem_bitmap);
	DeleteObject(used);

	//지우개 UI
	CTileUI* tileUI = new CTileUI(POINT(TILESIZE, TILESIZE));
	tileUI->setWindowData(CCore::Create()->GetToolWindowData());
	tileUI->setMemDc(mem_dc);
	
	CEventMgr::Create()->Event_CreateObj(tileUI, GROUP_TYPE::UI);
}

void CScene_Tool::Exit()
{
}

void CScene_Tool::Update()
{
	CCameraMgr::Create()->MoveCamera();

	if (KeyMgr::Create()->key(Key::L).pressed && KeyMgr::Create()->key(Key::LCTRL).pressed)
	{		
		Load();
		KeyMgr::Create()->SetModal(true);
	}

	if (KeyMgr::Create()->key(Key::S).pressed && KeyMgr::Create()->key(Key::LCTRL).pressed)
	{
		Save();
		KeyMgr::Create()->SetModal(true);


	}
		


	CScene::Update(); // 오브젝트 업데이트
	
	if (GetFocus() == CCore::Create()->GetWindowData().hwnd)
	{
		//클릭 됐을때 마우스 포지션 들고옴
		if (KeyMgr::Create()->key(Key::LEFTMOUSE).key_state == KeyState::TAP)
		{
			POINT mousePos = KeyMgr::Create()->GetMousePos();
			ScreenToClient(CCore::Create()->GetWindowData().hwnd, &mousePos);
			
			doublepoint p = CCameraMgr::Create()->RealCoordinate(doublepoint((double)mousePos.x, (double)mousePos.y));
			
			if (p.x >= 0 && p.y >= 0)
			{
				int Xindex = p.x / TileSize.x;
				int Yindex = p.y / TileSize.y;
					
					CTexture* tex = CResourceMgr::Create()->Load<CTexture>(BrushTile);

					if (tex)
					{
						int y = tex->GetHeight() / TileSize.y;
						int x = tex->GetWidth() / TileSize.x;

						if (Xindex + x <= TileCount.x && Yindex + y <= TileCount.y)
						{
							for (int i = 0; i < x; ++i)
								for (int j = 0; j < y; ++j)
								{
									int m = Xindex + i;
									int n = Yindex + j;

									int k = TileCount.x * n + m;

									CTile* tile = dynamic_cast<CTile*>(m_arrObj[(UINT)GROUP_TYPE::TILE][k]);
									if (tile)
									{
										tile->SetPath(BrushTile);
										tile->SetCuttingPoint(POINT(i * TileSize.x, j * TileSize.y));
									}


								}

						}
					}

					else
					{
						if (Xindex < TileCount.x && Yindex < TileCount.y)
						{
							int k = TileCount.x * Yindex + Xindex;
							
							CTile* tile = dynamic_cast<CTile*>(m_arrObj[(UINT)GROUP_TYPE::TILE][k]);
							if (tile)
							{
								tile->SetPath(BrushTile);
								tile->SetCuttingPoint(POINT(0, 0));
							}
						}
					}
					
							
					
						


				//}
			}
		}
	}
}

void CScene_Tool::Render(HDC _dc)
{
	Rectangle(mem_dc, -1, -1, CCore::Create()->GetToolWindowData().width + 1, CCore::Create()->GetToolWindowData().height + 1);
	
	CScene::Render(_dc);

	BitBlt(CCore::Create()->GetToolWindowData().hdc, 0, 0, CCore::Create()->GetToolWindowData().width, CCore::Create()->GetToolWindowData().height, mem_dc, 0, 0, SRCCOPY);
}

void CScene_Tool::CreateTiles(POINT count, POINT size)
{
	ClearObject(GROUP_TYPE::TILE);

	for (int y = 0; y < count.y; ++y)
	{
		for (int x = 0; x < count.x; ++x)
		{
			CTile* tile = new CTile;
			tile->Initialize();
			tile->SetPos(doublepoint{ (double)x * size.x, (double)y * size.y });
			tile->SetScale(doublepoint{ (double)size.x, (double)size.y });

			AddObject(tile, GROUP_TYPE::TILE);
		}
	}

	TileCount = count;
	TileSize = size;
}

void CScene_Tool::Save()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"Tile\0*.tile\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	std::wstring strTileFolder = PathMgr::Create()->GetContentPath();
	strTileFolder += L"\\content\\MapData";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		
	if(GetSaveFileName(&ofn))
		Save(szName);
	

	
}


void CScene_Tool::Save(const std::wstring& _Path)
{


	FILE* file = nullptr;
	_wfopen_s(&file, _Path.c_str(), L"wb");

	fwrite(&TileCount, sizeof(POINT), 1, file);
	fwrite(&TileSize, sizeof(POINT), 1, file);

	for (int i = 0; i < m_arrObj[(UINT)GROUP_TYPE::TILE].size(); ++i)
	{
		m_arrObj[(UINT)GROUP_TYPE::TILE][i]->Save(file);
	}

	fclose(file);
}

void CScene_Tool::Load()
{
	
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"Tile\0*.tile\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	std::wstring strTileFolder = PathMgr::Create()->GetContentPath();
	strTileFolder += L"\\content\\MapData";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;

	if(GetOpenFileName(&ofn))
		Load(szName);
}



void CScene_Tool::Load(const std::wstring& _Path)
{
	

	FILE* file = nullptr;
	_wfopen_s(&file, _Path.c_str(), L"rb");

	fread(&TileCount, sizeof(POINT), 1, file);
	fread(&TileSize, sizeof(POINT), 1, file);
	
	CreateTiles(TileCount, TileSize);

	for (int i = 0; i < m_arrObj[(UINT)GROUP_TYPE::TILE].size(); ++i)
	{
		m_arrObj[(UINT)GROUP_TYPE::TILE][i]->Load(file);
	}


	fclose(file);

	CCameraMgr::Create()->Reset();
}

void CScene_Tool::LoadTile()
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"BMP\0*.bmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	std::wstring strTileFolder = PathMgr::Create()->GetContentPath();
	strTileFolder += L"\\content\\texture";

	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;

	if (GetOpenFileName(&ofn))
	{
		//상대 경로 기록
		std::wstring relativePath = {};
		relativePath += szName;
		relativePath = relativePath.substr(strTileFolder.size()+1, relativePath.size() - strTileFolder.size());
		
		//UI 생성
		CTileUI* tileUI = new CTileUI(relativePath);
		tileUI->setWindowData(CCore::Create()->GetToolWindowData());
		tileUI->setMemDc(mem_dc);
		
		CEventMgr::Create()->Event_CreateObj(tileUI, GROUP_TYPE::UI);
		
	}
}
