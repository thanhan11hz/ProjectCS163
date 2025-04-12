#ifndef Button_hpp
#define Button_hpp

#include <iostream>
#include "raylib.h"

class Button {
    public:
        Rectangle rec;
        //Color color;
        bool isHovered;
        Texture2D picture;
    public:
        Button() {};
        Button(Rectangle rec, bool isHovered): rec(rec), isHovered(isHovered){};
        void draw();
        void checkHovered(Vector2 mousePosition);
        bool checkPress(Vector2 mousePosition, bool mousePressed);
};

#endif