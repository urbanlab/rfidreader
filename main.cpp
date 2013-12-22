#include <QtGui/QApplication>
#include <QMessageBox>

#include "configwindow.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(icon);

  QApplication a(argc, argv);
  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
      QMessageBox::critical(0, QObject::tr("Systray"),
                            QObject::tr("I couldn't detect any system tray "
                                        "on this system."));
      return 1;
  }

  QApplication::setQuitOnLastWindowClosed(false);
  ConfigWindow w;

  //QTimer::singleShot(0, &rfid, SLOT(run()));


  //w.show();
  
  return a.exec();
}
