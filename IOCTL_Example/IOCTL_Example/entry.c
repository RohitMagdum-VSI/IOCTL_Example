#include <wdm.h>
#include "Function.h"

VOID
Example_Unload(
	PDRIVER_OBJECT DriverObject
);

NTSTATUS
DriverEntry(
	PDRIVER_OBJECT pDriverObject,
	PUNICODE_STRING pRegistryPath
)
{
	UNREFERENCED_PARAMETER(pRegistryPath);

	DbgBreakPoint();

	NTSTATUS NtStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObject = NULL;
	UNICODE_STRING usDriverName, usDosDeviceName;

	DbgPrint("DriverEntry Called \r\n");

	RtlInitUnicodeString(&usDriverName, L"\\Device\\Example");
	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\Example");

	NtStatus = IoCreateDevice(pDriverObject, 0, &usDriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	if (NT_SUCCESS(NtStatus))
	{
		pDriverObject->MajorFunction[IRP_MJ_CLOSE] = Example_Close;
		pDriverObject->MajorFunction[IRP_MJ_CREATE] = Example_Create;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Example_IoControl;
		pDriverObject->MajorFunction[IRP_MJ_READ] = Example_Read;
		pDriverObject->MajorFunction[IRP_MJ_WRITE] = Example_WriteDirectIO;
		
		pDriverObject->DriverUnload = Example_Unload;

		pDeviceObject->Flags |= IO_TYPE;

		pDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);

		/*
		 * Create a Symbolic Link to the device. Example -> \Device\Example
		 */

		IoCreateSymbolicLink(&usDosDeviceName, &usDriverName);
	}
	return STATUS_SUCCESS;
}

VOID
Example_Unload(
	PDRIVER_OBJECT DriverObject
)
{
	UNICODE_STRING usDosDeviceName;

	DbgPrint("Example Unload Called.\n");

	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\Example");
	IoDeleteSymbolicLink(&usDosDeviceName);

	IoDeleteDevice(DriverObject->DeviceObject);
}