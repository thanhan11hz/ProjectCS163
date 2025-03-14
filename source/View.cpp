#include "View.hpp"

void View::CodeBlock::draw() {
    rec = {0,80,480,640};
    DrawRectangleRec(rec,PINK);
    if (codeline.size() > 30) {
        codeline.erase(codeline.begin(),codeline.end()-4);
    }
    for (int i = 0; i < codeline.size(); ++i) {
        float fontSize = 20;
        float spacing = 5;
        Vector2 textSize = MeasureTextEx(font,codeline[i].c_str(),fontSize,spacing);
        Vector2 textPos = {
            (480 - textSize.x)/2.0f,
            80 + 26*i + (26 - textSize.y)/2.0f
        };
        DrawTextEx(font,codeline[i].c_str(),textPos,fontSize,spacing,WHITE);
    }
}

void View::Panel::draw() {
    rec = {0,720,1440,90};
    DrawRectangleRec(rec,GREEN);
}

void View::Option::draw() {
    
}


bool isInitialize() {

}

bool isAdd() {

}

bool isDelete() {

}

bool isSearch() {

}

void View::Log::draw() {

}

void View::initView() {
    home.icon = LoadTexture("resource\\Texture\\home.png");
    code.codeline.clear();
    code.codeline.push_back("#include <iostream>");
    code.codeline.push_back("using namespace std");
}

void View::Home::draw() {
    Rectangle sourceRec = {0, 0, (float)icon.width, (float)icon.height};
    Rectangle destRec = {1400, 120, 40, 40}; 
    Vector2 origin = {0, 0}; 
    DrawTexturePro(icon, sourceRec, destRec, origin, 0.0f, WHITE);
}

bool View::Home::isReturnMenu() {
    Vector2 mousePos = GetMousePosition();
    Rectangle recTexture = {1400, 120, 40, 40};
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)&&CheckCollisionPointRec(mousePos,recTexture)) {
        return true;
    } return false;
}

void View::drawView() {
    Rectangle title = {0,0,1440,80};
    DrawRectangleRec(title,GREEN);
    std::string name;
    switch (mode) {
        case Mode::SLLIST: name = "Singly Linked List";
        break;
        case Mode::HTABLE: name = "Hash Table";
        break;
        case Mode::AVL: name = "AVL Binary Tree";
        break;
        case Mode::GRAPH: name = "Shortest Path";
    }
    int fontSize = 60;
    int spacing = 10;
    Vector2 textSize = MeasureTextEx(font,name.c_str(),fontSize,spacing);
    Vector2 textPos = {
        (screenWidth - textSize.x)/2.0f,
        (80 - textSize.y)/2.0f
    };
    DrawTextEx(font,name.c_str(),textPos,fontSize,spacing,WHITE);
    code.draw();
    panel.draw();
    option.draw();
    home.draw();
}

void View::eventView() {
    if (home.isReturnMenu()) {
        mode = Mode::MENU;
        exit();
    }
}
