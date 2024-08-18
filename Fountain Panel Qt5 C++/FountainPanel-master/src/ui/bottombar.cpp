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

BottomBar::BottomBar(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);//设置透明:窗体标题栏不透明,背景透明
    this->setFixedHeight(86);
    this->maxDuration = 0;

    /*
    QPalette palette;//调色板
    palette.setColor(QPalette::Background, QColor(18, 0, 169));//设置调色板参数为背景色，RGB颜色为深蓝色
    this->setAutoFillBackground(true);//将组件的背景设为可调
    this->setPalette(palette);//将调色板应用于组件
    */

    postionSlider = new SliderWidget;
    connect(postionSlider, SIGNAL(changeTo(qreal)), this, SLOT(postionChanged(qreal)));

    coverButton = new IconButton(":/res/icons/coverbutton_icon");
    connect(coverButton, SIGNAL(clicked()), this, SLOT(clickCover()));

    QPalette palette;//调色板
    palette.setColor(QPalette::WindowText, QColor(255, 255, 255));//设置调色板参数为按钮字体色，RGB颜色为白色
    nameLabel = new MyLabel;
    nameLabel->setText("The name of the song");
    nameLabel->setFixedWidth(150);
    nameLabel->setPalette(palette);

    timeLabel = new MyLabel;
    timeLabel->setText("00:00 | 00:00");
    timeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    timeLabel->setFixedWidth(150);
    timeLabel->setPalette(palette);

    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->addWidget(nameLabel);
    labelLayout->addWidget(timeLabel);

    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->addWidget(coverButton);
    leftLayout->addSpacing(8);
    leftLayout->addLayout(labelLayout);
    leftLayout->setMargin(0);

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

    QHBoxLayout *midLayout = new QHBoxLayout;
    //midLayout->addSpacing(50);
    midLayout->addWidget(preButton);
    midLayout->addSpacing(8);
    midLayout->addWidget(playButton, Qt::AlignHCenter);
    midLayout->addSpacing(8);
    midLayout->addWidget(nextButton);
    midLayout->addSpacing(20);
    midLayout->addWidget(soundButton);
    midLayout->addSpacing(4);
    midLayout->addWidget(voiceSlider);
    midLayout->setMargin(0);

    //play mode part
    playmode = Sequential;
    modeLabel = new MyLabel;
    modeLabel->setText("Sequential playback");
    modeLabel->setAlignment(Qt::AlignCenter);
    modeLabel->setFixedWidth(120);
    modeLabel->setPalette(palette);
    modeButton = new IconButton(":/res/icons/turnmusic_icon");
    MyMenu *menu = new MyMenu(this);
    QAction *onemusic = new QAction("Single play", menu);
    QAction *onerep = new QAction("Single cycle", menu);
    QAction *turnmusic = new QAction("Sequential playback", menu);
    QAction *allrep = new QAction("Loop", menu);
    QAction *randmusic = new QAction("Shuffle Playback", menu);
    connect(onemusic, SIGNAL(triggered()), this, SLOT(setModeonemusic()));
    connect(onerep, SIGNAL(triggered(bool)), this, SLOT(setModeonerep()));
    connect(turnmusic, SIGNAL(triggered(bool)), this, SLOT(setModeturnmusic()));
    connect(allrep, SIGNAL(triggered(bool)), this, SLOT(setModeallrep()));
    connect(randmusic, SIGNAL(triggered(bool)), this, SLOT(setModerandmusic()));
    menu->addAction(onemusic);
    menu->addAction(onerep);
    menu->addAction(turnmusic);
    menu->addAction(allrep);
    menu->addAction(randmusic);
    connect(modeButton, SIGNAL(clicked()), menu, SLOT(menuVisiable()));

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(modeLabel);
    rightLayout->addWidget(modeButton);
    rightLayout->setMargin(2);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addStretch();
    layout->addLayout(midLayout);
    layout->addStretch();
    layout->addLayout(rightLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(postionSlider);
    mainLayout->addSpacing(2);
    mainLayout->addLayout(layout);
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
    this->postionSlider->setAvailable(true);
}

void BottomBar::setPostionNotAvailable()
{
    this->postionSlider->setAvailable(false);
}

void BottomBar::setMaxDuration(qint64 max)
{
    maxDuration = max;
}

void BottomBar::setMusicTitle(QString name)
{
    this->nameLabel->setText(name);
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
        case CurrentItemOnce:
            setModeonemusic();
            break;
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
    int tmp = (int)(pos * maxDuration / 1000);
    int min = tmp / 60;
    int sec = tmp % 60;
    QString str =
        QString("%1:%2 | %3:%4").arg(
                QString::number(min, 10)).arg(
                QString::number(sec, 10)).arg(
                QString::number((int)((maxDuration/1000) / 60), 10)).arg(
                QString::number((int)((maxDuration/1000) % 60), 10));
    timeLabel->setText(str);
    emit setPostion((qint64)(pos * maxDuration));
}

void BottomBar::changePostionTo(qint64 pos)
{
    int min = (int)((pos / 1000) / 60);
    int sec = (int)((pos / 1000) % 60);
    QString str =
            QString("%1:%2 | %3:%4").arg(
                    QString::number(min, 10), 2, QChar('0')).arg(
                    QString::number(sec, 10), 2, QChar('0')).arg(
                    QString::number((int)((maxDuration/1000) / 60), 10), 2, QChar('0')).arg(
                    QString::number((int)((maxDuration/1000) % 60), 10), 2, QChar('0'));
    timeLabel->setText(str);
    postionSlider->moveTo(((qreal)pos / maxDuration));//(qreal)pos是必须的,否则进度条一直为0
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

void BottomBar::setModeonemusic()
{
    playmode = CurrentItemOnce;
    modeLabel->setText("Single play");
    modeButton->setTheIcon(":/res/icons/onemusic_icon");
    emit setMode(CurrentItemOnce);
}

void BottomBar::setModeonerep()
{
    playmode = CurrentItemInLoop;
    modeLabel->setText("Single cycle");
    modeButton->setTheIcon(":/res/icons/onerep_icon");
    emit setMode(CurrentItemInLoop);
}

void BottomBar::setModeturnmusic()
{
    playmode = Sequential;
    modeLabel->setText("Sequential playback");
    modeButton->setTheIcon(":/res/icons/turnmusic_icon");
    emit setMode(Sequential);
}

void BottomBar::setModeallrep()
{
    playmode = Loop;
    modeLabel->setText("Loop");
    modeButton->setTheIcon(":/res/icons/allrep_icon");
    emit setMode(Loop);
}

void BottomBar::setModerandmusic()
{
    playmode = Random;
    modeLabel->setText("Shuffle Playback");
    modeButton->setTheIcon(":/res/icons/randmusic_icon");
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

void BottomBar::dealMouse()
{
    connect(modeLabel, SIGNAL(mouseEnter()), this, SIGNAL(mouseEnter()));
    connect(nameLabel, SIGNAL(mouseEnter()), this, SIGNAL(mouseEnter()));
    connect(timeLabel, SIGNAL(mouseEnter()), this, SIGNAL(mouseEnter()));
}

MyLabel::MyLabel(QWidget *parent)
    : QLabel(parent)
{
}

void MyLabel::enterEvent(QEvent *e)//鼠标进入事件
{
    emit mouseEnter();
}
