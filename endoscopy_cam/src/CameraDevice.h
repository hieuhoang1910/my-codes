#pragma once
#include <string>
#include "Frame.h"

class CameraDevice {
public:
    virtual ~CameraDevice() = default;

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool captureFrame(Frame& outFrame) = 0;
    virtual std::string name() const = 0;
};
