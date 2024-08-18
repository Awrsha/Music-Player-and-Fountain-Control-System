#ifndef PROCEDUREVIEWFORM_H
#define PROCEDUREVIEWFORM_H

#include <QWidget>
#include "stateview.h"

namespace Ui {
  class ProcedureViewForm;
}
typedef QMap<int, bool> IntBoolMap;

class ProcedureViewForm : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureViewForm(QWidget *parent = 0);
  ~ProcedureViewForm();

  enum ProcedureState
  {
    DRIVE_ERROR = 0,
    POWER_LOSS = 1,
    BACK_WASH_ROUTINE = 2,
    NORMAL_ROUTINE = 3,
    IDLE = 4
  };

  enum TimedProcedureState
  {
    FINISHED = 0,
    RUNNING = 1,
    WAITING = 2,
    REQURIED = 3,
    PASSED = 4,
    INVALID = 5
  };

  void setProcedureStateView(const ProcedureViewForm::ProcedureState& procedureState);

  void setDriveStateView(const QMap<int, bool>& driveStateMap);

  void setPresureSwitchStateView(const QMap<int, bool>& presureSwitchStateMap);

  void setBackWashState(const ProcedureViewForm::TimedProcedureState& backwashState);

  void setBackWashTime(const double &min);

  void setPumpBackwashActive(const int& pumpNumber);

  void setFinalizeView();

  void setPresureSwitchState(const ProcedureViewForm::TimedProcedureState& presureSwitchState);

  void setPresureSwitchTime(const double &min);

  void setPresureCritical(const bool &state);

  void setPlayState(const bool &state);

  void updateRemainingToPresureTest(const double& remainingMin);

  void updateRemainingToCritical(const double& remainingMin);

  void setPlaybackInfo(const QString& musicName,
                       const QString& fountainName,
                       const QString& listName,
                       const int& currentIndex,
                       const int& listSize);


private slots:
  void setProcedureStateViewQueued(const ProcedureViewForm::ProcedureState& procedureState);

  void setDriveStateViewQueued(const IntBoolMap& driveStateMap);

  void setPresureSwitchStateViewQueued(const IntBoolMap& presureSwitchStateMap);

  void setBackWashStateQueued(const ProcedureViewForm::TimedProcedureState& backwashState);

  void setBackWashTimeQueued(const double &min);

  void setPumpBackwashActiveQueued(const int& pumpNumber);

  void setFinalizeViewQueued();

  void setPresureSwitchStateQueued(const ProcedureViewForm::TimedProcedureState& presureSwitchState);

  void setPresureSwitchTimeQueued(const double &min);

  void setPresureCriticalQueued(const bool &state);

  void setPlayStateQueued(const bool &state);

  void updateRemainingToPresureTestQueued(const double& remainingMin);

  void updateRemainingToCriticalQueued(const double& remainingMin);

private:
  void init();
  QMap<int, StateView*> driveStateViewMap;
  QMap<int, StateView*> presureSwichViewMap;
  Ui::ProcedureViewForm *ui;
};

#endif // PROCEDUREVIEWFORM_H
