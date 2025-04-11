#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include <string>
#include <cmath>
#include "raylib.h"
#include "GlobalVar.hpp"

class Node {
    public:
        int val;
        Vector2 position;
        int ID;
        float alpha = 1.0f;
        Color currentColor;
        Color targetColor;
        float progress = 0.0f;
        void draw();
        void drawHighlight();
        void drawAnimation();
};

#endif 