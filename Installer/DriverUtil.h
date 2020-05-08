#pragma once
#include <Windows.h>

#define FSCHANGEMON_FILTER_DRIVER_INF L"FsChangeMonitorDriver.inf"
#define FSCHANGEMON_FILTER_DRIVER_NAME L"FsChangeMonitorDriver"

#define HEADER_LEN 184


int InstallFilterDriver(
	const WCHAR *pwszinfFileName
);

int UninstallFilterDriver(
	const  WCHAR *pwszInfFileName
);

int StartFilterDriver(
	const WCHAR *pwszDriverServiceName
);

int StopFilterDriver(
	HANDLE hCommPort,
	const WCHAR *pwszDriverServiceName
);