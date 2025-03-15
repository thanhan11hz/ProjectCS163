#include "View.hpp"
#include <sstream>

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

void View::TextBox::draw(){
    if (!isOpen) return;

    Rectangle textBoxRec = {500, 300, 400, 100};
    DrawRectangleRec(textBoxRec, LIGHTGRAY);
    DrawRectangleLinesEx(textBoxRec, 2, DARKGRAY);

    Rectangle closeButton = {textBoxRec.x + textBoxRec.width - 30, textBoxRec.y + 10, 20, 20};
    DrawRectangleRec(closeButton, RED);
    DrawText("X", closeButton.x + 5, closeButton.y + 2, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        isOpen = false;
        value.clear();
    }
    std::string inputText = "Enter value: " + value;

    // Display value entered from keyboard on textBox
    DrawText(inputText.c_str(), textBoxRec.x + 10, textBoxRec.y + 40, 20, DARKGRAY);
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
    // biến isOpen, khi true thì hiện textbox, khi nhấn dấu x thì trả về false và tắt textbox
    if (option.isAdd() || option.isDelete() || option.isInitialize() || option.isSearch()){
        box.isOpen = true;
    }

    if (box.isOpen){
        box.draw();
    }
}

void View::eventView() {
    // Xử lý sự kiện trở về menu
    if (home.isReturnMenu()) {
        mode = Mode::MENU;
        // exit();
    }

    // Handle entering value from Textbox
    if (box.isOpen) {
        // Handle entering value from keyboard
        int key = GetCharPressed();
        while (key > 0) {
            // Allow numbers and space only
            if ((key >= '0' && key <= '9') || key == ' ') {
                box.value += static_cast<char>(key); // Add char to string
            }
            key = GetCharPressed();
        }

        // Xử lý xóa ký tự nếu nhấn phím Backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!box.value.empty()) {
                box.value.pop_back(); // Xóa ký tự cuối cùng
            }
        }

        // Xử lý khi nhấn phím Enter
        if (IsKeyPressed(KEY_ENTER)) {
            std::vector<int> numbers; 
            std::stringstream ss(box.value); 
            std::string token;

            // Tách chuỗi dựa trên dấu cách
            while (ss >> token) {
                try {
                    int num = std::stoi(token); // Chuyển đổi từ chuỗi sang số
                    numbers.push_back(num);    // Thêm số vào danh sách
                } catch (const std::invalid_argument&) {
                    // Bỏ qua nếu không thể chuyển đổi thành số
                }
            }

            // Thêm danh sách số vào someList
            someList.insert(someList.end(), numbers.begin(), numbers.end());

            // Xóa giá trị trong TextBox sau khi xử lý
            box.value.clear();
            box.isOpen = false;
        }
    }
}
