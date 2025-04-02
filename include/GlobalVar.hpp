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

enum class AnimateType {
    INSERTION,
    DELETION,
    MOVEMENT,
    HIGHLIGHT
};

extern Mode mode;

extern Function func;

extern Font font;

extern int globalID;

#endif