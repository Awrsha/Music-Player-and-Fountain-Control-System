#include "myplayer.h"
#include "playmode.h"
#include <QMediaContent>
#include <QTime>
#include <QMediaMetaData>
#include <QThread>
#include "data.h"

MyPlayer::MyPlayer() :
  currentDayNightMode(MusicFountainInfo::FIRST),
  currentNoWindWindMode(MusicFountainInfo::FIRST)
{
  playerState = NOMUSIC;
  player.setNotifyInterval(100);
  connect(&player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMusicStatusChange(QMediaPlayer::MediaStatus)));
  connect(&player, SIGNAL(durationChanged(qint64)), this, SIGNAL(musicDurationChanged(qint64)));
  connect(&player, SIGNAL(durationChanged(qint64)), this, SLOT(musicChanged()));
  connect(&player, SIGNAL(positionChanged(qint64)), this, SIGNAL(musicPositionChanged(qint64)));
  connect(this, SIGNAL(playerStateChanged(int)), this, SLOT(stateChanged(int)));

  fountainPlayer = new FountainPlayer();
  fountainPlayer->start(QThread::TimeCriticalPriority);

  connect(fountainPlayer, SIGNAL(fountainStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onFountainStatusChange(QMediaPlayer::MediaStatus)));
  connect(fountainPlayer, SIGNAL(durationChanged(qint64)), this, SIGNAL(fountainDurationChanged(qint64)));
  connect(fountainPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(musicChanged()));
  connect(fountainPlayer, SIGNAL(positionChanged(qint64)), this, SIGNAL(fountainPositionChanged(qint64)));

}

void MyPlayer::setPlay()
{
  player.play();
  fountainPlayer->play();
  emit playerStateChanged(PLAYING);
}

void MyPlayer::setPause()
{
  player.pause();
  fountainPlayer->pause();
  emit playerStateChanged(PAUSE);
}

void MyPlayer::musicChanged()
{
  emit musicChanged(currentList, currentindex);
}

int MyPlayer::getPlayerState()
{
  return this->playerState;
}

void MyPlayer::stateChanged(int state)
{
  this->playerState = state;
}

bool MyPlayer::jumpToNextValid(bool loopIsAllowed)
{
  if (this->mediaCount() == 0)
    return false;

  int index = getCurrentIndex();

  ++index;

  if ( index >= mediaCount())
    {
      if (loopIsAllowed)
        {
          index = 0;
        }
      else
        {
          return false;
        }
    }
  while ( !list[currentList].at(index).doesMeetState(currentDayNightMode, currentNoWindWindMode))
    {
      ++index;

      if ( index == (getCurrentIndex() + 1))
        {
          return false;
        }

      if ( index >= mediaCount() )
        {
          if (loopIsAllowed)
            {
              index = 0;
            }
          else
            {
              return false;
            }
        }


    }
  currentindex = index;
  return true;
}

bool MyPlayer::jumpToPreValid(bool loopIsAllowed)
{
  if (this->mediaCount() == 0)
    return false;

  int index = getCurrentIndex();

  --index;

  if ( index < 0)
    {
      if (loopIsAllowed)
        {
          index = mediaCount() - 1;
        }
      else
        {
          return false;
        }
    }

  while ( !list[currentList].at(index).doesMeetState(currentDayNightMode, currentNoWindWindMode))
    {
      --index;

      if ( index == (getCurrentIndex() - 1))
        {
          return false;
        }

      if ( index < 0)
        {
          if (loopIsAllowed)
            {
              index = mediaCount() - 1;
            }
          else
            {
              return false;
            }
        }
    }
  currentindex = index;
  return true;
}

bool MyPlayer::jumpToRandomValid()
{
  if (this->mediaCount() == 0)
    return false;

  int index = getCurrentIndex();

  ++index;

  if ( index >= mediaCount())
    {
      index = 0;
    }

  while ( !list[currentList].at(index).doesMeetState(currentDayNightMode, currentNoWindWindMode))
    {
      ++index;

      if ( index == (getCurrentIndex() + 1))
        {
          return false;
        }

      if ( index >= mediaCount())
        {
          index = 0;
        }


    }
  currentindex = index;
  return true;
}

