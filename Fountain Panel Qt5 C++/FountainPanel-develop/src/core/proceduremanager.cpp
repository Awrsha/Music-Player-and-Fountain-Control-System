#include "proceduremanager.h"
#include "src/util/utils.h"

#include "configurationmanager.h"

#include <QDebug>
#include <QProcess>
#include <QThread>


ProcedureManager::ProcedureManager(ProcedureViewForm *procedureViewForm, QObject *parent) :
  QObject(parent),
  procedureViewForm(procedureViewForm),
  enteredNormalRoutine(false),
  isFinalizing(false),
  backwashCommandRunning(false),
  presureSwitchTestPassed(false),
  presureSwitchTestRunning(false),
  sentPlaySignal(false),
  checkedPresureCritical(false),
  isInIdleMode(false),
  lastBackwashedPump(-1)
{
}

void ProcedureManager::onReceiveInputUpdate(const DigialInputResponse &response)
{
  bool driveErrorResult = handleDriveError(response)/*false*/;

  if (driveErrorResult)
    {
      setIsInIdleMode(false);

      ConfigurationManager::getInstance().changeTimerState(false);

      procedureViewForm->setProcedureStateView(ProcedureViewForm::DRIVE_ERROR);

      //do2
      // turn off everything.
      DmxInterface::sendDmxData(getDmxOffCommand());
      turnOffAllPumps();
      setSentPlaySignal(false);

      //send signal to door
      //same as final finish, except don't send signal
      //else clear signal
      //onDriveErrorOccured();

      //maybe?
      //return;
    }
  else if (response.getResponse(AddressBook::INPUT_POWER_LOSS_STATUS))
    {
      setSentPlaySignal(false);
      procedureViewForm->setProcedureStateView(ProcedureViewForm::POWER_LOSS);
      //finalize raspberry
      onFinalizeCommand();
      return;
    }
  else if (response.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_ROUTINE))
    {
      ConfigurationManager::getInstance().changeTimerState(false);
      procedureViewForm->setProcedureStateView(ProcedureViewForm::BACK_WASH_ROUTINE);
      setIsInIdleMode(false);
      // backwash routine
      if (!enteredNormalRoutine)
        {
          //stay in pump for n minitues
          //if true till 8 mintues wait and then
          //if pump command is turn off, stop everything.
          //clear requreid presure switch
          //send zero command
          // clear time when backwashing
          if (isBackwashCommandFinished())
            {
              procedureViewForm->setBackWashState(ProcedureViewForm::FINISHED);
              //finalize finish
              finalizeBackwashRoutine();
              handlePresureSwitchError(response);
              procedureViewForm->setPresureCritical(false);
            }
          else if (isBackwashCommandRunning())
            {
              procedureViewForm->setBackWashState(ProcedureViewForm::RUNNING);
              updateBackwashTimeView();
              //is there anything to do?
            }
          else
            {
              procedureViewForm->setBackWashState(ProcedureViewForm::WAITING);
              //compare with previues command
              int pumpNumber;
              if (isBackwashTriggered(response, pumpNumber))
                {
                  //do 2 2 mode waiting
                  procedureViewForm->setPumpBackwashActive(pumpNumber);
                  startBackwashRoutine(pumpNumber);
                }
              else
                {
                  //waiting for trigger command
                }
            }
        }
      else
        {
          procedureViewForm->setBackWashState(ProcedureViewForm::INVALID);
        }
    }
  else if (response.getResponse(AddressBook::INPUT_COMMAND_NORMAL_ROUTINE))
    {
      setIsInIdleMode(false);
      procedureViewForm->setProcedureStateView(ProcedureViewForm::NORMAL_ROUTINE);

      // normal routine
      if (!enteredNormalRoutine)
        {
          //          enteredNormalRoutine = true;
          setEnteredNormalRoutine(true);
        }

      if (response.getResponse(AddressBook::INPUT_COMMAND_FINALIZE) )
        {
          procedureViewForm->setFinalizeView();
          if (!isFinalizing)
            {
              qDebug()<<"Finalizing";
              //              isFinalizing = true;
              setIsFinalizing(true);
              //send slow finalize command -- canceled
              onFinalizeCommand(); //after finishing - canceled
            }
        }
      else
        {
          // check for every pump if drive is ok from input
          // 3 or 2 bits
          // fault OR local
          // drive ready
          // during run
          if (driveErrorResult)
            {
              //anything to do?
            }
          else
            {
              // check only once
              // run time is less than
              if (!presureSwitchTestPassed)
                {
                  if (isPresureSwitchTestFinished())
                    {
                      procedureViewForm->setPresureSwitchState(ProcedureViewForm::FINISHED);
                      // if finished
                      // read state
                      // turn off everything
                      // send singal for activating led on door
                      // if anyone is zero
                      // after the presure switch turned on, wait for 72 hours and then don't turn on eveything
                      handlePresureSwitchError(response);
                      finalizePresureSwitchTest();
                      //                      presureSwitchTestPassed = true;
                      setPresureSwitchTestPassed(true);
                    }
                  else if (isPresureSwitchTestRunning())
                    {
                      procedureViewForm->setPresureSwitchState(ProcedureViewForm::RUNNING);
                      procedureViewForm->setPresureSwitchTime(ConfigurationManager::getInstance().getPresureTestDurationM() -
                                                              RefUtils::getElapsedDateTimeRelativeMintues(presureTestStartTime));
                      //just wait
                    }
                  else if (isPresureTestRequried())
                    {
                      procedureViewForm->setPresureSwitchState(ProcedureViewForm::REQURIED);
                      // run presure swich test
                      startPresureSwitchTest();
                    }
                  else
                    {
                      procedureViewForm->setPresureSwitchState(ProcedureViewForm::PASSED);
                      //                      presureSwitchTestPassed = true;
                      setPresureSwitchTestPassed(true);

                      // presureswitch test is not running and is not required
                    }

                }
              else
                {
                  //normal start

                  if (!checkedPresureCritical && isPresureswitchCritical())
                    {
                      handlePresureSwitchError(response);
                      procedureViewForm->setPresureCritical(true);
                      ConfigurationManager::getInstance().changeTimerState(false);
                      // handling
                      onPresureswitchCritical();
                    }
                  else
                    {

                      if (!checkedPresureCritical)
                        {
                          setCheckedPresureCritical(true);
                        }
                      if (!sentPlaySignal)
                        {
                          ConfigurationManager::getInstance().changeTimerState(true);
                          turnOnAllPumps();
                          setSentPlaySignal(true);
                          // ok
                          // start playlist
                        }
                    }
                }
            }
        }
    }
  else
    {
      if (!isInIdleMode)
        {
          setIsInIdleMode(true);
          procedureViewForm->setProcedureStateView(ProcedureViewForm::IDLE);


          //      backwashCommandRunning = false;
          setBackwashCommandRunning(false);
          //      presureSwitchTestRunning = false;
          setPresureSwitchTestRunning(false);
          //      sentPlaySignal = false;
          setSentPlaySignal(false);
          setCheckedPresureCritical(false);

          setEnteredNormalRoutine(false);


          //send stop signal to playlist
          ConfigurationManager::getInstance().changeTimerState(false);


          this->thread()->msleep(1000);
          // idle
          //check idle state, if in idle, reset everything
          for (int i = 0 ; i < 10; ++i)
            DmxInterface::sendDmxData(getDmxOffCommand());
          this->thread()->msleep(1000);
          turnOffAllPumps();

        }


    }


  lastResponse = response;
}

