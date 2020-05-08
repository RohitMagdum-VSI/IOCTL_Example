#include <Windows.h>
#include <SetupAPI.h>
#include <fltUser.h>
#include <fltUserStructures.h>
#include "DriverUtil.h"
#include <stdio.h>

int InstallFilterDriver(
	const WCHAR *pwszInfFileName
)
{
	WCHAR *pwszTempStr;
	WCHAR wszModPath[MAX_PATH];
	WCHAR wszCommand[1024] = L"DefaultInstall 128 ";

	GetModuleFileName(NULL, wszModPath, MAX_PATH);

	pwszTempStr = wcsrchr(wszModPath, L'\\');
	if (NULL != pwszTempStr)
	{
		pwszTempStr++;
		*pwszTempStr = L'\0';

		wcscat_s(wszModPath, MAX_PATH, pwszInfFileName);
	}
	else
	{
		wcscat_s(wszModPath, MAX_PATH, L"\\");
		wcscat_s(wszModPath, MAX_PATH, pwszInfFileName);
	}

	wprintf(L"wszModPath = %s\n", wszModPath);

	wcscat_s(wszCommand, 1024, wszModPath);

	wprintf(L"wszCommand = %s\n", wszCommand);

	InstallHinfSection(NULL, NULL, wszCommand, 0);

	return 0;
}

int UninstallFilterDriver(
	const WCHAR *pwszInfFileName
)
{
	WCHAR *pwszTempStr;
	WCHAR wszModPath[MAX_PATH];
	WCHAR wszCommand[1024] = L"DefaultUninstall 128 ";

	GetModuleFileName(NULL, wszModPath, MAX_PATH);

	pwszTempStr = wcsrchr(wszModPath, L'\\');
	if (NULL != pwszTempStr)
	{
		pwszTempStr++;
		*pwszTempStr = L'\0';

		wcscat_s(wszModPath, MAX_PATH, pwszInfFileName);
	}
	else
	{
		wcscat_s(wszModPath, MAX_PATH, L"\\");
		wcscat_s(wszModPath, MAX_PATH, pwszInfFileName);
	}

	wcscat_s(wszCommand, 1024, wszModPath);

	InstallHinfSection(NULL, NULL, wszCommand, 0);

	return 0;
}

int StartFilterDriver(
	const WCHAR *pwszDriverServiceName
)
{
	LUID luid;
	HRESULT hRes;
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		printf("OpenProcessToken failed. = %d\n", GetLastError());
		return -1;
	}

	if (!LookupPrivilegeValue(NULL, SE_LOAD_DRIVER_NAME, &luid))
	{
		printf("LookupPrivilegeValue failed. Error = %d\n", GetLastError());
		CloseHandle(hToken);
		return -1;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges failed. Error = %d\n", GetLastError());
		CloseHandle(hToken);
		return -1;
	}

	hRes = FilterLoad(pwszDriverServiceName);
	if (S_OK != hRes)
	{
		if (HRESULT_FROM_WIN32(ERROR_SERVICE_ALREADY_RUNNING) == hRes)
		{
			printf("Service already Running.\n");
			return 0;
		}

		if (HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) == hRes)
		{
			InstallFilterDriver(FSCHANGEMON_FILTER_DRIVER_INF);

			hRes = FilterLoad(pwszDriverServiceName);

			printf("Error file not found.\n");
		}

		if (HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS) == hRes)
		{
			printf("Error Already Exists.\n");
		}

		if (HRESULT_FROM_WIN32(ERROR_BAD_EXE_FORMAT) == hRes)
		{
			printf("Error bad exe format.\n");
		}

		if (HRESULT_FROM_WIN32(ERROR_INVALID_IMAGE_HASH) == hRes)
		{
			printf("Error invalid image hash.\n");
		}

		if (S_OK != hRes)
		{
			printf("FilterLoad failed.\n");
			return -1;
		}
	}

	return 0;
}

int StopFilterDriver(
	HANDLE hCommPort,
	const WCHAR *pwszDriverServiceName
)
{
	HRESULT hRes;

	hRes = FilterUnload(pwszDriverServiceName);
	if (S_OK != hRes)
	{
		return -1;
	}

	return 0;
}