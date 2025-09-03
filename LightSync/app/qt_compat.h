/*
 * File: qt_compat.h
 * Desc: Conditional Qt includes with fallbacks for static linters
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_QT_COMPAT_H
#define LIGHTSYNC_QT_COMPAT_H

#if defined(__has_include)
#  if __has_include(<QtWidgets/QApplication>)
#    include <QtWidgets/QApplication>
#    include <QtWidgets/QMainWindow>
#    include <QtWidgets/QWidget>
#    include <QtWidgets/QVBoxLayout>
#    include <QtWidgets/QLabel>
#    include <QtCore/QDir>
#    include <QtCore/QStandardPaths>
#  else
// Minimal stubs to satisfy static analysis when Qt headers are not available
#    include <string>

class QVBoxLayout;

class QWidget {
public:
    virtual ~QWidget() {}
    void setLayout(QVBoxLayout*) {}
};

class QMainWindow {
public:
    explicit QMainWindow(QWidget* = nullptr) {}
    virtual ~QMainWindow() {}
    void setCentralWidget(QWidget*) {}
    void show() {}
};

class QApplication {
public:
    QApplication(int&, char**) {}
    int exec() { return 0; }
};

class QVBoxLayout {
public:
    void addWidget(QWidget*) {}
};

class QLabel : public QWidget {
public:
    explicit QLabel(const char*, QWidget* = nullptr) {}
};

class QString {
public:
    QString() = default;
    explicit QString(const char*) {}
    bool isEmpty() const { return false; }
    std::u32string toStdU32String() const { return std::u32string(); }
    friend QString operator+(const QString&, const char*) { return QString(); }
};

class QDir {
public:
    static QString homePath() { return QString("~"); }
    void mkpath(const QString&) {}
};

class QStandardPaths {
public:
    enum StandardLocation { AppDataLocation };
    static QString writableLocation(StandardLocation) { return QString("."); }
};
#  endif
#endif

#endif // LIGHTSYNC_QT_COMPAT_H


