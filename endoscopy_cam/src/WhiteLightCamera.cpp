#include "WhiteLightCamera.h"
#include <iostream>

WhiteLightCamera::WhiteLightCamera()
    : isOpen_(false), frameCounter_(0) {}

bool WhiteLightCamera::open() {
    std::cout << "[WhiteLightCamera] Opening USB device...\n";
    isOpen_ = true;
    frameCounter_ = 0;
    return true;
}

void WhiteLightCamera::close() {
    if (isOpen_) {
        std::cout << "[WhiteLightCamera] Closing device...\n";
        isOpen_ = false;
    }
}

bool WhiteLightCamera::captureFrame(Frame& outFrame) {
    if (!isOpen_) {
        std::cerr << "[WhiteLightCamera] Cannot capture, camera not open.\n";
        return false;
    }

    ++frameCounter_;
    outFrame.width       = 1920;
    outFrame.height      = 1080;
    outFrame.pixelFormat = "RGB";
    outFrame.sourceName  = name();
    outFrame.frameNumber = frameCounter_;

    std::cout << "[WhiteLightCamera] Captured frame #" << frameCounter_ << "\n";
    return true;
}

std::string WhiteLightCamera::name() const {
    return "WhiteLightCamera";
}
