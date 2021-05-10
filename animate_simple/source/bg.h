#ifndef BG
#define BG
#include <nds.h>
#include <background.h>
// u16* background_gfx_sprites[3];

// typedef struct{
// 	int x;
// 	int y;
// 	int gfx_frame;
// }Background;

// void initBackgroundSprites(int gfxoffset, u8* gfx)
// {
// 	gfx += gfxoffset;

// 	for(int i = 0; i < 3; i++)
// 	{
// 		background_gfx_sprites[i] = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_256Color);
// 		dmaCopy(gfx, background_gfx_sprites[i], 64*64);
// 		gfx += 64*64;
// 	}
// }

// void drawBackground(Background* bg, int* id)
// {
// 	int value = *id;
//     oamSet(&oamSub, value, bg->x, bg->y, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
// 			background_gfx_sprites[bg->gfx_frame], -1, false, false, false, false, false);
// 	(*id)++;
// }

void DrawBackground()
{
    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(backgroundTiles, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(backgroundPal, BG_PALETTE, 256*2);
    dmaCopy(backgroundPal, SPRITE_PALETTE_SUB, 256*2);
}

#endif