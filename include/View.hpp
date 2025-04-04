#ifndef View_hpp
#define View_hpp

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "raylib.h"
#include "GlobalVar.hpp"
#include "Node.hpp"

class View {
    public:
        struct Panel {
            Rectangle rec = {0, 720, 1440, 90};
            float buttonSize = 50;
            float spacing = 20;
            float totalWidth = 3 * buttonSize + 2 * spacing;
            float startX = 920 - totalWidth / 2;
            float startY = rec.y + (rec.height - buttonSize) / 2;

            Texture2D Play;
            Texture2D Pause;
            Texture2D Rewind;
            Texture2D Forward;
            Texture2D Final;

            void update();
            void draw();
            bool isPlaying = false;
            bool isPlayPressed();
            bool isPausePressed();
            bool isRewindPressed();
            bool isForwardPressed();
            
        } panel;
        struct Log {
            Rectangle rec;
            std::vector<std::string> infor;
            void draw();
        } log;
        struct CodeBlock {
            Rectangle rec;
            Rectangle scrollBar = {400,80,10,320};
            std::vector<std::string> codeline;
            int lineHighlighted = -1;
            const int lineHeight = 26;
            const int visibleLines = 10;  
            float scrollOffset = 0.0f;
            void draw();
            void update();
        } code;
        struct Option {
            Vector2 postion = {400, 545};
            bool isInitialize();
            bool isAdd();
            bool isDelete();
            bool isSearch();
            void draw();
        } option;
        struct Home {
            Texture2D icon;
            void draw();
            bool isReturnMenu();
        } home;

        View() {
            box.parent = this; // Gán con trỏ parent cho box
        }
        struct TextBox{

            View* parent;

            std::string value;
            std::vector<int> someList;

            // for graph
            std::vector<std::vector<int>> ADJmatrix;
            int startedVertex = 0;
            bool processingGraphMatrix = false;

            bool isOpen = false;
            bool isSelected = false;

            bool isFileMode = false;
            bool isURLMode = false;
            bool isTextboxMode = false;
            bool showInputOptions = false;
            bool isClosed = false;
            // For URL MODE

            bool processFileData(const std::string &filePath);
            
            void handleFileDrop();
            bool isDragDropMode = false;
            bool readFileData(const std::string &filePath);

            void update();
            void draw();

            // For HashTable
            int primeNumber = 0;
            bool enteredPrime = false;
            bool enteredValues = false;
            bool showFileButtons = false;
            
            bool readURLData(const std::string &url);
        } box;

        struct Slider {
            Rectangle bound = {50,780,300,15};
            float min = 0.25f, max = 4.0f, speed = 1.0f;
            bool isDragging = false;
            void update();
            void draw();
        } slider;

        struct InputPanel{
            bool isOpen = false;

            float buttonWidth = 120;
            float buttonHeight = 40;
            float startX = 40;
            float startY = 574;

            Rectangle panelRec = {0, 500, 400, 220};
            Rectangle textboxButton = {startX, startY, buttonWidth, buttonHeight};
            Rectangle fileButton = {startX, startY + 62, buttonWidth, buttonHeight};
            Rectangle urlButton = {startX + 200, startY, buttonWidth, buttonHeight};
            Rectangle randomButton = {startX, startY, buttonWidth, buttonHeight};
            Rectangle closeButton = {startX + 200, startY + 62, buttonWidth, buttonHeight};

            void draw();
            bool isTextboxPressed();
            bool isFilePressed();
            bool isURLPressed();
            bool isClosePressed();
            bool isRandomPressed();

        } inputPanel;

        virtual ~View() = default;
    
        void initView();
        void drawView();
        void eventView();
        virtual void exit() = 0;

        Camera2D camera;

};

#endif