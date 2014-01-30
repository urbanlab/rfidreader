#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QNetworkAccessManager>
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
  void OnReaderReady();

private slots:
  void on_checkStartup_toggled(bool checked);

  void on_checkDebug_toggled(bool checked);

  void on_urlStartup_textChanged(const QString &arg1);

  void on_urlHost_textChanged(const QString &arg1);

  void on_checkAutostart_stateChanged(int arg1);

private:
  QSettings *m_settings;
  Ui::ConfigWindow *ui;
  QSystemTrayIcon *m_trayIcon;
  QMenu *m_trayMenu;
  RfidTask *m_rfid;
  QNetworkAccessManager *m_networkManager;

};

#endif // CONFIGWINDOW_H
