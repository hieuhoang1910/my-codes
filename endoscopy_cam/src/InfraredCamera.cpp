#include "InfraredCamera.h"
#include <iostream>

InfraredCamera::InfraredCamera()
    : isOpen_(false), frameCounter_(0) {}

bool InfraredCamera::open() {
    std::cout << "[InfraredCamera] Opening IR sensor (Linux driver)...\n";
    isOpen_ = true;
    frameCounter_ = 0;
    return true;
}

void InfraredCamera::close() {
    if (isOpen_) {
        std::cout << "[InfraredCamera] Shutting down IR sensor...\n";
        isOpen_ = false;
    }
}

bool InfraredCamera::captureFrame(Frame& outFrame) {
    if (!isOpen_) {
        std::cerr << "[InfraredCamera] Cannot capture, camera not open.\n";
        return false;
    }

    ++frameCounter_;
    outFrame.width       = 1280;
    outFrame.height      = 720;
    outFrame.pixelFormat = "IR16";
    outFrame.sourceName  = name();
    outFrame.frameNumber = frameCounter_;

    std::cout << "[InfraredCamera] Captured IR frame #" << frameCounter_ << "\n";
    return true;
}

std::string InfraredCamera::name() const {
    return "InfraredCamera";
}
