#ifndef Step_hpp
#define Step_hpp

#include <iostream>
#include <vector>
#include <thread>
#include "raylib.h"
#include "Node.hpp"
struct Step {
    public:
        int highlightedLine;
        std::vector<std::string> description;
        Node* tempRoot;
        int highlightedNode = -1;

        std::vector<Node *> tempTable;

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
        void playAll();

        
};

#endif