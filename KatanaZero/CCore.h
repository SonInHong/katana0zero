#pragma once

class CCore
{
private:
	CCore();
	~CCore();

	WindowData A_WindowData;
	WindowData A_ToolWindowData;

	HDC mem_dc;
	HBITMAP mem_bitmap;
	HMENU mem_menu;
	HMENU mem_toolmenu;

public:
	static CCore* Create()
	{
		static CCore p;
		return &p;
	}

	void Initialize(WindowData, WindowData);
	void Update();
	void Render();
	void Destroy();

	HMENU& GetMenu() { return mem_menu; }
	void SetMenuBar(bool power);
	void SetToolMenuBar(bool power);

	WindowData GetWindowData() { return A_WindowData; }
	WindowData GetToolWindowData() { return A_ToolWindowData; }
};

