#include "View.hpp"

// =========================== CODE BLOCK ========================================

void View::CodeBlock::draw()
{
    rec = {0, 80, 400, 320};
    DrawRectangleRec(rec, (Color){199,0,57,255});
    Vector2 textSize = MeasureTextEx(font, "Code Block", 20, 5);
    Vector2 textPos = {
        (400 - textSize.x) / 2.0f,
        80 + (26*2 - textSize.y) / 2.0f};
    DrawTextEx(font, "Code Block", textPos, 20, 5, (Color){248,222,34,255}); 
    if (codeline.empty()) return;       
    int startLine = scrollOffset/lineHeight;

    for (int i = 0; i < visibleLines && (startLine + i < codeline.size()); ++i)
    {
        Color color = WHITE;
        float fontSize;
        if (startLine + i == lineHighlighted) color = BLACK;
        else fontSize = 20;
        float spacing = 5;
        Vector2 textSize = MeasureTextEx(font, codeline[startLine + i].c_str(), fontSize, spacing);
        Vector2 textPos = {
            (400 - textSize.x) / 2.0f,
            132 + 26 * i + (26 - textSize.y) / 2.0f};
        DrawTextEx(font, codeline[startLine + i].c_str(), textPos, fontSize, spacing, color);
    }
    float scrollHeight = (visibleLines/(float)codeline.size())*rec.height;
    float scrollY = rec.y + scrollOffset/(float) (codeline.size() - visibleLines)*lineHeight;
}

void View::CodeBlock::update() {
    float mouseWheel = GetMouseWheelMove();
    if (mouseWheel != 0) {
        scrollOffset -= mouseWheel*lineHeight;
        scrollOffset = std::max(0.0f,std::min(scrollOffset,(float)(codeline.size() - visibleLines)*lineHeight));
    }
}

//=================================== PANEL ==========================================================

void View::Panel::draw()
{
    
    DrawRectangleRec(rec, (Color){144,12,63,255});
    // Draw Rewind button
    Rectangle rewindButton = {startX, startY, buttonSize, buttonSize};
    DrawTexturePro(Rewind, {0, 0, (float)Rewind.width, (float)Rewind.height}, rewindButton, {0, 0}, 0.0f, LIGHTGRAY);

    // Draw Play/Pause button
    Rectangle playPauseButton = {startX + buttonSize + spacing, startY, buttonSize, buttonSize};
    if (isPlaying) {
        DrawTexturePro(Pause, {0, 0, (float)Pause.width, (float)Pause.height}, playPauseButton, {0, 0}, 0.0f, LIGHTGRAY);
    } else {
        DrawTexturePro(Play, {0, 0, (float)Play.width, (float)Play.height}, playPauseButton, {0, 0}, 0.0f, LIGHTGRAY);
    }

    // Draw Forward button
    Rectangle forwardButton = {startX + 2 * (buttonSize + spacing), startY, buttonSize, buttonSize};
    DrawTexturePro(Forward, {0, 0, (float)Forward.width, (float)Forward.height}, forwardButton, {0, 0}, 0.0f, LIGHTGRAY); 

    // if (CheckCollisionPointRec(GetMousePosition(), playPauseButton) && IsKeyPressed(MOUSE_LEFT_BUTTON)){
    //     isPlaying = !isPlaying;
    // }
}

void View::Panel::update() {
    if (isPlayPressed()) {
        isPlaying = true; 
    } else if (isPausePressed()) {
        isPlaying = false; 
    }
}

