#include "shared.h"
#include "menudef.h"
#include "render.h"
#include "client.h"

#define WINDOW_MOUSEOVER        0x00000001  // mouse is over it, non exclusive
#define WINDOW_HASFOCUS         0x00000002  // has cursor focus, exclusive
#define WINDOW_VISIBLE          0x00000004  // is visible
#define WINDOW_GREY             0x00000008  // is visible but grey ( non-active )
#define WINDOW_DECORATION       0x00000010  // for decoration only, no mouse, keyboard, etc..
#define WINDOW_FADINGOUT        0x00000020  // fading out, non-active
#define WINDOW_FADINGIN         0x00000040  // fading in
#define WINDOW_MOUSEOVERTEXT    0x00000080  // mouse is over it, non exclusive
#define WINDOW_INTRANSITION     0x00000100  // window is in transition
#define WINDOW_FORECOLORSET     0x00000200  // forecolor was explicitly set ( used to color alpha images or not )
#define WINDOW_HORIZONTAL       0x00000400  // for list boxes and sliders, vertical is default this is set of horizontal
#define WINDOW_LB_LEFTARROW     0x00000800  // mouse is over left/up arrow
#define WINDOW_LB_RIGHTARROW    0x00001000  // mouse is over right/down arrow
#define WINDOW_LB_THUMB         0x00002000  // mouse is over thumb
#define WINDOW_LB_PGUP          0x00004000  // mouse is over page up
#define WINDOW_LB_PGDN          0x00008000  // mouse is over page down
#define WINDOW_ORBITING         0x00010000  // item is in orbit
#define WINDOW_OOB_CLICK        0x00020000  // close on out of bounds click
#define WINDOW_WRAPPED          0x00040000  // manually wrap text
#define WINDOW_AUTOWRAPPED      0x00080000  // auto wrap text
#define WINDOW_FORCED           0x00100000  // forced open
#define WINDOW_POPUP            0x00200000  // popup
#define WINDOW_BACKCOLORSET     0x00400000  // backcolor was explicitly set
#define WINDOW_TIMEDVISIBLE     0x00800000  // visibility timing ( NOT implemented )
#define WINDOW_IGNORE_HUDALPHA  0x01000000  // window will apply cg_hudAlpha value to colors unless this flag is set
#define WINDOW_DRAWALWAYSONTOP  0x02000000
#define WINDOW_MODAL            0x04000000 // window is modal, the window to go back to is stored in a stack
#define WINDOW_FOCUSPULSE       0x08000000
#define WINDOW_TEXTASINT        0x10000000
#define WINDOW_TEXTASFLOAT      0x20000000
#define WINDOW_LB_SOMEWHERE     0x40000000

DWORD uo_ui_mp;

//#define UI_FILE_OFF(x) (ui_mp + (x - 0x400076A0)) //vmMain in IDA offset
#define UI_FILE_OFF(x) (uo_ui_mp + (x - 0x40000000)) //vmMain in IDA offset


void _UI_Init(int inGameLoad) {
	void(*o)(int);
	*(int*)&o = UI_FILE_OFF(0x4000D310);

	o(inGameLoad);

}


void UI_DisplayDownloadInfo(const char downloadName, float centerPoint, float yStart, float scale) {
	void(*DisplayText)(const char, float, float, float);
	*(int*)&DisplayText = UI_FILE_OFF(0x40010700);
	DisplayText(downloadName, centerPoint, yStart, 0.1);
}

void UI_Init(DWORD base) {
	uo_ui_mp = base;

	__call(UI_FILE_OFF(0x400112CB), (int)UI_DisplayDownloadInfo); // Smaller download text (UDP only).
}
