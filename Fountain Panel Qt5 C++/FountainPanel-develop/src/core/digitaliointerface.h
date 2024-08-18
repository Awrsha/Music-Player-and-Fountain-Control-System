#ifndef DIGITALIOINTERFACE_H
#define DIGITALIOINTERFACE_H

#include <QObject>
#include <QThread>
#include "configurationmanager.h"

#include "digialinputresponse.h"
#include "addressbook.h"



class DigitalIOInterface : public QThread
{
  Q_OBJECT
public:
  explicit DigitalIOInterface(QObject *parent = 0);


signals:
  void receivedInputUpdate(const DigialInputResponse& response);
  void receivedOutputUpdate(const DigialOutputResponse& response);

public slots:
  void onSendIOCommand(const AddressBook::OutputRamEntity& outputEntity, const bool& value);
  void onSendIOCommand(const AddressBook::AddressPair &addressPair, const bool &value);
  void sendReceivedInputUpdate(const DigialInputResponse& response);

private:
  void initInput();
  void initOutput();
  void initIO();
  void initOutputPins();
  quint8 readFromAddress(quint8 address);
  void writeToAddress(quint8 address, quint8 data);


  bool initInputsFinished;
  bool initOutputsFinished;
  // QThread interface
protected:

  QVector <quint8> outputPins;
  void run();
};

#endif // DIGITALIOINTERFACE_H
