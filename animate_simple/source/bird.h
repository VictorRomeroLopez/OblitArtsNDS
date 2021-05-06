#ifndef BIRD
#define BIRD

u16* bird_gfx_mem[6];

typedef struct{
	int x;
	int y;
	int gfx_frame;
}Bird;

#define BIRD_MAX_FRAME_ANIMATION 3

void initBirdSprites(int gfxoffset, u8* gfx){
	gfx += gfxoffset;

	for(int i = 0; i < 3; i++)
	{
		bird_gfx_mem[i] = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
		dmaCopy(gfx, bird_gfx_mem[i], 32*32);
		gfx += 32*32;
	}
}

void drawBird(Bird* bird, int id){
    oamSet(&oamSub, id, bird->x, bird->y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			bird_gfx_mem[bird->gfx_frame], -1, false, false, false, false, false);
}

void nextBirdAnimationFrame(Bird* bird){
    bird->gfx_frame++;
    
    if(bird->gfx_frame >= BIRD_MAX_FRAME_ANIMATION)
    {
        bird->gfx_frame = 0;
    }
}

#endif
