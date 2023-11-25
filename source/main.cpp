#include <gfx.h>

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

int main() {
    // Init default functions
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    romfsInit();

    // Create Render Targets
    C3D_RenderTarget* top = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTarget* topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    C3D_RenderTargetSetOutput(top, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

    // Main loop
    while(aptMainLoop()) {
        // Scan for user input
        hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break; // Break in order to return to the Homebrew Launcher
    };
    
    // Deinit default functions
    romfsExit();
    gfxExit();
    return 0;
};