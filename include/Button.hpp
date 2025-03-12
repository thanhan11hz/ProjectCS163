#ifndef Button_hpp
#define Button_hpp

#include <iostream>
#include "raylib.h"

class Button {
    public:
        Rectangle rec;
        Color color;
        bool isHovered;
        std::string text;
    public:
        //Constructor
        Button() {};
        Button(Rectangle rec, Color color,bool isHovered, std::string text): rec(rec), color(color), isHovered(isHovered), text(text) {};
        // Draw button
        void draw();
        // Check mouse if they is pressed or hovered
        void checkHovered(Vector2 mousePosition);
        bool checkPress(Vector2 mousePosition, bool mousePressed);
};

#endif