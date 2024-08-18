#ifndef PROCEDUREMANAGER_H
#define PROCEDUREMANAGER_H

#include <QObject>

#include "src/ui/procedureviewform.h"
#include "digitaliointerfaceclient.h"
#include "dmxinterface.h"

#include <QDateTime>

class ProcedureManager : public QObject, public DigitalIOInterfaceClient
{
  Q_OBJECT
public:
  explicit ProcedureManager(ProcedureViewForm* procedureViewForm, QObject *parent = 0);

  enum ProcedureState
  {
    PRESURE_SWICH_CHECKED = 0,
    PRESURE_SWICH_TEST = 1,
//    PRESURE_SWITCH
  };

  void setBackwashCommandRunning(bool value);

  void setPresureSwitchTestPassed(bool value);

  void setPresureSwitchTestRunning(bool value);

  void setEnteredNormalRoutine(bool value);

  void setIsFinalizing(bool value);

  void setSentPlaySignal(bool value);

  bool getCheckedPresureCritical() const;
  void setCheckedPresureCritical(bool value);

  bool getIsInIdleMode() const;
  void setIsInIdleMode(bool value);

signals:

public slots:

  // DigitalIOInterfaceClient interface
public slots:
  virtual void onReceiveInputUpdate(const DigialInputResponse &response);
  void onFinalizeCommand();
  void onDriveErrorOccured();


signals:
  virtual void sendIOCommand(const AddressBook::OutputRamEntity &address, const bool &value);
  virtual void sendIOCommand(const AddressBook::AddressPair &address, const bool &value);

  void sendPlayPause(const bool& play);


private slots:
  void sendPumpCommand(const int& pumpNumber, const bool& command);
  void turnOnSinglePump(const int& pumpNumber);
  void turnOffSinglePump(const int& pumpNumber);
  void turnOnAllPumps();
  void turnOffAllPumps();

  QVector<quint8> getDmxOffCommand();
  void getBackwashDmxCommand(const int& pumpNumber, QVector<quint8> &dmxFrameData);

  bool isPresureTestRequried();
  bool isPresureswitchCritical();

  void updateBackwashTimeView();



  bool getPumpDriveOutputAddress(const int& pumpNumber, AddressBook::OutputRamEntity& pumpDrive, AddressBook::OutputRamEntity& pumpDriveIndicator);
//  int getPumpNumber(const DigialInputResponse &response);

  bool isPresureSwitchTestFinished();
  bool isPresureSwitchTestRunning();

  bool handlePresureSwitchError(const DigialInputResponse &response);

  bool isBackwashCommandFinished();
  bool isBackwashCommandRunning();

  void startBackwashRoutine(const int &pumpNumber);
  void finalizeBackwashRoutine();

  bool isBackwashTriggered(const DigialInputResponse &response, int &pumpNumber);

  bool handleDriveError(const DigialInputResponse &response);

  void startPresureSwitchTest();
  void finalizePresureSwitchTest();

  void onPresureswitchCritical();

private:
  inline int getDmxAddress(const int& address)
  {
    return address - 1;
  }

  ProcedureViewForm* procedureViewForm;
  DigialInputResponse lastResponse;

  QDateTime presureTestStartTime;
  QDateTime backwashCommandRunTime;

  int lastBackwashedPump;


  bool backwashCommandRunning;
  bool presureSwitchTestPassed;
  bool presureSwitchTestRunning;
  bool enteredNormalRoutine;
  bool isFinalizing;
  bool sentPlaySignal;
  bool isInIdleMode;
  bool checkedPresureCritical;



  static const quint8 NOZZLE_DMX_ON_VALUE;




};

#endif // PROCEDUREMANAGER_H
