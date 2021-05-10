#define PIPE_HOLE_DISTANCE 16
#define SPRITE_SIZE 32

u8 pipe_speed = 1;
u16* pipe_gfx_sprites[2];

class PipeCpp{
    public:
        s16 x;
        s16 y;
    
    PipeCpp(){
        x = 256+SPRITE_SIZE/2;
        y = 115;
    }

    enum PipeState { PIPE_MIDDLE = 0, PIPE_CAP = 1};


    void drawPipe(u8* id)
    {
        s16 pipePositionXPivotCenter = x - SPRITE_SIZE / 2;
        s16 pipePositionYPivotDown = y - SPRITE_SIZE;
        s16 pipePositionYPivotUp = y;

        oamSet(&oamSub, *id, pipePositionXPivotCenter, pipePositionYPivotDown - PIPE_HOLE_DISTANCE,
                0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
                pipe_gfx_sprites[PIPE_CAP], -1, false, false, false, true, false);
        (*id)++;
        
        pipePositionYPivotDown -= SPRITE_SIZE;

        while (pipePositionYPivotDown > 0)
        {
            oamSet(&oamSub, *id, pipePositionXPivotCenter, pipePositionYPivotDown - PIPE_HOLE_DISTANCE,
                    0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
                    pipe_gfx_sprites[PIPE_MIDDLE], -1, false, false, false, false, false);
            (*id)++;
            pipePositionYPivotDown -= SPRITE_SIZE;
        }
        
        oamSet(&oamSub, *id, pipePositionXPivotCenter, pipePositionYPivotUp + PIPE_HOLE_DISTANCE,
                0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
                pipe_gfx_sprites[PIPE_CAP], -1, false, false, false, false, false);
        (*id)++;

        pipePositionYPivotUp += SPRITE_SIZE;

        while(pipePositionYPivotUp < 192){
            oamSet(&oamSub, *id, pipePositionXPivotCenter, pipePositionYPivotUp + PIPE_HOLE_DISTANCE,
                    0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
                    pipe_gfx_sprites[PIPE_MIDDLE], -1, false, false, false, false, false);
            (*id)++;
            pipePositionYPivotUp += SPRITE_SIZE;
        }
    }

    void movePipe(){
        if(x < -(SPRITE_SIZE/2)){
            x = 256 + SPRITE_SIZE/2;
        }else{
            x -= pipe_speed;
        }
    }
};