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
#    include <QtCore/QString>
#    include <QtCore/QMetaObject>
#    include <QtCore/Qt>
#  else
// Minimal stubs to satisfy static analysis when Qt headers are not available
#    include <string>

// Qt meta-object macros stubs for linters
#    ifndef Q_OBJECT
#      define Q_OBJECT
#    endif
#    ifndef slots
#      define slots
#    endif
#    ifndef signals
#      define signals public
#    endif

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLayoutItem;
class QLayout;
class QMouseEvent;
class QResizeEvent;

namespace Qt { enum AlignmentFlag { AlignCenter = 0x0004 }; enum ConnectionType { AutoConnection, DirectConnection, QueuedConnection }; }

class QWidget {
public:
    explicit QWidget(QWidget* = nullptr) {}
    virtual ~QWidget() {}
    void setLayout(QVBoxLayout*) {}
    void setLayout(QGridLayout*) {}
    void show() {}
    virtual void mousePressEvent(QMouseEvent*) {}
    virtual void resizeEvent(QResizeEvent*) {}
    int width() const { return 800; }
    void setMinimumSize(int, int) {}
    void setStyleSheet(const char*) {}
    void setFixedSize(int, int) {}
    void deleteLater() {}
};

class QMainWindow : public QWidget {
public:
    explicit QMainWindow(QWidget* p = nullptr) : QWidget(p) {}
    virtual ~QMainWindow() {}
    void setCentralWidget(QWidget*) {}
    QWidget* centralWidget() { return nullptr; }
    void resize(int, int) {}
};

class QMetaObject {
public:
    static bool invokeMethod(void*, const char*, int) { return true; }
    template<typename F>
    static bool invokeMethod(void*, F f, int) { f(); return true; }
};

class QApplication {
public:
    QApplication(int&, char**) {}
    int exec() { return 0; }
};

class QVBoxLayout {
public:
    void addWidget(QWidget*) {}
    void addLayout(QHBoxLayout*) {}
    void setSpacing(int) {}
    void setContentsMargins(int, int, int, int) {}
    void setAlignment(QWidget*, int) {}
    void addStretch(int = 0) {}
    int count() const { return 0; }
    QLayoutItem* takeAt(int) { return nullptr; }
};

class QHBoxLayout {
public:
    void addWidget(QWidget*) {}
    void setSpacing(int) {}
    void setContentsMargins(int, int, int, int) {}
    void addStretch(int = 0) {}
    int count() const { return 0; }
    QLayoutItem* takeAt(int) { return nullptr; }
};

class QGridLayout {
public:
    void addWidget(QWidget*, int, int) {}
    void setSpacing(int) {}
    void setContentsMargins(int, int, int, int) {}
    int count() const { return 0; }
    QLayoutItem* takeAt(int) { return nullptr; }
};

class QLayoutItem {
public:
    QWidget* widget() { return nullptr; }
    QLayout* layout() { return nullptr; }
};

class QString; // forward decl for stub signatures

class QLabel : public QWidget {
public:
    explicit QLabel(const char*, QWidget* = nullptr) {}
    explicit QLabel(const QString&, QWidget* = nullptr) {}
    void setAlignment(int) {}
};

class QString {
public:
    QString() = default;
    explicit QString(const char*) {}
    bool isEmpty() const { return false; }
    std::u32string toStdU32String() const { return std::u32string(); }
    friend QString operator+(const QString&, const char*) { return QString(); }
    static QString fromUtf8(const char*) { return QString(); }
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


