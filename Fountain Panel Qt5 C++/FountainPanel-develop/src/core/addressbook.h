#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H


#include <QObject>
#include <QMap>

class AddressBook
{
public:

  struct AddressPair
  {
    AddressPair(const quint8& byteNumber, const quint8& bitNumber) :
      byteNumber(byteNumber),
      bitNumber(bitNumber)
    {

    }

    AddressPair()
    {

    }

    void setAddress(const quint8& byteNumber, const quint8& bitNumber)
    {
      this->byteNumber = byteNumber;
      this->bitNumber = bitNumber;
    }

    quint8 byteNumber;
    quint8 bitNumber;
  };

  enum InputRamEntity{
    INPUT_DAY_NIGHT_STATUS,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7,
    INPUT_8,
    INPUT_9,
    INPUT_10,
    INPUT_11,
    INPUT_12,
    INPUT_13,
    INPUT_14,
    INPUT_15,
    INPUT_WIND_STATUS,
    INPUT_POWER_LOSS_STATUS,
    INPUT_DRIVE_FAULT_STATUS_4,
    INPUT_DRIVE_FAULT_STATUS_3,
    INPUT_DRIVE_FAULT_STATUS_2,
    INPUT_DRIVE_FAULT_STATUS_1,
    INPUT_DRIVE_LOCAL_REMOTE_STATUS_4,
    INPUT_DRIVE_LOCAL_REMOTE_STATUS_3,
    INPUT_DRIVE_LOCAL_REMOTE_STATUS_2,
    INPUT_DRIVE_LOCAL_REMOTE_STATUS_1,
    INPUT_DRIVE_READY_STATUS_4,
    INPUT_DRIVE_READY_STATUS_3,
    INPUT_DRIVE_READY_STATUS_2,
    INPUT_DRIVE_READY_STATUS_1,
    INPUT_PRESURE_SWITCH_STATUS_4,
    INPUT_PRESURE_SWITCH_STATUS_3,
    INPUT_PRESURE_SWITCH_STATUS_2,
    INPUT_PRESURE_SWITCH_STATUS_1,
    INPUT_COMMAND_NORMAL_ROUTINE,
    INPUT_COMMAND_FINALIZE,
    INPUT_COMMAND_BACKWASH_PUMP_4,
    INPUT_COMMAND_BACKWASH_PUMP_3,
    INPUT_COMMAND_BACKWASH_PUMP_2,
    INPUT_COMMAND_BACKWASH_PUMP_1,
    INPUT_COMMAND_BACKWASH_ROUTINE,
  };
  enum OutputRamEntity{
    OUTPUT_1,
    OUTPUT_2,
    OUTPUT_3,
    OUTPUT_4,
    OUTPUT_5,
    OUTPUT_6,
    OUTPUT_7,
    OUTPUT_8,
    OUTPUT_9,
    OUTPUT_10,
    OUTPUT_11,
    OUTPUT_12,
    OUTPUT_13,
    OUTPUT_14,
    OUTPUT_15,
    OUTPUT_16,
    OUTPUT_17,
    OUTPUT_18,
    OUTPUT_19,
    OUTPUT_20,
    OUTPUT_21,
    OUTPUT_FINALIZE_FINISHED,
    OUTPUT_DRIVE_ERROR_INDICATOR,
    OUTPUT_CRITICAL_BACKWASH_INDICATOR,
    OUTPUT_PRESURE_SWITCH_INDICATOR_4,
    OUTPUT_PRESURE_SWITCH_INDICATOR_3,
    OUTPUT_PRESURE_SWITCH_INDICATOR_2,
    OUTPUT_PRESURE_SWITCH_INDICATOR_1,
    OUTPUT_PUMP_INDICATOR_4,
    OUTPUT_PUMP_INDICATOR_3,
    OUTPUT_PUMP_INDICATOR_2,
    OUTPUT_PUMP_INDICATOR_1,
    OUTPUT_DRIVE_RUN_4,
    OUTPUT_DRIVE_RUN_3,
    OUTPUT_DRIVE_RUN_2,
    OUTPUT_DRIVE_RUN_1,
    OUTPUT_22,
    OUTPUT_23,
    OUTPUT_24,
    OUTPUT_25
  };

  static AddressBook::AddressPair getInputAddress(const AddressBook::InputRamEntity& ramEntity);
  static AddressBook::AddressPair getOutputAddress(const AddressBook::OutputRamEntity& ramEntity);

private:
  AddressBook();

  static QMap < AddressBook::InputRamEntity, AddressPair> initInputAddressBook();
  static QMap < AddressBook::OutputRamEntity, AddressPair> initOutputAddressBook();


  static QMap < AddressBook::InputRamEntity, AddressBook::AddressPair> inputAddressBookMap;
  static bool inputInitialized;
  static QMap < AddressBook::OutputRamEntity, AddressBook::AddressPair> outputAddressBookMap;
  static bool outputInitialized;
};






#endif // ADDRESSBOOK_H
