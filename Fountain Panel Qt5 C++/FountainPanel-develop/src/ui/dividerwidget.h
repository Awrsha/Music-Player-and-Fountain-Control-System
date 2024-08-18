#ifndef DIVIDERWIDGET_H
#define DIVIDERWIDGET_H

#include <QWidget>
#include <QPixmap>

class DividerWidget : public QWidget
{
  Q_OBJECT
public:
  explicit DividerWidget(QWidget *parent = 0);


  virtual void paintEvent(QPaintEvent * e);
signals:

public slots:

private:
   QPixmap pixmap;
};

#endif // DIVIDERWIDGET_H
