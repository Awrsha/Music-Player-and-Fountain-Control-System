#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "bottombar.h"
#include "musicpage.h"
#include "src/core/data.h"
#include "src/core/myplayer.h"
#include <QIcon>
#include <QPainter>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QQueue>
#include <QFileDialog>
#include <QStackedWidget>
#include "addtoplaylistdialog.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  player = new MyPlayer();
  connect(player, SIGNAL(playerStateChanged(int)), this, SLOT(playerStateChanged(int)));
  connect(player, SIGNAL(musicChanged(QString,int)), this, SLOT(musicChanged(QString,int)));



  musicPage = /*new MusicPage*/ui->musicPage;
  //  musicPage->setMaximumWidth(500);
  musicPage->setMinimumWidth(300);
  connect(musicPage, SIGNAL(tryToCreateList(QString)), this, SLOT(tryToCreateList(QString)));
  connect(musicPage, SIGNAL(deleteList(QString)), this, SLOT(deleteList(QString)));
  connect(musicPage, SIGNAL(addMusics()), this, SLOT(addMusics()));
  connect(musicPage, SIGNAL(play(QString,int)), player, SLOT(playTheMusic(QString,int)));
  connect(musicPage, SIGNAL(removeTheMusic(QString,int)), this, SLOT(removeTheMusic(QString,int)));
  connect(musicPage, SIGNAL(removeAllMusics(QString)), this, SLOT(removeAllMusics(QString)));
  connect(musicPage, SIGNAL(moveMusic(QString,int,int)), this, SLOT(moveMusic(QString,int,int)));

  bottomBar = /*new BottomBar(this)*/ui->bottomBar;
  connect(bottomBar, SIGNAL(setVoice(int)), player, SLOT(setVoice(int)));
  connect(bottomBar, SIGNAL(setMode(int)), player, SLOT(setPlayMode(int)));
  connect(bottomBar, SIGNAL(setPostion(qint64)), player, SLOT(setPosition(qint64)));
  //  connect(bottomBar, SIGNAL(setPostion(qint64)), lyricLabel, SLOT(setPostion(qint64)));
  connect(player, SIGNAL(durationChanged(qint64)), bottomBar, SLOT(setMaxDuration(qint64)));
  connect(player, SIGNAL(positionChanged(qint64)), bottomBar, SLOT(changePostionTo(qint64)));
  connect(musicPage, SIGNAL(play(QString,int)), bottomBar, SLOT(setPostionAvailable()));
  connect(bottomBar, SIGNAL(tryplay()), this, SLOT(clickPlay()));
  connect(bottomBar, SIGNAL(playPre()), this, SLOT(playPre()));
  connect(bottomBar, SIGNAL(playNext()), this, SLOT(playNext()));
  //  connect(systemTrayIcon, SIGNAL(setMode(int)), bottomBar, SLOT(setPlayMode(int)));
  //  connect(bottomBar, SIGNAL(setMode(int)), systemTrayIcon, SLOT(setPlayMode(int)));
  //  connect(bottomBar, SIGNAL(showLyric(QString)), topBar, SLOT(changeFuction(QString)));

  ui->mainToolBar->hide();
  ui->menuBar->hide();
  ui->statusBar->hide();

  readData();

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::playerStateChanged(int state)
{
  switch (state)
    {
    case MyPlayer::NOMUSIC:
      bottomBar->setPause();
      bottomBar->setMusicTitle("Music Name");
      bottomBar->setMaxDuration(0);
      bottomBar->setPostionNotAvailable();
      break;
    case MyPlayer::PAUSE:
      bottomBar->setPause();
      bottomBar->setPostionAvailable();
      break;
    case MyPlayer::PLAYING:
      bottomBar->setPlay();
      bottomBar->setPostionAvailable();
      break;
    }
}

void MainWindow::musicChanged(QString listName, int index)
{
  qDebug() << "music changed" << endl;
  QString artist = player->getArtist();
  bottomBar->setMusicTitle(Data::getMusicName(listName, index));
  musicPage->removeHighLight();
  musicPage->setHighLight(Data::getListId(listName), index);
  musicPage->setArtist(Data::getListId(listName), index, artist);
  Data::setArtist(listName, index, artist);

  //处理歌词
  //先对数据库查询歌词
  QString dir = Data::getMusicDir(listName, index);
  int len = dir.length();
  while (dir.at(len-1) != '.')
    {
      dir.remove(len-1, 1);
      len--;
    }
  dir = dir+"lrc";
}


void MainWindow::clickPlay()
{
  switch (player->getPlayerState())
    {
    case MyPlayer::NOMUSIC:
      break;
    case MyPlayer::PAUSE:
      player->setPlay();
      //bottomBar->setPlay();
      break;
    case MyPlayer::PLAYING:
      player->setPause();
      //bottomBar->setPause();
      break;
    }
}