bool View::Panel::isRewindPressed(){
    Rectangle rewindButton = {startX, startY, buttonSize, buttonSize};
    return (CheckCollisionPointRec(GetMousePosition(), rewindButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}

bool View::Panel::isPlayPressed(){
    Rectangle playPauseButton = {startX + buttonSize + spacing, startY, buttonSize, buttonSize};
    return CheckCollisionPointRec(GetMousePosition(), playPauseButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !isPlaying;
}

bool View::Panel::isPausePressed(){
    Rectangle playPauseButton = {startX + buttonSize + spacing, startY, buttonSize, buttonSize};
    return CheckCollisionPointRec(GetMousePosition(), playPauseButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPlaying;
}

bool View::Panel::isForwardPressed(){
    Rectangle forwardButton = {startX + 2 * (buttonSize + spacing), startY, buttonSize, buttonSize};
    return CheckCollisionPointRec(GetMousePosition(), forwardButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

//================================= OPTION ===============================================

void View::Option::draw()
{
    float buttonWidth = 120;
    float buttonHeight = 40;
    float startX = 40;
    float startY = 574;

    DrawRectangle(0,500,400,220,(Color){199,0,57,255});
    Vector2 textSize = MeasureTextEx(font,"Option",20,5);
    Vector2 textPos = {
        (400 - textSize.x)/2.0f,
        500 + (52 - textSize.y)/2.0f
    };
    DrawTextEx(font,"Option",textPos,20,5,(Color){248,222,34,255});
    Color buttonColor = LIGHTGRAY;
    Color textColor = DARKGRAY;

    Rectangle IniButton = {startX, startY, buttonWidth, buttonHeight};
    DrawRectangleRec(IniButton, buttonColor);
    DrawText("Initialize", IniButton.x + 10, IniButton.y + 10, 20, textColor);

    Rectangle InsertButton = {startX, startY + 62, buttonWidth, buttonHeight};
    DrawRectangleRec(InsertButton, buttonColor);
    DrawText("Insert", InsertButton.x + 10, InsertButton.y + 10, 20, textColor);

    Rectangle SearchButton = {startX + 200, startY, buttonWidth, buttonHeight};
    DrawRectangleRec(SearchButton, buttonColor);
    DrawText("Search", SearchButton.x + 10, SearchButton.y + 10, 20, textColor);

    Rectangle DeleteButton = {startX + 200, startY + 62, buttonWidth, buttonHeight};
    DrawRectangleRec(DeleteButton, buttonColor);
    DrawText("Delete", DeleteButton.x + 10, DeleteButton.y + 10, 20, textColor);
}

bool View::Option::isInitialize()
{   
    float buttonWidth = 120;
    float buttonHeight = 40;
    float startX = 40;
    float startY = 574;
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
    float startX = 40;
    float startY = 574;
    Rectangle InsertButton = {startX, startY + 62, buttonWidth, buttonHeight};

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
    float startX = 40;
    float startY = 574;
    Rectangle DeleteButton = {startX + 200, startY + 0, buttonWidth, buttonHeight};

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
    float startX = 40;
    float startY = 574;
    Rectangle SearchButton = {startX + 200, startY + 62, buttonWidth, buttonHeight};

    if (CheckCollisionPointRec(GetMousePosition(), SearchButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

// Text Box

void View::TextBox::draw(){
    if (!isOpen) return;
    if (isOpen) {
        Rectangle textBoxRec = {0, 500, 400, 220};
        DrawRectangleRec(textBoxRec, LIGHTGRAY);
        DrawRectangleLinesEx(textBoxRec, 2, DARKGRAY);

        Rectangle closeButton = {textBoxRec.x + textBoxRec.width - 30, textBoxRec.y + 10, 20, 20};
        DrawRectangleRec(closeButton, RED);
        DrawText("X", closeButton.x + 5, closeButton.y + 2, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            isOpen = false;
            value.clear();
            enteredValues = false;
            enteredPrime = false;
        }
        
        std::string inputText;
    if (mode == Mode::HTABLE) {
        if (!enteredValues) {
            inputText = "Enter values: " + value;
        } else if (!enteredPrime) {
            inputText = "Enter prime number: " + value;
        }
    } else {
        inputText = "Enter value: " + value;
    }
    DrawText(inputText.c_str(), textBoxRec.x + 10, textBoxRec.y + 40, 20, DARKGRAY);
    } 
    
}

void View::TextBox::update() {
    if (isOpen) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= '0' && key <= '9') || key == ' ') {
                value += static_cast<char>(key);
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !value.empty()) {
            value.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (mode == Mode::HTABLE) {
                if (!enteredValues) {
                    std::vector<int> numbers;
                    std::stringstream ss(value);
                    std::string token;
                    while (ss >> token) {
                        try {
                            int num = std::stoi(token);
                            numbers.push_back(num);
                        } catch (const std::invalid_argument&) {
                        }
                    }
                    someList.insert(someList.end(), numbers.begin(), numbers.end());
                    value.clear();
                    enteredValues = true;
                    if (enteredPrime){
                        isOpen = false;
                    }
                } else if (!enteredPrime) {
                    try {
                        primeNumber = std::stoi(value);
                        enteredPrime = true;
                        value.clear();
                        isOpen = false;
                    } catch (const std::invalid_argument&) {
                    }
                }
            } else {
                std::vector<int> numbers;
                std::stringstream ss(value);
                std::string token;
                while (ss >> token) {
                    try {
                        int num = std::stoi(token);
                        someList.push_back(num);
                    } catch (const std::invalid_argument&) {
                    }
                }
                value.clear();
                isOpen = false;
            }
        }
    }
}

// Log

void View::Log::draw()
{
    rec = {0, 400, 400, 100};
    DrawRectangleRec(rec, (Color){249,76,16,255}); 
    Vector2 textSize = MeasureTextEx(font, "Log", 20, 5);
    Vector2 textPos = {
        (400 - textSize.x) / 2.0f,
        400 + (26*2 - textSize.y) / 2.0f};
    DrawTextEx(font, "Log", textPos, 20, 5, (Color){248,222,34,255});
    if (infor.size() > 10)
    {
        infor.erase(infor.begin(), infor.end() - 11);
    }
    for (int i = 0; i < infor.size(); ++i)
    {
        float fontSize = 20;
        float spacing = 5;
        Vector2 textSize = MeasureTextEx(font, infor[i].c_str(), fontSize, spacing);
        Vector2 textPos = {
            (400 - textSize.x) / 2.0f,
            452 + 26 * i + (26 - textSize.y) / 2.0f};
        DrawTextEx(font, infor[i].c_str(), textPos, fontSize, spacing, WHITE);
    }
}

// ======================================== SLIDER ======================================================

void View::Slider::update() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos,bound)) {
        isDragging = true;
    }
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        isDragging = false;
    }
    if (isDragging) {
        float value = min + (mousePos.x - bound.x)/bound.width*(max - min);
        speed = std::max(min,std::min(max,value));
    }
}

void View::Slider::draw() {
    float roundness = 1.0f; 
    int segments = 10; 
    DrawRectangleRounded(bound, roundness, segments, GRAY);
    float buttonX = bound.x + (speed - min)/(max - min)*bound.width;
    DrawCircle(buttonX,bound.y + bound.height/2.0f, bound.height*3/4.0f, BLACK);
    Vector2 textSize = MeasureTextEx(font, "Speed: ", 20, 5);
    Vector2 textPos = {
        50 + (150 - textSize.x) / 2.0f,
        720 + (45 - textSize.y) / 2.0f
    };
    DrawTextEx(font, "Speed: ", textPos, 20, 5, (Color){248,222,34,255});
    textSize = MeasureTextEx(font, std::to_string(speed).c_str(), 20, 5);
    textPos = {
        200 + (150 - textSize.x) / 2.0f,
        720 + (45 - textSize.y) / 2.0f
    };
    DrawTextEx(font, std::to_string(speed).c_str(), textPos, 20, 5, (Color){248,222,34,255});
}       

//=========================================== HOME =======================================

void View::Home::draw()
{
    Rectangle sourceRec = {0, 0, (float)icon.width, (float)icon.height};
    Rectangle destRec = {1380, 20, 40, 40};
    Vector2 origin = {0, 0};
    DrawTexturePro(icon, sourceRec, destRec, origin, 0.0f, (Color){144,12,63,255});
}

bool View::Home::isReturnMenu()
{
    Vector2 mousePos = GetMousePosition();
    Rectangle recTexture = {1380, 20, 40, 40};
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, recTexture))
    {
        return true;
    }
    return false;
}

