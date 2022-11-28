#include "pch.h"
#include "CResourceMgr.h"
#include "CTexture.h"
#include "PathMgr.h"
#include "CCore.h"
#include <math.h>

#include <filesystem>

CResourceMgr::CResourceMgr()
	:ResourceTree{}
{
}

CResourceMgr::~CResourceMgr()
{
}

CTexture* CResourceMgr::MakeSpriteSheet(std::wstring Path, std::wstring name)
{
	std::map<std::wstring, CResource*>::iterator iter = ResourceTree.find(name);
	if (iter != ResourceTree.end())
		return dynamic_cast<CTexture*>(iter->second);

	else
	{
		std::wstring _str = {};
		_str += PathMgr::Create()->GetContentPath();
		_str += L"\\content\\texture\\";
		_str += Path;
		
		std::filesystem::path _path(_str);

		CTexture* sprite = new CTexture;
		std::vector<CTexture*> temp = {};

		int width = 0;
		int height = 0;
		int count = 0;

		

		std::filesystem::directory_iterator iter(_path);


		//for (auto& p : std::filesystem::recursive_directory_iterator(_path))
		while(iter != std::filesystem::end(iter))
		{
			
			CTexture* _Texture = new CTexture;
			std::wstring _filepath = Path + L"\\" + iter->path().filename().wstring(); //p.path().filename().wstring();
			_Texture->Load(_filepath);
			
			temp.push_back(_Texture);
			

			width = max(_Texture->GetWidth(), width);
			height = max(_Texture->GetHeight(), height);
			++count;
			++iter;
		}

		sprite->MakeBlankTexture(width*count, height);

		
		for (int i = 0; i < count; ++i)
		{
			BitBlt(sprite->GetDC(), width * i, 0, temp[i]->GetWidth(), temp[i]->GetHeight(), temp[i]->GetDC(), 0, 0, SRCCOPY);
			delete temp[i];
		}

		ResourceTree.insert(std::make_pair(name, sprite));

		

		return sprite;
	}

	
}
