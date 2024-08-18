#include "qiconbutton.h"

QIconButton::QIconButton(QWidget *parent):
  QPushButton(parent)
{
  setAttribute(Qt::WA_TranslucentBackground);
}

QIconButton::QIconButton(const QString &iconDir, const QSize &iconSize, QWidget *parent):
  QPushButton(parent)
{
  setAttribute(Qt::WA_TranslucentBackground);
  setStyleSheet("background-color: #00000000;"
                "border-width: 0");

  setIcon(QIcon(iconDir));
  setIconSize(iconSize);
}
