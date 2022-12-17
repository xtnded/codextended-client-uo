#include "stdafx.h"
#include "shared.h"
#include "client.h"

void CleanupExit() {
	void(*o)();
	*(UINT32*)&o = 0x40F8E0;
	o();

	void Sys_Unload();
	Sys_Unload();
}

void Main_UnprotectModule(HMODULE hModule) {
	PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);

	// unprotect the entire PE image
	SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
	DWORD oldProtect;
	VirtualProtect((LPVOID)hModule, size, PAGE_EXECUTE_READWRITE, &oldProtect);
}

bool apply_hooks() {
	// allow alt tab - set dwExStyle from WS_EX_TOPMOST to WS_EX_LEFT (default), which allows minimizing
	XUNLOCK((void*)0x4f45b0, 1);
	memset((void*)0x4f45b0, 0x00, 1);

	HMODULE hModule;
	if (SUCCEEDED(GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)apply_hooks, &hModule))) {
		Main_UnprotectModule(hModule);
	}

	void patch_opcode_loadlibrary();
	patch_opcode_loadlibrary();
	
	if (codversion != CODUO_51) return true;

	PATCH_PUSH_STRING_PTR_VALUE(0x4F467C, "CoD UO Extended");

	__call(0x46B565, (int)CleanupExit);

	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	__call(0x56FB98, (int)WinMain);

	unlock_client_structure(); // make some client cls_ structure members writeable etc

	void CL_FrameStub();
	__call(0x43C8C3, (int)CL_FrameStub);

	void CL_Init();
	__call(0x43C166, (int)CL_Init);
	__call(0x43C7C7, (int)CL_Init);

	int Com_EventLoop(void);
	__call(0x43C6A7, (int)Com_EventLoop); //Calling EventLoop to increase gamestate memory :)

	__nop(0x4E6106, 1);

	*(int*)0x4015A0 = 131072;

	return true;
}