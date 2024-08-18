#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QStringList>


class ConfigurationManager : public QObject
{
  Q_OBJECT
public:

  static ConfigurationManager& getInstance()
  {
    static ConfigurationManager instance;
    return instance;
  }

  qint64 getUsfountainInterval() const;
  void setUsfountainInterval(const qint64 &value);

private:

  explicit ConfigurationManager(QObject *parent = 0);



  qint64 usfountainInterval;

};

#endif // CONFIGURATIONMANAGER_H
