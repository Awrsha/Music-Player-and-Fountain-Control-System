#include "myplayer.h"
#include "playmode.h"
#include <QMediaContent>
#include <QTime>
#include <QMediaMetaData>
#include <QThread>
MyPlayer::MyPlayer()
{
  playerState = NOMUSIC;
  player.setNotifyInterval(500);
  connect(&player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(playNext(QMediaPlayer::MediaStatus)));
  connect(&player, SIGNAL(durationChanged(qint64)), this, SIGNAL(durationChanged(qint64)));
  connect(&player, SIGNAL(durationChanged(qint64)), this, SLOT(musicChanged()));
  connect(&player, SIGNAL(positionChanged(qint64)), this, SIGNAL(positionChanged(qint64)));
  connect(this, SIGNAL(playerStateChanged(int)), this, SLOT(stateChanged(int)));

  fountainPlayer = new FountainPlayer();
  fountainPlayer->start(QThread::TimeCriticalPriority);
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

void MyPlayer::setVoice(int voice)
{
  this->player.setVolume(voice);
}

void MyPlayer::setPlayMode(int mode)
{
  this->playMode = mode;
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
      list[name].push_back(MusicFountainFile(QUrl::fromLocalFile(musicFountainInfo.musicInfo.getDir()),
                                             QUrl::fromLocalFile(musicFountainInfo.fountainInfo.getDir())));
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
  MusicFountainFile tmp;
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
  this->currentindex = index;
  this->player.setMedia(QMediaContent(list[currentList][index].musicUrl));
  fountainPlayer->setMedia(list[currentList][index].fountainUrl);

}

int MyPlayer::getCurrentIndex()
{
  return this->currentindex;
}

int MyPlayer::mediaCount()
{
  return this->list[currentList].size();
}

void MyPlayer::addMedia(QString listName, QUrl musicUrl, QUrl fountainUrl)
{

  this->list[listName].push_back(MusicFountainFile(musicUrl, fountainUrl));
}

void MyPlayer::clear(QString listName)
{
  this->list[listName].clear();
  if (list[currentList].size() == 0)
    {
      player.stop();
      fountainPlayer->stop();

    }

}

void MyPlayer::playNext(QMediaPlayer::MediaStatus state)
{
  if (!(state == QMediaPlayer::EndOfMedia))
    return;
  switch (playMode)
    {
    case CurrentItemOnce:
      player.stop();
      fountainPlayer->stop();

      emit playerStateChanged(PAUSE);
      break;
    case CurrentItemInLoop:
      player.play();
      fountainPlayer->play();

      emit playerStateChanged(PLAYING);
      break;
    case Sequential:
      currentindex++;
      if (currentindex < list[currentList].size())
        {
          setCurrentIndex(currentindex);
          player.play();
          fountainPlayer->play();

          emit playerStateChanged(PLAYING);
        }
      else
        {
          currentindex--;
          player.stop();
          fountainPlayer->stop();

          emit playerStateChanged(PAUSE);
        }
      break;
    case Loop:
      currentindex++;
      if (currentindex < list[currentList].size())
        {
          setCurrentIndex(currentindex);
          player.play();
          fountainPlayer->play();

          emit playerStateChanged(PLAYING);
        }
      else
        {
          currentindex = 0;
          setCurrentIndex(currentindex);
          player.play();
          fountainPlayer->play();

          emit playerStateChanged(PLAYING);
        }
      break;
    case Random:
      QTime time;
      time = QTime::currentTime();
      qsrand(time.msec()+time.second()*1000);
      currentindex = qrand()%list[currentList].size();
      setCurrentIndex(currentindex);
      player.play();
      fountainPlayer->play();

      emit playerStateChanged(PLAYING);
      break;
    }
}

void MyPlayer::playTheMusic(QString listName, int index)
{
  currentList = listName;
  this->setCurrentIndex(index);
  player.play();
  fountainPlayer->play();

  emit playerStateChanged(PLAYING);
}

bool MyPlayer::playPre()
{
  if (this->mediaCount() == 0)
    return false;
  int index = this->getCurrentIndex();
  if(--index <= 0)
    index=0;
  this->setCurrentIndex(index);
  player.play();
  fountainPlayer->play();

  emit playerStateChanged(PLAYING);
  return true;
}

bool MyPlayer::playNext()
{
  if (this->mediaCount() == 0)
    return false;
  int index = this->getCurrentIndex();
  if(++index == this->mediaCount())
    index=0;
  this->setCurrentIndex(index);
  player.play();
  fountainPlayer->play();
  emit playerStateChanged(PLAYING);
  return true;
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
