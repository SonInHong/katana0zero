#include "pch.h"
#include "KeyMgr.h"


KeyMgr::KeyMgr()
	:keyinfo{}
	,mousePos{}
	,wasInModal(false)
{
}

KeyMgr::~KeyMgr()
{
}

void KeyMgr::Initialize()
{
	for (int i = 0; i < Key::END; ++i)
	{
		keyinfo.push_back(KeyInfo{ NONE,false });
	}
}

void KeyMgr::Update()
{
	for (int i = 0; i < Key::END; ++i)
	{
		if (GetAsyncKeyState(ASCII[i]))
		{
			keyinfo[i].pressed = true;

			switch (keyinfo[i].key_state)
			{
			case TAP:
				keyinfo[i].key_state = PRESS;
				break;

			case PRESS:
				break;

			case NONE:
				keyinfo[i].key_state = TAP;
				break;

			case RELEASE:
				keyinfo[i].key_state = TAP;
				break;

			}

			continue;
		}

		if (keyinfo[i].key_state == TAP)
		{
			keyinfo[i].pressed = false;
			keyinfo[i].key_state = RELEASE;
			continue;
		}


		if (keyinfo[i].key_state == PRESS)
		{
			keyinfo[i].pressed = false;
			keyinfo[i].key_state = RELEASE;
			continue;
		}

		if (keyinfo[i].key_state == RELEASE)
		{
			keyinfo[i].pressed = false;
			keyinfo[i].key_state = NONE;
			continue;
		}

	}

	GetCursorPos(&mousePos);

	if (wasInModal == true)
	{
		Reset();
		wasInModal = false;
	}

	
	
}

void KeyMgr::Reset()
{
	for (int i = 0; i < Key::END; ++i)
	{
		keyinfo[i].pressed = false;
		keyinfo[i].key_state = NONE;
	}
}
