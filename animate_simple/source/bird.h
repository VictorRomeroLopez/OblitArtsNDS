#define BIRD_MAX_FRAME_ANIMATION 3

u16* Sprites::bird_gfx_mem[6];
u8 bird_positions[4] = {38, 76, 115, 153};

class BirdCpp
{
    public:
        s16 x;
        s16 y;
        u8 gfx_frame;
        u8 bird_position;

        BirdCpp()
        {
            x = 0;
            y = 0;
            gfx_frame = 0;
            bird_position = 0;
        }

        BirdCpp(s16 x): x(x)
        {
            bird_position = 0;
            updatePosition();
            gfx_frame = 0;
        }

        void drawBird(u8* id)
        {
            u8 value = *id;
            oamSet(&oamSub, value, x-16, y-16, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
                    Sprites::bird_gfx_mem[gfx_frame], -1, false, false, false, false, false);
            (*id)++;
        }

        void animate()
        {
            gfx_frame++;

            if(gfx_frame >= BIRD_MAX_FRAME_ANIMATION)
            {
                gfx_frame = 0;
            }
        }

        void updatePosition()
        {
            y = bird_positions[bird_position];
        }

        int numPositions()
        {
            return sizeof(bird_positions)/sizeof(bird_positions[0]);
        }

        void moveUp()
        {
            if(bird_position == 0)
            {
                return;
            }
            bird_position--;
        }

        void moveDown()
        {
            if(bird_position == numPositions()-1)
            {
                return;
            }
            bird_position++;
        }
};