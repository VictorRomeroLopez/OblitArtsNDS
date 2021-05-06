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
#include "pipe.h"
#include "bird.h"

//---------------------------------------------------------------------
// Screen dimentions
//---------------------------------------------------------------------
enum {SCREEN_TOP = 0, SCREEN_BOTTOM = 192, SCREEN_LEFT = 0, SCREEN_RIGHT = 256};

void initBackgrounds()
{
    /*  Set up affine background 3 on main as a 16-bit color background. */
    REG_BG3CNT = BG_BMP16_256x256 |
                 BG_BMP_BASE(0) | // The starting place in memory
                 BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    /*  Place main screen background 3 at the origin (upper left of the
     *  screen).
     */
    REG_BG3X = 0;
    REG_BG3Y = 0;

    /*  Set up affine background 2 on main as a 16-bit color background. */
    REG_BG2CNT = BG_BMP16_128x128 |
                 BG_BMP_BASE(8) | // The starting place in memory
                 BG_PRIORITY(2);  // A higher priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG2PA = 1 << 8;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 1 << 8;

    /*  Place main screen background 2 in an interesting place. */
    REG_BG2X = -(SCREEN_WIDTH / 2 - 32) << 8;
    REG_BG2Y = -32 << 8;

    /*  Set up affine background 3 on the sub screen as a 16-bit color
     *  background.
     */
    REG_BG3CNT_SUB = BG_BMP16_256x256 |
                     BG_BMP_BASE(0) | // The starting place in memory
                     BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the sub screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA_SUB = 1 << 8;
    REG_BG3PB_SUB = 0;
    REG_BG3PC_SUB = 0;
    REG_BG3PD_SUB = 1 << 8;

    /*
     *  Place main screen background 3 at the origin (upper left of the screen)
     */
    REG_BG3X_SUB = 0;
    REG_BG3Y_SUB = 0;
}

//---------------------------------------------------------------------
// main
//---------------------------------------------------------------------
int main(void) 
{
	int frameCounter = 0;
	Bird bird = {38,0};
	Pipe middle_pipe = {10, 10, PIPE_MIDDLE};
	Pipe down_pipe = {10, 32+10, PIPE_DOWN};

	//-----------------------------------------------------------------
	// Initialize the graphics engines
	//-----------------------------------------------------------------
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	//-----------------------------------------------------------------
	// Init sprites
	//-----------------------------------------------------------------
	initBirdSprites(0, (u8*)flappy_yellowTiles);
	initPipeSprites(32*32*6, (u8*)flappy_yellowTiles);
	updateBirdPosition(&bird);
	
	//dmaCopy(manPal, SPRITE_PALETTE, 512);
	dmaCopy(flappy_yellowPal, SPRITE_PALETTE_SUB, 512);

	//-----------------------------------------------------------------
	// main loop
	//-----------------------------------------------------------------
	while(1) 
	{
		frameCounter++;
		scanKeys();

		int keys = keysDown();
		
		if(keys & KEY_START) break;

		if(keys)
		{
			if(keys & KEY_UP)
			{
				moveBirdUp(&bird);
			}
			if(keys & KEY_LEFT)
			{

			}
			if(keys & KEY_RIGHT)
			{

			}
			if(keys & KEY_DOWN)
			{
				moveBirdDown(&bird);
			}
			updateBirdPosition(&bird);
		}

		if(frameCounter % 10 == 0){
			nextBirdAnimationFrame(&bird);
		}

		//-----------------------------------------------------------------
		// Set oam attributes, notice the only difference is in the sprite 
		// graphics memory pointer argument.  The man only has one pointer
		// while the women has an array of pointers
		//------------------------------------------------------------------

		//int mainId = 0;
		// oamSet(&oamMain, mainId, man.x, man.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
		// 	man.sprite_gfx_mem, -1, false, false, false, false, false);
		
		int subId = 0;
		drawBird(&bird, &subId);
		drawPipe(&down_pipe, &subId);
		drawPipe(&middle_pipe, &subId);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0;
}
