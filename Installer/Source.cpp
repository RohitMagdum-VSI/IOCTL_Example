#include <Windows.h>
#include <stdio.h>
#include "Installer.h"
#include "DriverUtil.h"

int main()
{
	int iRetVal = 0;

	iRetVal = InstallFilterDriver(
		PASSTHROUGH_FILTER_DRIVER_INF
	);
	if (0 != iRetVal)
	{
		printf("main: Failed to install the driver.");
		return 0;
	}

	printf("Install successful. Starting driver\n");
	iRetVal = StartFilterDriver(
		PASSTHROUGH_FILTER_DRIVER_NAME
	);
	if (0 != iRetVal)
	{
		printf("Failed to start the filter driver.\n");
		return iRetVal;
	}
	printf("Driver Started.\n");
	return 0;
}