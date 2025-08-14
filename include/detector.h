#pragma once

#include <QString>
#include <QStringList>
#include <vector>
#include <optional>

struct SuspiciousProcess {
    int pid;
    QString name;
    QString reason;
};

class Detector {
public:
    virtual ~Detector() = default;
    virtual std::vector<SuspiciousProcess> scanKeyboardHooks() = 0;
    virtual std::vector<SuspiciousProcess> scanHiddenProcesses() = 0;
    virtual std::vector<SuspiciousProcess> yaraScan(const QString &path) = 0;
};
std::unique_ptr<Detector> createDetector();
