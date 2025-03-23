#include "Step.hpp"

void StepManager::nextStep() {
    if (currentStep < step.size() - 1) currentStep++;
}

void StepManager::prevStep() {
    if (currentStep > 0) currentStep--;
}

void StepManager::playAll() {
    while (isPlaying && currentStep < step.size() - 1) {
        nextStep();

        // Debug: In ra thông tin bước hiện tại
        std::cout << "Debug: Playing step " << currentStep << std::endl;

        // Đợi một khoảng thời gian trước khi chuyển sang bước tiếp theo
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / speed)));
    }
    isPlaying = false; // Dừng playback khi hoàn thành
}