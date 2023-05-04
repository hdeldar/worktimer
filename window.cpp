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
#include <QTableWidget>
#include <QApplication>
#include <QScreen>

#pragma comment(lib, "Wininet.lib")

Window::Window(QWidget *parent)
   : QDialog(parent)
   , m_ui(new Ui::Window)
{
	m_ui->setupUi(this);
	m_currentDate = QDate::currentDate();
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

	m_iconList << QIcon(":/images/relax-96.png") << QIcon(":/images/working-96.png") << QIcon(":/images/working-96-1.png");

	m_trayIcon->show();

	setWindowTitle(tr("Work Timer"));
  
	//---
	//updateTaskTable();
	setIcon(0);

	showDate();

	m_ui->durLabel->setText(Util::millisecondsToTime(0));
	m_timer.setInterval(1000);
	connect(&m_timer, &QTimer::timeout, this, &Window::showDuration);
	//---
	
}

Window::~Window()
{
	resting();
}

void Window::updateTaskTable()
{
	QSettings settings;
	m_tasksTotalTime = Util::calculateTaskTotalTime(getLogFilePathName());
	QList<QString> keys = m_tasksTotalTime.keys();
	//QSet<QString> items = settings.value("TasksList").toStringList().toSet();
	//items.unite(keys.toSet());
	//keys = items.toList();
	keys.sort();
	int crow = settings.value("CurrentRow", 0).toInt();
	m_ui->taskTableWidget->blockSignals(true);
	m_ui->taskTableWidget->clear();
	m_ui->taskTableWidget->setStyleSheet("QHeaderView::section { background-color:#abc }QTableWidget { selection-background-color:#a50}");
	m_ui->taskTableWidget->setRowCount(keys.size());
	m_ui->taskTableWidget->setColumnCount(2);
	m_ui->taskTableWidget->setHorizontalHeaderLabels({ "Tasks","Total time" });
	m_ui->taskTableWidget->setColumnWidth(0, 120);
	m_ui->taskTableWidget->setColumnWidth(1, 80);
	m_totalTime = 0;
	for (int i = 0; i < keys.size(); ++i)
	{
		QTableWidgetItem *item = new QTableWidgetItem(keys.at(i));
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
		m_ui->taskTableWidget->setItem(i, 0, item);

		item = new QTableWidgetItem(Util::secondsToTime(m_tasksTotalTime.value(keys.at(i), 0)));
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		m_ui->taskTableWidget->setItem(i, 1, item);
		m_totalTime += m_tasksTotalTime.value(keys.at(i), 0);
	}
	m_ui->taskTableWidget->blockSignals(false);
	m_ui->taskTableWidget->setCurrentCell(crow, 0);
	m_ui->totalLabel->setText(Util::secondsToTime(m_totalTime));
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
	int crow = m_ui->taskTableWidget->currentRow();
	QSettings settings;
	settings.setValue("CurrentRow", crow);

	showDate();
	saveTableItms();
	
	//settings.setValue("TasksList", items);

	
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
		//rejected();
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
		QRect screenrect = QApplication::primaryScreen()->availableGeometry();
		move(screenrect.right() - width(), screenrect.bottom() - height() - 40);
		showNormal();
		activateWindow();
		raise();
// 		if (res == QDialog::Accepted) 
// 		{
// 			if (m_currentState == 0)
// 				working();
// 			else
// 				resting();
// 		}

	 }
        break;
    case QSystemTrayIcon::MiddleClick:
        //showMessage();
        break;
    default:
        ;
    }
}
void Window::showMessage(const QString& message)
{
   QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            
   m_trayIcon->showMessage(tr("Working Timer"),
       message, icon, 100);
}

