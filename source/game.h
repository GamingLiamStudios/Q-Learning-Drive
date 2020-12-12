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
#define OLC_PGEX_GRAPHICS2D
#undef GLS_GAME
#endif
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

#include <fstream>

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

        // Render
        olc::Sprite *           sprite;
        olc::GFX2D::Transform2D transform;
    } _car;

    olc::Sprite            _track;
    int                    _track_sel;
    std::vector<olc::vf2d> _track_start;

    bool reset;

public:
    Game();

public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};
