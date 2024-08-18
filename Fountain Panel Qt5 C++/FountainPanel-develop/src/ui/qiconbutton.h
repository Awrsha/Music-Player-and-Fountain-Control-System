#ifndef QICONBUTTON_H
#define QICONBUTTON_H

#include <QPushButton>


class QIconButton : public QPushButton
{
public:
  QIconButton();

  Q_OBJECT
public:
  QIconButton(QWidget *parent = 0);
  explicit QIconButton(const QString &iconDir, const QSize &iconSize, QWidget *parent = 0);
};

#endif // QICONBUTTON_H
