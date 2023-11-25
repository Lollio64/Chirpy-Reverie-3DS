#include <gfx.h>

// Define static vars
DVLB_s* gfx::vshader_dvlb = NULL;
shaderProgram_s gfx::program;
int gfx::uLoc_projection;
C3D_Mtx gfx::projection;
C2D_SpriteSheet gfx::spriteSheet;
Sprite gfx::sprites[MAX_SPRITES];

gfx::gfx() {
    // Init graphics
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // Load spriteSheet and crash the app if it's not being found
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

    // Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");
    
	// Configure attributes for use with the vertex shader
	// Attribute format and element count are ignored in immediate mode
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 3); // v1=color

	// Compute the projection matrix
	Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);

	// Configure the first fragment shading substage to just pass through the vertex color
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
};

Sprite* gfx::initSprite(size_t sprIdx, float px, float py, float sx, float sy, float rad) {
    Sprite* spriteSelect = &new sprites[sprIdx];

    C2D_SpriteFromSheet(&spriteSelect->sprite, spriteSheet, sprIdx);
    C2D_SpriteSetCenter(&spriteSelect->sprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&spriteSelect->sprite, px, py);
    C2D_SpriteSetRotation(&spriteSelect->sprite, rad);
    C2D_SpriteSetScale(&spriteSelect->sprite, sx, sy);

    return spriteSelect;
};

void gfx::rotSprite(Sprite* spriteSelect, float rad) {
    C2D_SpriteRotate(&spriteSelect->sprite, rad);
};

void gfx::moveSprite(Sprite* spriteSelect, float x, float y) {
    C2D_SpriteMove(&spriteSelect->sprite, x, y);
};

void gfx::scaleSprite(Sprite* spriteSelect, float x, float y) {
    C2D_SpriteScale(&spriteSelect->sprite, x, y);
};

void gfx::animSprite(Sprite* spriteSelect, size_t oSprIdx, size_t nSprIdx, float px, float py, float sx, float sy, float rad) {
    delete spriteSelect[oSprIdx];
    initSprite(size_t nSprIdx, float px, float py, float sx, float sy, float rad);
};

gfx::~gfx() {
    // Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);

    // Deinit graphics
    C2D_SpriteSheetFree(spriteSheet);
    C2D_Fini();
    C3D_Fini();
};