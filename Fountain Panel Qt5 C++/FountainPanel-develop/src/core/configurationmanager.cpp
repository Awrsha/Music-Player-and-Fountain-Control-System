#include "configurationmanager.h"
#include <QSettings>
#include <QDebug>

ConfigurationManager::ConfigurationManager(QObject *parent) :
  presureTestRunning(false),
  presureCriticalRunning(false),
  usfountainInterval(100000),
  retryIntervalMS(100),
  ioReadMS(200),
  presureTestTime(0, 5, 0),
  backwashDurationM(8),
  presureTestDurationM(3),
  presureCriticalElapsedM(0),
  presureTestElapsedM(0),
  presureTestMaxElasped(30 * 60),
  presureCriticalMaxElasped(72 * 60)
{
  mutexData = new QMutex;
  loadSettings();
}
QMutex *ConfigurationManager::getMutexData() const
{
  return mutexData;
}

void ConfigurationManager::setMutexData(QMutex *value)
{
  mutexData = value;
}

ProcedureViewForm *ConfigurationManager::getProcedureViewForm() const
{
  return procedureViewForm;
}

void ConfigurationManager::setProcedureViewForm(ProcedureViewForm *value)
{
  procedureViewForm = value;
  procedureViewForm->updateRemainingToPresureTest(presureTestMaxElasped - presureTestElapsedM);
  if (presureCriticalRunning)
    {
      procedureViewForm->updateRemainingToCritical(presureCriticalMaxElasped - presureCriticalElapsedM);
    }
  else
    {

    }
}

void ConfigurationManager::changeTimerState(const bool &timerOn)
{
  if (timerOn && !presureTimer->isActive())
    {
      presureTimer->start();
    }

  if (!timerOn && presureTimer->isActive())
    {
      presureTimer->stop();
    }
}



bool ConfigurationManager::getPresureCriticalRunning() const
{
  return presureCriticalRunning;
}

void ConfigurationManager::setPresureCriticalRunning(bool value)
{
  presureCriticalRunning = value;
  saveSettings();
}

void ConfigurationManager::clearAllPrestureSwitchStateAndCritical()
{
//  presureCriticalRunning = false;
//  presureTestRunning = false;

//  presureCriticalElapsedM = 0;
//  presureTestElapsedM = 0;
  setPresureCriticalRunning(false);
  setPresureCriticalElapsed(0);
  setPresureTestElapsed(0);
}

void ConfigurationManager::clearPrestureSwitchTestTime()
{
  setPresureTestElapsed(0);
}

bool ConfigurationManager::getPresureTestRunning() const
{
  return presureTestRunning;
}

void ConfigurationManager::setPresureTestRunning(bool value)
{
  presureTestRunning = value;
}

double ConfigurationManager::getPresureTestDurationM() const
{
  return presureTestDurationM;
}

void ConfigurationManager::setPresureTestDurationM(double value)
{
  presureTestDurationM = value;
}

double ConfigurationManager::getPresureCriticalMaxElasped() const
{
  return presureCriticalMaxElasped;
}

void ConfigurationManager::setPresureCriticalMaxElasped(const double &value)
{
  presureCriticalMaxElasped = value;
}

double ConfigurationManager::getPresureTestMaxElasped() const
{
  return presureTestMaxElasped;
}

void ConfigurationManager::setPresureTestMaxElasped(const double &value)
{
  presureTestMaxElasped = value;
}

double ConfigurationManager::getPresureCriticalElapsed() const
{
  return presureCriticalElapsedM;
}

void ConfigurationManager::setPresureCriticalElapsed(const double &value)
{
  presureCriticalElapsedM = value;
  saveSettings();
  if (procedureViewForm)
    {
      procedureViewForm->updateRemainingToCritical(presureCriticalMaxElasped - presureCriticalElapsedM);
    }
}

double ConfigurationManager::getPresureTestElapsed() const
{
  return presureTestElapsedM;
}

