#include <windows.h>
#include <stdio.h>

int _cdecl main(void)
{
	HANDLE hFile;
	DWORD dwReturn;
	BOOL bRetVal;

	hFile = CreateFileA("\\\\.\\Example", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		printf("Create File successful. Writing.\n");
		bRetVal = WriteFile(hFile, "Hello from user mode!", sizeof("Hello from user mode!"), &dwReturn, NULL);
		if (bRetVal == 0)
		{
			printf("Failed to write. Error = %d\n", GetLastError());

		}
		CloseHandle(hFile);
	}

	printf("Failed to open file. Error = %d.\n", GetLastError());

	return 0;
}