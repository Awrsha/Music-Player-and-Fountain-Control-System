#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include <QMutex>

#include "src/ui/procedureviewform.h"

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

  quint16 getSecsRetryInterval() const;
  void setSecsRetryInterval(const quint16 &value);

  quint16 getIoReadMS() const;
  void setIoReadMS(const quint16 &value);

  QTime getPresureTestTime() const;
  void setPresureTestTime(const QTime &value);

  double getBackwashDuration() const;
  void setBackwashDuration(double value);

  double getPresureTestElapsed() const;
  void setPresureTestElapsed(const double &value);

  double getPresureCriticalElapsed() const;
  void setPresureCriticalElapsed(const double &value);

  double getPresureTestMaxElasped() const;
  void setPresureTestMaxElasped(const double &value);

  double getPresureCriticalMaxElasped() const;
  void setPresureCriticalMaxElasped(const double &value);

  double getPresureTestDurationM() const;
  void setPresureTestDurationM(double value);

  bool getPresureTestRunning() const;
  void setPresureTestRunning(bool value);

  bool getPresureCriticalRunning() const;
  void setPresureCriticalRunning(bool value);


  void clearAllPrestureSwitchStateAndCritical();
  void clearPrestureSwitchTestTime();


  ProcedureViewForm *getProcedureViewForm() const;
  void setProcedureViewForm(ProcedureViewForm *value);

  void changeTimerState(const bool& timerOn);

  QMutex *getMutexData() const;
  void setMutexData(QMutex *value);

private slots:

public slots:

  void onIncrementPresureElapsed();
  void init();

private:
  void saveSettings();
  void loadSettings();
  explicit ConfigurationManager(QObject *parent = 0);

  ProcedureViewForm* procedureViewForm;

  qint64 usfountainInterval;
  quint16 retryIntervalMS;
  quint16 ioReadMS;

  QTime presureTestTime;

  double backwashDurationM;
  double presureTestDurationM;

  QTimer* presureTimer;

  double presureTestElapsedM;
  double presureCriticalElapsedM;

  bool presureTestRunning;
  bool presureCriticalRunning;


  double presureTestMaxElasped;
  double presureCriticalMaxElasped;

  QMutex *mutexData;

  static const QString TEXT_FILE_NAME;
  static const QString TEXT_PRESURE_TEST_ELAPSED_M;
  static const QString TEXT_PRESURE_CRITICAL_ELAPSED_M;
  static const QString TEXT_PRESURE_CRITICAL_TIME_RUNNING_M;

};

#endif // CONFIGURATIONMANAGER_H