void ProcedureManager::onFinalizeCommand()
{
  //  QVector<quint8> dmxFrameData;
  //  getFinalizeCommand(dmxFrameData);
  //  DmxInterface::sendDmxData(dmxFrameData);

  // todo ! failed!
  //open all nozzles
  // 2
  //set pump to zero
  // 3
  //close all nozzles
  // 3
  //turn all lights off
  //send frame


  DmxInterface::sendDmxData(getDmxOffCommand());
  turnOffAllPumps();

  //send io command signal turn off
  //shutdown itself
  sendIOCommand(AddressBook::OUTPUT_FINALIZE_FINISHED, true);
  QProcess shutDownProccess;
  shutDownProccess.start("sudo shutdown");
  shutDownProccess.waitForStarted();
  shutDownProccess.waitForFinished();

}

void ProcedureManager::onDriveErrorOccured()
{
  //stop
  //turn on error ui
  //turn on led
}

void ProcedureManager::getBackwashDmxCommand(const int &pumpNumber, QVector<quint8>& dmxFrameData)
{
  dmxFrameData = getDmxOffCommand();
  //todo check pump number

  if (pumpNumber == 0)
    {
      qDebug()<<"Fatal in " << Q_FUNC_INFO;
      return;
    }
  if (pumpNumber == 1)
    {
      dmxFrameData[getDmxAddress(400)] = 50/*10*/;
      //403 - 35
    }
  else if (pumpNumber == 2)
    {
      dmxFrameData[getDmxAddress(401)] = 50/*10*/;
      //402 - 45
    }
  else if (pumpNumber == 3)
    {
      dmxFrameData[getDmxAddress(402)] = 50/*10*/;
      //401 - 45
    }
  else if (pumpNumber == 4)
    {
      dmxFrameData[getDmxAddress(403)] = 25/*10*/;
      //400 - 45
    }
}
bool ProcedureManager::isPresureTestRequried()
{
  return false;
  return !presureSwitchTestPassed &&
      ConfigurationManager::getInstance().getPresureTestElapsed() >=
      ConfigurationManager::getInstance().getPresureTestMaxElasped();
}

