#include "procedureviewform.h"
#include "ui_procedureviewform.h"
#include <QTime>
#include "src/core/configurationmanager.h"
#include <QDebug>
#include "src/util/projecttheme.h"



ProcedureViewForm::ProcedureViewForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ProcedureViewForm)
{
  ui->setupUi(this);

  //todofix
  ConfigurationManager::getInstance().setProcedureViewForm(this);
  init();
}

ProcedureViewForm::~ProcedureViewForm()
{
  delete ui;
}

void ProcedureViewForm::setProcedureStateView(const ProcedureViewForm::ProcedureState &procedureState)
{
  QMetaObject::invokeMethod(this,
                            "setProcedureStateViewQueued",
                            Qt::QueuedConnection,
                            Q_ARG(ProcedureViewForm::ProcedureState, procedureState));
}

void ProcedureViewForm::setDriveStateView(const QMap<int, bool> &driveStateMap)
{
  QMetaObject::invokeMethod(this,
                            "setDriveStateViewQueued",
                            Qt::QueuedConnection,
                            Q_ARG(IntBoolMap, driveStateMap));
}

void ProcedureViewForm::setPresureSwitchStateView(const QMap<int, bool> &presureSwitchStateMap)
{
  QMetaObject::invokeMethod(this,
                            "setPresureSwitchStateViewQueued",
                            Qt::QueuedConnection,
                            Q_ARG(IntBoolMap, presureSwitchStateMap));
}

void ProcedureViewForm::setBackWashState(const ProcedureViewForm::TimedProcedureState &backwashState)
{
  QMetaObject::invokeMethod(this,
                            "setBackWashStateQueued",
                            Qt::QueuedConnection,
                            Q_ARG(ProcedureViewForm::TimedProcedureState, backwashState));
}

void ProcedureViewForm::setBackWashTime(const double &min)
{
  QMetaObject::invokeMethod(this,
                            "setBackWashTimeQueued",
                            Qt::QueuedConnection,
                            Q_ARG(double, min));
}

void ProcedureViewForm::setPumpBackwashActive(const int &pumpNumber)
{
  QMetaObject::invokeMethod(this,
                            "setPumpBackwashActiveQueued",
                            Qt::QueuedConnection,
                            Q_ARG(int, pumpNumber));
}

void ProcedureViewForm::setFinalizeView()
{
  QMetaObject::invokeMethod(this,
                            "setFinalizeViewQueued",
                            Qt::QueuedConnection);
}

void ProcedureViewForm::setPresureSwitchState(const ProcedureViewForm::TimedProcedureState &presureSwitchState)
{
  QMetaObject::invokeMethod(this,
                            "setPresureSwitchStateQueued",
                            Qt::QueuedConnection,
                            Q_ARG(ProcedureViewForm::TimedProcedureState, presureSwitchState));
}

void ProcedureViewForm::setPresureSwitchTime(const double &min)
{
  QMetaObject::invokeMethod(this,
                            "setPresureSwitchTimeQueued",
                            Qt::QueuedConnection,
                            Q_ARG(double, min));
}

void ProcedureViewForm::setPresureCritical(const bool &state)
{
  QMetaObject::invokeMethod(this,
                            "setPresureCriticalQueued",
                            Qt::QueuedConnection,
                            Q_ARG(bool, state));
}

void ProcedureViewForm::setPlayState(const bool &state)
{
  QMetaObject::invokeMethod(this,
                            "setPlayStateQueued",
                            Qt::QueuedConnection,
                            Q_ARG(bool, state));
}

void ProcedureViewForm::updateRemainingToPresureTest(const double &remainingMin)
{
  QMetaObject::invokeMethod(this,
                            "updateRemainingToPresureTestQueued",
                            Qt::QueuedConnection,
                            Q_ARG(double, remainingMin));
}

void ProcedureViewForm::updateRemainingToCritical(const double &remainingMin)
{
  QMetaObject::invokeMethod(this,
                            "updateRemainingToCriticalQueued",
                            Qt::QueuedConnection,
                            Q_ARG(double, remainingMin));
}

