#include "View.hpp"

void View::CodeBlock::draw()
{
    rec = {0, 80, 400, 640};
    DrawRectangleRec(rec, PINK);
    if (codeline.size() > 30)
    {
        codeline.erase(codeline.begin(), codeline.end() - 4);
    }
    for (int i = 0; i < codeline.size(); ++i)
    {
        float fontSize = 20;
        float spacing = 5;
        Vector2 textSize = MeasureTextEx(font, codeline[i].c_str(), fontSize, spacing);
        Vector2 textPos = {
            (480 - textSize.x) / 2.0f,
            80 + 26 * i + (26 - textSize.y) / 2.0f};
        DrawTextEx(font, codeline[i].c_str(), textPos, fontSize, spacing, WHITE);
    }
}

void View::Panel::draw()
{
    rec = {0, 720, 1440, 90};
    DrawRectangleRec(rec, GREEN);
}

//
void View::Option::draw()
{
    float buttonWidth = 120;
    float buttonHeight = 40;
    float startX = 400;
    float startY = 545;

    Color buttonColor = LIGHTGRAY;
    Color textColor = DARKGRAY;

    Rectangle IniButton = {startX, startY, buttonWidth, buttonHeight};
    DrawRectangleRec(IniButton, buttonColor);
    DrawText("Initialize", IniButton.x + 10, IniButton.y + 10, 20, textColor);

    Rectangle InsertButton = {startX, startY + 45, buttonWidth, buttonHeight};
    DrawRectangleRec(InsertButton, buttonColor);
    DrawText("Insert", InsertButton.x + 10, InsertButton.y + 10, 20, textColor);

    Rectangle SearchButton = {startX, startY + 90, buttonWidth, buttonHeight};
    DrawRectangleRec(SearchButton, buttonColor);
    DrawText("Search", SearchButton.x + 10, SearchButton.y + 10, 20, textColor);

    Rectangle DeleteButton = {startX, startY + 135, buttonWidth, buttonHeight};
    DrawRectangleRec(DeleteButton, buttonColor);
    DrawText("Delete", DeleteButton.x + 10, DeleteButton.y + 10, 20, textColor);
}


bool View::Option::isInitialize()
{   
    float buttonWidth = 120;
    float buttonHeight = 40;
    float startX = 400;
    float startY = 545;
    Rectangle IniButton = {startX, startY, buttonWidth, buttonHeight};
    if (CheckCollisionPointRec(GetMousePosition(), IniButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

bool View::Option::isAdd()
{
    float buttonWidth = 120;
    float buttonHeight = 40;
    float startX = 400;
    float startY = 545;
    Rectangle InsertButton = {startX, startY + 45, buttonWidth, buttonHeight};

    if (CheckCollisionPointRec(GetMousePosition(), InsertButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}




bool View::Option::isDelete()
{
    float buttonWidth = 120;
    float buttonHeight = 40;
    float startX = 400;
    float startY = 545;
    Rectangle DeleteButton = {startX, startY + 135, buttonWidth, buttonHeight};

    if (CheckCollisionPointRec(GetMousePosition(), DeleteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

bool View::Option::isSearch()
{
    float buttonWidth = 120;
    float buttonHeight = 40;
    float startX = 400;
    float startY = 545;
    Rectangle SearchButton = {startX, startY + 90, buttonWidth, buttonHeight};

    if (CheckCollisionPointRec(GetMousePosition(), SearchButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

void View::Log::draw()
{
}

void View::initView()
{
    home.icon = LoadTexture("resource\\Texture\\home.png");
    code.codeline.clear();
    code.codeline.push_back("#include <iostream>");
    code.codeline.push_back("using namespace std");
}

void View::Home::draw()
{
    Rectangle sourceRec = {0, 0, (float)icon.width, (float)icon.height};
    Rectangle destRec = {1400, 120, 40, 40};
    Vector2 origin = {0, 0};
    DrawTexturePro(icon, sourceRec, destRec, origin, 0.0f, WHITE);
}

bool View::Home::isReturnMenu()
{
    Vector2 mousePos = GetMousePosition();
    Rectangle recTexture = {1400, 120, 40, 40};
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, recTexture))
    {
        return true;
    }
    return false;
}

void View::drawView()
{
    Rectangle title = {0, 0, 1440, 80};
    DrawRectangleRec(title, GREEN);
    std::string name;
    switch (mode)
    {
    case Mode::SLLIST:
        name = "Singly Linked List";
        break;
    case Mode::HTABLE:
        name = "Hash Table";
        break;
    case Mode::AVL:
        name = "AVL Binary Tree";
        break;
    case Mode::GRAPH:
        name = "Shortest Path";
    }
    int fontSize = 60;
    int spacing = 10;
    Vector2 textSize = MeasureTextEx(font, name.c_str(), fontSize, spacing);
    Vector2 textPos = {
        (screenWidth - textSize.x) / 2.0f,
        (80 - textSize.y) / 2.0f};
    DrawTextEx(font, name.c_str(), textPos, fontSize, spacing, WHITE);
    code.draw();
    panel.draw();
    option.draw();
    home.draw();

}

void View::eventView()
{
    if (home.isReturnMenu())
    {
        mode = Mode::MENU;
        // exit();
    }
}
