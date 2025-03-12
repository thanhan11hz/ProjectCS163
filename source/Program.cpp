#include <iostream>
#include "Program.hpp"
#include "GlobalVar.hpp"

void initBackGround() {

}

void drawBackGround() {
    DrawRectangle(0,0,screenWidth,screenHeight,PINK);
}

Program::Program() {
    InitWindow(screenWidth,screenHeight,"My Raylib App");
    SetTargetFPS(60);
    initBackGround();
    mode = Mode::MENU;
}

void Program::run() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        eventProcessing();
        drawing();
        EndDrawing();
    }
    CloseWindow();
}

void Program::eventProcessing() {
    if (mode == Mode::MENU) {
        int x;
        if (x == 0) {
            mode = Mode::SLLIST;
            list.init();
        } else if (x == 1) {
            mode = Mode::HTABLE;
            table.init();
        } else if (x == 2) {
            mode = Mode::AVL;
            tree.init();
        } else if (x == 3) {
            mode = Mode::GRAPH;
            graph.init();
        }
    }
}

void Program::drawing() {
    if (mode == Mode::MENU) {
        drawBackGround();
        menu.draw();
    } else if (mode == Mode::SLLIST) {
        list.draw();
    } else if (mode == Mode::HTABLE) {
        table.draw();
    } else if (mode == Mode::AVL) {
        tree.draw();
    } else if (mode == Mode::GRAPH) {
        graph.draw();
    }
}


