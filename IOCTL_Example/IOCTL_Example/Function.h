#pragma once

typedef unsigned int UINT;
typedef char* PCHAR;

NTSTATUS
Example_Create(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);

NTSTATUS 
Example_Close(
	PDEVICE_OBJECT DeviceObject, 
	PIRP Irp
);

NTSTATUS 
Example_IoControl(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);

NTSTATUS 
Example_Read(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);

NTSTATUS
Example_WriteBufferedIO(
	PDEVICE_OBJECT DeviceObject, 
	PIRP Irp
);

NTSTATUS 
Example_WriteDirectIO(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);

NTSTATUS 
Example_WriteNeither(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);

NTSTATUS 
Example_UnSupportedFunction(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
);

BOOLEAN 
Example_IsStringTerminated(
	PCHAR pString, 
	UINT uiLength
);

#define IO_TYPE DO_DIRECT_IO