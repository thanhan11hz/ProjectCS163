#include "View.hpp"

// =========================== CODE BLOCK ========================================

void View::CodeBlock::draw()
{
    rec = {0, 90, 400, 300};
    if (theme == colorType::HOT)
    {
        DrawRectangleRounded(rec, 0.3f, 10, myColor1[2]);
        DrawRectangleRoundedLinesEx(rec, 0.3f, 10, 3, myColor1[0]);
        DrawLineEx({0, 130}, {400, 130}, 3, myColor1[0]);
    }
    else
    {
        DrawRectangleRounded(rec, 0.3f, 10, myColor2[2]);
        DrawRectangleRoundedLinesEx(rec, 0.3f, 10, 3, myColor2[0]);
        DrawLineEx({0, 130}, {400, 130}, 3, myColor2[0]);
    }

    Vector2 textSize = MeasureTextEx(font, "Code Block", 20, 5);
    Vector2 textPos = {
        (400 - textSize.x) / 2.0f,
        80 + (26 * 2 - textSize.y) / 2.0f};
    if (theme == colorType::HOT)
        DrawTextEx(font, "Code Block", textPos, 20, 5, myColor1[0]);
    else
        DrawTextEx(font, "Code Block", textPos, 20, 5, myColor2[0]);

    BeginScissorMode(rec.x, rec.y + 50, rec.width - 25, rec.height - 50);
    {
        if (!codeline.empty())
        {
            int startLine = scrollOffset / lineHeight;
            for (int i = 0; i < visibleLines && (startLine + i < codeline.size()); ++i)
            {
                Color color = WHITE;
                float fontSize = 15;
                if (startLine + i == lineHighlighted)
                {
                    color = BLACK;
                    DrawRectangle(rec.x, 132 + 26 * i, rec.width - 25, lineHeight, LIGHTGRAY);
                }

                float spacing = 5;
                Vector2 textSize = MeasureTextEx(font, codeline[startLine + i].c_str(), fontSize, spacing);
                Vector2 textPos = {
                    rec.x + 20,
                    132 + 26 * i + (lineHeight - textSize.y) / 2.0f};
                DrawTextEx(font, codeline[startLine + i].c_str(), textPos, fontSize, spacing, color);
            }
        }
    }
    EndScissorMode();

    if (theme == colorType::HOT)
        DrawRectangleRounded(scrollBar, 1.0f, 10, Fade(myColor1[1], 0.2f));
    else
        DrawRectangleRounded(scrollBar, 1.0f, 10, Fade(myColor2[1], 0.2f));

    if (theme == colorType::HOT)
        DrawRectangleRounded(scrollThumb, 1.0f, 10, isDragging ? ColorAlpha(myColor1[0], 0.9f) : myColor1[1]);
    else
        DrawRectangleRounded(scrollThumb, 1.0f, 10, isDragging ? ColorAlpha(myColor2[0], 0.9f) : myColor2[1]);
}

void View::CodeBlock::calculateMaxScroll()
{
    if (codeline.size() <= visibleLines)
    {
        maxScrollOffset = 0;
    }
    else
    {
        maxScrollOffset = (codeline.size() - visibleLines) * lineHeight;
    }

    float thumbHeight = (visibleLines / (float)codeline.size()) * scrollBar.height;
    thumbHeight = std::min(thumbHeight, std::max(20.0f, scrollBar.height));
    scrollThumb.height = thumbHeight;
}

void View::CodeBlock::update()
{
    Vector2 mousePos = GetMousePosition();

    static size_t lastCodeSize = 0;
    if (codeline.size() != lastCodeSize)
    {
        calculateMaxScroll();
        lastCodeSize = codeline.size();
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mousePos, scrollThumb))
        {
            isDragging = true;
            dragOffsetY = mousePos.y - scrollThumb.y;
        }
    }

    if (isDragging)
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            isDragging = false;
        }
        else
        {
            float newY = mousePos.y - dragOffsetY;
            newY = std::min(std::max(newY, scrollBar.y), scrollBar.y + scrollBar.height - scrollThumb.height);
            scrollThumb.y = newY;

            if (scrollBar.height > scrollThumb.height)
            {
                float ratio = (newY - scrollBar.y) / (scrollBar.height - scrollThumb.height);
                scrollOffset = ratio * maxScrollOffset;
            }
        }
    }
}

//=================================== PANEL ==========================================================

void View::Panel::draw()
{
    float roundness = 0.3f;
    int segments = 10;

    if (theme == colorType::HOT)
        DrawRectangleRounded(rec, roundness, segments, myColor1[3]);
    else
        DrawRectangleRounded(rec, roundness, segments, myColor2[3]);

    // Draw Rewind button
    Rectangle rewindButton = {startX, startY, buttonSize, buttonSize};
    // Draw Play/Pause button
    Rectangle playPauseButton = {startX + buttonSize + spacing, startY, buttonSize, buttonSize};
    // Draw Forward button
    Rectangle forwardButton = {startX + 2 * (buttonSize + spacing), startY, buttonSize, buttonSize};

    bool hoverRewind = CheckCollisionPointRec(GetMousePosition(), rewindButton);
    bool hoverPlayPause = CheckCollisionPointRec(GetMousePosition(), playPauseButton);
    bool hoverForward = CheckCollisionPointRec(GetMousePosition(), forwardButton);
    float defaultAlpha = 1.0f;
    float hoverAlpha = 0.3f;

    DrawTexturePro(Rewind, {0, 0, (float)Rewind.width, (float)Rewind.height}, rewindButton, {0, 0}, 0.0f, Fade(LIGHTGRAY, hoverRewind ? hoverAlpha : defaultAlpha));

    if (isPlaying)
    {
        DrawTexturePro(Pause, {0, 0, (float)Pause.width, (float)Pause.height}, playPauseButton, {0, 0}, 0.0f, Fade(LIGHTGRAY, hoverPlayPause ? hoverAlpha : defaultAlpha));
    }
    else
    {
        DrawTexturePro(Play, {0, 0, (float)Play.width, (float)Play.height}, playPauseButton, {0, 0}, 0.0f, Fade(LIGHTGRAY, hoverPlayPause ? hoverAlpha : defaultAlpha));
    }

    DrawTexturePro(Forward, {0, 0, (float)Forward.width, (float)Forward.height}, forwardButton, {0, 0}, 0.0f, Fade(LIGHTGRAY, hoverForward ? hoverAlpha : defaultAlpha));
}

void View::Panel::update()
{
    if (isPlayPressed())
    {
        isPlaying = true;
    }
    else if (isPausePressed())
    {
        isPlaying = false;
    }
}

