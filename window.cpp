#include "window.h"
#include <qt_windows.h>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <WinInet.h>
#include <QCloseEvent>
#include <QApplication>
#include <QTimer>
#include <QTime>
#include <QSettings>
#include "ui_window.h"
#pragma comment(lib, "Wininet.lib")
//! [0]
Window::Window(QWidget *parent)
   : QDialog(parent)
   , m_ui(new Ui::Window)
{
   m_ui->setupUi(this);
    //createIconGroupBox();
    //createMessageGroupBox();

    //iconLabel->setMinimumWidth(durationLabel->sizeHint().width());

    createActions();
    createTrayIcon();

    //connect(showMessageButton, SIGNAL(clicked()), this, SLOT(showMessage()));
    //connect(showIconCheckBox, SIGNAL(toggled(bool)), trayIcon, SLOT(setVisible(bool)));
    //connect(iconComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setIcon(int)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(iconGroupBox);
    //mainLayout->addWidget(messageGroupBox);
    //setLayout(mainLayout);
    iconList << QIcon(":/images/disconnect1.png") << QIcon(":/images/connect1.png") << QIcon(":/images/connect2.png") ;

	 //int cindex = getConnectionState();
	 //setIcon(cindex);
    //iconComboBox->setCurrentIndex(cindex);

    trayIcon->show();

    setWindowTitle(tr("Internet Connection"));
    //resize(400, 300);
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &Window::checkConnectionState);
    timer->start(1000);
    //---
    QSettings settings;
    m_connectionName = settings.value("connectionName").toString();
    m_userName = settings.value("userName").toString();
    m_password = QByteArray::fromBase64(QByteArray::fromBase64(
       QByteArray::fromBase64(settings.value("password").toString().toUtf8())));
    m_ui->connectionLineEdit->setText(m_connectionName);
    m_ui->userLineEdit->setText(m_userName);
    m_ui->passwordLineEdit->setText(m_password);
}
//! [0]

//! [1]
void Window::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}
//! [1]

//! [2]
void Window::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Internet Connection"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}
//! [2]

//! [3]
void Window::setIcon(int index)
{
   QIcon icon = iconList.at(index);//iconComboBox->itemIcon(index);
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
    QString str;
    switch (index)
    {
    case 0:
       str = tr("Disconnect");
       break;
    case 1:
       str = tr("Connect");
       break;
    case 2:
       str = tr("Connected");
       break;
    }
    trayIcon->setToolTip(str);
}
//! [3]

//! [4]
void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
	 {
       timer->stop();
       QApplication::processEvents();
		//iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
      //int n = currentState;//iconComboBox->currentIndex();
		if (currentState == 0)
			connectToInternet();
		else
			disconnectFromInternet();

      timer->start(1000);
	 }
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}
//! [4]

//! [5]
void Window::showMessage()
{
   QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Warning);
            //typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
    trayIcon->showMessage(tr("Internet Connection Warning"), 
       tr("Your Internet connection is opened for long time. If your connection has daily limitation and you have forgot to disconnect, you can do now!"), icon,
                          /*durationSpinBox->value()*/10 * 1000);
}
//! [5]

//! [6]
void Window::messageClicked()
{
    QMessageBox::information(0, tr("Internet Connection Warning"),
       tr(R"(Your Internet connection is opened for long time. If your connection has daily limitation and you have forgot to disconnect, you can do now!)"));
}

void Window::checkConnectionState()
{
   setIcon(getConnectionState());
   if (currentState && messageTime.elapsed() > 10*60000)
   {
      showMessage();
      messageTime.restart();
   }
//    QIcon icon = QIcon(":/images/disconnect1.png");
//    DWORD lpdwFlags = 0;
//    BOOL b = InternetGetConnectedState(&lpdwFlags, 0);
//    if (b && (lpdwFlags & INTERNET_RAS_INSTALLED) && 
//       (lpdwFlags & INTERNET_CONNECTION_CONFIGURED) && 
//       (lpdwFlags & INTERNET_CONNECTION_MODEM))
//    {
//       if(blinkConnection)
//          icon = QIcon(":/images/connect2.png");
//       else
//          icon = QIcon(":/images/connect1.png");
//       blinkConnection = !blinkConnection;
//    }
//    trayIcon->setIcon(icon);
}

void Window::on_connectionLineEdit_textEdited(const QString &text)
{
   m_connectionName = text;
}

void Window::on_userLineEdit_textEdited(const QString &text)
{
   m_userName = text;
}

void Window::on_passwordLineEdit_textEdited(const QString &text)
{
   m_password = text;
}

//! [6]