void ProcedureViewForm::setPlaybackInfo(const QString &musicName, const QString &fountainName, const QString &listName, const int &currentIndex, const int &listSize)
{
  if (!musicName.isEmpty())
    {
      ui->songNameLabel->setText(musicName);
    }
  else
    {
      ui->songNameLabel->setText("-");
    }

  if (!fountainName.isEmpty())
    {
      ui->fountainNameLabel->setText(fountainName);
    }
  else
    {
      ui->fountainNameLabel->setText("-");
    }

  ui->listNameLabel->setText(listName);

  QString indexText = "%1 / %2";
  ui->currentIndexLabel->setText(indexText.arg(QString::number(currentIndex + 1)).arg(QString::number(listSize)));
}

void ProcedureViewForm::setProcedureStateViewQueued(const ProcedureState &procedureState)
{
//  ui->driveStateView->setState(false);
//  ui->backwashModeView->setState(false);
//  ui->normalModeView->setState(false);
//  ui->idleView->setState(false);


  QString procedureText;
  QString icon;
  switch (procedureState) {
    case DRIVE_ERROR:
      {
//        ui->driveStateView->setState(true);
        procedureText = "Drive Error, Aborting.";
        icon = ProjectTheme::Resource::ICON_WARNING;
        break;
      }
    case POWER_LOSS:
      {
        procedureText = "Power Loss, Aborting.";
        icon = ProjectTheme::Resource::ICON_POWER;
        break;
      }
    case BACK_WASH_ROUTINE:
      {
        procedureText = "Backwash Routine";
//        ui->backwashModeView->setState(true);
        icon = ProjectTheme::Resource::ICON_BACKWASH;
        if (/*ui->stackedWidget->currentIndex() != 1*/ui->tabWidget->currentIndex() != 0)
          {
//            ui->stackedWidget->setCurrentIndex(1);
            ui->tabWidget->setCurrentIndex(0);
          }
        break;
      }
    case NORMAL_ROUTINE:
      {
        procedureText = "Normal Routine";
//        ui->normalModeView->setState(true);
        icon = ProjectTheme::Resource::ICON_NORMAL;
        if (/*ui->stackedWidget->currentIndex() != 0*/ui->tabWidget->currentIndex() != 1)
          {
//            ui->stackedWidget->setCurrentIndex(0);
            ui->tabWidget->setCurrentIndex(1);

          }
        break;
      }
    case IDLE:
      {
        procedureText = "Idle";
//        ui->idleView->setState(true);
        icon = ProjectTheme::Resource::ICON_WAITING;
        break;
      }

    default:
      {
        procedureText = "invalid text";
        icon = ProjectTheme::Resource::ICON_WAITING;
        break;
      }
    }
  ui->procedureStateLabel->setText(procedureText);
  ui->procedureStateIconLabel->setPixmap(QPixmap(icon).scaledToWidth(48, Qt::SmoothTransformation));
}

void ProcedureViewForm::setDriveStateViewQueued(const QMap<int, bool> &driveStateMap)
{
  for (int pumpNumber = 1; pumpNumber < 5; ++pumpNumber) {
      if (driveStateMap.contains(pumpNumber))
        {
          driveStateViewMap.value(pumpNumber)->setState(driveStateMap.value(pumpNumber));
        }
      else
        {
          driveStateViewMap.value(pumpNumber)->setState(true);
        }
    }
}

void ProcedureViewForm::setPresureSwitchStateViewQueued(const QMap<int, bool> &presureSwitchStateMap)
{
  for (int pumpNumber = 1; pumpNumber < 5; ++pumpNumber) {
      if (presureSwitchStateMap.contains(pumpNumber))
        {
          presureSwichViewMap.value(pumpNumber)->setState(presureSwitchStateMap.value(pumpNumber));
        }
      else
        {
          presureSwichViewMap.value(pumpNumber)->setState(true);
        }
    }
}

