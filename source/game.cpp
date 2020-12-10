#define GLS_GAME
#include "game.h"

Game::Game()
{
    sAppName = "Q-Learning";
}

bool Game::OnUserCreate()    // Called once at the start, so create things here
{
    // Initalize Car
    _car.pos = { WIDTH / 2, HEIGHT / 2 };
    _car.rot = 0.0f;
    _car.mov = { 0.f, 0.f };
    _car.sprite.Create(14, 28);    // TODO: Car Texture
    for (int i = 0; i < 14 * 28; i++) _car.sprite.Sprite()->GetData()[i] = olc::WHITE;
    _car.sprite.Decal()->Update();

    // Load Map
    //_track = olc::Renderable::Load("track.png");

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)    // called once per frame
{
    std::cout << std::to_string(fElapsedTime) << "\n";
    // Accept User Input
    if (GetKey(olc::Key::ESCAPE).bPressed)
    {
        _car.pos = { WIDTH / 2, HEIGHT / 2 };
        _car.rot = 0.0f;
        _car.mov = { 0.f, 0.f };
    }
    _car.shi = GetKey(olc::Key::SHIFT).bHeld;

    if (GetKey(olc::Key::LEFT).bHeld) _car.rot -= rot_speed * fElapsedTime;
    if (GetKey(olc::Key::RIGHT).bHeld) _car.rot += rot_speed * fElapsedTime;

    // Practically does the exact same as 'rot %= 360'
    _car.rot /= 360.f;
    _car.rot -= (int) _car.rot;
    _car.rot *= 360.f;

    // Decelerate car to simulate friction
    if (_car.shi)
        _car.mov *= 1.f - (frict * fElapsedTime);
    else
        _car.mov *= 1.f - (3.f * fElapsedTime);

    if (GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld)
        _car.mov -= accel * fElapsedTime * olc::vf2d(sin(-_car.rot), cos(_car.rot));
    if (GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld)
        _car.mov += accel * fElapsedTime * olc::vf2d(sin(-_car.rot), cos(_car.rot));

    // Stop car from accelerating too much
    _car.mov.x = std::clamp(_car.mov.x, -max_speed, max_speed);
    _car.mov.y = std::clamp(_car.mov.y, -max_speed, max_speed);

    // Move car
    _car.pos += _car.mov;

    DrawRotatedDecal(_car.pos, _car.sprite.Decal(), _car.rot, { 7, 14 });    // Render car
    return true;
}
