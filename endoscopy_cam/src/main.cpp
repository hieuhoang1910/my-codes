#include <iostream>
#include "CameraManager.h"
#include "WhiteLightCamera.h"
#include "InfraredCamera.h"
#include "DiagnosticCamera.h"

int main() {
    CameraManager manager;

    manager.addCamera(std::make_unique<WhiteLightCamera>());
    manager.addCamera(std::make_unique<InfraredCamera>());
    manager.addCamera(std::make_unique<DiagnosticCamera>());  // new one

    if (!manager.openAll()) {
        std::cerr << "Failed to open all cameras. Exiting.\n";
        return 1;
    }

    manager.startCaptureLoop(5);
    manager.waitUntilDone();
    manager.closeAll();

    return 0;
}

//g++ -std=c++17 main.cpp CameraManager.cpp WhiteLightCamera.cpp InfraredCamera.cpp DiagnosticCamera.cpp -o endoscopy_demo.exe -- compile
//.\endoscopy_demo.exe -- Run

