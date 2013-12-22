#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include "rfidtask.h"

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit ConfigWindow(QWidget *parent = 0);
  ~ConfigWindow();
  
public slots:
  void OnNFCTagDetected(QString tagId);
  void OnNFCError(QString err);

private:
  Ui::ConfigWindow *ui;
  QSystemTrayIcon *m_trayIcon;
  QMenu *m_trayMenu;
  RfidTask *m_rfid;
};

#endif // CONFIGWINDOW_H
