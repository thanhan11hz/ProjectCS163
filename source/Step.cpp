#include "Step.hpp"

void StepManager::nextStep() {
    if (currentStep < (int)step.size()-1) currentStep++;
}

void StepManager::prevStep() {
    if (currentStep > 0) currentStep--;
}
    
void StepManager::goToFinal() {
    currentStep = (int)step.size()-1;
}
    
void StepManager::playAll() { 
    isPlaying = true;
}     