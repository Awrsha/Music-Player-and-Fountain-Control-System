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

MusicFountainInfo::WorkMode MusicFountainInfo::getNoWindWindMode() const
{
    return noWindWindMode;
}

void MusicFountainInfo::setNoWindWindMode(const WorkMode &value)
{
    noWindWindMode = value;
}

MusicFountainInfo::WorkMode MusicFountainInfo::getDayNightMode() const
{
    return dayNightMode;
}

void MusicFountainInfo::setDayNightMode(const WorkMode &value)
{
    dayNightMode = value;
}
