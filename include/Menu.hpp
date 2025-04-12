#ifndef Menu_hpp
#define Menu_hpp

#include <iostream>
#include <vector>
#include "raylib.h"
#include "GlobalVar.hpp"
#include "Button.hpp"

class Menu {
    private:
        std::vector<Button> button;
        std::string title = "Data Structure Visualization";
        std::string logo = "Project CS163 by Group 7";
        Texture2D hotList;
        Texture2D hotTable;
        Texture2D hotTree;
        Texture2D hotGraph;
        Texture2D coldList;
        Texture2D coldTable;
        Texture2D coldTree;
        Texture2D coldGraph;
    public:
        void init();
        void draw();
        int modePresson();
        void update();
};

#endif 