#include "QtScreenCapturer.h"
#include <QGuiApplication>
#include <QPixmap>

QtScreenCapturer::QtScreenCapturer() : ScreenCapturer() {}

QtScreenCapturer::~QtScreenCapturer()
{
    continue_capture = false;

    if(capture_thread != nullptr)
    {
        capture_thread->join();
        delete capture_thread;
        capture_thread = nullptr;
    }
}

void QtScreenCapturer::SetScreen(int screen_index)
{
    QList<QScreen*> screens = QGuiApplication::screens();

    if(screen_index >= 0 && screen_index < screens.size())
    {
        screen = screens[screen_index];
    }
    else
    {
        screen = nullptr;
    }
}

void QtScreenCapturer::Start()
{
    if(capture_thread == nullptr)
    {
        continue_capture = true;
        capture_thread = new std::thread(&QtScreenCapturer::CaptureThreadFunction, this);
    }
}

void QtScreenCapturer::Stop()
{
    if(capture_thread != nullptr)
    {
        printf("[QtScreenCapturer] Stopping capture thread...\n");
        continue_capture = false;
        capture_thread->join();
        delete capture_thread;
        capture_thread = nullptr;
    }
}

void QtScreenCapturer::CaptureThreadFunction()
{
    printf("[QtScreenCapturer] Thread started\n");

    while(continue_capture)
    {
        int delay = 1000 / framerate;

        if(screen == nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        auto start = std::chrono::steady_clock::now();

        emit OnImage(screen->grabWindow(0).toImage());

        int took = (int) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        int delta = delay - took;

        // sleep 2 ms minimum
        std::this_thread::sleep_for(std::chrono::milliseconds(delta > 2 ? delta : 2));
    }

    printf("[QtScreenCapturer] Thread ended\n");
}
