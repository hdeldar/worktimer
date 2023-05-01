#include "Util.h"
#include <QFile>
#include <QMap>
#include <QLocale>

Util::Util(QObject *parent)
	: QObject(parent)
{
}

Util::~Util()
{
}

QString Util::getPersianDate(QString format, QDate today)
{
	QList<QString> week = { "يكشنبه", "دوشنبه", "سه شنبه", "چهارشنبه", "پنج شنبه", "جمعه", "شنبه" };
	QList<QString> months = { "فروردين", "ارديبهشت", "خرداد", "تير", "مرداد", "شهريور", "مهر", "آبان", "آذر", "دي", "بهمن", "اسفند" };
	//QDate today = QDate::currentDate();
	int d = today.dayOfWeek();
	int day = today.day();
	int month = today.month();
	int year = today.year();
	//year = (window.navigator.userAgent.indexOf('MSIE') > 0) ? year : 1900 + year;
	if (year == 0) {
		year = 2000;
	}
	if (year < 100) {
		year += 1900;
	}
	int y = 1;
	for (int i = 0; i < 3000; i += 4) {
		if (year == i) {
			y = 2;
		}
	}
	for (int i = 1; i < 3000; i += 4) {
		if (year == i) {
			y = 3;
		}
	}
	if (y == 1) {
		year -= ((month < 3) || ((month == 3) && (day < 21))) ? 622 : 621;
		switch (month) {
		case 1:
			(day < 21) ? (month = 10, day += 10) : (month = 11, day -= 20);
			break;
		case 2:
			(day < 20) ? (month = 11, day += 11) : (month = 12, day -= 19);
			break;
		case 3:
			(day < 21) ? (month = 12, day += 9) : (month = 1, day -= 20);
			break;
		case 4:
			(day < 21) ? (month = 1, day += 11) : (month = 2, day -= 20);
			break;
		case 5:
		case 6:
			(day < 22) ? (month -= 3, day += 10) : (month -= 2, day -= 21);
			break;
		case 7:
		case 8:
		case 9:
			(day < 23) ? (month -= 3, day += 9) : (month -= 2, day -= 22);
			break;
		case 10:
			(day < 23) ? (month = 7, day += 8) : (month = 8, day -= 22);
			break;
		case 11:
		case 12:
			(day < 22) ? (month -= 3, day += 9) : (month -= 2, day -= 21);
			break;
		default:
			break;
		}
	}
	if (y == 2) {
		year -= ((month < 3) || ((month == 3) && (day < 20))) ? 622 : 621;
		switch (month) {
		case 1:
			(day < 21) ? (month = 10, day += 10) : (month = 11, day -= 20);
			break;
		case 2:
			(day < 20) ? (month = 11, day += 11) : (month = 12, day -= 19);
			break;
		case 3:
			(day < 20) ? (month = 12, day += 10) : (month = 1, day -= 19);
			break;
		case 4:
			(day < 20) ? (month = 1, day += 12) : (month = 2, day -= 19);
			break;
		case 5:
			(day < 21) ? (month = 2, day += 11) : (month = 3, day -= 20);
			break;
		case 6:
			(day < 21) ? (month = 3, day += 11) : (month = 4, day -= 20);
			break;
		case 7:
			(day < 22) ? (month = 4, day += 10) : (month = 5, day -= 21);
			break;
		case 8:
			(day < 22) ? (month = 5, day += 10) : (month = 6, day -= 21);
			break;
		case 9:
			(day < 22) ? (month = 6, day += 10) : (month = 7, day -= 21);
			break;
		case 10:
			(day < 22) ? (month = 7, day += 9) : (month = 8, day -= 21);
			break;
		case 11:
			(day < 21) ? (month = 8, day += 10) : (month = 9, day -= 20);
			break;
		case 12:
			(day < 21) ? (month = 9, day += 10) : (month = 10, day -= 20);
			break;
		default:
			break;
		}
	}
	if (y == 3) {
		year -= ((month < 3) || ((month == 3) && (day < 21))) ? 622 : 621;
		switch (month) {
		case 1:
			(day < 20) ? (month = 10, day += 11) : (month = 11, day -= 19);
			break;
		case 2:
			(day < 19) ? (month = 11, day += 12) : (month = 12, day -= 18);
			break;
		case 3:
			(day < 21) ? (month = 12, day += 10) : (month = 1, day -= 20);
			break;
		case 4:
			(day < 21) ? (month = 1, day += 11) : (month = 2, day -= 20);
			break;
		case 5:
		case 6:
			(day < 22) ? (month -= 3, day += 10) : (month -= 2, day -= 21);
			break;
		case 7:
		case 8:
		case 9:
			(day < 23) ? (month -= 3, day += 9) : (month -= 2, day -= 22);
			break;
		case 10:
			(day < 23) ? (month = 7, day += 8) : (month = 8, day -= 22);
			break;
		case 11:
		case 12:
			(day < 22) ? (month -= 3, day += 9) : (month -= 2, day -= 21);
			break;
		default:
			break;
		}
	}
	if (format == "y/m/d")
		return QString("%1/%2/%3").arg(year).arg(month).arg(day);
	if (format == "ym")
		return QString("%1%2").arg(year).arg(month,2,10,QChar('0'));
	if (format == "y/m")
		return QString("%1/%2").arg(year).arg(month, 2, 10, QChar('0'));
	if (format == "d/m/y") 
		return QString("%1/%2/%3").arg(day).arg(month).arg(year);
	if (month < 1 || month > 12)month = 1;
	if (d < 0 || d > 6)d = 0;
	return QString("%1 %2 %3 %4").arg(week[d]).arg(day).arg(months[month - 1]).arg(year);
}

