#include "detector.h"
#include <memory>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

#ifdef KS_PLATFORM_LINUX

class LinuxDetector : public Detector {
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
    return std::make_unique<LinuxDetector>();
}

#endif
