#include "Logic.hpp"

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