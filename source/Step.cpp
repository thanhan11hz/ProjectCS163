#include "Step.hpp"

void StepManager::nextStep() {
    if (currentStep < (int)step.size() - 1) currentStep++;
}

// void StepManager::prevStep() {
//     if (currentStep > 0) currentStep--;
// }

void StepManager::playAll() {
    while (isPlaying && currentStep < (int)step.size() - 1) {
        nextStep();
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / speed)));
    }
    isPlaying = false; // Stop playback when done
}