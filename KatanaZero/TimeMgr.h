#pragma once
class TimeMgr
{
private:
	TimeMgr();
	~TimeMgr();

	LARGE_INTEGER m_PrevCount;
	LARGE_INTEGER m_CurCount;
	LARGE_INTEGER m_Frequency;
	
	LARGE_INTEGER PrevTimer;
	LARGE_INTEGER FinishTimer;

	double delta_time;
	int m_FPS;
	int count;

	double TimeSum;

	struct StopWatch
	{
		double time;
		bool OnOff;
		bool TimeOver;
	} m_StopWatch;

public:
	static TimeMgr* Create()
	{
		static TimeMgr p;
		return &p;
	}

	void Initialize();
	void Update();
	double dt() { return delta_time; }

	void StartStopWatch(double time) { m_StopWatch.time = time; m_StopWatch.OnOff = true; m_StopWatch.TimeOver = false; }
	void EndStopWatch() { m_StopWatch.OnOff = false; }
	bool Timeover() 
	{ 
		if (m_StopWatch.OnOff)
		{
			if (m_StopWatch.TimeOver)
				m_StopWatch.OnOff = false;

			return m_StopWatch.TimeOver;
		}

		else
			return false;
		
	}

	void StartTimer() { QueryPerformanceCounter(&PrevTimer); }
	double CheckTimer() 
	{
		QueryPerformanceCounter(&FinishTimer);
		return ((double)FinishTimer.QuadPart - (double)PrevTimer.QuadPart) / (double)m_Frequency.QuadPart; 
	}
	
};

