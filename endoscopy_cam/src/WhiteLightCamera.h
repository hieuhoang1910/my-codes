#pragma once
#include "CameraDevice.h"

class WhiteLightCamera : public CameraDevice {
public:
    WhiteLightCamera();

    bool open() override;
    void close() override;
    bool captureFrame(Frame& outFrame) override;
    std::string name() const override;

private:
    bool isOpen_;
    int  frameCounter_;
};
