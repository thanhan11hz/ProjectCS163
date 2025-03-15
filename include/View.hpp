#ifndef View_hpp
#define View_hpp

#include <iostream>
#include <vector>
#include <algorithm>
#include "raylib.h"
#include "GlobalVar.hpp"
#include "Node.hpp"

class View {
    public:
        struct Panel {
            Rectangle rec;
            Texture2D Play;
            Texture2D Pause;
            Texture2D Rewind;
            Texture2D Forward;
            Texture2D Final;
            void draw();
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
            const int visibleLines = 12;  
            float scrollOffset = 0.0f;
            void draw();
            void update();
        } code;
        struct Option {
            bool isInitialize();
            bool isAdd();
            bool isDelete();
            bool isSearch();
            void draw();
            Vector2 postion = {400, 545};
        } option;
        struct Home {
            Texture2D icon;
            void draw();
            bool isReturnMenu();
        } home;
        
        virtual ~View() = default;
        void initView();
        void drawView();
        void eventView();
        virtual void exit() = 0;

        struct TextBox{
            std::string value;
            void draw();
            bool isOpen = false;
        } box;
        std::vector<int> someList;

        struct Slider {
            Rectangle bound = {50,757.5,300,15};
            float min = 0.25f, max = 4.0f, speed = 1.0f;
            bool isDragging = false;
            void update();
            void draw();
        } slider;
};

#endif