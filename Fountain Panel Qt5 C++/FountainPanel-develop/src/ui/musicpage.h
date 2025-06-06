#ifndef MUSICPAGE_H
#define MUSICPAGE_H

#include <QWidget>
#include <QQueue>
#include "src/core/mediainfo.h"

class ListList;
class QStackedWidget;
class QPushButton;

class MusicPage : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPage(QWidget *parent = 0);
    void setCurrentList(int index);
    QString getCurrentList();
//    void addMusics(QStringList musicsDir, QStringList fountainsDir);
    void addMusics(int listId, QQueue<MusicFountainInfo> musicFountainInfoQueue);
    void addMusic(MusicFountainInfo musicFountainInfo);
    void clearMusic();//
    void removeHighLight();
    void setHighLight(int list, int index);
    void setArtist(int list, int index, QString artist);

protected:
    void enterEvent(QEvent *e);//鼠标进入事件
//    void addMusic(const QString& name, const QString& artist, const QString& fountainName);
    void addMusic(int listId, MusicFountainInfo musicFountainInfo);

signals:
    void mouseEnter();
    void addMusics();
    void tryToCreateList(QString name);
    void createList(QString name);
    void deleteList(QString name);
    void changeMusicList(QString listName);
    void removeTheMusic(QString listName, int index);
    void removeAllMusics(QString listName);
    void play(QString name, int index);
    void hightLightChanged();
    void moveMusic(QString listName, int from, int to);

private slots:
    void changeMusicList(int index);
    void createList();
    void deleteList(int index);
    void removeTheMusic(int index);
    void removeAllMusics();
    void play(int index);
    void moveMusic(int from, int to);

private:
    QPushButton *addButton;
    ListList *listList;
    QStackedWidget *musicLists;
};

#endif // MUSICPAGE_H
