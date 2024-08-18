#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QWidget>
#include <QLabel>
#include "QPaintEvent"

#include "qiconbutton.h"
#include "src/core/digitaliointerfaceclient.h"

class SliderWidget;
class VoiceSlider;
class IconButton;
class MyLabel;


class BottomBar : public QWidget, public DigitalIOInterfaceClient
{
    Q_OBJECT
public:
    explicit BottomBar(QWidget *parent = 0);
    int getPlayMode();
    int getSoundLevel();
    void setSoundLevel(int sound);
    void setMusicTitle(QString name);
    void setFountainTitle(QString name);

    void setNoMusic();
    void setNoFountain();


public slots:
    void setPlayMode(int mode);
    void setPostionAvailable();
    void setPostionNotAvailable();
    void onSetMaxMusicDuration(qint64 max);
    void onSetMaxFountainDuration(qint64 max);

    void onUpdateMusicPosition(qint64 pos);
    void onUpdateFountainPosition(qint64 pos);

    void setPlay();
    void setPause();

protected:
    void dealMouse();

signals:
    void mouseEnter();
    void showLyric(QString name);
    void setPostion(qint64 pos);//
    void tryplay();
    void pause();
    void playPre();
    void playNext();
    void addMusicClicked();

    void setVoice(int voice);
    void setMode(int mode);

private slots:
    void clickCover();
    void postionChanged(qreal pos);
    void voiceChanged(qreal pos);
    void clickSoundButton();
//    void setModeonemusic();
    void setModeonerep();
    void setModeturnmusic();
    void setModeallrep();
    void setModerandmusic();

private:
    SliderWidget *musicSlider;
    SliderWidget *fountainSlider;

//    IconButton *coverButton;
//    MyLabel *musicNameLabel;
//    MyLabel *musicTimeLabel;
    MyLabel *musicTotalTimeLabel;
    MyLabel *musicElapsedTimeLabel;


//    MyLabel *fountainNameLabel;
//    MyLabel *fountainTimeLabel;
    MyLabel *fountainTotalTimeLabel;
    MyLabel *fountainElapsedTimeLabel;

    qint64 maxMusicDuration;
    qint64 maxFountainDuration;

    /*IconButton*/QIconButton *preButton;
    /*IconButton*/QIconButton *playButton;
    /*IconButton*/QIconButton *nextButton;

    bool nosound;
    /*IconButton*/QIconButton *soundButton;
    VoiceSlider *voiceSlider;
    QIconButton *addMusicButton;

    QLabel *dayNightState;
    QLabel *noWindWindState;

    int playmode;
//    MyLabel *modeLabel;
    IconButton *modeButton;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);

    // DigitalIOInterfaceClient interface
public slots:
    virtual void onReceiveInputUpdate(const DigialInputResponse &response);
};

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    MyLabel(QWidget *parent = 0);

signals:
    void mouseEnter();

private:
    void enterEvent(QEvent *e);
};

#endif // BOTTOMBAR_H
