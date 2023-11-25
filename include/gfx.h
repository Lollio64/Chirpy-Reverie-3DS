#ifndef CR3DS_GFX_H
#define CR3DS_GFX_H

#include "vshader_shbin.h"
#include <citro2d.h>
#include <citro3d.h>
#include <3ds.h>

// Define the maximum amount of sprites
#define MAX_SPRITES 50

// That's a sprite
typedef struct {
    C2D_Sprite sprite;
    float x, y;
} Sprite;

class gfx {
    public:
    gfx();
    ~gfx();

    // For simple 3D models, Yoshi Island style
    static DVLB_s* vshader_dvlb;
    static shaderProgram_s program;
    static int uLoc_projection;
    static C3D_Mtx projection;

    // Sprite functions
    Sprite* initSprite(Sprite* spriteSelect, size_t sprIdx, float px, float py, float sx, float sy, float rad);
    void rotSprite(Sprite* spriteSelect, float rad);
    void moveSprite(Sprite* spriteSelect, float x, float y);
    void scaleSprite(Sprite* spriteSelect, float x, float y);
    void animSprite(Sprite* spriteSelect, size_t oSprIdx, size_t nSprIdx, float px, float py, float sx, float sy, float rad);

    static void gfxDraw3D();
    static void gfxDraw2D();

    // Creating spriteSheet object and Sprite array
    static C2D_SpriteSheet spriteSheet;
    static Sprite sprites[MAX_SPRITES];
};
#endif