#ifndef GlobalVar_hpp
#define GlobalVar_hpp

#include <iostream>
#include <vector>
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
    SEARCH,
    UPDATE,
};

enum class AnimateType {
    INSERTION,
    DELETION,
    MOVEMENT,
    HIGHLIGHT
};

enum class colorType {
    HOT,
    COLD
};

extern colorType theme;

extern std::vector<Color> myColor1, myColor2;

extern std::vector<Font> myFont;

extern Mode mode;

extern Function func;

extern Font font;

extern int globalID;

extern Music music;

#endif