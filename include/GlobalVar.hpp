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

enum class Function {
    NONE,
    INIT,
    INSERT,
    DELETE,
    SEARCH
};

extern Mode mode;

extern Function func;

extern Font font;

#endif