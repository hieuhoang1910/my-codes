#include "DiagnosticCamera.h"
#include <iostream>

DiagnosticCamera::DiagnosticCamera()
    : isOpen_(false), frameCounter_(0) {}

bool DiagnosticCamera::open() {
    std::cout << "[DiagnosticCamera] Initializing diagnostics module...\n";
    isOpen_ = true;
    frameCounter_ = 0;
    return true;
}

void DiagnosticCamera::close() {
    if (isOpen_) {
        std::cout << "[DiagnosticCamera] Shutting down diagnostics module...\n";
        isOpen_ = false;
    }
}

bool DiagnosticCamera::captureFrame(Frame& outFrame) {
    if (!isOpen_) {
        std::cerr << "[DiagnosticCamera] Cannot capture, camera not open.\n";
        return false;
    }

    ++frameCounter_;
    outFrame.width       = 1024;
    outFrame.height      = 768;
    outFrame.pixelFormat = "RGB-DIAG";  // pretend some special diagnostic format
    outFrame.sourceName  = name();
    outFrame.frameNumber = frameCounter_;

    std::cout << "[DiagnosticCamera] Captured diagnostic frame #" << frameCounter_ << "\n";
    return true;
}

std::string DiagnosticCamera::name() const {
    return "DiagnosticCamera";
}