bool ProcedureManager::isPresureswitchCritical()
{
  return ConfigurationManager::getInstance().getPresureCriticalElapsed() >=
      ConfigurationManager::getInstance().getPresureCriticalMaxElasped();
}

void ProcedureManager::updateBackwashTimeView()
{
  procedureViewForm->setBackWashTime(
        ConfigurationManager::getInstance().getBackwashDuration() -
        RefUtils::getElapsedDateTimeRelativeMintues(backwashCommandRunTime));
}

void ProcedureManager::sendPumpCommand(const int &pumpNumber, const bool &command)
{
  AddressBook::OutputRamEntity outputPump;
  AddressBook::OutputRamEntity outputPumpIndicator;
  if (getPumpDriveOutputAddress(pumpNumber, outputPump, outputPumpIndicator))
    {
      emit sendIOCommand(outputPump, command);
      emit sendIOCommand(outputPumpIndicator, command);
      //this->thread()->msleep(100);
    }
}

void ProcedureManager::turnOnSinglePump(const int &pumpNumber)
{
  sendPumpCommand(pumpNumber, true);
}

void ProcedureManager::turnOffSinglePump(const int &pumpNumber)
{
  sendPumpCommand(pumpNumber, false);
}

void ProcedureManager::turnOnAllPumps()
{
  for (int pumpNumber = 1; pumpNumber < 5; ++pumpNumber) {
      turnOnSinglePump(pumpNumber);
    }
}

void ProcedureManager::turnOffAllPumps()
{
  for (int pumpNumber = 1; pumpNumber < 5; ++pumpNumber) {
      turnOffSinglePump(pumpNumber);
    }
}

QVector<quint8> ProcedureManager::getDmxOffCommand()
{
//  QVector<quint8> dmxFrameData;
//  dmxFrameData.resize(DmxInterface::DMX_FRAME_COUNT);
//  for (int index = 0; index < DmxInterface::DMX_FRAME_COUNT; ++index) {
//      dmxFrameData[index] = 0;
//    }
//  return dmxFrameData;
  QVector<quint8> dmxFrameData;
  for (int index = 0 ; index < DmxInterface::DMX_FRAME_COUNT; ++index)
    {
      dmxFrameData.push_back(0);
    }
  return dmxFrameData;
}

