#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include <string>
#include "raylib.h"
#include "GlobalVar.hpp"

class Node {
    public:
        int val;
        Vector2 position;
        //int isHighlighted;
        int ID;
        void drawNode();
        void drawHighlightNode();
        void drawEdge(Node* x);
};

#endif 