#pragma once
#include "CObject.h"

class CComponent
{
public:
	CComponent();
	virtual ~CComponent();

	virtual void Initialize(CObject* p);
	virtual void Update();
	virtual void Render(HDC _dc);
	UINT GetID() { return ID; }

	CObject* GetOwner() { return Owner; }


protected:
	CObject* Owner;
	UINT ID;
	static UINT Next_ID;
};

