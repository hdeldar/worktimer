#include "Util.h"
#include <QDate>

Util::Util(QObject *parent)
	: QObject(parent)
{
}

Util::~Util()
{
}

QString Util::getPersianDate(QString format)
{
	QList<QString> week = { "يكشنبه", "دوشنبه", "سه شنبه", "چهارشنبه", "پنج شنبه", "جمعه", "شنبه" };
	QList<QString> months = { "فروردين", "ارديبهشت", "خرداد", "تير", "مرداد", "شهريور", "مهر", "آبان", "آذر", "دي", "بهمن", "اسفند" };
	QDate today = QDate::currentDate();
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
	if (format == "d/m/y") 
		return QString("%1/%2/%3").arg(day).arg(month).arg(year);	
	return QString("%1 %2 %3 %4").arg(week[d]).arg(day).arg(months[month - 1]).arg(year);
}

QString Util::millisecondsToTime(quint64 milliseconds)
{
	quint64 seconds = milliseconds / 1000;
	milliseconds = milliseconds % 1000;
	quint64 minutes = seconds / 60;
	seconds = seconds % 60;
	quint64 hours = minutes / 60;
	minutes = minutes % 60;
	//quint64 day = hours / 24;
	//day = day % 24;
	return QString("%1:%2:%3").arg(hours).arg(minutes).arg(seconds);
}