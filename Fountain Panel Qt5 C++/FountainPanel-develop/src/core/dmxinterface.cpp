#include "dmxinterface.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
//#include <sys/ioctl.h>

#include <asm/termios.h>
#include <asm/ioctls.h>
#include <asm/ioctl.h>
#include <stropts.h>

#include "src/hardware/RPI.h"

#include <QDebug>
#include "configurationmanager.h"

DmxInterface::DmxInterface(QObject *parent) : QObject(parent)
{

}

void DmxInterface::sendDmxData(const QVector<quint8> &dmxFrame)
{
#if RASPBERRY_PI
  QMutexLocker l(ConfigurationManager::getInstance().getMutexData());

  if (!initilized)
    {
      init();
    }

  int tlen = 0 ;

  char zero = 0;

  write ( fileDescriptor, &zero, 1 ) ;
  //send data
  for (int index = 0; index < dmxFrame.size(); ++index)
    {
      write ( fileDescriptor, &dmxFrame[index], 1 ) ;
      while ( tlen != 0 )
        {
          ioctl( fileDescriptor, TIOCOUTQ, &tlen ) ;
        }
      tlen = 1 ;
    }


  // Define pin 8 as output
  OUT_GPIO(14);
  //for ( flag = 0 ; flag < 200 ;flag ++ )
  //	flag++ ;
  usleep ( 400 ) ;
  //GPIO_CLR |= 1 << 4;
  GPIO_CLR = 1 << 14;
  usleep ( 500 ) ;
  //for ( flag = 0 ; flag < 1000000 ;flag ++ )
  //	flag++ ;
  //GPIO_SET |= 1 << 4;
  GPIO_SET = 1 << 14;

  SET_GPIO_TX0 ;
#endif
}

void DmxInterface::init()
{
  struct termios2  config ;

  //  int fd, fd2;



//  if(mapPeripheral(&gpio) == -1)
//    {
//      printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
//      //          return 0;
//      return;
//    }

  //OUT_GPIO(4);

  fileDescriptor = open ( "/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_SYNC ) ; // | O_NDELAY ) ;
  if ( fileDescriptor == -1)
    {
      printf ( "failed to open port\n" ) ;
      //          return 0;
      return;
    } ;
  //  fd2 = open ( "/home/pi/Desktop/RPI_DMX_2/data.bin", O_RDWR | O_NOCTTY | O_SYNC ) ; // | O_NDELAY ) ;
  //  if ( fd2 == -1)
  //  {
  //          printf ( "failed to open file\n" ) ;
  ////          return 0;

  //  } ;
  //close ( fd2 ) ;


  ioctl ( fileDescriptor, TCGETS2, &config ) ;
  config.c_cflag &= ~CBAUD ;
  config.c_cflag |= CS8 ;
  config.c_cflag |= BOTHER ;
  config.c_cflag |= CSTOPB ;
  config.c_cflag &= ~PARENB ;
  config.c_ispeed = 250000 ;
  config.c_ospeed = 250000 ;
  config.c_oflag &= ~ONLCR ;
  config.c_oflag &= ~OCRNL ;
  ioctl ( fileDescriptor, TCSETS2, &config ) ;

  //OUT_GPIO(4);
  SET_TX_FIFO ( 0 ) ;


  initilized = true;
}

void DmxInterface::finalize()
{
  close ( fileDescriptor ) ;
  //  close ( fd2 ) ;
}

const quint16 DmxInterface::DMX_FRAME_COUNT = 512;
int DmxInterface::fileDescriptor;
bool DmxInterface::initilized = false;
