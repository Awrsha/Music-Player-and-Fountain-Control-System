#ifndef MUSICINFO_H
#define MUSICINFO_H

#include <QString>
#include <QUrl>


class MediaInfo
{
    public:
        MediaInfo();
        void setDir(QString dirStr);
        void setName(QString nameStr) {name = nameStr;}
        void setArtist(QString artistStr) {artist = artistStr;}
        QString getDir() {return dir;}
        QString getName() const{return name;}
        QString getArtist() {return artist;}

    private:
        QString dir;
        QString name;
        QString artist;
};


class MusicFountainInfo
{
public:

  enum WorkMode {
    FIRST = 0, SECOND = 1, BOTH = 2
  };

  QUrl getMusicUrl()
  {
    return QUrl::fromLocalFile(musicInfo.getDir());
  }

  QUrl getFountainUrl()
  {
    return QUrl::fromLocalFile(fountainInfo.getDir());
  }

  bool doesMeetState(WorkMode dayNightMode, WorkMode noWindWindMode) const
  {
    return ( (this->dayNightMode == BOTH   || this->dayNightMode == dayNightMode) &&
             (this->noWindWindMode == BOTH || this->noWindWindMode == noWindWindMode));
  }


  MediaInfo musicInfo;
  MediaInfo fountainInfo;

  WorkMode dayNightMode;
  WorkMode noWindWindMode;

  MusicFountainInfo::WorkMode getDayNightMode() const;
  void setDayNightMode(const WorkMode &value);
  MusicFountainInfo::WorkMode getNoWindWindMode() const;
  void setNoWindWindMode(const WorkMode &value);
};

#endif // MUSICINFO_H