void Window::createIconGroupBox()
{
    iconGroupBox = new QGroupBox(tr("Tray Icon"));

    iconLabel = new QLabel("Icon:");

    iconComboBox = new QComboBox;
    iconComboBox->addItem(QIcon(":/images/connect1.png"), tr("Connect"));
    iconComboBox->addItem(QIcon(":/images/disconnect1.png"), tr("Disconnect"));
    //iconComboBox->addItem(QIcon(":/images/connect2.png"), tr("Connected"));

    showIconCheckBox = new QCheckBox(tr("Show icon"));
    showIconCheckBox->setChecked(true);

    QHBoxLayout *iconLayout = new QHBoxLayout;
    iconLayout->addWidget(iconLabel);
    iconLayout->addWidget(iconComboBox);
    iconLayout->addStretch();
    iconLayout->addWidget(showIconCheckBox);
    iconGroupBox->setLayout(iconLayout);
}

void Window::createMessageGroupBox()
{
    messageGroupBox = new QGroupBox(tr("Balloon Message"));

    typeLabel = new QLabel(tr("Type:"));

    typeComboBox = new QComboBox;
    typeComboBox->addItem(tr("None"), QSystemTrayIcon::NoIcon);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxInformation), tr("Information"),
            QSystemTrayIcon::Information);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxWarning), tr("Warning"),
            QSystemTrayIcon::Warning);
    typeComboBox->addItem(style()->standardIcon(
            QStyle::SP_MessageBoxCritical), tr("Critical"),
            QSystemTrayIcon::Critical);
    typeComboBox->setCurrentIndex(1);

    durationLabel = new QLabel(tr("Duration:"));

    durationSpinBox = new QSpinBox;
    durationSpinBox->setRange(5, 60);
    durationSpinBox->setSuffix(" s");
    durationSpinBox->setValue(15);

    durationWarningLabel = new QLabel(tr("(some systems might ignore this "
                                         "hint)"));
    durationWarningLabel->setIndent(10);

    titleLabel = new QLabel(tr("Title:"));

    titleEdit = new QLineEdit(tr("Cannot connect to network"));

    bodyLabel = new QLabel(tr("Body:"));

    bodyEdit = new QTextEdit;
    bodyEdit->setPlainText(tr("Don't believe me. Honestly, I don't have a "
                              "clue.\nClick this balloon for details."));

    showMessageButton = new QPushButton(tr("Show Message"));
    showMessageButton->setDefault(true);

    QGridLayout *messageLayout = new QGridLayout;
    messageLayout->addWidget(typeLabel, 0, 0);
    messageLayout->addWidget(typeComboBox, 0, 1, 1, 2);
    messageLayout->addWidget(durationLabel, 1, 0);
    messageLayout->addWidget(durationSpinBox, 1, 1);
    messageLayout->addWidget(durationWarningLabel, 1, 2, 1, 3);
    messageLayout->addWidget(titleLabel, 2, 0);
    messageLayout->addWidget(titleEdit, 2, 1, 1, 4);
    messageLayout->addWidget(bodyLabel, 3, 0);
    messageLayout->addWidget(bodyEdit, 3, 1, 2, 4);
    messageLayout->addWidget(showMessageButton, 5, 4);
    messageLayout->setColumnStretch(3, 1);
    messageLayout->setRowStretch(4, 1);
    messageGroupBox->setLayout(messageLayout);
}

void Window::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    //trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}


int Window::getConnectionState()
{
	QProcess process;
	//process.start("cmd", QStringList() << "/c" << "dir");
	process.start("rasdial", QStringList());
	process.waitForFinished();
	QString strOut = process.readAllStandardOutput();
	//qDebug() << strOut;
	if (strOut.contains("No connections"))
		return 0;
   blinkConnection = !blinkConnection;
   if (blinkConnection)
      return 1;
   return 2;
}

void Window::connectToInternet()
{
   currentState = 1;
   messageTime.restart();
   QSettings settings;
   settings.setValue("connectionName", m_connectionName);
   settings.setValue("userName", m_userName);
   settings.setValue("password", m_password.toUtf8().toBase64().toBase64().toBase64());
	QProcess process;
	process.start("rasdial", QStringList() << m_connectionName << m_userName << m_password);
	process.waitForFinished();
   checkConnectionState();
	QString strOut = process.readAllStandardOutput();
	//qDebug() << strOut;
   //QTimer::singleShot(1000, this, &Window::chekConnectionState);
}

void Window::disconnectFromInternet()
{
   currentState = 0;
   //timer->stop();
	QProcess process;
	process.start("rasdial", QStringList() << "/DISCONNECT");
	process.waitForFinished();
   checkConnectionState();
	//qDebug() << process.readAllStandardOutput();
}