bool ProcedureManager::getPumpDriveOutputAddress(const int &pumpNumber, AddressBook::OutputRamEntity &pumpDrive, AddressBook::OutputRamEntity &pumpDriveIndicator)
{
  if (pumpNumber == 1)
    {
      pumpDrive = AddressBook::OUTPUT_DRIVE_RUN_1;
      pumpDriveIndicator = AddressBook::OUTPUT_PUMP_INDICATOR_1;
    }
  else if (pumpNumber == 2)
    {
      pumpDrive = AddressBook::OUTPUT_DRIVE_RUN_2;
      pumpDriveIndicator = AddressBook::OUTPUT_PUMP_INDICATOR_2;
    }
  else if (pumpNumber == 3)
    {
      pumpDrive = AddressBook::OUTPUT_DRIVE_RUN_3;
      pumpDriveIndicator = AddressBook::OUTPUT_PUMP_INDICATOR_3;
    }
  else if (pumpNumber == 4)
    {
      pumpDrive = AddressBook::OUTPUT_DRIVE_RUN_4;
      pumpDriveIndicator = AddressBook::OUTPUT_PUMP_INDICATOR_4;
    }
  else
    {
      return false;
    }
  return true;
}

//int ProcedureManager::getPumpNumber(const DigialInputResponse &response)
//{

//}

bool ProcedureManager::isPresureSwitchTestFinished()
{
  return presureSwitchTestRunning && RefUtils::getElapsedDateTimeRelativeMintues(presureTestStartTime) > ConfigurationManager::getInstance().getPresureTestDurationM();
}

bool ProcedureManager::isPresureSwitchTestRunning()
{
  return presureSwitchTestRunning;
}

bool ProcedureManager::handlePresureSwitchError(const DigialInputResponse &response)
{
  //  if
  bool presureSwitchError = false;
  QMap<int, bool> stateViewMap;

  if (response.getResponse(AddressBook::INPUT_PRESURE_SWITCH_STATUS_1))
    {
      presureSwitchError = true;
      stateViewMap.insert(1, false);
      sendIOCommand(AddressBook::OUTPUT_PRESURE_SWITCH_INDICATOR_1, true);
    }
  else
    {
      sendIOCommand(AddressBook::OUTPUT_PRESURE_SWITCH_INDICATOR_1, false);
    }
  if (response.getResponse(AddressBook::INPUT_PRESURE_SWITCH_STATUS_2))
    {
      presureSwitchError = true;
      stateViewMap.insert(2, false);
      sendIOCommand(AddressBook::OUTPUT_PRESURE_SWITCH_INDICATOR_2, true);
    }
  else
    {
      sendIOCommand(AddressBook::OUTPUT_PRESURE_SWITCH_INDICATOR_2, false);
    }
  if (response.getResponse(AddressBook::INPUT_PRESURE_SWITCH_STATUS_3))
    {
      presureSwitchError = true;
      stateViewMap.insert(3, false);
      sendIOCommand(AddressBook::OUTPUT_PRESURE_SWITCH_INDICATOR_3, true);
    }
  else
    {
      sendIOCommand(AddressBook::OUTPUT_PRESURE_SWITCH_INDICATOR_3, false);
    }
  if (response.getResponse(AddressBook::INPUT_PRESURE_SWITCH_STATUS_4))
    {
      presureSwitchError = true;
      stateViewMap.insert(4, false);
      sendIOCommand(AddressBook::OUTPUT_PRESURE_SWITCH_INDICATOR_4, true);
    }
  else
    {
      sendIOCommand(AddressBook::OUTPUT_PRESURE_SWITCH_INDICATOR_4, false);
    }

  procedureViewForm->setPresureSwitchStateView(stateViewMap);
  if (presureSwitchError)
    {
      ConfigurationManager::getInstance().setPresureCriticalRunning(true);
    }
}

