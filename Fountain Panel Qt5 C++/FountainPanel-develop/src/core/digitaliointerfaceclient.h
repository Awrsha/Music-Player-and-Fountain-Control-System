#ifndef DIGITALIOINTERFACECLIENT_H
#define DIGITALIOINTERFACECLIENT_H

#include "digialinputresponse.h"

class DigitalIOInterfaceClient
{
public:
  DigitalIOInterfaceClient();

  virtual void onReceiveInputUpdate(const DigialInputResponse& response);
  virtual void onReceiveOutputUpdate(const DigialOutputResponse& response);
  virtual void sendIOCommand(const AddressBook::OutputRamEntity &address, const bool& value);
  virtual void sendIOCommand(const AddressBook::AddressPair &address, const bool& value);

};

#endif // DIGITALIOINTERFACECLIENT_H
