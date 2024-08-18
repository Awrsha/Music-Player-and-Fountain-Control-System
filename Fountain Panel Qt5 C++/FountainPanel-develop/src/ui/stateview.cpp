#include "stateview.h"
#include "src/util/projecttheme.h"
#include <QPainter>

StateView::StateView(QWidget *parent) : QWidget(parent), state(false)
{
  ok.load(ProjectTheme::Resource::ICON_CHECKED);
  cancel.load(ProjectTheme::Resource::ICON_ERROR);
}

bool StateView::getState() const
{
  return state;
}

void StateView::setState(bool value)
{
  if (state != value)
    {
      state = value;
      update();
    }
}

void StateView::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  painter.setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform |
                         QPainter::HighQualityAntialiasing);
  if (state)
    {
      painter.drawPixmap(this->rect().topLeft(), ok.scaled(rect().width(), rect().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  else
    {
      painter.drawPixmap(this->rect().topLeft(), cancel.scaled(rect().width(), rect().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
