class GameStarting : public GameState{
    public:
        Game* game;
        GameStarting(Game* game):game(game){};

        void OnEnter() override
        {
            videoSetMode(MODE_5_2D);
            videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);

            oamInit(&oamMain, SpriteMapping_1D_128, false);
            oamInit(&oamSub, SpriteMapping_1D_128, false);
            
            int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
            dmaCopy(backgroundBitmap, bgGetGfxPtr(bg3), Constants::SCREEN_RIGHT*Constants::SCREEN_RIGHT);
            dmaCopy(backgroundPal, BG_PALETTE, Constants::SCREEN_RIGHT*2);
            dmaCopy(backgroundPal, BG_PALETTE_SUB, Constants::SCREEN_RIGHT*2);

            Sprites::initBirdSprites(0, (u8*)flappy_yellowTiles);
            Sprites::initPipeSprites(32*32*4, (u8*)flappy_yellowTiles);
            Sprites::initNumbers(32*32*6, (u8*)flappy_yellowTiles);
            Sprites::initEndWindowSprites(32*32*8, (u8*)flappy_yellowTiles);

            dmaCopy(flappy_yellowPal, SPRITE_PALETTE_SUB, 512);

            game->loadMusic();
        }

        void Update() override
        {
            scanKeys();
            int keys = keysDown();

            if(keys)
            {
                if(keys & KEY_TOUCH)
                {
                    game->changeState = true;
                    game->currentState = GameStateType::Playing;
                }
            }

            game->resetDraw();
            game->draw();

            swiWaitForVBlank();

            oamUpdate(&oamMain);
            oamUpdate(&oamSub);
        }

        void OnExit() override
        {

        }
};
