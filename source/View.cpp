#include "View.hpp"

void View::CodeBlock::draw() {
    rec = {0,0,480,720};
    DrawRectangleRec(rec,YELLOW);
    if (codeline.size() > 3) {
        codeline.erase(codeline.begin(),codeline.end()-4);
    }
    for (int i = 0; i < codeline.size(); ++i) {
        float fontSize = 20;
        float spacing = 10;
        Vector2 textSize = MeasureTextEx(font,codeline[i].c_str(),fontSize,spacing);
        Vector2 textPos = {
            26*i + (26 - textSize.x)/2.0f,
            (480 - textSize.y)/2.0f
        };
        DrawTextEx(font,codeline[i].c_str(),textPos,fontSize,spacing,WHITE);
    }
}

void View::Panel::draw() {
    rec = {0,720,1440,90};
    DrawRectangleRec(rec,RED);
}

void View::Option::draw() {
    
}

void View::Log::draw() {

}

void View::initView() {

}

void View::drawView() {
    code.draw();
    panel.draw();
    option.draw();
    home.draw();
}

void View::eventView() {
    if (home.isReturnMenu()) {
        mode == Mode::MENU;
    }
}
