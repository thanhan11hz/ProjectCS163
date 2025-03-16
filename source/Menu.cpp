#include <vector>
#include "Menu.hpp"

void Menu::init() {
    button.resize(4);
    button[0] = Button({415,405,200,100},(Color){199,0,57,255},false,"Singly Linked List");
    button[1] = Button({825,405,200,100},(Color){199,0,57,255},false,"Hash Table");
    button[2] = Button({415,605,200,100},(Color){199,0,57,255},false,"AVL Tree");
    button[3] = Button({825,605,200,100},(Color){199,0,57,255},false, "Graph");
}

void Menu::draw() {
    float fontSize = 80;
    float spacing = 10;
    Vector2 textSize = MeasureTextEx(font, title.c_str(), fontSize, spacing);
    Vector2 textPos = {
        (screenWidth - textSize.x)/2.0f,
        (405 - textSize.y)/2.0f
    };
    DrawTextEx(font, title.c_str(), textPos, fontSize, spacing, (Color){248,222,34,255});
    for (auto v: button) {
        Vector2 mousePosition = GetMousePosition();
        v.checkHovered(mousePosition);
        v.draw();
    }
}

int Menu::modePresson() {
    bool mousePress = false;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) mousePress = true;
    Vector2 mousePosition = GetMousePosition();
    int d = 0;
    for (auto v: button) {
        if (v.checkPress(mousePosition,mousePress)) {
            return d;
        }
        d++;
    }
    return -1;
}