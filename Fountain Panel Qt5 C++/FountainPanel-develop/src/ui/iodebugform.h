#ifndef IODEBUGFORM_H
#define IODEBUGFORM_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>

#include <QMap>

#include "src/core/digitaliointerfaceclient.h"
namespace Ui {
  class IoDebugForm;
}

class IoDebugForm : public QWidget, public DigitalIOInterfaceClient
{
  Q_OBJECT

public:
  explicit IoDebugForm(QWidget *parent = 0);
  ~IoDebugForm();

private:

  void addInputGui();
  void addOutputGui();
  Ui::IoDebugForm *ui;

  QMap<quint8, QCheckBox*> inputMap;
  QMap<quint8, QCheckBox*> outputMap;
  QMap<QCheckBox*, quint8> outputInverseMap;


  static AddressBook::AddressPair indexToOutputAddress(const int& index)
  {
    return AddressBook::AddressPair ((index / 8 + 8), index % 8);
  }
  static AddressBook::AddressPair indexToInputAddress(const int& index)
  {
    return AddressBook::AddressPair ((index / 8), index % 8);
  }

  // DigitalIOInterfaceClient interface
private slots:
  virtual void onReceiveInputUpdate(const DigialInputResponse &response);
  virtual void onReceiveOutputUpdate(const DigialOutputResponse &response);

  void onCheckboxToggled();

signals:
  virtual void sendIOCommand(const AddressBook::OutputRamEntity &address, const bool &value);
  virtual void sendIOCommand(const AddressBook::AddressPair &address, const bool &value);
};

#endif // IODEBUGFORM_H
