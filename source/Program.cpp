#include "Program.hpp"

Program::Program()
{
    InitWindow(screenWidth, screenHeight, "My Raylib App");
    SetTargetFPS(60);
    menu.init();
    mode = Mode::MENU;
    font = GetFontDefault();
    InitAudioDevice();
    music = LoadMusicStream("resource\\Music\\VietTiepCauChuyenHoaBinh-NguyenVanChungNguyenDuyenQuynh-12664541.mp3");
    myFont[0] = GetFontDefault();
    myFont[1] = LoadFont("resource\\Font\\Roboto-Bold.ttf");
    myFont[2] = LoadFont("resource\\Font\\Consolas-Bold.ttf");
    myFont[3] = LoadFont("resource\\Font\\Inter-Bold.ttf");
    myFont[4] = LoadFont("resource\\Font\\OpenSans-Bold.ttf");
}

void Program::run()
{
    PlayMusicStream(music);
    SetMusicVolume(music, 0.0f);
    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        eventProcessing();
        drawing();
        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
}

void Program::eventProcessing()
{
    if (mode == Mode::MENU)
    {
        menu.update();
        int x = menu.modePresson();
        if (x == 0)
        {
            mode = Mode::SLLIST;
            list.init();
        }
        else if (x == 1)
        {
            mode = Mode::HTABLE;
            table.init();
        }
        else if (x == 2)
        {
            mode = Mode::AVL;
            tree.init();
        }
        else if (x == 3)
        {
            mode = Mode::GRAPH;
            graph.init();
        }
    }
    if (mode == Mode::SLLIST)
    {
        list.run();
    }
    if (mode == Mode::HTABLE)
    {
        table.run();
    }
    if (mode == Mode::AVL)
    {
        tree.run();
    }
    if (mode == Mode::GRAPH)
    {
        graph.run();
    }
}

void Program::drawing()
{
    if (mode == Mode::MENU)
    {
        menu.draw();
    }
    else if (mode == Mode::SLLIST)
    {
        list.draw();
    }
    else if (mode == Mode::HTABLE)
    {
        table.draw();
    }
    else if (mode == Mode::AVL)
    {
        tree.draw();
    }
    else if (mode == Mode::GRAPH)
    {
        graph.draw();
    }
}
