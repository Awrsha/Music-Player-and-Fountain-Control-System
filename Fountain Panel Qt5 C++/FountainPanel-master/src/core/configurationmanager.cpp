#include "configurationmanager.h"


ConfigurationManager::ConfigurationManager(QObject *parent) :
  usfountainInterval(10000)
{

}

qint64 ConfigurationManager::getUsfountainInterval() const
{
  return usfountainInterval;
}

void ConfigurationManager::setUsfountainInterval(const qint64 &value)
{
  usfountainInterval = value;
}
