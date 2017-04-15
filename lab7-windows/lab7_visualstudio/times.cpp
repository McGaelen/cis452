#include <iostream>
#include <Windows.h>

using namespace std;

int main ()
{
	__int64 freq, count, begin_time, end_time;

	/* number of clock_ticks/sec */
	QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

	/* 1. return frequency of high-res counter */
	cout << "Frequency of high-res counter: \t\t" << freq << endl;
	
	/* 2. return period of hi-res counter in milliseconds */
	double ms_per_tick = (1 / (freq / 1000.0));
	cout << "Period of hi-res counter (in ms): \t" << ms_per_tick << endl;

	/* 3. measure and report duration in ms of 1,000,000 iteration empty loop */
	QueryPerformanceCounter((LARGE_INTEGER *)&begin_time);
	for (int i = 0; i < 1000000; i++)
		;
	QueryPerformanceCounter((LARGE_INTEGER *)&end_time);
	cout << "Duration of 1,000,000 iteration loop: \t" << (end_time - begin_time) * ms_per_tick << endl;

	/* 4. report overhead in ms of call to hi-res counter itself */
	QueryPerformanceCounter((LARGE_INTEGER *)&begin_time);
	QueryPerformanceCounter((LARGE_INTEGER *)&count);
	QueryPerformanceCounter((LARGE_INTEGER *)&end_time);
	cout << "Duration of call to hi-res counter: \t" << (end_time - begin_time) * ms_per_tick << "\n" << endl;

	return 0;
}