#pragma once

#include <QObject>
#include <QMap>
#include <QDate>

class Util : public QObject
{
	Q_OBJECT
public:
	Util(QObject *parent);
	~Util();
	static QString getPersianDate(QString format = "y/m/d", QDate today = QDate::currentDate());
	static QString millisecondsToTime(quint64 milliseconds);
	static QString secondsToTime(quint64 seconds);
	static QMap<QString, quint64> calculateTaskTotalTime(QString filePath);
	static void removeTaskFromFile(QString task, QString filePath);
};
