#pragma  once

#include <QSystemTrayIcon>
#include <QTime>
#include <QDialog>
#include <memory>

class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
class QTimer;
#include "ui_window.h"
//namespace Ui { class Window; };

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void setIcon(int index);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void messageClicked();
    void checkConnectionState();
    void on_connectionLineEdit_textEdited(const QString &text);
    void on_userLineEdit_textEdited(const QString &text);
    void on_passwordLineEdit_textEdited(const QString &text);
private:
    void createIconGroupBox();
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

	 int getConnectionState();
	 void connectToInternet();
	 void disconnectFromInternet();

    QGroupBox *iconGroupBox;
    QLabel *iconLabel;
    QComboBox *iconComboBox;
    QCheckBox *showIconCheckBox;

    QGroupBox *messageGroupBox;
    QLabel *typeLabel;
    QLabel *durationLabel;
    QLabel *durationWarningLabel;
    QLabel *titleLabel;
    QLabel *bodyLabel;
    QComboBox *typeComboBox;
    QSpinBox *durationSpinBox;
    QLineEdit *titleEdit;
    QTextEdit *bodyEdit;
    QPushButton *showMessageButton;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QTimer *timer; 
    QTime messageTime;
    bool blinkConnection = true;
    std::unique_ptr<Ui::Window> m_ui;
    QList<QIcon> iconList;
    int currentState = 0;

    //---
    QString m_connectionName;
    QString m_userName;
    QString m_password;
};

