#include <nds.h>

#define PIPE_SEPARATION 72
#define NUM_PIPES 4

class Game
{
    public:
        BirdCpp bird;
        std::vector<PipeCpp> pipeVector;
        GameStateType currentState;
        u8 score;
        u8 subId;
        touchPosition touch;
        short frameCounter;
        bool gameStarted;
        bool isRunning;
        bool changeState;

        mm_sound_effect bgr = {
            { SFX_COINS } , //id
            1024,           //rate
            0,              //handle
            255,            //volume
            128,            //panning
        };

        Game()
        {
            score = 0;
            frameCounter = 0;
            gameStarted = false;
            bird = BirdCpp(40);
            pipeVector = std::vector<PipeCpp>(NUM_PIPES);

            resetPipes();

            isRunning = true;
            changeState = false;
            currentState = GameStateType::Start;
        }

        void end()
        {
            isRunning = false;
            Sprites::unloadSprites();
            unloadMusic();
        }

        void reset()
        {
            resetPipes();
            score = 0;
            oamClear(&oamSub, 0, 0);
        }

        void resetPipes()
        {
            for(unsigned int i = 0; i < pipeVector.size(); i++)
            {
                pipeVector[i].setPosition(Constants::SCREEN_RIGHT + SPRITE_SIZE/2 + (PIPE_SEPARATION * i), bird_positions[rand()%4]);
            }
        }

        void drawNumber(int number, int x, int y, u8* id)
        {
            oamSet(&oamSub, *id, x, y, 0, 0, SpriteSize_8x8, SpriteColorFormat_256Color,
                    Sprites::numbers_gfx_sprites[number/10], -1, false, false, false, false, false);
            (*id)++;
            oamSet(&oamSub, *id, x+8, y, 0, 0, SpriteSize_8x8, SpriteColorFormat_256Color,
                    Sprites::numbers_gfx_sprites[number%10], -1, false, false, false, false, false);
            (*id)++;
        }

        void resetDraw()
        {
            subId = 0;
            oamClear(&oamSub, 0, 0);
        }

        void drawScore()
        {
            drawNumber(score, Constants::SCREEN_RIGHT/2 + 27, Constants::SCREEN_BOTTOM/2 - 13, &subId);

            int posX = Constants::SCREEN_RIGHT/2;
            int posY = Constants::SCREEN_BOTTOM/2;

            for(int i = 0; i < 2; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    oamSet(&oamSub, subId, posX  + 32*j - 64, posY  + 32*i - 32, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
                            Sprites::endscreen_gfx_sprites[i*4+j], -1, false, false, false, false, false);
                subId++;
                }
            }
        }

        void drawScoreOutside()
        {
            drawNumber(score, 0, 0, &subId);
        }

        void draw()
        {
            bird.drawBird(&subId);

            for(PipeCpp pipe : pipeVector)
            {
                pipe.drawPipe(&subId);
            }
        }

        void checkBirdPipeCollision()
        {
             for(unsigned int i = 0; i < pipeVector.size(); i++)
             {
                if(pipeVector[i].isOffScreen())
                {
                    pipeVector[i].setPositionInit();
                }

                if(pipeVector[i].checkCollision(&bird))
                {
                    changeState = true;
                    currentState = GameStateType::End;
                    break;
                }
                
                pipeVector[i].movePipe();
                
                if(pipeVector[i].checkScored(bird.x))
                {
                    mmEffectEx(&bgr);
                    score++;
                }
            }
        }

        void loadMusic()
        {
            mmInitDefaultMem((mm_addr)soundbank_bin);
            mmLoadEffect(SFX_COINS);
            mmLoad(MOD_FLATOUTLIES);
            mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP);
        }

        void unloadMusic()
        {
            mmUnloadEffect(SFX_COINS);
            mmUnload(MOD_FLATOUTLIES);
        }
};