#include "imageframe.h"
#include <QPainter>

#include "src/util/projecttheme.h"
#include <QDebug>
#include <QIcon>
ImageFrame :: ImageFrame(QWidget * parent) : QFrame(parent/*, Qt::Window|Qt::FramelessWindowHint*/)
{
  setAttribute(Qt::WA_TranslucentBackground);

  pixmap = (QIcon(ProjectTheme::Resource::ICON_OVAL).pixmap(512, 512));
}

void ImageFrame :: paintEvent(QPaintEvent * /*e*/)
{
  QPainter painter(this);
  //   p.drawPixmap(0,0,width(),height(), pixmap);

  painter.setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform |
                         QPainter::HighQualityAntialiasing);

  QPixmap scaledPixmap = pixmap.scaledToHeight(height() - 5, Qt::SmoothTransformation);

  painter.drawPixmap(rect().center().x() - scaledPixmap.width() / 2,
                     rect().center().y() - scaledPixmap.height() / 2,
                     scaledPixmap.width(),
                     scaledPixmap.height(),
                     scaledPixmap );

  foreach (QWidget* child, findChildren<QWidget*>()) {
      child->setAttribute(Qt::WA_TranslucentBackground);
    }
}
