#pragma once

#define WIDTH  800
#define HEIGHT 600

constexpr float accel     = 5.f;
constexpr float frict     = 1.3f;
constexpr float max_speed = 10.f;
constexpr float rot_speed = 2;

#ifdef GLS_GAME
#define OLC_IMAGE_STB
#define OLC_PGE_APPLICATION
#undef GLS_GAME
#endif
#include "olcPixelGameEngine.h"

class Game : public olc::PixelGameEngine
{
private:
    struct
    {
        // Basic Positional Data
        olc::vf2d pos;
        float     rot;

        // Drifting Data
        olc::vf2d mov;
        bool      shi;

        olc::Renderable sprite;
    } _car;

    olc::Sprite _track;
    int         _track_sel;

    bool reset;

public:
    Game();

public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};
