#include "Menu.hpp"

void Menu::init() {
    button.resize(4);
    button[0] = Button({113,300,222,368},false);
    button[1] = Button({445,300,222,368},false);
    button[2] = Button({777,300,222,368},false);
    button[3] = Button({1109,300,222,368},false);
    hotList = LoadTexture("resource\\Texture\\1List.png");
    hotTable = LoadTexture("resource\\Texture\\1Table.png");
    hotTree = LoadTexture("resource\\Texture\\1Tree.png");
    hotGraph = LoadTexture("resource\\Texture\\1Graph.png");
}

void Menu::draw() {

    if (theme == colorType::HOT) {
        DrawRectangle(0, 0, screenWidth, screenHeight, myColor1[3]);
        int fontSize = 80;
        int spacing = 10;
        Vector2 textSize = MeasureTextEx(font, title.c_str(), fontSize, spacing);
        Vector2 textPos = {
            (screenWidth - textSize.x) / 2.0f,
            (300 - textSize.y) / 2.0f
        };
        DrawTextEx(font, title.c_str(), textPos, fontSize, spacing, myColor1[0]);
        fontSize = 25;
        spacing = 5;
        textSize = MeasureTextEx(font, logo.c_str(), fontSize, spacing);
        textPos = {
            (screenWidth - textSize.x - 38) / 1.0f,
            704 + (142 - textSize.y) / 2.0f
        };
        //change logo position to right side of the screen
        DrawTextEx(font, logo.c_str(), textPos, fontSize, spacing, myColor1[0]);
    } else {
        DrawRectangle(0, 0, screenWidth, screenHeight, myColor2[3]);
        int fontSize = 80;
        int spacing = 10;
        Vector2 textSize = MeasureTextEx(font, title.c_str(), fontSize, spacing);
        Vector2 textPos = {
            (screenWidth - textSize.x) / 2.0f,
            (300 - textSize.y) / 2.0f
        };
        DrawTextEx(font, title.c_str(), textPos, fontSize, spacing, myColor2[0]);
        fontSize = 25;
        spacing = 5;
        textSize = MeasureTextEx(font, logo.c_str(), fontSize, spacing);
        textPos = {
            (screenWidth - textSize.x - 38) / 1.0f,
            704 + (142 - textSize.y) / 2.0f
        };
        //change logo position to right side of the screen
        DrawTextEx(font, logo.c_str(), textPos, fontSize, spacing, myColor2[0]);
    }

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
    int d = 0; //check if one of the button is pressed, return the index of the button for processing
    for (auto v: button) {
        if (v.checkPress(mousePosition, mousePress)) {
            return d;
        }
        d++;
    }
    return -1;
}

void Menu::update() {
    if (theme == colorType::HOT) {
        button[0].picture = hotList;
        button[1].picture = hotTable;
        button[2].picture = hotTree;
        button[3].picture = hotGraph;
    }
}