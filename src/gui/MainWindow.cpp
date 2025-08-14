#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#include <QDateTime>
#include <QEvent>
#include <QKeyEvent>
#ifdef KS_PLATFORM_WINDOWS
#include <windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("KeyShield – Антикейлоггер");
    resize(1024, 640);

    applySoftTheme();

    detector_ = createDetector();

    connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::onScanOnce);
    connect(ui->realtimeToggle, &QPushButton::toggled, this, &MainWindow::onToggleRealtime);
    connect(ui->captureToggle, &QPushButton::toggled, this, &MainWindow::onToggleCapture);
    connect(ui->addAutostartButton, &QPushButton::clicked, this, &MainWindow::onAddAutostart);
    connect(ui->removeAutostartButton, &QPushButton::clicked, this, &MainWindow::onRemoveAutostart);

    realtimeTimer_.setInterval(5000);
    connect(&realtimeTimer_, &QTimer::timeout, this, &MainWindow::onScanOnce);

    qApp->installEventFilter(this);
    if (ui->authorLabel) ui->authorLabel->setStyleSheet("color:#94a3b8; font-weight:600;");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::applySoftTheme() {
    QString style = R"(
        QMainWindow { background-color: #0f172a; }
        QWidget { color: #e2e8f0; font-size: 16px; }
        QPushButton {
            background-color: #1e293b; color: #e2e8f0; padding: 12px 20px;
            border-radius: 12px; border: 1px solid #334155;
        }
        QPushButton:hover { background-color: #223145; }
        QPushButton:checked { background-color: #3b4759; }
        QTextEdit { background-color: #0b1220; border: 1px solid #334155; border-radius: 12px; padding: 8px; }
        QLabel#authorLabel { color: #94a3b8; font-weight: 600; }
        QStatusBar { color: #94a3b8; }
    )";
    qApp->setStyleSheet(style);
}

void MainWindow::appendLog(const QString &line) {
    auto ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->logView->append(QString("[%1] %2").arg(ts, line));
}

void MainWindow::onScanOnce() {
    appendLog("Сканирование перехватов клавиатуры...");
    auto hooks = detector_->scanKeyboardHooks();
    if (hooks.empty()) appendLog("Перехваты клавиатуры не обнаружены.");
    for (const auto &p : hooks) {
        appendLog(QString("Перехват: pid=%1 имя=%2 причина=%3").arg(p.pid).arg(p.name, p.reason));
    }

    appendLog("Сканирование скрытых процессов...");
    auto hidden = detector_->scanHiddenProcesses();
    if (hidden.empty()) appendLog("Скрытые процессы не найдены.");
    for (const auto &p : hidden) {
        appendLog(QString("Скрытый: pid=%1 имя=%2 причина=%3").arg(p.pid).arg(p.name, p.reason));
    }
}

void MainWindow::onToggleCapture(bool checked) {
    capturing_ = checked;
    ui->captureToggle->setText(checked ? "Захват: ВКЛ" : "Захват: ВЫКЛ");
    appendLog(checked ? "Захват включён." : "Захват выключен.");
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    Q_UNUSED(obj);
    if (!capturing_) return false;
    if (event->type() == QEvent::KeyPress) {
        auto *ke = static_cast<QKeyEvent*>(event);
        QString text = ke->text();
        if (!text.isEmpty()) {
            QString shown = text;
            shown.replace('\n', "<Enter>").replace('\r', "").replace('\t', "<Tab>");
            appendLog(QString("Key: %1").arg(shown));
        } else {
            appendLog(QString("KeyCode: %1").arg(ke->key()));
        }
    }
    return false;
}

void MainWindow::onToggleRealtime(bool checked) {
    if (checked) {
        realtimeTimer_.start();
        ui->realtimeToggle->setText("Защита: ВКЛ");
        appendLog("Защита в реальном времени включена.");
    } else {
        realtimeTimer_.stop();
        ui->realtimeToggle->setText("Защита: ВЫКЛ");
        appendLog("Защита в реальном времени выключена.");
    }
}

void MainWindow::onAddAutostart() {
    if (addToAutostart()) {
        appendLog("Добавлено в автозапуск.");
        QMessageBox::information(this, "Автозапуск", "KeyShield добавлен в автозапуск.");
    } else {
        QMessageBox::warning(this, "Автозапуск", "Не удалось добавить в автозапуск.");
    }
}

void MainWindow::onRemoveAutostart() {
    if (removeFromAutostart()) {
        appendLog("Удалено из автозапуска.");
        QMessageBox::information(this, "Автозапуск", "KeyShield удалён из автозапуска.");
    } else {
        QMessageBox::warning(this, "Автозапуск", "Не удалось убрать из автозапуска.");
    }
}

bool MainWindow::addToAutostart() {
#ifdef KS_PLATFORM_WINDOWS
    QSettings reg("HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run", QSettings::NativeFormat);
    QString exe = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    reg.setValue("KeyShield", '"' + exe + '"');
    return reg.status() == QSettings::NoError;
#elif defined(KS_PLATFORM_LINUX)
    QString autostartDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";
    QDir().mkpath(autostartDir);
    QString desktopFile = autostartDir + "/KeyShield.desktop";
    QFile f(desktopFile);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream out(&f);
        out << "[Desktop Entry]\n";
        out << "Type=Application\n";
        out << "Name=KeyShield\n";
        out << "Exec=" << QCoreApplication::applicationFilePath() << "\n";
        out << "X-GNOME-Autostart-enabled=true\n";
        f.close();
        return true;
    }
    return false;
#else
    return false;
#endif
}

bool MainWindow::removeFromAutostart() {
#ifdef KS_PLATFORM_WINDOWS
    QSettings reg("HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run", QSettings::NativeFormat);
    reg.remove("KeyShield");
    return reg.status() == QSettings::NoError;
#elif defined(KS_PLATFORM_LINUX)
    QString desktopFile = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/KeyShield.desktop";
    return QFile::remove(desktopFile);
#else
    return false;
#endif
}