//=================================== INPUT PANEL ======================================

void View::InputPanel::draw() {
    if (!isOpen) return;
    
    DrawRectangleRec(panelRec, (Color){199,0,57,255});
    Vector2 textSize = MeasureTextEx(font, "Input Method", 20, 5);
    Vector2 textPos = {
        (panelRec.width - textSize.x) / 2.0f,
        panelRec.y + (52 - textSize.y) / 2.0f
    };
    DrawTextEx(font, "Input Method", textPos, 20, 5, (Color){248,222,34,255});

    // Draw buttons
    Color buttonColor = LIGHTGRAY;
    Color textColor = DARKGRAY;

    DrawRectangleRec(textboxButton, buttonColor);
    DrawText("Textbox", textboxButton.x + 10, textboxButton.y + 10, 20, textColor);

    DrawRectangleRec(fileButton, buttonColor);
    DrawText("File", fileButton.x + 10, fileButton.y + 10, 20, textColor);

    DrawRectangleRec(urlButton, buttonColor);
    DrawText("URL", urlButton.x + 10, urlButton.y + 10, 20, textColor);

    DrawRectangleRec(closeButton, buttonColor);
    DrawText("Close", closeButton.x + 10, closeButton.y + 10, 20, textColor);
}

bool View::InputPanel::isTextboxPressed() {
    return CheckCollisionPointRec(GetMousePosition(), textboxButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool View::InputPanel::isFilePressed() {
    return CheckCollisionPointRec(GetMousePosition(), fileButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool View::InputPanel::isURLPressed() {
    return CheckCollisionPointRec(GetMousePosition(), urlButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool View::InputPanel::isClosePressed() {
    return CheckCollisionPointRec(GetMousePosition(), closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

//=================================== VIEW ======================================

void View::initView()
{
    if (mode == Mode::SLLIST || mode == Mode::HTABLE || mode == Mode::AVL){
        if (option.isInitialize()) {
            inputPanel.isOpen = true;
            box.isOpen = false; // Đảm bảo textbox đóng khi mở inputPanel
        }
    }

    func = Function::NONE;
    home.icon = LoadTexture("resource\\Texture\\home.png");
    code.codeline.clear();
    panel.Play = LoadTexture("resource\\Texture\\Play.png");
    panel.Pause = LoadTexture("resource\\Texture\\Pause.png");
    panel.Rewind = LoadTexture("resource\\Texture\\Rewind.png");
    panel.Forward = LoadTexture("resource\\Texture\\Forward.png");
    panel.Final = LoadTexture("resource\\Texture\\Final.png");
}


void View::drawView()
{
    Rectangle title = {0, 0, 1440, 80};
    DrawRectangleRec(title, (Color){144,12,63,255} );
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
    DrawTextEx(font, name.c_str(), textPos, fontSize, spacing, (Color){248,222,34,255});
    code.draw();
    panel.draw();
    option.draw();
    home.draw();
    log.draw();

    if (inputPanel.isOpen){
        inputPanel.draw();
    }

    else if (box.isOpen){
        box.draw();
    }

    slider.draw();
}

void View::eventView() {

    if (home.isReturnMenu()) {
        mode = Mode::MENU;
        exit();
    }

    if (inputPanel.isOpen){
        if (inputPanel.isTextboxPressed()){
            inputPanel.isOpen = false;
            box.isOpen = true;
            box.isTextboxMode = true;
        }
        else if (inputPanel.isFilePressed()){
            inputPanel.isOpen = false;
            box.isOpen = true;
            box.isFileMode = true;
        }
        else if (inputPanel.isURLPressed()){
            inputPanel.isOpen = false;
            box.isOpen = true;
            box.isURLMode = true;
        }
        else if (inputPanel.isClosePressed()){
            inputPanel.isOpen = false;
        }
    }

    else {
        if (option.isInitialize()){
            box.isOpen = false;
            inputPanel.isOpen = true;
        }
        else if (option.isAdd() || option.isDelete() || option.isSearch()){
            box.isOpen = true;
            inputPanel.isOpen = false;
        }
    }

    box.update();
    slider.update();
    code.update();
   
}