void Window::on_addTask_clicked(bool checked)
{
	Q_UNUSED(checked)
	int row = m_ui->taskTableWidget->rowCount();
	m_ui->taskTableWidget->blockSignals(true);
	m_ui->taskTableWidget->insertRow(row);
	QTableWidgetItem *item = new QTableWidgetItem("new task");
	item->setText("new task");
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	m_ui->taskTableWidget->setItem(row, 0, item);

	item = new QTableWidgetItem("");
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	m_ui->taskTableWidget->setItem(row, 1, item);
	m_ui->taskTableWidget->blockSignals(false);
	m_ui->taskTableWidget->setCurrentCell(row, 0);
// 	QTableWidgetItem *item = new QTableWidgetItem("new task");
// 	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
// 	m_ui->taskTableWidget->setItem(0,0,item);
// 	m_ui->taskTableWidget->setCurrentItem(item);
}

void Window::on_removeTask_clicked(bool checked)
{
	Q_UNUSED(checked)
	auto res = QMessageBox::question(this, tr("Work Timer"),
			tr("Are you sure to remove current task?"), QMessageBox::No | QMessageBox::Yes);
	if (res == QMessageBox::No) 
		return;
	Util::removeTaskFromFile(m_currentTask, getLogFilePathName());
	QTableWidgetItem *it = m_ui->taskTableWidget->takeItem(m_ui->taskTableWidget->currentRow(),0);
	delete it;
	it = m_ui->taskTableWidget->takeItem(m_ui->taskTableWidget->currentRow(), 1);
	delete it;
	m_ui->taskTableWidget->removeRow(m_ui->taskTableWidget->currentRow());
	saveTableItms();
}
void Window::on_okBtn_clicked(bool checked)
{
	Q_UNUSED(checked)
	//hide();
	//accept();
	if (m_currentState == 0)
		working();
	else
		resting();
}

void Window::on_pmBtn_clicked(bool checked)
{
	Q_UNUSED(checked)
	m_currentDate = m_currentDate.addMonths(-1);
	m_ui->dateLabel->setText(Util::getPersianDate("y/m", m_currentDate));
	updateTaskTable();
}
void Window::on_nmBtn_clicked(bool checked)
{
	Q_UNUSED(checked)
	m_currentDate = m_currentDate.addMonths(1);
	m_ui->dateLabel->setText(Util::getPersianDate("y/m", m_currentDate));
	updateTaskTable();
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
	updateTaskTable();
}

void Window::on_taskTableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
	Q_UNUSED(previous)
	//auto item = m_ui->taskTableWidget->item(currentRow,0);
	if (current)
	{
		m_ui->currentTaskLineEdit->setText(current->text());
		m_currentTask = current->text();
	}
	int crow = m_ui->taskTableWidget->currentRow();
	QSettings settings;
	settings.setValue("CurrentRow", crow);
}

void Window::on_taskTableWidget_itemChanged(QTableWidgetItem *it)
{
	int crow = m_ui->taskTableWidget->currentRow();
	if (it->row() == crow)
	{
		m_ui->currentTaskLineEdit->setText(it->text());
		m_currentTask = it->text();
	}
	saveTableItms();

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
	m_currentTask = getCurrentTask();
	if (m_currentTask.isEmpty())
	{
		m_ui->taskTableWidget->setCurrentCell(0, 0);
	}
	showDate(); 
	m_currentTask = getCurrentTask();
	if (m_currentTask.isEmpty())
	{
		showMessage(tr("Please add task to start timer."));
		return;
	}
	m_ui->pmBtn->setEnabled(false);
	m_ui->nmBtn->setEnabled(false);
	m_ui->addTask->setEnabled(false);
	m_ui->removeTask->setEnabled(false);
	m_ui->pathBtn->setEnabled(false);
	m_ui->taskTableWidget->setEnabled(false);
// 	QRect screenrect = QApplication::primaryScreen()->availableGeometry();
// 	move(screenrect.right() - width(), screenrect.bottom() - height() - 40);
// 	int res = exec();
// 	if (res == QDialog::Rejected) return;
	
	m_elapsedTime.restart();
	m_startTime = Util::getPersianDate() + " " + QLocale().toString(QTime::currentTime(), "HH:mm:ss");
	m_currentState = 1;
	m_ui->okBtn->setText("Stop");
	setIcon(m_currentState);
	m_timer.start();
	if (!m_tasksTotalTime.contains(m_currentTask))
	{
		QString stopTime = Util::getPersianDate() + " " + QLocale().toString(QTime::currentTime(), "HH:mm:ss");
		QString duration = Util::millisecondsToTime(0);
		writeLog(m_currentTask, m_startTime, stopTime, duration, 0);
	}
}

