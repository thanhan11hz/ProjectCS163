#ifndef Step_hpp
#define Step_hpp

#include <iostream>
#include <vector>
#include "raylib.h"

struct Step {
    public:
        int highlightedLine;
        std::string description;
        int highlightedNode;
};

class StepManager {
    public:
        std::vector<Step> step;
        int currentStep;
        float speed;
        bool isPlaying;
        StepManager(): currentStep(-1), speed(1.0f), isPlaying(false) {};
        void nextStep();
        void prevStep();
        void goToFinal();
        void playAll();
};

#endif