bool View::Panel::isRewindPressed()
{
    Rectangle rewindButton = {startX, startY, buttonSize, buttonSize};
    return (CheckCollisionPointRec(GetMousePosition(), rewindButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}

bool View::Panel::isPlayPressed()
{
    Rectangle playPauseButton = {startX + buttonSize + spacing, startY, buttonSize, buttonSize};
    return CheckCollisionPointRec(GetMousePosition(), playPauseButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !isPlaying;
}

bool View::Panel::isPausePressed()
{
    Rectangle playPauseButton = {startX + buttonSize + spacing, startY, buttonSize, buttonSize};
    return CheckCollisionPointRec(GetMousePosition(), playPauseButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPlaying;
}

bool View::Panel::isForwardPressed()
{
    Rectangle forwardButton = {startX + 2 * (buttonSize + spacing), startY, buttonSize, buttonSize};
    return CheckCollisionPointRec(GetMousePosition(), forwardButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

//================================= OPTION ===============================================

void View::Option::draw()
{
    float buttonWidth = 100;
    float buttonHeight = 40;
    float startX = 20;  // Đẩy Initialize sát biên trái hơn
    float startY = 574;

    if (theme == colorType::HOT)
    {
        DrawRectangleRounded({0, 510, 400, 200}, 0.3f, 10, myColor1[2]);
        DrawRectangleRoundedLinesEx({0, 510, 400, 200}, 0.3f, 10, 3, myColor1[0]);
        DrawLineEx({0, 550}, {400, 550}, 3, myColor1[0]);
    }
    else
    {
        DrawRectangleRounded({0, 510, 400, 200}, 0.3f, 10, myColor2[2]);
        DrawRectangleRoundedLinesEx({0, 510, 400, 200}, 0.3f, 10, 3, myColor2[0]);
        DrawLineEx({0, 550}, {400, 550}, 3, myColor2[0]);
    }

    Vector2 textSize = MeasureTextEx(font, "Option", 20, 5);
    Vector2 textPos = {
        (400 - textSize.x) / 2.0f,
        510 + (40 - textSize.y) / 2.0f};
    if (theme == colorType::HOT)
        DrawTextEx(font, "Option", textPos, 20, 5, myColor1[0]);
    else
        DrawTextEx(font, "Option", textPos, 20, 5, myColor2[0]);

    // ******************* INI BUTTON ************************
    Rectangle IniButton = {startX, startY, buttonWidth, buttonHeight};
    if (CheckCollisionPointRec(GetMousePosition(), IniButton))
    {
        if (theme == colorType::HOT)
        {
            DrawRectangleRoundedLinesEx(IniButton, 0.3f, 10, 3, myColor1[0]);
        }
        else
        {
            DrawRectangleRoundedLinesEx(IniButton, 0.3f, 10, 3, myColor2[0]);
        }
    }

    textSize = MeasureTextEx(font, "Init", 20, 5);
    textPos = {
        startX + (buttonWidth - textSize.x) / 2.0f,
        startY + (buttonHeight - textSize.y) / 2.0f};
    if (theme == colorType::HOT)
    {
        DrawRectangleRounded(IniButton, 0.3f, 10, myColor1[1]);
        DrawTextEx(font, "Init", textPos, 20, 5, myColor1[0]);
    }
    else
    {
        DrawRectangleRounded(IniButton, 0.3f, 10, myColor2[1]);
        DrawTextEx(font, "Init", textPos, 20, 5, myColor2[0]);
    }

    if (mode == Mode::AVL || mode == Mode::SLLIST || mode == Mode::HTABLE)
    {
        // ******************* UPDATE BUTTON ************************
        Rectangle UpdateButton = {150, startY, buttonWidth, buttonHeight};
        if (CheckCollisionPointRec(GetMousePosition(), UpdateButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(UpdateButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(UpdateButton, 0.3f, 10, 3, myColor2[0]);
            }
        }

        textSize = MeasureTextEx(font, "Update", 20, 5);
        textPos = {
            150 + (buttonWidth - textSize.x) / 2.0f,
            startY + (buttonHeight - textSize.y) / 2.0f};
        if (theme == colorType::HOT)
        {
            DrawRectangleRounded(UpdateButton, 0.3f, 10, myColor1[1]);
            DrawTextEx(font, "Update", textPos, 20, 5, myColor1[0]);
        }
        else
        {
            DrawRectangleRounded(UpdateButton, 0.3f, 10, myColor2[1]);
            DrawTextEx(font, "Update", textPos, 20, 5, myColor2[0]);
        }

        // ***************** SEARCH BUTTON **************************
        Rectangle SearchButton = {280, startY, buttonWidth, buttonHeight}; 
        if (CheckCollisionPointRec(GetMousePosition(), SearchButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(SearchButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(SearchButton, 0.3f, 10, 3, myColor2[0]);
            }
        }

        textSize = MeasureTextEx(font, "Search", 20, 5);
        textPos = {
            280 + (buttonWidth - textSize.x) / 2.0f,
            startY + (buttonHeight - textSize.y) / 2.0f};
        if (theme == colorType::HOT)
        {
            DrawRectangleRounded(SearchButton, 0.3f, 10, myColor1[1]);
            DrawTextEx(font, "Search", textPos, 20, 5, myColor1[0]);
        }
        else
        {
            DrawRectangleRounded(SearchButton, 0.3f, 10, myColor2[1]);
            DrawTextEx(font, "Search", textPos, 20, 5, myColor2[0]);
        }

        // *************** INSERT BUTTON ****************************
        Rectangle InsertButton = {85, startY + 62, buttonWidth, buttonHeight}; 
        if (CheckCollisionPointRec(GetMousePosition(), InsertButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(InsertButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(InsertButton, 0.3f, 10, 3, myColor2[0]);
            }
        }

        textSize = MeasureTextEx(font, "Insert", 20, 5);
        textPos = {
            85 + (buttonWidth - textSize.x) / 2.0f,
            startY + 62 + (buttonHeight - textSize.y) / 2.0f};
        if (theme == colorType::HOT)
        {
            DrawRectangleRounded(InsertButton, 0.3f, 10, myColor1[1]);
            DrawTextEx(font, "Insert", textPos, 20, 5, myColor1[0]);
        }
        else
        {
            DrawRectangleRounded(InsertButton, 0.3f, 10, myColor2[1]);
            DrawTextEx(font, "Insert", textPos, 20, 5, myColor2[0]);
        }

        // ***************** DELETE BUTTON ******************************
        Rectangle DeleteButton = {215, startY + 62, buttonWidth, buttonHeight}; 
        if (CheckCollisionPointRec(GetMousePosition(), DeleteButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(DeleteButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(DeleteButton, 0.3f, 10, 3, myColor2[0]);
            }
        }

        textSize = MeasureTextEx(font, "Delete", 20, 5);
        textPos = {
            215 + (buttonWidth - textSize.x) / 2.0f,
            startY + 62 + (buttonHeight - textSize.y) / 2.0f};
        if (theme == colorType::HOT)
        {
            DrawRectangleRounded(DeleteButton, 0.3f, 10, myColor1[1]);
            DrawTextEx(font, "Delete", textPos, 20, 5, myColor1[0]);
        }
        else
        {
            DrawRectangleRounded(DeleteButton, 0.3f, 10, myColor2[1]);
            DrawTextEx(font, "Delete", textPos, 20, 5, myColor2[0]);
        }
    }
    else if (mode == Mode::GRAPH)
    {
        
        Rectangle InsertButton = {85, startY + 62, buttonWidth, buttonHeight};
        if (CheckCollisionPointRec(GetMousePosition(), InsertButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(InsertButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(InsertButton, 0.3f, 10, 3, myColor2[0]);
            }
        }

        textSize = MeasureTextEx(font, "Dijkstra", 16, 5);
        textPos = {
            85 + (buttonWidth - textSize.x) / 2.0f, 
            startY + 62 + (buttonHeight - textSize.y) / 2.0f};
        if (theme == colorType::HOT)
        {
            DrawRectangleRounded(InsertButton, 0.3f, 10, myColor1[1]);
            DrawTextEx(font, "Dijkstra", textPos, 16, 5, myColor1[0]);
        }
        else
        {
            DrawRectangleRounded(InsertButton, 0.3f, 10, myColor2[1]);
            DrawTextEx(font, "Dijkstra", textPos, 16, 5, myColor2[0]);
        }

        // ****************** PRIM BUTTON ****************************
        Rectangle SearchButton = {280, startY, buttonWidth, buttonHeight};
        if (CheckCollisionPointRec(GetMousePosition(), SearchButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(SearchButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(SearchButton, 0.3f, 10, 3, myColor2[0]);
            }
        }

        textSize = MeasureTextEx(font, "Prim", 20, 5);
        textPos = {
            280 + (buttonWidth - textSize.x) / 2.0f, 
            startY + (buttonHeight - textSize.y) / 2.0f};
        if (theme == colorType::HOT)
        {
            DrawRectangleRounded(SearchButton, 0.3f, 10, myColor1[1]);
            DrawTextEx(font, "Prim", textPos, 20, 5, myColor1[0]);
        }
        else
        {
            DrawRectangleRounded(SearchButton, 0.3f, 10, myColor2[1]);
            DrawTextEx(font, "Prim", textPos, 20, 5, myColor2[0]);
        }

        // ********************** KRUSKAL BUTTON ***********************
        Rectangle DeleteButton = {215, startY + 62, buttonWidth, buttonHeight};
        if (CheckCollisionPointRec(GetMousePosition(), DeleteButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(DeleteButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(DeleteButton, 0.3f, 10, 3, myColor2[0]);
            }
        }

        textSize = MeasureTextEx(font, "Kruskal", 18, 5);
        textPos = {
            215 + (buttonWidth - textSize.x) / 2.0f, // Căn giữa theo tọa độ x của nút
            startY + 62 + (buttonHeight - textSize.y) / 2.0f};
        if (theme == colorType::HOT)
        {
            DrawRectangleRounded(DeleteButton, 0.3f, 10, myColor1[1]);
            DrawTextEx(font, "Kruskal", textPos, 18, 5, myColor1[0]);
        }
        else
        {
            DrawRectangleRounded(DeleteButton, 0.3f, 10, myColor2[1]);
            DrawTextEx(font, "Kruskal", textPos, 18, 5, myColor2[0]);
        }
    }
}

bool View::Option::isInitialize()
{
    float buttonWidth = 100;
    float buttonHeight = 40;
    float startX = 20;  // Sát biên trái
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
    float buttonWidth = 100;
    float buttonHeight = 40;
    float startX = 20;
    float startY = 574;
    Rectangle InsertButton = {85, startY + 62, buttonWidth, buttonHeight}; 
    if (CheckCollisionPointRec(GetMousePosition(), InsertButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

bool View::Option::isDelete()
{
    float buttonWidth = 100;
    float buttonHeight = 40;
    float startY = 574;
    Rectangle DeleteButton = {215, startY + 62, buttonWidth, buttonHeight}; 
    if (CheckCollisionPointRec(GetMousePosition(), DeleteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

bool View::Option::isSearch()
{
    float buttonWidth = 100;
    float buttonHeight = 40;
    float startY = 574;
    Rectangle SearchButton = {280, startY, buttonWidth, buttonHeight}; 
    if (CheckCollisionPointRec(GetMousePosition(), SearchButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

bool View::Option::isUpdate()
{
    float buttonWidth = 100;
    float buttonHeight = 40;
    float startY = 574;
    Rectangle UpdateButton = {150, startY, buttonWidth, buttonHeight}; 
    if (CheckCollisionPointRec(GetMousePosition(), UpdateButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}
// ========================= TEXTBOX ========================================

void View::TextBox::draw()
{
    if (!isOpen)
        return;

    Rectangle textBoxRec = {0, 510, 400, 200};
    if (theme == colorType::HOT)
    {   
        DrawRectangleRounded(textBoxRec, 0.3f, 10, LIGHTGRAY);
        DrawRectangleRoundedLinesEx(textBoxRec, 0.3f, 10, 3, myColor1[0]);
    }
    else
    {
        DrawRectangleRounded(textBoxRec, 0.3f, 10, LIGHTGRAY);
        DrawRectangleRoundedLinesEx(textBoxRec, 0.3f, 10, 3, myColor2[0]);
    }

    Rectangle closeButton = {textBoxRec.x + textBoxRec.width - 30, textBoxRec.y + 10, 20, 20};
    if (theme == colorType::HOT)
    {
        DrawRectangleRec(closeButton, myColor1[2]);
        DrawText("X", closeButton.x + 5, closeButton.y + 2, 20, WHITE);
    }
    else
    {
        DrawRectangleRec(closeButton, myColor2[2]);
        DrawText("X", closeButton.x + 5, closeButton.y + 2, 20, WHITE);
    }

    if (CheckCollisionPointRec(GetMousePosition(), closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        isOpen = false;
        value.clear();
        enteredValues = false;
        enteredPrime = false;
        isDragDropMode = false;
    }

    if (isDragDropMode)
    {
        // Hiển thị hướng dẫn kéo thả file
        DrawText("Drag & drop file here", textBoxRec.x + 10, textBoxRec.y + 40, 20, DARKGRAY);
        DrawText("Supported format:", textBoxRec.x + 10, textBoxRec.y + 70, 16, GRAY);
        if (mode == Mode::HTABLE)
        {
            DrawText("Line 1: Values, Line 2: Prime", textBoxRec.x + 10, textBoxRec.y + 90, 16, GRAY);
        }
        else if (mode == Mode::GRAPH)
        {
            DrawText("Adjacency matrix: one row per line", textBoxRec.x + 10, textBoxRec.y + 90, 16, GRAY);
        }
        else
        {
            DrawText("One value per line", textBoxRec.x + 10, textBoxRec.y + 90, 16, GRAY);
        }
    }
    else
    {
        std::string inputText;
        if (mode == Mode::HTABLE)
        {
            if (!enteredValues)
            {
                inputText = "Enter values: " + value;
            }
            else if (!enteredPrime)
            {
                inputText = "Enter prime number: " + value;
            }
        }
        else if (func == Function::UPDATE){
            inputText = "Enter exactly two numbers: " + value;
            DrawText("First: value to replace", textBoxRec.x + 10, textBoxRec.y + 70, 16, GRAY);
            DrawText("Second: new value", textBoxRec.x + 10, textBoxRec.y + 90, 16, GRAY);
        }
        else if (mode == Mode::GRAPH && isFileMode){
            inputText = "Enter the file path: " + value;
        }
        else
        {
            inputText = "Enter value: " + value;
        }
        float maxWidth = textBoxRec.width - 60; // Chừa lề 40 pixel
        float lineHeight = 25;                  // Chiều cao mỗi dòng (có thể điều chỉnh)
        float currentY = textBoxRec.y + 40;     // Vị trí Y bắt đầu vẽ văn bản
        float fontSize = 20;
        float spacing = 1;

        // Chia văn bản thành các từ (dựa trên khoảng trắng)
        std::vector<std::string> words;
        std::stringstream ss(inputText);
        std::string word;
        while (ss >> word)
        {
            words.push_back(word);
        }

        // Tạo các dòng văn bản
        std::string currentLine = "";
        for (size_t i = 0; i < words.size(); ++i)
        {
            std::string testLine = currentLine.empty() ? words[i] : currentLine + " " + words[i];
            Vector2 testSize = MeasureTextEx(GetFontDefault(), testLine.c_str(), fontSize, spacing);

            if (testSize.x <= maxWidth)
            {
                // Nếu dòng hiện tại vẫn vừa, thêm từ vào
                currentLine = testLine;
            }
            else
            {
                // Nếu không vừa, vẽ dòng hiện tại và bắt đầu dòng mới
                if (!currentLine.empty())
                {
                    DrawText(currentLine.c_str(), textBoxRec.x + 10, currentY, fontSize, DARKGRAY);
                    currentY += lineHeight; // Xuống dòng
                }
                currentLine = words[i]; // Bắt đầu dòng mới với từ hiện tại
            }

            // Nếu đây là từ cuối cùng, vẽ dòng còn lại
            if (i == words.size() - 1 && !currentLine.empty())
            {
                DrawText(currentLine.c_str(), textBoxRec.x + 10, currentY, fontSize, DARKGRAY);
            }

            // Kiểm tra nếu vượt quá chiều cao của TextBox (tùy chọn)
            if (currentY + lineHeight > textBoxRec.y + textBoxRec.height - 20)
            {
                break; // Ngừng vẽ nếu vượt quá chiều cao của TextBox
            }
        }
    }
}

void View::TextBox::handleFileDrop()
{
    if (!IsFileDropped())
    {
        return;
    }

    FilePathList droppedFiles = LoadDroppedFiles(); // List of file paths
    if (droppedFiles.count == 0 || droppedFiles.paths == nullptr)
    {
        UnloadDroppedFiles(droppedFiles);
        return;
    }

    std::string filePath = droppedFiles.paths[0]; // take the first one
    if (filePath.empty())
    {
        UnloadDroppedFiles(droppedFiles);
        return;
    }

    if (processFileData(filePath))
    {
        isOpen = false;
        isDragDropMode = false;
    }

    UnloadDroppedFiles(droppedFiles);
}



bool View::TextBox::processFileData(const std::string &filePath)
{
    if (filePath.empty())
    {
        parent->log.infor.push_back("Error: File path is empty");
        return false;
    }

    char *fileData = LoadFileText(filePath.c_str());
    if (!fileData)
    {
        parent->log.infor.push_back("Failed to load file: " + filePath);
        return false;
    }

    std::stringstream ss(fileData);
    std::string line;
    someList.clear();
    adjMatrix.clear();

    if (mode == Mode::GRAPH)
    {
        processingGraphMatrix = true;
        while (std::getline(ss, line))
        {
            std::vector<int> row;
            std::stringstream lineStream(line);
            std::string token;
            while (lineStream >> token)
            {
                try
                {
                    row.push_back(std::stoi(token));
                }
                catch (std::invalid_argument)
                {
                }
            }
            adjMatrix.push_back(row);
        }
        processingGraphMatrix = false;
        parent->log.infor.push_back("Loaded adjacency matrix successfully: " + std::to_string(adjMatrix.size()) + " x " + std::to_string(adjMatrix.empty() ? 0 : adjMatrix[0].size()));
    }

    else if (mode == Mode::HTABLE)
    {
        if (std::getline(ss, line))
        {
            std::stringstream valuesStream(line);
            std::string token;
            while (valuesStream >> token)
            {
                try
                {
                    someList.push_back(std::stoi(token));
                }
                catch (const std::invalid_argument &)
                {
                    parent->log.infor.push_back("Invalid value in file: " + token);
                }
            }
        }

        if (std::getline(ss, line))
        {
            try
            {
                primeNumber = std::stoi(line);
                enteredPrime = true;
            }
            catch (const std::invalid_argument &)
            {
                parent->log.infor.push_back("Invalid prime number in file: " + line);
            }
        }
        enteredValues = true;
    }
    else
    {
        while (std::getline(ss, line))
        {
            std::stringstream lineStream(line);
            std::string token;
            while (lineStream >> token)
            {
                try
                {
                    someList.push_back(std::stoi(token));
                    enteredValues = true;
                    if (mode != Mode::HTABLE)
                        enteredPrime = true;
                }
                catch (const std::invalid_argument &)
                {
                    parent->log.infor.push_back("Invalid value in file: " + token);
                }
            }
        }
    }

    UnloadFileText(fileData);
    parent->log.infor.push_back("Loaded data from: " + filePath);

    // DEBUG
    if (mode == Mode::HTABLE)
    {

        std::cout << "Values count: " << someList.size() << std::endl;
        std::cout << "Prime number: " << someList.size() << std::endl;
    }
    else if (mode == Mode::GRAPH)
    {
        parent->log.infor.push_back("Number of vertices: " + std::to_string(adjMatrix.size()));
        std::cout << "Number of vertices: " << adjMatrix.size() << std::endl;
    }

    return true;
}

void View::TextBox::update()
{
    if (isOpen)
    {

        if (isDragDropMode)
        {
            handleFileDrop();
            if (!someList.empty())
            {                   // if loaded successfully
                isOpen = false; // Close TextBox
            }
            return;
        }
        int key = GetCharPressed();
        while (key > 0)
        {
            if (mode == Mode::GRAPH && isFileMode){
                value += static_cast<char>(key);
            }
            else if ((key >= '0' && key <= '9') || key == ' ')
            { // allow numbers and space only
                value += static_cast<char>(key);
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !value.empty())
        {
            value.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            if (mode == Mode::GRAPH && isFileMode){
                if (processFileData(value)){
                    isOpen = false;
                    isFileMode = false;
                }
            }
            else if (mode == Mode::HTABLE)
            {
                if (!enteredValues)
                {
                    std::vector<int> numbers;
                    std::stringstream ss(value);
                    std::string token;
                    while (ss >> token)
                    {
                        try
                        {
                            int num = std::stoi(token);
                            numbers.push_back(num);
                        }
                        catch (const std::invalid_argument &)
                        {
                        }
                    }
                    someList.insert(someList.end(), numbers.begin(), numbers.end());
                    value.clear();
                    enteredValues = true;
                    if (enteredPrime)
                    {
                        isOpen = false;
                    }
                }
                else if (!enteredPrime)
                {
                    try
                    {
                        primeNumber = std::stoi(value);
                        enteredPrime = true;
                        value.clear();
                        isOpen = false;
                    }
                    catch (const std::invalid_argument &)
                    {
                    }
                }
            }
            else
            {
                std::vector<int> numbers;
                std::stringstream ss(value);
                std::string token;
                while (ss >> token)
                {
                    try
                    {
                        int num = std::stoi(token);
                        someList.push_back(num);
                    }
                    catch (const std::invalid_argument &)
                    {
                    }
                }
                value.clear();
                isOpen = false;
            }
        }
    }
}

bool View::TextBox::readFileData(const std::string &filePath)
{

    long fileSize = GetFileLength(filePath.c_str());
    if (fileSize > 1000000)
    {
        std::cout << "ERROR: File too large" << std::endl;
        return false;
    }

    try
    {
        char *fileData = LoadFileText(filePath.c_str());
        if (!fileData)
        {
            // log.infor.push_back("ERROR: Failed to load file: " + filePath);
            std::cout << "Failed to load" << std::endl;
            return false;
        }

        std::stringstream ss(fileData);
        std::string line;
        someList.clear();

        // Xử lý nội dung file trong try-catch
        try
        {
            if (mode == Mode::HTABLE)
            {
                // Xử lý HTABLE
                if (std::getline(ss, line))
                {
                    std::stringstream valuesStream(line);
                    std::string token;
                    while (valuesStream >> token)
                    {
                        someList.push_back(std::stoi(token));
                    }
                }
                if (std::getline(ss, line))
                {
                    primeNumber = std::stoi(line);
                    enteredPrime = true;
                }
                enteredValues = true;
            }
            else
            {
                // Xử lý các mode khác
                while (std::getline(ss, line))
                {
                    std::stringstream lineStream(line);
                    std::string token;
                    while (lineStream >> token)
                    {
                        someList.push_back(std::stoi(token));
                    }
                }
                enteredValues = true;
                if (mode != Mode::HTABLE)
                    enteredPrime = true;
            }
        }
        catch (const std::exception &e)
        {
            // log.infor.push_back("ERROR: Invalid file format: " + std::string(e.what()));
            UnloadFileText(fileData);
            return false;
        }

        UnloadFileText(fileData);
        return true;
    }
    catch (...)
    {
        // log.infor.push_back("CRITICAL ERROR: Failed to process file");
        std::cout << "Failed to process file" << std::endl;
        return false;
    }
}

void View::TextBox::generateRandomValues()
{
    someList.clear();
    int count = GetRandomValue(15, 20);

    for (int i = 0; i < count; i++)
    {
        someList.push_back(GetRandomValue(1, 99));
    }
    enteredValues = true;

    if (mode == Mode::HTABLE)
    {
        std::vector<int> primes = {3, 5, 7, 11, 13};
        if (!primes.empty())
        {
            primeNumber = primes[GetRandomValue(0, primes.size() - 1)];
            enteredPrime = true;
        }
        else
        {
            primeNumber = 7;
            enteredPrime = true;
        }
        std::cout << "Generate succesfully" << std::endl;
        std::cout << "The amount: " << someList.size() << std::endl;
        std::cout << "Prime number: " << primeNumber << std::endl;
    }
}

void View::TextBox::generateRandomGraphMatrix(){
    const int minVertices = 5;
    const int maxVertices = 15;
    const int maxWeight = 10;
    const float edgeDensity = 0.2f;

    // int n = GetRandomValue(minVertices, maxVertices);
    // adjMatrix.clear();
    // adjMatrix.resize(n, std::vector<int>(n, 0));
    // for (int i = 0; i < n; ++i) {
    //     for (int j = i + 1; j < n; ++j) {
    //         if (GetRandomValue(0, 100) < static_cast<int>(edgeDensity * 100)) {
    //             int weight = GetRandomValue(1, maxWeight);
    //             adjMatrix[i][j] = weight;
    //             adjMatrix[j][i] = weight; 
    //         }
    //     }
    // }
    int n = minVertices + rand() % (maxVertices - minVertices + 1);
    adjMatrix.clear();
    adjMatrix.resize(n, std::vector<int>(n, 0));
    std::vector<int> nodes(n);
    for (int i = 0; i < n; ++i) nodes[i] = i;
    std::random_shuffle(nodes.begin(), nodes.end());

    for (int i = 1; i < n; ++i) {
        int u = nodes[i];
        int v = nodes[rand() % i]; 
        int weight = 1 + rand() % maxWeight;
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight;
    }

    int totalPossibleEdges = n * (n - 1) / 2;
    int targetEdges = static_cast<int>(edgeDensity * totalPossibleEdges);
    int currentEdges = n - 1;

    while (currentEdges < targetEdges) {
        int i = rand() % n;
        int j = rand() % n;
        if (i != j && adjMatrix[i][j] == 0) {
            int weight = 1 + rand() % maxWeight;
            adjMatrix[i][j] = weight;
            adjMatrix[j][i] = weight;
            ++currentEdges;
        }
    }
}
// =================================== LOG ==============================================
void View::Log::draw()
{
    rec = {0, 400, 400, 100};
    if (theme == colorType::HOT)
    {
        DrawRectangleRounded(rec, 0.6f, 10, myColor1[2]);
        DrawRectangleRoundedLinesEx(rec, 0.6f, 10, 3, myColor1[0]);
        DrawLineEx({0, 440}, {400, 440}, 3, myColor1[0]);
    }
    else
    {
        DrawRectangleRounded(rec, 0.6f, 10, myColor2[2]);
        DrawRectangleRoundedLinesEx(rec, 0.6f, 10, 3, myColor2[0]);
        DrawLineEx({0, 440}, {400, 440}, 3, myColor2[0]);
    }

    Vector2 textSize = MeasureTextEx(font, "Log", 20, 5);
    Vector2 textPos = {
        (400 - textSize.x) / 2.0f,
        400 + (40 - textSize.y) / 2.0f};
    if (theme == colorType::HOT)
        DrawTextEx(font, "Log", textPos, 20, 5, myColor1[0]);
    else
        DrawTextEx(font, "Log", textPos, 20, 5, myColor2[0]);
    if (infor.size() == 0)
        return;

    BeginScissorMode(0, 425, 400, 75);
    float fontSize = 15;
    float spacing = 5;
    textSize = MeasureTextEx(font, infor[infor.size() - 1].c_str(), fontSize, spacing);
    textPos = {
        (400 - textSize.x) / 2.0f,
        452 + (26 - textSize.y) / 2.0f};
    DrawTextEx(font, infor[infor.size() - 1].c_str(), textPos, fontSize, spacing, WHITE);
    EndScissorMode();
}

// ======================================== SLIDER ======================================================

void View::Slider::update()
{
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, bound))
    {
        isDragging = true;
    }
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        isDragging = false;
    }
    if (isDragging)
    {
        float value = min + (mousePos.x - bound.x) / bound.width * (max - min);
        speed = std::max(min, std::min(max, value));
    }
}

void View::Slider::draw()
{
    DrawRectangleRounded(bound, 1.0f, 10, GRAY);
    float buttonX = bound.x + (speed - min) / (max - min) * bound.width;
    DrawCircle(buttonX, bound.y + bound.height / 2.0f, bound.height * 3 / 4.0f, BLACK);
    Vector2 textSize = MeasureTextEx(font, "Speed: ", 20, 5);
    Vector2 textPos = {
        50 + (150 - textSize.x) / 2.0f,
        720 + (45 - textSize.y) / 2.0f};
    if (theme == colorType::HOT)
        DrawTextEx(font, "Speed: ", textPos, 20, 5, myColor1[0]);
    else
        DrawTextEx(font, "Speed: ", textPos, 20, 5, myColor2[0]);
    textSize = MeasureTextEx(font, std::to_string(speed).c_str(), 20, 5);
    textPos = {
        200 + (150 - textSize.x) / 2.0f,
        720 + (45 - textSize.y) / 2.0f};

    float roundedSpeed = round(speed * 10) / 10.0f;

    char buffer[32];
    sprintf(buffer, "%g", roundedSpeed);
    std::string speedIndex = buffer;
    speedIndex += ".x";

    if (theme == colorType::HOT)
        DrawTextEx(font, speedIndex.c_str(), textPos, 20, 5, myColor1[0]);
    else
        DrawTextEx(font, speedIndex.c_str(), textPos, 20, 5, myColor2[0]);
}

//=========================================== HOME =======================================

void View::Home::draw()
{
    Rectangle sourceRec;
    if (theme == colorType::HOT)
        sourceRec = {0, 0, (float)hot_icon.width, (float)hot_icon.height};
    else
        sourceRec = {0, 0, (float)cold_icon.width, (float)cold_icon.height};
    Rectangle destRec = {1380, 20, 40, 40};
    Vector2 origin = {0, 0};
    if (theme == colorType::HOT)
        DrawTexturePro(hot_icon, sourceRec, destRec, origin, 0.0f, WHITE);
    else
        DrawTexturePro(cold_icon, sourceRec, destRec, origin, 0.0f, WHITE);
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

void View::InputPanel::draw()
{
    if (!isOpen)
        return;

    if (theme == colorType::HOT)
    {
        DrawRectangleRounded(panelRec, 0.3f, 10, myColor1[2]);
        DrawRectangleRoundedLines(panelRec, 0.3f, 10, myColor1[0]);
        DrawLineEx({panelRec.x, panelRec.y + 40}, {panelRec.x + panelRec.width, panelRec.y + 40}, 3, myColor1[0]);
    }
    else
    {
        DrawRectangleRounded(panelRec, 0.3f, 10, myColor2[2]);
        DrawRectangleRoundedLines(panelRec, 0.3f, 10, myColor2[0]);
        DrawLineEx({panelRec.x, panelRec.y + 40}, {panelRec.x + panelRec.width, panelRec.y + 40}, 3, myColor2[0]);
    }

    Vector2 textSize = MeasureTextEx(font, "Input Method", 20, 5);
    Vector2 textPos = {
        (panelRec.width - textSize.x) / 2.0f,
        panelRec.y + (40 - textSize.y) / 2.0f};
    if (theme == colorType::HOT)
        DrawTextEx(font, "Input Method", textPos, 20, 5, myColor1[0]);
    else
        DrawTextEx(font, "Input Method", textPos, 20, 5, myColor2[0]);

    // Draw buttons
    Color buttonColor, textColor;
    if (theme == colorType::HOT)
    {
        buttonColor = myColor1[1];
        textColor = myColor1[0];
    }
    else
    {
        buttonColor = myColor2[1];
        textColor = myColor2[0];
    }

    DrawRectangleRounded(randomButton, 0.3f, 10, buttonColor);
    // ***************** RANDOM BUTTON ***************************
    if (CheckCollisionPointRec(GetMousePosition(), randomButton))
    {
        if (theme == colorType::HOT)
        {
            DrawRectangleRoundedLinesEx(randomButton, 0.3f, 10, 3, myColor1[0]);
        }
        else
        {
            DrawRectangleRoundedLinesEx(randomButton, 0.3f, 10, 3, myColor2[0]);
        }
    }
    textSize = MeasureTextEx(font, "Random", 20, 5);
    textPos = {
        randomButton.x + (randomButton.width - textSize.x) / 2.0f,
        randomButton.y + (randomButton.height - textSize.y) / 2.0f};
    DrawTextEx(font, "Random", textPos, 20, 5, textColor);

    // ******************** URL BUTTON *******************
    DrawRectangleRounded(urlButton, 0.3f, 10, buttonColor);
    if (CheckCollisionPointRec(GetMousePosition(), urlButton))
    {
        if (theme == colorType::HOT)
        {
            DrawRectangleRoundedLinesEx(urlButton, 0.3f, 10, 3, myColor1[0]);
        }
        else
        {
            DrawRectangleRoundedLinesEx(urlButton, 0.3f, 10, 3, myColor2[0]);
        }
    }
    textSize = MeasureTextEx(font, "URL", 20, 5);
    textPos = {
        urlButton.x + (urlButton.width - textSize.x) / 2.0f,
        urlButton.y + (urlButton.height - textSize.y) / 2.0f};
    DrawTextEx(font, "URL", textPos, 20, 5, textColor);

    // ****************** CLOSE BUTTON ************************
    DrawRectangleRounded(closeButton, 0.3f, 10, buttonColor);
    if (CheckCollisionPointRec(GetMousePosition(), closeButton))
    {
        if (theme == colorType::HOT)
        {
            DrawRectangleRoundedLinesEx(closeButton, 0.3f, 10, 3, myColor1[0]);
        }
        else
        {
            DrawRectangleRoundedLinesEx(closeButton, 0.3f, 10, 3, myColor2[0]);
        }
    }
    textSize = MeasureTextEx(font, "Close", 20, 5);
    textPos = {
        closeButton.x + (closeButton.width - textSize.x) / 2.0f,
        closeButton.y + (closeButton.height - textSize.y) / 2.0f};
    DrawTextEx(font, "Close", textPos, 20, 5, textColor);

    if (mode == Mode::GRAPH)
    {
        // ********************** FILE BUTTON *************************
        DrawRectangleRounded(fileButton, 0.3f, 10, buttonColor);
        if (CheckCollisionPointRec(GetMousePosition(), fileButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(fileButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(fileButton, 0.3f, 10, 3, myColor2[0]);
            }
        }
        textSize = MeasureTextEx(font, "File", 20, 5);
        textPos = {
            fileButton.x + (fileButton.width - textSize.x) / 2.0f,
            fileButton.y + (fileButton.height - textSize.y) / 2.0f};
        DrawTextEx(font, "File", textPos, 20, 5, textColor);
    }

    else
    {
        // ************************* TEXTBOX BUTTON **********************
        DrawRectangleRounded(textboxButton, 0.3f, 10, buttonColor);
        if (CheckCollisionPointRec(GetMousePosition(), textboxButton))
        {
            if (theme == colorType::HOT)
            {
                DrawRectangleRoundedLinesEx(textboxButton, 0.3f, 10, 3, myColor1[0]);
            }
            else
            {
                DrawRectangleRoundedLinesEx(textboxButton, 0.3f, 10, 3, myColor2[0]);
            }
        }
        textSize = MeasureTextEx(font, "Text box", 20, 5);
        textPos = {
            textboxButton.x + (textboxButton.width - textSize.x) / 2.0f,
            textboxButton.y + (textboxButton.height - textSize.y) / 2.0f};
        DrawTextEx(font, "Text Box", textPos, 20, 5, textColor);
    }
}

bool View::InputPanel::isTextboxPressed()
{
    return CheckCollisionPointRec(GetMousePosition(), textboxButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool View::InputPanel::isFilePressed()
{
    if (mode != Mode::GRAPH)
        return false;
    return CheckCollisionPointRec(GetMousePosition(), fileButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool View::InputPanel::isURLPressed()
{
    return CheckCollisionPointRec(GetMousePosition(), urlButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool View::InputPanel::isClosePressed()
{
    return CheckCollisionPointRec(GetMousePosition(), closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool View::InputPanel::isRandomPressed()
{
    return CheckCollisionPointRec(GetMousePosition(), randomButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

//=================================== SETTING ======================================

void View::Setting::draw()
{
    if (!isOpen)
    {
        Rectangle sourceRec;
        if (theme == colorType::HOT)
            sourceRec = {0, 0, (float)hotSetting.width, (float)hotSetting.height};
        else
            sourceRec = {0, 0, (float)coldSetting.width, (float)coldSetting.height};
        Rectangle destRec = {20, 20, 40, 40};
        Vector2 origin = {0, 0};
        if (theme == colorType::HOT)
            DrawTexturePro(hotSetting, sourceRec, destRec, origin, 0.0f, WHITE);
        else
            DrawTexturePro(coldSetting, sourceRec, destRec, origin, 0.0f, WHITE);
    }
    else
    {
        Rectangle sourceRec;
        if (theme == colorType::HOT)
            sourceRec = {0, 0, (float)hotSetting.width, (float)hotSetting.height};
        else
            sourceRec = {0, 0, (float)coldSetting.width, (float)coldSetting.height};
        Rectangle destRec = {20, 20, 40, 40};
        Vector2 origin = {0, 0};
        if (theme == colorType::HOT)
            DrawTexturePro(hotSetting, sourceRec, destRec, origin, 0.0f, WHITE);
        else
            DrawTexturePro(coldSetting, sourceRec, destRec, origin, 0.0f, WHITE);

        if (theme == colorType::HOT)
        {
            DrawRectangleRounded({30, 30, 400, 180}, 0.3f, 10, myColor1[2]);
            DrawRectangleRoundedLinesEx({30, 30, 400, 180}, 0.3f, 10, 3, myColor1[0]);
            DrawLineEx({30, 90}, {430, 90}, 3, myColor1[0]);
            DrawLineEx({30, 150}, {430, 150}, 3, myColor1[0]);
            DrawLineEx({190, 30}, {190, 210}, 3, myColor1[0]);

            DrawTriangle({415, 60}, {400, 40}, {400, 80}, myColor1[1]);  // Tam giác phải (Font)
            DrawTriangle({205, 60}, {220, 80}, {220, 40}, myColor1[1]);  // Tam giác trái (Font)
            DrawTriangle({415, 180}, {400, 160}, {400, 200}, myColor1[1]); // Tam giác phải (Theme)
            DrawTriangle({205, 180}, {220, 200}, {220, 160}, myColor1[1]); // Tam giác trái (Them

            Vector2 textSize = MeasureTextEx(font, "Font", 20, 5);
            Vector2 textPos = {
                30 + (160 - textSize.x) / 2.0f,
                30 + (60 - textSize.y) / 2.0f};
            DrawTextEx(font, "Font", textPos, 20, 5, myColor1[0]);
            textSize = MeasureTextEx(font, "Music", 20, 5);
            textPos = {
                30 + (160 - textSize.x) / 2.0f,
                90 + (60 - textSize.y) / 2.0f};
            DrawTextEx(font, "Music", textPos, 20, 5, myColor1[0]);
            textSize = MeasureTextEx(font, "Theme", 20, 5);
            textPos = {
                30 + (160 - textSize.x) / 2.0f,
                150 + (60 - textSize.y) / 2.0f};
            DrawTextEx(font, "Theme", textPos, 20, 5, myColor1[0]);

            if (themeView)
            {
                textSize = MeasureTextEx(font, "Hot", 20, 5);
                textPos = {
                    210 + (180 - textSize.x) / 2.0f,
                    150 + (60 - textSize.y) / 2.0f};
                DrawTextEx(font, "Hot", textPos, 20, 5, myColor1[0]);
            }
            else
            {
                textSize = MeasureTextEx(font, "Cold", 20, 5);
                textPos = {
                    210 + (180 - textSize.x) / 2.0f,
                    150 + (60 - textSize.y) / 2.0f};
                DrawTextEx(font, "Cold", textPos, 20, 5, myColor1[0]);
            }

            if (!isMuted)
            {
                sourceRec = {0, 0, (float)hotSpeaker.width, (float)hotSpeaker.height};
                destRec = {290, 100, 40, 40};
                origin = {0, 0};
                DrawTexturePro(hotSpeaker, sourceRec, destRec, origin, 0.0f, WHITE);
            }
            else
            {
                sourceRec = {0, 0, (float)hotMuted.width, (float)hotMuted.height};
                destRec = {290, 100, 40, 40};
                origin = {0, 0};
                DrawTexturePro(hotMuted, sourceRec, destRec, origin, 0.0f, WHITE);
            }

            textSize = MeasureTextEx(font, fontList[fontType].c_str(), 20, 5);
            textPos = {
                210 + (180 - textSize.x) / 2.0f,
                30 + (60 - textSize.y) / 2.0f};
            DrawTextEx(font, fontList[fontType].c_str(), textPos, 20, 5, myColor1[0]);
        }
        else
        {
            DrawRectangleRounded({30, 30, 400, 180}, 0.3f, 10, myColor2[2]);
            DrawRectangleRoundedLinesEx({30, 30, 400, 180}, 0.3f, 10, 3, myColor2[0]);
            DrawLineEx({30, 90}, {430, 90}, 3, myColor2[0]);
            DrawLineEx({30, 150}, {430, 150}, 3, myColor2[0]);
            DrawLineEx({190, 30}, {190, 210}, 3, myColor2[0]);

            DrawTriangle({415, 60}, {400, 40}, {400, 80}, myColor2[1]);  
            DrawTriangle({205, 60}, {220, 80}, {220, 40}, myColor2[1]);  
            DrawTriangle({415, 180}, {400, 160}, {400, 200}, myColor2[1]); 
            DrawTriangle({205, 180}, {220, 200}, {220, 160}, myColor2[1]); 

            Vector2 textSize = MeasureTextEx(font, "Font", 20, 5);
            Vector2 textPos = {
                30 + (160 - textSize.x) / 2.0f,
                30 + (60 - textSize.y) / 2.0f};
            DrawTextEx(font, "Font", textPos, 20, 5, myColor2[0]);
            textSize = MeasureTextEx(font, "Music", 20, 5);
            textPos = {
                30 + (160 - textSize.x) / 2.0f,
                90 + (60 - textSize.y) / 2.0f};
            DrawTextEx(font, "Music", textPos, 20, 5, myColor2[0]);
            textSize = MeasureTextEx(font, "Theme", 20, 5);
            textPos = {
                30 + (160 - textSize.x) / 2.0f,
                150 + (60 - textSize.y) / 2.0f};
            DrawTextEx(font, "Theme", textPos, 20, 5, myColor2[0]);

            if (themeView)
            {
                textSize = MeasureTextEx(font, "Hot", 20, 5);
                textPos = {
                    210 + (180 - textSize.x) / 2.0f,
                    150 + (60 - textSize.y) / 2.0f};
                DrawTextEx(font, "Hot", textPos, 20, 5, myColor2[0]);
            }
            else
            {
                textSize = MeasureTextEx(font, "Cold", 20, 5);
                textPos = {
                    210 + (180 - textSize.x) / 2.0f,
                    150 + (60 - textSize.y) / 2.0f};
                DrawTextEx(font, "Cold", textPos, 20, 5, myColor2[0]);
            }

            if (!isMuted)
            {
                sourceRec = {0, 0, (float)coldSpeaker.width, (float)coldSpeaker.height};
                destRec = {290, 100, 40, 40};
                origin = {0, 0};
                DrawTexturePro(coldSpeaker, sourceRec, destRec, origin, 0.0f, WHITE);
            }
            else
            {
                sourceRec = {0, 0, (float)coldMuted.width, (float)coldMuted.height};
                destRec = {290, 100, 40, 40};
                origin = {0, 0};
                DrawTexturePro(coldMuted, sourceRec, destRec, origin, 0.0f, WHITE);
            }

            textSize = MeasureTextEx(font, fontList[fontType].c_str(), 20, 5);
            textPos = {
                210 + (180 - textSize.x) / 2.0f,
                30 + (60 - textSize.y) / 2.0f};
            DrawTextEx(font, fontList[fontType].c_str(), textPos, 20, 5, myColor2[0]);
        }
    }
}

void View::Setting::update()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        if (!isOpen)
        {
            if (CheckCollisionPointRec(mousePos, {20, 20, 40, 40}))
            {
                isOpen = true;
            }
        }
        else
        {
            if (!CheckCollisionPointRec(mousePos, {30, 30, 400, 180}))
            {
                isOpen = false;
            }
            else
            {
                if (CheckCollisionPointTriangle(mousePos, {205, 180}, {220, 200}, {220, 160}) || 
                    CheckCollisionPointTriangle(mousePos, {415, 180}, {400, 160}, {400, 200}))
                {
                    themeView = !themeView;
                    if (themeView)
                        theme = colorType::HOT;
                    else
                        theme = colorType::COLD;
                }

                if (CheckCollisionPointTriangle(mousePos, {205, 60}, {220, 80}, {220, 40}))
                {
                    fontType = (fontType + fontList.size() - 1) % fontList.size();
                }

                if (CheckCollisionPointTriangle(mousePos, {415, 60}, {400, 40}, {400, 80}))
                {
                    fontType = (fontType + 1) % fontList.size();
                }

                font = myFont[fontType];

                if (CheckCollisionPointRec(mousePos, {290, 100, 40, 40}))
                {
                    isMuted = !isMuted;
                    if (isMuted)
                    {
                        PauseMusicStream(music);
                    }
                    else
                    {
                        if (!IsMusicStreamPlaying(music))
                        {
                            PlayMusicStream(music);
                        }
                        else
                        {
                            ResumeMusicStream(music);
                        }
                        SetMusicVolume(music, 0.5f);
                    }
                }
            }
        }
    }
}

//=================================== VIEW ======================================

void View::initView()
{
    if (mode == Mode::SLLIST || mode == Mode::HTABLE || mode == Mode::AVL)
    {
        if (option.isInitialize())
        {
            inputPanel.isOpen = true;
            box.isOpen = false; // Đảm bảo textbox đóng khi mở inputPanel
        }
    }

    func = Function::NONE;
    code.codeline.clear();
    home.hot_icon = LoadTexture("resource\\Texture\\hotHome.png");
    home.cold_icon = LoadTexture("resource\\Texture\\coldHome.png");
    panel.Play = LoadTexture("resource\\Texture\\Play.png");
    panel.Pause = LoadTexture("resource\\Texture\\Pause.png");
    panel.Rewind = LoadTexture("resource\\Texture\\Rewind.png");
    panel.Forward = LoadTexture("resource\\Texture\\Forward.png");
    panel.Final = LoadTexture("resource\\Texture\\Final.png");
    setting.hotSetting = LoadTexture("resource\\Texture\\hotSetting.png");
    setting.coldSetting = LoadTexture("resource\\Texture\\coldSetting.png");
    setting.hotSpeaker = LoadTexture("resource\\Texture\\hotSpeaker.png");
    setting.hotMuted = LoadTexture("resource\\Texture\\hotMuted.png");
    setting.coldSpeaker = LoadTexture("resource\\Texture\\coldSpeaker.png");
    setting.coldMuted = LoadTexture("resource\\Texture\\coldMuted.png");
    if (!IsMusicStreamPlaying(music))
        setting.isMuted = true;
    else
        setting.isMuted = false;
    setting.isOpen = false;

    camera = {0};
    camera.offset = (Vector2){0, 0};
    camera.target = (Vector2){0, 0};
    camera.zoom = 1.0f;
}

void View::drawView()
{
    if (theme == colorType::HOT)
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(myColor1[3], 0.5f));
    else
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(myColor2[3], 0.5f));

    Rectangle title = {0, -10, 1440, 90};
    if (theme == colorType::HOT)
        DrawRectangleRounded(title, 0.3f, 10, myColor1[3]);
    else
        DrawRectangleRounded(title, 0.3f, 10, myColor2[3]);

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
        name = "Graph Algorithm";
    }
    int fontSize = 60;
    int spacing = 10;
    Vector2 textSize = MeasureTextEx(font, name.c_str(), fontSize, spacing);
    Vector2 textPos = {
        (screenWidth - textSize.x) / 2.0f,
        (80 - textSize.y) / 2.0f};
    if (theme == colorType::HOT)
        DrawTextEx(font, name.c_str(), textPos, fontSize, spacing, myColor1[0]);
    else
        DrawTextEx(font, name.c_str(), textPos, fontSize, spacing, myColor2[0]);
    code.draw();
    panel.draw();
    option.draw();
    home.draw();
    log.draw();
    setting.draw();

    if (inputPanel.isOpen)
    {
        inputPanel.draw();
    }

    else if (box.isOpen)
    {
        box.draw();
    }

    slider.draw();
}

void View::eventView()
{

    if (home.isReturnMenu())
    {
        mode = Mode::MENU;
        exit();
    }



    if (inputPanel.isOpen)
    {

        if (mode == Mode::GRAPH)
        {

            if (inputPanel.isRandomPressed())
            {
                inputPanel.isOpen = false;
                box.isFileMode = false;
                box.generateRandomGraphMatrix();
                box.IniFunction = true;
                func = Function::INIT;
            }

            else if (inputPanel.isFilePressed())
            {
                inputPanel.isOpen = false;
                box.isOpen = true;
                box.isFileMode = true;
                box.isDragDropMode = false;    
                box.isURLMode = false; 
                box.value.clear();
                box.adjMatrix.clear();
                box.IniFunction = true;
            }

            else if (inputPanel.isURLPressed())
            {
                inputPanel.isOpen = false;
                box.isOpen = true;
                box.isURLMode = true;
                box.isDragDropMode = true;
                box.isFileMode = false; 
                box.value.clear();
                box.adjMatrix.clear();
                box.IniFunction = true;
            }

            else if (inputPanel.isClosePressed())
            {
                inputPanel.isOpen = false;
                box.IniFunction = false;
                box.isFileMode = false;
            }
        }
        else
        {
            if (inputPanel.isTextboxPressed())
            {
                inputPanel.isOpen = false;
                box.isOpen = true;
                box.isTextboxMode = true;
                box.isURLMode = false;
                box.value.clear();
                box.IniFunction = true;
            }

            else if (inputPanel.isRandomPressed())
            {
                inputPanel.isOpen = false;
                box.isOpen = false;
                box.generateRandomValues();
                box.IniFunction = true;
                func = Function::INIT;

                box.value.clear();
            }
            else if (inputPanel.isURLPressed())
            {
                inputPanel.isOpen = false;
                box.isOpen = true;
                box.isURLMode = true;
                box.isTextboxMode = false;
                box.isDragDropMode = true;
                box.value.clear();
                box.isFileMode = false;
                box.IniFunction = true;
            }
            else if (inputPanel.isClosePressed())
            {
                inputPanel.isOpen = false;
            }
        }
    }

    else
    {
        if (mode == Mode::SLLIST || mode == Mode::AVL || mode == Mode::HTABLE)
        {
            if (option.isInitialize())
            {
                box.isOpen = false;
                inputPanel.isOpen = true;
            }
            else if (!box.IniFunction && (option.isAdd() || option.isDelete() || option.isSearch() || option.isUpdate()))
            {
                box.isOpen = true;
                inputPanel.isOpen = false;
                box.isTextboxMode = true;
                box.isURLMode = false;
                box.value.clear();
            }
        }
        else if (mode == Mode::GRAPH)
        {
            if (option.isInitialize())
            {
                box.isOpen = false;
                inputPanel.isOpen = true;
            }
            else if (option.isAdd() || option.isDelete() || option.isSearch())
            {
                box.isOpen = false;
                inputPanel.isOpen = false;
            }
        }
    }
    if (box.isOpen && box.isDragDropMode)
    {
        box.handleFileDrop(); // Xử lý kéo thả file
        if (!box.isOpen)
        { // Sau khi xử lý file thành công
            // Cập nhật giao diện hoặc dữ liệu
            func = Function::INIT; // Cần dòng này để sau khi textbox đóng, kéo thả file xong thì chuyển về mode INIT
            code.codeline.clear();
            for (int val : box.someList)
            {
                code.codeline.push_back(std::to_string(val));
            }
            if (mode == Mode::HTABLE && box.enteredPrime)
            {
                code.codeline.push_back("Prime: " + std::to_string(box.primeNumber));
            }
            box.IniFunction = false;
        }
    }
    box.update();
    slider.update();
    code.update();
    setting.update();

    if (IsKeyDown(KEY_RIGHT))
        camera.target.x -= 2;
    if (IsKeyDown(KEY_LEFT))
        camera.target.x += 2;
    if (IsKeyDown(KEY_UP))
        camera.target.y += 2;
    if (IsKeyDown(KEY_DOWN))
        camera.target.y -= 2;
    camera.zoom += GetMouseWheelMove() * 0.1f;
}