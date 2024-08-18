#include "fountainplayer.h"
#include "configurationmanager.h"

#include "dmxinterface.h"
//#include <fcntl.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sched.h>
////#include <sys/ioctl.h>

//#include <asm/termios.h>
//#include <asm/ioctls.h>
//#include <asm/ioctl.h>
//#include <stropts.h>

//#include "src/hardware/RPI.h"



FountainPlayer::FountainPlayer(QObject *parent) :
  QThread(parent),
  running(false)/*,*/
//  initilized(false)
{

}

//UnknownMediaStatus,
//NoMedia,
//LoadingMedia,
//LoadedMedia,
//StalledMedia,
//BufferingMedia,
//BufferedMedia,
//EndOfMedia,
//InvalidMedia
void FountainPlayer::play()
{
  QMutexLocker l(&runningMutex);
  running = true;
  totalTime.start();
  qDebug()<<"Time Started";
//  if (!isFilePlaybackRemaining())
//    {
//      currentFrameIndex = 0;
//    }
}

void FountainPlayer::pause()
{
  QMutexLocker l(&runningMutex);
  running = false;
}

void FountainPlayer::stop()
{
  QMutexLocker l(&runningMutex);
  running = false;
  currentFrameIndex = 0;

}

void FountainPlayer::setMedia(const QUrl &fileAddress)
{
  QMutexLocker l(&runningMutex);
  running = false;
  currentData = readDataFromFile(fileAddress);
  currentFrameIndex = 0;
  emit fountainStatusChanged(QMediaPlayer::BufferingMedia);
  emit durationChanged(currentData.size() * ConfigurationManager::getInstance().getUsfountainInterval() / 1000 );
}

void FountainPlayer::run()
{
  QMutexLocker l(&runningMutex);
  l.unlock();
  while (true) {


//      if (!initilized)
//        {
//          init();

//          msleep(ConfigurationManager::getInstance().getSecsRetryInterval());
//          continue;
//        }

      l.relock();
      if (running)
        {
          if (isFilePlaybackRemaining())
            {
              startTimer();

              QElapsedTimer e;
              e.start();
              DmxInterface::sendDmxData(currentData[currentFrameIndex++]);
//              qDebug()<<"E"<<e.elapsed();
//              currentFrameIndex++;

              l.unlock();
              emit positionChanged(currentFrameIndex * ConfigurationManager::getInstance().getUsfountainInterval() / 1000 );
              usleep(getSleepMicroSecs(getElapsedMicroSecs()));
            }
          else
            {
//              qDebug()<< "time " << totalTime.elapsed();
              emit fountainStatusChanged(QMediaPlayer::EndOfMedia);
              running = false;
              l.unlock();
              usleep(ConfigurationManager::getInstance().getUsfountainInterval());
              // handle finished
            }
        }

      else
        {
          l.unlock();
          usleep(ConfigurationManager::getInstance().getUsfountainInterval());
        }

    }

}

void FountainPlayer::startTimer()
{
  elapsedTimer.restart();
}

qint64 FountainPlayer::getElapsedMicroSecs()
{
  return elapsedTimer.nsecsElapsed() / 1000;
}

qint64 FountainPlayer::getSleepMicroSecs(const qint64& elapsedMicroSecs)
{
  qint64 sleepTime = ConfigurationManager::getInstance().getUsfountainInterval() - elapsedMicroSecs;

  if (sleepTime < 0)
    {
      qDebug()<<"Warning, Sending Fountain Command Took " << QString::number((qlonglong)elapsedMicroSecs) + " us";
      sleepTime = 0;
    }
  return sleepTime;
}

bool FountainPlayer::isFilePlaybackRemaining()
{
  //  bool b = (currentData.size() < currentFrameIndex);
  return (currentData.size() > currentFrameIndex);
}

//void FountainPlayer::sendDmxData(const QVector<quint8> dmxFrame)
//{
//  int tlen = 0 ;

//  char zero = 0;

//  write ( fileDescriptor, &zero, 1 ) ;
//  //send data
//  for (int index = 0; index < dmxFrame.size(); ++index)
//    {
//      write ( fileDescriptor, &dmxFrame[index], 1 ) ;
//      while ( tlen != 0 )
//        {
//          ioctl( fileDescriptor, TIOCOUTQ, &tlen ) ;
//        }
//      tlen = 1 ;
//    }


//  // Define pin 8 as output
//  OUT_GPIO(14);
//  //for ( flag = 0 ; flag < 200 ;flag ++ )
//  //	flag++ ;
//  usleep ( 400 ) ;
//  //GPIO_CLR |= 1 << 4;
//  GPIO_CLR |= 1 << 14;
//  usleep ( 500 ) ;
//  //for ( flag = 0 ; flag < 1000000 ;flag ++ )
//  //	flag++ ;
//  //GPIO_SET |= 1 << 4;
//  GPIO_SET |= 1 << 14;

//  SET_GPIO_TX0 ;


//}

QVector<QVector<quint8> > FountainPlayer::readDataFromFile(const QUrl &fileAddress)
{
  QFile file(fileAddress.path());

  QVector<QVector<quint8> > dataVector;
  QVector<quint8> frameVector;

  if (file.open(QIODevice::ReadOnly))
    {
      QDataStream dataStream(&file);

      while (!dataStream.atEnd()) {
          quint8 data;
          dataStream >> data;
          frameVector.push_back(data);

          if (frameVector.size() == DMX_FRAME_COUNT)
            {
              dataVector.push_back(frameVector);
              frameVector.clear();
            }
        }

    }
  else
    {
      //handle file error
    }
  return dataVector;
}

//void FountainPlayer::init()
//{
//  struct termios2  config ;

//  //  int fd, fd2;



//  if(RPI::mapPeripheral(&gpio) == -1)
//    {
//      printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
//      //          return 0;
//      return;
//    }

//  //OUT_GPIO(4);

//  fileDescriptor = open ( "/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_SYNC ) ; // | O_NDELAY ) ;
//  if ( fileDescriptor == -1)
//    {
//      printf ( "failed to open port\n" ) ;
//      //          return 0;
//      return;
//    } ;
//  //  fd2 = open ( "/home/pi/Desktop/RPI_DMX_2/data.bin", O_RDWR | O_NOCTTY | O_SYNC ) ; // | O_NDELAY ) ;
//  //  if ( fd2 == -1)
//  //  {
//  //          printf ( "failed to open file\n" ) ;
//  ////          return 0;

//  //  } ;
//  //close ( fd2 ) ;


//  ioctl ( fileDescriptor, TCGETS2, &config ) ;
//  config.c_cflag &= ~CBAUD ;
//  config.c_cflag |= CS8 ;
//  config.c_cflag |= BOTHER ;
//  config.c_cflag |= CSTOPB ;
//  config.c_cflag &= ~PARENB ;
//  config.c_ispeed = 250000 ;
//  config.c_ospeed = 250000 ;
//  config.c_oflag &= ~ONLCR ;
//  config.c_oflag &= ~OCRNL ;
//  ioctl ( fileDescriptor, TCSETS2, &config ) ;

//  //OUT_GPIO(4);
//  SET_TX_FIFO ( 0 ) ;


//  initilized = true;


//}

//void FountainPlayer::finalize()
//{
//  close ( fileDescriptor ) ;
//  //  close ( fd2 ) ;
//}


const quint16 FountainPlayer::DMX_FRAME_COUNT = 512;

