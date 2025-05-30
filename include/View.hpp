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
            Rectangle rec = {0, 720, 1440, 100};
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
        Rectangle scrollBar = {377.5f, 140, 15, 200}; // Scrollbar track
        Rectangle scrollThumb = {377.5f, 140, 15, 40}; // Scrollbar thumb
        std::vector<std::string> codeline;
        int lineHighlighted = -1;
        const int lineHeight = 26;
        const int visibleLines = 8;
        float scrollOffset = 0;
        float maxScrollOffset = 0;
        bool isDragging = false;
        float dragOffsetY = 0;
        
        void draw();
        void update();
        void calculateMaxScroll();
        } code;
        struct Option {
            Vector2 postion = {400, 545};
            bool isInitialize();
            bool isAdd();
            bool isDelete();
            bool isSearch();
            bool isUpdate();
            void draw();
        } option;
        struct Home {
            Texture2D hot_icon;
            Texture2D cold_icon;
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
            std::vector<std::vector<int>> adjMatrix;
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

            // For random MODE
            void generateRandomValues();
            void generateRandomGraphMatrix();
            bool IniFunction = false;

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
            float min = 0.25f, max = 7.0f, speed = 1.0f;
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

            Rectangle panelRec = {0, 510, 400, 200};
            Rectangle textboxButton = {startX, startY, buttonWidth, buttonHeight};   // keep
            Rectangle randomButton = {startX, startY + 62, buttonWidth, buttonHeight};
            Rectangle urlButton = {startX + 200, startY, buttonWidth, buttonHeight};  // keep
            Rectangle fileButton = {startX, startY, buttonWidth, buttonHeight};
            Rectangle closeButton = {startX + 200, startY + 62, buttonWidth, buttonHeight}; // keep

            void draw();
            bool isTextboxPressed();
            bool isFilePressed();
            bool isURLPressed();
            bool isClosePressed();
            bool isRandomPressed();

        } inputPanel;


        struct Setting {
            bool isOpen = false;
            bool isMuted = true;
            int fontType = 0;
            std::vector<std::string> fontList = {"Default", "Roboto", "Consolas", "Inter", "OpenSans"};
            bool themeView = true;
            Texture2D hotSetting;
            Texture2D coldSetting;
            Texture2D hotSpeaker;
            Texture2D coldSpeaker;
            Texture2D hotMuted;
            Texture2D coldMuted;
            void draw();
            void update();
        } setting;

        virtual ~View() = default;
        void initView();
        void drawView();
        void eventView();
        virtual void exit() = 0;

        Camera2D camera;

};

#endif