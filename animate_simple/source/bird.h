#define BIRD_MAX_FRAME_ANIMATION 3

u16* bird_gfx_mem[6];
int bird_positions[4] = {38, 76, 115, 153};
int bird_position = 0;

class BirdCpp{
    public:
        s16 x;
        s16 y;
        u8 gfx_frame;

    BirdCpp(s16 x, s16 y): x(x), y(y){
        gfx_frame = 0;
    }

    void drawBird(u8* id){
        u8 value = *id;
        oamSet(&oamSub, value, x-16, y-16, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
                bird_gfx_mem[gfx_frame], -1, false, false, false, false, false);
        (*id)++;
    }

    void Animate(){

        gfx_frame++;
        
        if(gfx_frame >= BIRD_MAX_FRAME_ANIMATION)
        {
            gfx_frame = 0;
        }
    }

    void UpdatePosition(){
        y = bird_positions[bird_position];
    }
};