void MainWindow::playPre()
{
  if (player->getPlayerState() == MyPlayer::NOMUSIC) return;
  if (!player->playPre()) return;
}

void MainWindow::playNext()
{
  if (player->getPlayerState() == MyPlayer::NOMUSIC) return;
  if (!player->playNext()) return;
}

void MainWindow::addMusics()
{

  AddToPlaylistDialog addDialog;

  if (addDialog.exec() == QDialog::Accepted)
    {
      MusicFountainInfo musicFountainInfo;

      musicFountainInfo.musicInfo.setDir(addDialog.getMusicAddress());
      musicFountainInfo.musicInfo.setName(QFileInfo(addDialog.getMusicAddress()).baseName());
      musicFountainInfo.fountainInfo.setDir(addDialog.getFountainAddress());
      musicFountainInfo.fountainInfo.setName(QFileInfo(addDialog.getFountainAddress()).baseName());

      musicPage->addMusic(musicFountainInfo);


      musicFountainInfo.musicInfo.setDir(addDialog.getMusicAddress());
      musicFountainInfo.musicInfo.setName(QFileInfo(addDialog.getMusicAddress()).baseName());
      musicFountainInfo.musicInfo.setArtist("");

      musicFountainInfo.fountainInfo.setDir(addDialog.getFountainAddress());
      musicFountainInfo.fountainInfo.setName(QFileInfo(addDialog.getFountainAddress()).baseName());
      musicFountainInfo.fountainInfo.setArtist("");


      QQueue<MusicFountainInfo> musicFountainQueue;
      musicFountainQueue.push_back(musicFountainInfo);

      player->addMusics(musicPage->getCurrentList(), musicFountainQueue);


//      QQueue<MediaInfo> queue;
//      queue.push_back(musicFountainInfo.musicInfo);
//      Data::addMusicsToEnd(musicPage->getCurrentList(), queue);
        Data::addMusicsToEnd(musicPage->getCurrentList(), musicFountainQueue);
    }


  //  QQueue<MediaInfo> musics;
  //  MediaInfo musicInfo;
  //  QStringList fileDir = QFileDialog::getOpenFileNames(
  //        this,
  //        tr("Add"),
  //        ".",
  //        tr("MP3 Audio files(*.mp1 *.mp2 *.mp3);; WMA Audio files(*.wma)"));
  //  if (fileDir.isEmpty())
  //    return;
  //  QStringList::iterator it;
  //  musicPage->addMusics(fileDir);
  //  for (it = fileDir.begin(); it != fileDir.end(); ++it)
  //    {
  //      musicInfo.setDir(*it);
  //      musicInfo.setName(QFileInfo(*it).baseName());
  //      musicInfo.setArtist("");
  //      musics.push_back(musicInfo);
  //    }
  //  player->addMusics(musicPage->getCurrentList(), musics);
  //  Data::addMusicsToEnd(musicPage->getCurrentList(), musics);
}

void MainWindow::addMusic(MusicFountainInfo musicInfo)
{
  musicPage->addMusic(musicInfo);
  QQueue<MusicFountainInfo> musicFountainQueue;
  musicFountainQueue.push_back(musicInfo);
  player->addMusics(musicPage->getCurrentList(), musicFountainQueue);
  Data::addMusicsToEnd(musicPage->getCurrentList(), musicFountainQueue);
}

void MainWindow::removeTheMusic(QString listName, int index)
{
  player->removeTheMusic(listName, index);
  Data::deleteMusic(listName, index);
}

void MainWindow::removeAllMusics(QString listName)
{
  player->removeAllMusics(listName);
  Data::deleteAllMusic(listName);
}

void MainWindow::moveMusic(QString listName, int from, int to)
{
  player->moveMusic(listName, from, to);
  Data::moveMusic(listName, from, to);
}

void MainWindow::tryToCreateList(QString name)
{
  if (name.size() && Data::addList(name))
    musicPage->createList(name);
}

void MainWindow::deleteList(QString name)
{
  Data::deleteList(name);
  //try to delete from player;
}

void MainWindow::readData()
{
  if (!Data::connectData()) return;
  //  this->setBackgroud(Data::getCurrentBackground());
  this->bottomBar->setPlayMode(Data::getPlayMode());
  this->bottomBar->setSoundLevel(Data::getSoundLevel());

    QQueue <QString> qq = Data::getListList();
    QString listName;
    QQueue <MusicFountainInfo> q;
    while (!qq.empty())
      {
        listName = qq.front();
        qq.pop_front();
        musicPage->createList(listName);
        q = Data::getMusicList(listName);
        musicPage->addMusics(Data::getListId(listName), q);
        player->addMusics(listName, q);
      }
  musicPage->setCurrentList(0);
}
