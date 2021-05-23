#include <nds.h>

class Sprites{
    public:
        static u16* endscreen_gfx_sprites[8];
        static u16* numbers_gfx_sprites[10];
        static u16* pipe_gfx_sprites[2];
        static u16* bird_gfx_mem[6];

        static void unloadSprites()
        {
            int i;

            for(i =0; i < 2; i++){
                oamFreeGfx(&oamSub, endscreen_gfx_sprites[i]);
                oamFreeGfx(&oamSub, numbers_gfx_sprites[i]);
                oamFreeGfx(&oamSub, pipe_gfx_sprites[i]);
                oamFreeGfx(&oamSub, bird_gfx_mem[i]);
            }

            for(i = 2; i < 6; i++){
                oamFreeGfx(&oamSub, endscreen_gfx_sprites[i]);
                oamFreeGfx(&oamSub, numbers_gfx_sprites[i]);
                oamFreeGfx(&oamSub, bird_gfx_mem[i]);
            }

            for(i = 6; i < 8; i++){
                oamFreeGfx(&oamSub, endscreen_gfx_sprites[i]);
                oamFreeGfx(&oamSub, numbers_gfx_sprites[i]);
            }
            
            for(i = 8; i < 10; i++){
                oamFreeGfx(&oamSub, numbers_gfx_sprites[i]);
            }
        }

        static void initEndWindowSprites(int gfxoffset, u8* gfx)
        {
            gfx += gfxoffset;

            for(int i = 0; i < 8; i++)
            {
                endscreen_gfx_sprites[i] = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
                dmaCopy(gfx, endscreen_gfx_sprites[i], 32*32);
                gfx += 32*32;
            }
        }

        static void initNumbers(int gfxoffset, u8* gfx)
        {
            gfx += gfxoffset;

            for(int i = 0; i < 10; i++)
            {
                numbers_gfx_sprites[i] = oamAllocateGfx(&oamSub, SpriteSize_8x8, 
                SpriteColorFormat_256Color);
                dmaCopy(gfx, numbers_gfx_sprites[i], 8*8);
                gfx += 8*8;
            }
        }

        static void initPipeSprites(int gfxoffset, u8* gfx)
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

        static void initBirdSprites(int gfxoffset, u8* gfx)
        {
            gfx += gfxoffset;

            for(int i = 0; i < 3; i++)
            {
                bird_gfx_mem[i] = oamAllocateGfx(&oamSub, 
                SpriteSize_32x32, SpriteColorFormat_256Color);
                dmaCopy(gfx, bird_gfx_mem[i], 32*32);
                gfx += 32*32;
            }
        }
};