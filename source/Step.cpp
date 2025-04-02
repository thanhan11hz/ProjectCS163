#include "Step.hpp"

void StepManager::nextStep() {
    if (currentStep < step.size() - 1) {
        currentStep++;
        startTransition();
    }
}

void StepManager::prevStep() {
    if (currentStep > 0) {
        currentStep--;
    }
}

void StepManager::playAll() {
    while (isPlaying && currentStep < step.size() - 1) {
        nextStep();
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / speed)));
    }
    isPlaying = false; 
}

void StepManager::startTransition() {
    isTransitioning = true;
}

void StepManager::updateTransitionProgress() {
    step[currentStep].animQueue.update(GetFrameTime());
    if (isTransitionComplete()) {
        finishTransition();
    }
}

bool StepManager::isTransitionComplete() {
    return step[currentStep].animQueue.isComplete();
}

void StepManager::finishTransition() {
    isTransitioning = false;
}