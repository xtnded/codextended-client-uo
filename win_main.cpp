#include "shared.h"
#include "Commctrl.h"
#include "ShlObj.h"
#include "Shlwapi.h"
#include "Shellapi.h"

static int(__stdcall *main)(HINSTANCE, HINSTANCE, LPSTR, int) = (int(__stdcall*)(HINSTANCE, HINSTANCE, LPSTR, int))0x46C5C0;

char sys_cmdline[MAX_STRING_CHARS];
extern HMODULE hModule;
HINSTANCE hInst;

extern "C" bool bClosing = false;

void Sys_Unload() {
	bClosing = true;
	static bool unloaded = false;

	if (unloaded)
		return;
	unloaded = true;

	void CL_DiscordShutdown();
	CL_DiscordShutdown();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hInst = hInstance;
	strncpy(sys_cmdline, lpCmdLine, sizeof(sys_cmdline) - 1);

	void MSS32_Hook();
	MSS32_Hook();

	extern bool miles32_loaded;
	if (!miles32_loaded)
		return 0;

	void CL_DiscordInitialize();
	CL_DiscordInitialize();

	return main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}