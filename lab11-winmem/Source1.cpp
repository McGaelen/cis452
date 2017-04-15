#include <Windows.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(void) {
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	cout << "Page size: " << info.dwPageSize << " Bytes." << endl;

	char *mem = (char *)malloc(1000000);
	
	MEMORY_BASIC_INFORMATION meminfo;
	VirtualQuery(mem, &meminfo, sizeof(meminfo));
	if (meminfo.State == MEM_COMMIT) {
		cout << "Memory State: Committed" << endl;
	} else if (meminfo.State == MEM_FREE) {
		cout << "Memory State: Free" << endl;
	} else if (meminfo.State == MEM_RESERVE) {
		cout << "Memory State: Reserved" << endl;
	}

	free(mem);

	VirtualQuery(mem, &meminfo, sizeof(meminfo));
	if (meminfo.State == MEM_COMMIT) {
		cout << "Memory State: Committed" << endl;
	} else if (meminfo.State == MEM_FREE) {
		cout << "Memory State: Free" << endl;
	} else if (meminfo.State == MEM_RESERVE) {
		cout << "Memory State: Reserved" << endl;
	}

	return 0;
}