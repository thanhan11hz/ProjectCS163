#ifndef View_hpp
#define View_hpp

#include <iostream>
#include <vector>
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
            std::vector<std::string> codeline;
            int lineHighlighted = -1;
            void draw();
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
        //virtual void exit() = 0;

        struct TextBox{
            std::string value;
            void draw();
            bool isOpen = false;
        } box;
        std::vector<int> someList;
};

#endif