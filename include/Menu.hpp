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
        std::string title = "Visualize Data Structures";
    public:
        // Initialize the new menu
        void init();
        // Draw button, logo, ... in the menu
        void draw();
        // What button of a data structure is pressed
        int modePresson();
};

#endif 