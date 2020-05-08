#include <Windows.h>
#include <stdio.h>

int
main(void)
{
	SC_HANDLE hSCManager;
	SC_HANDLE hService;
	SERVICE_STATUS ss;
	BOOL bRetVal;

	hSCManager = OpenSCManager(
		NULL,
		NULL,
		SC_MANAGER_CREATE_SERVICE
	);

	printf("Load Driver\n");

	if (hSCManager)
	{
		printf("Create Service\n");

		hService = CreateServiceA(
			hSCManager,
			"IOCTL_Example", 
			"Example Driver",
			SERVICE_START | DELETE | SERVICE_STOP,
			SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_IGNORE,
			"C:\\Users\\vishal\\Desktop\\Windows_Lecture\\IOCTL_Example.sys", NULL, NULL, NULL, NULL, NULL);

		if (!hService)
		{
			printf("Failed to create service. Error = %d.\n", GetLastError());

			hService = OpenServiceA(hSCManager, "Example", SERVICE_START | DELETE | SERVICE_STOP);
		}

		if (hService)
		{
			printf("Start Service\n");

			bRetVal = StartService(hService, 0, NULL);
			if (bRetVal == 0)
			{
				printf("Failed to start the service. Error = %d\n", GetLastError());
			}
			printf("Press Enter to close service\r\n");
			getchar();
			ControlService(hService, SERVICE_CONTROL_STOP, &ss);

			CloseServiceHandle(hService);

			DeleteService(hService);
		}

		CloseServiceHandle(hSCManager);
	}

	return 0;
}