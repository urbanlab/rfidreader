#include <QUrl>
#include <QNetworkRequest>
#include <QSettings>
#include <QDesktopServices>

#include "configwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ConfigWindow)
{
  ui->setupUi(this);

  QAction * exitAction;
  QAction * showConfig;

  m_settings = new QSettings("Museolab", "Rfid Reader");

  m_rfid = new RfidTask(this);

  m_networkManager = new QNetworkAccessManager(this);

  m_trayMenu = new QMenu(this);
  showConfig = new QAction("Configuration", this);
  m_trayMenu->addAction(showConfig);

  exitAction = new QAction("Exit", this);
  m_trayMenu->addAction(exitAction);

  m_trayIcon = new QSystemTrayIcon(this);
  m_trayIcon->setIcon(QIcon(":/icon/trayicon"));
  m_trayIcon->setContextMenu(m_trayMenu);
  m_trayIcon->show();

  m_trayIcon->showMessage("RFID reader", "Ready", QSystemTrayIcon::NoIcon, 3000);

  connect(showConfig, SIGNAL(triggered()), this, SLOT(showNormal()));
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(m_rfid, SIGNAL(tagDetected(QString)), this, SLOT(OnNFCTagDetected(QString)));
  connect(m_rfid, SIGNAL(errorDetected(QString)), this, SLOT(OnNFCError(QString)));
  connect(ui->exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));


  ui->urlStartup->setText(m_settings->value("urlStartUp","http://host.erasme.org").toString());
  ui->urlHost->setText(m_settings->value("urlHOST","http://host.erasme.org:80/cloud/rfid/AiADSVTkpd/enter").toString());
  ui->checkDebug->setChecked(m_settings->value("showTags", false).toBool());
  ui->checkStartup->setChecked(m_settings->value("startBrowser", true).toBool());
  QDesktopServices::openUrl(ui->urlStartup->text());

  m_rfid->start();
}

void ConfigWindow::OnNFCTagDetected(QString tagId)
{
  if(ui->checkDebug->isChecked())
  {
    m_trayIcon->showMessage("NFC tag detected", "ID = "+tagId);
  }
  QString url = ui->urlHost->text() + "?rfid=" + tagId;
  m_networkManager->get(QNetworkRequest(QUrl(url)));
}

void ConfigWindow::OnNFCError(QString err)
{
  m_trayIcon->showMessage("RFID reader error", err);
}

ConfigWindow::~ConfigWindow()
{
  delete ui;
}

void ConfigWindow::on_checkStartup_toggled(bool checked)
{
  Q_UNUSED(checked);
  m_settings->setValue("startBrowser", ui->checkStartup->isChecked());
}

void ConfigWindow::on_checkDebug_toggled(bool checked)
{
  Q_UNUSED(checked);
  m_settings->setValue("showTags", ui->checkDebug->isChecked());
}

void ConfigWindow::on_urlStartup_textChanged(const QString &arg1)
{
  Q_UNUSED(arg1);
  m_settings->setValue("urlStartUp", ui->urlStartup->text());
}

void ConfigWindow::on_urlHost_textChanged(const QString &arg1)
{
  Q_UNUSED(arg1);
  m_settings->setValue("urlHOST", ui->urlHost->text());
}
