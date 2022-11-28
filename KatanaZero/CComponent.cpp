#include "pch.h"
#include "CComponent.h"

UINT CComponent::Next_ID = 0;

CComponent::CComponent()
	:Owner(nullptr)
	, ID(Next_ID++)
{
}

CComponent::~CComponent()
{
}

void CComponent::Initialize(CObject* p)
{
	Owner = p;
}

void CComponent::Update()
{
}

void CComponent::Render(HDC _dc)
{
}
