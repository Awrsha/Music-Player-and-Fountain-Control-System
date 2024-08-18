#include "mediainfo.h"
#include <QMediaPlayer>
#include <QMediaContent>
#include <QMediaMetaData>

MediaInfo::MediaInfo()
{

}

void MediaInfo::setDir(QString dirStr)
{
    this->dir = dirStr;
    //QMediaPlayer *player = new QMediaPlayer();
    //connect(player, SIGNAL(metaDataChanged())
    //player->setMedia(QMediaContent(QUrl(dirStr)));
    //this->artist = player->metaData(QMediaMetaData::Title).toString();
    //this->artist = player->metaData(QMediaMetaData::ContributingArtist).toString();
}
