#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include "CameraDevice.h"

class CameraManager {
public:
    CameraManager();
    ~CameraManager();

    void addCamera(std::unique_ptr<CameraDevice> cam);

    bool openAll();
    void closeAll();

    void startCaptureLoop(int framesPerCamera);
    void waitUntilDone();

private:
    std::vector<std::unique_ptr<CameraDevice>> cameras_;
    std::thread workerThread_;
    std::atomic<bool> running_;
};
