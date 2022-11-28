#include "pch.h"
#include "TimeMgr.h"
#include "CCore.h"

TimeMgr::TimeMgr()
	:m_PrevCount{}
	, m_CurCount{}
	, m_Frequency{}
	, delta_time(0)
	, m_FPS(0)
	, count(0)
	, TimeSum(0)
	,m_StopWatch{0,false,false}
{

}

TimeMgr::~TimeMgr()
{
}

void TimeMgr::Initialize()
{
	QueryPerformanceCounter(&m_PrevCount);

	QueryPerformanceFrequency(&m_Frequency);

	count = 0;
	TimeSum = 0;
}

void TimeMgr::Update()
{
	QueryPerformanceCounter(&m_CurCount);

	delta_time = ((double)m_CurCount.QuadPart - (double)m_PrevCount.QuadPart) / (double)m_Frequency.QuadPart;

	m_PrevCount = m_CurCount;

	TimeSum += delta_time;
	++count;

	if (TimeSum > 1)
	{
		m_FPS = count;

		wchar_t _Buffer[250];
		swprintf_s(_Buffer, L"FPS: %d , Delta Time: %f ", m_FPS, delta_time);
		SetWindowText(CCore::Create()->GetWindowData().hwnd, _Buffer);

		count = 0;
		TimeSum = 0;
	}

	//스탑워치
	if (m_StopWatch.OnOff == true)
		m_StopWatch.time -= delta_time;

	if (m_StopWatch.time < 0)
	{
		m_StopWatch.TimeOver = true;
		
	}
		
		

}