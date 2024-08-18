#include "addressbook.h"

AddressBook::AddressPair AddressBook::getInputAddress(const AddressBook::InputRamEntity &ramEntity)
{
  if (!inputInitialized)
    {
      inputInitialized = true;
      inputAddressBookMap = initInputAddressBook();
    }
  return inputAddressBookMap.value(ramEntity);
}

AddressBook::AddressPair AddressBook::getOutputAddress(const AddressBook::OutputRamEntity &ramEntity)
{
  if (!outputInitialized)
    {
      outputInitialized = true;
      outputAddressBookMap = initOutputAddressBook();
    }
  return outputAddressBookMap.value(ramEntity);
}

QMap<AddressBook::InputRamEntity, AddressBook::AddressPair> AddressBook::initInputAddressBook()
{
  QMap<AddressBook::InputRamEntity, AddressPair> addressBookMap;

  addressBookMap.insert(INPUT_COMMAND_BACKWASH_PUMP_1, AddressPair(4,2));
  addressBookMap.insert(INPUT_COMMAND_BACKWASH_PUMP_2, AddressPair(4,3));
  addressBookMap.insert(INPUT_COMMAND_BACKWASH_PUMP_3, AddressPair(4,4));
  addressBookMap.insert(INPUT_COMMAND_BACKWASH_PUMP_4, AddressPair(4,5));

  addressBookMap.insert(INPUT_COMMAND_BACKWASH_ROUTINE, AddressPair(4,7));
  addressBookMap.insert(INPUT_COMMAND_NORMAL_ROUTINE, AddressPair(4,6));

  addressBookMap.insert(INPUT_PRESURE_SWITCH_STATUS_1, AddressPair(3,2));
  addressBookMap.insert(INPUT_PRESURE_SWITCH_STATUS_2, AddressPair(3,3));
  addressBookMap.insert(INPUT_PRESURE_SWITCH_STATUS_3, AddressPair(3,4));
  addressBookMap.insert(INPUT_PRESURE_SWITCH_STATUS_4, AddressPair(3,5));

  addressBookMap.insert(INPUT_WIND_STATUS, AddressPair(3,6));
  addressBookMap.insert(INPUT_DAY_NIGHT_STATUS, AddressPair(3,7));

  addressBookMap.insert(INPUT_DRIVE_READY_STATUS_1, AddressPair(1,1));
  addressBookMap.insert(INPUT_DRIVE_READY_STATUS_2, AddressPair(0,6));
  addressBookMap.insert(INPUT_DRIVE_READY_STATUS_3, AddressPair(0,3));
  addressBookMap.insert(INPUT_DRIVE_READY_STATUS_4, AddressPair(0,0));

  addressBookMap.insert(INPUT_DRIVE_LOCAL_REMOTE_STATUS_1, AddressPair(1,2));
  addressBookMap.insert(INPUT_DRIVE_LOCAL_REMOTE_STATUS_2, AddressPair(0,7));
  addressBookMap.insert(INPUT_DRIVE_LOCAL_REMOTE_STATUS_3, AddressPair(0,4));
  addressBookMap.insert(INPUT_DRIVE_LOCAL_REMOTE_STATUS_4, AddressPair(0,1));

  addressBookMap.insert(INPUT_DRIVE_FAULT_STATUS_1, AddressPair(1,3));
  addressBookMap.insert(INPUT_DRIVE_FAULT_STATUS_2, AddressPair(1,0));
  addressBookMap.insert(INPUT_DRIVE_FAULT_STATUS_3, AddressPair(0,5));
  addressBookMap.insert(INPUT_DRIVE_FAULT_STATUS_4, AddressPair(0,2));

  addressBookMap.insert(INPUT_COMMAND_FINALIZE, AddressPair(4,0));
  addressBookMap.insert(INPUT_POWER_LOSS_STATUS, AddressPair(3,1));

  addressBookMap.insert(INPUT_2, AddressPair(0,1));
  addressBookMap.insert(INPUT_3, AddressPair(0,2));
  addressBookMap.insert(INPUT_4, AddressPair(0,3));
  addressBookMap.insert(INPUT_5, AddressPair(0,4));
  addressBookMap.insert(INPUT_6, AddressPair(0,5));
  addressBookMap.insert(INPUT_7, AddressPair(0,6));
  addressBookMap.insert(INPUT_8, AddressPair(0,7));
  addressBookMap.insert(INPUT_9, AddressPair(1,0));
  addressBookMap.insert(INPUT_10, AddressPair(1,1));
  addressBookMap.insert(INPUT_11, AddressPair(1,2));
  addressBookMap.insert(INPUT_12, AddressPair(1,3));
  addressBookMap.insert(INPUT_13, AddressPair(1,4));
  addressBookMap.insert(INPUT_14, AddressPair(1,5));
  addressBookMap.insert(INPUT_15, AddressPair(1,6));
  return addressBookMap;

}

