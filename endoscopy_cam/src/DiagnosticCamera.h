#pragma once
#include "CameraDevice.h"

class DiagnosticCamera : public CameraDevice {
public:
    DiagnosticCamera();

    bool open() override;
    void close() override;
    bool captureFrame(Frame& outFrame) override;
    std::string name() const override;

private:
    bool isOpen_;
    int  frameCounter_;
};
