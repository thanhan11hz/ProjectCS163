#ifndef GlobalVar_hpp
#define GlobalVar_hpp

#include <iostream>
#include "raylib.h"

extern const int screenWidth;
extern const int screenHeight;

enum class Mode {
    MENU,
    SLLIST,
    HTABLE,
    AVL,
    GRAPH
};

extern Mode mode;

extern Font font;

#endif