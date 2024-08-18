#include "musicpage.h"
#include "listlist.h"
#include "musicfountainlist.h"
#include "src/core/mediainfo.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSplitter>
#include <QStackedWidget>
#include <QFileDialog>
#include <QStackedWidget>

#include <QtDebug>
#include "qiconbutton.h"
#include "src/core/data.h"

MusicPage::MusicPage(QWidget *parent) : QWidget(parent)
{
  this->setAttribute(Qt::WA_TranslucentBackground, true);

//  QHBoxLayout *controlLayout = new QHBoxLayout;
//  addButton = new QPushButton("Add to");
//  addButton->setFixedHeight(60);
//  controlLayout->addWidget(addButton);
//  controlLayout->setSpacing(1);
//  controlLayout->setMargin(0);
//  connect(addButton, SIGNAL(clicked(bool)), this, SIGNAL(addMusics()));

//  QWidget *controlBar = new QWidget;
  //    controlBar->setStyleSheet("background:rgba(244,244,244,40%);");
  //controlBar->setAttribute(Qt::WA_TranslucentBackground, true);
//  controlBar->setFixedHeight(60);
//  controlBar->setLayout(controlLayout);

  QIconButton *addList = new QIconButton(":/res/icons/add_list_icon", QSize(32, 32));
  //    addList->setStyleSheet("background:rgba(244,244,244,40%);");


  listList = new ListList;
  connect(addList, SIGNAL(clicked(bool)), listList, SLOT(wantToCreateList()));
  connect(listList, SIGNAL(tryToCreateList(QString)), this, SIGNAL(tryToCreateList(QString)));
  connect(this, SIGNAL(createList(QString)), listList, SLOT(createList(QString)));
  connect(this, SIGNAL(createList(QString)), this, SLOT(createList()));
  connect(listList, SIGNAL(changeList(int)), this, SLOT(changeMusicList(int)));
  connect(listList, SIGNAL(deleteList(QString)), this, SIGNAL(deleteList(QString)));
  connect(listList, SIGNAL(deleteList(int)), this, SLOT(deleteList(int)));

  QVBoxLayout *leftLayout = new QVBoxLayout;
  leftLayout->addWidget(addList);
  leftLayout->addWidget(listList);
  leftLayout->setSpacing(1);
  leftLayout->setMargin(0);

  QWidget *leftWidget = new QWidget;
  leftWidget->setLayout(leftLayout);
  //leftWidget->setFixedWidth(80);
  leftWidget->setMinimumWidth(80);
  leftWidget->setMaximumWidth(200);

  musicLists = new QStackedWidget;
  musicLists->setMinimumWidth(80);
  //    musicLists->setMaximumWidth(420);


  QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
  splitter->setOpaqueResize(true);
  splitter->setChildrenCollapsible(false);
  splitter->addWidget(leftWidget);
  splitter->addWidget(musicLists);
  splitter->setStretchFactor(0, 2);
  splitter->setStretchFactor(1, 5);
  //    splitter->setStyleSheet("QSplitter::handle { background-color: rgba(0,0,0,0%) }");
  splitter->setHandleWidth(1);
  QVBoxLayout *mainLayout = new QVBoxLayout;
//  mainLayout->addWidget(controlBar);
  //mainLayout->addLayout(midLayout);
  mainLayout->addWidget(splitter);
  mainLayout->setSpacing(1);
  mainLayout->setMargin(1);

  this->setLayout(mainLayout);
}

//void MusicPage::addMusic(const QString &name, const QString &artist, const QString &fountainName)
//{
//  MusicFountainList *musicList = (MusicFountainList *)musicLists->currentWidget();
//  int row = musicList->count();

//  QListWidgetItem *item = new QListWidgetItem(musicList);
//  item->setSizeHint(QSize(item->sizeHint().width(), 30));
//  musicList->insertItem(row, item);
//  MusicFountainListItem *itemwidget = new MusicFountainListItem(musicList);
//  itemwidget->setName(name);
//  itemwidget->setArtist(artist);
//  itemwidget->setFountainName(fountainName);
//  musicList->setItemWidget(item, itemwidget);
//  musicList->setCurrentRow(row);
  /*
    musicList->insertRow(row);
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    QTableWidgetItem *artiseItem = new QTableWidgetItem(artist);

    nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    artiseItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    musicList->setItem(row, 0, nameItem);
    musicList->setItem(row, 1, artiseItem);

    musicList->setCurrentCell(row, 0);
    */
//}

void MusicPage::addMusic(int listId, MusicFountainInfo musicFountainInfo)
{
  MusicFountainList *musicList = (MusicFountainList *)musicLists->widget(listId);
  int row = musicList->count();

  QListWidgetItem *item = new QListWidgetItem(musicList);
  item->setSizeHint(QSize(item->sizeHint().width(), 30));
  musicList->insertItem(row, item);
  MusicFountainListItem *itemwidget = new MusicFountainListItem(musicList);
  itemwidget->setMusicFountainInfo(musicFountainInfo);
  //  itemwidget->setName(name);
  //  itemwidget->setArtist(artist);
  //  itemwidget->setFountainName(fountainName);
  musicList->setItemWidget(item, itemwidget);
  musicList->setCurrentRow(row);

}

