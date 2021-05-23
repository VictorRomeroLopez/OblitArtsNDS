class GamePlaying : public GameState
{
    public:
        Game* game;

        GamePlaying(Game* game):game(game){};

        void OnEnter() override
        {
            game->reset();
        }

        void Update() override
        {
            game->frameCounter++;
            scanKeys();
            int keys = keysDown();

            if(game->frameCounter % 10 == 0)
            {
                game->bird.animate();
            }

            if(keys)
            {
                if(keys & KEY_UP)
                {
                    game->bird.moveUp();
                }
                if(keys & KEY_DOWN)
                {
                    game->bird.moveDown();
                }
                if(keys & KEY_TOUCH)
                {
                    touchRead(&game->touch);

                    if(game->touch.py < 192/2)
                    {
                        game->bird.moveUp();
                    }
                    else
                    {
                        game->bird.moveDown();
                    }
                }

                game->bird.updatePosition();
            }
            
            game->checkBirdPipeCollision();

            game->resetDraw();
            game->drawScoreOutside();
            game->draw();

            swiWaitForVBlank();

		    oamUpdate(&oamMain);
		    oamUpdate(&oamSub);
        }

        void OnExit() override
        {

        }
};