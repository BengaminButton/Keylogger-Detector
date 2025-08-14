#pragma once
#include <QMainWindow>
#include <QTimer>
#include <memory>
#include "detector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onScanOnce();
    void onToggleRealtime(bool checked);
    void onAddAutostart();
    void onRemoveAutostart();
    void onToggleCapture(bool checked);

private:
    void appendLog(const QString &line);
    void applySoftTheme();
    bool addToAutostart();
    bool removeFromAutostart();

    std::unique_ptr<Detector> detector_;
    QTimer realtimeTimer_;
    bool capturing_ = false;
    Ui::MainWindow *ui;
};