bool ProcedureManager::isBackwashCommandFinished()
{
  return backwashCommandRunning &&
      RefUtils::getElapsedDateTimeRelativeMintues(backwashCommandRunTime) >= ConfigurationManager::getInstance().getBackwashDuration();
}

bool ProcedureManager::isBackwashCommandRunning()
{
  return backwashCommandRunning;
}

void ProcedureManager::startBackwashRoutine(const int& pumpNumber)
{
  if (pumpNumber != 0)
    {
      turnOnSinglePump(pumpNumber);

      QVector<quint8> dmxFrameData;
      // todo get frame // done
      getBackwashDmxCommand(pumpNumber, dmxFrameData);
      DmxInterface::sendDmxData(dmxFrameData);
      //update the view backwash and pump

      //      backwashCommandRunning = true;
      setBackwashCommandRunning(true);
      backwashCommandRunTime = QDateTime::currentDateTime();
    }
  else
    {
      qDebug()<< "Fatal in " << Q_FUNC_INFO;
    }
}

void ProcedureManager::finalizeBackwashRoutine()
{
  //  backwashCommandRunning = false;
  setBackwashCommandRunning(false);
  DmxInterface::sendDmxData(getDmxOffCommand());
  turnOffAllPumps();

  ConfigurationManager::getInstance().clearAllPrestureSwitchStateAndCritical();
  //todo clear critical and presure time in configuration manager
}

bool ProcedureManager::isBackwashTriggered(const DigialInputResponse &response, int &pumpNumber)
{
  pumpNumber = 0;
  bool result = true;

  if (response.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_PUMP_1)
      && !lastResponse.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_PUMP_1))
    {
      pumpNumber = 1;
    }
  else if (response.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_PUMP_2)
           && !lastResponse.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_PUMP_2))
    {
      pumpNumber = 2;
    }
  else if (response.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_PUMP_3)
           && !lastResponse.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_PUMP_3))
    {
      pumpNumber = 3;
    }
  else if (response.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_PUMP_4)
           && !lastResponse.getResponse(AddressBook::INPUT_COMMAND_BACKWASH_PUMP_4))
    {
      pumpNumber = 4;
    }
  else
    {
      result = false;
    }
  return result;
}

bool ProcedureManager::handleDriveError(const DigialInputResponse &response)
{

  bool driveError = false;

  QMap <int, bool> driveStateMap;

//  qDebug()<< "Status 1" << response.getResponse(AddressBook::INPUT_DRIVE_READY_STATUS_1);
//  qDebug()<< "Status 2" << response.getResponse(AddressBook::INPUT_DRIVE_READY_STATUS_2);
//  qDebug()<< "Status 3" << response.getResponse(AddressBook::INPUT_DRIVE_READY_STATUS_3);
//  qDebug()<< "Status 4" << response.getResponse(AddressBook::INPUT_DRIVE_READY_STATUS_4);

//  qDebug()<< "Fault 4" << response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_4);
//  qDebug()<< "Remote 4" << response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_4);

//    qDebug()<< "Remote 1" << response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_1);
//    qDebug()<< "Remote 2" << response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_2);
//    qDebug()<< "Remote 3" << response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_3);
//    qDebug()<< "Remote 4" << response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_4);


//  qDebug()<< "Fault 1" << response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_1);
//  qDebug()<< "Fault 2" << response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_2);
//  qDebug()<< "Fault 3" << response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_3);
//  qDebug()<< "Fault 4" << response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_4);
  // todo handle ui
  if (!response.getResponse(AddressBook::INPUT_DRIVE_READY_STATUS_1) ||
      response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_1) ||
      response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_1))
    {
      driveStateMap.insert(1, false);
      driveError = true;
    }
  if (!response.getResponse(AddressBook::INPUT_DRIVE_READY_STATUS_2) ||
      response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_2) ||
      response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_2))
    {
      driveStateMap.insert(2, false);
      driveError = true;
    }
  if (!response.getResponse(AddressBook::INPUT_DRIVE_READY_STATUS_3) ||
      response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_3) ||
      response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_3))
    {
      driveStateMap.insert(3, false);
      driveError = true;
    }
  if (!response.getResponse(AddressBook::INPUT_DRIVE_READY_STATUS_4) ||
      response.getResponse(AddressBook::INPUT_DRIVE_FAULT_STATUS_4) ||
      response.getResponse(AddressBook::INPUT_DRIVE_LOCAL_REMOTE_STATUS_4))
    {
      driveStateMap.insert(4, false);
      driveError = true;
    }

  procedureViewForm->setDriveStateView(driveStateMap);
  sendIOCommand(AddressBook::OUTPUT_DRIVE_ERROR_INDICATOR, driveError);

  return driveError;
}

