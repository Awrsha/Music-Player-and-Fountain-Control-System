#ifndef IMAGEFRAME_H
#define IMAGEFRAME_H

#include <QFrame>
#include <QPixmap>

class ImageFrame : public QFrame
{
public:
   ImageFrame(QWidget * parent);

   virtual void paintEvent(QPaintEvent * e);

private:
   QPixmap pixmap;
};

#endif // IMAGEFRAME_H
