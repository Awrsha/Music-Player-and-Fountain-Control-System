#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QQueue>
#include <QUrl>
#include <QMediaPlayer>
#include "mediainfo.h"

#include "fountainplayer.h"
#include "digitaliointerfaceclient.h"

class MusicFountainFile
{
public:
  MusicFountainFile (QUrl musicUrl, QUrl fountainUrl):
    musicUrl(musicUrl),
    fountainUrl(fountainUrl)
  {

  }
  MusicFountainFile() {}
  QUrl musicUrl;
  QUrl fountainUrl;
};

class MyPlayer : public QObject, public DigitalIOInterfaceClient
{
  Q_OBJECT

public:
  enum PlyaerState
  {
    NOMUSIC,
    PAUSE,
    PLAYING
  };

public:
  MyPlayer();
  void setPlay();
  void setPause();
  void setCurrentIndex(int index);
  int getCurrentIndex();
  int getPlayerState();
  int mediaCount();
//  void addMedia(QString listName, QUrl musicUrl, QUrl fountainUrl);
  void clear(QString listName);
  QString getArtist();

public slots:
  void setVoice(int voice);
  void setPlayMode(int mode);
  void addMusics(QString name, QQueue<MusicFountainInfo> q);
  void removeTheMusic(QString listName, int index);
  void removeAllMusics(QString listName);
  void moveMusic(QString listName, int from, int to);
  void setPosition(qint64 pos);
//  void playNext(QMediaPlayer::MediaStatus state);
  void playTheMusic(QString listName, int index);
  void onMusicStatusChange(const QMediaPlayer::MediaStatus &state);
  void onFountainStatusChange(const QMediaPlayer::MediaStatus &state);
  void handleMediaStatusChange();
  bool playPre();
  bool playNext();
  void play();
  void pause();

  void playFirstValid(const QString &listName);

  int getListSize();
private slots:
  void musicChanged();
  void stateChanged(int state);


  bool jumpToNextValid(bool loopIsAllowed = false);
  bool jumpToPreValid(bool loopIsAllowed = false);
  bool jumpToRandomValid();
signals:
  void musicDurationChanged(qint64);
  void fountainDurationChanged(qint64);
  void musicChanged(QString listName, int index);
  void musicPositionChanged(qint64);
  void fountainPositionChanged(qint64);
  void musicNotAvailable();
  void playerStateChanged(int state);

private:
  QMediaPlayer player;
  FountainPlayer* fountainPlayer;

  QString currentList;
  QMap<QString, QVector <MusicFountainInfo> > list;
  int playMode;
  int currentindex;
  int playerState;


  bool isMusicFinished;
  bool isFountainFinished;

  MusicFountainInfo::WorkMode currentDayNightMode;
  MusicFountainInfo::WorkMode currentNoWindWindMode;


  // DigitalIOInterfaceClient interface
public slots:
  virtual void onReceiveInputUpdate(const DigialInputResponse &response);
};

#endif // MYPLAYER_H
