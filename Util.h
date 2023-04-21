#pragma once

#include <QObject>

class Util : public QObject
{
	Q_OBJECT
public:
	Util(QObject *parent);
	~Util();
	static QString getPersianDate(QString format = "y/m/d");
	static QString millisecondsToTime(quint64 milliseconds);
};
