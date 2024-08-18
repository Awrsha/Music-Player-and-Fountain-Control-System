#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QListWidget>
#include <QBrush>

class MusicFountainList : public QListWidget
{
    Q_OBJECT

public:
    MusicFountainList(QWidget *parent = 0);
    ~MusicFountainList();
    void setHighLight(int row);
    void setArtist(int index, QString artist);

public slots:
    void removeHighLight();

private slots:
    void playTheMusic();
    void removeTheMusic();
    void removeAllMusic();
    void doubleClickedEvent(QModelIndex index);

signals:
    void mouseEnter();
    void rightClicked();
    void playTheMusic(int index);
    void removeTheMusic(int index);
    void removeAllMusics();
    void moveMusic(int from, int to);

private:
    void enterEvent(QEvent *e);//鼠标进入事件
    void contextMenuEvent(QContextMenuEvent *event);//右击事件
    void dropEvent(QDropEvent *event);
    int highLightRow = -1;
};

class MusicFountainListItem : public QWidget
{
    Q_OBJECT

public:
    MusicFountainListItem(QWidget *parent = 0);
    void setName(QString name);
    void setArtist(QString artist);
    void setHighLight();
    QString getName();
    QString getArtist();
    void removeHighLight();

    QString getFountainName() const;
    void setFountainName(const QString &value);

private:
    void paintEvent(QPaintEvent *e);

private:
    bool isHighLight;
    QString name;
    QString artist;
    QString fountainName;
};

#endif // MUSICLIST_H