void Window::resting()
{
	if (!m_currentTask.isEmpty() && !m_startTime.isEmpty())
	{
		QString stopTime = Util::getPersianDate() + " " + QLocale().toString(QTime::currentTime(), "HH:mm:ss");
		quint64 elapsed = m_elapsedTime.restart();
		QString duration = Util::millisecondsToTime(elapsed);
		writeLog(m_currentTask, m_startTime, stopTime, duration, elapsed/1000);
		m_currentState = 0;
		m_ui->okBtn->setText("Start");
		setIcon(m_currentState);
		showMessage(tr("Your working time is write on log file."));
		m_timer.stop();
		m_ui->durLabel->setText(Util::millisecondsToTime(0));
		
		elapsed = elapsed / 1000;;
		if (m_tasksTotalTime.contains(m_currentTask))
			m_tasksTotalTime[m_currentTask] += elapsed;
		else
			m_tasksTotalTime[m_currentTask] = elapsed;
	
	}
	//m_currentTask.clear();
	m_startTime.clear();
	showDate();
	m_ui->pmBtn->setEnabled(true);
	m_ui->nmBtn->setEnabled(true);
	m_ui->addTask->setEnabled(true);
	m_ui->removeTask->setEnabled(true);
	m_ui->pathBtn->setEnabled(true);
	m_ui->taskTableWidget->setEnabled(true);
}

QString Window::getLogFilePathName()
{
	return m_logFileDir + "/" + Util::getPersianDate("ym", m_currentDate) + ".csv";
}

void Window::saveTableItms()
{
	int crow = m_ui->taskTableWidget->currentRow();
	QSettings settings;
	settings.setValue("CurrentRow", crow);

// 	QStringList items;
// 	for (int i = 0; i < m_ui->taskTableWidget->rowCount(); ++i)
// 	{
// 		items << m_ui->taskTableWidget->item(i, 0)->text();
// 	}
// 	settings.setValue("TasksList", items);

	QStringList items;
	for (int i = 0; i < m_ui->taskTableWidget->rowCount(); ++i)
	{
		QString itemText = m_ui->taskTableWidget->item(i, 0)->text();
		if (!m_tasksTotalTime.contains(itemText))
		{
			QString startStopTime = Util::getPersianDate() + " " + QLocale().toString(QTime::currentTime(), "HH:mm:ss");
			QString duration = Util::millisecondsToTime(0);
			writeLog(itemText, startStopTime, startStopTime, duration, 0);
		}
	}
}

void Window::writeLog(QString task, QString start, QString stop, QString duration, quint64 elapsed)
{
	QString fileName = getLogFilePathName();
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
		m_logFile->write(QString("%1,%2,%3,%4,%5\r\n").arg(task).arg(start).arg(stop).arg(duration).arg(elapsed).toUtf8());
		m_logFile->flush();
		m_logFile->close();
		QFile::copy(m_logFileName, m_logFileName + ".backup");
	}
}

void Window::showDate()
{
	m_currentDate = QDate::currentDate();
	updateTaskTable();
	m_ui->dateLabel->setText(Util::getPersianDate());
}

void Window::showDuration()
{
	m_ui->durLabel->setText(Util::millisecondsToTime(m_elapsedTime.elapsed()));
	QIcon icon;
	if (m_workingIcon == 0)
	{
		icon = m_iconList.at(1);
		m_trayIcon->setIcon(icon);
	}
	else if (m_workingIcon == 4)
	{
		icon = m_iconList.at(2);
		m_trayIcon->setIcon(icon);
	}
	m_workingIcon++;
	if (m_workingIcon > 8)
		m_workingIcon = 0;
}