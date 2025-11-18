#pragma once
#include <string>

struct Frame {
    int width{};
    int height{};
    std::string pixelFormat;   // e.g. "RGB", "IR16", etc.
    std::string sourceName;    // which camera produced it
    int frameNumber{};
};
