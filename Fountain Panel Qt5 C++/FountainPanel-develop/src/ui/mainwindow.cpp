#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMediaMetaData>
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
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  qRegisterMetaType<DigialInputResponse>("DigialInputResponse");
  qRegisterMetaType<DigialOutputResponse>("DigialOutputResponse");
  qRegisterMetaType<AddressBook::OutputRamEntity>("AddressBook::OutputRamEntity");
  qRegisterMetaType<AddressBook::InputRamEntity>("AddressBook::InputRamEntity");
  qRegisterMetaType<ProcedureViewForm::ProcedureState>("ProcedureViewForm::ProcedureState");
  qRegisterMetaType<ProcedureViewForm::TimedProcedureState>("ProcedureViewForm::TimedProcedureState");
  qRegisterMetaType<IntBoolMap>("IntBoolMap");


  ConfigurationManager::getInstance().init();

  ui->setupUi(this);

  player = new MyPlayer();
  connect(player, SIGNAL(playerStateChanged(int)), this, SLOT(playerStateChanged(int)));
  connect(player, SIGNAL(musicChanged(QString,int)), this, SLOT(musicChanged(QString,int)));



  musicPage = /*new MusicPage*/ui->musicPage;
  //  musicPage->setMaximumWidth(500);
  musicPage->setMinimumWidth(300);
  connect(musicPage, SIGNAL(tryToCreateList(QString)), this, SLOT(tryToCreateList(QString)));
  connect(musicPage, SIGNAL(deleteList(QString)), this, SLOT(deleteList(QString)));
  //  connect(musicPage, SIGNAL(addMusics()), this, SLOT(addMusics()));
  connect(musicPage, SIGNAL(play(QString,int)), player, SLOT(playTheMusic(QString,int)));
  connect(musicPage, SIGNAL(removeTheMusic(QString,int)), this, SLOT(removeTheMusic(QString,int)));
  connect(musicPage, SIGNAL(removeAllMusics(QString)), this, SLOT(removeAllMusics(QString)));
  connect(musicPage, SIGNAL(moveMusic(QString,int,int)), this, SLOT(moveMusic(QString,int,int)));

  bottomBar = /*new BottomBar(this)*/ui->bottomBar;
  connect(bottomBar, SIGNAL(setVoice(int)), player, SLOT(setVoice(int)));
  connect(bottomBar, SIGNAL(setMode(int)), player, SLOT(setPlayMode(int)));
  connect(bottomBar, SIGNAL(setPostion(qint64)), player, SLOT(setPosition(qint64)));
  //  connect(bottomBar, SIGNAL(setPostion(qint64)), lyricLabel, SLOT(setPostion(qint64)));
  connect(player, SIGNAL(musicDurationChanged(qint64)), bottomBar, SLOT(onSetMaxMusicDuration(qint64)));
  connect(player, SIGNAL(fountainDurationChanged(qint64)), bottomBar, SLOT(onSetMaxFountainDuration(qint64)));
  connect(player, SIGNAL(musicPositionChanged(qint64)), bottomBar, SLOT(onUpdateMusicPosition(qint64)));
  connect(player, SIGNAL(fountainPositionChanged(qint64)), bottomBar, SLOT(onUpdateFountainPosition(qint64)));
  connect(musicPage, SIGNAL(play(QString,int)), bottomBar, SLOT(setPostionAvailable()));
  connect(bottomBar, SIGNAL(tryplay()), this, SLOT(clickPlay()));
  connect(bottomBar, SIGNAL(playPre()), this, SLOT(playPre()));
  connect(bottomBar, SIGNAL(playNext()), this, SLOT(playNext()));
  connect(bottomBar, SIGNAL(addMusicClicked()), SLOT(addMusics()));

  //  connect(systemTrayIcon, SIGNAL(setMode(int)), bottomBar, SLOT(setPlayMode(int)));
  //  connect(bottomBar, SIGNAL(setMode(int)), systemTrayIcon, SLOT(setPlayMode(int)));
  //  connect(bottomBar, SIGNAL(showLyric(QString)), topBar, SLOT(changeFuction(QString)));



  digitalIoInterface = new DigitalIOInterface();
  procedureManager = new ProcedureManager(ui->procedureView);
  procedureManagerThread = new QThread();
  procedureManager->moveToThread(procedureManagerThread);

  connect(ui->ioDebugForm, SIGNAL(sendIOCommand(AddressBook::AddressPair,bool)),
          digitalIoInterface, SLOT(onSendIOCommand(AddressBook::AddressPair,bool)));
  connect(digitalIoInterface, SIGNAL(receivedInputUpdate(DigialInputResponse)),
          ui->ioDebugForm, SLOT(onReceiveInputUpdate(DigialInputResponse)));
  connect(digitalIoInterface, SIGNAL(receivedOutputUpdate(DigialOutputResponse)),
          ui->ioDebugForm, SLOT(onReceiveOutputUpdate(DigialOutputResponse)));

  connect(digitalIoInterface, SIGNAL(receivedInputUpdate(DigialInputResponse)),
          bottomBar, SLOT(onReceiveInputUpdate(DigialInputResponse)));
  connect(digitalIoInterface, SIGNAL(receivedInputUpdate(DigialInputResponse)),
          player, SLOT(onReceiveInputUpdate(DigialInputResponse)));

  connect(digitalIoInterface, SIGNAL(receivedInputUpdate(DigialInputResponse)),
          procedureManager, SLOT(onReceiveInputUpdate(DigialInputResponse)));

  connect(procedureManager, SIGNAL(sendPlayPause(bool)),
          SLOT(onReceivedPlayPause(bool)));
  connect(procedureManager, SIGNAL(sendIOCommand(AddressBook::AddressPair,bool)),
          digitalIoInterface, SLOT(onSendIOCommand(AddressBook::AddressPair,bool)));
  connect(procedureManager, SIGNAL(sendIOCommand(AddressBook::OutputRamEntity,bool)),
          digitalIoInterface, SLOT(onSendIOCommand(AddressBook::OutputRamEntity,bool)));


  digitalIoInterface->start();
  procedureManagerThread->start();


  ui->mainToolBar->hide();
  ui->menuBar->hide();
  ui->statusBar->hide();
  setWindowFlags(Qt::FramelessWindowHint);


  // uncomment this line if you need to write a bit in start up.
  // digitalIoInterface->onSendIOCommand(AddressBook::OUTPUT_24, true);


  readData();
  setupLoginPage();

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
      bottomBar->setMusicTitle("Music's' Name");
      bottomBar->setFountainTitle("Fountain's Name");
      bottomBar->onSetMaxMusicDuration(0);
      bottomBar->onSetMaxFountainDuration(0);
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
  qDebug() << "playback changed" << endl;
  QString artist = player->getArtist();
  QString musicName = Data::getMusicName(listName, index);
  if (!musicName.isEmpty())
    {
      bottomBar->setMusicTitle(musicName);
    }
  else
    {
      bottomBar->setNoMusic();
    }

  QString fountainName = Data::getFountainName(listName, index);
  if (!fountainName.isEmpty())
    {
      bottomBar->setFountainTitle(fountainName);
    }
  else
    {
      bottomBar->setNoFountain();
    }
  musicPage->removeHighLight();
  musicPage->setHighLight(Data::getListId(listName), index);
  if (!artist.isEmpty())
    {
      musicPage->setArtist(Data::getListId(listName), index, artist);
      Data::setArtist(listName, index, artist);
    }


  QString dir = Data::getMusicDir(listName, index);

  ui->procedureView->setPlaybackInfo(musicName, fountainName, listName, index, player->getListSize());

  if (!dir.isEmpty())
    {
      int len = dir.length();
      while (dir.at(len-1) != '.')
        {
          dir.remove(len-1, 1);
          len--;
        }
      dir = dir+"lrc";
    }

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

  //  addDialog.setModal(true);

  if (addDialog.exec() == QDialog::Accepted)
    {
      MusicFountainInfo musicFountainInfo;

      musicFountainInfo.musicInfo.setDir(addDialog.getMusicAddress());
      musicFountainInfo.musicInfo.setName(QFileInfo(addDialog.getMusicAddress()).completeBaseName());
      musicFountainInfo.musicInfo.setArtist("");

      musicFountainInfo.fountainInfo.setDir(addDialog.getFountainAddress());
      musicFountainInfo.fountainInfo.setName(QFileInfo(addDialog.getFountainAddress()).completeBaseName());
      musicFountainInfo.fountainInfo.setArtist("");

      musicFountainInfo.setDayNightMode(addDialog.getDayNightMode());
      musicFountainInfo.setNoWindWindMode(addDialog.getNoWindWindMode());

      musicPage->addMusic(musicFountainInfo);


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

void MainWindow::onReceivedPlayPause(const bool &play)
{
  if (play)
    {
      player->playFirstValid(musicPage->getCurrentList());
    }
  else
    {
      player->pause();
    }
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
  musicPage->setCurrentList(Data::getLastListIndex());
}

void MainWindow::setupLoginPage()
{
  connect(ui->lockScreenWidget, SIGNAL(loginPassed()),
          SLOT(setNormalPage()));
  connect(&loginPageTimer, SIGNAL(timeout()),
          SLOT(setLoginPage()));

  loginPageTimer.setInterval(30000);
  loginPageTimer.setSingleShot(true);

  this->installEventFilter(this);
  setMouseTracking(true );

}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
  //qDebug()<<"Event filter";
  if(event->type() == QEvent::MouseMove ||
     event->type() == QEvent::HoverMove)
    {
      loginPageTimer.start();
    }
  // Repeat for other mouse events

  return false;
}

void MainWindow::on_settingButton_clicked()
{
  changePasswordDialog.exec();
}

void MainWindow::setLoginPage()
{
  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::setNormalPage()
{
  ui->stackedWidget->setCurrentIndex(1);
  loginPageTimer.start();
}

void MainWindow::on_lockButton_clicked()
{
  ui->stackedWidget->setCurrentIndex(0);
}
