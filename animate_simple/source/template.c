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

#include <man.h>
#include <woman.h>
#include <flappy_yellow.h>

#define FRAMES_PER_ANIMATION 3

//---------------------------------------------------------------------
// The man sprite
// he needs a single pointer to sprite memory
// and a reference to his frame graphics so they
// can be loaded as needed
//---------------------------------------------------------------------
typedef struct 
{
	int x;
	int y;

	u16* sprite_gfx_mem;
	u8*  frame_gfx;

	int state;
	int anim_frame;
}Man;

//---------------------------------------------------------------------
// The womman sprite
// she needs an array of pointers to sprite memory since all 
// her frames are to be loaded.
// she also needs to keep track of which sprite memory pointer is in use
//---------------------------------------------------------------------

typedef struct
{
	int x;
	int y;

	u16* sprite_gfx_mem[12];
	int gfx_frame;

	int state;
	int anim_frame;


}Woman;

typedef struct
{
	int x;
	int y;

	u16* sprite_gfx_mem[12];
	int gfx_frame;

	int anim_frame;
}Bird;

//---------------------------------------------------------------------
// The state of the sprite (which way it is walking)
//---------------------------------------------------------------------
enum SpriteState {W_UP = 0, W_RIGHT = 1, W_DOWN = 2, W_LEFT = 3};

//---------------------------------------------------------------------
// Screen dimentions
//---------------------------------------------------------------------
enum {SCREEN_TOP = 0, SCREEN_BOTTOM = 192, SCREEN_LEFT = 0, SCREEN_RIGHT = 256};

//---------------------------------------------------------------------
// Animating a man requires us to copy in a new frame of data each time
//---------------------------------------------------------------------
void animateMan(Man *sprite)
{
	int frame = sprite->anim_frame + sprite->state * FRAMES_PER_ANIMATION;

	u8* offset = sprite->frame_gfx + frame * 32*32;

	dmaCopy(offset, sprite->sprite_gfx_mem, 32*32);
}

//---------------------------------------------------------------------
// Initializing a man requires little work, allocate room for one frame
// and set the frame gfx pointer
//---------------------------------------------------------------------
void initMan(Man *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

//---------------------------------------------------------------------
// Animating a woman only requires us to alter which sprite memory pointer
// she is using
//---------------------------------------------------------------------
void animateWoman(Woman *sprite)
{
	sprite->gfx_frame = sprite->anim_frame + sprite->state * FRAMES_PER_ANIMATION;
}

//---------------------------------------------------------------------
// Initializing a woman requires us to load all of her graphics frames 
// into memory
//---------------------------------------------------------------------
void initWoman(Woman *sprite, u8* gfx)
{
	int i;

	for(i = 0; i < 12; i++)
	{
		sprite->sprite_gfx_mem[i] = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
		dmaCopy(gfx, sprite->sprite_gfx_mem[i], 32*32);
		gfx += 32*32;
	}
}

void animateBird(Bird *sprite)
{
	sprite->gfx_frame = sprite->anim_frame;
}

void showPipe(Bird *sprite){
	sprite->gfx_frame = 0;
}

void initBird(Bird* sprite, int gfxoffset, u8* gfx)
{
	int i;

	gfx += gfxoffset;

	for(i = 0; i < 3; i++)
	{
		sprite->sprite_gfx_mem[i] = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
		dmaCopy(gfx, sprite->sprite_gfx_mem[i], 32*32);
		gfx += 32*32;
	}
}

void initPipe(Bird* sprite, int gfxoffset, u8* gfx){
	
	gfx += gfxoffset;

	sprite->sprite_gfx_mem[0] = oamAllocateGfx(&oamSub, SpriteSize_32x64, SpriteColorFormat_256Color);
	dmaCopy(gfx, sprite->sprite_gfx_mem[0], 32*64);
	gfx += 32*64;
}

void initBackgrounds() {
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
	Man man = {0,0};
	Bird bird = {0,0};
	Bird bird_red = {0,0};
	Bird pipe = {0,0};

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
	// Initialize the two sprites
	//-----------------------------------------------------------------
	initMan(&man, (u8*)manTiles);
	initBird(&bird, 0, (u8*)flappy_yellowTiles);
	initBird(&bird_red, 32*32*3, (u8*)flappy_yellowTiles);
	initPipe(&pipe, 32*32*6, (u8*)flappy_yellowTiles);
	
	dmaCopy(manPal, SPRITE_PALETTE, 512);
	dmaCopy(flappy_yellowPal, SPRITE_PALETTE_SUB, 512);

	//-----------------------------------------------------------------
	// main loop
	//-----------------------------------------------------------------
	while(1) 
	{
		frameCounter++;
		scanKeys();

		int keys = keysHeld();
		
		if(keys & KEY_START) break;

		if(keys)
		{
			if(keys & KEY_UP)
			{
				if(man.y >= SCREEN_TOP) man.y--;
				if(bird.y >= SCREEN_TOP) bird.y--;

				man.state = W_UP;
			}
			if(keys & KEY_LEFT)
			{
				if(man.x >= SCREEN_LEFT) man.x--;
				if(bird.x >= SCREEN_LEFT) bird.x--;

				man.state = W_LEFT;
			}
			if(keys & KEY_RIGHT)
			{
				if(man.x <= SCREEN_RIGHT) man.x++;
				if(bird.x <= SCREEN_RIGHT) bird.x++;

				man.state = W_RIGHT;
			}
			if(keys & KEY_DOWN)
			{
				if(man.y <= SCREEN_BOTTOM) man.y++;
				if(bird.y <= SCREEN_BOTTOM) bird.y++;

				man.state = W_DOWN;
			}
		}

		if(frameCounter % 10 == 0){
			man.anim_frame++;
			bird.anim_frame++;
			bird_red.anim_frame++;
		}

		if(man.anim_frame >= FRAMES_PER_ANIMATION) 
		{
			man.anim_frame = 0;
		}

		if(bird.anim_frame >= FRAMES_PER_ANIMATION)
		{
			bird.anim_frame = 0;
		}

		if(bird_red.anim_frame >= FRAMES_PER_ANIMATION)
		{
			bird_red.anim_frame = 0;
		}

		animateMan(&man);
		animateBird(&bird);
		animateBird(&bird_red);
		showPipe(&pipe);

		//-----------------------------------------------------------------
		// Set oam attributes, notice the only difference is in the sprite 
		// graphics memory pointer argument.  The man only has one pointer
		// while the women has an array of pointers
		//-----------------------------------------------------------------
		int mainId = 0;
		oamSet(&oamMain, mainId, man.x, man.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			man.sprite_gfx_mem, -1, false, false, false, false, false);
		
		int subId = 0;
		oamSet(&oamSub, subId, bird.x, bird.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			bird.sprite_gfx_mem[bird.gfx_frame], -1, false, false, false, false, false);

		subId++;
		oamSet(&oamSub, subId, bird_red.x, bird_red.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			bird_red.sprite_gfx_mem[bird_red.gfx_frame], -1, false, false, false, false, false);
			
		subId++;
		oamSet(&oamSub, subId, pipe.x, pipe.y, 0, 0, SpriteSize_32x64, SpriteColorFormat_256Color, 
			pipe.sprite_gfx_mem[pipe.gfx_frame], -1, false, false, false, false, false);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0;
}
