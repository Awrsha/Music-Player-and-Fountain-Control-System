#include "digitaliointerface.h"
#include <QDebug>
#include "src/hardware/RPI.h"
#include "src/flags/projectflags.h"
#include <QApplication>

DigitalIOInterface::DigitalIOInterface(QObject *parent) :
  QThread(parent),
  initInputsFinished(false),
  initOutputsFinished(false)
{
}

void DigitalIOInterface::onSendIOCommand(const AddressBook::OutputRamEntity &outputEntity, const bool &value)
{
  onSendIOCommand(AddressBook::getOutputAddress(outputEntity), value);
}

void DigitalIOInterface::onSendIOCommand(const AddressBook::AddressPair &addressPair, const bool &value)
{
  if (outputPins.size() != 5)
    {
      return;
    }

  std::bitset<8> sendCommand(outputPins.at(addressPair.byteNumber - 8));
  if (sendCommand[addressPair.bitNumber] != value)
    {
      qDebug()<< "Address is " << addressPair.byteNumber << " " << addressPair.bitNumber <<
                 " Value is: " << value;
      sendCommand.set(addressPair.bitNumber, value);
      outputPins[addressPair.byteNumber - 8] = sendCommand.to_ulong();
      writeToAddress(addressPair.byteNumber, outputPins[addressPair.byteNumber - 8]);

      DigialOutputResponse outputUpdate;
      outputUpdate.setResponseBytes(outputPins);
      emit receivedOutputUpdate(outputUpdate);
    }
}

void DigitalIOInterface::sendReceivedInputUpdate(const DigialInputResponse &response)
{
  emit receivedInputUpdate(response);
}

void DigitalIOInterface::initInput()
{
#if RASPBERRY_PI
  INP_GPIO( D0 ) ;
  INP_GPIO( D1 ) ;
  INP_GPIO( D2 ) ;
  INP_GPIO( D3 ) ;
  INP_GPIO( D4 ) ;
  INP_GPIO( D5 ) ;
  INP_GPIO( D6 ) ;
  INP_GPIO( D7 ) ;
  initIO();
#endif
}

void DigitalIOInterface::initOutput()
{

#if RASPBERRY_PI
  OUT_GPIO( D0 ) ;
  OUT_GPIO( D1 ) ;
  OUT_GPIO( D2 ) ;
  OUT_GPIO( D3 ) ;
  OUT_GPIO( D4 ) ;
  OUT_GPIO( D5 ) ;
  OUT_GPIO( D6 ) ;
  OUT_GPIO( D7 ) ;
  initIO();
#endif
}

void DigitalIOInterface::initIO()
{
#if RASPBERRY_PI
  GPIO_SET = 1 << A0 ;
  GPIO_SET = 1 << A1 ;
  GPIO_SET = 1 << A2 ;
  GPIO_SET = 1 << A3 ;
  GPIO_SET = 1 << CLK_I ;
  GPIO_SET = 1 << CLK_O ;
  OUT_GPIO ( A0 ) ;
  OUT_GPIO ( A1 ) ;
  OUT_GPIO ( A2 ) ;
  OUT_GPIO ( A3 ) ;
  OUT_GPIO ( CLK_I ) ;
  OUT_GPIO ( CLK_O ) ;
  GPIO_SET = 1 << A0 ;
  GPIO_SET = 1 << A1 ;
  GPIO_SET = 1 << A2 ;
  GPIO_SET = 1 << A3 ;
  GPIO_SET = 1 << CLK_I ;
  GPIO_SET = 1 << CLK_O ;
#endif
}

void DigitalIOInterface::initOutputPins()
{

  if(mapPeripheral(&gpio) == -1)
    {
      qDebug()<<("Failed to map the physical GPIO registers into the virtual memory space.\n");
      //          return 0;
      return;
    }
  EN_OUTPUTS;
  outputPins.resize(5);

  outputPins[0] = 0;
  outputPins[1] = 0;
  outputPins[2] = 0;
  outputPins[3] = 0;
  outputPins[4] = 0;

  writeToAddress(8, 0);
  writeToAddress(9, 0);
  writeToAddress(10, 0);
  writeToAddress(11, 0);
  writeToAddress(12, 0);

  DigialOutputResponse responseOutput;
  responseOutput.setResponseBytes(outputPins);
  emit receivedOutputUpdate(responseOutput);
}

