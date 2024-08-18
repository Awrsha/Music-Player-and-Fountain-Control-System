#ifndef DIGIALINPUTRESPONSE_H
#define DIGIALINPUTRESPONSE_H

#include <QObject>
#include <QVector>
#include "addressbook.h"
#include <bitset>
class DigialInputResponse
{
public:
  DigialInputResponse();

  bool getResponse(const AddressBook::InputRamEntity& inputRamEntity) const;
  bool getResponse(const AddressBook::AddressPair& inputRamEntity) const;

  void setResponseBytes(const QVector<quint8> &value);

private:
  QVector<quint8> responseBytes;

};

class DigialOutputResponse
{
public:
  DigialOutputResponse();

  bool getResponse(const AddressBook::OutputRamEntity& outputRamEntity) const;
  bool getResponse(const AddressBook::AddressPair& addressPair) const;
  void setResponseBytes(const QVector<quint8> &value);

private:
  QVector<quint8> responseBytes;

};

#endif // DIGIALINPUTRESPONSE_H
