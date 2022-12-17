#include "shared.h"
#include <WinSock2.h>

bool apply_hooks();
bool determine_cod_version();
bool find_cod_version();

void codextended() {
	srand(time(NULL));

	if (!determine_cod_version()) {
		if (!find_cod_version()) {
			MsgBox("Failed to find Call of Duty version, please report this at www.cod1x.eu.");
			return;
		}
	}

	if (!apply_hooks()) {
		MsgBox("Failed to initialize CoDExtended, please report this at www.cod1x.eu.\n");
		Com_Quit_f();
	}
}

void codextended_unload() {}