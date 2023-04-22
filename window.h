#pragma  once

#include <QSystemTrayIcon>
#include <QTime>
#include <QDialog>
#include <memory>
#include <QElapsedTimer>
#include <QFile>
#include "ui_window.h"
//namespace Ui { class Window; };

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
	~Window();
    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void setIcon(int index);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
	void on_addTask_clicked(bool checked);
	void on_removeTask_clicked(bool checked);
	void on_okBtn_clicked(bool checked);
	void on_pathBtn_clicked(bool checked);
	void on_taskListWidget_currentRowChanged(int currentRow);
private:
    void createActions();
    void createTrayIcon();
	QString getCurrentTask();
	void working();
	void resting();
	void writeLog(QString task, QString start, QString stop, QString duration, quint64 elapsed);

private:
    QAction *m_minimizeAction;
    QAction *m_maximizeAction;
    QAction *m_restoreAction;
    QAction *m_quitAction;

    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
	QElapsedTimer m_elapsedTime;
    std::unique_ptr<Ui::Window> m_ui;
    QList<QIcon> m_iconList;
	bool m_currentState = 0;
    //---
    QString m_connectionName;
    QString m_userName;
    QString m_password;
	QString m_currentTask;
	QString m_startTime;
	//--
	QString m_logFileDir;
	QString m_logFileName;
	QFile* m_logFile = nullptr;
};

