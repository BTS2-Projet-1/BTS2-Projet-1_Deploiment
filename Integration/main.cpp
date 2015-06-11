#include <QApplication>
#include <QTextCodec>
#include "consolewindows.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setDesktopSettingsAware(false);
    #ifdef __WIN32__
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    #endif

    #ifdef __unix__
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    #endif

    ConsoleWindows w;
   // w.setWindowState(w.windowState() ^ Qt::WindowFullScreen);
    w.show();
    
    return app.exec();
}
