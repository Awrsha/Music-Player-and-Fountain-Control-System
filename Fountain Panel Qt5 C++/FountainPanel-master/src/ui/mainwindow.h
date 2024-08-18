#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>

#include "src/core/mediainfo.h"


class MySystemTrayIcon;
class TopBar;
class QStackedWidget;
class LyricLabel;
class NetworkPage;
class MusicPage;
class BottomBar;
class MyPlayer;


namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

//public slots:
//    void setBackgroud(QString dir);
//    void aboutQtMusic();

private slots:
    void clickPlay();
    void playPre();
    void playNext();
    void playerStateChanged(int state);
    void musicChanged(QString listName, int index);
    void tryToCreateList(QString name);
    void addMusics();
    void addMusic(MusicFountainInfo musicInfo);
    void removeTheMusic(QString listName, int index);
    void removeAllMusics(QString listName);
    void moveMusic(QString listName, int from, int to);
    void deleteList(QString name);

private:
    void readData();
//    void paintEvent(QPaintEvent *);
//    void dealMouse();

private:
  Ui::MainWindow *ui;
  QPixmap currentBackground;
  MySystemTrayIcon *systemTrayIcon;
  TopBar *topBar;
  QStackedWidget *fuctionPage;
  LyricLabel *lyricLabel;
  NetworkPage *networkPage;
  MusicPage *musicPage;
  BottomBar *bottomBar;
  MyPlayer *player;
};

#endif // MAINWINDOW_H
