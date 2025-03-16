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
        struct TextBox{
            std::string value;
            std::vector<int> someList;
            bool isOpen = false;
            void update();
            void draw();
        } box;

        struct Slider {
            Rectangle bound = {50,780,300,15};
            float min = 0.25f, max = 4.0f, speed = 1.0f;
            bool isDragging = false;
            void update();
            void draw();
        } slider;

        virtual ~View() = default;
        void initView();
        void drawView();
        void eventView();
        virtual void exit() = 0;

};

#endif