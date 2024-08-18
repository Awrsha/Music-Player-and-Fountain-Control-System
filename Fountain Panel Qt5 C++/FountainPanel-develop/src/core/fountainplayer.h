#ifndef FOUNTAINPLAYER_H
#define FOUNTAINPLAYER_H

#include <QObject>
#include <QThread>
#include <QMediaPlayer>
#include <QMutex>
#include <QElapsedTimer>
#include <QFile>
#include <QDataStream>

class FountainPlayer : public QThread
{
  Q_OBJECT
public:
  explicit FountainPlayer(QObject *parent = 0);

signals:
  void fountainStatusChanged(const QMediaPlayer::MediaStatus& mediaStatus);
  void durationChanged(const qint64& duration);
  void positionChanged(const qint64& duration);

public slots:
  void play();
  void pause();
  void stop();

  void setMedia(const QUrl& fileAddress);
  // QThread interface
protected:
  virtual void run();

private:
  inline void startTimer();
  inline qint64 getElapsedMicroSecs();
  inline qint64 getSleepMicroSecs(const qint64 &elapsedMicroSecs);
  inline bool isFilePlaybackRemaining();
//  inline void sendDmxData(const QVector<quint8> dmxFrame);
  static QVector<QVector<quint8> > readDataFromFile(const QUrl& fileAddress);

//  void init();
//  void finalize();

private:
  int notifyInterval;
  QElapsedTimer elapsedTimer;
  QMutex runningMutex;
  bool running;
//  int fileDescriptor;

//  bool initilized;
  QVector<QVector<quint8> > currentData;

  quint64 currentFrameIndex;

  static const quint16 DMX_FRAME_COUNT;


  QElapsedTimer totalTime;


};

#endif // FOUNTAINPLAYER_H
