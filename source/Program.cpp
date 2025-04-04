#include <iostream>
#include "Program.hpp"
#include "GlobalVar.hpp"

void initBackGround()
{
    font = GetFontDefault();
}

void drawBackGround()
{
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){144, 12, 63, 255});
}

Program::Program()
{
    InitWindow(screenWidth, screenHeight, "My Raylib App");
    SetTargetFPS(60);
    initBackGround();
    menu.init();
    mode = Mode::MENU;
}

void Program::run()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        eventProcessing();
        drawing();
        EndDrawing();
    }
    CloseWindow();
}

void Program::eventProcessing()
{
    if (mode == Mode::MENU)
    {
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
        drawBackGround();
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
