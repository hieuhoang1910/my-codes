#include "CameraManager.h"
#include <iostream>
#include <chrono>

CameraManager::CameraManager()
    : running_(false) {}

CameraManager::~CameraManager() {
    running_ = false;
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
    closeAll();
}

void CameraManager::addCamera(std::unique_ptr<CameraDevice> cam) {
    cameras_.push_back(std::move(cam));
}

bool CameraManager::openAll() {
    bool ok = true;
    for (auto& cam : cameras_) {
        if (!cam->open()) {
            std::cerr << "[CameraManager] Failed to open " << cam->name() << "\n";
            ok = false;
        }
    }
    return ok;
}

void CameraManager::closeAll() {
    for (auto& cam : cameras_) {
        cam->close();
    }
}

void CameraManager::startCaptureLoop(int framesPerCamera) {
    if (running_) return;
    running_ = true;

    workerThread_ = std::thread([this, framesPerCamera]() {
        std::cout << "[CameraManager] Capture loop started.\n";

        for (int i = 0; i < framesPerCamera && running_; ++i) {
            for (auto& cam : cameras_) {
                Frame f{};
                if (cam->captureFrame(f)) {
                    std::cout << "  -> Frame from " << f.sourceName
                              << " #" << f.frameNumber
                              << " (" << f.width << "x" << f.height
                              << ", " << f.pixelFormat << ")\n";
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        std::cout << "[CameraManager] Capture loop finished.\n";
        running_ = false;
    });
}

void CameraManager::waitUntilDone() {
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}
