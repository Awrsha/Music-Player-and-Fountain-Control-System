#ifndef MUSICINFO_H
#define MUSICINFO_H

#include <QString>


class MediaInfo
{
    public:
        MediaInfo();
        void setDir(QString dirStr);
        void setName(QString nameStr) {name = nameStr;}
        void setArtist(QString artistStr) {artist = artistStr;}
        QString getDir() {return dir;}
        QString getName() {return name;}
        QString getArtist() {return artist;}

    private:
        QString dir;
        QString name;
        QString artist;
};


class MusicFountainInfo
{
public:
  MediaInfo musicInfo;
  MediaInfo fountainInfo;
};

#endif // MUSICINFO_H
