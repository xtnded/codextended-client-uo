#include "shared.h"
#include "gl/gl.h"
#pragma comment(lib, "libs/detours/detours.lib")
#include "libs/detours/detours.h"

HMODULE(WINAPI *orig_LoadLibraryA)(LPCSTR lpFileName);
HMODULE WINAPI hLoadLibraryA(LPSTR lpFileName) {
	HMODULE hModule = orig_LoadLibraryA(lpFileName);
	DWORD pBase = (DWORD)GetModuleHandle(lpFileName);

	if (!pBase) return hModule;

	void Main_UnprotectModule(HMODULE hModule);
	Main_UnprotectModule(hModule);

	if (codversion != CODUO_51) return hModule;

	if (strstr(lpFileName, "uo_cgame_mp") != NULL) {
		void CG_Init(DWORD);
		CG_Init(pBase);
	}

	return hModule;
}

void patch_opcode_loadlibrary(void) {
	orig_LoadLibraryA = (struct HINSTANCE__ *(__stdcall*)(const char*)) \
	DetourFunction((LPBYTE)LoadLibraryA, (LPBYTE)hLoadLibraryA);
}