QString Util::millisecondsToTime(quint64 milliseconds)
{
	quint64 seconds = milliseconds / 1000;
	//milliseconds = milliseconds % 1000;
	return secondsToTime(seconds);
}

QString Util::secondsToTime(quint64 seconds)
{
	quint64 minutes = seconds / 60;
	seconds = seconds % 60;
	quint64 hours = minutes / 60;
	minutes = minutes % 60;
	//quint64 day = hours / 24;
	//day = day % 24;
	return QString("%1:%2:%3").arg(hours).arg(minutes).arg(seconds);
}

QMap<QString, quint64> Util::calculateTaskTotalTime(QString filePath)
{
	QMap<QString, quint64> mtt;
	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly))
	{
		while (!file.atEnd()) {
			QString line = file.readLine();
			QStringList sl = line.split(",");
			if (sl.length() == 5)
			{
				if (mtt.contains(sl.at(0)))
					mtt[sl.at(0)] += sl.at(4).toULongLong();
				else
					mtt[sl.at(0)] = sl.at(4).toULongLong();
			}
		}
	}
	else
	{
		auto e = file.errorString();
	}
	return mtt;
}


void Util::removeTaskFromFile(QString task, QString filePath)
{
	QFile::copy(filePath, filePath + QString("_%1.backup")
		.arg(QLocale().toString(QDateTime::currentDateTime(), "yyyy-MM-dd_HH-mm-ss")));
	QFile file(filePath);
	if (file.open(QIODevice::ReadWrite))
	{
		QStringList remaindLines;
		int readLinesCount = 0;
		while (!file.atEnd()) {
			readLinesCount++;
			QString line = file.readLine();
			QStringList sl = line.split(",");
			if (sl.length() == 5)
			{
				if (sl.at(0) != task)
					remaindLines << line;
			}
		}
		if (remaindLines.length() < readLinesCount)
		{
			file.resize(0);
			for (auto l : remaindLines)
			{
				file.write(l.toUtf8());
			}
			file.flush();
			file.close();
		}
	}
}
