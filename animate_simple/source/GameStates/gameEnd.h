class GameEnd : public GameState
{
    public:
        Game* game;

        GameEnd(Game* game) : game(game){};

        void OnEnter() override
        {

        }

        void Update() override
        {
            
            game->frameCounter++;

            if(game->frameCounter % 10 == 0)
            {
                game->bird.animate();
            }

            scanKeys();
            int keys = keysDown();

            if(keys)
            {
                if(keys & KEY_TOUCH)
                {
                    game->changeState = true;
                    game->currentState = GameStateType::Playing;
                }
                else if(keys & KEY_START)
                {
                    game->end();
                }
            }

            game->resetDraw();
            game->drawScore();
            game->draw();

            swiWaitForVBlank();

            oamUpdate(&oamMain);
            oamUpdate(&oamSub);
        }

        void OnExit() override
        {

        }
};