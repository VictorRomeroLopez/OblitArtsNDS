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

#include <iostream>
#include <flappy_yellow.h>
#include <background.h>
#include <vector>
#include <time.h>
#include <map>

#include "sprites.h"
#include "bird.h"
#include "pipe.h"
#include "GameStates/gameState.h"
#include "GameStates/gameStarting.h"
#include "game.h"

//---------------------------------------------------------------------
// Screen dimentions
//---------------------------------------------------------------------
enum {
	SCREEN_TOP = 0, 
	SCREEN_BOTTOM = 192, 
	SCREEN_LEFT = 0, 
	SCREEN_RIGHT = 256
};


u16* Sprites::endscreen_gfx_sprites[8];
u16* Sprites::numbers_gfx_sprites[10];

u8 score = 0;

bool _drawScore = false;

#pragma region Bg

void DrawBackground()
{
    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(backgroundBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(backgroundPal, BG_PALETTE, 256*2);
    dmaCopy(backgroundPal, BG_PALETTE_SUB, 256*2);
}

#pragma endregion

void DrawNumber(int number, int x, int y, u8* id){
	
	oamSet(&oamSub, *id, x, y, 0, 0, SpriteSize_8x8, SpriteColorFormat_256Color,
			Sprites::numbers_gfx_sprites[number/10], -1, false, false, false, false, false);
	(*id)++;
	oamSet(&oamSub, *id, x+8, y, 0, 0, SpriteSize_8x8, SpriteColorFormat_256Color,
			Sprites::numbers_gfx_sprites[number%10], -1, false, false, false, false, false);
	(*id)++;
}

void Draw(BirdCpp* bird, std::vector<PipeCpp> pipes){
	// Set oam attributes, notice the only difference is in the sprite 
	// graphics memory pointer argument.  The man only has one pointer
	// while the women has an array of pointers
	//------------------------------------------------------------------

	//int mainId = 0;
	// oamSet(&oamMain, mainId, man.x, man.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
	// 	man.sprite_gfx_mem, -1, false, false, false, false, false);

	u8 subId = 0;

	if(_drawScore){

		DrawNumber(score, SCREEN_RIGHT/2 + 27, SCREEN_BOTTOM/2 - 13, &subId);

		int posX = SCREEN_RIGHT/2;
		int posY = SCREEN_BOTTOM/2;

		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 4; j++){
				oamSet(&oamSub, subId, posX  + 32*j - 64, posY  + 32*i - 32, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
						Sprites::endscreen_gfx_sprites[i*4+j], -1, false, false, false, false, false);
			subId++;
			}
		}
	}

	bird->drawBird(&subId);

	for(PipeCpp pipe : pipes){
		pipe.drawPipe(&subId);
	}
}

int main(void) 
{
	srand(time(NULL));
	
	u8 birdPositionX = 40;
	short frameCounter = 0;
	bool gameStarted = false;
	BirdCpp birdcpp = BirdCpp(birdPositionX);
	std::vector<PipeCpp> pipeVector = std::vector<PipeCpp>();

	Game game = Game();
	game.Things();
	game.Stuff();

	GameStateType currentState = GameStateType::Start;
	std::map<GameStateType, GameState*> stateTypeToGameState = std::map<GameStateType, GameState*>();
	stateTypeToGameState.insert(std::pair<GameStateType, GameState*>(GameStateType::Start, new GameStarting));
	
	stateTypeToGameState[currentState]->OnEnter();

	//touch Input Init
	touchPosition touch;

	for(int i = 0; i < 4 ; i++){
		pipeVector.push_back(PipeCpp(SCREEN_RIGHT + SPRITE_SIZE/2 + (72*i), bird_positions[rand()%4]));
	}

	DrawBackground();

	//dmaCopy(manPal, SPRITE_PALETTE, 512);
	//dmaCopy(flappy_yellowPal, SPRITE_PALETTE_SUB, 512);
	
	while(!gameStarted){
		
		scanKeys();
		int keys = keysDown();

		if(keys & KEY_START) break;

		if(keys)
		{
			if(keys & KEY_TOUCH)
			{
				gameStarted = true;
			}
		}

		Draw(&birdcpp, pipeVector);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	//-----------------------------------------------------------------
	// main loop
	//-----------------------------------------------------------------
	while(1)
	{
		frameCounter++;
		
		scanKeys();
		int keys = keysDown();

		if(keys & KEY_START) break;

		if(frameCounter % 10 == 0){
			birdcpp.Animate();
		}

		//GAME LOOP
		if(keys)
		{
			if(keys & KEY_UP)
			{
				birdcpp.moveUp();
			}
			if(keys & KEY_LEFT)
			{

			}
			if(keys & KEY_RIGHT)
			{

			}
			if(keys & KEY_DOWN)
			{
				birdcpp.moveDown();
			}
			if(keys & KEY_TOUCH)
			{
				touchRead(&touch);

				if(touch.py < SCREEN_BOTTOM/2)
				{
					birdcpp.moveUp();
				}
				else
				{
					birdcpp.moveDown();
				}
			}
			
			birdcpp.UpdatePosition();
		}

		bool birdCollidesWithPipe = false;
		for(int i = 0; i < pipeVector.size(); i++){
			if(pipeVector[i].isOffScreen()){
				pipeVector[i].setPositionInit();
			}

			if(pipeVector[i].checkCollision(&birdcpp)){
				birdCollidesWithPipe = true;
				break;
			}

			pipeVector[i].movePipe();

			if(pipeVector[i].checkScored(birdPositionX)){
				score++;
			}
		}

		Draw(&birdcpp, pipeVector);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		
		if(birdCollidesWithPipe){
			_drawScore = true;
			break;
		}
	}

	while(1){
		Draw(&birdcpp, pipeVector);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}
	return 0;
}
