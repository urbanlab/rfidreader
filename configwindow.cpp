#include "configwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ConfigWindow)
{
  ui->setupUi(this);

  QAction * exitAction;
  QAction * showConfig;

  m_rfid = new RfidTask(this);

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

  m_rfid->start();
}

void ConfigWindow::OnNFCTagDetected(QString tagId)
{
  m_trayIcon->showMessage("NFC tag detected", "ID = "+tagId);
}

void ConfigWindow::OnNFCError(QString err)
{
  m_trayIcon->showMessage("RFID reader error", err);
}

ConfigWindow::~ConfigWindow()
{
  delete ui;
}
