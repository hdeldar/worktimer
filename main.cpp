#include <QApplication>
#include <QMessageBox>
#include "window.h"
#include <QDebug>
#include <QScreen>
#include <QDesktopWidget>
#include <Windows.h>

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	Q_UNUSED(pExceptionInfo)
	return EXCEPTION_CONTINUE_SEARCH;
}


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(worktimer);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("HD");
    QCoreApplication::setOrganizationDomain("HD");
    QCoreApplication::setApplicationName("WorkTimer");
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("WorkTimer"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
	QLocale::setDefault(QLocale::English);
    Window window;
	QRect screenrect = QApplication::primaryScreen()->availableGeometry();
	window.move(screenrect.right() - window.width(), screenrect.bottom() - window.height() - 40);
    window.show();
	::SetUnhandledExceptionFilter(TopLevelExceptionHandler);
    return app.exec();
}
// 
// #else
// 
// #include <QLabel>
// #include <QDebug>
// 
// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);
//     QString text("QSystemTrayIcon is not supported on this platform");
// 
//     QLabel *label = new QLabel(text);
//     label->setWordWrap(true);
// 
//     label->show();
//     qDebug() << text;
// 
//     app.exec();
// }
// 
// #endif