void ProcedureViewForm::setBackWashStateQueued(const ProcedureViewForm::TimedProcedureState &backwashState)
{
  QString text;
  switch (backwashState) {
    case FINISHED:
      {
        ui->backwashTimeLabel->setText("-");
        text = "Backwash Finished.";
        break;
      }
    case RUNNING:
      {
        text = "Backwash Running.";
        break;
      }
    case WAITING:
      {
        ui->backwashTimeLabel->setText("-");
        text = "Waiting for Command.";
        break;
      }
    case INVALID:
      {
        ui->backwashTimeLabel->setText("-");
        text = "Backwash Not Valid.";
        break;
      }
    default:
      {
        text = "Invalid";
        break;
      }
    }

  ui->backwashStateLabel->setText(text);
}

void ProcedureViewForm::setBackWashTimeQueued(const double &min)
{
  ui->backwashTimeLabel->setText(QTime(0, 0, 0).addSecs(min * 60).toString());
}

void ProcedureViewForm::setPumpBackwashActiveQueued(const int &pumpNumber)
{
  if (pumpNumber == 1)
    {
      ui->pump1BackwashView->setState(true);
    }
  else if (pumpNumber == 2)
    {
      ui->pump2BackwashView->setState(true);
    }
  else if (pumpNumber == 3)
    {
      ui->pump3BackwashView->setState(true);
    }
  else if (pumpNumber == 4)
    {
      ui->pump4BackwashView->setState(true);
    }
}

void ProcedureViewForm::setFinalizeViewQueued()
{
  ui->finalizeView->setState(true);
}

void ProcedureViewForm::setPresureSwitchStateQueued(const ProcedureViewForm::TimedProcedureState &presureSwitchState)
{
  QString text;
  switch (presureSwitchState) {
    case FINISHED:
      {
        ui->presureSwitchTestTime->setText("-");
        text = "Presure Swtich Test Finished.";
        break;
      }
    case RUNNING:
      {
        text = "Presure Swtich Test Running.";
        break;
      }
    case REQURIED:
      {
        ui->presureSwitchTestTime->setText("-");
        text = "Presure Swtich Test is Requried.";
        break;
      }
    case PASSED:
      {
        ui->presureSwitchTestTime->setText("-");
        text = "Presure Swtich Test Passed.";
        break;
      }
    default:
      {
        text = "Invalid";
        break;
      }
    }

  ui->presureSwitchTestLabel->setText(text);
}

void ProcedureViewForm::setPresureSwitchTimeQueued(const double &min)
{
  ui->presureSwitchTestTime->setText(QTime(0, 0, 0).addSecs(min * 60).toString());

}

void ProcedureViewForm::setPresureCriticalQueued(const bool& state)
{
  ui->driveCriticalView->setState(state);
}

void ProcedureViewForm::setPlayStateQueued(const bool& state)
{
  ui->playingView->setState(state);
}

void ProcedureViewForm::updateRemainingToPresureTestQueued(const double &remainingMin)
{
  if (remainingMin < 0)
    {
      ui->nextPresureSwitchTestTime->setText("00:00:00");
      return;
    }
  ui->nextPresureSwitchTestTime->setText( QString::number((int)remainingMin / 60) + ":" +
                                      QString::number((int)remainingMin % 60) + ":" +
                                      QString::number(((int)(remainingMin * 100) % 100 * 60)));
}

void ProcedureViewForm::updateRemainingToCriticalQueued(const double &remainingMin)
{
  if (remainingMin < 0)
    {
      ui->presureSwitchCriticalTime->setText("00:00:00");
      return;
    }
  ui->presureSwitchCriticalTime->setText( QString::number((int)remainingMin / 60) + ":" +
                                      QString::number((int)remainingMin % 60) + ":" +
                                      QString::number(((int)(remainingMin * 100) % 100 * 60)));
}

void ProcedureViewForm::init()
{
  driveStateViewMap.insert(1, ui->drive1View);
  driveStateViewMap.insert(2, ui->drive2View);
  driveStateViewMap.insert(3, ui->drive3View);
  driveStateViewMap.insert(4, ui->drive4View);

  presureSwichViewMap.insert(1, ui->presureSwitch1View);
  presureSwichViewMap.insert(2, ui->presureSwitch2View);
  presureSwichViewMap.insert(3, ui->presureSwitch3View);
  presureSwichViewMap.insert(4, ui->presureSwitch4View);

}