void MyPlayer::onReceiveInputUpdate(const DigialInputResponse &response)
{
  if (response.getResponse(AddressBook::INPUT_DAY_NIGHT_STATUS))
    {
      currentDayNightMode = MusicFountainInfo::SECOND;
    }
  else
    {
      currentDayNightMode = MusicFountainInfo::FIRST;

    }

  if (response.getResponse(AddressBook::INPUT_WIND_STATUS))
    {
      currentNoWindWindMode = MusicFountainInfo::SECOND;
    }
  else
    {
      currentNoWindWindMode = MusicFountainInfo::FIRST;

    }
}

void MyPlayer::setVoice(int voice)
{
  this->player.setVolume(voice);
  Data::changeSoundLevel(voice);
}

void MyPlayer::setPlayMode(int mode)
{
  this->playMode = mode;
  Data::changePlayMode(mode);
}

void MyPlayer::setPosition(qint64 pos)
{
  player.setPosition(pos);
}

QString MyPlayer::getArtist()
{
  return player.metaData(QMediaMetaData::ContributingArtist).toString();
}

void MyPlayer::addMusics(QString name, QQueue<MusicFountainInfo> q)
{
  MusicFountainInfo musicFountainInfo;
  while (!q.empty())
    {
      musicFountainInfo = q.front();
      q.pop_front();
      //      list[name].push_back(MusicFountainFile(QUrl::fromLocalFile(musicFountainInfo.musicInfo.getDir()),
      //                                             QUrl::fromLocalFile(musicFountainInfo.fountainInfo.getDir())));
      list[name].push_back(musicFountainInfo);
    }
}

void MyPlayer::removeTheMusic(QString listName, int index)
{
  list[listName].removeAt(index);
  if (listName == currentList && index == currentindex)
    {
      player.stop();
      fountainPlayer->stop();

      emit musicNotAvailable();
      emit playerStateChanged(NOMUSIC);
    }
}

void MyPlayer::removeAllMusics(QString listName)
{
  list[listName].clear();
  if (list[currentList].size() == 0)
    {
      player.stop();
      fountainPlayer->stop();
      emit musicNotAvailable();
      emit playerStateChanged(NOMUSIC);
    }
}

void MyPlayer::moveMusic(QString listName, int from, int to)
{
  /*MusicFountainFile*/MusicFountainInfo tmp;
  tmp = list[listName][from];
  list[listName].removeAt(from);
  list[listName].insert(to, tmp);
  //    QUrl tmp;
  //    tmp = list[listName][from];
  //    list[listName].removeAt(from);
  //    list[listName].insert(to, tmp);

}

void MyPlayer::setCurrentIndex(int index)
{
  qDebug()<< Q_FUNC_INFO;
  this->currentindex = index;

  bool noMusic = false, noFountain = false;
  qDebug() << "noMusic is:" << noMusic;
  qDebug()<< "Checking Music";
  if (list[currentList][index].getMusicUrl().isValid() &&
      QFile::exists(list[currentList][index].getMusicUrl().path()))
    {
      qDebug()<< "music is valid";
      QString filePath = list[currentList][index].getMusicUrl().path();
      qDebug()<< filePath;
      isMusicFinished = false;
      this->player.setMedia(QMediaContent(list[currentList][index].getMusicUrl()));

    }
  else
    {
      qDebug()<< "music is invalid";
      noMusic = true;
    }
  qDebug()<< "Checking fountain";
  if (list[currentList][index].getFountainUrl().isValid() &&
      QFile::exists(list[currentList][index].getFountainUrl().path()))
    {
      qDebug() << "fountain valid";
      isFountainFinished = false;
      fountainPlayer->setMedia(list[currentList][index].getFountainUrl());
    }
  else
    {
      qDebug() << "fountain invalid";
      noFountain = true;
    }
  if (noMusic)
    {
      qDebug()<< "setting no music";
      this->player.setMedia(QMediaContent());
      isMusicFinished = true;
    }
  if (noFountain)
    {
      qDebug()<< "setting no fountain";
      fountainPlayer->setMedia(QUrl());
      isFountainFinished = true;
    }

  qDebug()<< Q_FUNC_INFO << "Finished";
}

