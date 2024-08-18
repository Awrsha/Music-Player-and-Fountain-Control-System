#ifndef REFUTILS_H
#define REFUTILS_H

#include <QTime>

class RefUtils
{

public:
  static QTime getElapsedTime(const QTime& time);
  static double getElapsedTimeRelative(const QTime& time);


private:
  RefUtils();
};

#endif // REFUTILS_H
