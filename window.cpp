#include "window.h"
#include <qt_windows.h>
#include <QAction>
#include <QCheckBox>
#include <QDir>
#include <QStandardPaths>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <WinInet.h>
#include <QCloseEvent>
#include <QApplication>
#include <QTimer>
#include <QTime>
#include <QSettings>
#include "ui_window.h"
#include "Util.h"
#include <QListWidget>
#include <QApplication>
#include <QScreen>

#pragma comment(lib, "Wininet.lib")

Window::Window(QWidget *parent)
   : QDialog(parent)
   , m_ui(new Ui::Window)
{
	m_ui->setupUi(this);
	QSettings settings;
	m_logFileDir = settings.value("LogFileDir", "").toString();
	if (m_logFileDir.isEmpty())
	{
		m_logFileDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + QApplication::applicationName();
		QDir dir(m_logFileDir);
		if (!dir.exists())
			dir.mkdir(".");
		settings.setValue("LogFileDir", m_logFileDir);
	}
	m_ui->pathLineEdit->setText(m_logFileDir);
	createActions();
	createTrayIcon();

	connect(m_trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	m_iconList << QIcon(":/images/relax-96.png") << QIcon(":/images/working-96.png") ;

	m_trayIcon->show();

	setWindowTitle(tr("Work Timer"));
  
	//---
	
	QStringList items = settings.value("TasksList").toStringList();
	int crow = settings.value("CurrentRow", 0).toInt();
	for (int i = 0; i < items.length(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem(items.at(i));
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
		m_ui->taskListWidget->addItem(item);
	}
	m_ui->taskListWidget->setCurrentRow(crow);
	setIcon(0);
}

Window::~Window()
{

}

void Window::setVisible(bool visible)
{
    m_minimizeAction->setEnabled(visible);
    m_maximizeAction->setEnabled(!isMaximized());
    m_restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

void Window::closeEvent(QCloseEvent *event)
{
	QSettings settings;
	QStringList items;
	int crow = m_ui->taskListWidget->currentRow();
	for (int i = 0; i < m_ui->taskListWidget->count(); ++i)
	{
		items << m_ui->taskListWidget->item(i)->text();
	}
	settings.setValue("TasksList", items);
	settings.setValue("CurrentRow", crow);
	bool firstTime = settings.value("FirstTime", true).toBool();
    if (m_trayIcon->isVisible()) {
		if (firstTime)
		{
			QMessageBox::information(this, tr("Work Timer"),
				tr("The program will keep running in the "
					"system tray. To terminate the program, "
					"choose <b>Quit</b> in the context menu "
					"of the system tray entry."));
			settings.setValue("FirstTime", false);
		}
        hide();
        event->ignore();
    }
}

void Window::setIcon(int index)
{
   QIcon icon = m_iconList.at(index);
    m_trayIcon->setIcon(icon);
    //setWindowIcon(icon);
    QString str;
    switch (index)
    {
    case 0:
       str = tr("resting");
       break;
    case 1:
       str = tr("working");
       break;
    }
    m_trayIcon->setToolTip(str);
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
	 {
		if (m_currentState == 0)
			working();
		else
			resting();

	 }
        break;
    case QSystemTrayIcon::MiddleClick:
        //showMessage();
        break;
    default:
        ;
    }
}
void Window::showMessage()
{
   QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            
   m_trayIcon->showMessage(tr("Working Timer"),
       tr("Your working time is write on log file."), icon, 100);
}

void Window::on_addTask_clicked(bool checked)
{
	Q_UNUSED(checked)
	QListWidgetItem *item = new QListWidgetItem("new task");
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	m_ui->taskListWidget->addItem(item);
	m_ui->taskListWidget->setCurrentItem(item);
}

void Window::on_removeTask_clicked(bool checked)
{
	Q_UNUSED(checked)
	QListWidgetItem *it = m_ui->taskListWidget->takeItem(m_ui->taskListWidget->currentRow());
	delete it;
}
void Window::on_okBtn_clicked(bool checked)
{
	Q_UNUSED(checked)
	hide();
}

void Window::on_pathBtn_clicked(bool checked)
{
	Q_UNUSED(checked)
	QSettings settings;
	m_logFileDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			m_logFileDir,
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);
	settings.setValue("LogFileDir", m_logFileDir);
	m_ui->pathLineEdit->setText(m_logFileDir);
}

void Window::on_taskListWidget_currentRowChanged(int currentRow)
{
	auto item = m_ui->taskListWidget->item(currentRow);
	if (item)
		m_ui->currentTaskLineEdit->setText(item->text());
}

QString Window::getCurrentTask()
{
	return m_ui->currentTaskLineEdit->text();
}

void Window::createActions()
{
    m_minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(m_minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	m_maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(m_maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

	m_restoreAction = new QAction(tr("&Restore"), this);
    connect(m_restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::createTrayIcon()
{
	m_trayIconMenu = new QMenu(this);
	m_trayIconMenu->addAction(m_minimizeAction);
    //trayIconMenu->addAction(maximizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setContextMenu(m_trayIconMenu);
}


void Window::working()
{
	QRect screenrect = QApplication::primaryScreen()->availableGeometry();
	move(screenrect.right() - width(), screenrect.bottom() - height() - 40);
	exec();
	m_currentTask = getCurrentTask();
	m_elapsedTime.restart();
	m_startTime = Util::getPersianDate() + " " + QLocale().toString(QTime::currentTime(), "HH:mm:ss");
	m_currentState = 1;
	setIcon(m_currentState);
}

void Window::resting()
{
	QString stopTime = Util::getPersianDate() + " " + QLocale().toString(QTime::currentTime(), "HH:mm:ss");
	QString duration =  Util::millisecondsToTime(m_elapsedTime.restart());
	writeLog(m_currentTask, m_startTime, stopTime, duration);
	m_currentState = 0;
	setIcon(m_currentState);
	showMessage();
}

void Window::writeLog(QString task, QString start, QString stop, QString duration)
{
	QString fileName = m_logFileDir + "/" + Util::getPersianDate("ym") + ".csv";
	if (m_logFileName != fileName)
	{
		if (m_logFile)
		{
			m_logFile->flush();
			m_logFile->close();
			m_logFile->deleteLater();
			m_logFile = nullptr;
		}
	}
	m_logFileName = fileName;
	if (!m_logFile)
	{
		m_logFile = new QFile(m_logFileName);
	}
	if (!m_logFile->isOpen())
	{
		m_logFile->open(QIODevice::Append | QIODevice::WriteOnly);
	}
	if (m_logFile->isOpen())
	{
		m_logFile->write(QString("%1,%2,%3,%4\r\n").arg(task).arg(start).arg(stop).arg(duration).toUtf8());
		m_logFile->flush();
		m_logFile->close();
	}
}