int MyPlayer::getCurrentIndex()
{
  return this->currentindex;
}

int MyPlayer::mediaCount()
{
  return this->list[currentList].size();
}

//void MyPlayer::addMedia(QString listName, QUrl musicUrl, QUrl fountainUrl)
//{
//  this->list[listName].push_back(MusicFountainFile(musicUrl, fountainUrl));
//}

void MyPlayer::clear(QString listName)
{
  this->list[listName].clear();
  if (list[currentList].size() == 0)
    {
      player.stop();
      fountainPlayer->stop();

    }

}

//void MyPlayer::playNext(QMediaPlayer::MediaStatus state)
//{
////  qDebug()<< state;
////  if (!(state == QMediaPlayer::EndOfMedia))
////    return;
//  switch (playMode)
//    {
//    case CurrentItemOnce:
//      player.stop();
//      fountainPlayer->stop();

//      emit playerStateChanged(PAUSE);
//      break;
//    case CurrentItemInLoop:
//      player.play();
//      fountainPlayer->play();

//      emit playerStateChanged(PLAYING);
//      break;
//    case Sequential:
//      currentindex++;
//      if (currentindex < list[currentList].size())
//        {
//          setCurrentIndex(currentindex);
//          player.play();
//          fountainPlayer->play();

//          emit playerStateChanged(PLAYING);
//        }
//      else
//        {
//          currentindex--;
//          player.stop();
//          fountainPlayer->stop();

//          emit playerStateChanged(PAUSE);
//        }
//      break;
//    case Loop:
//      currentindex++;
//      if (currentindex < list[currentList].size())
//        {
//          setCurrentIndex(currentindex);
//          player.play();
//          fountainPlayer->play();

//          emit playerStateChanged(PLAYING);
//        }
//      else
//        {
//          currentindex = 0;
//          setCurrentIndex(currentindex);
//          player.play();
//          fountainPlayer->play();

//          emit playerStateChanged(PLAYING);
//        }
//      break;
//    case Random:
//      QTime time;
//      time = QTime::currentTime();
//      qsrand(time.msec()+time.second()*1000);
//      currentindex = qrand()%list[currentList].size();
//      setCurrentIndex(currentindex);
//      player.play();
//      fountainPlayer->play();

//      emit playerStateChanged(PLAYING);
//      break;
//    }
//}

void MyPlayer::playTheMusic(QString listName, int index)
{
  currentList = listName;
  this->setCurrentIndex(index);
  player.play();
  fountainPlayer->play();

  emit playerStateChanged(PLAYING);
}

void MyPlayer::onMusicStatusChange(const QMediaPlayer::MediaStatus& state)
{

  qDebug()<< "Music " << state;
  if (state == QMediaPlayer::EndOfMedia ||
      state == QMediaPlayer::NoMedia ||
      state == QMediaPlayer::InvalidMedia)
    {
      isMusicFinished = true;
    }
  else
    {
      isMusicFinished = false;
    }
  handleMediaStatusChange();
}

void MyPlayer::onFountainStatusChange(const QMediaPlayer::MediaStatus &state)
{
  qDebug()<< Q_FUNC_INFO << state;
  if (state == QMediaPlayer::EndOfMedia ||
      state == QMediaPlayer::NoMedia ||
      state == QMediaPlayer::InvalidMedia)
    {
      isFountainFinished = true;
    }
  else
    {
      isFountainFinished = false;
    }
  handleMediaStatusChange();
}