quint8 DigitalIOInterface::readFromAddress(quint8 address)
{
#if RASPBERRY_PI
  QMutexLocker l(ConfigurationManager::getInstance().getMutexData());

  quint8 response = 0 ;

  //address limiting 0 1 2 3
  if ( address > 4 )
    return 0 ;
//  if (!initInputsFinished)
//    {
      initInput();
//      initInputsFinished = true;
//    }

  //CLK_I (latching data)
  usleep ( IO_delay ) ;
  GPIO_CLR = 1 << CLK_I ;
  usleep ( IO_delay ) ;
  GPIO_SET = 1 << CLK_I ;
  usleep ( IO_delay ) ;

  //Effective Address Calculation
  if ( (address & 1) != 0 )
    GPIO_SET = 1 << A0 ;
  else
    GPIO_CLR = 1 << A0 ;

  if ( (address & 2) != 0 )
    GPIO_SET = 1 << A1 ;
  else
    GPIO_CLR = 1 << A1 ;

  if ( (address & 4) != 0 )
    GPIO_SET = 1 << A2 ;
  else
    GPIO_CLR = 1 << A2 ;

  if ( (address & 8) != 0 )
    GPIO_SET = 1 << A3 ;
  else
    GPIO_CLR = 1 << A3 ;

  //Select Input
  usleep ( IO_delay ) ;
  GPIO_CLR = 1 << CLK_O ;
  usleep ( IO_delay ) ;

  //Reading Input
  response = ( (static_cast<bool>(GPIO_READ ( D0 ))) << 0 ) |
      ( (static_cast<bool>(GPIO_READ ( D1 ))) << 1 ) |
      ( (static_cast<bool>(GPIO_READ ( D2 ))) << 2 ) |
      ( (static_cast<bool>(GPIO_READ ( D3 ))) << 3 ) |
      ( (static_cast<bool>(GPIO_READ ( D4 ))) << 4 ) |
      ( (static_cast<bool>(GPIO_READ ( D5 ))) << 5 ) |
      ( (static_cast<bool>(GPIO_READ ( D6 ))) << 6 ) |
      ( (static_cast<bool>(GPIO_READ ( D7 ))) << 7 ) ;

  //Deselect Input
  usleep ( IO_delay ) ;
  GPIO_SET = 1 << CLK_O ;
  usleep ( 10 ) ;

  return response ;
#endif
}

void DigitalIOInterface::writeToAddress(quint8 address, quint8 data)
{
#if RASPBERRY_PI
  QMutexLocker l(ConfigurationManager::getInstance().getMutexData());
  //address limiting 9 10 11 12
  if ( address < 8 )
    return ;
  if ( address > 12 )
    return ;

//  if (!initOutputsFinished)
//    {
      initOutput();
//      initOutputsFinished = true;
//    }

  //Effective Address Calculation
  if ( ( address & 1 ) != 0 )
    GPIO_SET = 1 << A0 ;
  else
    GPIO_CLR = 1 << A0 ;

  if ( ( address & 2 ) != 0 )
    GPIO_SET = 1 << A1 ;
  else
    GPIO_CLR = 1 << A1 ;

  if ( ( address & 4 ) != 0 )
    GPIO_SET = 1 << A2 ;
  else
    GPIO_CLR = 1 << A2 ;

  if ( ( address & 8 ) != 0 )
    GPIO_SET = 1 << A3 ;
  else
    GPIO_CLR = 1 << A3 ;

  //Filling Data Bus
  if ( ( data & 1 ) != 0 )
    GPIO_SET = 1 << D0 ;
  else
    GPIO_CLR = 1 << D0 ;

  if ( ( data & 2 ) != 0 )
    GPIO_SET = 1 << D1 ;
  else
    GPIO_CLR = 1 << D1 ;

  if ( ( data & 4 ) != 0 )
    GPIO_SET = 1 << D2 ;
  else
    GPIO_CLR = 1 << D2 ;

  if ( ( data & 8 ) != 0 )
    GPIO_SET = 1 << D3 ;
  else
    GPIO_CLR = 1 << D3 ;

  if ( ( data & 16 ) != 0 )
    GPIO_SET = 1 << D4 ;
  else
    GPIO_CLR = 1 << D4 ;

  if ( ( data & 32 ) != 0 )
    GPIO_SET = 1 << D5 ;
  else
    GPIO_CLR = 1 << D5 ;

  if ( ( data & 64 ) != 0 )
    GPIO_SET = 1 << D6 ;
  else
    GPIO_CLR = 1 << D6 ;

  if ( ( data & 128 ) != 0 )
    GPIO_SET = 1 << D7 ;
  else
    GPIO_CLR = 1 << D7 ;

  //Clocking outputs
  usleep ( IO_delay ) ;
  GPIO_CLR = 1 << CLK_O ;
  usleep ( IO_delay ) ;
  GPIO_SET = 1 << CLK_O ;

  return ;
#endif

}


void DigitalIOInterface::run()
{
  initOutputPins();

  while (true)
    {
#if RASPBERRY_PI
      QVector<quint8> responseByte;
      responseByte.resize(5);

      responseByte[0] = ~readFromAddress(0);
      responseByte[1] = ~readFromAddress(1);
      responseByte[2] = ~readFromAddress(2);
      responseByte[3] = ~readFromAddress(3);
      responseByte[4] = ~readFromAddress(4);

      DigialInputResponse inputResponse;

      inputResponse.setResponseBytes(responseByte);

//      QMetaObject::invokeMethod(this, "sendReceivedInputUpdate", Qt::QueuedConnection, Q_ARG(
//                                  DigialInputResponse, inputResponse));
      emit receivedInputUpdate(inputResponse);
#endif

      msleep(ConfigurationManager::getInstance().getIoReadMS());

    }

}
