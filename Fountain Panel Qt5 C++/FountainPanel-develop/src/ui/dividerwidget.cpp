#include "dividerwidget.h"
#include <QPainter>
#include "src/util/projecttheme.h"
#include <QIcon>

DividerWidget :: DividerWidget(QWidget * parent) : QWidget(parent/*, Qt::Window|Qt::FramelessWindowHint*/)
{
  setAttribute(Qt::WA_TranslucentBackground);
}

void DividerWidget :: paintEvent(QPaintEvent * /*e*/)
{
  QPainter painter(this);

//  painter.setRenderHints(QPainter::Antialiasing |
//                         QPainter::SmoothPixmapTransform |
//                         QPainter::HighQualityAntialiasing);




  QPoint left = rect().topLeft();
  left.setY(rect().center().y());
  QPoint right = rect().topRight();
  right.setY(rect().center().y());

  QPen pen;
  pen.setCosmetic(true);
  pen.setColor(ProjectTheme::Colors::ACCENT_COLD_LIGHT);
  painter.setPen(pen);
  painter.drawLine(left, right);

}
