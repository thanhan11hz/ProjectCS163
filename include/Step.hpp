#ifndef Step_hpp
#define Step_hpp

#include <iostream>
#include <vector>
#include <thread>
#include "raylib.h"
#include "Node.hpp"
#include "Edge.hpp"
#include "Animation.hpp"
struct Step {
    public:
        int highlightedLine = -1;
        int highlightedNode = -1;
        std::vector<std::string> description;
        Node* tempRoot;
        std::vector<Node *> tempTable;
        std::vector<Edge*> tempEdge;
        AnimationQueue animQueue;
        std::vector<Node *> nodeHightlight;
        std::vector<Edge *> edgeHighlight;
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
        bool isTransitioning = false;
        float transitionDuration = 0.5f;
        void startTransition();
        void updateTransitionProgress();
        bool isTransitionComplete();
        void finishTransition();
};

#endif