void ProcedureManager::startPresureSwitchTest()
{
  //  presureSwitchTestRunning = true;
  setPresureSwitchTestRunning(true);
  presureTestStartTime = QDateTime::currentDateTime();

  turnOnAllPumps();
  this->thread()->sleep(1);
  QVector<quint8> data = getDmxOffCommand();
  // turn all nozzles on
  // turn the lights red
  // same frequences as before
  for (int i = 0; i < 213; i = 4+i)
    {
      data[i] = NOZZLE_DMX_ON_VALUE;
    }
  DmxInterface::sendDmxData(data);
  this->thread()->sleep(3);
  for (int i = 0; i < 213; i = 4+i)
    {
      data[i+1] = 255;
    }
  DmxInterface::sendDmxData(data);
  this->thread()->sleep(3);
  //todo check pumpnumber
  data[getDmxAddress(400)] = 50/*10*/;
  data[getDmxAddress(401)] = 50/*10*/;
  data[getDmxAddress(402)] = 50/*10*/;
  data[getDmxAddress(403)] = 25/*10*/;
  DmxInterface::sendDmxData(data);



}

void ProcedureManager::finalizePresureSwitchTest()
{
  //  presureSwitchTestRunning = false;
  setPresureSwitchTestRunning(true);
  DmxInterface::sendDmxData(getDmxOffCommand());
  turnOffAllPumps();
  ConfigurationManager::getInstance().clearPrestureSwitchTestTime();
}

void ProcedureManager::onPresureswitchCritical()
{
  DmxInterface::sendDmxData(getDmxOffCommand());
  turnOffAllPumps();
}
bool ProcedureManager::getIsInIdleMode() const
{
  return isInIdleMode;
}

void ProcedureManager::setIsInIdleMode(bool value)
{
  isInIdleMode = value;
}

bool ProcedureManager::getCheckedPresureCritical() const
{
  return checkedPresureCritical;
}

void ProcedureManager::setCheckedPresureCritical(bool value)
{
  checkedPresureCritical = value;
}


void ProcedureManager::setSentPlaySignal(bool value)
{
  sentPlaySignal = value;
  procedureViewForm->setPlayState(value);
  emit sendPlayPause(value);
}

void ProcedureManager::setIsFinalizing(bool value)
{
  isFinalizing = value;
}

void ProcedureManager::setEnteredNormalRoutine(bool value)
{
  enteredNormalRoutine = value;
}

void ProcedureManager::setPresureSwitchTestRunning(bool value)
{
  presureSwitchTestRunning = value;
}

void ProcedureManager::setPresureSwitchTestPassed(bool value)
{
  presureSwitchTestPassed = value;
}

void ProcedureManager::setBackwashCommandRunning(bool value)
{
  backwashCommandRunning = value;
}


const quint8 ProcedureManager::NOZZLE_DMX_ON_VALUE = 0x07;
