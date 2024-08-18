#ifndef STATEVIEW_H
#define STATEVIEW_H

#include <QWidget>
#include <QPaintEvent>

class StateView : public QWidget
{
  Q_OBJECT
public:
  explicit StateView(QWidget *parent = 0);

  bool getState() const;
  void setState(bool value);

signals:

public slots:


private:

  bool state;
  QPixmap ok;
  QPixmap cancel;


  // QWidget interface
protected:
  virtual void paintEvent(QPaintEvent *event);
};

#endif // STATEVIEW_H
