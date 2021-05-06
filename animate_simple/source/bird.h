#ifndef BIRD
#define BIRD

u16* bird_gfx_mem[6];
int* bird_positions[4] = {38, 76, 115, 153};
int bird_position = 0;

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

void drawBird(Bird* bird, int* id){
	int value = *id;
    oamSet(&oamSub, value, bird->x-16, bird->y-16, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			bird_gfx_mem[bird->gfx_frame], -1, false, false, false, false, false);
	(*id)++;
}

void moveBirdUp(Bird* bird)
{
	bird_position--;
	CheckPosition();
}

void moveBirdDown(Bird* bird)
{
	bird_position++;
	CheckPosition();
}

void CheckPosition()
{
	if(bird_position < 0)
	{
		bird_position = 0;
	}
	else if(bird_position >= numPositions())
	{
		bird_position = numPositions() - 1;
	}
}

int numPositions()
{
	return sizeof(bird_positions)/sizeof(bird_positions[0]);
}

void updateBirdPosition(Bird* bird)
{
	bird->y = bird_positions[bird_position];
}

void nextBirdAnimationFrame(Bird* bird)
{
    bird->gfx_frame++;
    
    if(bird->gfx_frame >= BIRD_MAX_FRAME_ANIMATION)
    {
        bird->gfx_frame = 0;
    }
}

#endif
