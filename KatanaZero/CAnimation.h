#pragma once
#include "CTexture.h"
#include "CCore.h"



class CAnimator;

class CAnimation
{
public:
	struct Event
	{
		std::vector<std::function<void()>> func;

		void operator = (std::function<void()> F)
		{
			func.push_back(std::move(F));
		}

		void operator () (void)
		{
			if (!func.empty())
			{
				for (int i = 0; i < func.size(); ++i)
					func[i]();
			}
		}
	};

	struct ImageSave
	{
		int Index;
		doublepoint Pos;
		doublepoint Size;
	};

	CAnimation();
	~CAnimation();

	void Update();
	void Render(HDC _dc);
	std::wstring GetName() { return m_Name; }

	void Reset();
	int& GetCurIndex() { return m_CurIndex; }
	

	friend class CAnimator;

private:
	CAnimator* m_Owner;
	std::wstring m_Name;
	CTexture* m_Texture;
	doublepoint m_LeftTop;
	doublepoint m_SpriteSize;
	int m_FrameCount;
	int m_CurIndex;
	double m_FrameTime;
	bool m_Loop;
	bool m_Complete;
	bool m_Start;

	double Timer;

	doublepoint OffSet;

	
	
	bool AfterImageOnOff;
	std::vector<ImageSave> AfterImage;
	CTexture* AfterTexture;

	void AfterImageOn() 
	{ 
		AfterImage.clear();
		AfterImageOnOff = true; 
		
	}


	void AfterImageOff() { AfterImageOnOff = false; AfterImage.clear(); }
	


public:
	Event m_StartEvent, m_EndEvent, m_CompleteEvent;


};

