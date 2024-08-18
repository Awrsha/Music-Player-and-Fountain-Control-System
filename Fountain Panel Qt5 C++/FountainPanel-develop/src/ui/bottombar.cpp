#include "bottombar.h"
#include "sliderwidget.h"
#include "voiceslider.h"
#include "iconbutton.h"
#include "mymenu.h"
#include "src/core/playmode.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "src/util/projecttheme.h"
#include <QPainter>

#include <QtDebug>

#include <QApplication>
BottomBar::BottomBar(QWidget *parent) : QWidget(parent), maxMusicDuration(0)
{
  this->setAttribute(Qt::WA_TranslucentBackground, true);//设置透明:窗体标题栏不透明,背景透明
  this->setFixedHeight(115);

  /*
    QPalette palette;//调色板
    palette.setColor(QPalette::Background, QColor(18, 0, 169));//设置调色板参数为背景色，RGB颜色为深蓝色
    this->setAutoFillBackground(true);//将组件的背景设为可调
    this->setPalette(palette);//将调色板应用于组件
    */

  musicSlider = new SliderWidget;
  musicTotalTimeLabel = new MyLabel;
  musicElapsedTimeLabel = new MyLabel;

  connect(musicSlider, SIGNAL(changeTo(qreal)), this, SLOT(postionChanged(qreal)));
  musicTotalTimeLabel->setText("--:--");
  musicTotalTimeLabel->setFixedWidth(38);
  musicTotalTimeLabel->setFont(QFont(QApplication::font().family(), 11));
  musicTotalTimeLabel->setAlignment(Qt::AlignCenter);
  musicElapsedTimeLabel->setText("--:--");
  musicElapsedTimeLabel->setFixedWidth(38);
  musicElapsedTimeLabel->setFont(QFont(QApplication::font().family(), 11));
  musicElapsedTimeLabel->setAlignment(Qt::AlignCenter);

  QHBoxLayout* musicSliderTimeLayout = new QHBoxLayout;

  musicSliderTimeLayout->addWidget(musicElapsedTimeLabel);
  musicSliderTimeLayout->addWidget(musicSlider);
  musicSliderTimeLayout->addWidget(musicTotalTimeLabel);
  musicSliderTimeLayout->setSpacing(6);

  fountainSlider = new SliderWidget;
  fountainTotalTimeLabel = new MyLabel;
  fountainElapsedTimeLabel = new MyLabel;

  fountainTotalTimeLabel->setText("--:--");
  fountainTotalTimeLabel->setFixedWidth(38);
  fountainTotalTimeLabel->setFont(QFont(QApplication::font().family(), 11));
  fountainTotalTimeLabel->setAlignment(Qt::AlignCenter);

  fountainElapsedTimeLabel->setText("--:--");
  fountainElapsedTimeLabel->setFixedWidth(38);
  fountainElapsedTimeLabel->setFont(QFont(QApplication::font().family(), 11));
  fountainElapsedTimeLabel->setAlignment(Qt::AlignCenter);


  QHBoxLayout* fountainSliderTimeLayout = new QHBoxLayout;

  fountainSliderTimeLayout->addWidget(fountainElapsedTimeLabel);
  fountainSliderTimeLayout->addWidget(fountainSlider);
  fountainSliderTimeLayout->addWidget(fountainTotalTimeLabel);
  fountainSliderTimeLayout->setSpacing(6);


  //    coverButton = new IconButton(":/res/icons/coverbutton_icon");
  //    connect(coverButton, SIGNAL(clicked()), this, SLOT(clickCover()));

  //  QPalette palette;//调色板
  //  palette.setColor(QPalette::WindowText, QColor(255, 255, 255));//设置调色板参数为按钮字体色，RGB颜色为白色
  //  musicNameLabel = new MyLabel;
  //  musicNameLabel->setText("Song's Name");
  //  musicNameLabel->setFixedWidth(150);
  //  musicNameLabel->setPalette(palette);

  //  musicTimeLabel = new MyLabel;
  //  musicTimeLabel->setText("--:-- | --:--");
  //  musicTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  //  musicTimeLabel->setFixedWidth(150);
  //  musicTimeLabel->setPalette(palette);

  //  fountainNameLabel = new MyLabel;
  //  fountainNameLabel->setText("Fountain's Name");
  //  fountainNameLabel->setFixedWidth(150);
  //  fountainNameLabel->setPalette(palette);

  //  fountainTimeLabel = new MyLabel;
  //  fountainTimeLabel->setText("--:-- | --:--");
  //  fountainTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  //  fountainTimeLabel->setFixedWidth(150);
  //  fountainTimeLabel->setPalette(palette);

  //  QVBoxLayout *labelLayout = new QVBoxLayout;
  //  labelLayout->addWidget(musicNameLabel);
  //  labelLayout->addWidget(musicTimeLabel);

  //  labelLayout->addWidget(fountainNameLabel);
  //  labelLayout->addWidget(fountainTimeLabel);

  //  QHBoxLayout *leftLayout = new QHBoxLayout;
  //  //    leftLayout->addWidget(coverButton);
  //  //    leftLayout->addSpacing(8);
  //  leftLayout->addLayout(labelLayout);
  //  leftLayout->setMargin(0);

  QSize buttonLSize(64, 64);
  QSize buttonMSize(48, 48);

  preButton = new /*IconButton*/QIconButton(":/res/icons/prebutton_icon", buttonMSize);
  connect(preButton, SIGNAL(clicked(bool)), this, SIGNAL(playPre()));

  playButton = new /*IconButton*/QIconButton(":/res/icons/pausebutton_icon", buttonLSize);
  connect(playButton, SIGNAL(clicked(bool)), this, SIGNAL(tryplay()));

  nextButton = new /*IconButton*/QIconButton(":/res/icons/nextbutton_icon", buttonMSize);
  connect(nextButton, SIGNAL(clicked(bool)), this, SIGNAL(playNext()));

  nosound = false;
  soundButton = new /*IconButton*/QIconButton(":/res/icons/soundbutton_icon", buttonMSize);
  connect(soundButton, SIGNAL(clicked(bool)), this, SLOT(clickSoundButton()));

  voiceSlider = new VoiceSlider;
  voiceSlider->setFixedWidth(60);
  voiceSlider->moveTo(0.25);//初始音量
  connect(voiceSlider, SIGNAL(changeTo(qreal)), this, SLOT(voiceChanged(qreal)));

  addMusicButton = new /*IconButton*/QIconButton(":/res/icons/add_music_icon", buttonMSize);
  connect(addMusicButton, SIGNAL(clicked(bool)), SIGNAL(addMusicClicked()));

  dayNightState = new QLabel();
  dayNightState->setPixmap(QPixmap(ProjectTheme::Resource::ICON_ONLY_DAY).scaledToWidth( 60, Qt::SmoothTransformation));

  noWindWindState = new QLabel();
  noWindWindState->setPixmap(QPixmap(ProjectTheme::Resource::ICON_NO_WIND).scaledToWidth( 60, Qt::SmoothTransformation));


  QHBoxLayout *midLayout = new QHBoxLayout;
  //midLayout->addSpacing(50);

//  midLayout->addSpacing(20);
  midLayout->setSpacing(10);
  midLayout->addSpacing(soundButton->width() + voiceSlider->width() + addMusicButton->width() + 50);

  //  midLayout->addStretch();
  midLayout->addWidget(preButton/*, Qt::AlignCenter*/);
//  midLayout->addSpacing(8);
  midLayout->addWidget(playButton/*, Qt::AlignCenter*/);
//  midLayout->addSpacing(8);
  midLayout->addWidget(nextButton/*, Qt::AlignCenter*/);
//  midLayout->addStretch();
  midLayout->addSpacing(50);
  midLayout->addWidget(soundButton);
//  midLayout->addSpacing(4);
  midLayout->addWidget(voiceSlider);
//  midLayout->addSpacing(20);
  midLayout->addWidget(addMusicButton);
  midLayout->setMargin(0);


  //play mode part
  playmode = Sequential;
//  modeLabel = new MyLabel;
//  modeLabel->setText("Sequential playback");
//  modeLabel->setAlignment(Qt::AlignCenter);
//  modeLabel->setFixedWidth(120);
  //  modeLabel->setPalette(palette);
  modeButton = new IconButton(ProjectTheme::Resource::ICON_PLAYBACK_SEQUENCE);
  modeButton->setFixedSize(60, 60);

  MyMenu *menu = new MyMenu(this);
//  QAction *onemusic = new QAction("Single Play", menu);
  QAction *onerep = new QAction("Single Cycle", menu);
  QAction *turnmusic = new QAction("Sequential", menu);
  QAction *allrep = new QAction("Loop", menu);
  QAction *randmusic = new QAction("Shuffle", menu);
//  connect(onemusic, SIGNAL(triggered()), this, SLOT(setModeonemusic()));
  connect(onerep, SIGNAL(triggered(bool)), this, SLOT(setModeonerep()));
  connect(turnmusic, SIGNAL(triggered(bool)), this, SLOT(setModeturnmusic()));
  connect(allrep, SIGNAL(triggered(bool)), this, SLOT(setModeallrep()));
  connect(randmusic, SIGNAL(triggered(bool)), this, SLOT(setModerandmusic()));
//  menu->addAction(onemusic);
  menu->addAction(onerep);
  menu->addAction(turnmusic);
  menu->addAction(allrep);
  menu->addAction(randmusic);
  connect(modeButton, SIGNAL(clicked()), menu, SLOT(menuVisiable()));

//  QVBoxLayout *rightLayout = new QVBoxLayout;
//  rightLayout->addWidget(modeLabel);
//  rightLayout->addWidget(modeButton);
//  rightLayout->setMargin(2);

  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->setSpacing(6);
  //  layout->addLayout(leftLayout);
  bottomLayout->addWidget(dayNightState);
  bottomLayout->addWidget(noWindWindState);
  bottomLayout->addStretch();
  bottomLayout->addLayout(midLayout);
  bottomLayout->addStretch();
  bottomLayout->addSpacing(60);
  bottomLayout->addWidget(modeButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
//  mainLayout->addWidget(musicSlider);
  mainLayout->addLayout(musicSliderTimeLayout);
  mainLayout->addSpacing(2);
//  mainLayout->addWidget(fountainSlider);
  mainLayout->addLayout(fountainSliderTimeLayout);
  mainLayout->addSpacing(5);
  mainLayout->addLayout(bottomLayout);
  mainLayout->setMargin(0);
  mainLayout->setSpacing(0);


  setLayout(mainLayout);

  dealMouse();
}

void BottomBar::clickCover()
{
  emit showLyric("lyric");
}

void BottomBar::setPostionAvailable()
{
  this->musicSlider->setAvailable(true);
}

void BottomBar::setPostionNotAvailable()
{
  this->musicSlider->setAvailable(false);
}

void BottomBar::onSetMaxMusicDuration(qint64 max)
{
  maxMusicDuration = max;
  QString str =
      QString("%1:%2").arg(
        QString::number((int)((maxMusicDuration/1000) / 60), 10), 2, QChar('0')).arg(
        QString::number((int)((maxMusicDuration/1000) % 60), 10), 2, QChar('0'));
  musicTotalTimeLabel->setText(str);
}

void BottomBar::onSetMaxFountainDuration(qint64 max)
{
  maxFountainDuration = max;
  QString str =
      QString("%1:%2").arg(
        QString::number((int)((maxFountainDuration/1000) / 60), 10), 2, QChar('0')).arg(
        QString::number((int)((maxFountainDuration/1000) % 60), 10), 2, QChar('0'));
  fountainTotalTimeLabel->setText(str);
}

void BottomBar::setMusicTitle(QString name)
{
  //todo
//  this->musicNameLabel->setText(name);
}

void BottomBar::setFountainTitle(QString name)
{
  //todo
  //  this->fountainNameLabel->setText(name);
}

void BottomBar::setNoMusic()
{
  musicSlider->moveTo(0);
  musicTotalTimeLabel->setText("--:--");
  musicElapsedTimeLabel->setText("--:--");
}

void BottomBar::setNoFountain()
{
  fountainSlider->moveTo(0);
  fountainTotalTimeLabel->setText("--:--");
  fountainElapsedTimeLabel->setText("--:--");
}

int BottomBar::getPlayMode()
{
  return playmode;
}

void BottomBar::setPlayMode(int mode)
{
  playmode = mode;
  switch (mode)
    {
//    case CurrentItemOnce:
//      setModeonemusic();
//      break;
    case CurrentItemInLoop:
      setModeonerep();
      break;
    case Sequential:
      setModeturnmusic();
      break;
    case Loop:
      setModeallrep();
      break;
    case Random:
      setModerandmusic();
      break;
    }
}

int BottomBar::getSoundLevel()
{
  return (int)(voiceSlider->getCurrentPostion()*100);
}

void BottomBar::setSoundLevel(int sound)
{
  voiceChanged((qreal)sound / 100);
  this->voiceSlider->moveTo((qreal)sound / 100);
}

void BottomBar::setPlay()
{
  //    playButton->setTheIcon(":/res/icons/playbutton_icon");
  playButton->setIcon(QIcon(":/res/icons/playbutton_icon"));
}

void BottomBar::setPause()
{
  //    playButton->setTheIcon(":/res/icons/pausebutton_icon");
  playButton->setIcon(QIcon(":/res/icons/pausebutton_icon"));
}

void BottomBar::postionChanged(qreal pos)
{
  int tmp = (int)(pos * maxMusicDuration / 1000);
  int min = tmp / 60;
  int sec = tmp % 60;
  QString str =
      QString("%1:%2").arg(
        QString::number(min, 10), 2, QChar('0')).arg(
        QString::number(sec, 10), 2, QChar('0'));
  musicElapsedTimeLabel->setText(str);
  emit setPostion((qint64)(pos * maxMusicDuration));
}

void BottomBar::onUpdateMusicPosition(qint64 pos)
{
  int min = (int)((pos / 1000) / 60);
  int sec = (int)((pos / 1000) % 60);
  QString str =
      QString("%1:%2").arg(
        QString::number(min, 10), 2, QChar('0')).arg(
        QString::number(sec, 10), 2, QChar('0'));
  musicElapsedTimeLabel->setText(str);
  musicSlider->moveTo(((qreal)pos / maxMusicDuration));//(qreal)pos
}

void BottomBar::onUpdateFountainPosition(qint64 pos)
{
  int min = (int)((pos / 1000) / 60);
  int sec = (int)((pos / 1000) % 60);
  QString str =
      QString("%1:%2").arg(
        QString::number(min, 10), 2, QChar('0')).arg(
        QString::number(sec, 10), 2, QChar('0'));
  fountainElapsedTimeLabel->setText(str);
  fountainSlider->moveTo(((qreal)pos / maxFountainDuration));//(qreal)pos
}

void BottomBar::voiceChanged(qreal pos)
{
  nosound = false;
  //    soundButton->setTheIcon(":/res/icons/soundbutton_icon");
  soundButton->setIcon(QIcon(":/res/icons/soundbutton_icon"));
  emit setVoice((int)(pos*100));
}

void BottomBar::clickSoundButton()
{
  if (nosound)
    {
      nosound = false;
      //        soundButton->setTheIcon(":/res/icons/soundbutton_icon");
      soundButton->setIcon(QIcon(":/res/icons/soundbutton_icon"));
      emit setVoice((int)(voiceSlider->getCurrentPostion()*100));
    }
  else
    {
      nosound = true;
      //        soundButton->setTheIcon(":/res/icons/nosoundbutton_icon");
      soundButton->setIcon(QIcon(":/res/icons/nosoundbutton_icon"));
      emit setVoice(0);
    }
}

//void BottomBar::setModeonemusic()
//{
//  playmode = CurrentItemOnce;
//  modeLabel->setText("Single play");
//  modeButton->setTheIcon(":/res/icons/onemusic_icon");
//  emit setMode(CurrentItemOnce);
//}

void BottomBar::setModeonerep()
{
  playmode = CurrentItemInLoop;
//  modeLabel->setText("Single cycle");
  modeButton->setTheIcon(ProjectTheme::Resource::ICON_PLAYBACK_LOOP_ONE);
  emit setMode(CurrentItemInLoop);
}

void BottomBar::setModeturnmusic()
{
  playmode = Sequential;
//  modeLabel->setText("Sequential playback");
  modeButton->setTheIcon(ProjectTheme::Resource::ICON_PLAYBACK_SEQUENCE);
  emit setMode(Sequential);
}

void BottomBar::setModeallrep()
{
  playmode = Loop;
//  modeLabel->setText("Loop");
  modeButton->setTheIcon(ProjectTheme::Resource::ICON_PLAYBACK_LOOP);
  emit setMode(Loop);
}

void BottomBar::setModerandmusic()
{
  playmode = Random;
//  modeLabel->setText("Shuffle Playback");
  modeButton->setTheIcon(ProjectTheme::Resource::ICON_PLAYBACK_SHUFFLE);
  emit setMode(Random);
}

void BottomBar::paintEvent(QPaintEvent *event)
{

  QWidget::paintEvent(event);
  //  QPainter painter(this);

  //  QBrush brush(/*QColor(0, 0, 0, 50)*/ProjectTheme::Colors::PRIMARY_LIGHT);
  //  painter.setBrush(brush);
  //  painter.setPen(Qt::NoPen);
  //  painter.drawRect(rect());


}

void BottomBar::onReceiveInputUpdate(const DigialInputResponse &response)
{

  if (!response.getResponse(AddressBook::INPUT_DAY_NIGHT_STATUS))
    {
      dayNightState->setPixmap(QPixmap(ProjectTheme::Resource::ICON_ONLY_DAY).scaledToWidth( 60, Qt::SmoothTransformation));
    }
  else
    {
      dayNightState->setPixmap(QPixmap(ProjectTheme::Resource::ICON_ONLY_NIGHT).scaledToWidth( 60, Qt::SmoothTransformation));
    }

  if (!response.getResponse(AddressBook::INPUT_WIND_STATUS))
    {
      noWindWindState->setPixmap(QPixmap(ProjectTheme::Resource::ICON_NO_WIND).scaledToWidth( 60, Qt::SmoothTransformation));
    }
  else
    {
      noWindWindState->setPixmap(QPixmap(ProjectTheme::Resource::ICON_WIND).scaledToWidth( 60, Qt::SmoothTransformation));
    }
}

void BottomBar::dealMouse()
{
//  connect(modeLabel, SIGNAL(mouseEnter()), this, SIGNAL(mouseEnter()));
//  connect(musicNameLabel, SIGNAL(mouseEnter()), this, SIGNAL(mouseEnter()));
//  connect(musicTimeLabel, SIGNAL(mouseEnter()), this, SIGNAL(mouseEnter()));
}

MyLabel::MyLabel(QWidget *parent)
  : QLabel(parent)
{
}

void MyLabel::enterEvent(QEvent *e)//鼠标进入事件
{
  emit mouseEnter();
}
