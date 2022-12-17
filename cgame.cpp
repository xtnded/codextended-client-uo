#include "shared.h"
#include "client.h"
#include "render.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

DWORD uo_cgame_mp;
#define M_DrawShadowString(x,y,font,fontscale,color,text,a,b,c) \
	RE_SetColor(vColorBlack); \
	SCR_DrawString(x + 1,y + 1,font,fontscale,vColorBlack,text,a,b,c); \
	RE_SetColor(color); \
	SCR_DrawString(x,y,font,fontscale,color,text,a,b,c); \
	RE_SetColor(NULL);

void CG_DrawFPS(float y) {
	cvar_t* xui_fps = Cvar_Get("cg_xui_fps", "1", CVAR_ARCHIVE);

	if (xui_fps->integer) {
		cvar_t* x = Cvar_Get("cg_xui_fps_x", "597", CVAR_ARCHIVE); // uh this x y values just look good with my hp bar
		cvar_t* y = Cvar_Get("cg_xui_fps_y", "8", CVAR_ARCHIVE);

		#define	FPS_FRAMES 4
		static int previousTimes[FPS_FRAMES];
		static int index;
		int	i, total;
		int	fps;
		static int previous;
		int	t, frameTime;

		t = timeGetTime();
		frameTime = t - previous;
		previous = t;
		previousTimes[index % FPS_FRAMES] = frameTime;
		index++;

		if (index > FPS_FRAMES) {
			total = 0;
			for (i = 0; i < FPS_FRAMES; i++) {
				total += previousTimes[i];
			}
			if (!total) {
				total = 1;
			}
			fps = 1000 * FPS_FRAMES / total;

			M_DrawShadowString(x->integer, y->integer, 1, .20, vColorWhite, va("FPS: %d", fps), NULL, NULL, NULL);
		}
	} else {
		void(*call)(float);
		*(int*)&call = CGAME_OFF(0x30018090);
		call(y);
	}
}

int JumpHeightCrap(float height) {
	char* x_jump_height = Info_ValueForKey(cs1, "x_jump_height");
	if (strlen(x_jump_height)) {
		height = atof(x_jump_height);
	}

	int(*call)(float);
	*(int*)(&call) = CGAME_OFF(0x30008C70);
	int result = call(height);
	return result;
}

void CG_Init(DWORD base) {
	uo_cgame_mp = base;
	
	
	__call(CGAME_OFF(0x3001875C), (int)CG_DrawFPS);
	

	__call(CGAME_OFF(0x30008D72), (int)JumpHeightCrap);
	*(UINT32*)CGAME_OFF(0x3008523C) = CVAR_ARCHIVE; // Enable cg_fov
	*(UINT32*)CGAME_OFF(0x3008570C) = CVAR_ARCHIVE; // Unlock cg_thirdpersonangle
	*(UINT32*)CGAME_OFF(0x3008571C) = CVAR_ARCHIVE; // Unlock cg_thirdperson
}