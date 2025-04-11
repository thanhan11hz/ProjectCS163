#ifndef Logic_hpp
#define Logic_hpp

#include <iostream>
#include <vector>
#include <chrono>
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
        std::vector<Node *> nodeHighlight;
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
        bool isTransitioning = false;
        float transitionDuration = 0.5f;
        void startTransition();
        void updateTransitionProgress();
        bool isTransitionComplete();
        void finishTransition();
};

class Logic {
    protected:
        StepManager stepmanager;
    public:
        virtual ~Logic() = default;
        std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
        float accumulatedTime = 0.0f;
        const float stepDuration = 0.5f;    
};

#endif