QMap<AddressBook::OutputRamEntity, AddressBook::AddressPair> AddressBook::initOutputAddressBook()
{
  QMap<AddressBook::OutputRamEntity, AddressPair> addressBookMap;
  addressBookMap.insert(OUTPUT_DRIVE_RUN_1, AddressPair(8,0));
  addressBookMap.insert(OUTPUT_DRIVE_RUN_2, AddressPair(8,1));
  addressBookMap.insert(OUTPUT_DRIVE_RUN_3, AddressPair(8,2));
  addressBookMap.insert(OUTPUT_DRIVE_RUN_4, AddressPair(8,3));

  addressBookMap.insert(OUTPUT_PUMP_INDICATOR_1, AddressPair(11,6));
  addressBookMap.insert(OUTPUT_PUMP_INDICATOR_2, AddressPair(11,7));
  addressBookMap.insert(OUTPUT_PUMP_INDICATOR_3, AddressPair(12,0));
  addressBookMap.insert(OUTPUT_PUMP_INDICATOR_4, AddressPair(12,1));

  addressBookMap.insert(OUTPUT_PRESURE_SWITCH_INDICATOR_1, AddressPair(12,5));
  addressBookMap.insert(OUTPUT_PRESURE_SWITCH_INDICATOR_2, AddressPair(12,4));
  addressBookMap.insert(OUTPUT_PRESURE_SWITCH_INDICATOR_3, AddressPair(11,3));
  addressBookMap.insert(OUTPUT_PRESURE_SWITCH_INDICATOR_4, AddressPair(12,2));



  addressBookMap.insert(OUTPUT_CRITICAL_BACKWASH_INDICATOR, AddressPair(11,4));
  addressBookMap.insert(OUTPUT_DRIVE_ERROR_INDICATOR, AddressPair(11,5));
  addressBookMap.insert(OUTPUT_FINALIZE_FINISHED, AddressPair(12,7));




  addressBookMap.insert(OUTPUT_1, AddressPair(8,0));
  addressBookMap.insert(OUTPUT_2, AddressPair(8,1));
  addressBookMap.insert(OUTPUT_3, AddressPair(8,2));
  addressBookMap.insert(OUTPUT_4, AddressPair(8,3));
  addressBookMap.insert(OUTPUT_5, AddressPair(8,4));
  addressBookMap.insert(OUTPUT_6, AddressPair(8,5));
  addressBookMap.insert(OUTPUT_7, AddressPair(8,6));
  addressBookMap.insert(OUTPUT_8, AddressPair(8,7));
  addressBookMap.insert(OUTPUT_9, AddressPair(9,0));
  addressBookMap.insert(OUTPUT_10, AddressPair(9,1));
  addressBookMap.insert(OUTPUT_11, AddressPair(9,2));
  addressBookMap.insert(OUTPUT_12, AddressPair(9,3));
  addressBookMap.insert(OUTPUT_13, AddressPair(9,4));
  addressBookMap.insert(OUTPUT_14, AddressPair(9,5));
  addressBookMap.insert(OUTPUT_15, AddressPair(9,6));
  addressBookMap.insert(OUTPUT_16, AddressPair(9,7));
  addressBookMap.insert(OUTPUT_17, AddressPair(10,0));
  addressBookMap.insert(OUTPUT_18, AddressPair(10,1));
  addressBookMap.insert(OUTPUT_19, AddressPair(10,2));
  addressBookMap.insert(OUTPUT_20, AddressPair(10,3));
  addressBookMap.insert(OUTPUT_21, AddressPair(10,4));
  addressBookMap.insert(OUTPUT_22, AddressPair(12,4));
  addressBookMap.insert(OUTPUT_23, AddressPair(12,5));
  addressBookMap.insert(OUTPUT_24, AddressPair(12,6));

  return addressBookMap;
}


QMap < AddressBook::InputRamEntity, AddressBook::AddressPair> AddressBook::inputAddressBookMap;
QMap < AddressBook::OutputRamEntity, AddressBook::AddressPair> AddressBook::outputAddressBookMap;

bool AddressBook::inputInitialized = false;
bool AddressBook::outputInitialized = false;
