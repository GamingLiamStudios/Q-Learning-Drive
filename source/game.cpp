#define GLS_GAME
#include "game.h"

Game::Game()
{
    sAppName = "Q-Learning-Drive";
}

bool Game::OnUserCreate()    // Called once at the start, so create things here
{
    // Init layers
    while (GetLayers().size() < 2) CreateLayer();
    EnableLayer(1, true);

    // Load Starting Positions
    int  x, y;
    auto _start_file = std::ifstream("tracks/start.txt");
    while (_start_file)
    {
        _start_file >> x;
        _start_file >> y;
        _track_start.push_back(olc::vf2d(x, y));
    }
    _start_file.close();

    // Load Map
    _track.LoadFromFile("tracks/1.png");
    _track_sel = 0;

    SetDrawTarget(1);
    DrawSprite({ 0, 0 }, &_track);
    SetDrawTarget(uint8_t(0));

    // Initalize Car
    _car.pos    = _track_start.at(_track_sel);
    _car.rot    = 0.0f;
    _car.mov    = { 0.f, 0.f };
    _car.sprite = new olc::Sprite(14, 28);    // TODO: Car Texture
    for (int i = 0; i < 14 * 28; i++) _car.sprite->GetData()[i] = olc::RED;

    reset = false;

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)    // called once per frame
{
    if (reset)
    {
        _car.pos = _track_start.at(_track_sel);
        _car.rot = 0.0f;
        _car.mov = { 0.f, 0.f };

        reset = false;
    }

    // Accept User Input
    reset    = GetKey(olc::Key::ESCAPE).bPressed;
    _car.shi = GetKey(olc::Key::SHIFT).bHeld;
    if (GetKey(olc::Key::EQUALS).bPressed)
    {
        _track_sel++;
        if (_track_sel >= 3) _track_sel--;    // Magic Number
        _track.LoadFromFile("tracks/" + std::to_string(_track_sel + 1) + ".png");

        SetDrawTarget(1);
        DrawSprite({ 0, 0 }, &_track);
        SetDrawTarget(uint8_t(0));

        reset = true;
    }
    if (GetKey(olc::Key::MINUS).bPressed)
    {
        _track_sel--;
        if (_track_sel < 0) _track_sel++;
        _track.LoadFromFile("tracks/" + std::to_string(_track_sel + 1) + ".png");

        SetDrawTarget(1);
        DrawSprite({ 0, 0 }, &_track);
        SetDrawTarget(uint8_t(0));

        reset = true;
    }

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
        _car.mov *= 1.f - (3.f * fElapsedTime);    // Magic Number

    if (GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld)
        _car.mov -= accel * fElapsedTime * olc::vf2d(sin(-_car.rot), cos(_car.rot));
    if (GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld)
        _car.mov += accel * fElapsedTime * olc::vf2d(sin(-_car.rot), cos(_car.rot));

    // Stop car from accelerating too much
    _car.mov.x = std::clamp(_car.mov.x, -max_speed, max_speed);
    _car.mov.y = std::clamp(_car.mov.y, -max_speed, max_speed);

    // Move car
    _car.pos += _car.mov;

    // Render
    Clear(olc::BLANK);

    _car.transform.Reset();
    _car.transform.Translate(-7, -14);
    _car.transform.Rotate(-_car.rot);
    _car.transform.Translate(_car.pos.x, _car.pos.y);

    olc::GFX2D::DrawSprite(_car.sprite, _car.transform);

    // Colision detection
    for (int x = _car.pos.x - 14; x < _car.pos.x + 14; x++)    // Magic Number
    {
        for (int y = _car.pos.y - 14; y < _car.pos.y + 14; y++)
            if (_track.GetPixel(x, y) == olc::BLACK && GetDrawTarget()->GetPixel(x, y) == olc::RED)
            {
                reset = true;
                break;
            }
        if (reset) break;
    }

    return true;
}
