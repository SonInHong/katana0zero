#pragma once
#include <map>
#include "CResource.h"

class CTexture;

class CResourceMgr
{
private:
	CResourceMgr();
	~CResourceMgr();

	std::map<std::wstring, CResource*> ResourceTree;

public:
	static CResourceMgr* Create() { static CResourceMgr p; return &p; }

	template <typename T>
	T* Find(std::wstring Path)
	{

		std::map<std::wstring, CResource*>::iterator iter = ResourceTree.find(Path);

		if (iter == ResourceTree.end())
			return nullptr;

		else
			return dynamic_cast<T*>(iter->second);

	}

	template <typename T>
	T* Load(std::wstring Path)
	{
		T* p = Find<T>(Path);
		if (p != nullptr)
			return p;

		p = new T;
		if (p->Load(Path))
		{
			ResourceTree.insert(std::make_pair(Path, p));

			return dynamic_cast<T*>(p);
		}
		
		else
		{
			delete p;
			return nullptr;
		}

	}

	CTexture* MakeSpriteSheet(std::wstring Path, std::wstring name);


};