void ConfigurationManager::setPresureTestElapsed(const double &value)
{
  presureTestElapsedM = value;
  saveSettings();
  if (procedureViewForm)
    {
      procedureViewForm->updateRemainingToPresureTest(presureTestMaxElasped - presureTestElapsedM);
    }
  else
    {
    }
}

double ConfigurationManager::getBackwashDuration() const
{
  return backwashDurationM;
}

void ConfigurationManager::setBackwashDuration(double value)
{
  backwashDurationM = value;
}

void ConfigurationManager::onIncrementPresureElapsed()
{
  setPresureTestElapsed(presureTestElapsedM + presureTimer->interval() / 60 / 1000);

  if (presureCriticalRunning)
    {
      setPresureCriticalElapsed(presureCriticalElapsedM + presureTimer->interval() / 60 / 1000);
    }
}

void ConfigurationManager::init()
{
  presureTimer = new QTimer;
  presureTimer->setInterval(1 * 60 * 1000);
  connect(presureTimer, SIGNAL(timeout()),
          SLOT(onIncrementPresureElapsed()));
}

void ConfigurationManager::saveSettings()
{
  QSettings settings(TEXT_FILE_NAME, QSettings::NativeFormat);

  settings.setValue(TEXT_PRESURE_TEST_ELAPSED_M, presureTestElapsedM);
  settings.setValue(TEXT_PRESURE_CRITICAL_ELAPSED_M, presureCriticalElapsedM);
  settings.setValue(TEXT_PRESURE_CRITICAL_TIME_RUNNING_M, presureCriticalRunning);
}

void ConfigurationManager::loadSettings()
{
  QSettings settings(TEXT_FILE_NAME, QSettings::NativeFormat);
  if( settings.contains(TEXT_PRESURE_TEST_ELAPSED_M) &&
      settings.contains(TEXT_PRESURE_CRITICAL_ELAPSED_M) &&
      settings.contains(TEXT_PRESURE_CRITICAL_TIME_RUNNING_M))
    {
      qDebug()<<"Configuration Manager: configuration is  available";

      presureTestElapsedM = settings.value(TEXT_PRESURE_TEST_ELAPSED_M).toDouble();
      presureCriticalElapsedM = settings.value(TEXT_PRESURE_CRITICAL_ELAPSED_M).toDouble();
      presureCriticalRunning = settings.value(TEXT_PRESURE_CRITICAL_TIME_RUNNING_M).toBool();
    }
  else
    {
      qDebug()<<"Configuration Manager: configuration is not available";
      saveSettings();
    }
}

QTime ConfigurationManager::getPresureTestTime() const
{
  return presureTestTime;
}

void ConfigurationManager::setPresureTestTime(const QTime &value)
{
  presureTestTime = value;
}

quint16 ConfigurationManager::getIoReadMS() const
{
  return ioReadMS;
}

void ConfigurationManager::setIoReadMS(const quint16 &value)
{
  ioReadMS = value;
}

quint16 ConfigurationManager::getSecsRetryInterval() const
{
  return retryIntervalMS;
}

void ConfigurationManager::setSecsRetryInterval(const quint16 &value)
{
  retryIntervalMS = value;
}

qint64 ConfigurationManager::getUsfountainInterval() const
{
  return usfountainInterval;
}

void ConfigurationManager::setUsfountainInterval(const qint64 &value)
{
  usfountainInterval = value;
}

const QString ConfigurationManager::TEXT_FILE_NAME = "fountain_plaher_settings";
const QString ConfigurationManager::TEXT_PRESURE_TEST_ELAPSED_M = "presure_test_elapsed_m";
const QString ConfigurationManager::TEXT_PRESURE_CRITICAL_ELAPSED_M = "presure_critical_elapsed_m";
const QString ConfigurationManager::TEXT_PRESURE_CRITICAL_TIME_RUNNING_M = "presure_critical_time_running_m";
