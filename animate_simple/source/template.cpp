/*---------------------------------------------------------------------------------

Sprite animation using two naive but common approaches to animating frames.  

Sprites were created by via the sprite tool at:

http://charas-project.net/
http://charas-project.net/charas2/index.php

They were altered from their original 24x32 to 32x32 to make loading of frames simple 
for example purposes only.  They are converted using grit via the supplied sprite.grit file.

The man sprite loads new frame graphics every time the animation frame changes.  This
has the advantage of only consuming 32x32 bytes of data in sprite video memory and the
disadvantages of having to store the rest of the frames in main memory and having the overhead
of copying the data in each frame.

The woman sprite loads all animation frames to sprite memory upon initialization.  This 
has the advantage of allowing main memory to be freed, and causes the animation process
to be significantly faster as only a pointer is changed each frame.  The disadvantage is
this single sprite is  consuming nearly 10% of available sprite graphics memory for 
the sub display.

If one of these two methods are to be employed I recommend the manly approach as the sprite memory is
a scarce resource while main memory and cpu cycles are comparatively abundant.

A more advanced approach is to keep track of which frames of which sprites are loaded into memory
during the animation stage, load new graphics frames into memory overwriting the currently unused
frames only when sprite memory is full.  Decide which frame to unload by keeping track of how
often they are being used and be sure to mark all a sprites frames as unused when it is "killed"

This demo is using a very rigid animation engine in that it assumes that each frame of sprite
graphics is the same size, that each sprite can only walk up down left or right and that
each of these states is 3 frames in durration.  Significantly more advance animations can be
done by creating data structures to describe an animation sequence.  Perhaps more advanced
demos will follow this one.

-- dovoto (jason rogers)

---------------------------------------------------------------------------------*/
#include <nds.h>

#include <flappy_yellow.h>
#include <background.h>
#include <vector>

#include "bird.h"
#include "pipe.h"

//---------------------------------------------------------------------
// Screen dimentions
//---------------------------------------------------------------------
enum {SCREEN_TOP = 0, SCREEN_BOTTOM = 192, SCREEN_LEFT = 0, SCREEN_RIGHT = 256};

#pragma region Bg

void DrawBackground()
{
    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(backgroundTiles, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(backgroundPal, BG_PALETTE, 256*2);
    dmaCopy(backgroundPal, BG_PALETTE_SUB, 256*2);
}

#pragma endregion

#pragma region Bird

void initBirdSprites(int gfxoffset, u8* gfx){
	gfx += gfxoffset;

	for(int i = 0; i < 3; i++)
	{
		bird_gfx_mem[i] = oamAllocateGfx(&oamSub, 
		SpriteSize_32x32, SpriteColorFormat_256Color);
		dmaCopy(gfx, bird_gfx_mem[i], 32*32);
		gfx += 32*32;
	}
}

void CheckPosition();

void moveBirdUp()
{
	bird_position--;
	CheckPosition();
}

void moveBirdDown()
{
	bird_position++;
	CheckPosition();
}

int numPositions()
{
	return sizeof(bird_positions)/sizeof(bird_positions[0]);
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

#pragma endregion

#pragma region Pipe-

void initPipeSprites(int gfxoffset, u8* gfx)
{
	gfx += gfxoffset;

	for(int i = 0; i < 2; i++)
	{
		pipe_gfx_sprites[i] = oamAllocateGfx(&oamSub, SpriteSize_32x32, 
		SpriteColorFormat_256Color);
		dmaCopy(gfx, pipe_gfx_sprites[i], 32*32);
		gfx += 32*32;
	}
}

#pragma endregion

int main(void) 
{
	short frameCounter = 0;

	BirdCpp birdcpp = BirdCpp(38,0);
	PipeCpp pipecpp = PipeCpp();

	//-----------------------------------------------------------------
	// Initialize the graphics engines
	//-----------------------------------------------------------------
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	
	//touch Input Init
	touchPosition touch;

	//-----------------------------------------------------------------
	// Init sprites
	//-----------------------------------------------------------------
	initBirdSprites(0, (u8*)flappy_yellowTiles);
	initPipeSprites(32*32*6, (u8*)flappy_yellowTiles);
	birdcpp.UpdatePosition();
	DrawBackground();
	//dmaCopy(manPal, SPRITE_PALETTE, 512);
	dmaCopy(flappy_yellowPal, SPRITE_PALETTE_SUB, 512);
	
	//-----------------------------------------------------------------
	// main loop
	//-----------------------------------------------------------------
	while(1) 
	{
		frameCounter++;

		#pragma region KeyDetection

		scanKeys();

		int keys = keysDown();
		
		if(keys & KEY_START) break;

		if(keys)
		{
			if(keys & KEY_UP)
			{
				moveBirdUp();
			}
			if(keys & KEY_LEFT)
			{

			}
			if(keys & KEY_RIGHT)
			{

			}
			if(keys & KEY_DOWN)
			{
				moveBirdDown();
			}
			if(keys & KEY_TOUCH)
			{
				touchRead(&touch);

				if(touch.py < SCREEN_BOTTOM/2)
				{
					moveBirdUp();
				}
				else
				{
					moveBirdDown();
				}
			}

			birdcpp.UpdatePosition();
		}

		#pragma endregion

		if(frameCounter % 10 == 0){
			birdcpp.Animate();
		}

		pipecpp.movePipe();

		//-----------------------------------------------------------------
		// Set oam attributes, notice the only difference is in the sprite 
		// graphics memory pointer argument.  The man only has one pointer
		// while the women has an array of pointers
		//------------------------------------------------------------------

		//int mainId = 0;
		// oamSet(&oamMain, mainId, man.x, man.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
		// 	man.sprite_gfx_mem, -1, false, false, false, false, false);
		
		u8 subId = 0;
		birdcpp.drawBird(&subId);
		pipecpp.drawPipe(&subId);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0;
}
