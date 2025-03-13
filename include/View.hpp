#ifndef View_hpp
#define View_hpp

#include <iostream>
#include <vector>
#include "raylib.h"
#include "GlobalVar.hpp"

class View {
    public:
        struct Panel {
            Rectangle rec;
            Texture2D Play;
            Texture2D Pause;
            void draw();
        } panel;
        struct Log {
            void draw();
        } log;
        struct CodeBlock {
            Rectangle rec;
            std::vector<std::string> codeline;
            void draw();
        } code;
        struct Option {
            void draw();
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
};

#endif