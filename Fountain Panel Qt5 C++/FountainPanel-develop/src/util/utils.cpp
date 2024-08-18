#include "utils.h"

QTime RefUtils::getElapsedTime(const QTime &time)
{
  if (!time.isValid())
    {
      return (QTime(23,59,59));
    }
  QTime elapsedTime(0,0,0);
  return elapsedTime.addSecs(time.secsTo(QTime::currentTime()));
}

double RefUtils::getElapsedTimeRelative(const QTime &time)
{
  double t;
  QTime elapsedTime = getElapsedTime(time);

  t = elapsedTime.hour();

  t = t + ((double)elapsedTime.minute() / 60.0);

  return t;

}

QTime RefUtils::getElapsedDateTime(const QDateTime &dateTime, int &days)
{
  days = 0;

  if (!dateTime.isValid())
    {
      return (QTime(23,59,59));
    }

  QTime elapsedTime(0,0,0);
  quint64 secondsPassed = dateTime.secsTo(QDateTime::currentDateTime());

  days = secondsPassed / SECONDS_IN_A_DAY;

  return elapsedTime.addSecs(secondsPassed % SECONDS_IN_A_DAY);
}

double RefUtils::getElapsedDateTimeRelativeHour(const QDateTime &dateTime)
{
  double t = 0;
  int days;
  QTime elapsedTime = getElapsedDateTime(dateTime, days);

  t += days * 24;
  t += elapsedTime.hour();

  t += ((double)elapsedTime.minute() / 60.0);

  return t;
}

double RefUtils::getElapsedDateTimeRelativeMintues(const QDateTime &dateTime)
{
  double t = 0;
  int days;
  QTime elapsedTime = getElapsedDateTime(dateTime, days);

  t += days * 24 * 60;
  t += elapsedTime.hour() * 60;

  t += elapsedTime.minute();

  t += static_cast<double>(elapsedTime.second()) / 60.0;

  return t;
}

RefUtils::RefUtils()
{

}
