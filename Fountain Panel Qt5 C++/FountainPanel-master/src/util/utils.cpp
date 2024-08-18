#include "utils.h"

QTime RefUtils::getElapsedTime(const QTime &time)
{
  if (!time.isValid())
    {
      return (QTime(100,0,0));
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

RefUtils::RefUtils()
{

}
