#include <wdm.h>
#include "Function.h"

#pragma alloc_text(PAGE, Example_Create) 
#pragma alloc_text(PAGE, Example_Close) 
#pragma alloc_text(PAGE, Example_IoControl) 
#pragma alloc_text(PAGE, Example_Read)
#pragma alloc_text(PAGE, Example_WriteDirectIO)
#pragma alloc_text(PAGE, Example_WriteBufferedIO)
#pragma alloc_text(PAGE, Example_WriteNeither)
#pragma alloc_text(PAGE, Example_UnSupportedFunction)
#pragma alloc_text(PAGE, Example_IsStringTerminated)

NTSTATUS 
Example_Create(
	PDEVICE_OBJECT DeviceObject, 
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(Irp);
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	DbgPrint("Example_Create Called \r\n");

	return NtStatus;
}

NTSTATUS 
Example_Close(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(Irp);
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	DbgPrint("Example_Close Called \r\n");

	return NtStatus;
}

NTSTATUS 
Example_IoControl(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(Irp);
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	DbgPrint("Example_IoControl Called \r\n");

	return NtStatus;
}

NTSTATUS 
Example_Read(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(Irp);
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	DbgPrint("Example_Read Called \r\n");

	return NtStatus;
}

NTSTATUS 
Example_WriteDirectIO(
	PDEVICE_OBJECT DeviceObject, 
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStackIrp = NULL;
	PCHAR pWriteDataBuffer;

	DbgPrint("Example_WriteDirectIO Called \r\n");

	pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);

	if (pIoStackIrp)
	{
		pWriteDataBuffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);

		if (pWriteDataBuffer)
		{
			if (Example_IsStringTerminated(pWriteDataBuffer, pIoStackIrp->Parameters.Write.Length))
			{
				DbgPrint(pWriteDataBuffer);
			}
		}
	}

	return NtStatus;
}

NTSTATUS
Example_WriteBufferedIO(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStackIrp = NULL;
	PCHAR pWriteDataBuffer;

	DbgPrint("Example_WriteBufferedIO Called \r\n");

	pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);

	if (pIoStackIrp)
	{
		pWriteDataBuffer = (PCHAR)Irp->AssociatedIrp.SystemBuffer;

		if (pWriteDataBuffer)
		{
			if (Example_IsStringTerminated(pWriteDataBuffer, pIoStackIrp->Parameters.Write.Length))
			{
				DbgPrint(pWriteDataBuffer);
			}
		}
	}

	return NtStatus;
}

NTSTATUS 
Example_WriteNeither(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStackIrp = NULL;
	PCHAR pWriteDataBuffer;

	DbgPrint("Example_WriteNeither Called \r\n");

	pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);

	if (pIoStackIrp)
	{
		__try {

			ProbeForRead(Irp->UserBuffer, pIoStackIrp->Parameters.Write.Length, sizeof(char));
			pWriteDataBuffer = Irp->UserBuffer;

			if (pWriteDataBuffer)
			{
				if (Example_IsStringTerminated(pWriteDataBuffer, pIoStackIrp->Parameters.Write.Length))
				{
					DbgPrint(pWriteDataBuffer);
				}
			}

		}
		__except (EXCEPTION_EXECUTE_HANDLER) {

			NtStatus = GetExceptionCode();
		}

	}

	return NtStatus;
}

NTSTATUS 
Example_UnSupportedFunction(
	PDEVICE_OBJECT DeviceObject, 
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	NTSTATUS NtStatus = STATUS_NOT_SUPPORTED;
	DbgPrint("Example_UnSupportedFunction Called \r\n");

	return NtStatus;
}

BOOLEAN 
Example_IsStringTerminated(
	PCHAR pString,
	UINT uiLength
)
{
	BOOLEAN bStringIsTerminated = FALSE;
	UINT uiIndex = 0;

	while (uiIndex < uiLength && bStringIsTerminated == FALSE)
	{
		if (pString[uiIndex] == '\0')
		{
			bStringIsTerminated = TRUE;
		}
		else
		{
			uiIndex++;
		}
	}

	return bStringIsTerminated;
}