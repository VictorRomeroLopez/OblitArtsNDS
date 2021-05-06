#ifndef PIPE
#define PIPE

u16* pipe_gfx_sprites[3];

typedef struct{
	int x;
	int y;
	int gfx_frame;
}Pipe;

enum PipeState { PIPE_MIDDLE = 0, PIPE_UP = 1, PIPE_DOWN = 2};

void initPipeSprites(int gfxoffset, u8* gfx)
{
	gfx += gfxoffset;

	for(int i = 0; i < 3; i++)
	{
		pipe_gfx_sprites[i] = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
		dmaCopy(gfx, pipe_gfx_sprites[i], 32*32);
		gfx += 32*32;
	}
}

void drawPipe(Pipe* pipe, int id){
    oamSet(&oamSub, id, pipe->x, pipe->y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			pipe_gfx_sprites[pipe->gfx_frame], -1, false, false, false, false, false);
}

#endif