void MyPlayer::handleMediaStatusChange()
{
  qDebug()<< Q_FUNC_INFO;
  qDebug()<<"handleMediaStatusChange F:M" << isFountainFinished << " " << isMusicFinished;
  if (isFountainFinished && isMusicFinished)
    {
      switch (playMode)
        {
        //        case CurrentItemOnce:
        //          player.stop();
        //          fountainPlayer->stop();

        //          emit playerStateChanged(PAUSE);
        //          break;
        case CurrentItemInLoop:

          setCurrentIndex(currentindex);
          player.play();
          fountainPlayer->play();

          emit playerStateChanged(PLAYING);
          break;
        case Sequential:
          //          currentindex++;
          //          if (currentindex < list[currentList].size())
          //            {
          if (jumpToNextValid())
            {
              setCurrentIndex(currentindex);
              player.play();
              fountainPlayer->play();

              emit playerStateChanged(PLAYING);
            }
          else
            {
              //              currentindex--;
              player.stop();
              fountainPlayer->stop();

              emit playerStateChanged(PAUSE);
            }
          break;
        case Loop:
          //          currentindex++;

          //          if (currentindex < list[currentList].size())
          //            {
          if (jumpToNextValid(true))
            {
              setCurrentIndex(currentindex);
              player.play();
              fountainPlayer->play();

              emit playerStateChanged(PLAYING);
            }
          else
            {
              //              currentindex = 0;
              //              setCurrentIndex(currentindex);
              //              player.play();
              //              fountainPlayer->play();

              //              emit playerStateChanged(PLAYING);

              player.stop();
              fountainPlayer->stop();

              emit playerStateChanged(PAUSE);
            }
          break;
        case Random:
          //          QTime time;
          //          time = QTime::currentTime();
          //          qsrand(time.msec()+time.second()*1000);
          //          currentindex = qrand()%list[currentList].size();
          //          setCurrentIndex(currentindex);
          //          player.play();
          //          fountainPlayer->play();

          //          emit playerStateChanged(PLAYING);
          if (jumpToRandomValid())
            {
              setCurrentIndex(currentindex);
              player.play();
              fountainPlayer->play();

              emit playerStateChanged(PLAYING);
            }
          else
            {
              player.stop();
              fountainPlayer->stop();

              emit playerStateChanged(PAUSE);
            }
          break;
        }
    }
  qDebug()<< Q_FUNC_INFO << "Finished ";
}

bool MyPlayer::playPre()
{
  //  if (this->mediaCount() == 0)
  //    return false;
  //  int index = this->getCurrentIndex();
  //  if(--index <= 0)
  //    index=0;
  if (jumpToPreValid(true))
    {
      this->setCurrentIndex(currentindex);
      player.play();
      fountainPlayer->play();
      emit playerStateChanged(PLAYING);
      return true;
    }
  else
    {
      player.pause();
      fountainPlayer->pause();
      emit playerStateChanged(PAUSE);
    }
}

bool MyPlayer::playNext()
{
  //  if (this->mediaCount() == 0)
  //    return false;
  //  int index = this->getCurrentIndex();
  //  if(++index == this->mediaCount())
  //    index=0;
  if (jumpToNextValid(true))
    {
      this->setCurrentIndex(currentindex);
      player.play();
      fountainPlayer->play();
      emit playerStateChanged(PLAYING);
      return true;
    }
  else
    {
      player.pause();
      fountainPlayer->pause();
      emit playerStateChanged(PAUSE);
    }
}

void MyPlayer::play()
{
  player.play();
  fountainPlayer->play();

  emit playerStateChanged(PLAYING);
}

void MyPlayer::pause()
{
  player.pause();
  fountainPlayer->pause();

  emit playerStateChanged(PAUSE);
}

void MyPlayer::playFirstValid(const QString& listName)
{
  currentList = listName;
  currentindex = mediaCount() -1 ;
  if (jumpToNextValid(true))
    {
      qDebug()<<"Playing the first valid";
      this->setCurrentIndex(currentindex);
      player.play();
      fountainPlayer->play();
      emit playerStateChanged(PLAYING);
    }
}

int MyPlayer::getListSize()
{
  return mediaCount();
}
