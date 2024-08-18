#ifndef DMXINTERFACE_H
#define DMXINTERFACE_H

#include <QObject>
#include <QMutex>
#include <QVector>

#include "src/flags/projectflags.h"

class DmxInterface : public QObject
{
  Q_OBJECT
public:
  static void sendDmxData(const QVector<quint8>& dmxFrame);
  static void init();
  static void finalize();
  static const quint16 DMX_FRAME_COUNT;

signals:

public slots:

private:
  explicit DmxInterface(QObject *parent = 0);
  static int fileDescriptor;
  static bool initilized;

};

#endif // DMXINTERFACE_H





