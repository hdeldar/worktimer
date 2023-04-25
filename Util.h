#pragma once

#include <QObject>
#include <QMap>

class Util : public QObject
{
	Q_OBJECT
public:
	Util(QObject *parent);
	~Util();
	static QString getPersianDate(QString format = "y/m/d");
	static QString millisecondsToTime(quint64 milliseconds);
	static QString secondsToTime(quint64 seconds);
	static QMap<QString, quint64> calculateTaskTotalTime(QString filePath);
};
