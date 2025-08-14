#include "detector.h"
#include <memory>

#ifdef KS_PLATFORM_WINDOWS
#include <windows.h>

class WindowsDetector : public Detector {
public:
    std::vector<SuspiciousProcess> scanKeyboardHooks() override {
        std::vector<SuspiciousProcess> results;
        return results;
    }

    std::vector<SuspiciousProcess> scanHiddenProcesses() override {
        std::vector<SuspiciousProcess> results;
        return results;
    }

    std::vector<SuspiciousProcess> yaraScan(const QString &path) override {
        std::vector<SuspiciousProcess> results;
        Q_UNUSED(path);
        return results;
    }
};

std::unique_ptr<Detector> createDetector() {
    return std::make_unique<WindowsDetector>();
}
#endif