//void MusicPage::addMusics(QStringList musicsDir, QStringList fountainsDir)
//{
////    QStringList::iterator it;
////    for (it = fountainsDir.begin(); it != fountainsDir.end(); ++it)
////        this->addMusic(QFileInfo(*it).baseName(), "");

//  for (int index = 0; index < musicsDir.size(); ++index) {
//      this->addMusic(musicsDir.at(index), "", fountainsDir.at(index));
//    }
//}

void MusicPage::addMusics(int listId, QQueue<MusicFountainInfo> musicFountainInfoQueue)
{
  MusicFountainInfo musicFountainInfo;
  MusicFountainList *musicFountainList;
  int row;
  while (!musicFountainInfoQueue.empty())
    {
      musicFountainInfo = musicFountainInfoQueue.front();
      musicFountainInfoQueue.pop_front();
//      musicFountainList = (MusicFountainList *)musicLists->widget(listId);
//      row = musicFountainList->count();
//      QListWidgetItem *item = new QListWidgetItem(musicFountainList);
//      item->setSizeHint(QSize(item->sizeHint().width(), 30));
//      musicFountainList->insertItem(row, item);
//      MusicFountainListItem *itemwidget = new MusicFountainListItem(musicFountainList);
//      itemwidget->setName(musicFountainInfo.musicInfo.getName());
//      itemwidget->setArtist(musicFountainInfo.musicInfo.getArtist());
//      itemwidget->setFountainName(musicFountainInfo.fountainInfo.getName());
//      musicFountainList->setItemWidget(item, itemwidget);

      addMusic(listId, musicFountainInfo);
      /*
        musicList->insertRow(row);
        QTableWidgetItem *nameItem = new QTableWidgetItem(musicInfo.getName());
        QTableWidgetItem *artiseItem = new QTableWidgetItem(musicInfo.getArtist());

        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        artiseItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        musicList->setItem(row, 0, nameItem);
        musicList->setItem(row, 1, artiseItem);

        musicList->setCurrentCell(row, 0);
        */
    }
}

void MusicPage::addMusic(MusicFountainInfo musicFountainInfo)
{
//  this->addMusic(musicFountainInfo.musicInfo.getName(),
//                 musicFountainInfo.musicInfo.getArtist(),
//                 musicFountainInfo.fountainInfo.getName());

  addMusic(musicLists->currentIndex(), musicFountainInfo);
}

void MusicPage::clearMusic()
{
  /*
    int row = musicList->rowCount()-1;
    while (row >= 0)
    {
        for (itn i = 0; i < 2; i++)
        {
            QTableWidgetItem *item = musicList->takeItem(row, i);
            delete item;
            item = Q_NULLPTR;
        }
        //虽然removeRow(int row)中的Item会被Qt自动删除,
        //但总觉得自己删除会好一些
        musicList->removeRow(row);
        row--;
    }*/
}

void MusicPage::play(int index)
{
  emit play(listList->getHighLightRowName(), index);
}

void MusicPage::removeHighLight()
{
  emit hightLightChanged();
}

void MusicPage::setHighLight(int list, int index)
{
  MusicFountainList *musicList = (MusicFountainList *)musicLists->widget(list);
  musicList->setHighLight(index);
}

void MusicPage::setArtist(int list, int index, QString artist)
{
  MusicFountainList *musicList = (MusicFountainList *)musicLists->widget(list);
  musicList->setArtist(index, artist);
}

void MusicPage::setCurrentList(int index)
{
//  listList->setHighLight(index);
  changeMusicList(index);
}

QString MusicPage::getCurrentList()
{
  return listList->getHighLightRowName();
}

void MusicPage::changeMusicList(int index)
{
  this->musicLists->setCurrentIndex(index);
  Data::changeLastListIndex(index);
  listList->setHighLight(index);
  emit changeMusicList(listList->item(index, 0)->text());
}

void MusicPage::createList()
{
  MusicFountainList *musicList = new MusicFountainList;
  musicLists->addWidget(musicList);
  connect(musicList, SIGNAL(playTheMusic(int)), this, SLOT(play(int)));
  connect(musicList, SIGNAL(removeTheMusic(int)), this, SLOT(removeTheMusic(int)));
  connect(musicList, SIGNAL(removeAllMusics()), this, SLOT(removeAllMusics()));
  connect(musicList, SIGNAL(moveMusic(int,int)), this, SLOT(moveMusic(int,int)));
  connect(this, SIGNAL(hightLightChanged()), musicList, SLOT(removeHighLight()));
}

void MusicPage::deleteList(int index)
{
  QWidget *item = musicLists->widget(index);
  musicLists->removeWidget(item);
  delete item;
}

void MusicPage::removeTheMusic(int index)
{
  emit removeTheMusic(this->getCurrentList(), index);
}

void MusicPage::removeAllMusics()
{
  emit removeAllMusics(this->getCurrentList());
}

void  MusicPage::moveMusic(int from, int to)
{
  emit moveMusic(this->getCurrentList(), from, to);
  //qDebug() << "move" << endl;
}

void MusicPage::enterEvent(QEvent *e)//鼠标进入事件
{
  emit mouseEnter();
}
