#include "digialinputresponse.h"
#include <QDebug>

DigialInputResponse::DigialInputResponse()
{

}

bool DigialInputResponse::getResponse(const AddressBook::InputRamEntity &inputRamEntity) const
{
  return getResponse(AddressBook::getInputAddress(inputRamEntity));
}

bool DigialInputResponse::getResponse(const AddressBook::AddressPair &addressPair) const
{
  if (responseBytes.size() != 5)
    {
      return 0;
    }
  return std::bitset<8> (responseBytes.at(addressPair.byteNumber))[addressPair.bitNumber];
}

void DigialInputResponse::setResponseBytes(const QVector<quint8> &value)
{
  responseBytes = value;
}

DigialOutputResponse::DigialOutputResponse()
{

}

bool DigialOutputResponse::getResponse(const AddressBook::OutputRamEntity &outputRamEntity) const
{
  return getResponse(AddressBook::getOutputAddress(outputRamEntity));
}

bool DigialOutputResponse::getResponse(const AddressBook::AddressPair &addressPair) const
{
  if (responseBytes.size() != 5)
    {
      return 0;
    }
  return std::bitset<8> (responseBytes.at(addressPair.byteNumber - 8))[addressPair.bitNumber];
}

void DigialOutputResponse::setResponseBytes(const QVector<quint8> &value)
{
  responseBytes = value;
}
