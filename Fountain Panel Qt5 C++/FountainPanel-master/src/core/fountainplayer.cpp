#include "fountainplayer.h"
#include "configurationmanager.h"
FountainPlayer::FountainPlayer(QObject *parent) :
  QThread(parent),
  running(false)
{

}

void FountainPlayer::play()
{
  QMutexLocker l(&runningMutex);
  running = true;
  currentFrameIndex = 0;
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
}

void FountainPlayer::run()
{
  QMutexLocker l(&runningMutex);
  while (true) {

      l.relock();
      if (running)
        {
          if (isFilePlaybackRemaining())
            {
              startTimer();

              sendDmxData(currentData[currentFrameIndex++]);

              l.unlock();
              usleep(getSleepMicroSecs(getElapsedMicroSecs()));
            }
          else
            {
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
      qDebug()<<"Warning, Sending Fountain Command Took " << elapsedMicroSecs + " us";
      sleepTime = 0;
    }

  return sleepTime;
}

bool FountainPlayer::isFilePlaybackRemaining()
{
//  bool b = (currentData.size() < currentFrameIndex);
  return (currentData.size() > currentFrameIndex);
}

void FountainPlayer::sendDmxData(const QVector<quint8> dmxFrame)
{
  //send data
}

QVector<QVector<quint8>> FountainPlayer::readDataFromFile(const QUrl &fileAddress)
{
  QFile file(fileAddress.path());

  QVector<QVector<quint8>> dataVector;
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


const quint16 FountainPlayer::DMX_FRAME_COUNT = 512;

