#include "GlobalVar.hpp"

// Set value for the size of the screen
const int screenWidth = 1440;
const int screenHeight = 810;

// Determine the data structure to display
Mode mode;

Function func = Function::NONE;

Font font;

int globalID = 0;

colorType theme = colorType::HOT;

std::vector<Color> myColor1 = {
    (Color){248, 222, 34, 255},
    (Color){249, 76, 16, 255},
    (Color){199, 0, 57, 255},
    (Color){144, 12, 63, 255}
};

std::vector<Color> myColor2 = {
    (Color){180, 255, 220, 255},
    (Color){16, 160, 160, 255},
    (Color){30, 50, 100, 255},
    (Color){70, 30, 90, 255}
};

std::vector<Font> myFont(5);

Music music;