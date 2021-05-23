#include <nds.h>

#define PIPE_HOLE_DISTANCE 17
#define SPRITE_SIZE 32

u16* Sprites::pipe_gfx_sprites[2];
u8 pipe_speed = 1;

class PipeCpp
{
    public:
        s16 x;
        s16 y;
        bool counted;

    PipeCpp()
    {
        x = 0;
        y = 0;
        counted = false;
    }

    PipeCpp(s16 x, s16 y) : x(x), y(y)
    {
        counted = false;
    }

    enum PipeState { PIPE_MIDDLE = 0, PIPE_CAP = 1};

    void drawPipe(u8* id)
    {
        s16 pipePositionXPivotCenter = x - SPRITE_SIZE / 2;
        s16 pipePositionYPivotDown = y - SPRITE_SIZE;
        s16 pipePositionYPivotUp = y;

        oamSet(&oamSub, *id, pipePositionXPivotCenter, pipePositionYPivotDown - PIPE_HOLE_DISTANCE,
                0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
                Sprites::pipe_gfx_sprites[PIPE_CAP], -1, false, false, false, true, false);
        (*id)++;
        
        pipePositionYPivotDown -= SPRITE_SIZE;

        while (pipePositionYPivotDown + PIPE_HOLE_DISTANCE > 0)
        {
            oamSet(&oamSub, *id, pipePositionXPivotCenter, pipePositionYPivotDown - PIPE_HOLE_DISTANCE,
                    0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
                    Sprites::pipe_gfx_sprites[PIPE_MIDDLE], -1, false, false, false, false, false);
            (*id)++;
            pipePositionYPivotDown -= SPRITE_SIZE;
        }
        
        oamSet(&oamSub, *id, pipePositionXPivotCenter, pipePositionYPivotUp + PIPE_HOLE_DISTANCE,
                0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
                Sprites::pipe_gfx_sprites[PIPE_CAP], -1, false, false, false, false, false);
        (*id)++;

        pipePositionYPivotUp += SPRITE_SIZE;

        while(pipePositionYPivotUp < Constants::SCREEN_BOTTOM)
        {
            oamSet(&oamSub, *id, pipePositionXPivotCenter, pipePositionYPivotUp + PIPE_HOLE_DISTANCE,
                    0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
                    Sprites::pipe_gfx_sprites[PIPE_MIDDLE], -1, false, false, false, false, false);
            (*id)++;
            pipePositionYPivotUp += SPRITE_SIZE;
        }
    }

    bool isOffScreen()
    {
        return x < -(SPRITE_SIZE/2);
    }

    void movePipe()
    {
        x -= pipe_speed;
    }

    bool checkScored(u8 birdPositionX)
    {
        if(!counted && x < birdPositionX-32)
        {
            counted = true;
            return true;
        }
        
        return false;
    }

    void setPositionInit()
    {
        counted = false;
        x += Constants::SCREEN_RIGHT + SPRITE_SIZE;
        y = bird_positions[rand() % 4];
    }

    void setPosition(s16 xPos, s16 yPos)
    {
        x = xPos;
        y = yPos;
    }

    bool checkCollision(BirdCpp* bird)
    {
        return bird->x - 8 < x + SPRITE_SIZE/2 - 4 &&
        bird->x + 8 > x-SPRITE_SIZE/2 + 4 &&
        (bird->y - 6 < y - PIPE_HOLE_DISTANCE ||
        bird->y - 6 > y + PIPE_HOLE_DISTANCE);
    }
};