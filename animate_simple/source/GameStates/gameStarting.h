class GameStarting : public GameState{
    public:
        virtual void OnEnter(){
            videoSetMode(MODE_5_2D);
            videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);
            vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,
                     VRAM_B_MAIN_BG_0x06020000,
                     VRAM_C_SUB_BG_0x06200000,
                     VRAM_D_SUB_SPRITE);
            oamInit(&oamMain, SpriteMapping_1D_128, false);
	        oamInit(&oamSub, SpriteMapping_1D_128, false);

            Sprites::initBirdSprites(0, (u8*)flappy_yellowTiles);
            Sprites::initPipeSprites(32*32*4, (u8*)flappy_yellowTiles);
            Sprites::initNumbers(32*32*6, (u8*)flappy_yellowTiles);
        	Sprites::initEndWindowSprites(32*32*8, (u8*)flappy_yellowTiles);
            
	        dmaCopy(flappy_yellowPal, SPRITE_PALETTE_SUB, 512);
        };

        virtual void Update(){

        };

        virtual void OnExit(){

        };
};
