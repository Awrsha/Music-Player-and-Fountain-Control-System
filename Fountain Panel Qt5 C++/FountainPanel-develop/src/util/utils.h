#ifndef REFUTILS_H
#define REFUTILS_H

#include <QTime>

class RefUtils
{

public:
  static QTime getElapsedTime(const QTime& time);
  static double getElapsedTimeRelative(const QTime& time);

  static QTime getElapsedDateTime(const QDateTime &dateTime, int& days );
  static double getElapsedDateTimeRelativeHour(const QDateTime &dateTime);
  static double getElapsedDateTimeRelativeMintues(const QDateTime &dateTime);
private:
  RefUtils();

  static const qint64 SECONDS_IN_A_DAY = 86400;

};

#endif // REFUTILS_H
