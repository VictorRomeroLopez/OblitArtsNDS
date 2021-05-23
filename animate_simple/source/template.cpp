#include <nds.h>
#include <maxmod9.h>

#include <iostream>
#include <flappy_yellow.h>
#include <background.h>
#include <vector>
#include <time.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "constants.h"
#include "sprites.h"
#include "bird.h"
#include "pipe.h"
#include "GameStates/gameState.h"
#include "game.h"
#include "GameStates/gameStarting.h"
#include "GameStates/gamePlaying.h"
#include "GameStates/gameEnd.h"
#include "GameStates/gameStateManager.h"

u16* Sprites::endscreen_gfx_sprites[8];
u16* Sprites::numbers_gfx_sprites[10];

int main(void)
{
	srand(time(NULL));

	Game game = Game();
	GameStateManager gameStateManager = GameStateManager(&game);

	while(game.isRunning)
	{
		gameStateManager.Update();

		if(game.changeState)
		{
			game.changeState = false;
			gameStateManager.ChangeState(game.currentState);
		}
	}

	return 0;
}
